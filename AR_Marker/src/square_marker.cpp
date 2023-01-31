/* **************************************************** square_marker.c *** *
 *
 * ************************************************************************* */
#include <opencv2/opencv.hpp>
#include "square_marker.h"
#include "square.h"

 /*
  * Constructor
  */
SquareMarker::SquareMarker()
{
	this->R = Eigen::MatrixXd::Identity(3, 3);
	this->T = Eigen::VectorXd::Zero(3);
	for (int n = 0; n < 16; n++) this->M[n] = 0;
	this->M[15] = 1;
}

/*
 * Constructor
 *
 * @param [in] square
 */
SquareMarker::SquareMarker(const Square& square) {

	this->marker_square = square;

	this->R = Eigen::MatrixXd::Identity(3, 3);
	this->T = Eigen::VectorXd::Zero(3);
	for (int n = 0; n < 16; n++) this->M[n] = 0;
	this->M[15] = 1;
}

SquareMarker::SquareMarker(const Square& square, const Eigen::MatrixXd homography_mat) {

	this->marker_square = square;
	this->H = homography_mat;

	this->R = Eigen::MatrixXd::Identity(3, 3);
	this->T = Eigen::VectorXd::Zero(3);
	for (int n = 0; n < 16; n++) this->M[n] = 0;
	this->M[15] = 1;
}


/*
 * Destructor
 */
SquareMarker::~SquareMarker() {
	
}

/*
 * 
	@Compute Camera Parameters
 *
 */
void SquareMarker::ComputeCameraParam(Eigen::MatrixXd proj_matrix) {

	this->H = this->H.transpose();

	Eigen::Vector3d h1 = H.col(0);
	Eigen::Vector3d h2 = H.col(1);
	Eigen::Vector3d h3 = H.col(2);

	Eigen::MatrixXd proj_mat_inv = proj_matrix.inverse();
	double Lambda = ((proj_mat_inv * h1).norm());

	Eigen::Vector3d r1 = (Lambda * (proj_mat_inv * h1)).normalized();
	Eigen::Vector3d r2 = (Lambda * (proj_mat_inv * h2)).normalized();
	Eigen::Vector3d r3 = (r1.cross(r2)).normalized();

	T = Lambda * (proj_mat_inv * h3);

	R.col(0) = r1;
	R.col(1) = r2;
	R.col(2) = r3;

	Eigen::JacobiSVD<Eigen::MatrixXd> svd;
	svd.compute(R, Eigen::ComputeFullU | Eigen::ComputeFullV);

	Eigen::MatrixXd U = svd.matrixU();
	Eigen::MatrixXd V = svd.matrixV();

	R = U * V;
	double det = R.determinant();
	if (det < 0) {
		V(2, 0) *= -1;
		V(2, 1) *= -1;
		V(2, 2) *= -1;
		R = U * V;
	}

	std::cout << "R: " << std::endl;
	std::cout << R << std::endl;

	std::cout << "T: " << std::endl;
	std::cout << T << std::endl;
}

void SquareMarker::Convert_to_OpenGL() {
	Eigen::MatrixXd R_ = Eigen::Matrix3d::Zero(3, 3);
	R_(0, 1) = 1.0;
	R_(1, 0) = 1.0;
	R_(2, 2) = -1.0;

	// Open_GL
	R = R_ * R;
	T = R_ * T;

	M[0] = R(0, 0); M[1] = R(1, 0); M[2] = R(2, 0);
	M[4] = R(0, 1); M[5] = R(1, 1); M[6] = R(2, 1);
	M[8] = R(0, 2); M[9] = R(1, 2); M[10] = R(2, 2);
	M[12] = T(0); M[13] = T(1); M[14] = T(2);
}

/* ********************************************* End of cicular_marker.c *** */
