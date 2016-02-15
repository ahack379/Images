/**
 * \file MCShowerTest.h
 *
 * \ingroup Pi0Stuff
 * 
 * \brief Class def header for a class MCShowerTest
 *
 * @author ah673
 */

/** \addtogroup Pi0Stuff

    @{*/

#ifndef LARLITE_MCSHOWERTEST_H
#define LARLITE_MCSHOWERTEST_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class MCShowerTest
     User custom analysis class made by SHELL_USER_NAME
   */
  class MCShowerTest : public ana_base{
  
  public:

    /// Default constructor
    MCShowerTest(){ _name="MCShowerTest"; _fout=0; _tree=nullptr;}

    /// Default destructor
    virtual ~MCShowerTest(){}

    /** IMPLEMENT in MCShowerTest.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MCShowerTest.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MCShowerTest.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

  TTree * _tree ;

  double _mc_length;
  double _calc_length;
  double _mc_angle;
  double _calc_angle;
  double _mc_energy;
  double _reco_energy; 

  double _opening_angle;
    
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
