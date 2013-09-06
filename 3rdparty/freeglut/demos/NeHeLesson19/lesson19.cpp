/*
 NeHe (nehe.gamedev.net) OpenGL tutorial series
 GLUT port.in 2001 by milix (milix_gr@hotmail.com)
 Most comments are from the original tutorials found in NeHe.
 For VC++ users, create a Win32 Console project and link 
 the program with glut32.lib, glu32.lib, opengl32.lib
*/

#include <stdio.h>           // Standard C/C++ Input-Output
#include <stdlib.h>          // Standard C/C++ Library
#include <string.h>          // Standard C/C++ Library
#include <gl/glut.h>         // The GL Utility Toolkit (GLUT) Header

#define WCX           640    // Window Width
#define WCY           480    // Window Height
#define TEXTURES_NUM  1      // We Have 1 Texture 
#define	MAX_PARTICLES 1000   // Number Of Particles To Create

// A Structure For RGB Bitmaps
typedef struct _RGBIMG {
	GLuint   w;    // Image's Width
	GLuint   h;    // Image's Height
	GLubyte* data; // Image's Data (Pixels)
} RGBIMG;

// Global Variables
bool    g_gamemode;            // GLUT GameMode ON/OFF
bool    g_fullscreen;          // Fullscreen Mode ON/OFF (When g_gamemode Is OFF)
bool    g_keys[256];           // Keys Array
bool    g_rainbow=true;
float	g_slowdown = 2.0f;     // Slow Down Particles
float   g_xspeed = 0.0f;	   // X Rotation Speed 
float   g_yspeed = 0.0f;	   // Y Rotation Speed 
float	g_zoom = -40.0f;       // Used To Zoom Out
GLuint	g_col = 0;             // Current Color Selection
GLuint	g_delay = 0;           // Rainbow Effect Delay
GLuint	g_texid[TEXTURES_NUM]; // Our Textures' Id List 

typedef struct {               // Create A Structure For Particle
	bool  active;              // Active (Yes/No)
	float life;                // Particle Life
	float fade;                // Fade Speed
	float r;                   // Red Value
	float g;                   // Green Value
	float b;                   // Blue Value
	float x;                   // X Position
	float y;                   // Y Position
	float z;                   // Z Position
	float xi;                  // X Direction
	float yi;                  // Y Direction
	float zi;                  // Z Direction
	float xg;                  // X Gravity
	float yg;                  // Y Gravity
	float zg;                  // Z Gravity
} particles;                   // Particles Structure

particles g_particle[MAX_PARTICLES]; // Particle Array (Room For Particle Info)

static GLfloat COLORS[12][3] = {     // Rainbow Of Colors
	{1.0f,0.5f,0.5f},{1.0f,0.75f,0.5f},{1.0f,1.0f,0.5f},{0.75f,1.0f,0.5f},
	{0.5f,1.0f,0.5f},{0.5f,1.0f,0.75f},{0.5f,1.0f,1.0f},{0.5f,0.75f,1.0f},
	{0.5f,0.5f,1.0f},{0.75f,0.5f,1.0f},{1.0f,0.5f,1.0f},{1.0f,0.5f,0.75f}
};

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
	if (fsize != (long)sz) {
		fclose(file);
		return false;
	}
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
	if (!load_rgb_image("particle_32x32.raw", 32, 32, &img)) return false;
	// Create Linear Filtered Texture
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
	int i;
	if (!setup_textures()) return false;
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f,0.0f,0.0f,0.0f);					// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glDisable(GL_DEPTH_TEST);							// Disable Depth Testing
	glEnable(GL_BLEND);									// Enable Blending
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Type Of Blending To Perform
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);	// Really Nice Perspective Calculations
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);				// Really Nice Point Smoothing
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glBindTexture(GL_TEXTURE_2D,g_texid[0]);			// Select Our Texture
	for (i=0 ; i<MAX_PARTICLES ; i++) {
		g_particle[i].active = true;							// Make All The Particles Active
		g_particle[i].life = 1.0f;								// Give All The Particles Full Life
		g_particle[i].fade = float(rand()%100)/1000.0f+0.003f;	// Random Fade Speed
		g_particle[i].r  = COLORS[(i+1)/(MAX_PARTICLES/12)][0];	// Select Red Rainbow Color
		g_particle[i].g  = COLORS[(i+1)/(MAX_PARTICLES/12)][1];	// Select Green Rainbow Color
		g_particle[i].b  = COLORS[(i+1)/(MAX_PARTICLES/12)][2];	// Select Blue Rainbow Color
		g_particle[i].xi = float((rand()%50)-26.0f)*10.0f;		// Random Speed On X Axis
		g_particle[i].yi = float((rand()%50)-25.0f)*10.0f;		// Random Speed On Y Axis
		g_particle[i].zi = float((rand()%50)-25.0f)*10.0f;		// Random Speed On Z Axis
		g_particle[i].xg = 0.0f;								// Set Horizontal Pull To Zero
		g_particle[i].yg = -0.8f;								// Set Vertical Pull Downward
		g_particle[i].zg = 0.0f;								// Set Pull On Z Axis To Zero
	}
	memset(g_keys,0,sizeof(g_keys));
	return true;
}

// Our Rendering Is Done Here
void render(void)   
{
	int i;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	glLoadIdentity();										// Reset The ModelView Matrix

	for (i=0 ; i<MAX_PARTICLES ; i++) {						// Loop Through All The Particles
		if (g_particle[i].active) {							// If The Particle Is Active
			float x=g_particle[i].x;						// Grab Our Particle X Position
			float y=g_particle[i].y;						// Grab Our Particle Y Position
			float z=g_particle[i].z+g_zoom;				// Particle Z Pos + Zoom

			// Draw The Particle Using Our RGB Values, Fade The Particle Based On It's Life
			glColor4f(g_particle[i].r,g_particle[i].g,g_particle[i].b,g_particle[i].life);

			glBegin(GL_TRIANGLE_STRIP);							// Build Quad From A Triangle Strip
			    glTexCoord2d(1,1); glVertex3f(x+0.5f,y+0.5f,z); // Top Right
				glTexCoord2d(0,1); glVertex3f(x-0.5f,y+0.5f,z); // Top Left
				glTexCoord2d(1,0); glVertex3f(x+0.5f,y-0.5f,z); // Bottom Right
				glTexCoord2d(0,0); glVertex3f(x-0.5f,y-0.5f,z); // Bottom Left
			glEnd();											// Done Building Triangle Strip

			g_particle[i].x += g_particle[i].xi/(g_slowdown*1000);// Move On The X Axis By X Speed
			g_particle[i].y += g_particle[i].yi/(g_slowdown*1000);// Move On The Y Axis By Y Speed
			g_particle[i].z += g_particle[i].zi/(g_slowdown*1000);// Move On The Z Axis By Z Speed

			g_particle[i].xi += g_particle[i].xg;			// Take Pull On X Axis Into Account
			g_particle[i].yi += g_particle[i].yg;			// Take Pull On Y Axis Into Account
			g_particle[i].zi += g_particle[i].zg;			// Take Pull On Z Axis Into Account
			g_particle[i].life -= g_particle[i].fade;		// Reduce Particles Life By 'Fade'

			if (g_particle[i].life < 0.0f) {				// If Particle Is Burned Out
				g_particle[i].life = 1.0f;					// Give It New Life
				g_particle[i].fade = float(rand()%100)/1000.0f+0.003f;	// Random Fade Value
				g_particle[i].x = 0.0f;						// Center On X Axis
				g_particle[i].y = 0.0f;						// Center On Y Axis
				g_particle[i].z = 0.0f;						// Center On Z Axis
				g_particle[i].xi = g_xspeed+float((rand()%60)-32.0f); // X Axis Speed And Direction
				g_particle[i].yi = g_yspeed+float((rand()%60)-30.0f); // Y Axis Speed And Direction
				g_particle[i].zi = float((rand()%60)-30.0f);// Z Axis Speed And Direction
				g_particle[i].r = COLORS[g_col][0];			// Select Red From Color Table
				g_particle[i].g = COLORS[g_col][1];			// Select Green From Color Table
				g_particle[i].b = COLORS[g_col][2];			// Select Blue From Color Table
			}

			// If Number Pad 8 And Y Gravity Is Less Than 1.5 Increase Pull Upwards
			if (g_keys['8'] && (g_particle[i].yg<1.5f)) g_particle[i].yg += 0.01f;

			// If Number Pad 2 And Y Gravity Is Greater Than -1.5 Increase Pull Downwards
			if (g_keys['2'] && (g_particle[i].yg>-1.5f)) g_particle[i].yg -= 0.01f;

			// If Number Pad 6 And X Gravity Is Less Than 1.5 Increase Pull Right
			if (g_keys['6'] && (g_particle[i].xg<1.5f)) g_particle[i].xg += 0.01f;

			// If Number Pad 4 And X Gravity Is Greater Than -1.5 Increase Pull Left
			if (g_keys['4'] && (g_particle[i].xg>-1.5f)) g_particle[i].xg -= 0.01f;

			if (g_keys['\t']) {										// Tab Key Causes A Burst
				g_particle[i].x = 0.0f;								// Center On X Axis
				g_particle[i].y = 0.0f;								// Center On Y Axis
				g_particle[i].z = 0.0f;								// Center On Z Axis
				g_particle[i].xi = float((rand()%50)-26.0f)*10.0f;	// Random Speed On X Axis
				g_particle[i].yi = float((rand()%50)-25.0f)*10.0f;	// Random Speed On Y Axis
				g_particle[i].zi = float((rand()%50)-25.0f)*10.0f;	// Random Speed On Z Axis
			}
		}
    }

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
	gluPerspective(45, (float)w/(float)h, 0.1, 200.0);
	glMatrixMode(GL_MODELVIEW);      // Select The Modelview Matrix
	glLoadIdentity(); // Reset The Modelview Matrix
}

void game_function(void)
{
	if (g_keys[GLUT_KEY_PAGE_UP])	g_zoom += 0.1f;	// Zoom In
	if (g_keys[GLUT_KEY_PAGE_DOWN])	g_zoom -= 0.1f;	// Zoom Out
	// If Up Arrow And Y Speed Is Less Than 200 Increase Upward Speed
	if (g_keys[GLUT_KEY_UP] && (g_yspeed < 200)) g_yspeed += 1.0f;
	// If Down Arrow And Y Speed Is Greater Than -200 Increase Downward Speed
	if (g_keys[GLUT_KEY_DOWN] && (g_yspeed > -200)) g_yspeed -= 1.0f;
	// If Right Arrow And X Speed Is Less Than 200 Increase Speed To The Right
	if (g_keys[GLUT_KEY_RIGHT] && (g_xspeed < 200)) g_xspeed += 1.0f;
	// If Left Arrow And X Speed Is Greater Than -200 Increase Speed To The Left
	if (g_keys[GLUT_KEY_LEFT] && (g_xspeed > -200)) g_xspeed -= 1.0f;
	if (g_keys[' '] || (g_rainbow && (g_delay>25))) { // Space Or Rainbow Mode
		if (g_keys[' ']) g_rainbow = false;	// If Spacebar Is Pressed Disable Rainbow Mode
		g_delay = 0;						// Reset The Rainbow Color Cycling Delay
		g_col++;							// Change The Particle Color
		if (g_col > 11) g_col = 0;			// If Color Is To High Reset It
	}
	// Increase Rainbow Mode Color Cycling Delay Counter
	g_delay++;					

	// Do The Rendering
	render();
}

// Our Keyboard Handler (Normal Keys)
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 27:        
			exit(0);
		break;
		case '+':
			if (g_slowdown > 1.0f) g_slowdown -= 0.01f;	// Speed Up Particles
		break;
		case '-':
			if (g_slowdown < 4.0f) g_slowdown += 0.01f;	// Slow Down Particles
		break;
		case '\n':
			g_rainbow = !g_rainbow;
		break;
		default:
			g_keys[key] = true;
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
				else glutReshapeWindow(WCX, WCY);   // We Went In Windowed Mode
			}
		break;
		default:
			g_keys[a_keys] = true;
		break;
	}
}

// Our Keyboard Handler For Special Key Releases.
void special_keys_up(int key, int x, int y)
{
	g_keys[key] = false;
}

// Our Keyboard Handler For Normal Key Releases.
void normal_keys_up(unsigned char key, int x, int y)
{
	g_keys[key] = false;
}

// Main Function For Bringing It All Together.
int main(int argc, char** argv)
{
	glutInit(&argc, argv);                           // GLUT Initializtion
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE); 
	if (g_gamemode) {
		glutGameModeString("640x480:16");            // Select The 640x480 In 16bpp Mode
		if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
			glutEnterGameMode();                     // Enter Full Screen
		else g_gamemode = false;                     // Cannot Enter Game Mode, Switch To Windowed
	}
	if (!g_gamemode) {
		glutInitWindowSize(WCX, WCY);                // Window Size If We Start In Windowed Mode
		glutCreateWindow("NeHe's OpenGL Framework"); // Window Title 
	}
	if (!init()) {                                   // Our Initialization
		//MessageBox(NULL,"Cannot initialize textures.","Error",MB_ICONSTOP);
		return -1;
	}
	glutDisplayFunc(render);                         // Register The Display Function
	glutReshapeFunc(reshape);                        // Register The Reshape Handler
	glutKeyboardFunc(keyboard);                      // Register The Keyboard Handler
	glutSpecialFunc(special_keys);                   // Register Special Keys Handler
	glutKeyboardUpFunc(normal_keys_up);              // Called When A Normal Key Released 
	glutSpecialUpFunc(special_keys_up);              // Called When A Special Key Released
	glutIdleFunc(game_function);                     // We Render In Idle Time
	glutMainLoop();                                  // Go To GLUT Main Loop
	return 0;
}
