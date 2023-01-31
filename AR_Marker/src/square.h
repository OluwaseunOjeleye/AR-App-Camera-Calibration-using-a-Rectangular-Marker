#pragma once
/* ***************** Square.h *********************************************
 * ************************************************************************* */
#pragma once

#include <opencv2/opencv.hpp>

class Square {
	public:
		Square();
		Square(const std::vector<cv::Point> points);

		~Square();


		std::vector<cv::Point> point_list;
};

/* *************************************************** End of Square.h *** */
