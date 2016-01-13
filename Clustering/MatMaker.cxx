#ifndef MATMAKER_CXX
#define MATMAKER_CXX

#include "MatMaker.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

void MatMaker::Clear(){

  _mat_v.clear() ;
  _canny_v.clear();
  
  _w_min_v.clear();
  _w_max_v.clear();
  _t_min_v.clear();
  _t_max_v.clear();
  _q_max_v.clear();

  _div_x.clear();
  _div_y.clear();

  }

void MatMaker::FillMat(const ::larlite::event_hit * ev_hit){

  for(auto const& h : *ev_hit) {

    if(h.Integral()<5.) continue;
    size_t hit_plane = h.WireID().Plane;
    if(hit_plane >= _w_min_v.size()) {
      _w_min_v.resize(hit_plane+1,4000);
      _w_max_v.resize(hit_plane+1,0);
      _t_min_v.resize(hit_plane+1,9600);
      _t_max_v.resize(hit_plane+1,0);
      _q_max_v.resize(hit_plane+1,0);
      }

      float wire = h.WireID().Wire;
      float time = h.PeakTime();
      float  q = h.Integral();
      if( _w_min_v[hit_plane] > wire ) _w_min_v[hit_plane] = wire ;
      if( _w_max_v[hit_plane] < wire ) _w_max_v[hit_plane] = wire ;
      if( _t_min_v[hit_plane] > time ) _t_min_v[hit_plane] = time ;
      if( _t_max_v[hit_plane] < time ) _t_max_v[hit_plane] = time ;
      if( _q_max_v[hit_plane] < q    ) _q_max_v[hit_plane] = q;

      }

    //Resize for the correct number of planes
    _div_x.resize(_w_min_v.size());
    _div_y.resize(_w_min_v.size());

    for(size_t plane=0; plane<_w_min_v.size(); ++plane) {

      _div_x[plane] = ( _w_max_v[plane] - _w_min_v[plane] + 2*_offset ) /_n_bins;
      _div_y[plane] = ( _t_max_v[plane] - _t_min_v[plane] + 2*_offset ) /_n_bins;

      ::cv::Mat mat(_n_bins,_n_bins,CV_8UC1, cvScalar(0.));
      ::cv::Mat canny;
      canny.create(mat.size(),mat.type());

      _mat_v.emplace_back(mat);
      _canny_v.emplace_back(canny);
      }

  for(auto const& h : *ev_hit) {

    if(h.Integral()<5.) continue;
    size_t plane = h.WireID().Plane;
    int wire = (h.WireID().Wire - (_w_min_v[plane] - _offset))/_div_x[plane];
    int time = ((int)(h.PeakTime()) - (_t_min_v[plane] - _offset))/_div_y[plane];
    auto& mat = _mat_v[plane];
    int charge = ((256. * h.Integral() / _q_max_v[plane]));

    if(charge>256) charge = 256;

     mat.at<unsigned char>(wire,time) += (unsigned char)(charge);

    }
  }


// Visually check on the contours being drawn. Still doesn't match python 
void MatMaker::DrawTestPlane(::cv::Mat canny, std::vector<std::vector<cv::Point>> found){

  ::cv::RNG rng(12345);
  ::cv::Mat drawing = ::cv::Mat::zeros( canny.size(), CV_8UC3 );
//  char* source_window = "Source";
//  ::cv::imshow( "edges", canny);
  //std::vector<::cv::Mat> allPlanes;
  //allPlanes.reserve(found.size());

  for( int i = 0; i< found.size(); i++ ){
    ::cv::Scalar color = ::cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    ::cv::drawContours( drawing, found, i, color) ;//, 2, 8, hierarchy, 0, Point() );
    //allPlanes.emplace_back(drawing);
    }   
    ::cv::imshow( "contours", drawing);
    ::cv::waitKey(0);
  }   

#endif
