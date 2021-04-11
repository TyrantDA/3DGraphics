//OPENGL 3.2 DEMO FOR RENDERING OBJECTS LOADED FROM OBJ FILES

//includes areas for keyboard control, mouse control, resizing the window
//and draws a spinning rectangle

#include <windows.h>		// Header File For Windows
#include "gl/glew.h"
#include "gl/wglew.h"
#pragma comment(lib, "glew32.lib")

#include "shaders/Shader.h"   // include shader header file, this is not part of OpenGL


#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"

Shader* myShader;  ///shader object 
Shader* myBasicShader;

//MODEL LOADING
#include "3DStruct\threeDModel.h"
#include "Obj\OBJLoader.h"
#include "Character.h"
#include "spin.h"

Character sub;
Actor objectTest = Actor(-10, -10, 0);
Actor seaBed1 = Actor(0, 0, 0);
Actor seaBed2 = Actor(0, 0, 0);
Actor seaBed3 = Actor(0, 0, 0);
Actor seaBed4 = Actor(0, 0, 0);
Actor seaWall1 = Actor(0, 0, 0);
Actor seaWall2 = Actor(0, 0, 0);
Actor seaWall3 = Actor(0, 0, 0);
Actor seaWall4 = Actor(0, 0, 0);
Actor seaBox = Actor(0, -1300, 0);
Actor skyBox = Actor(0, 0, 0);
Spin upDown;
Spin leftRight;
float speedOfUpDown;
float speedOfLeftRight;

float amount = 0;
float temp = 0.002f;
float velX = 0, velY = 0, velZ = 0;

ThreeDModel model, modelbox;
OBJLoader objLoader;
///END MODEL LOADING

glm::mat4 ProjectionMatrix; // matrix for the orthographic projection
  // matrix for the modelling and viewing

int	mouse_x=0, mouse_y=0;
int oldMouseX = 0;
float angle = 0.0;
bool LeftPressed = false;
int screenWidth=600, screenHeight=600;
bool keys[256];
float spin=180;
float speed=0;

//OPENGL FUNCTION PROTOTYPES
void display();				//called in winmain to draw everything to the screen
void reshape(int width, int height);				//called when the window is resized
void init();				//called in winmain when the program starts.
void changeShader();
void returnShader();
void processKeys();         //called in winmain to process keyboard input
void update();				//called in winmain to update variables


/*************    START OF OPENGL FUNCTIONS   ****************/
void display()									
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);

	glUseProgram(myShader->handle());  // use the shader

	//create viewMatrix
	Camera::view(sub.getPos(),sub.getRot(),angle);

	//objectTest.draw();

	seaBed1.draw();
	seaBed2.draw();
	seaBed3.draw();
	seaBed4.draw();
	
	skyBox.draw();

	sub.draw();
	glEnable(GL_BLEND);
	seaBox.draw();
	glDisable(GL_BLEND);
	glFlush();
}

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth=width; screenHeight = height;           // to ensure the mouse coordinates match 
														// we will use these values to set the coordinate system

	glViewport(0,0,width,height);						// Reset The Current Viewport

	//Set the projection matrix
	ProjectionMatrix = glm::perspective(60.0f, (GLfloat)screenWidth/(GLfloat)screenHeight, 1.0f, 10000.0f);

	sub.projection(ProjectionMatrix);

	objectTest.projection(ProjectionMatrix);
	seaBed1.projection(ProjectionMatrix);
	seaBed2.projection(ProjectionMatrix);
	seaBed3.projection(ProjectionMatrix);
	seaBed4.projection(ProjectionMatrix);

	seaBox.projection(ProjectionMatrix);
	skyBox.projection(ProjectionMatrix);
}
void init()
{
	glClearColor(1.0,1.0,1.0,0.0);						//sets the clear colour to yellow
														//glClear(GL_COLOR_BUFFER_BIT) in the display function
														//will clear the buffer to this colour
	glEnable(GL_DEPTH_TEST);

	//load shaders
	myShader = new Shader;
    if(!myShader->load("BasicView", "glslfiles/basicTransformations.vert", "glslfiles/basicTransformations.frag"))
	{
		std::cout << "failed to load shader" << std::endl;
	}		

	myBasicShader = new Shader;
	if(!myBasicShader->load("Basic", "glslfiles/basicTransformations.vert", "glslfiles/basicTransformationsWithoutFog.frag"))
	{
		std::cout << "failed to load shader" << std::endl;
	}		

	glUseProgram(myShader->handle());  // use the shader

	glEnable(GL_TEXTURE_2D);

	//load all models
	sub.shader(myShader);
	sub.loadObj("TestModels/sub.obj",true);

	objectTest.shader(myShader);
	objectTest.loadObj("TestModels/box.obj",true);


	seaBed1.shader(myShader);
	seaBed1.loadObj("TestModels/seaBed1.obj", false);

	seaBed2.shader(myShader);
	seaBed2.loadObj("TestModels/seaBed2.obj", false);
	
	seaBed3.shader(myShader);
	seaBed3.loadObj("TestModels/seaBed3.obj", false);

	seaBed4.shader(myShader);
	seaBed4.loadObj("TestModels/seaBed4.obj", false);

	seaBox.shader(myShader);
	seaBox.loadObj("TestModels/seaBox.obj", true);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	skyBox.shader(myShader);
	skyBox.loadObj("TestModels/skyBox2.obj", true);
}

//removes water shader
void changeShader()
{
	sub.shader(myBasicShader);

	objectTest.shader(myBasicShader);

	seaBed1.shader(myBasicShader);

	seaBed2.shader(myBasicShader);

	seaBed3.shader(myBasicShader);

	seaBed4.shader(myBasicShader);

	seaBox.shader(myBasicShader);

	skyBox.shader(myBasicShader);
}

//add water shader back
void returnShader()
{
	sub.shader(myShader);

	objectTest.shader(myShader);

	seaBed1.shader(myShader);

	seaBed2.shader(myShader);

	seaBed3.shader(myShader);

	seaBed4.shader(myShader);

	seaBox.shader(myShader);

	skyBox.shader(myShader);
}

void processKeys()
{
	
	if(keys[VK_UP])
	{
		upDown.setPositiveSpin(true);
	}
	else if (!keys[VK_UP])
	{
		upDown.setPositiveSpin(false);
	}
	if(keys[VK_DOWN])
	{
		upDown.setNeqativeSpin(true);

	}
	else if (!keys[VK_DOWN])
	{
		upDown.setNeqativeSpin(false);
	}
	if(keys[VK_LEFT])
	{
		leftRight.setPositiveSpin(true);
	}
	else if (!keys[VK_LEFT])
	{
		leftRight.setPositiveSpin(false);
	}
	if(keys[VK_RIGHT])
	{
		
		leftRight.setNeqativeSpin(true);
	}
	else if (!keys[VK_RIGHT])
	{
		
		leftRight.setNeqativeSpin(false);
	}
	if(keys[VK_SPACE])
	{
		velZ =  velZ + 0.0001f;
	}
	else if (!keys[VK_SPACE])
	{
		if (velZ > 0)
		{
			velZ = velZ - 0.00005f;
		}
		if (velZ < 0)
		{
			velZ = 0;
		}
	} 


	speedOfUpDown = upDown.spinSet();
	speedOfLeftRight = leftRight.spinSet();
	//cout << speedOfLeftRight << " & " << speedOfUpDown << endl;
}

void update()
{
	
	glm::vec3 frontOfSub(0, 0, 0);
	glm::vec3 posSub(0, 0, 0);
	glm::vec3 forwardVec(0, 0, 0);
	posSub = sub.getPos();
	forwardVec = sub.getForwardVelocity();

	//runs movement of sub
	sub.move(speedOfLeftRight, speedOfUpDown, velZ);

	//remove water shader (blue tint) when above water line
	if (sub.getwaterLevel())
	{
		changeShader();
	}
	else
	{
		returnShader();
	}

	//collsion detection using Point vs. AABB
	frontOfSub = posSub + float(50.0) * forwardVec;
	bool col = seaBed1.collisions(frontOfSub);
	bool col2 = seaBed2.collisions(frontOfSub);
	bool col3 = seaBed3.collisions(frontOfSub);
	bool col4 = seaBed4.collisions(frontOfSub);
	//std::cout << objectTest.getPos().x << ":" << objectTest.getPos().y <<":" << objectTest.getPos().z << " " << frontOfSub.x << ":" << frontOfSub.y << ":" << frontOfSub.z << std::endl;
	if (col || col2 || col3 || col4)
	{
		//std::cout << "in box" << std::endl;
		sub.moveBack();
	}

	/*spin += speed;
	if(spin > 360)
		spin = 0;*/
}
/**************** END OPENGL FUNCTIONS *************************/

//WIN32 functions
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void KillGLWindow();									// releases and destroys the window
bool CreateGLWindow(char* title, int width, int height); //creates the window
int WINAPI WinMain(	HINSTANCE, HINSTANCE, LPSTR, int);  // Win32 main function

//win32 global variabless
HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


/******************* WIN32 FUNCTIONS ***************************/
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	bool	done=false;								// Bool Variable To Exit Loop

	AllocConsole();
	FILE* stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);

	// Create Our OpenGL Window
	if (!CreateGLWindow("OpenGL Win32 Example",screenWidth,screenHeight))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=true;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			if(keys[VK_ESCAPE])
				done = true;

			processKeys();			//process keyboard
			
			display();					// Draw The Scene
			update();					// update variables
			SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
		}
	}

	
	// Shutdown
	KillGLWindow();									// Kill The Window
	return (int)(msg.wParam);						// Exit The Program
}

//WIN32 Processes function - useful for responding to user inputs or other events.
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}
		break;

		case WM_SIZE:								// Resize The OpenGL Window
		{
			reshape(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
		break;

		case WM_LBUTTONDOWN:
			{
	            mouse_x = LOWORD(lParam);          
				mouse_y = screenHeight - HIWORD(lParam);
				LeftPressed = true;
			}
		break;

		case WM_LBUTTONUP:
			{
	            LeftPressed = false;
			}
		break;

		case WM_MOUSEMOVE:
			{
				oldMouseX = mouse_x;

	            mouse_x = LOWORD(lParam);          
				mouse_y = screenHeight  - HIWORD(lParam);
				
				if ((mouse_x - oldMouseX) > 0)             // mouse moved to the right
					angle += 3.0f;
				else if ((mouse_x - oldMouseX) < 0)     // mouse moved to the left
					angle -= 3.0f;
				return 0;
			}
		break;
		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = true;					// If So, Mark It As TRUE
			if (keys[wParam] == keys['C'])
			{
				Camera::changeCamera();
			}
			return 0;								// Jump Back
		}
		break;
		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = false;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}
		break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void KillGLWindow()								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*/
 
bool CreateGLWindow(char* title, int width, int height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}
	
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	HGLRC tempContext = wglCreateContext(hDC);
	wglMakeCurrent(hDC, tempContext);

	glewInit();

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 1,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		0
	};
	
    if(wglewIsSupported("WGL_ARB_create_context") == 1)
    {
		hRC = wglCreateContextAttribsARB(hDC,0, attribs);
		wglMakeCurrent(NULL,NULL);
		wglDeleteContext(tempContext);
		wglMakeCurrent(hDC, hRC);
	}
	else
	{	//It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
		hRC = tempContext;
		cout << " not possible to make context "<< endl;
	}

	//Checking GL version
	const GLubyte *GLVersionString = glGetString(GL_VERSION);

	cout << GLVersionString << endl;

	//OpenGL 3.2 way of checking the version
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

	cout << OpenGLVersion[0] << " " << OpenGLVersion[1] << endl;

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	reshape(width, height);					// Set Up Our Perspective GL Screen

	init();
	
	return true;									// Success
}



