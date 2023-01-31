/* ****************************************** square_marker_detection.c *** *
 * Square Marker Detection
 * ************************************************************************* */
#include "square_marker_detection.h"

 /*
  * Constructor
  */
SquareMarkerDetection::SquareMarkerDetection() {
	drawMarker = false;
}

/*
 * Destructor
 */
SquareMarkerDetection::~SquareMarkerDetection() {
	;
}

Eigen::MatrixXd SquareMarkerDetection::compute_Homography(std::vector<cv::Point> src_points, std::vector<cv::Point> dst_points) {
	double x1 = src_points[0].x, y1 = src_points[0].y, x2 = src_points[1].x, y2 = src_points[1].y;
	double x3 = src_points[2].x, y3 = src_points[2].y, x4 = src_points[3].x, y4 = src_points[3].y;

	double x1_ = dst_points[0].x, y1_ = dst_points[0].y, x2_ = dst_points[1].x, y2_ = dst_points[1].y;
	double x3_ = dst_points[2].x, y3_ = dst_points[2].y, x4_ = dst_points[3].x, y4_ = dst_points[3].y;

	Eigen::MatrixXd P = Eigen::MatrixXd::Zero(8, 9);
	P << -x1, -y1, -1, 0, 0, 0, x1*x1_, y1*x1_, x1_,
		0, 0, 0, -x1, -y1, -1, x1*y1_, y1*y1_, y1_,
		-x2, -y2, -1, 0, 0, 0, x2*x2_, y2*x2_, x2_,
		0, 0, 0, -x2, -y2, -1, x2*y2_, y2*y2_, y2_,
		-x3, -y3, -1, 0, 0, 0, x3*x3_, y3*x3_, x3_,
		0, 0, 0, -x3, -y3, -1, x3*y3_, y3*y3_, y3_,
		-x4, -y4, -1, 0, 0, 0, x4*x4_, y4*x4_, x4_,
		0, 0, 0, -x4, -y4, -1, x4*y4_, y4*y4_, y4_;


	Eigen::JacobiSVD<Eigen::MatrixXd> svd;
	svd.compute(P, Eigen::ComputeFullU | Eigen::ComputeFullV);

	Eigen::MatrixXd h = svd.matrixV().col(8);

	Eigen::MatrixXd homography = Eigen::MatrixXd::Identity(3, 3);
	homography << h(0, 0) / h(8, 0), h(1, 0) / h(8, 0), h(2, 0) / h(8, 0),
					h(3, 0) / h(8, 0), h(4, 0) / h(8, 0), h(5, 0) / h(8, 0),
					h(6, 0) / h(8, 0), h(7, 0) / h(8, 0), h(8, 0) / h(8, 0);

	return homography;
}

/*
 * Square Marker Detection
 * @param [in] ellipse_list
 * @param [in] image
 * @param [out] marker_list
 *
 * @return true if at least a marker is detected, else false
 */
bool SquareMarkerDetection::Detect(const std::vector<Square> &square_list, cv::Mat &image, std::vector<SquareMarker> &marker_list){
	marker_list.clear();

	cv::Mat temp = cv::imread("./Marker.png");
	/*
	int width = temp.rows;
	int height = temp.cols;
	std::cout << width << "x" << height << std::endl;

	int x1 = 0;
	int x2 = width - 1;
	int y1 = 0;
	int y2 = height - 1;
	*/

	int x1 = 20;
	int x2 = 525;
	int y1 = 25;
	int y2 = 528;

	std::vector<cv::Point> temp_points;
	temp_points.push_back(cv::Point(x1, y1));
	temp_points.push_back(cv::Point(x2, y1));
	temp_points.push_back(cv::Point(x2, y2));
	temp_points.push_back(cv::Point(x1, y2));
	
	if (square_list.size() > 0) {
		// Computing Homography Matrix
		Eigen::MatrixXd H = this->compute_Homography(temp_points, square_list[0].point_list);
		std::cout << "H: \n" << H << std::endl;

		// OpenCV matrix to Eigen matrix
		cv::Mat h;
		cv::eigen2cv(H, h);

		// Warp Perspective
		cv::Mat img1_warp;
		cv::warpPerspective(temp, img1_warp, h, temp.size());
		cv::imshow("Warp Perspective", img1_warp);

		SquareMarker marker(square_list[0], H);
		marker_list.push_back(marker);
	}
	else {
		return false;
	}

	if (drawMarker) {
		for (int i = 0; i < marker_list.size(); i++) {
			const cv::Point* p = &marker_list[i].marker_square.point_list[0];

			int n = (int)marker_list[i].marker_square.point_list.size();
			//dont detect the border
			if (p->x > 3 && p->y > 3)
				cv::polylines(image, &p, &n, 1, true, cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
		}
	}
	return true;
}

/* *********************************** End of cicular_marker_detection.c *** */
