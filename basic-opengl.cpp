//-----------------------------------------------------------------------------
//    Description: This sample demonstrates how to collect user input and 
//                 build a custom view matrix for First Person Shooter style 
//                 controls.
//
//   Control Keys: Up         - View moves forward
//                 Down       - View moves backward
//                 Left       - View strafes left
//                 Right      - View strafes Right
//                 Left Mouse - Perform looking
//                 Mouse      - Look about the scene
//                 Ctrl+H     - View moves up
//                 Ctrl+E     - View moves down
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include "matrix4x4f.h"
#include "vector3f.h"

struct POINT {
  long x;
  long y;
};

#define KEY_ESCAPE      27
#define KEY_END         35
#define KEY_HOME        36
#define KEY_LEFT_ARROW  100
#define KEY_UP_ARROW    101
#define KEY_RIGHT_ARROW 102
#define KEY_DOWN_ARROW  103
#define KEY_T  84
#define KEY_t  116
#define KEY_S  83
#define KEY_s  115
#define KEY_R  82
#define KEY_r  114
#define KEY_H  72
#define KEY_E  69

#define TEAPOT          1
#define SPHERE          2
#define TORUS           3

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------

int nXDiff = 0;
int nYDiff = 0; 

float Znear = 0.1f, 
      Zfar  = 100.0f;

POINT  g_ptLastMousePosit;
POINT  g_ptCurrentMousePosit;
bool   g_bMousing = false;
float  g_fMoveSpeed = 0.10f;

int    g_object = TEAPOT;
//
//  Eye point and vectors associated with  
vector3f g_vEye(5.0f, 5.0f, 5.0f);     // Eye Position
vector3f g_vLook(-0.5f, -0.5f, -0.5f); // Look Vector
vector3f g_vUp(0.0f, 1.0f, 0.0f);      // Up Vector
vector3f g_vRight(1.0f, 0.0f, 0.0f);   // Right Vector

struct Vertex
{
	// GL_C4UB_V3F
	unsigned char r, g, b, a;
	float x, y, z;
};

Vertex g_lineVertices[] =
{
	{ 255,   0,   0, 255,  0.0f, 0.0f, 0.0f }, // red   = +x Axis
	{ 255,   0,   0, 255,  5.0f, 0.0f, 0.0f },
	{   0, 255,   0, 255,  0.0f, 0.0f, 0.0f }, // green = +y Axis
	{   0, 255,   0, 255,  0.0f, 5.0f, 0.0f },
	{   0,   0, 255, 255,  0.0f, 0.0f, 0.0f }, // blue  = +z Axis
	{   0,   0, 255, 255,  0.0f, 0.0f, 5.0f }
};

//-----------------------------------------------------------------------------
// PROTOTYPES
//-----------------------------------------------------------------------------

void render(void);
void shutDown(void);
void getRealTimeUserInput(void);
void updateViewMatrix(void);


  
//*****************************************************
//  
//  worldMotion
//
//*****************************************************
void worldMotion(int x, int y)
{
  // update the current mouse position
  g_ptCurrentMousePosit.x = x;
  g_ptCurrentMousePosit.y = y;
  // compute the difference between the current and previous positions
  nXDiff = x - g_ptLastMousePosit.x;
  nYDiff = y - g_ptLastMousePosit.y;
  // update the last position as the old current position
  g_ptLastMousePosit.x = g_ptCurrentMousePosit.x;
  g_ptLastMousePosit.y = g_ptCurrentMousePosit.y;
    
}


//*****************************************************
//  
//  Mouse fucntion
//
//*****************************************************
void Mouse(int button, int state, int x, int y)
{
    // initializes the current mouse coordinates when a mouse button is pressed
	g_ptCurrentMousePosit.x = x;
    g_ptCurrentMousePosit.y = y;
    
    // if the left mouse is pressed allow the rotation of the camera
    if (button == GLUT_LEFT_BUTTON) {
	   if (state == GLUT_DOWN) 
             g_bMousing = true;
	   else 
			 g_bMousing = false;
    }
}

void process_SHIFT_ALT_CTRL(unsigned char key, int x, int y) 
{
    // Press ALT or  SHIFT or  CTRL in combination with other keys.
    printf("key_code =%d  \n",key);

    int mod = glutGetModifiers();

    if (mod != 0) //ALT=4  SHIFT=1  CTRL=2
    {      

          printf("MOD %d\n",mod);
          switch(mod)
          {
             case 1 :  printf("SHIFT key %d\n",mod);  break;
             case 2 :  printf("CTRL  key %d\n",mod);  break;
             case 4 :  printf("ALT   key %d\n",mod);  break;
             mod=0;
          }
    }
}

void process_Normal_Keys(int key, int x, int y) 
{
    vector3f tmpLook  = g_vLook;
    vector3f tmpRight = g_vRight;

    // Left Arrow Key - View side-steps or strafes to the left
    if (key == KEY_LEFT_ARROW)  
        g_vEye -= (tmpRight*g_fMoveSpeed);
    // Right Arrow Key - View side-steps or strafes to the right
    if (key == KEY_RIGHT_ARROW) 
        g_vEye += (tmpRight*g_fMoveSpeed);		
    // UP ARROW - View moves forward
    if (key == KEY_UP_ARROW)  
        g_vEye -= tmpLook*-g_fMoveSpeed;
    // DOWN ARROW - View moves backward
    if (key == KEY_DOWN_ARROW)  
        g_vEye += (tmpLook*-g_fMoveSpeed);

}

void KeyboardFunc(unsigned char key, int x, int y) {

    int mod = glutGetModifiers();

    if (mod != 0) //ALT=4  SHIFT=1  CTRL=2
    {      

          printf("MOD %d\n",mod);
          switch(mod)
          {
             case 1 :  printf("SHIFT key %d\n",mod);  break;
             case 2 :  printf("CTRL  key %d\n",mod);  break;
             case 4 :  printf("ALT   key %d\n",mod);  break;
             mod=0;
          }
    }

    // T KEY - Draw Teapot
    if (key == KEY_t || key == KEY_T) {
        g_object = TEAPOT;
    }

    // R KEY - Draw Torus
    if (key == KEY_r || key == KEY_R) {
        g_object = TORUS;
    }

    // T KEY - Draw sphere
    if (key == KEY_s || key == KEY_S) {
        g_object = SPHERE;
    }

    // HOME KEY - View moves up
    if (key == KEY_H)
        g_vEye.y += g_fMoveSpeed;
    // END KEY - View moves down
    if (key == KEY_E)
        g_vEye.y -= g_fMoveSpeed;
    // ESCAPE KEY - Quits the program
    if (key == KEY_ESCAPE) {
        exit(1); 
    }
}

//*****************************************************************
// Name: getRealTimeUserInput()
// Desc: 
//*****************************************************************
void getRealTimeUserInput( void )
{
   vector3f tmpLook  = g_vLook;
   vector3f tmpRight = g_vRight;

	matrix4x4f matRotation;

    if( g_bMousing )
    {        
        if( nYDiff != 0 )
        {
            matRotation.rotate( -(float)nYDiff / 3.0f, g_vRight );
            matRotation.transformVector( &g_vLook );
            matRotation.transformVector( &g_vUp );
        }

        if( nXDiff != 0 )
        {
            vector3f tmp = vector3f(0.0f, 1.0f, 0.0f);
            matRotation.rotate( -(float)nXDiff / 3.0f, tmp );
            matRotation.transformVector( &g_vLook );
            matRotation.transformVector( &g_vUp );
        }
    }

    g_ptLastMousePosit.x = g_ptCurrentMousePosit.x;
    g_ptLastMousePosit.y = g_ptCurrentMousePosit.y;

}

//*****************************************************************
// Name : updateViewMatrix() 
// Desc : Builds a view matrix suitable for OpenGL.
//
// Here's what the final view matrix should look like:
//
//  |  rx   ry   rz  -(r.e) |
//  |  ux   uy   uz  -(u.e) |
//  | -lx  -ly  -lz   (l.e) |
//  |   0    0    0     1   |
//
// Where r = Right vector
//       u = Up vector
//       l = Look vector
//       e = Eye position in world space
//       . = Dot-product operation
//
//*****************************************************************
void updateViewMatrix( void )
{
	matrix4x4f view;
	view.identity();

	// normalize the lookat vector. We will call this vector "n" 
	g_vLook.normalize();
    // compute and normalize the "X-direction" of the camera coordinate system.
	// We will call this vector "u"
	g_vRight = crossProduct(g_vLook, g_vUp);
	g_vRight.normalize();
    // compute and normalize the "Y-direction" of the camera coordinate system.
	// We will call this vector "v"
	g_vUp = crossProduct(g_vRight, g_vLook);
	g_vUp.normalize();

	// set the elements of the view matrix as a ModelView
	view.m[0] =  g_vRight.x;
	view.m[1] =  g_vUp.x;
	view.m[2] = -g_vLook.x;
	view.m[3] =  0.0f;

	view.m[4] =  g_vRight.y;
	view.m[5] =  g_vUp.y;
	view.m[6] = -g_vLook.y;
	view.m[7] =  0.0f;

	view.m[8]  =  g_vRight.z;
	view.m[9]  =  g_vUp.z;
	view.m[10] = -g_vLook.z;
	view.m[11] =  0.0f;

	view.m[12] = -dotProduct(g_vRight, g_vEye);
	view.m[13] = -dotProduct(g_vUp, g_vEye);
	view.m[14] =  dotProduct(g_vLook, g_vEye);
	view.m[15] =  1.0f;

	// multiply the current ModelView matrix (current an Identity Matrix) 
	// by the content of the matrix view
	glMultMatrixf( view.m );
}

//***********************************************
// Name: render()
// Desc: 
//***********************************************
void render( void )
{
    //  clear buffers
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	//  reads user input
	getRealTimeUserInput();
    //  update modelview matrix with the result of user input
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
	updateViewMatrix();
    //  set color to white and draws a wireframe version of the teapot
    glColor3f( 1.0f, 1.0f, 1.0f );

	if(g_object == TEAPOT) 
		glutWireTeapot( 2.0f );
	if(g_object == TORUS) 
		glutWireTorus( 2.0f, 2.0f, 100, 100 );
	if(g_object == SPHERE) 
		glutWireSphere( 2.0f, 100, 100 );

    //  draw the colored lines for the three axes
	glInterleavedArrays( GL_C4UB_V3F, 0, g_lineVertices );
    glDrawArrays( GL_LINES, 0, 6 );
    //  swap buffers 
	glutSwapBuffers();
}

//***********************************************
//
//  OpenGL Reshape function
//
//  Sets the viewport and the projection matrix
//  
//***********************************************
void openglReshape(int w, int h)
{
  // set the viewport to the size of the window
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  // set the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective( 45.0f, 640.0f / 480.0f, Znear, Zfar); 
  // set the modelview matrix to identity
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


//*****************************************************
//  main
//*****************************************************

int main(int argc, char *argv[])
{
//
//   initialize the window
//
  glutInit( & argc, argv );
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(640, 480);
  glutCreateWindow("Simple OpenGL Application");
  glutDisplayFunc(render);
  glutReshapeFunc(openglReshape);
  glutMouseFunc(Mouse);
  glutMotionFunc(worldMotion);
  glutSpecialFunc( process_Normal_Keys );
  glutKeyboardFunc(KeyboardFunc);
  glutIdleFunc(render);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  // set the background color for the window
  glClearColor(0.35f, 0.53f, 0.7f, 1.0f);
  glutMainLoop();
  return 0;
}
