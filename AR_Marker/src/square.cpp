/* *********************************************************** square.c *** *
 * ************************************************************************* */
#include "square.h"


 /*
  * Constructor
  */
Square::Square() {

}

Square::Square(const std::vector<cv::Point> points) {
	this->point_list = points;
}

/*
 * Destructor
 */
Square::~Square() {

}


/* **************************************************** End of square.c *** */
