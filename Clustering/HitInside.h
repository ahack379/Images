/**
 * \file HitInside.h
 *
 * \ingroup ImageProcessing
 * 
 * \brief Class def header for a class HitInside
 *
 * @author arian Hackenburg
 */

/** \addtogroup ImageProcessing

    @{*/
#ifndef HITINSIDE_H
#define HITINSIDE_H

#include <iostream>
#include "DataFormat/hit.h"
#include "Utils/DataTypes.h"

/**
   \class HitInside
   User defined class HitInside ... these comments are used to generate
   doxygen documentation!
 */
class HitInside{

public:

  /// Default constructor
  HitInside(){}

  /// Default destructor
  ~HitInside(){}

   /// Is this hit in the contour?
   ///int InContour(std::vector<std::pair<double,double>> & contour, const ::larlite::hit & h) ;
   int InContour(std::vector<std::pair<int,int>> contour, std::pair<double,double> point) ;
   int InContour(larcv::Point2DArray contour, std::pair<float,float> point) ;
    
   /// Calcualte the angle between hit and 2 sets of coordinates
   double Angle2D(double& x1, double& y1, double& x2, double& y2);

//   int GetXYMin(larlite::hit, 

};

#endif
/** @} */ // end of doxygen group 

