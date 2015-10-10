/**
  * Interaction class for opengl.
  * OpenGL Interactor. (gliInteractor)
  *
  * 2001/3/19 Jyh-Ming
  */

#if OPENGL_ON

#include "gli.h"
#ifdef _WIN32
typedef char bool;
#define true    1
#define false   0
#else
#include <stdbool.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

GLfloat m_CameraPos[3] = {0,0,100};
GLfloat m_deltaDis[3]  = {0,0,0};

bool m_DisalbeMouseControl=false;

GLfloat m_currentAzim = 0;
GLfloat m_deltaAzim   = 0;
GLfloat m_currentElev = 0; 
GLfloat m_deltaElev   = 0;

int m_StartX        =0; 
int m_StartY        =0; 
int m_PressedButton =0;

GLI_DISPLAY_FUNC  m_dislpayFunc  = NULL;
GLI_MOUSE_FUNC    m_mouseFunc    = NULL;
GLI_MOTION_FUNC   m_motionFunc   = NULL;

float m_WindowX[3]={1,0,0};
float m_WindowY[3]={0,1,0};

void gliInit()
{
    glutDisplayFunc( gliDisplay );
    glutMouseFunc( gliMouse );
    glutMotionFunc( gliMotion );
}

void gliMainLoop() { glutMainLoop(); }
 
void gliDisplay( void )
{
    //Draw scene
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    if( !m_DisalbeMouseControl ){ //mouse control is enabled
        glTranslatef( (m_CameraPos[0]+m_deltaDis[0]),
                     -(m_CameraPos[1]+m_deltaDis[1]), 
                     -(m_CameraPos[2]+m_deltaDis[2]) );
        glRotated(m_currentElev+m_deltaElev, 1.0, 0.0, 0.0);
        glRotated(m_currentAzim+m_deltaAzim, 0.0, 1.0, 0.0);
    }

    if( m_dislpayFunc!=NULL ){
        m_dislpayFunc();
    }

    //draw reference axis
    glMatrixMode(GL_PROJECTION); //change to Ortho view
    glPushMatrix(); 
    glLoadIdentity();
    gluOrtho2D(0,20,0,20);

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();

    glTranslatef(1.1f,1,0);
    glRotated(m_currentElev+m_deltaElev, 1.0, 0.0, 0.0);
    glRotated(m_currentAzim+m_deltaAzim, 0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(1,0,0);
    glColor3f(0,1,0);
    glVertex3f(0,0,0);
    glVertex3f(0,1,0);
    glColor3f(0,0,1);
    glVertex3f(0,0,0);
    glVertex3f(0,0,1);
    glEnd();
    glPopMatrix();
    
    //pop GL_PROJECTION
    glMatrixMode(GL_PROJECTION); //change to Pers view
    glPopMatrix();

    glutSwapBuffers();
}

void gliMouse( int button, int state, int x, int y )
{
    int iD;
	
    if( !m_DisalbeMouseControl ){
        if( glutGetModifiers()!=GLUT_ACTIVE_CTRL ){
            if( state == GLUT_UP ) //reset every thing
            {
                for(iD=0;iD<3;iD++ ){
                    m_CameraPos[iD] += m_deltaDis[iD];
                    m_deltaDis[iD]=0;
                }

                m_currentElev += m_deltaElev;
                m_currentAzim += m_deltaAzim;

                m_deltaElev = 0.0;
                m_deltaAzim = 0.0;
            }
            else { //press down

                m_StartY = y;
                m_StartX = x;
                m_StartY = y;

                m_PressedButton = button;
            }
        }
        else m_PressedButton=-1;
    }

    //call it when alt is not pressed
    if( m_mouseFunc!=NULL )
        m_mouseFunc(button, state, x, y);

    //redraw any way
    glutPostRedisplay();
}

void  gliMotion( int x, int y )
{

    if( !m_DisalbeMouseControl ){

        if( m_PressedButton == GLUT_RIGHT_BUTTON )
        {
            m_deltaDis[2] = ((m_CameraPos[2]>10)?m_CameraPos[2]:10) * ((GLfloat)(y - m_StartY))/100.0;
        }
        else if( m_PressedButton == GLUT_MIDDLE_BUTTON ) {
            m_deltaDis[0] = ((m_CameraPos[0]>5)?m_CameraPos[0]:5) * ((GLfloat)(x - m_StartX))/20.0;
            m_deltaDis[1] = ((m_CameraPos[1]>5)?m_CameraPos[1]:5) * ((GLfloat)(y - m_StartY))/20.0;
          }
        else if(m_PressedButton == GLUT_LEFT_BUTTON)
        {
            m_deltaAzim = ((GLfloat)(x - m_StartX))/5.0;
            m_deltaElev = ((GLfloat)(y - m_StartY))/5.0;
            //compute window x, y dir
            m_WindowX[0]=1; m_WindowX[1]=0; m_WindowX[2]=0;
            m_WindowY[0]=0; m_WindowY[1]=1; m_WindowY[2]=0;
            gliRotateX(m_WindowX, m_currentElev+m_deltaElev);
            gliRotateY(m_WindowX, m_currentAzim+m_deltaAzim);
            m_WindowX[2]=-m_WindowX[2];
            gliRotateX(m_WindowY, m_currentElev+m_deltaElev);
            gliRotateY(m_WindowY, m_currentAzim+m_deltaAzim);   
            m_WindowY[2]=-m_WindowY[2];
        }
    }

    if( m_motionFunc!=NULL )
        m_motionFunc(x, y);

    //redraw any way
    glutPostRedisplay();
}

void gliRotateX(float v[3], float degree){
    float c=cos(3.1415926*degree/180);
    float s=sin(3.1415926*degree/180);
    float v1=v[1]*c-v[2]*s;
    float v2=v[1]*s+v[2]*c;
    v[1]=v1; v[2]=v2;
}

void gliRotateY(float v[3], float degree){
    float c=cos(3.1415926*degree/180);
    float s=sin(3.1415926*degree/180);
    float v0=v[0]*c+v[2]*s;
    float v2=-v[0]*s+v[2]*c;
    v[0]=v0; v[2]=v2;
}

//access functions

void gliMouseFunc( GLI_MOUSE_FUNC mouse ){
	m_mouseFunc = mouse;
}

void gliDisplayFunc( GLI_DISPLAY_FUNC display ){
	m_dislpayFunc = display;
}

void gliMotionFunc( GLI_MOTION_FUNC motion ){
	m_motionFunc = motion;
}   

void gliDisableMouse(){ m_DisalbeMouseControl=true; }

void gliEndMouse(){ m_DisalbeMouseControl=true; }

const float * getCameraPos() { return m_CameraPos; }

void setCameraPosX(float x) { m_CameraPos[0]=x; }

void setCameraPosY(float y) { m_CameraPos[1]=y; }

void setCameraPosZ(float z) { m_CameraPos[2]=z; }

const float * getWindowX() { return m_WindowX; }

const float * getWindowY() { return m_WindowY; }

#endif //OPENGL_ON
