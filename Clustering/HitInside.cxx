#ifndef HITINSIDE_CXX
#define HITINSIDE_CXX

#include "HitInside.h"
#include "math.h"

 int HitInside::InContour(std::vector<std::pair<int,int>> points, std::pair<double,double> p)
  {
     double angle=0;
     double x1, y1, x2, y2; 
     double pi = atan(1)*4 ;

     for (size_t i = 0 ; i < points.size() ; i+=10) {
        x1 = points[i].first  - p.first;
        y1 = points[i].second - p.second;
        x2 = points[(i+10)].first - p.first;
        y2 = points[(i+10)].second - p.second;
        angle += Angle2D(x1,y1,x2,y2);
     }   

//       std::cout<<"angle is: "<<angle<<std::endl ;

   if (fabs(angle) < 1.1*pi) 
      return false;
   else
      return true;
   }   

 int HitInside::InContour(larcv::Point2DArray arr, std::pair<float,float> p)
  {
     double angle=0;
     double x1, y1, x2, y2; 
     double pi = atan(1)*4 ;

//    std::cout<<"Size of contour : "<<arr.size()<<std::endl ;
    
     for (size_t i = 0 ; i < arr.size()-10 ; i+=10) {
        x1 = arr.point(i).x - p.first;
        y1 = arr.point(i).y - p.second;
        x2 = arr.point(i+10).x - p.first;
        y2 = arr.point(i+10).y - p.second;
        angle += Angle2D(x1,y1,x2,y2);
//        std::cout<<"x1, x2, y1, y2 : "<<x1<<", "<<x2<<", "<<y1<<", "<<y2 <<std::endl; 
     }   

//     if( angle > 0.00001*pi )
  //     std::cout<<"angle is: "<<angle<<std::endl ;

   if (fabs(angle) < 0.9*pi) 
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
