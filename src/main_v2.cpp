#define UNICODE
#define _UNICODE

#include <windows.h>
#include <gl/gl.h>

void drawScene(HDC* hDC);
void EnableOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC);
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR pCmdLine,
    int nCmdShow
)
{
    // Register the window class
    // -------------------------
    const wchar_t *CLASS_NAME = L"GLSample";

    WNDCLASS wc;

    MSG msg;

    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window
    // -----------------

    HWND hWnd = CreateWindowEx(
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class
        L"OpenGL Sample",    // Window header
        WS_OVERLAPPEDWINDOW,            // Window style
        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 512, 512,
        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hWnd == NULL)
    {
        return 0;
    }

    ShowWindow(hWnd, nCmdShow);

    // enable OpenGL for the window
    // ----------------------------

    HDC hDC;
    HGLRC hRC;

    EnableOpenGL(hWnd, &hDC, &hRC);

    // Run the message loop
    // --------------------
    
    while (TRUE)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            drawScene(&hDC);
        }
    }

    // shutdown OpenGL
    DisableOpenGL(hWnd, hDC, hRC);

    // destroy the window explicitly
    DestroyWindow(hWnd);

    return 0;

}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    RECT rcClient;

    switch (uMsg)
    {
    case WM_CREATE:
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    
    case WM_SIZE:   // main window changed size 

        // Get the dimensions of the main window's client 
        // area, and enumerate the child windows. Pass the 
        // dimensions to the child windows during enumeration. 

        GetClientRect(hWnd, &rcClient);

        int w = rcClient.right - rcClient.left;  // 获得客户区宽度
        int h = rcClient.bottom - rcClient.top;  // 获得客户区高度
        glViewport(0, 0, w, h);

        return 0;

    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


void drawScene(HDC* hDC)
{
    /* OpenGL animation code goes here */

    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //画一个矩形面
    glColor4f(1.0f, 0.0f, 0.0f, 0.0f);
    //  glColor3ub(255, 0, 255);
    glBegin(GL_QUADS);
        glVertex3f(-0.6, -0.6f, 0.0f);
        glVertex3f(0.6, -0.6f, 0.0f);
        glVertex3f(0.6, 0.6f, 0.0f);
        glVertex3f(-0.6, 0.6f, 0.0f);
    glEnd();

    SwapBuffers(*hDC);

    Sleep(1);
};

// Enable OpenGL
// -------------

void EnableOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC)
{
    // get the device context (DC)
    *hDC = GetDC(hWnd);

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),    // size of this pfd  
        1,                                // version number  
        PFD_DRAW_TO_WINDOW |              // support window  
        PFD_SUPPORT_OPENGL |              // support OpenGL  
        PFD_DOUBLEBUFFER,                 // double buffered  
        PFD_TYPE_RGBA,                    // RGBA type  
        24,                               // 24-bit color depth  
        0, 0, 0, 0, 0, 0,                 // color bits ignored  
        0,                                // no alpha buffer  
        0,                                // shift bit ignored  
        0,                                // no accumulation buffer  
        0, 0, 0, 0,                       // accum bits ignored  
        32,                               // 32-bit z-buffer      
        0,                                // no stencil buffer  
        0,                                // no auxiliary buffer  
        PFD_MAIN_PLANE,                   // main layer  
        0,                                // reserved  
        0, 0, 0                           // layer masks ignored  
    };
    
    int iPixelFormat = ChoosePixelFormat(*hDC, &pfd);
    SetPixelFormat(*hDC, iPixelFormat, &pfd);
    
    // create and enable the render context (RC)
    *hRC = wglCreateContext(*hDC);
    wglMakeCurrent(*hDC, *hRC);

}

// Disable OpenGL
// --------------
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
}
