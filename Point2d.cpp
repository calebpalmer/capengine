//@brief - implementation file for the Point2d class
//@author - Caleb Palmer

#include "Point2d.h"

using namespace CapEngine;

Point2d::Point2d() : x(0.0), y(0.0) {}

Point2d::Point2d(double x_in, double y_in) : x(x_in), y(y_in) {}

double Point2d::getX() const{
  return x;
}

double Point2d::getY() const{
  return y;
}

void Point2d::setX(double x_in){
  x = x_in;
}

void Point2d::setY(double y_in){
  y = y_in;
}

