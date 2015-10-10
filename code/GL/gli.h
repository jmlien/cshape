/**
  * Interaction class for opengl.
  * OpenGL Interactor. (gliInteractor)
  *
  * 2001/3/19 Jyh-Ming Lien
  */
#if OPENGL_ON

#ifndef _GLI_INTERACTOR_H_
#define _GLI_INTERACTOR_H_

#ifdef MACOS
#include <GLUT/glut.h>
#else
#include <stdlib.h>
#include <GL/glut.h>
#endif


////////////////////////////////////////////////////////////////////////////////
//
//  TypeDefs
//
////////////////////////////////////////////////////////////////////////////////

typedef void (* GLI_DISPLAY_FUNC) ( void );
typedef void (* GLI_MOUSE_FUNC)   ( int button, int state, int x, int y );
typedef void (* GLI_MOTION_FUNC)  ( int x, int y );

////////////////////////////////////////////////////////////////////////////////
//
//  Initializer
//
////////////////////////////////////////////////////////////////////////////////

void gliInit();
void gliMainLoop();

////////////////////////////////////////////////////////////////////////////////
//
//  Access Methods
//
////////////////////////////////////////////////////////////////////////////////

void gliMouseFunc( GLI_MOUSE_FUNC mouse );
void gliDisplayFunc( GLI_DISPLAY_FUNC display );
void gliMotionFunc( GLI_MOTION_FUNC motion );
void gliDisableMouse();
void gliEndMouse();

const float * getCameraPos();
void setCameraPosX(float x);
void setCameraPosY(float y);
void setCameraPosZ(float z);
const float * getWindowX();
const float * getWindowY();
float getAzim();
float getElev();

void gliDisplay( void );
void gliMouse( int button, int state, int x, int y );
void gliMotion( int x, int y );
void gliRotateX(float v[3], float degree);
void gliRotateY(float v[3], float degree);
   

#endif //_GLI_INTERACTOR_H_

#endif //#if OPENGL_ON
