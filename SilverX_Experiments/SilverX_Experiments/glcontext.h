#pragma once
/**
	OpenGLContext is a class designed to store all of your OpenGL functions and keep them
	out of the way of your application logic. Here we have the ability to create an OpenGL
	context on a given window and then render to that window.
*/
class OpenGLContext {
public:
	OpenGLContext(void); // Default constructor
	OpenGLContext(HWND hwnd); // Constructor for creating our context given a hwnd
	~OpenGLContext(void); // Destructor for cleaning up our application
	bool create30Context(HWND hwnd); // Creation of our OpenGL 3.x context
	bool createSharedContext(HWND hwnd, HGLRC sharedRC);
	void setupScene(void); // All scene information can be setup here
	void reshapeWindow(int w, int h); // Method to get our window width and height on resize
	void renderScene(void); // Render scene (display method from previous OpenGL tutorials)
	bool WGLisExtensionSupported(const char *extension);
	bool InitMultisample(HINSTANCE hInstance,HWND hWnd,PIXELFORMATDESCRIPTOR pfd);

	HGLRC getGLRC();

	bool makeCurrent(HWND hWnd);

private:
	int windowWidth; // Store the width of our window
	int windowHeight; // Store the height of our window
	bool arbMultisampleSupported;
	int arbMultisampleFormat;

protected:
	HGLRC hrc; // Rendering context
	HDC hdc; // Device context
	HWND hwnd; // Window identifier
	bool bCreated;
};