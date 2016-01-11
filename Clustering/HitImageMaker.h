/**
 * \file HitImageMaker.h
 *
 * \ingroup ImageProcessing 
 * 
 * \brief Class def header for a class HitImageMaker
 *
 * @author kazuhiro + ahack
 */

/** \addtogroup ImageProcessing 

    @{*/

#ifndef HITIMAGEMAKER_H
#define HITIMAGEMAKER_H

#include "Analysis/ana_base.h"
#include "opencv2/core/mat.hpp"
#include "Utils/DataTypes.h"
#include "DataFormat/hit.h"
#include "DataFormat/cluster.h"
//#include "TObject.h"

  /**
     \class HitImageMaker
     User custom analysis class made by SHELL_USER_NAME
   */
  class HitImageMaker { //: public TObject { 
  
  public:

    HitImageMaker();

    ~HitImageMaker(){}

    std::vector<std::vector<larcv::Point2DArray>> GetClusters(const ::larlite::event_hit * hits);

    void SetParams ( int blur, int canny_upper, int canny_lower, int kernel ){
       _blur        = blur;
       _canny_upper = canny_upper;
       _canny_lower = canny_lower;
       _kernel      = kernel;
      }

    size_t NumContours(const size_t plane) const;

    float GetWireMax(int plane) { return x_max_v[plane] ; }

    float GetWireMin(int plane) { return x_min_v[plane] ; }

    float GetTimeMax(int plane) { return y_max_v[plane] ; }

    float GetTimeMin(int plane) { return y_min_v[plane] ; }

    float GetDivWire(int plane) {return _div_x[plane] ; }

    float GetDivTime(int plane) {return _div_y[plane] ; }

    int GetBins() { return _n_bins; }

    int GetOffset() { return _offset; }

    // Pass Canny edges for 1 plane and found contours in that plane 
    void DrawTestPlane( ::cv::Mat canny, std::vector<std::vector<::cv::Point>> ) ;

    void ResetVectors() ;

    void GetMaxMinWTQ(const ::larlite::event_hit * hits ) ;

  protected:

    std::vector<::cv::Mat> _mat_v;
    std::vector<::cv::Mat> _canny_v;
    //Per plane, vector of contours (which are the point2darrays)
    std::vector< std::vector<larcv::Point2DArray> >_contour_v;

    int _blur;
    double _canny_upper;
    double _canny_lower;
    int _kernel;
    int _n_bins;
    int _offset;
    std::vector<float> _div_x;
    std::vector<float> _div_y;

    std::vector<float> x_max_v ;
    std::vector<float> x_min_v ;
    std::vector<float> y_max_v ;
    std::vector<float> y_min_v ;
    std::vector<float> q_max_v;


  };

#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
