#ifndef HITIMAGEMAKER_CXX
#define HITIMAGEMAKER_CXX

#include "HitImageMaker.h"
//#include "Utils/NDArrayConverter.h"
#include "DataFormat/hit.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

  HitImageMaker::HitImageMaker() { 
    _blur = 3 ;
    _canny_lower = 5.;
    _canny_upper = 10.;
    _kernel = 3;
    //Offset is for defining the boundaries of the Mat object
    _offset = 50;
    _n_bins = 200; 
    }

  std::vector<std::vector<larcv::Point2DArray>> HitImageMaker::GetClusters(const ::larlite::event_hit * ev_hit) {

    _mat_v.clear();
    _canny_v.clear();
    _contour_v.clear();
    
    x_min_v.clear();
    x_max_v.clear();
    y_min_v.clear();
    y_max_v.clear();
    q_max_v.clear();

    _div_x.clear();
    _div_y.clear();


    std::vector<float> q_max_v;
    for(auto const& h : *ev_hit) {

      if(h.Integral()<5.) continue;
      size_t hit_plane = h.WireID().Plane;
      if(hit_plane >= x_min_v.size()) {

	x_min_v.resize(hit_plane+1,4000);
	x_max_v.resize(hit_plane+1,0);
	y_min_v.resize(hit_plane+1,9600);
	y_max_v.resize(hit_plane+1,0);
	q_max_v.resize(hit_plane+1,0);
      }

      float wire = h.WireID().Wire;
      float time = h.PeakTime();
      float  q = h.Integral();
      if( x_min_v[hit_plane] > wire ) x_min_v[hit_plane] = wire ;
      if( x_max_v[hit_plane] < wire ) x_max_v[hit_plane] = wire ;
      if( y_min_v[hit_plane] > time ) y_min_v[hit_plane] = time ;
      if( y_max_v[hit_plane] < time ) y_max_v[hit_plane] = time ;
      if( q_max_v[hit_plane] < q    ) q_max_v[hit_plane] = q;
    }


    //Resize for the correct number of planes
    _div_x.resize(x_max_v.size());
    _div_y.resize(x_max_v.size());

    for(size_t plane=0; plane<x_min_v.size(); ++plane) {
      //std::cout<<"Plane "<<plane<<" : "<<x_min_v[plane]<<" => "<<x_max_v[plane]<< " : "<<y_min_v[plane]<<" => "<<y_max_v[plane]<< std::endl;

      _div_x[plane] = ( x_max_v[plane] - x_min_v[plane] + 2*_offset ) /_n_bins;
      _div_y[plane] = ( y_max_v[plane] - y_min_v[plane] + 2*_offset ) /_n_bins;

      ::cv::Mat mat(_n_bins,_n_bins,CV_8UC1, cvScalar(0.));
      ::cv::Mat canny;
      canny.create(mat.size(),mat.type());
      
      _mat_v.emplace_back(mat);
      _canny_v.emplace_back(canny);
    }
    _contour_v.resize(x_min_v.size());

    for(auto const& h : *ev_hit) {

      if(h.Integral()<5.) continue;
//      int wire = h.WireID().Wire;
//     int time = (int)(h.PeakTime());
      size_t plane = h.WireID().Plane;
      int wire = (h.WireID().Wire - (x_min_v[plane] - _offset))/_div_x[plane];
      int time = ((int)(h.PeakTime()) - (y_min_v[plane] - _offset))/_div_y[plane];
      auto& mat = _mat_v[plane];
      int charge = ((256. * h.Integral() / q_max_v[plane]));

      if(charge>256) charge = 256;

      mat.at<unsigned char>(wire,time) += (unsigned char)(charge);

    }

    for(size_t plane=0; plane<_mat_v.size(); plane++) {

//      std::cout<<"Blur canny: "<<_blur<<", "<<_canny_lower<<", "<<_canny_upper<<std::endl ;
      ::cv::blur( _mat_v[plane], _mat_v[plane], ::cv::Size(_blur,_blur) );
//      ::cv::Canny(_mat_v[plane],_canny_v[plane],_canny_lower,_canny_upper,_kernel);
      ::cv::Canny(_mat_v[plane],_canny_v[plane],0.1,0.2,_kernel);

      //Contours per this plane
      std::vector<std::vector<cv::Point> > cv_contour_v;
      std::vector<cv::Vec4i> cv_hierarchy_v;
 
      ::cv::findContours(_canny_v[plane],cv_contour_v,cv_hierarchy_v,
			 CV_RETR_EXTERNAL,
			 CV_CHAIN_APPROX_SIMPLE);

      _contour_v[plane].reserve(cv_contour_v.size());

      int n_clus = 0;
      // found vector is for the draw block below 
      std::vector<std::vector<cv::Point>> found ;

      //Loop over all contours in whatever plane we're at
      for(size_t c_index=0; c_index<cv_contour_v.size(); ++c_index) {

	auto& cv_contour  = cv_contour_v[c_index];
	auto area = ::cv::contourArea(cv_contour);

        if(area <= 30) continue;
        n_clus++;
        found.push_back(cv_contour);
        //std::cout<<"Area is: "<<area<<std::endl;

        larcv::Point2DArray points;
	points.resize(cv_contour.size());

	for(size_t p_index=0; p_index<cv_contour.size(); ++p_index)
	  points.set(p_index, cv_contour[p_index].x, cv_contour[p_index].y);
 
	 _contour_v[plane].push_back(points); 
        }

       // This block is to visually check on the contours being drawn. Still doesn't match python 
       ::cv::RNG rng(12345);
       if (plane == 0){
         ::cv::Mat drawing = ::cv::Mat::zeros( _canny_v[plane].size(), CV_8UC3 );
         for( int i = 0; i< found.size(); i++ ){
           ::cv::Scalar color = ::cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
           ::cv::drawContours( drawing, found, i, color) ;//, 2, 8, hierarchy, 0, Point() );
          }
       char* source_window = "Source";
       ::cv::imshow( source_window, drawing);
       ::cv::waitKey(0);
        }

      std::cout<<"Number of clus saved for plane : "<<found.size()<<std::endl;
      }

    return _contour_v;
  }

  size_t HitImageMaker::NumContours(const size_t plane) const
  {
    if(plane >= _mat_v.size()) {
      std::cout<<"Invalid plane ID requested..."<<std::endl;
      throw std::exception();
    }
    return _contour_v[plane].size();
  }
#endif
