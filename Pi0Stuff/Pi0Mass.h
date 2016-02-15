/**
 * \file Pi0Mass.h
 *
 * \ingroup Pi0Stuff
 * 
 * \brief Class def header for a class Pi0Mass
 *
 * @author ah673
 */

/** \addtogroup Pi0Stuff

    @{*/

#ifndef LARLITE_PI0MASS_H
#define LARLITE_PI0MASS_H

#include "Analysis/ana_base.h"
#include "TH1D.h"

namespace larlite {
  /**
     \class Pi0Mass
     User custom analysis class made by SHELL_USER_NAME
   */
  class Pi0Mass : public ana_base{
  
  public:

    /// Default constructor
    Pi0Mass(){ _name="Pi0Mass"; _fout=0; _mass=0;}

    /// Default destructor
    virtual ~Pi0Mass(){}

    /** IMPLEMENT in Pi0Mass.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in Pi0Mass.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in Pi0Mass.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

  int _events;
  int _enough_sh;

  TH1D * _mass ;
    
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
