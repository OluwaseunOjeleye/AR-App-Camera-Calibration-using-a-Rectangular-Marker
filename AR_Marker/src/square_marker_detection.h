/* ****************************************** square_marker_detection.h *** *
 *
 * ************************************************************************* */
#pragma once

#include <opencv2/opencv.hpp>
#include "square_marker.h"

class SquareMarkerDetection{
	public:
		// Constructor
		SquareMarkerDetection();

		// Destructor
		~SquareMarkerDetection();

		// Detect
		bool Detect(const std::vector<Square> &square_list, cv::Mat &image, std::vector<SquareMarker> &marker_list);

		bool   drawMarker;  // draw detected square marker
		double length;		// length of square marker 

		private:
			void load_template_image();
			Eigen::MatrixXd compute_Homography(std::vector<cv::Point> src_points, std::vector<cv::Point> dst_points);

			Square template_img;
};

/* *********************************** End of cicular_marker_detection.h *** */
