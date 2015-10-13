
#include <limits.h>
#include <assert.h>
#include "draw.h"
#include "shape.h"
#include "delauany.h"
#include "crust.h"
#include "alpha-shape.h"

/*-------------------------------------------------------------------*/
// global data
char * filename=NULL;
bool draw3D  = true;
bool b_alphashape =false;  
unsigned int  alpha=UINT_MAX;
bool b_crust =false;  
bool b_delaunay = false;

/*-------------------------------------------------------------------*/
//defined in shape.h/.c
extern tVertex vertices;
extern tEdge edges;
extern tFace faces;
extern tTetra tetras;

/*-------------------------------------------------------------------*/
#include "qhull.h"
#include "poly.h"
#include "qset.h"

tEdge QHullCreateEdge(tVertex v1[3], tVertex v2[3])
{
    int i, j, in, jn;
    bool found = false;
	tEdge e=NULL;

    for(i=0;i<3;i++)
	{
        in=i+1;
        if(in==3) in=0;
        for(j=0;j<3;j++){
            jn=j+1;
            if(jn==3) jn=0;
            if(v1[i]==v2[j] && v1[in]==v2[jn]){ found=true; break; }
            if(v1[in]==v2[j] && v1[i]==v2[jn]){ found=true; break; }
        }

        if(found) break;
    }

	assert(found);

    e=MakeNullEdge();
    e->endpts[0]=v1[i];
    e->endpts[1]=v1[in];

    return e;
}

void QHull()
{
	tVertex  ptr_v;
	tVertex * all_v=NULL;
	int vsize=0;
	int id=0;
	
	//global varibles for qhull
	static char * options=(char *)"qhull QJ Pp";
    int curlong, totlong;
	coordT * pt=NULL;
    facetT *facet=NULL;
    vertexT *vertex=NULL;
    vertexT **vertexp=NULL;
    facetT *neighbor, **neighborp;
    int vid=0;
	tFace  face;

    //count number of points
    ptr_v=vertices;
	do {                                 
	   vsize++;
	   ptr_v = ptr_v->next;
	} while ( ptr_v != vertices );
    
    //allocate memory
    pt=(coordT*)calloc(vsize*3,sizeof(coordT)); //each point will have three coord
    all_v=(tVertex*)calloc(vsize,sizeof(tVertex));
    assert(pt && all_v);

    //copy points
    ptr_v=vertices;
	do {                                 
	   pt[id++]=ptr_v->v[0];
	   pt[id++]=ptr_v->v[1];
	   pt[id++]=ptr_v->v[2];
	   all_v[ptr_v->vnum]=ptr_v;
	   ptr_v = ptr_v->next;
	} while ( ptr_v != vertices );

    //using qhull

    qh_init_A(stdin, stdout, stderr, 0, NULL);

    //qh DELAUNAY= True;     /* 'd'   */
    //qh SCALElast= True;    /* 'Qbb' */
    //qh KEEPcoplanar= True; /* 'Qc', to keep coplanars in 'p' */

    qh_initflags(options);
    qh_init_B (pt, vsize, 3, false);
    qh_qhull();
    qh_check_output();

    //loop through all faces
    FORALLfacets 
	{    
        face = MakeNullFace(); //make a face

        //get vertices of facet
        //loop through each vertex
        vid=0;
        FOREACHvertex_(facet->vertices)
        {
            //get the id of the vertex
            face->vertex[vid++]=all_v[qh_pointid(vertex->point)];
        }

        FOREACHneighbor_(facet)
        {
            if(facet<neighbor){

                tVertex vertices[3];
                vid=0;
                FOREACHvertex_(neighbor->vertices)
                {
                    //get vertex
                    vertices[vid++]=all_v[qh_pointid(vertex->point)];
                }

                QHullCreateEdge(face->vertex, vertices);
            }
        }//FOREACHneighbor_

    }
    
    //not used
    free(pt); 
    free(all_v);
    pt=NULL;
    all_v=NULL;
    
    //free mem
    qh_freeqhull(!qh_ALL);
    qh_memfreeshort (&curlong, &totlong);
}


/*-------------------------------------------------------------------*/
void PrintUsage(int argc, char *argv[])
{
	 printf ("Usage:  %s -t[urn-off-GL] -s[hape] delaunay|crust|alpha data-file\n", *argv );
	 printf("Ex:  %s  -s delaunay data-file \n", *argv);
	 printf ("Ex:  %s  -s crust data-file (this generates crust boundary)\n", *argv );
	 printf ("Ex:  %s  -s alpha 3 data-file (this generates alpha shape with alpha=3)\n", *argv );
}

int ParseArg(int argc, char *argv[])
{
   
   int i=0;
   
   if ( argc > 1 ) {
	 
     for(i=1;i<argc;i++){
     
		 if( argv[i][1] == 't'){
		 	draw3D=false;
		 }
		 else if( argv[i][1] == 's'){
		 	
		 	if(i+1>=argc) return 1; //can't read the next value
		 	if(argv[i+1][0]=='a')
		 	{
			 	b_alphashape=true;
				if (i + 2 >= argc)
				{
					printf("! Error: No alpha value if provided\n");
					return 1; //can't read the next value			 	
				}
			 	alpha=atoi(argv[i+2]);
			}
			else if (argv[i + 1][0] == 'd')
			{
				b_delaunay = true;
			}
			else if(argv[i+1][0]=='c')
			    b_crust=true;
		 }	
		 
		 else{
		    filename=argv[i];
		 }
		 
	 }//end for
	 
   }//end if
   
   if (!(b_crust || b_delaunay || b_alphashape))
   {
	   printf("! Wanring: No shape method is specified. Convex hull will be built.\n");
   }

   if (filename == NULL)
   {
	   printf("! Error: No file is specified.\n");
	   return 1;
   }

   return 0;
}

/*-------------------------------------------------------------------*/
int main( int argc, char *argv[] )
{

   if( ParseArg(argc,argv)!=0 ){
      PrintUsage(argc,argv);
   	  return 1;
   }
   
   ReadVertices(filename);
   
   if (b_delaunay)
   {
	   Delaunay(); //compute delaunay triangulation
   }
   else if(b_alphashape)
   {
   	   AlphaShape(alpha); //compute alpha shape with value alpha
   }
   else if(b_crust)
   {
       Crust();           //compute crust surface
   }
   else
   {
       QHull(); //this simply shows an example of how qhull can be used to build CH
   }
      
   if(vertices==NULL && faces==NULL && edges==NULL && tetras==NULL){
       printf("! Error: empty tetras, vertices, faces and edges\n"); 
       return 1;
   }
   
   if(draw3D) Draw3D(argc,argv);
   
   return 0;
}

