/*!
 * @file	camera.c
 * @brief	Camera Class
 */
#include "camera.h"

/*!
 * @brief  Constructor
 */
CCamera::CCamera (){
    deviceID         = 0;
    inputMode        = INPUT_CAMERA;
    outputMode       = OUTPUT_WINDOW_AND_FILE;
    flipFlag         = false;
    undistortionFlag = false;
    inputFileName    = "";
    outputFileName   = "";
}

/*!
 * @brief Constructor
 *
 * @param[in] _deviceID         
 * @param[in] _inputMode        
 * @param[in] _outputMode        
 * @param[in] _flipFlag        
 * @param[in] _undistortionFlag  
 */
CCamera::CCamera(int _deviceID, int _inputMode, int _outputMode, bool _flipFlag, bool _undistortionFlag){
    deviceID         = _deviceID;
    inputMode        = _inputMode;
    outputMode       = _outputMode;
    flipFlag         = _flipFlag;
    undistortionFlag = _undistortionFlag;
    inputFileName    = "";
    outputFileName   = "";
}

/*!
 * @brief  Destructor
 */
CCamera::~CCamera (){
    Close();
}

/*!
 * @brief　Close Camera
 */
void CCamera::Close (void){
	if (capture.isOpened ()) capture.release ();
}

/*!
 * @brief  Open Camera
 *
 * @param[in]	name		
 * @param[out]	width		
 * @param[out]	height		
 * @param[out]	channels
 *
 * @retval	True or False
 */
bool CCamera::Open (int &w, int &h, int &c){
	if (inputMode == CCamera::INPUT_VIDEO) {
		return OpenVideo(inputFileName, w, h, c);
	}
	else {
		return OpenCamera(w, h, c);
	}
}

/*!
 * @brief　Image Aquisition
 *
 * @param[out]	w	
 * @param[out]	h	
 * @param[out]	c	
 *
 * @retval  True or False   
 */
bool CCamera::OpenCamera (int &w, int &h, int &c){
	inputMode = CCamera::INPUT_CAMERA;

	if (!capture.isOpened ()) {
		if (deviceID != -1) {
			if (!capture.open (deviceID)) {
				fprintf (stderr, "Camera ID %d is not found\n", deviceID);
				return false;
			}
		} 
		else {
			for (int n = 0; n < 5; n++) {
				if (!capture.open (n)) {
					fprintf (stderr, "Camera ID %d is not found\n", n);
				} 
				else {
					fprintf (stdout, "Camera is opened on device ID %d\n", n);
					deviceID = n;
					break;
				}
			}
			if (deviceID == -1) return false;
		}

		// capture image
		// cv::waitKey(2000);		// wait for 2sec webcam to fully establish connection with PC

		capture.set (cv::CAP_PROP_FRAME_WIDTH, w);
		capture.set (cv::CAP_PROP_FRAME_HEIGHT, h);

		// Get Frames
		int count = 0;
		while (image.data == NULL) {
			capture >> image;
			++count;
			if (count > 10) {
				fprintf(stderr, "Cannot retrieve images\n");
				// If the image cannot be obtained with the specified image size,
                // Change the image size and check again
				w = 640;
				h = 480;
				capture.set (cv::CAP_PROP_FRAME_WIDTH, w);
				capture.set (cv::CAP_PROP_FRAME_HEIGHT, h);
				count = 0;
				while (image.data == NULL) {
					capture >> image;
					++count;
					if (count > 10){
						fprintf(stderr, "Cannot retrieve images\n");
						return false;
					}
				}
			}
		}
		width = image.cols;
		height = image.rows;
		channels = c = image.channels();
		imageSize = height * (int) image.step;

		return true;
	}

	return false;
}

/*!
 * @brief  Open Video File
 *
 * @param[in]	name		
 * @param[out]	width		
 * @param[out]	height		
 * @param[out]	channels
 *
 * @retval	True or False
 */
bool CCamera::OpenVideo (const std::string &name, int &w, int &h, int &c){
	inputMode = CCamera::INPUT_VIDEO;
	inputFileName = name;

	bool isOpen = capture.open (inputFileName);
	if (isOpen) {
		capture >> image;
		w = width = image.cols;
		h = height = image.rows;
		c = channels = image.channels ();
		imageSize = height * (int) image.step;
	}
	return isOpen;
}

/*! 
 * @brief  Set Input File
 *
 * @param[in] name  - Filename
 */
void CCamera::SetInputFile (const std::string& name){
  inputMode = INPUT_VIDEO;
  inputFileName = name;
}

/*! 
 * @brief  Get Input File
 * 
 * @return filename
 */
std::string CCamera::GetInputFile (void){
    return inputFileName;
}

/*!
 * @brief　Save Video Settings
 * @param[in]	name	
 * @param[in]	fps		
 * @retval　True or Flase
 */
bool CCamera::SaveVideoSetting (const std::string &name, const float fps){
	bool isOpen;

	isOpen = capture.isOpened ();
	if (isOpen) {	
		outputMode = OUTPUT_WINDOW_AND_VIDEO;
		outputFileName = name;
		bool isColor = (channels == 3) ? true : false;
		return writer.open (outputFileName,	int (NULL), fps, cv::Size(width, height), isColor);
	}
	else {
		return false;
	}
}

/*!
 * @brief  Capture Image
 * @param[out]	img		
 *
 * @retval	True or False
 */
bool CCamera::CaptureImage (void){

    if (capture.isOpened ()) {
        capture >> image;
        if (image.data == NULL) return false;
    } 
    else {
        fprintf (stderr, "Failed to capture\n");
        return false;
    }

    if (undistortionFlag) {
        cv::remap (image.clone (), image, mapx, mapy, cv::INTER_LINEAR);
    }
    if (flipFlag) {
        cv::flip(image.clone(), image, 1);
    }
    return true;
}

/*!
 * @brief Save Image
 *
 * @param[in] name  
 */
void CCamera::SaveImage (const std::string &name){
    if (outputMode == CCamera::OUTPUT_WINDOW_AND_FILE && name != "") {
        cv::imwrite (name, image);
    }
    else {
        writer << image;
    }
}

/*!
 * @brief Load Camera Parameters
 *
 * @param[in]	name		 
 * @param[in]	undistortion
 *
 * @retval	True or False
 */
bool CCamera::LoadParameters (const std::string &name, const bool undistortion){
    cv::FileStorage fs;
    if (!fs.open (name, cv::FileStorage::READ)) {
        fprintf (stderr, "Cannot load camera parameters\n");
        return false;
    }
    //cv::FileNode node (fs.fs, NULL);

    //cv::read(node["A"], internalParams);
    //cv::read(node["D"], distortionParams);

    undistortionFlag = undistortion;

    if (undistortionFlag) {
        cv::initUndistortRectifyMap (internalParams, distortionParams, cv::Mat (), internalParams, image.size (), CV_32FC1, mapx, mapy);
        distortionParams = cv::Mat_<double>::zeros(5, 1);
    }
    return true;
}

/*!
 * @brief  Save Parameters
 *
 * @param[in]	name
 *
 * @retval  True of False
 */
bool CCamera::SaveParameters (const std::string &name) const
{
  if (internalParams.empty () || distortionParams.empty ()) {
    fprintf (stderr, "Empty parameters\n");
    return false;
  }
  cv::FileStorage fs;
  fs.open (name + ".xml", cv::FileStorage::WRITE);
  cv::write (fs, "A", internalParams);
  cv::write (fs, "D", distortionParams);

  fs.release();

  return true;
}

/*!
 * @breif	Get Image Width
 * 
 * @retval	width
 */
int CCamera::GetImageWidth(void){
    return width;
}

/*!
 * @breif	Get Image Height
 *
 * @retval	height
 */
int CCamera::GetImageHeight(void){
    return height;
}

/*! 
 * @breif  Set Device ID
 *
 * @param[in] _deviceID
 */
void CCamera::SetDeviceID(int _deviceID)
{
  deviceID = _deviceID;
  std::cout << "Set devide ID to " << deviceID << std::endl;

}

/*!
 * @brief   Get Device ID
 *
 * retval  deviceID
 */
int  CCamera::GetDeviceID(void)
{
  return deviceID;
}
