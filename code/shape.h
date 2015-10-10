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

#pragma once

#include <stdio.h>
#include <math.h>
#include <assert.h>

#ifdef _WIN32
typedef char bool;
#define true    1
#define false   0
#else
#include <stdbool.h>
#endif

/*Define Boolean type */


/* Define vertex indices. */
#define X   0
#define Y   1
#define Z   2

/* Define structures for vertices, edges and faces */
typedef struct tVertexStructure tsVertex;
typedef tsVertex *tVertex;

typedef struct tEdgeStructure tsEdge;
typedef tsEdge *tEdge;

typedef struct tFaceStructure tsFace;
typedef tsFace *tFace;

typedef struct tTetraStructure tsTetra;
typedef tsTetra *tTetra;

typedef struct tVVStructure tsVV;
typedef tsVV *tVV;

typedef struct tListStructure tsList;
typedef tsList *tList;

//a link list
struct tVertexStructure {
   double   v[3];
   int	    vnum;               //vertex id
   tList    vvlist;             //a list of Voronoi vertices
   bool     ispole;
   tVertex  next, prev;
};

struct tEdgeStructure {
   tFace    adjface[2]; //adjacent faces
   tVertex  endpts[2];
   tEdge    next, prev;
};

struct tFaceStructure {
   tEdge    edge[3];
   tVertex  vertex[3];
   tFace    next, prev;
};


//this is a tetrahedron
struct tTetraStructure {
	tFace face[4];
	tVertex  vertex[4];
    tTetra next, prev;
};

struct tListStructure {
   void * p;
   tList  next, prev;
};

// Define flags 
#define ONHULL   	true
#define REMOVED  	true
#define VISIBLE  	true
#define PROCESSED	true
#define SAFE		1000000		/* Range of safe coord values. */

// some basic function declarations
tVertex MakeNullVertex( void );
tEdge   MakeNullEdge( void );
tFace   MakeNullFace( void );
tTetra  MakeNullTetra( void );
tFace   MakeFace( tVertex v0, tVertex v1, tVertex v2, tFace f );

void    ReadVertices( char * filename );
void    Print( void );
void	PrintPoint( tVertex p );

int     VolumeSign(tFace f, tVertex p);
double 	Volumei( tFace f, tVertex p );
void	EdgeOrderOnFaces ( void );


//these are implmeneted in check.c
bool	Collinear( tVertex a, tVertex b, tVertex c );
void    CheckEuler(int V, int E, int F );
void	Consistency( void );
void	CheckEndpts ( void );

//these are implmeneted in print.c
void	PrintOut( tVertex v );
void	PrintVertices( void );
void	PrintEdges( void );
void	PrintFaces( void );
void    SubVec( int a[3], int b[3], int c[3]);

#include "macros.h"

