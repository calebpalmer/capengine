#ifndef POINT2D_H
#define POINT2D_H

//@brief - Class representing a point
//@author - Caleb Palmer

namespace CapEngine {

  class Point2d {

    double x;
    double y;
    
  private:
    //Making copy constructor private
    Point2d(const Point2d&){};
    // make operator= private so that it is not used
    Point2d& operator=(const Point2d&){return *this;}
  
  public:
    Point2d();
    Point2d(double x_in, double y_in);
    double getX() const;
    double getY() const;
    void setX(double x_in);
    void setY(double y_in);
  
  };
}
#endif
