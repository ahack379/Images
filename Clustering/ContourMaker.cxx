#ifndef CONTOURMAKER_CXX
#define CONTOURMAKER_CXX

#include "ContourMaker.h"
//#include "Utils/NDArrayConverter.h"
#include "DataFormat/hit.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

  ContourMaker::ContourMaker() { 
    _blur = 3 ;
    _canny_lower = 5;
    _canny_upper = 10;
    _kernel = 3;
    }

  std::vector<std::vector<larcv::Point2DArray>> ContourMaker::GetContours(const ::larlite::event_hit * ev_hit) {
  
    Clear();
    _contour_v.clear();
    _area_v.clear();
    _length_v.clear();
    _height_v.clear();
    _aspectRatio_v.clear();
    _extent_v.clear();

    FillMat(ev_hit);
    _contour_v.resize(GetMat().size());

    for(size_t plane=0; plane<GetMat().size(); plane++) {

      //std::cout<<"Size is : "<< GetMat().size()<<", "<<GetMat()[plane].size()<<std::endl ;
       
      ::cv::Mat out;
      ::cv::Mat cans;

      //::cv::imshow("Before",GetMat()[plane]);
      //::cv::pyrDown(GetMat()[plane],GetMat()[plane],::cv::Size(2,2));

      //Previous blur/edge finder in use
      //::cv::GaussianBlur( GetMat()[plane], out, ::cv::Size(_blur,_blur),3 );
      //::cv::Canny( out,cans,_canny_lower,_canny_upper,_kernel);

      //The blur/edge finder in use
      ::cv::blur( GetMat()[plane],out,::cv::Size(_blur,_blur) );
      ::cv::threshold(out,cans,2,100,CV_THRESH_BINARY);
      //::cv::imshow("After",cans);
      //Contours per this plane
      std::vector<std::vector<cv::Point> > cv_contour_v;
      std::vector<cv::Vec4i> cv_hierarchy_v;
 
      ::cv::findContours(cans,cv_contour_v,cv_hierarchy_v,
			 CV_RETR_EXTERNAL,
			 CV_CHAIN_APPROX_SIMPLE);

      _area_v.reserve(_area_v.size() + cv_contour_v.size()) ;
      _length_v.reserve(_length_v.size() + cv_contour_v.size()) ;
      _height_v.reserve(_height_v.size() + cv_contour_v.size()) ;
      _aspectRatio_v.reserve(_aspectRatio_v.size() + cv_contour_v.size()) ;
      _extent_v.reserve(_extent_v.size() + cv_contour_v.size()) ;

      _contour_v[plane].reserve(cv_contour_v.size());

      // found vector is for DrawTestPlane function 
      std::vector<std::vector<cv::Point>> found ;

      //Loop over all contours in this plane
      for(size_t c_index=0; c_index<cv_contour_v.size(); ++c_index) {

	auto& cv_contour  = cv_contour_v[c_index];
	auto area   = ::cv::contourArea(cv_contour);

        if(area <= 30) continue;

	auto length = ::cv::arcLength(cv_contour,1);
        auto rect = ::cv::boundingRect(cv_contour);
	auto height = ( rect.height > rect.width ? rect.height : rect.width );
//	auto ellipse = ::cv::fitEllipse(cv_contour);
        double aspectR = double(rect.width)/rect.height;
	double extent = double(area)/(rect.width*rect.height) ;
	std::cout<<"Length and width : "<<rect.height<<", "<<rect.width<<std::endl ;

//	for(auto & r : rect.x.size() ) {
//	  std::cout<<"r.x,r.y : "<<rect.x[r]<<", "<<rect.y<<std::endl ;
//	  }

        //Find width of contour at mid-point

        _area_v.push_back(area);
	_length_v.push_back(length);
        _height_v.push_back(height);
        _aspectRatio_v.push_back(aspectR);
	_extent_v.push_back(extent);
        larcv::Point2DArray points;
	points.resize(cv_contour.size());
        found.push_back(cv_contour);
        //std::cout<<"Area is: "<<area<<std::endl ;

	for(size_t p_index=0; p_index<cv_contour.size(); ++p_index)
	  points.set(p_index, cv_contour[p_index].y, cv_contour[p_index].x);
 
	 _contour_v[plane].push_back(points); 
        }

      //std::cout<<"Number of clus saved for plane : "<<found.size()<<std::endl;
      //DrawTestPlane( cans,found ) ;
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

     if (fabs(angle) < 0.8*pi)
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
