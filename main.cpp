//  Created by Sail Wang on 17/07/2017.
//  Copyright © 2017 Sail Wang. All rights reserved.
//

//============================================================
// ============================================================

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
# include <iostream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CONSTANTS
/////////////////////////////////////////////////////////////////////////////

#define PI                  3.1415926535897932384626433832795

#define MAX_NUM_OF_DISCS    100     // Limit the number of discs.
#define MIN_RADIUS          10.0    // Minimum radius of disc.
#define MAX_RADIUS          40.0    // Maximum radius of disc.
#define NUM_OF_SIDES        18      // Number of polygon sides to approximate a disc.

#define MIN_X_SPEED         1.0     // Minimum speed of disc in X direction.
#define MAX_X_SPEED         20.0    // Maximum speed of disc in X direction.
#define MIN_Y_SPEED         1.0     // Minimum speed of disc in Y direction.
#define MAX_Y_SPEED         20.0    // Maximum speed of disc in Y direction.

#define DESIRED_FPS         30      // Approximate desired number of frames per second.


/////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
/////////////////////////////////////////////////////////////////////////////

typedef struct discType
{
    double pos[2];          // The X and Y coordinates of the center of the disc.
    double speed[2];        // The velocity of the disc in X and Y directions. Can be negative.
    double radius;          // Radius of the disc.
    unsigned char color[3]; // R, G, B colors of the disc.
} discType;


int numDiscs = 0;                   // Number of discs that have been added.

discType disc[ MAX_NUM_OF_DISCS ];  // Array for storing discs.

bool drawWireframe = false;         // Draw polygons in wireframe if true, otherwise
// otherwise polygons are filled.

int winWidth = 800;                 // Window width in pixels.
int winHeight = 600;                // Window height in pixels.

int PositionX = 0;
int PositionY = 0;
float Xspeed = 0.0;
float Yspeed = 0.0;
float Radius = 0.0;
float fanx[NUM_OF_SIDES+1];
float fany[NUM_OF_SIDES+1];
int r,g,b;

/////////////////////////////////////////////////////////////////////////////
// Draw the disc in its color using GL_TRIANGLE_FAN.
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////
// pre-compute the fans in a circle
///////////////////////////////
void circleFAN()
{
    float doublepi = 2.0 * PI;
    for (int i = 0; i<=NUM_OF_SIDES+1; i++)
    {
        fanx[i] = cos(i * doublepi / NUM_OF_SIDES);
        fany[i] = sin(i * doublepi / NUM_OF_SIDES);
    }
}
////////////////////////////////
// transfer specifications to a new disc
/////////////////////////////
void addmore()
{
    disc[numDiscs].pos[0] = PositionX;
    disc[numDiscs].pos[1] = winHeight - 1 - PositionY;
    disc[numDiscs].color[0] = r;
    disc[numDiscs].color[1] = g;
    disc[numDiscs].color[2] = b;
    disc[numDiscs].speed[0] = Xspeed;
    disc[numDiscs].speed[1] = Yspeed;
    disc[numDiscs].radius = Radius;
}

/////////////////////////////
/// FPS control function
///////////////////////////
int alpha;

void OnTimer(int value)
{
    alpha++;
    alpha = alpha % (int)(1000/DESIRED_FPS);
    glutPostRedisplay();
    glutTimerFunc(1000/DESIRED_FPS,OnTimer,1);
}

void DrawDisc( const discType *d )
{
    
    GLfloat x0 = d->pos[0];
    GLfloat y0 = d->pos[1];
    GLfloat radius = d->radius;
    GLchar R = d->color[0];
    GLchar G = d->color[1];
    GLchar B = d->color[2];

    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(R, G, B);
    glVertex2f(x0, y0); // center of circle
    for(int i = 0; i <= NUM_OF_SIDES; i++)
    {
        glVertex2f(
                   x0 + (radius * fanx[i]),
                   y0 + (radius * fany[i])
                   );
    }
    glEnd();
}



/////////////////////////////////////////////////////////////////////////////
// The display callback function.
/////////////////////////////////////////////////////////////////////////////

void MyDisplay( void )
{
    glClear( GL_COLOR_BUFFER_BIT );
    
    if ( drawWireframe )
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    else
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    
    for ( int i = 0; i < numDiscs; i++ ) DrawDisc( &disc[i] );
    glutSwapBuffers ();
    glFlush();
}



/////////////////////////////////////////////////////////////////////////////
// The mouse callback function.
// If mouse left button is pressed, a new disc is created with its center
// at the mouse cursor position. The new disc is assigned the followings:
// (1) a random radius between MIN_RADIUS and MAX_RADIUS,
// (2) a random speed in X direction in the range
//     from -MAX_X_SPEED to -MIN_X_SPEED, and from MIN_X_SPEED to MAX_X_SPEED.
// (3) a random speed in Y direction in the range
//     from -MAX_Y_SPEED to -MIN_Y_SPEED, and from MIN_Y_SPEED to MAX_Y_SPEED.
// (4) R, G, B color, each ranges from 0 to 255.
/////////////////////////////////////////////////////////////////////////////

void MyMouse( int btn, int state, int x, int y )
{
    if ( btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
       // printf( "x = %d, y = %d\n", x, y );
        if ( numDiscs >= MAX_NUM_OF_DISCS )
            printf( "Already reached maximum number of discs.\n" );
        else
        {
            //===========================
            // WRITE YOUR CODE HERE.
            //===========================
            PositionX = x;
            PositionY = y;
            Radius = MIN_RADIUS + static_cast <float> (rand())/( static_cast <float> (RAND_MAX/(MAX_RADIUS-MIN_RADIUS)));
            if ( rand() / double(RAND_MAX) >= 0.5 )
                Xspeed = MIN_X_SPEED + static_cast <float> (rand())/( static_cast <float> (RAND_MAX/(MAX_X_SPEED-MIN_X_SPEED)));
            else
                Xspeed = 0.0 - (MIN_X_SPEED + static_cast <float> (rand())/( static_cast <float> (RAND_MAX/(MAX_X_SPEED-MIN_X_SPEED))));
            if ( rand() / double(RAND_MAX) >= 0.5 )
                Yspeed =  MIN_Y_SPEED + static_cast <float> (rand())/( static_cast <float> (RAND_MAX/(MAX_Y_SPEED-MIN_Y_SPEED)));
            else
                Yspeed = 0.0 - (MIN_Y_SPEED + static_cast <float> (rand())/( static_cast <float> (RAND_MAX/(MAX_Y_SPEED-MIN_Y_SPEED))));
                
            r = (char) rand()%256;
            g = (char) rand()%256;
            b = (char) rand()%256;
            addmore();
            numDiscs++;
            glutPostRedisplay();
        }
    }
}



/////////////////////////////////////////////////////////////////////////////
// The reshape callback function.
// It also sets up the viewing.
/////////////////////////////////////////////////////////////////////////////

void MyReshape( int w, int h )
{
    winWidth = w;
    winHeight = h;
    
    glViewport( 0, 0, w, h ); // reset viewpoint to the whole window
    
    glMatrixMode( GL_PROJECTION );
    
    glLoadIdentity();
    glOrtho(0.0, (GLdouble) w , 0.0, (GLdouble) h , -1.0, 1.0);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}


/////////////////////////////////////////////////////////////////////////////
// The keyboard callback function.
/////////////////////////////////////////////////////////////////////////////

void MyKeyboard( unsigned char key, int x, int y )
{
    switch ( key )
    {
            // Quit program.
        case 'q':
        case 'Q': exit(0);
            break;
            
            // Toggle wireframe or filled polygons.
        case 'w':
        case 'W': drawWireframe = !drawWireframe;
            glutPostRedisplay();
            break;
            
            // Reset and erase all discs.
        case 'r':
        case 'R': numDiscs = 0;
            glutPostRedisplay();
            break;
    }
}



/////////////////////////////////////////////////////////////////////////////
// Updates the positions of all the discs.
//
// Increments the position of each disc by its speed in each of the
// X and Y directions. Note that the speeds can be negative.
// At its new position, if the disc is entirely or partially outside the
// left window boundary, then shift it right so that it is inside the
// window and just touches the left window boundary. Its speed in the X
// direction must now be reversed (negated). Similar approach is
// applied for the cases of the right, top, and bottom window boundaries.
/////////////////////////////////////////////////////////////////////////////

void UpdateAllDiscPos( void )
{
   
    for ( int i = 0; i < numDiscs; i++ )
    {
  
    
       disc[i].pos[0] = disc[i].pos[0] + disc[i].speed[0] ;
       disc[i].pos[1] = disc[i].pos[1] + disc[i].speed[1] ;
       
        //collision detection
       if (winWidth - disc[i].pos[0] <= disc[i].radius )
       {
           disc[i].speed[0] = -disc[i].speed[0];
           disc[i].pos[0] = winWidth - disc[i].radius;
       }
        
        if (disc[i].pos[0] <= disc[i].radius)
        {
            disc[i].speed[0] = -disc[i].speed[0];
            disc[i].pos[0] = disc[i].radius;
        }

        if (winHeight - disc[i].pos[1] <= disc[i].radius)
        {
            disc[i].speed[1] = -disc[i].speed[1];
            disc[i].pos[1] = winHeight - disc[i].radius;
        }
        if (disc[i].pos[1] <= disc[i].radius)
        {
            disc[i].speed[1] = -disc[i].speed[1];
            disc[i].pos[1] = disc[i].radius;
        }
        
    }
    
    
    
    
    
    glutPostRedisplay();
}



/////////////////////////////////////////////////////////////////////////////
// The init function.
// It initializes some OpenGL states.
/////////////////////////////////////////////////////////////////////////////

void MyInit( void )
{
    glClearColor( 0.0, 0.0, 0.0, 1.0 ); // Black background color.
    glShadeModel( GL_FLAT );
}



/////////////////////////////////////////////////////////////////////////////
// The main function.
/////////////////////////////////////////////////////////////////////////////

int main( int argc, char** argv )
{
    circleFAN();
    glutInit( &argc, argv );
    glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE );
    glutInitWindowSize( winWidth, winHeight );
    glutCreateWindow( "assign1" );
    
    MyInit();
    
    //precompute the triangle fan
    // Register the callback functions.
    glutDisplayFunc( MyDisplay );
    glutReshapeFunc( MyReshape );
    glutMouseFunc( MyMouse );
    glutKeyboardFunc( MyKeyboard );
    glutIdleFunc( UpdateAllDiscPos );
    
    // Display user instructions in console window.
    printf( "Click LEFT MOUSE BUTTON in window to add new disc.\n" );
    printf( "Press 'w' to toggle wireframe.\n" );
    printf( "Press 'r' to erase all discs.\n" );
    printf( "Press 'q' to quit.\n\n" );
    
    // Enter GLUT event loop.
    glutTimerFunc(1000/DESIRED_FPS,OnTimer,1);
    glutMainLoop();
    return 0;
}
