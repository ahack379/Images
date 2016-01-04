/**
 * \file checkInside.h
 *
 * \ingroup Image
 * 
 * \brief Class def header for a class checkInside
 *
 * @author ah673
 */

/** \addtogroup Image

    @{*/

#ifndef LARLITE_CHECKINSIDE_H
#define LARLITE_CHECKINSIDE_H

#include "Analysis/ana_base.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"

namespace larlite {
  /**
     \class checkInside
     User custom analysis class made by SHELL_USER_NAME
   */
  class checkInside : public ana_base{
  
  public:

    /// Default constructor
    checkInside(){ _name="checkInside"; _fout=0; _file=0; _contour_tree=0; _hit_tree=0; _contour_array=0;}

    /// Default destructor
    virtual ~checkInside(){}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    /// Check if a hit is inside a contour
    int InPolygon(std::vector<std::pair<double,double>> & contour, std::pair<double,double> & hit);

    /// Calcualte the angle between hit and 2 sets of coordinates
    double Angle2D(double& x1, double& y1, double& x2, double& y2);

    void newTree(const int & event, const int & plane);

  protected:

    TFile * _file ;
    TTree * _contour_tree ;
    TTree * _hit_tree ;

    int _event ; 

    std::vector<std::vector<std::pair<double,double>>> * _contour_array;
    std::vector<std::vector<std::pair<double,double>>> _hits; 
    
  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
