/* **************************************************** square_marker.h *** *
 * Square Marker
 * ************************************************************************* */
#pragma once

#include <opencv2/opencv.hpp>
#include <Eigen/Dense>

#include <opencv2/core/eigen.hpp>

#include "square.h"

class SquareMarker{
	public:
		// Constructors
		SquareMarker();
		SquareMarker(const Square& square);
		SquareMarker(const Square& square, const Eigen::MatrixXd homography_mat);

		// Destructor
		~SquareMarker();

		// Compute Camera Extrinsic Parameters
		void ComputeCameraParam(Eigen::MatrixXd proj_matrix);
		void Convert_to_OpenGL();


		// Important Matrices and vector
		Eigen::MatrixXd H;		// Homography matrix
		Eigen::MatrixXd R;     // 
		Eigen::VectorXd T;     // 
		float           M[16]; //（OpenGL）

		// Square
		Square marker_square;
};

/* ********************************************* End of square_marker.h *** */
