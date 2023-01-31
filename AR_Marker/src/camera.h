/*!
 * @file	camera.h
 * @brief	camera class
 */
#pragma once

#include <opencv2/opencv.hpp>

/*!
 * @class  CCamera
 * @brief　Camera
 */
class CCamera{
    public:
        // Constructors
        CCamera ();
        CCamera (int _deviceID, int _inputMode, int _outputMode,
	             bool _flipFlag, bool _undistortionFlag = false);
		
        // Destructors
        ~CCamera ();

        void SetDeviceID (int _deviceID);
        int  GetDeviceID (void);

        // Open Camera and Get Frames
        bool Open (int &w, int &h, int &c);
        bool OpenCamera (int &w, int &h, int &c);
        bool OpenVideo (const std::string &name, int &w, int &h, int &c);

        // Close Camera
        void Close (void);

        // Capture Image
        bool CaptureImage (void);

        // Save Image
        void SaveImage(const std::string &name);

        // Input File
        void SetInputFile(const std::string& name);
        std::string GetInputFile (void);

        // Save Video Settings
        bool SaveVideoSetting(const std::string &name, const float fps = 30.f);

        // Load Parameters
        bool LoadParameters (const std::string &name, const bool undistortion = true);
    
        // Save Parameters 
        bool SaveParameters (const std::string &name) const;

        // Get Width and Height
        int GetImageWidth (void);
        int GetImageHeight (void);

        int deviceID;             // Device ID

        // Image
        cv::Mat image;            // Image data
        int width;
        int height;
        int channels;
        int imageSize;           // (width*height*channels)

    private:
      static const int INPUT_CAMERA = 0;
      static const int INPUT_VIDEO  = 1;

      static const int OUTPUT_WINDOW_AND_FILE  = 0;
      static const int OUTPUT_WINDOW_AND_VIDEO = 1;
  
      int     inputMode;        // Input mode (0: camera, 1: video)
      int     outputMode;       // Output mode (0: image, 1: video)
      bool    flipFlag;         // Flip the image horizontally
      bool    undistortionFlag; // Perform distortion correction 

      cv::VideoCapture capture;
		
      // Image Distortion Correction
      cv::Mat mapx;              
      cv::Mat mapy;              
      cv::Mat internalParams;  
      cv::Mat distortionParams;  

      // Video
      cv::VideoWriter writer;     

      std::string inputFileName;
      std::string outputFileName;
};
