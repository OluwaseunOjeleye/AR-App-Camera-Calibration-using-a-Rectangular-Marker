/* ************************************************* ellipse_detection.c *** *
 * SQUARE DETECTION - 
 * ************************************************************************* */
#include <opencv2/features2d/features2d.hpp>
#include "square_detection.h"
#include "square.h"

 /*
  * Constructor
  */
SquareDetection::SquareDetection() {
	this->drawSquare = false;
}

/*
 * Destructor
 */
SquareDetection::~SquareDetection() {
	
}

/*
 * Square Detection Method
 *
 * @param [in] image         
 * @param [out] square_list
	Github /alyssaq/opencv/blob/master/squares.cpp
 *
 * @return true if at least square exists, false
 */

double SquareDetection::angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1 * dy2) / sqrt((dx1*dx1 + dy1 * dy1)*(dx2*dx2 + dy2 * dy2) + 1e-10);
}

bool SquareDetection::Detect(cv::Mat& image, std::vector<Square>& square_list) {

	/*
		Detector - Detect edges edges of the image, detect lines,
		get four corner points from intersection of lines,
		and get midpoint of four corner point

		ellipse_list.push_back(midpoint); - add candidate midpoints to this list
	*/


	int thresh = 300;
	square_list.clear();

	// blur will enhance edge detection
	cv::Mat timg;
	cv::medianBlur(image, timg, 9);
	cv::Mat gray0(timg.size(), CV_8U), gray;

	std::vector<std::vector<cv::Point>> contours;

	// find squares in every color plane of the image
	for (int c = 0; c < 3; c++)
	{
		int ch[] = { c, 0 };
		cv::mixChannels(&timg, 1, &gray0, 1, ch, 1);

		// apply Canny. Take the upper threshold from slider
		// and set the lower to 0 (which forces edges merging)
		cv::Canny(gray0, gray, 5, thresh, 5);
		// cv::imshow("Canny", gray);

		// dilate canny output to remove potential
		// holes between edge segments
		cv::dilate(gray, gray, cv::Mat(), cv::Point(-1, -1));
		// cv::imshow("Dilation", gray);

		// find contours and store them all as a list
		findContours(gray, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

		std::vector<cv::Point> approx;

		// test each contour
		for (size_t i = 0; i < contours.size(); i++)
		{
			// approximate contour with accuracy proportional
			// to the contour perimeter
			cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

			// square contours should have 4 vertices after approximation
			// relatively large area (to filter out noisy contours)
			// and be convex.
			// Note: absolute value of an area is used because
			// area may be positive or negative - in accordance with the
			// contour orientation
			if (approx.size() == 4 && fabs(cv::contourArea(cv::Mat(approx))) > 3000 && 
				fabs(cv::contourArea(cv::Mat(approx))) < 10000 && cv::isContourConvex(cv::Mat(approx))){
				double maxCosine = 0;

				double total_angle = 0;
				for (int j = 2; j < 5; j++)
				{
					// find the maximum cosine of the angle between joint edges
					double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
					maxCosine = MAX(maxCosine, cosine);

					total_angle += acos(cosine) * 180 / 3.1415;
				}

				// if cosines of all angles are small
				// (all angles are ~90 degree) then write quandrange
				// vertices to resultant sequence
				if (maxCosine < 0.3) {
					square_list.push_back(Square(approx));
					/*
					double maxCosine = 0;

					double total_angle = 0;
					for (int j = 2; j < 5; j++)
					{
						// find the maximum cosine of the angle between joint edges
						double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
						maxCosine = MAX(maxCosine, cosine);

						total_angle += acos(cosine) * 180 / 3.1415;
					}
					std::cout << "Angle in degree: " << total_angle << std::endl;
					*/
				}
			}
		}
	}


	if (drawSquare) {
		for (int i = 0; i < square_list.size(); i++) {
			const cv::Point* p = &square_list[i].point_list[0];

			int n = (int)square_list[i].point_list.size();
			//dont detect the border
			if (p->x > 3 && p->y > 3)
				cv::polylines(image, &p, &n, 1, true, cv::Scalar(0, 255, 0), 3, cv::LINE_AA);
		}
	}
	return true;
}
