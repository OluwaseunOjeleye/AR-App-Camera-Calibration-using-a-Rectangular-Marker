/* ************************************************* square_detection.h *** *
 * Square Detection
 * ************************************************************************* */
#pragma once

#include <opencv2/opencv.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include "square.h"

class SquareDetection{
	public:
		SquareDetection(); 		// Constructor

		~SquareDetection();		// Destructor

		bool Detect(cv::Mat& input, std::vector<Square>& square_list);	// Detect Square

		bool drawSquare;            // Draw Square

	private:
		double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);

};

/* ****************************************** End of square_detection.h *** */
