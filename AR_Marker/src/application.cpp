/* ******************************************************* application.c *** *
 * Application
 * ************************************************************************* */
#include "application.h"
#include <fstream>

/*
 * Constructor
 */
Application::Application(const char* title) {
    // Camera default settings
    camera.deviceID = 0;
    camera.width    = IMAGE_WIDTH;
    camera.height   = IMAGE_HEIGHT;
  
    focus    = CAMERA_FOCUS;
    u0       = IMAGE_U0;
    v0       = IMAGE_V0;      

    proj_param.horiz = (camera.width  / 2.0) * DEFAULT_SCALE;
    proj_param.vert  = (camera.height / 2.0) * DEFAULT_SCALE;
    proj_param.nearDist  = focus * DEFAULT_SCALE;
    proj_param.farDist   = proj_param.nearDist * DEFAULT_FAR_SCALE;
    
    // Creating window for drawing
    window.MakeContext (camera.width, camera.height, title);
  
    drawMode = DRAW_INPUT;

    // Projection matrix
    A = Eigen::MatrixXd::Zero(3, 3);
    A << 0.0, focus, u0, -focus, 0.0, v0, 0.0, 0.0, 1.0;

    // Detector default settings
    marker_detector.drawMarker = true;
    square_detector.drawSquare = false;
}

/*
 * Destructor
 */
Application::~Application() {
    camera.Close();
}

/*!
 * @brief  Read Settings.txt file
 *
 * @param[in] filename
 */
void Application::ReadSettings(const std::string& filename){
    std::ifstream ifs(filename);
    if (!ifs.fail()) {
        ifs >> camera.deviceID;
        ifs >> camera.width >> camera.height;
        ifs >> focus;
        ifs >> model_filename;
        ifs >> model.scale;
        ifs >> marker_detector.length;
        ifs.close();
    } 
    else {
        std::cout << "Setting file open error." << std::endl;
    }
    u0 = camera.width  / 2.0;
    v0 = camera.height / 2.0;

    A << 0.0, focus, u0, -focus, 0.0, v0, 0.0, 0.0, 1.0;
    //A << focus, 0.0, u0, 0.0, -focus, v0, 0.0, 0.0, 1.0;
    std::cout << A << std::endl;
  
    proj_param.horiz = (camera.width  / 2.0) * DEFAULT_SCALE;
    proj_param.vert  = (camera.height / 2.0) * DEFAULT_SCALE;
    proj_param.nearDist  = focus * DEFAULT_SCALE;
    proj_param.farDist   = proj_param.nearDist * DEFAULT_FAR_SCALE;

    glfwSetWindowSize (window.window, camera.width, camera.height);
    glViewport(0, 0, camera.width, camera.height);  
}

/*!
 * @brief  Open camera
 *
 * @retval  True of False
 */
bool Application::OpenCamera(void){
    int channel = 3;
    return camera.Open(camera.width, camera.height, channel);
}

/*!
 * @brief Marker Detector
 */
bool Application::MarkerDetect(void) {
    camera.CaptureImage();

    bool retval = square_detector.Detect(camera.image, square_list);        // Detect Square
    if (retval) {
        retval = marker_detector.Detect(square_list, camera.image, marker_list);    // Detect Marker
    }
    return retval;
}
/* ************************************************ End of application.c *** */
