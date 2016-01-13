/**
 * \file ContourMaker.h
 *
 * \ingroup ImageProcessing 
 * 
 * \brief Class def header for a class ContourMaker
 *
 * @author kazuhiro + ahack
 */

/** \addtogroup ImageProcessing 

    @{*/

#ifndef CONTOURMAKER_H
#define CONTOURMAKER_H

#include "Analysis/ana_base.h"
#include "opencv2/core/mat.hpp"
#include "Utils/DataTypes.h"
#include "DataFormat/hit.h"
#include "DataFormat/cluster.h"
#include "MatMaker.h"

  /**
     \class ContourMaker
     User custom analysis class made by SHELL_USER_NAME
   */
  class ContourMaker : public MatMaker { 
  
  public:

    ContourMaker();

    ~ContourMaker(){}

    std::vector<std::vector<larcv::Point2DArray>> GetContours(const ::larlite::event_hit * hits);

    void SetParams ( int blur, int canny_upper, int canny_lower, int kernel ){
       _blur        = blur;
       _canny_upper = canny_upper;
       _canny_lower = canny_lower;
       _kernel      = kernel;
      }

    std::vector<double> GetAreas() { return _area_v ; }

    std::vector<double> GetLengths() { return _length_v ; } 

    std::vector<double> GetHeights() { return _height_v ; } 

    std::vector<double> GetAspectRatio() { return _aspectRatio_v ; } 

    std::vector<double> GetExtent() { return _extent_v; } 

    std::vector<double> GetMin() { return _min_v; } 

    std::vector<double> GetMax() { return _max_v; } 

    void FillVariables( std::vector<::cv::Point> contour );

    size_t NumContours(const size_t plane) ;

    bool InContour(std::vector<std::pair<int,int>> points, std::pair<double,double> p, int n_steps);

    bool InContour(larcv::Point2DArray arr, std::pair<float,float> p, int n_steps);

    double Angle2D(double & x1, double & y1, double & x2, double & y2) ;

  protected:

    //Per plane, vector of contours (which are the point2darrays)
    std::vector< std::vector<larcv::Point2DArray> >_contour_v;

    std::vector<double> _area_v ;
    std::vector<double> _length_v;
    std::vector<double> _height_v;
    std::vector<double> _aspectRatio_v ;
    std::vector<double> _extent_v;
    std::vector<double> _min_v;
    std::vector<double> _max_v;


    int _blur;
    double _canny_upper;
    double _canny_lower;
    double _threshold;
    int _kernel;


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
