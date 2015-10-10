/*
This code is described in "Computational Geometry in C" (Second Edition),
Chapter 4.  It is not written to be comprehensible without the 
explanation in that book.

Written by Joseph O'Rourke, with contributions by 
  Kristy Anderson, John Kutcher, Catherine Schevon, Susan Weller.
Last modified: May 2000
Questions to orourke@cs.smith.edu.

--------------------------------------------------------------------
This code is Copyright 2000 by Joseph O'Rourke.  It may be freely 
redistributed in its entirety provided that this copyright notice is 
not removed.
--------------------------------------------------------------------
*/

#include "shape.h"

/* Global variable definitions */
tVertex vertices = NULL;
tEdge edges    	 = NULL;
tFace faces    	 = NULL;
tTetra tetras    = NULL;

/*---------------------------------------------------------------------
MakeNullVertex: Makes a vertex, nulls out fields.
---------------------------------------------------------------------*/
tVertex	MakeNullVertex( void )
{
   tVertex  v;
   NEW( v, tsVertex );
   ADD( vertices, v );
   v->vvlist=NULL;
   v->ispole=false;
   return v;
}

/*---------------------------------------------------------------------
ReadVertices: Reads in the vertices, and links them into a circular
list with MakeNullVertex.  There is no need for the # of vertices to be
the first line: the function looks for EOF instead.  Sets the global
variable vertices via the ADD macro.
---------------------------------------------------------------------*/
void	ReadVertices( char * filename )
{
   tVertex  v;
   int      x, y, z;
   int	    vnum = 0;
   
   FILE * fin=fopen(filename,"r");
   if(fin==NULL){
      fprintf(stderr,"! Error: Cannot open file (%s)\n",filename);
      exit(1);
   }

   while ( fscanf (fin,"%d %d %d", &x, &y, &z ) != EOF )  {
      v = MakeNullVertex();
      v->v[X] = x;
      v->v[Y] = y;
      v->v[Z] = z;
      v->vnum = vnum++;
      if ( ( abs(x) > SAFE ) || ( abs(y) > SAFE ) || ( abs(z) > SAFE ) ) {
         printf("Coordinate of vertex below might be too large: run with -d flag\n");
         PrintPoint(v);
      }
   }/*end while*/
   
   fclose(fin);
}


/*---------------------------------------------------------------------
VolumeSign returns the sign of the volume of the tetrahedron determined by f
and p.  VolumeSign is +1 iff p is on the negative side of f,
where the positive side is determined by the rh-rule.  So the volume 
is positive if the ccw normal to f points outside the tetrahedron.
The final fewer-multiplications form is due to Bob Williamson.
---------------------------------------------------------------------*/
int  VolumeSign( tFace f, tVertex p )
{
   double  vol;
   double  ax, ay, az, bx, by, bz, cx, cy, cz;

   ax = f->vertex[0]->v[X] - p->v[X];
   ay = f->vertex[0]->v[Y] - p->v[Y];
   az = f->vertex[0]->v[Z] - p->v[Z];
   bx = f->vertex[1]->v[X] - p->v[X];
   by = f->vertex[1]->v[Y] - p->v[Y];
   bz = f->vertex[1]->v[Z] - p->v[Z];
   cx = f->vertex[2]->v[X] - p->v[X];
   cy = f->vertex[2]->v[Y] - p->v[Y];
   cz = f->vertex[2]->v[Z] - p->v[Z];

   vol =   ax * (by*cz - bz*cy)
         + ay * (bz*cx - bx*cz)
         + az * (bx*cy - by*cx);

   /* The volume should be an integer. */
   if      ( vol >  0.5 )  return  1;
   else if ( vol < -0.5 )  return -1;
   else                    return  0;
}

/*---------------------------------------------------------------------
Same computation, but computes using ints, and returns the actual volume.
---------------------------------------------------------------------*/
double  Volumei( tFace f, tVertex p )
{
   double  vol;
   double  ax, ay, az, bx, by, bz, cx, cy, cz;

   ax = f->vertex[0]->v[X] - p->v[X];
   ay = f->vertex[0]->v[Y] - p->v[Y];
   az = f->vertex[0]->v[Z] - p->v[Z];
   bx = f->vertex[1]->v[X] - p->v[X];
   by = f->vertex[1]->v[Y] - p->v[Y];
   bz = f->vertex[1]->v[Z] - p->v[Z];
   cx = f->vertex[2]->v[X] - p->v[X];
   cy = f->vertex[2]->v[Y] - p->v[Y];
   cz = f->vertex[2]->v[Z] - p->v[Z];

   vol =  (ax * (by*cz - bz*cy)
         + ay * (bz*cx - bx*cz)
         + az * (bx*cy - by*cx));

   return vol;
}


/*-------------------------------------------------------------------*/
void	PrintPoint( tVertex p )
{
   int	i;

   for ( i = 0; i < 3; i++ )
      printf("\t%f", p->v[i]);
   putchar('\n');
}


/*---------------------------------------------------------------------
MakeNullEdge creates a new cell and initializes all pointers to NULL
and sets all flags to off.  It returns a pointer to the empty cell.
---------------------------------------------------------------------*/
tEdge 	MakeNullEdge( void )
{
   tEdge  e;

   NEW( e, tsEdge );
   e->adjface[0] = e->adjface[1] = NULL;
   e->endpts[0] = e->endpts[1] = NULL;
   ADD( edges, e );
   return e;
}

/*--------------------------------------------------------------------
MakeNullFace creates a new face structure and initializes all of its
flags to NULL and sets all the flags to off.  It returns a pointer
to the empty cell.
---------------------------------------------------------------------*/
tFace 	MakeNullFace( void )
{
   tFace  f;
   int    i;

   NEW( f, tsFace);
   for ( i=0; i < 3; ++i ) {
      f->edge[i] = NULL;
      f->vertex[i] = NULL;
   }
   ADD( faces, f );
   return f;
}

/*--------------------------------------------------------------------
MakeNullTetra creates a new tetrahedron structure and initializes all of its
flags to NULL and sets all the flags to off.  It returns a pointer
to the empty cell.
---------------------------------------------------------------------*/

tTetra  MakeNullTetra( void )
{
   int i;
   tTetra t;
   NEW( t, tsTetra);
   for ( i=0; i < 4; ++i ) {
      t->face[i] = NULL;
      t->vertex[i]=NULL;
   }
   ADD( tetras, t );
   return t;
}

/*---------------------------------------------------------------------
MakeFace creates a new face structure from three vertices (in ccw
order).  It returns a pointer to the face.
---------------------------------------------------------------------*/
tFace   MakeFace( tVertex v0, tVertex v1, tVertex v2, tFace fold )
{
   tFace  f;
   tEdge  e0, e1, e2;

   /* Create edges of the initial triangle. */
   if( !fold ) {
     e0 = MakeNullEdge();
     e1 = MakeNullEdge();
     e2 = MakeNullEdge();
   }
   else { /* Copy from fold, in reverse order. */
     e0 = fold->edge[2];
     e1 = fold->edge[1];
     e2 = fold->edge[0];
   }
   
   e0->endpts[0] = v0;              e0->endpts[1] = v1;
   e1->endpts[0] = v1;              e1->endpts[1] = v2;
   e2->endpts[0] = v2;              e2->endpts[1] = v0;
	
   /* Create face for triangle. */
   f = MakeNullFace();
   f->edge[0]   = e0;  f->edge[1]   = e1; f->edge[2]   = e2;
   f->vertex[0] = v0;  f->vertex[1] = v1; f->vertex[2] = v2;
	
   /* Link edges to face. */
   e0->adjface[0] = e1->adjface[0] = e2->adjface[0] = f;
	
   return f;
}


/*------------------------------------------------------------------
  EdgeOrderOnFaces: puts e0 between v0 and v1, e1 between v1 and v2,
  e2 between v2 and v0 on each face.  This should be unnecessary, alas.
  Not used in code, but useful for other purposes.
------------------------------------------------------------------*/
void    EdgeOrderOnFaces ( void ) 
{
  tFace f = faces;
  tEdge new;
  int i,j;

  do {
    for (i = 0; i < 3; i++) {
      if (!(((f->edge[i]->endpts[0] == f->vertex[i]) &&
             (f->edge[i]->endpts[1] == f->vertex[(i+1)%3])) ||
            ((f->edge[i]->endpts[1] == f->vertex[i]) &&
             (f->edge[i]->endpts[0] == f->vertex[(i+1)%3])))) {
        /* Change the order of the edges on the face: */
        for (j = 0; j < 3; j ++) {
          /* find the edge that should be there */
          if (((f->edge[j]->endpts[0] == f->vertex[i]) &&
               (f->edge[j]->endpts[1] == f->vertex[(i+1)%3])) ||
              ((f->edge[j]->endpts[1] == f->vertex[i]) &&
               (f->edge[j]->endpts[0] == f->vertex[(i+1)%3]))) {
            /* Swap it with the one erroneously put into its place: */
            new = f->edge[i];
            f->edge[i] = f->edge[j];
            f->edge[j] = new;
          }
        }
      }
    }
    f = f->next;
  } while (f != faces);

}
