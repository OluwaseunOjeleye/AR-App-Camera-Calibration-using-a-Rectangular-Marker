/* ******************************************************* glfw_window.c *** *
 * GLFW
 * ************************************************************************* */
#include "glfw_window.h"

/*
 * Key Function
 */
static void _keyFunc (GLFWwindow*	window,
		      int		key,
		      int		scancode,
		      int		action,
		      int		mods) {
    if (key == GLFW_KEY_Q) {
        glfwTerminate();
        exit(1);
    }
}

/*
 * Constructor
 */
GLFWWindow::GLFWWindow() {
    if (glfwInit() == GL_FALSE) {
        fprintf (stderr, "GLFW initialization failed.\n");
    }
}

/*
 * Constructor
 *
 * @param [in] width  
 * @param [in] height 
 * @param [in] title  
 */
GLFWWindow::GLFWWindow(int width, int height, char* title) {
    if (glfwInit() == GL_FALSE) {
        fprintf (stderr, "GLFW initialization failed.\n");
    } 
    else {
        window = glfwCreateWindow (width, height, title, NULL, NULL);
        glfwSetKeyCallback(window, _keyFunc);
        glfwMakeContextCurrent (window);
    }
}

/*
 * Destructor
 */
GLFWWindow::~GLFWWindow() {
  glfwTerminate();
}

/*
 * Make Context
 *
 * @param [in] width  
 * @param [in] height 
 * @param [in] title 
 */
void GLFWWindow::MakeContext (int width, int height, const char* title) {
    window = glfwCreateWindow (width, height, title, NULL, NULL);
    glfwSetKeyCallback(window, _keyFunc);
    glfwMakeContextCurrent (window);
}

/*
 * Draw Image
 *
 * @param [in] pixels
 * @param [in] width  
 * @param [in] height 
 */
void GLFWWindow::drawImage (GLubyte* pixels, int image_width, int image_height, int window_width, int window_height) {
    glDisable (GL_DEPTH_TEST);

    glMatrixMode (GL_MODELVIEW);
    glPushMatrix ();
    glLoadIdentity ();

    glMatrixMode (GL_PROJECTION);
    glPushMatrix ();
    glLoadIdentity ();

    glViewport(0, 0, window_width, window_height);
    glOrtho (0.0, (GLdouble) image_width, 0.0, (GLdouble) image_height, -1.0, 1.0);
    glPixelZoom ((double) window_width / image_width, (double) -window_height / image_height);
    glRasterPos2i (0, image_height - 1);
    glDrawPixels (image_width, image_height, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

    glMatrixMode (GL_PROJECTION);
    glPopMatrix ();
    glMatrixMode (GL_MODELVIEW);
    glPopMatrix ();

    glEnable (GL_DEPTH_TEST);
}

/*
 * Draw Axis
 */
void GLFWWindow::drawWAxis(void) {
    glBegin(GL_LINES);
    {
        glColor4f(1.0, 0.0, 0.0, 1.0);
        glVertex3d(300.0, 0.0, 0.0);
        glVertex3d(0.0, 0.0, 0.0);

        glColor4f(0.0, 1.0, 0.0, 1.0);
        glVertex3d(0.0, 300.0, 0.0);
        glVertex3d(0.0, 0.0, 0.0);

        glColor4f(0.0, 0.0, 1.0, 1.0);
        glVertex3d(0.0, 0.0, 300.0);
        glVertex3d(0.0, 0.0, 0.0);
    }
    glEnd();
}

/*
 * Set Keypad Function
 */
void::GLFWWindow::SetKeyFunc(void (*KeyFunc) (GLFWwindow*	window,
					      int		key,
					      int		scancode,
					      int		action,
					      int		mods)) {
  glfwSetKeyCallback(window, KeyFunc);
}

/* ************************************************ End of glfw_window.c *** */
