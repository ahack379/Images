#ifndef HITINSIDE_CXX
#define HITINSIDE_CXX

#include "HitInside.h"
#include "math.h"

 int HitInside::InContour(std::vector<std::pair<int,int>> points, std::pair<double,double> p, int n_steps)
  {
     double angle=0;
     double x1, y1, x2, y2; 
     double pi = atan(1)*4 ;

     if (points.size() < n_steps) n_steps = 1;

     for (size_t i = 0 ; i < points.size()-n_steps ; i+=n_steps) {
        x1 = points[i].first  - p.first;
        y1 = points[i].second - p.second;
        x2 = points[(i+n_steps)].first - p.first;
        y2 = points[(i+n_steps)].second - p.second;
        angle += Angle2D(x1,y1,x2,y2);
     }   

//       std::cout<<"angle is: "<<angle<<std::endl ;

   if (fabs(angle) < pi) 
      return false;
   else
      return true;
   }   

 int HitInside::InContour(larcv::Point2DArray arr, std::pair<float,float> p, int n_steps)
  {
     double angle=0;
     double x1, y1, x2, y2; 
     double pi = atan(1)*4 ;

     if (arr.size() < n_steps) n_steps = 1;

     for (size_t i = 0 ; i < arr.size()-n_steps ; i+=n_steps) {
        x1 = arr.point(i).x - p.first;
        y1 = arr.point(i).y - p.second;
        x2 = arr.point(i+n_steps).x - p.first;
        y2 = arr.point(i+n_steps).y - p.second;
        angle += Angle2D(x1,y1,x2,y2);
     }   

// if( angle > 0.8*pi )
//       std::cout<<"angle is: "<<angle<<std::endl ;

   if (fabs(angle) < 0.5*pi) 
      return false;
   else
      return true;
   }   

 double HitInside::Angle2D(double & x1, double & y1, double & x2, double & y2) 
  {
     double dtheta,theta1,theta2;
     double pi = atan(1)*4 ;
  
     theta1 = atan2(y1,x1);
     theta2 = atan2(y2,x2);
     dtheta = theta2 - theta1;
     while (dtheta > pi) 
        dtheta -= 2*pi ;
     while (dtheta < -pi)
        dtheta += 2*pi ;

  
     return dtheta ;
  }




#endif
