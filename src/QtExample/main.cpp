#include <qmatrix4x4.h>
#include <qtransform.h>

#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

#include "../LibGizmoQt/IGizmo.h"

QMatrix4x4 objectMatrix;
float objMatrix[16];

IGizmo *gizmo = nullptr;

IGizmo *gizmoMove, *gizmoRotate, *gizmoScale;
int screenWidth, screenHeight;

int mousex;
int mousey;
bool mbMouseDown = 0;

HDC	hDC = nullptr;		// Private GDI Device Context
HGLRC hRC = nullptr;		// Permanent Rendering Context
HWND hWnd = nullptr;		// Holds Our Window Handle
HINSTANCE hInstance;		// Holds The Instance Of The Application

bool keys[256];			// Array Used For The Keyboard Routine
bool active = true;		// Window Active Flag Set To true By Default
bool fullscreen = true;	// Fullscreen Flag Set To Fullscreen Mode By Default

GLfloat	xrot;				// X Rotation ( NEW )
GLfloat	yrot;				// Y Rotation ( NEW )
GLfloat	zrot;				// Z Rotation ( NEW )

GLuint texture[1];			// Storage For One Texture ( NEW )

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

int LoadGLTextures()									// Load Bitmaps And Convert To Textures
{
    gizmoMove = CreateMoveGizmo();
    gizmoRotate = CreateRotateGizmo();
    gizmoScale = CreateScaleGizmo();

    gizmo = gizmoMove;

    gizmo->SetEditMatrix(objectMatrix);
    gizmo->SetScreenDimension(screenWidth, screenHeight);
    gizmoMove->SetDisplayScale(1.f);
    gizmoRotate->SetDisplayScale(1.f);
    gizmoScale->SetDisplayScale(1.f);
    return true;										// Return The Status
}

void ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
    if (height == 0)										// Prevent A Divide By Zero By
        height = 1;										// Making Height Equal One

    glViewport(0, 0, width, height);						// Reset The Current Viewport

    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();									// Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
    glLoadIdentity();									// Reset The Modelview Matrix
    gluLookAt(5, 5, 5, 0, 0, 0, 0, 1, 0);

    screenWidth = width; screenHeight = height;

    if (gizmo)
        gizmo->SetScreenDimension(screenWidth, screenHeight);
}

int InitGL()											// All Setup For OpenGL Goes Here
{
    if (!LoadGLTextures())								// Jump To Texture Loading Routine ( NEW )
        return false;									// If Texture Didn't Load Return false

    glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
    glClearDepth(1.0f);									// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
    glDepthMask(1);

    return true;										// Initialization Went OK
}

int DrawGLScene()										// Here's Where We Do All The Drawing
{
    float r = 1.7777777778;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer

    // View Matrix
    QMatrix4x4 viewMatrix(
                -0.5000, -0.2647, 0.8394, 0.0000,
                0.0000, 1.0000, 0.3000, 0.0000,
                -0.8802, 0.1427, -0.5000, 0.0000,
                180.0000, -110.0000, -90.0000, 1.0000);
    float viewMat[16];
    viewMatrix.copyDataTo(viewMat);

    // Projection Matrix
    QMatrix4x4 projectionMatrix/*(
                0.5625, 0.0000, 0.0000, 0.0000,
                0.0000, 1.0000, 0.0000, 0.0000,
                0.0000, 0.0000, -1.0002, -1.0000,
                0.0000, 0.0000, -0.2000, 0.0000)*/;

    projectionMatrix.perspective(45, 1.7777, 1, 0.01);
    //projectionMatrix(0, 0) = 1 / r * tanf(45);
    //projectionMatrix(1, 1) = 1 / tanf(45);
    projectionMatrix(2, 2) = -1.0002;
    projectionMatrix(2, 3) = -1;
    projectionMatrix(3, 2) = -0.2;
    float projMat[16];
    projectionMatrix.copyDataTo(projMat);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixf(projMat);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixf(viewMat);

    glPushMatrix();
    glMultMatrixf(objMatrix);
    glScalef(50, 50, 50);
    glColor4f(0.5f, 0.5f, 0.5f, 1.f);
    glCullFace(GL_CW);
    glEnable(GL_CULL_FACE);
    glBegin(GL_QUADS);

    // Front Face
    glColor4f(0.6f, 0.6f, 0.6f, 1.f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glColor4f(0.6f, 0.6f, 0.6f, 1.f); glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glColor4f(0.6f, 0.6f, 0.6f, 1.f); glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glColor4f(0.6f, 0.6f, 0.6f, 1.f); glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    // Back Face
    glColor4f(0.5f, 0.5f, 0.5f, 1.f); glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glColor4f(0.5f, 0.5f, 0.5f, 1.f); glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glColor4f(0.5f, 0.5f, 0.5f, 1.f); glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glColor4f(0.5f, 0.5f, 0.5f, 1.f); glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    // Top Face
    glColor4f(0.4f, 0.4f, 0.4f, 1.f); glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glColor4f(0.4f, 0.4f, 0.4f, 1.f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glColor4f(0.4f, 0.4f, 0.4f, 1.f); glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glColor4f(0.4f, 0.4f, 0.4f, 1.f); glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    // Bottom Face
    glColor4f(0.6f, 0.6f, 0.6f, 1.f); glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glColor4f(0.6f, 0.6f, 0.6f, 1.f); glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glColor4f(0.6f, 0.6f, 0.6f, 1.f); glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glColor4f(0.6f, 0.6f, 0.6f, 1.f); glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    // Right face
    glColor4f(0.5f, 0.5f, 0.5f, 1.f); glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glColor4f(0.5f, 0.5f, 0.5f, 1.f); glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glColor4f(0.5f, 0.5f, 0.5f, 1.f); glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glColor4f(0.5f, 0.5f, 0.5f, 1.f); glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    // Left Face
    glColor4f(0.4f, 0.4f, 0.4f, 1.f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glColor4f(0.4f, 0.4f, 0.4f, 1.f); glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glColor4f(0.4f, 0.4f, 0.4f, 1.f); glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glColor4f(0.4f, 0.4f, 0.4f, 1.f); glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glEnd();
    glPopMatrix();

    if (gizmo)
    {
        gizmo->SetCameraMatrix(viewMat, projMat);
        gizmo->Draw();
    }

    return true;										// Keep Going
}

void KillGLWindow()										// Properly Kill The Window
{
    if (fullscreen)										// Are We In Fullscreen Mode?
    {
        ChangeDisplaySettings(nullptr, 0);					// If So Switch Back To The Desktop
        ShowCursor(true);								// Show Mouse Pointer
    }

    // Do We Have A Rendering Context?
    if (hRC)
    {
        // Are We Able To Release The DC And RC Contexts?
        if (!wglMakeCurrent(nullptr, nullptr))
            MessageBox(nullptr, L"Release Of DC And RC Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

        // Are We Able To Delete The RC?
        if (!wglDeleteContext(hRC))
            MessageBox(nullptr, L"Release Rendering Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

        hRC = nullptr;										// Set RC To nullptr
    }

    if (hDC && !ReleaseDC(hWnd, hDC))					// Are We Able To Release The DC
    {
        MessageBox(nullptr, L"Release Device Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        hDC = nullptr;										// Set DC To nullptr
    }

    if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
    {
        MessageBox(nullptr, L"Could Not Release hWnd.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        hWnd = nullptr;										// Set hWnd To nullptr
    }

    if (!UnregisterClass(L"OpenGL", hInstance))			// Are We Able To Unregister Class
    {
        MessageBox(nullptr, L"Could Not Unregister Class.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        hInstance = nullptr;									// Set hInstance To nullptr
    }
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
*	title			- Title To Appear At The Top Of The Window				*
*	width			- Width Of The GL Window Or Fullscreen Mode				*
*	height			- Height Of The GL Window Or Fullscreen Mode			*
*	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
*	fullscreenflag	- Use Fullscreen Mode (true) Or Windowed Mode (false)	*/

BOOL CreateGLWindow(char *title, int width, int height, int bits, bool fullscreenflag)
{
    GLuint		PixelFormat;									// Holds The Results After Searching For A Match
    WNDCLASS	wc;												// Windows Class Structure
    DWORD		dwExStyle;										// Window Extended Style
    DWORD		dwStyle;										// Window Style
    RECT		WindowRect;										// Grabs Rectangle Upper Left / Lower Right Values
    WindowRect.left = (long)0;									// Set Left Value To 0
    WindowRect.right = (long)width;								// Set Right Value To Requested Width
    WindowRect.top = (long)0;									// Set Top Value To 0
    WindowRect.bottom = (long)height;							// Set Bottom Value To Requested Height

    fullscreen = fullscreenflag;								// Set The Global Fullscreen Flag

    hInstance = GetModuleHandle(nullptr);						// Grab An Instance For Our Window
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;				// Redraw On Size, And Own DC For Window.
    wc.lpfnWndProc = (WNDPROC)WndProc;							// WndProc Handles Messages
    wc.cbClsExtra = 0;											// No Extra Window Data
    wc.cbWndExtra = 0;											// No Extra Window Data
    wc.hInstance = hInstance;									// Set The Instance
    wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);					// Load The Default Icon
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);				// Load The Arrow Pointer
    wc.hbrBackground = nullptr;									// No Background Required For GL
    wc.lpszMenuName = nullptr;									// We Don't Want A Menu
    wc.lpszClassName = L"OpenGL";								// Set The Class Name

    // Attempt To Register The Window Class
    if (!RegisterClass(&wc))
    {
        MessageBox(nullptr, L"Failed To Register The Window Class.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;											// Return false
    }

    if (fullscreen)												// Attempt Fullscreen Mode?
    {
        DEVMODE dmScreenSettings;								// Device Mode
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Size Of The Devmode Structure
        dmScreenSettings.dmPelsWidth = width;					// Selected Screen Width
        dmScreenSettings.dmPelsHeight = height;					// Selected Screen Height
        dmScreenSettings.dmBitsPerPel = bits;					// Selected Bits Per Pixel
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
            // If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
            if (MessageBox(nullptr, L"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", L"NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
            {
                fullscreen = false;		// Windowed Mode Selected.  Fullscreen = false
            }
            else
            {
                // Pop Up A Message Box Letting User Know The Program Is Closing.
                MessageBox(nullptr, L"Program Will Now Close.", L"ERROR", MB_OK | MB_ICONSTOP);
                return false;									// Return false
            }
        }
    }

    if (fullscreen)												// Are We Still In Fullscreen Mode?
    {
        dwExStyle = WS_EX_APPWINDOW;								// Window Extended Style
        dwStyle = WS_POPUP;										// Windows Style
        ShowCursor(false);										// Hide Mouse Pointer
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
        dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style
    }

    AdjustWindowRectEx(&WindowRect, dwStyle, false, dwExStyle);		// Adjust Window To true Requested Size

    // Create The Window
    if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
                                L"OpenGL",									// Class Name
                                (LPCWCH)title,								// Window Title
                                dwStyle |									// Defined Window Style
                                WS_CLIPSIBLINGS |							// Required Window Style
                                WS_CLIPCHILDREN,							// Required Window Style
                                0, 0,										// Window Position
                                WindowRect.right - WindowRect.left,			// Calculate Window Width
                                WindowRect.bottom - WindowRect.top,			// Calculate Window Height
                                nullptr,									// No Parent Window
                                nullptr,									// No Menu
                                hInstance,									// Instance
                                nullptr)))									// Dont Pass Anything To WM_CREATE
    {
        KillGLWindow();								// Reset The Display
        MessageBox(nullptr, L"Window Creation Error.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;								// Return false
    }

    static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
    {
            sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
            1,											// Version Number
            PFD_DRAW_TO_WINDOW |						// Format Must Support Window
            PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
            PFD_DOUBLEBUFFER,							// Must Support Double Buffering
            PFD_TYPE_RGBA,								// Request An RGBA Format
            static_cast<BYTE>(bits),										// Select Our Color Depth
            0, 0, 0, 0, 0, 0,							// Color Bits Ignored
            0,											// No Alpha Buffer
            0,											// Shift Bit Ignored
            0,											// No Accumulation Buffer
            0, 0, 0, 0,									// Accumulation Bits Ignored
            24,											// 16Bit Z-Buffer (Depth Buffer)
            8,											// No Stencil Buffer
            0,											// No Auxiliary Buffer
            PFD_MAIN_PLANE,								// Main Drawing Layer
            0,											// Reserved
            0, 0, 0										// Layer Masks Ignored
};

    if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
    {
        KillGLWindow();								// Reset The Display
        MessageBox(nullptr, L"Can't Create A GL Device Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;								// Return false
    }

    if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
    {
        KillGLWindow();								// Reset The Display
        MessageBox(nullptr, L"Can't Find A Suitable PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;								// Return false
    }

    if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
    {
        KillGLWindow();								// Reset The Display
        MessageBox(nullptr, L"Can't Set The PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;								// Return false
    }

    if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
    {
        KillGLWindow();								// Reset The Display
        MessageBox(nullptr, L"Can't Create A GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;								// Return false
    }

    if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
    {
        KillGLWindow();								// Reset The Display
        MessageBox(nullptr, L"Can't Activate The GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;								// Return false
    }

    ShowWindow(hWnd, SW_SHOW);						// Show The Window
    SetForegroundWindow(hWnd);						// Slightly Higher Priority
    SetFocus(hWnd);									// Sets Keyboard Focus To The Window
    ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

    if (!InitGL())									// Initialize Our Newly Created GL Window
    {
        KillGLWindow();								// Reset The Display
        MessageBox(nullptr, L"Initialization Failed.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;								// Return false
    }

    return true;									// Success
}

LRESULT CALLBACK WndProc(HWND	hWnd,				// Handle For This Window
                         UINT	uMsg,									// Message For This Window
                         WPARAM	wParam,									// Additional Message Information
                         LPARAM	lParam)									// Additional Message Information
{
    switch (uMsg)									// Check For Windows Messages
    {
    case WM_ACTIVATE:								// Watch For Window Activate Message
    {
        if (!HIWORD(wParam))						// Check Minimization State
        {
            active = true;							// Program Is Active
        }
        else
        {
            active = false;							// Program Is No Longer Active
        }

        return 0;									// Return To The Message Loop
    }

    case WM_SYSCOMMAND:								// Intercept System Commands
    {
        switch (wParam)								// Check System Calls
        {
        case SC_SCREENSAVE:							// Screensaver Trying To Start?
        case SC_MONITORPOWER:						// Monitor Trying To Enter Powersave?
            return 0;								// Prevent From Happening
        }
        break;										// Exit
    }
    case WM_KEYDOWN:
        if (wParam == 0x31)
        {
            gizmo = gizmoMove;
            gizmo->SetLocation(IGizmo::LOCATE_LOCAL);
        }
        else if (wParam == 0x32)
        {
            gizmo = gizmoRotate;
            gizmo->SetLocation(IGizmo::LOCATE_LOCAL);
        }
        else if (wParam == 0x33)
        {
            gizmo = gizmoScale;
            gizmo->SetLocation(IGizmo::LOCATE_LOCAL);
        }

        gizmo->SetEditMatrix(objMatrix);
        gizmo->SetScreenDimension(screenWidth, screenHeight);
        break;
    case WM_CLOSE:								// Did We Receive A Close Message?
    {
        PostQuitMessage(0);						// Send A Quit Message
        return 0;								// Jump Back
    }
    case WM_MOUSEMOVE:
    {
        mousex = LOWORD(lParam);
        mousey = HIWORD(lParam);
        if (gizmo)
            //gizmo->OnMouseMove( 513, 366 );//mousex, mousey );
            gizmo->OnMouseMove(mousex, mousey);
        return 0;
    }
    case WM_LBUTTONDOWN:
        mbMouseDown = true;
        if (gizmo)
            if (gizmo->OnMouseDown(mousex, mousey))
                SetCapture(hWnd);
        break;
    case WM_LBUTTONUP:
        ReleaseCapture();
        mbMouseDown = false;
        if (gizmo)
            gizmo->OnMouseUp(mousex, mousey);
        break;


    case WM_SIZE:								// Resize The OpenGL Window
    {
        ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
        return 0;								// Jump Back
    }
    }

    // Pass All Unhandled Messages To DefWindowProc
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE	hInstance,			// Instance
                   HINSTANCE	hPrevInstance,		// Previous Instance
                   LPSTR		lpCmdLine,			// Command Line Parameters
                   int			nCmdShow)			// Window Show State
{
    MSG		msg;									// Windows Message Structure
    BOOL	done = false;								// Bool Variable To Exit Loop

    objectMatrix = QMatrix4x4(
                -0.3210, 0.0000, 0.9471, 0.0000,
                0.0000, 1.0000, 0.0000, 0.0000,
                -0.9471, 0.0000, -0.3210, 0.0000,
                -137, 16.5, 300, 1.0000);
    objectMatrix.translate(0, 0, 0);
    objectMatrix.copyDataTo(objMatrix);

    // Ask The User Which Screen Mode They Prefer
    MessageBox(nullptr, L"Press 1/2/3 to change gizmo.", L"Manual", MB_OK);

    fullscreen = false;							// Windowed Mode

    char title[66] = "LibGizmo Test application Base on NeHe's Texture Mapping Tutorial";
    // Create Our OpenGL Window
    if (!CreateGLWindow(title, 1280, 720, 32, fullscreen))
        return 0; // Quit If Window Was Not Created

    while (!done)									// Loop That Runs While done=false
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
        {
            if (msg.message == WM_QUIT)				// Have We Received A Quit Message?
            {
                done = true;							// If So done=true
            }
            else									// If Not, Deal With Window Messages
            {
                TranslateMessage(&msg);				// Translate The Message
                DispatchMessage(&msg);				// Dispatch The Message
            }
        }
        else										// If There Are No Messages
        {
            // Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
            if ((active && !DrawGLScene()) || keys[VK_ESCAPE])	// Active?  Was There A Quit Received?
            {
                done = true;							// ESC or DrawGLScene Signalled A Quit
            }
            else									// Not Time To Quit, Update Screen
            {
                SwapBuffers(hDC);					// Swap Buffers (Double Buffering)
            }

            if (keys[VK_F1])						// Is F1 Being Pressed?
            {
                keys[VK_F1] = false;					// If So Make Key false
                KillGLWindow();						// Kill Our Current Window
                fullscreen = !fullscreen;				// Toggle Fullscreen / Windowed Mode
                // Recreate Our OpenGL Window

                if (!CreateGLWindow(title, 640, 480, 16, fullscreen))
                {
                    return 0;						// Quit If Window Was Not Created
                }
            }
        }
    }

    // Shutdown
    KillGLWindow();									// Kill The Window
    return (msg.wParam);							// Exit The Program
}
