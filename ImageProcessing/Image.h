/**
 * \file Image.h
 *
 * \ingroup Image
 * 
 * \brief Class def header for a class Image
 *
 * @author ariana Hackenburg
 */

/** \addtogroup Image

    @{*/

#ifndef LARLITE_IMAGE_H
#define LARLITE_IMAGE_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class Image
     This class stores hit information (wire, time) by extracting hit-association 
     information from each cluster.  It also stores max, min wire, time variables for
     later plotting purposes
   */
  class Image : public ana_base{
  
  public:

    /// Default constructor
    Image(){ _name="Image"; _fout=0; _event_tree=nullptr; _nplanes = 3; }

    /// Default destructor
    virtual ~Image(){}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    void Reset();

  protected:

    TTree * _event_tree;
    std::vector<double> _xmax ;
    std::vector<double> _xmin ;
    std::vector<double> _ymax ;
    std::vector<double> _ymin ;

    std::vector<std::vector<double>> _wires;
    std::vector<std::vector<double>> _times;

    int _nplanes ;
    
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
