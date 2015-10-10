#if OPENGL_ON

#pragma once

#include "GL/gli.h"
#include "shape.h"

/* Global variable definitions */

//defined in shape.c
extern tVertex vertices;
extern tEdge edges;
extern tFace faces;
extern tTetra tetras;

extern bool debug;
extern bool check;

bool showF=true, showE=true, showV=true, showT=true;
double COM[3], R; //center and radius

#define SQR(x) ((x)*(x))

void computeCOM_R()
{
    //-------------------------------------------------------------------------
    // compute center of mass and R...
	int total=0;
    int i=0;
    double d=0;
    tVertex  v= vertices;
    COM[0]=COM[1]=COM[2]=0;
    R=0;
    
	do {                                 
	   COM[0]+=v->v[0];
	   COM[1]+=v->v[1];
	   COM[2]+=v->v[2];
	   total++;
	   v = v->next;
	} 
	while ( v != vertices );
   
    for(i=0;i<3;i++) COM[i]/=total;

    v= vertices;
    do {                                 
	   d=SQR(v->v[0]-COM[0])+SQR(v->v[1]-COM[1])+SQR(v->v[2]-COM[2]);
	   if(d>R) R=d;
	   v = v->next;
	} while ( v != vertices );
	
    R=sqrt(R);
}

void DrawTetrahedra()
{
    tTetra   t;
    tVertex  v1, v2, v3, v4;

    if(tetras==NULL) return;

    t = tetras;

    do {

      v1=t->vertex[0];
      v2=t->vertex[1];
      v3=t->vertex[2];
      v4=t->vertex[3];

      //t1
      glVertex3dv(v1->v);
      glVertex3dv(v2->v);
      glVertex3dv(v3->v);

      //t2
      glVertex3dv(v1->v);
      glVertex3dv(v2->v);
      glVertex3dv(v4->v);

      //t3
      glVertex3dv(v2->v);
      glVertex3dv(v3->v);
      glVertex3dv(v4->v);

      //t4
      glVertex3dv(v3->v);
      glVertex3dv(v1->v);
      glVertex3dv(v4->v);

      t = t->next;

    } while ( t != tetras );
}

//draw results using openGL
void DrawTriangles()
{
   
   tFace    f;
   tVertex  v1, v2, v3;
   
   if(faces==NULL) return;

   glEnable( GL_POLYGON_OFFSET_FILL );
   glPolygonOffset( 0.5f, 0.5f );
   glBegin(GL_TRIANGLES);
   f = faces;

   do {           
	
	  v1=f->vertex[0];
	  v2=f->vertex[1];
	  v3=f->vertex[2];

	  glVertex3dv(v1->v);
	  glVertex3dv(v2->v);
      glVertex3dv(v3->v);
      
      f = f->next;
   } while ( f != faces );
   
   glEnd();
   glDisable( GL_POLYGON_OFFSET_FILL );

   glPushAttrib(GL_ENABLE_BIT);
   glColor3f(0,0,0);
   f = faces;

   do {

      v1=f->vertex[0];
      v2=f->vertex[1];
      v3=f->vertex[2];

      glBegin(GL_LINE_LOOP);
      glVertex3dv(v1->v);
      glVertex3dv(v2->v);
      glVertex3dv(v3->v);
      glEnd();

      f = f->next;
   } while ( f != faces );


   glPopAttrib();
}

void DrawEdges()
{   
   tEdge    e;
   tVertex  v1, v2;
   
   if(edges==NULL) return;

   glBegin(GL_LINES);
   /* Edges. */	
   e = edges;
   do {
	  v1=e->endpts[0];
	  v2=e->endpts[1];
	  glVertex3dv(v1->v);
	  glVertex3dv(v2->v);
      e = e->next;
   } while ( e != edges );
   glEnd();
}


void DrawVertices(tVertex vlist)
{   
   tVertex  v;
   v = vlist;
   
   if(vlist==NULL) return;

   glBegin(GL_POINTS);
   do {                                 
	  glVertex3dv(v->v);
      v = v->next;
   } while ( v != vlist );
   glEnd();
}

//copied from meshlab
void DisplayBackground(void)
{
	float topcolor[]={1,1,1};
	float bottomcolor[]={0,0,0};
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1,1,-1,1,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLE_STRIP);
			glColor3fv(topcolor);      glVertex2f(-1, 1);
			glColor3fv(bottomcolor);   glVertex2f(-1,-1);
			glColor3fv(topcolor);      glVertex2f( 1, 1);
			glColor3fv(bottomcolor);   glVertex2f( 1,-1);
	glEnd();
	
	glPopAttrib();
	glPopMatrix(); // restore modelview
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


void Display(void)
{
    static GLfloat light_position1[] = {  100, 100, 100.0f, 1.0f };
    static GLfloat light_position2[] = { -100, -100, 50.0f, 1.0f };

	//Init Draw
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode(GL_MODELVIEW);
    DisplayBackground();
    
    glPushMatrix();
    glLoadIdentity();

    glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
    glPopMatrix();

	//draw
	glTranslated(-COM[0],-COM[1],-COM[2]);
	
	if(showF){
		glColor3f(1,1,1);
		DrawTriangles();
	}
	
	if(showE){
		glColor3f(0,0,0);
		DrawEdges();
	}
	
	if(showV){
		glPointSize(5);
		glColor3f(1,0,0);
		DrawVertices(vertices);
	}

	if(showT){
	    glEnable( GL_POLYGON_OFFSET_FILL );
	    glPolygonOffset( 0.5f, 0.5f );
	    glBegin(GL_TRIANGLES);
	    glColor3f(1,1,1);
	    DrawTetrahedra();
	    glEnd();
	    glDisable( GL_POLYGON_OFFSET_FILL );

	    glBegin(GL_LINES);
	    glColor3f(0,0,0);
	    DrawTetrahedra();
	    glEnd();
	}
}

//-----------------------------------------------------------------------------
// other regular openGL callback functions
bool InitGL()
{
    GLfloat Diffuse[] =  { 0.9f, 0.9f, 0.9f, 1.0f };
    GLfloat WhiteLight[] =  { 1.0f, 1.0f, 1.0f, 1.0f };

	// transparent
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    // others
    glEnable( GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glClearColor( 1,1,1,0 );

    //Let's have light!

    glMaterialfv(GL_FRONT, GL_DIFFUSE, Diffuse);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);


    glLightfv(GL_LIGHT0,GL_DIFFUSE,WhiteLight);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,WhiteLight);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    return true;
}

void Reshape( int w, int h)
{
    if(w>h)
        glViewport( 0, 0, (GLsizei)w, (GLsizei)w );
    else
        glViewport( 0, 0, (GLsizei)h, (GLsizei)h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 60, 1, 10, 10000000000 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

// show keys for controlling the rendering
void printGUIKeys()
{
	printf("GUI keys:\n");
	printf("1:   show/hide facets\n");
	printf("2:   show/hide edge\n");
	printf("3:   show/hide vertices\n");
	printf("4:   show/hide tetrahedra\n");

    printf("?:   show this message\n");
    printf("esc: quit\n");
}

//keyboard event function
void Keyboard( unsigned char key, int x, int y )
{	
    // find closest colorPt3D if ctrl is pressed...
    switch( key ){
        case 27: exit(0);
        case '1': showF=!showF; break;
        case '2': showE=!showE; break;
        case '3': showV=!showV; break;
        case '4': showT=!showT; break;
        case '?': printGUIKeys(); break;
    }

    glutPostRedisplay();
}


#endif //OPENGL_ON


void Draw3D(int argc, char ** argv)
{

#if OPENGL_ON
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH );
	glutInitWindowSize( 480, 480);
	glutInitWindowPosition( 50, 50 );
	glutCreateWindow( "shape" );

	InitGL();
	gliInit();
	gliDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);

	/////////////////////////////////////////////////////////////
	computeCOM_R();
	//set camera position
	setCameraPosZ(R*2);
	printGUIKeys();
	/////////////////////////////////////////////////////////////
	gliMainLoop();
	
#else

	fprintf(stderr,"! ERROR: OpenGL is not supported. Please recompile with OPENGL_ON=1 in Makefile\n");
	
#endif //OPENGL_ON

}
