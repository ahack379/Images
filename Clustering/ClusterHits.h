/**
 * \file ClusterHits.h
 *
 * \ingroup ImageProcessing
 * 
 * \brief Class def header for a class ClusterHits
 *
 * @author ariana Hackenburg
 */

/** \addtogroup ImageProcessing

    @{*/

#ifndef LARLITE_CLUSTERHITS_H
#define LARLITE_CLUSTERHITS_H

#include "Analysis/ana_base.h"
#include "HitImageMaker.h"
#include "HitInside.h"

namespace larlite {
  /**
     \class ClusterHits
     User custom analysis class made by SHELL_USER_NAME
   */
  class ClusterHits : public ana_base{
  
  public:

    /// Default constructor
    ClusterHits(){ _name="ClusterHits"; _fout=0; }

    /// Default destructor
    virtual ~ClusterHits(){}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

  protected:

    HitImageMaker _ImageMaker;
    HitInside     _InsideCheck;
    int           _nplanes;

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
