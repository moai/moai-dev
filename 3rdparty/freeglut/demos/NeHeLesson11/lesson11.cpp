/*
 NeHe (nehe.gamedev.net) OpenGL tutorial series
 GLUT port.in 2001 by milix (milix_gr@hotmail.com)
 Most comments are from the original tutorials found in NeHe.
 For VC++ users, create a Win32 Console project and link 
 the program with glut32.lib, glu32.lib, opengl32.lib
*/

#include <stdio.h>           // Standard C/C++ Input-Output
#include <math.h>            // Math Functions
#include <gl/glut.h>         // The GL Utility Toolkit (GLUT) Header

#define TEXTURES_NUM 1       // We Have 1 Texture 
#define PI_OVER_180  0.0174532925f

// A Structure For RGB Bitmaps
typedef struct _RGBIMG {
	GLuint   w;    // Image's Width
	GLuint   h;    // Image's Height
	GLubyte* data; // Image's Data (Pixels)
} RGBIMG;

// Global Variables
bool    g_gamemode;            // GLUT GameMode ON/OFF
bool    g_fullscreen;          // Fullscreen Mode ON/OFF (When g_gamemode Is OFF)
float   g_points[45][45][3];   // The Array For The Points On The Grid Of Our "Wave"
int     g_wiggle = 0;          // Counter Used To Control How Fast Flag Waves
GLfloat	g_xrot = 0.0;          // X Rotation 
GLfloat	g_yrot = 0.0;          // Y Rotation 
GLfloat	g_zrot = 0.0;          // Z Rotation 
GLuint	g_texid[TEXTURES_NUM]; // Our Textures' Id List 


// Loads A RGB Raw Image From A Disk File And Updates Our Image Reference
// Returns true On Success, False On Fail.
bool load_rgb_image(const char* file_name, int w, int h, RGBIMG* refimg)
{
	GLuint   sz;    // Our Image's Data Field Length In Bytes
	FILE*    file;  // The Image's File On Disk
	long     fsize; // File Size In Bytes
	GLubyte* p;     // Helper Pointer

	// Update The Image's Fields
	refimg->w = (GLuint) w;
	refimg->h = (GLuint) h;
	sz = (((3*refimg->w+3)>>2)<<2)*refimg->h;
	refimg->data = new GLubyte [sz];
	if (refimg->data == NULL) return false;

	// Open The File And Read The Pixels
	file = fopen(file_name , "rb");
	if (!file) return false;
	fseek(file, 0L, SEEK_END);
	fsize = ftell(file);
	if (fsize != (long)sz) return false;
	fseek(file, 0L, SEEK_SET);
	p = refimg->data;
	while (fsize > 0) {
		fread(p, 1, 1, file);
		p++;
		fsize--;
	}
	fclose(file); 
	return true;
}

// Setup Our Textures. Returns true On Success, false On Fail
bool setup_textures()
{
	RGBIMG img;

    // Create The Textures' Id List
	glGenTextures(TEXTURES_NUM, g_texid);          
	// Load The Image From A Disk File
	if (!load_rgb_image("tim_256x256.raw", 256, 256, &img)) return false;
	// Create Nearest Filtered Texture
	glBindTexture(GL_TEXTURE_2D, g_texid[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, img.w, img.h, 0, GL_RGB, GL_UNSIGNED_BYTE, img.data);
	// Finished With Our Image, Free The Allocated Data
	delete img.data;
	return true;
}

// Our GL Specific Initializations. Returns true On Success, false On Fail.
bool init(void)
{
	if (!setup_textures()) return false;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);             // Pixel Storage Mode To Byte Alignment
	glEnable(GL_TEXTURE_2D);                           // Enable Texture Mapping 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);                 // Set The Blending Function (NEW)
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);			   // Black Background (CHANGED)
    glClearDepth(1.0f);								   // Depth Buffer Setup
    glDepthFunc(GL_LEQUAL);							   // The Type Of Depth Testing To Do
    glEnable(GL_DEPTH_TEST);						   // Enables Depth Testing
    glShadeModel(GL_SMOOTH);						   // Enable Smooth Shading
    //ShowCursor(false);                                 // WiN API - Hides The Mouse Pointer (NEW)
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glPolygonMode(GL_BACK, GL_FILL);                   // Back Face Is Solid (NEW)
	glPolygonMode(GL_FRONT, GL_LINE);                  // Front Face Is Made Of Lines (NEW)
	for (int x = 0; x < 45; x++) {
		for(int y = 0; y < 45; y++) {
			g_points[x][y][0]=float((x/5.0f)-4.5f);
			g_points[x][y][1]=float((y/5.0f)-4.5f);
			g_points[x][y][2]=float(sin((((x/5.0f)*40.0f)/360.0f)*3.141592654*2.0f));
		}
	}
	return true;
}

// Our Rendering Is Done Here
void render(void)   
{
	int x, y;
	float float_x, float_y, float_xb, float_yb, tmp;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The View

	glTranslatef(0.0f,0.0f,-12.0f);
	  
	glRotatef(g_xrot,1.0f,0.0f,0.0f);
	glRotatef(g_yrot,0.0f,1.0f,0.0f);  
	glRotatef(g_zrot,0.0f,0.0f,1.0f);

	glBindTexture(GL_TEXTURE_2D, g_texid[0]);

	glBegin(GL_QUADS);
	for (x = 0; x < 44; x++) {
		for (y = 0; y < 44; y++) {
			float_x = float(x)/44.0f;
			float_y = float(y)/44.0f;
			float_xb = float(x+1)/44.0f;
			float_yb = float(y+1)/44.0f;

			glTexCoord2f(float_x, float_y);
			glVertex3f(g_points[x][y][0], g_points[x][y][1], g_points[x][y][2]);

			glTexCoord2f(float_x, float_yb);
			glVertex3f(g_points[x][y+1][0], g_points[x][y+1][1], g_points[x][y+1][2]);

			glTexCoord2f(float_xb, float_yb);
			glVertex3f(g_points[x+1][y+1][0], g_points[x+1][y+1][1], g_points[x+1][y+1][2]);

			glTexCoord2f(float_xb, float_y);
			glVertex3f(g_points[x+1][y][0], g_points[x+1][y][1], g_points[x+1][y][2]);
		}
	}
	glEnd();

	if (g_wiggle == 2) {
		for (y = 0; y < 45; y++) {
			tmp = g_points[0][y][2];
			for (x = 0; x < 44; x++) g_points[x][y][2] = g_points[x+1][y][2];
			g_points[44][y][2] = tmp;
		}
		g_wiggle = 0;
	}

	g_wiggle++;
	g_xrot += 0.3f;
	g_yrot += 0.2f;
	g_zrot += 0.4f;

    // Swap The Buffers To Become Our Rendering Visible
    glutSwapBuffers ( );
}

// Our Reshaping Handler (Required Even In Fullscreen-Only Modes)
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);     // Select The Projection Matrix
	glLoadIdentity();                // Reset The Projection Matrix
	// Calculate The Aspect Ratio And Set The Clipping Volume
	if (h == 0) h = 1;
	gluPerspective(45.0f, (float)w/(float)h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);      // Select The Modelview Matrix
	glLoadIdentity(); // Reset The Modelview Matrix
}

// Our Keyboard Handler (Normal Keys)
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 27:        // When Escape Is Pressed...
			exit(0);    // Exit The Program
		break;          // Ready For Next Case
		default:
		break;
	}
}

// Our Keyboard Handler For Special Keys (Like Arrow Keys And Function Keys)
void special_keys(int a_keys, int x, int y)
{
	switch (a_keys) {
		case GLUT_KEY_F1:
			// We Can Switch Between Windowed Mode And Fullscreen Mode Only
			if (!g_gamemode) {
				g_fullscreen = !g_fullscreen;       // Toggle g_fullscreen Flag
				if (g_fullscreen) glutFullScreen(); // We Went In Fullscreen Mode
				else glutReshapeWindow(500, 500);   // We Went In Windowed Mode
			}
		break;
		default:
		break;
	}
}

// Main Function For Bringing It All Together.
int main(int argc, char** argv)
{
	glutInit(&argc, argv);                           // GLUT Initializtion
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE); // (CHANGED)
	if (g_gamemode) {
		glutGameModeString("640x480:16");            // Select The 640x480 In 16bpp Mode
		if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
			glutEnterGameMode();                     // Enter Full Screen
		else g_gamemode = false;                     // Cannot Enter Game Mode, Switch To Windowed
	}
	if (!g_gamemode) {
		glutInitWindowSize(500, 500);                // Window Size If We Start In Windowed Mode
		glutCreateWindow("NeHe's OpenGL Framework"); // Window Title 
	}
	init();
	glutDisplayFunc(render);                     // Register The Display Function
	glutReshapeFunc(reshape);                    // Register The Reshape Handler
	glutKeyboardFunc(keyboard);                  // Register The Keyboard Handler
	glutSpecialFunc(special_keys);               // Register Special Keys Handler
	glutIdleFunc(render);                        // We Do Rendering In Idle Time
	glutMainLoop();                              // Go To GLUT Main Loop
	return 0;
}
