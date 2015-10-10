#include "crust.h"
#include "shape.h"

/*-------------------------------------------------------------------*/
//headers from qhull
#include "qhull.h"
#include "poly.h"
#include "qset.h"
#include "geom.h"

/*-------------------------------------------------------------------*/
//defined in shape.h/.c
extern tVertex vertices;
extern tEdge edges;
extern tFace faces;
extern tTetra tetras;

/*-------------------------------------------------------------------*/
void	Delaunay( void )
{
	// Use qhull to compute Delaunay triangulation of the points.
	// put results (vertices, edges, faces, and tetrahedra)
	// in the global structures above
	// see main.c for detail on how qhull does it


	//
	// create points in 4D (x,y,z,x^2+y^2+z^2)
	//

	//
	// compute convex hull in 4D by calling qhull
	// use flags: static char * options=(char *)"delaunay QJ Pp";
	//

	//
	//loop through all faces and call MakeNullTetra() to make a tetra 
	//remember that this is in 4D, so each face is a tetrahedron
	//
	//to fill the teta: get vertices of facet and loop through each vertex
	//use FOREACHvertex_()
	//
}



