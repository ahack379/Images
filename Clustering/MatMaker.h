/**
 * \file MatMaker.h
 *
 * \ingroup Clustering
 * 
 * \brief Class def header for a class MatMaker
 *
 * @author ariana Hackenburg 
 */

/** \addtogroup Clustering

    @{*/
#ifndef MATMAKER_H
#define MATMAKER_H

#include <iostream>
#include "opencv2/core/mat.hpp"
#include "DataFormat/hit.h"

/**
   \class MatMaker
   User defined class MatMaker ... these comments are used to generate
   doxygen documentation!
 */
class MatMaker{

public:

  /// Default constructor
  MatMaker(){ _n_bins = 200 ; _offset = 50; }

  /// Default destructor
  ~MatMaker(){}

  void Clear();

  void FillMat(const ::larlite::event_hit * ev_hit );

  void SetBins( int bins ) { _n_bins = bins; } 

  /// Pass one plane of canny + found contours from that plane 
  void DrawTestPlane(::cv::Mat canny, std::vector<std::vector<cv::Point>> found); 

  /// Max x boundary of Mat object
  float GetWireMax(int plane) { return _w_max_v[plane] ; }

  /// Min x boundary of Mat object
  float GetWireMin(int plane) { return _w_min_v[plane] ; }

  float GetTimeMax(int plane) { return _t_max_v[plane] ; }

  float GetTimeMin(int plane) { return _t_min_v[plane] ; }

  float GetDivWire(int plane) {return _div_x[plane] ; }

  float GetDivTime(int plane) {return _div_y[plane] ; }

  std::vector<::cv::Mat> GetMat  () { return _mat_v; }
  std::vector<::cv::Mat> GetCanny() { return _canny_v; }

  void SetMat  (std::vector<::cv::Mat> mat_v ) { _mat_v = mat_v; }
  void SetCanny(std::vector<::cv::Mat> canny_v) { _canny_v = canny_v; }

  int GetBins() { return _n_bins; }

  int GetOffset() { return _offset; }

  private:

   std::vector<::cv::Mat> _mat_v;
   std::vector<::cv::Mat> _canny_v;

   int   _n_bins ;
   float _offset ; 

   std::vector<float> _div_x;
   std::vector<float> _div_y;

   std::vector<float> _w_max_v ;
   std::vector<float> _w_min_v ;
   std::vector<float> _t_max_v ;
   std::vector<float> _t_min_v ;
   std::vector<float> _q_max_v;

};

#endif
/** @} */ // end of doxygen group 

