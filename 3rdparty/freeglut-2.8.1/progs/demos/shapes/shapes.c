/*! \file    shapes.c
    \ingroup demos

    This program is a test harness for the various shapes
    in OpenGLUT.  It may also be useful to see which
    parameters control what behavior in the OpenGLUT
    objects.
 
    Spinning wireframe and solid-shaded shapes are
    displayed.  Some parameters can be adjusted.
 
   Keys:
      -    <tt>Esc &nbsp;</tt> Quit
      -    <tt>q Q &nbsp;</tt> Quit
      -    <tt>i I &nbsp;</tt> Show info
      -    <tt>p P &nbsp;</tt> Toggle perspective or orthographic projection
      -    <tt>r R &nbsp;</tt> Toggle fixed or animated rotation around model X-axis
      -    <tt>= + &nbsp;</tt> Increase \a slices
      -    <tt>- _ &nbsp;</tt> Decreate \a slices
      -    <tt>, < &nbsp;</tt> Decreate \a stacks
      -    <tt>. > &nbsp;</tt> Increase \a stacks
      -    <tt>9 ( &nbsp;</tt> Decreate \a depth  (Sierpinski Sponge)
      -    <tt>0 ) &nbsp;</tt> Increase \a depth  (Sierpinski Sponge)
      -    <tt>up&nbsp; &nbsp;</tt> Increase "outer radius"
      -    <tt>down&nbsp;</tt> Decrease "outer radius"
      -    <tt>left&nbsp;</tt> Decrease "inner radius"
      -    <tt>right</tt> Increase "inner radius"
      -    <tt>PgUp&nbsp;</tt> Next shape-drawing function
      -    <tt>PgDn&nbsp;</tt> Prev shape-drawing function

    \author  Written by Nigel Stewart November 2003

    \author  Portions Copyright (C) 2004, the OpenGLUT project contributors. <br>
             OpenGLUT branched from freeglut in February, 2004.
 
    \image   html openglut_shapes.png OpenGLUT Geometric Shapes Demonstration
    \include demos/shapes/shapes.c
*/

#include <GL/freeglut.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _MSC_VER
/* DUMP MEMORY LEAKS */
#include <crtdbg.h>
#endif

/* report GL errors, if any, to stderr */
void checkError(const char *functionName)
{
    GLenum error;
    while (( error = glGetError() ) != GL_NO_ERROR) {
        fprintf (stderr, "GL error 0x%X detected in %s\n", error, functionName);
    }
}
/*
 * This macro is only intended to be used on arrays, of course.
 */
#define NUMBEROF(x) ((sizeof(x))/(sizeof(x[0])))

/*
 * These global variables control which object is drawn,
 * and how it is drawn.  No object uses all of these
 * variables.
 */
static int function_index;
static int slices = 16;
static int stacks = 16;
static double irad = .25;
static double orad = 1.0;   /* doubles as size for objects other than Torus */
static int depth = 4;
static double offset[ 3 ] = { 0, 0, 0 };
static GLboolean show_info = GL_TRUE;
static float ar;
static GLboolean persProject = GL_TRUE;
static GLboolean animateXRot = GL_FALSE;

/*
 * Enum to tell drawSizeInfo what to draw for each object
 */
#define GEO_NO_SIZE 0
#define GEO_SIZE 1
#define GEO_SCALE 2
#define GEO_INNER_OUTER_RAD 4
#define GEO_RAD 8
#define GEO_BASE_HEIGHT 16
#define GEO_RAD_HEIGHT 32

/*
 * These one-liners draw particular objects, fetching appropriate
 * information from the above globals.  They are just thin wrappers
 * for the FreeGLUT objects.
 */
static void drawSolidTetrahedron(void)         { glutSolidTetrahedron ();                        }
static void drawWireTetrahedron(void)          { glutWireTetrahedron ();                         }
static void drawSolidCube(void)                { glutSolidCube(orad);                            }  /* orad doubles as size input */
static void drawWireCube(void)                 { glutWireCube(orad);                             }  /* orad doubles as size input */
static void drawSolidOctahedron(void)          { glutSolidOctahedron ();                         }
static void drawWireOctahedron(void)           { glutWireOctahedron ();                          }
static void drawSolidDodecahedron(void)        { glutSolidDodecahedron ();                       }
static void drawWireDodecahedron(void)         { glutWireDodecahedron ();                        }
static void drawSolidRhombicDodecahedron(void) { glutSolidRhombicDodecahedron ();                }
static void drawWireRhombicDodecahedron(void)  { glutWireRhombicDodecahedron ();                 }
static void drawSolidIcosahedron(void)         { glutSolidIcosahedron ();                        }
static void drawWireIcosahedron(void)          { glutWireIcosahedron ();                         }
static void drawSolidSierpinskiSponge(void)    { glutSolidSierpinskiSponge (depth, offset, orad);}  /* orad doubles as size input */
static void drawWireSierpinskiSponge(void)     { glutWireSierpinskiSponge (depth, offset, orad); }  /* orad doubles as size input */
static void drawSolidTorus(void)               { glutSolidTorus(irad,orad,slices,stacks);        }
static void drawWireTorus(void)                { glutWireTorus (irad,orad,slices,stacks);        }
static void drawSolidSphere(void)              { glutSolidSphere(orad,slices,stacks);            }  /* orad doubles as size input */
static void drawWireSphere(void)               { glutWireSphere(orad,slices,stacks);             }  /* orad doubles as size input */
static void drawSolidCone(void)                { glutSolidCone(irad,orad,slices,stacks);         }  /* irad doubles as base input, and orad as height input */
static void drawWireCone(void)                 { glutWireCone(irad,orad,slices,stacks);          }  /* irad doubles as base input, and orad as height input */
static void drawSolidCylinder(void)            { glutSolidCylinder(irad,orad,slices,stacks);     }  /* irad doubles as radius input, and orad as height input */
static void drawWireCylinder(void)             { glutWireCylinder(irad,orad,slices,stacks);      }  /* irad doubles as radius input, and orad as height input */
static void drawSolidTeapot(void)
{
    /* per Glut manpage, it should be noted that the teapot is rendered
     * with clockwise winding for front facing polygons...
     */
    glFrontFace(GL_CW);
    glutSolidTeapot(orad);  /* orad doubles as size input */
    glFrontFace(GL_CCW);
}
static void drawWireTeapot(void)
{
    /* per Glut manpage, it should be noted that the teapot is rendered
     * with clockwise winding for front facing polygons...
     */
    glFrontFace(GL_CW);
    glutWireTeapot(orad);  /* orad doubles as size input */
    glFrontFace(GL_CCW);
}

#define RADIUSFAC    0.70710678118654752440084436210485f

static void drawSolidCuboctahedron(void)
{
  GLfloat radius = RADIUSFAC*(GLfloat)orad; /* orad doubles as size */
  glBegin( GL_TRIANGLES );
    glNormal3d( 0.577350269189, 0.577350269189, 0.577350269189); glVertex3d( radius, radius, 0.0 ); glVertex3d( 0.0, radius, radius ); glVertex3d( radius, 0.0, radius );
    glNormal3d( 0.577350269189, 0.577350269189,-0.577350269189); glVertex3d( radius, radius, 0.0 ); glVertex3d( radius, 0.0,-radius ); glVertex3d( 0.0, radius,-radius );
    glNormal3d( 0.577350269189,-0.577350269189, 0.577350269189); glVertex3d( radius,-radius, 0.0 ); glVertex3d( radius, 0.0, radius ); glVertex3d( 0.0,-radius, radius );
    glNormal3d( 0.577350269189,-0.577350269189,-0.577350269189); glVertex3d( radius,-radius, 0.0 ); glVertex3d( 0.0,-radius,-radius ); glVertex3d( radius, 0.0,-radius );
    glNormal3d(-0.577350269189, 0.577350269189, 0.577350269189); glVertex3d(-radius, radius, 0.0 ); glVertex3d(-radius, 0.0, radius ); glVertex3d( 0.0, radius, radius );
    glNormal3d(-0.577350269189, 0.577350269189,-0.577350269189); glVertex3d(-radius, radius, 0.0 ); glVertex3d( 0.0, radius,-radius ); glVertex3d(-radius, 0.0,-radius );
    glNormal3d(-0.577350269189,-0.577350269189, 0.577350269189); glVertex3d(-radius,-radius, 0.0 ); glVertex3d( 0.0,-radius, radius ); glVertex3d(-radius, 0.0, radius );
    glNormal3d(-0.577350269189,-0.577350269189,-0.577350269189); glVertex3d(-radius,-radius, 0.0 ); glVertex3d(-radius, 0.0,-radius ); glVertex3d( 0.0,-radius,-radius );
  glEnd();

  glBegin( GL_QUADS );
    glNormal3d( 1.0, 0.0, 0.0 ); glVertex3d( radius, radius, 0.0 ); glVertex3d( radius, 0.0, radius ); glVertex3d( radius,-radius, 0.0 ); glVertex3d( radius, 0.0,-radius );
    glNormal3d(-1.0, 0.0, 0.0 ); glVertex3d(-radius, radius, 0.0 ); glVertex3d(-radius, 0.0,-radius ); glVertex3d(-radius,-radius, 0.0 ); glVertex3d(-radius, 0.0, radius );
    glNormal3d( 0.0, 1.0, 0.0 ); glVertex3d( radius, radius, 0.0 ); glVertex3d( 0.0, radius,-radius ); glVertex3d(-radius, radius, 0.0 ); glVertex3d( 0.0, radius, radius );
    glNormal3d( 0.0,-1.0, 0.0 ); glVertex3d( radius,-radius, 0.0 ); glVertex3d( 0.0,-radius, radius ); glVertex3d(-radius,-radius, 0.0 ); glVertex3d( 0.0,-radius,-radius );
    glNormal3d( 0.0, 0.0, 1.0 ); glVertex3d( radius, 0.0, radius ); glVertex3d( 0.0, radius, radius ); glVertex3d(-radius, 0.0, radius ); glVertex3d( 0.0,-radius, radius );
    glNormal3d( 0.0, 0.0,-1.0 ); glVertex3d( radius, 0.0,-radius ); glVertex3d( 0.0,-radius,-radius ); glVertex3d(-radius, 0.0,-radius ); glVertex3d( 0.0, radius,-radius );
  glEnd();
}

static void drawWireCuboctahedron(void)
{
  GLfloat radius = RADIUSFAC*(GLfloat)orad; /* orad doubles as size */
  glBegin( GL_LINE_LOOP );
    glNormal3d( 1.0, 0.0, 0.0 ); glVertex3d( radius, radius, 0.0 ); glVertex3d( radius, 0.0, radius ); glVertex3d( radius,-radius, 0.0 ); glVertex3d( radius, 0.0,-radius );
  glEnd();
  glBegin( GL_LINE_LOOP );
    glNormal3d(-1.0, 0.0, 0.0 ); glVertex3d(-radius, radius, 0.0 ); glVertex3d(-radius, 0.0,-radius ); glVertex3d(-radius,-radius, 0.0 ); glVertex3d(-radius, 0.0, radius );
  glEnd();
  glBegin( GL_LINE_LOOP );
    glNormal3d( 0.0, 1.0, 0.0 ); glVertex3d( radius, radius, 0.0 ); glVertex3d( 0.0, radius,-radius ); glVertex3d(-radius, radius, 0.0 ); glVertex3d( 0.0, radius, radius );
  glEnd();
  glBegin( GL_LINE_LOOP );
    glNormal3d( 0.0,-1.0, 0.0 ); glVertex3d( radius,-radius, 0.0 ); glVertex3d( 0.0,-radius, radius ); glVertex3d(-radius,-radius, 0.0 ); glVertex3d( 0.0,-radius,-radius );
  glEnd();
  glBegin( GL_LINE_LOOP );
    glNormal3d( 0.0, 0.0, 1.0 ); glVertex3d( radius, 0.0, radius ); glVertex3d( 0.0, radius, radius ); glVertex3d(-radius, 0.0, radius ); glVertex3d( 0.0,-radius, radius );
  glEnd();
  glBegin( GL_LINE_LOOP );
    glNormal3d( 0.0, 0.0,-1.0 ); glVertex3d( radius, 0.0,-radius ); glVertex3d( 0.0,-radius,-radius ); glVertex3d(-radius, 0.0,-radius ); glVertex3d( 0.0, radius,-radius );
  glEnd();
}

#undef RADIUSFAC

/*
 * This structure defines an entry in our function-table.
 */
typedef struct
{
    const char * const name;
    void (*solid) (void);
    void (*wire)  (void);
    int drawSizeInfoFlag;
} entry;

#define ENTRY(e,f) {#e, drawSolid##e, drawWire##e,f}
static const entry table [] =
{
    ENTRY (Tetrahedron,GEO_NO_SIZE),
    ENTRY (Cube,GEO_SIZE),
    ENTRY (Octahedron,GEO_NO_SIZE),
    ENTRY (Dodecahedron,GEO_NO_SIZE),
    ENTRY (RhombicDodecahedron,GEO_NO_SIZE),
    ENTRY (Icosahedron,GEO_NO_SIZE),
    ENTRY (SierpinskiSponge,GEO_SCALE),
    ENTRY (Teapot,GEO_SIZE),
    ENTRY (Torus,GEO_INNER_OUTER_RAD),
    ENTRY (Sphere,GEO_RAD),
    ENTRY (Cone,GEO_BASE_HEIGHT),
    ENTRY (Cylinder,GEO_RAD_HEIGHT),
    ENTRY (Cuboctahedron,GEO_SIZE)   /* This one doesn't work when in shader mode and is then skipped */
};
#undef ENTRY

/*!
    Does printf()-like work using freeglut
    glutBitmapString().  Uses a fixed font.  Prints
    at the indicated row/column position.

    Limitation: Cannot address pixels.
    Limitation: Renders in screen coords, not model coords.
*/
static void shapesPrintf (int row, int col, const char *fmt, ...)
{
    static char buf[256];
    int viewport[4];
    void *font = GLUT_BITMAP_9_BY_15;
    va_list args;

    va_start(args, fmt);
#if defined(WIN32) && !defined(__CYGWIN__)
    (void) _vsnprintf (buf, sizeof(buf), fmt, args);
#else
    (void) vsnprintf (buf, sizeof(buf), fmt, args);
#endif
    va_end(args);

    glGetIntegerv(GL_VIEWPORT,viewport);

    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

        glOrtho(0,viewport[2],0,viewport[3],-1,1);

        glRasterPos2i
        (
              glutBitmapWidth(font, ' ') * col,
            - glutBitmapHeight(font) * row + viewport[3]
        );
        glutBitmapString (font, (unsigned char*)buf);

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

/* Print info about the about the current shape and render state on the screen */
static void DrawSizeInfo(int *row)
{
    switch (table [function_index].drawSizeInfoFlag)
    {
    case GEO_NO_SIZE:
        break;
    case GEO_SIZE:
        shapesPrintf ((*row)++, 1, "Size  Up  Down : %f", orad);
        break;
    case GEO_SCALE:
        shapesPrintf ((*row)++, 1, "Scale  Up  Down : %f", orad);
        break;
    case GEO_INNER_OUTER_RAD:
        shapesPrintf ((*row)++, 1, "Inner radius Left Right: %f", irad);
        shapesPrintf ((*row)++, 1, "Outer radius  Up  Down : %f", orad);
        break;
    case GEO_RAD:
        shapesPrintf ((*row)++, 1, "Radius  Up  Down : %f", orad);
        break;
    case GEO_BASE_HEIGHT:
        shapesPrintf ((*row)++, 1, "Base   Left Right: %f", irad);
        shapesPrintf ((*row)++, 1, "Height  Up  Down : %f", orad);
        break;
    case GEO_RAD_HEIGHT:
        shapesPrintf ((*row)++, 1, "Radius Left Right: %f", irad);
        shapesPrintf ((*row)++, 1, "Height  Up  Down : %f", orad);
        break;
    }
}

static void drawInfo()
{
    int row = 1;
    shapesPrintf (row++, 1, "Shape PgUp PgDn: %s", table [function_index].name);
    shapesPrintf (row++, 1, "Slices +-: %d   Stacks <>: %d", slices, stacks);
    shapesPrintf (row++, 1, "nSides +-: %d   nRings <>: %d", slices, stacks);
    shapesPrintf (row++, 1, "Depth  (): %d", depth);
    DrawSizeInfo(&row);
    if (persProject)
        shapesPrintf (row++, 1, "Perspective projection (p)");
    else
        shapesPrintf (row++, 1, "Orthographic projection (p)");
    if (animateXRot)
        shapesPrintf (row++, 1, "2D rotation (r)");
    else
        shapesPrintf (row++, 1, "1D rotation (r)");
}

/* GLUT callback Handlers */
static void
resize(int width, int height)
{
    ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
}

static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*89.0;
    const double b = (animateXRot?t:1)*67.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (persProject)
        glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    else
        glOrtho(-ar*3, ar*3, -3.0, 3.0, 2.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_LIGHTING);

    glColor3d(1,0,0);

    glPushMatrix();
        glTranslated(0,1.2,-6);
        glRotated(b,1,0,0);
        glRotated(a,0,0,1);
        table [function_index].solid ();
    glPopMatrix();

    glPushMatrix();
        glTranslated(0,-1.2,-6);
        glRotated(b,1,0,0);
        glRotated(a,0,0,1);
        table [function_index].wire ();
    glPopMatrix();

    glDisable(GL_LIGHTING);
    glColor3d(0.1,0.1,0.4);

    if( show_info )
        /* print info to screen */
        drawInfo();
    else
        /* print to command line instead */
        printf ( "Shape %d slides %d stacks %d\n", function_index, slices, stacks ) ;

    glutSwapBuffers();
}


static void
key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 :
    case 'Q':
    case 'q': glutLeaveMainLoop () ;      break;

    case 'I':
    case 'i': show_info=!show_info;       break;

    case '=':
    case '+': slices++;                   break;

    case '-':
    case '_': if( slices > -1 ) slices--; break;

    case ',':
    case '<': if( stacks > -1 ) stacks--; break;

    case '.':
    case '>': stacks++;                   break;

    case '9': 
    case '(': if( depth > -1 ) depth--;   break;

    case '0': 
    case ')': ++depth;                    break;

    case 'P':
    case 'p': persProject=!persProject;   break;

    case 'R':
    case 'r': animateXRot=!animateXRot;   break;

    default:
        break;
    }

    glutPostRedisplay();
}

static void special (int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_PAGE_UP:    ++function_index; break;
    case GLUT_KEY_PAGE_DOWN:  --function_index; break;
    case GLUT_KEY_UP:         orad *= 2;        break;
    case GLUT_KEY_DOWN:       orad /= 2;        break;

    case GLUT_KEY_RIGHT:      irad *= 2;        break;
    case GLUT_KEY_LEFT:       irad /= 2;        break;

    default:
        break;
    }

    if (0 > function_index)
        function_index = NUMBEROF (table) - 1;

    if (NUMBEROF (table) <= ( unsigned )function_index)
        function_index = 0;
}


static void
idle(void)
{
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Program entry point */

int
main(int argc, char *argv[])
{
    glutInitWindowSize(800,600);
    glutInitWindowPosition(40,40);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

    glutCreateWindow("FreeGLUT Shapes");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(special);
    glutIdleFunc(idle);

    glutSetOption ( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION ) ;

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glutMainLoop();

#ifdef _MSC_VER
    /* DUMP MEMORY LEAK INFORMATION */
    _CrtDumpMemoryLeaks () ;
#endif

    return EXIT_SUCCESS;
}
