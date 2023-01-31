/* ******************************************************* application.h *** *
 * Application
 * ************************************************************************* */
#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

#include <Eigen/Dense>
#include "camera.h"
#include "glfw_window.h"

#include "square.h"
#include "square_detection.h"
#include "square_marker.h"
#include "square_marker_detection.h"

#include "metasequoia.h"

typedef struct _GLProjectionParam {
    double horiz;
    double vert;
    double nearDist;
    double farDist;
} GLProjectionParam;

class Application {
    public:
        // Constructor
        Application(const char* title);

        // Destructor
        ~Application();

        // Camera Methods
        void ReadSettings(const std::string& filename);
        bool OpenCamera(void);

        // Marker Detection Methods
        bool MarkerDetect(void);

        // Member Data
        CCamera camera;                          // Camera
        GLFWWindow window;                      // Window App

        Eigen::MatrixXd A;                      // Projection Matrix
        GLProjectionParam proj_param;           // Projection Parameter - OpenGL

        std::vector<SquareMarker> marker_list;  // List of detected markers

        Metasequoia model;                      // 3D Model for virtual object
        char model_filename[1024];

    private:  
        const int DRAW_INPUT      = 0;
        const int IMAGE_WIDTH     = 640;
        const int IMAGE_HEIGHT    = 480;  
        const int IMAGE_U0        = 320;
        const int IMAGE_V0        = 240;
        const double CAMERA_FOCUS = 700;
        const double DEFAULT_SCALE = 0.005;
        const double DEFAULT_FAR_SCALE = 1.0e+6;
  

        cv::Mat image;     // Input Image
        int drawMode;      // 
        double focus;      // Focal length of camera

        // Coordinate of Screen Center(u0, v0)
        double u0;         
        double v0;

        // Square Detection
        SquareDetection			square_detector;
        std::vector<Square>       square_list;

        // Marker Detection
        SquareMarkerDetection     marker_detector;

        // 3D Model
        double model_scale;
};

/* ************************************************ End of application.h *** */
