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
#include "ContourMaker.h"
#include "ClusterRecoUtil/Base/CRUHelper.h"
//#include "ClusterRecoUtil/Alg/FillGeomParams.h"
#include "ClusterRecoUtil/Alg/DefaultParamsAlg.h"

namespace larlite {
  /**
     \class ClusterHits
     User custom analysis class made by SHELL_USER_NAME
   */
  class ClusterHits : public ana_base{
  
  public:

    /// Default constructor
    ClusterHits(){ _name="ClusterHits"; _fout=0; _area_tree=0; _dpa_tree=0;}//  _scale = 0;}

    /// Default destructor
    virtual ~ClusterHits(){}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

//    void Clear();

  protected:

    ContourMaker _ConMaker;
    ::cluster::CRUHelper    _CRUHelper;
//    ::cluster::FillGeomParams _Filler;
    ::cluster::DefaultParamsAlg _DPA;

    int _nplanes;
    int _nsteps;
    float _scale;

    TTree * _area_tree ;
    double _area;
    double _length;
    double _height;
    double _width;
    double _max_width;
    double _min_width;
    double _mindist ;
    double _maxdist;
    double _hit_dens ;
    double _n_hits;

    TTree * _dpa_tree;
    double _dpa_width ;
    double _dpa_length;
    double _sh ;
    double _trk ;
    double _open_angle ;


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
