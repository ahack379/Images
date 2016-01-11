#ifndef CONTOURMAKER_CXX
#define CONTOURMAKER_CXX

#include "ContourMaker.h"
//#include "Utils/NDArrayConverter.h"
#include "DataFormat/hit.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

  ContourMaker::ContourMaker() { 
    _blur = 3 ;
    _canny_lower = 5.;
    _canny_upper = 10.;
    _kernel = 3;
    }

  std::vector<std::vector<larcv::Point2DArray>> ContourMaker::GetContours(const ::larlite::event_hit * ev_hit) {
  
    Clear();
    _contour_v.clear();

    FillMat(ev_hit);
    _contour_v.resize(GetMat().size());

    for(size_t plane=0; plane<GetMat().size(); plane++) {


      std::cout<<"Size is : "<< GetMat().size()<<", "<<GetMat()[plane].size()<<std::endl ;

      

      ::cv::blur( GetMat()[plane], GetMat()[plane], ::cv::Size(_blur,_blur) );
      ::cv::Canny( GetMat()[plane],GetCanny()[plane],_canny_lower,_canny_upper,_kernel);

      //auto & mat_v = GetMat()[plane] ;
      //auto & canny_v = GetCanny()[plane] ;

      //Contours per this plane
      std::vector<std::vector<cv::Point> > cv_contour_v;
      std::vector<cv::Vec4i> cv_hierarchy_v;
 
      ::cv::findContours(GetCanny()[plane],cv_contour_v,cv_hierarchy_v,
			 CV_RETR_EXTERNAL,
			 CV_CHAIN_APPROX_SIMPLE);

      _contour_v[plane].reserve(cv_contour_v.size());

      // found vector is for DrawTestPlane function 
      std::vector<std::vector<cv::Point>> found ;

      //Loop over all contours in this plane
      for(size_t c_index=0; c_index<cv_contour_v.size(); ++c_index) {

	auto& cv_contour  = cv_contour_v[c_index];
	auto area = ::cv::contourArea(cv_contour);

        if(area <= 30) continue;

        larcv::Point2DArray points;
	points.resize(cv_contour.size());
        found.push_back(cv_contour);

	for(size_t p_index=0; p_index<cv_contour.size(); ++p_index)
	  points.set(p_index, cv_contour[p_index].x, cv_contour[p_index].y);
 
	 _contour_v[plane].push_back(points); 
        }

      //std::cout<<"Number of clus saved for plane : "<<found.size()<<std::endl;
       //DrawTestPlane( _canny_v[plane],found ) ;
      }

    return _contour_v;
  }

 bool ContourMaker::InContour(std::vector<std::pair<int,int>> points, std::pair<double,double> p, int n_steps)
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

   if (fabs(angle) < pi) 
      return false;
   else
      return true;
   }   

 bool ContourMaker::InContour(larcv::Point2DArray arr, std::pair<float,float> p, int n_steps)
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

 double ContourMaker::Angle2D(double & x1, double & y1, double & x2, double & y2)
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

  size_t ContourMaker::NumContours(const size_t plane) 
  {
    if(plane >= GetMat().size()) {
      std::cout<<"Invalid plane ID requested..."<<std::endl;
      throw std::exception();
    }
    return _contour_v[plane].size();
  }
#endif
