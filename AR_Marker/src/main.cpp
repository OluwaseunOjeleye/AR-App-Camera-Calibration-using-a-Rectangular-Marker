#include <opencv2/opencv.hpp>
#include <math.h>
#include <string.h>
#include "application.h"
#include <iostream>
#include <fstream>

// Camera Calibration Information
int data_count = 0;

Eigen::MatrixXd A_total = Eigen::MatrixXd::Zero(3, 3);
Eigen::MatrixXd R_total = Eigen::MatrixXd::Zero(3, 3);
Eigen::VectorXd T_total = Eigen::VectorXd::Zero(3);

static void display (Application& app) {
    // Clear screen for rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    /* ****************************************************************** *
    * Marker Detection
    * ****************************************************************** */
    bool detected = app.MarkerDetect();
  
    // Rendering Image
    int window_width, window_height;
    glfwGetWindowSize(app.window.window, &window_width, &window_height);
    app.window.drawImage ((GLubyte *) app.camera.image.data, app.camera.width, app.camera.height, window_width, window_height);

    // 3D Camera Parameter Computation - For detected marker
    if (detected) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-app.proj_param.horiz, app.proj_param.horiz,
	            -app.proj_param.vert, app.proj_param.vert,
	            app.proj_param.nearDist, app.proj_param.farDist);

        for (int n = 0; n < (int) app.marker_list.size(); n++) {
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            /* ************************************************************* *
            * Computing R, T from Homography Matrix and Rendering 3D Virtual Object
            * ************************************************************* */
            /* Computing  */
            app.marker_list[n].ComputeCameraParam(app.A);

	        data_count++;
	        A_total += app.A;
	        R_total += app.marker_list[n].R;
	        T_total += app.marker_list[n].T;

            /* Converting to OpenGL Coordinate and Loading new matrix */
	        app.marker_list[n].Convert_to_OpenGL();
            glLoadMatrixf(app.marker_list[n].M);

            /* Drawing Model / Virtual Objects */
            app.model.DrawModel();
        }
    }
    glfwSwapBuffers (app.window.window);
}

static void CustomKeyFunc (GLFWwindow*	window,
			   int		key,
			   int		scancode,
			   int		action,
			   int		mods) {
    static int count = 0;

    if (key == GLFW_KEY_Q) {
        glfwTerminate();
        exit(1);
    } 
    else if (key == GLFW_KEY_S) {
        unsigned char* imageBuffer = NULL;
        glReadBuffer (GL_BACK);
        cv::Mat image(cv::Size(640, 480), CV_8UC4);
        glReadPixels (0, 0,
		        640, 480,
		        GL_BGRA_EXT,
		        GL_UNSIGNED_BYTE,
		        image.data);
        char name[1024];
        sprintf_s (name, "output-%03d.png", count++);
        cv::flip (image ,image, 0);
        cv::imwrite (name, image);

        // Saving Data
        std::fstream file;
        file.open("Calibration.txt", std::fstream::out);
        file << "A:" << A_total / data_count << '\n';
        file << "R:" << R_total / data_count << '\n';
        file << "T:" << T_total / data_count << '\n';

        file.close();
        std::cout << "File saved..." << std::endl;
    }
}

int main (int argc, char **argv){
    Application app("AR APP: Camera Calibration using Rectangular Marker");
  
    app.ReadSettings ("./settings.txt");
    if (!app.OpenCamera()) {
	    do
	    {
		    std::cout << '\n' << "Press a key to continue...";
	    } while (std::cin.get() != '\n');
    }

    app.model.OpenModel(app.model_filename); 
    std::cout << "Model Loaded..." << std::endl;

    app.window.SetKeyFunc (CustomKeyFunc);
  
    while (!glfwWindowShouldClose(app.window.window)) {
        display (app);
        glfwWaitEventsTimeout(1e-03);
    }

    return 0;
}
