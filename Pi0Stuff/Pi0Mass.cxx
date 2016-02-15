#ifndef LARLITE_PI0MASS_CXX
#define LARLITE_PI0MASS_CXX

#include "Pi0Mass.h"
#include "DataFormat/shower.h"

namespace larlite {

  bool Pi0Mass::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //

    if(!_mass){
      _mass = new TH1D("Mass","Pi0 Mass Peak with Fuzzy; Mass[MeV]",100,0,600);
      }

    _events = 0;
    _enough_sh =0;

    return true;
  }
  
  bool Pi0Mass::analyze(storage_manager* storage) {
  
    auto const & ev_s = storage->get_data<event_shower>("showerreco");
    _events++;
     
    //std::cout<<"New Event!" <<std::endl ;
//    if(ev_s->size())
//      _enough_sh++;

    if( ev_s->size() == 2 ){
      _enough_sh++;
      auto s1 = ev_s->at(0) ;
      auto s2 = ev_s->at(1) ;

      double energy_a = s1.Energy(2); //s1.best_plane()) ; 
      double energy_b = s2.Energy(2); //s2.best_plane()) ;
    
      auto angle = s1.Direction().Angle(s2.Direction());
 
      auto mass = sqrt(4 * energy_a * energy_b * pow(sin(angle/2.),2));

      //std::cout<<"mass is: "<< mass<<", angle: "<<angle<<", energy : "<<energy_a<<", "<<energy_b<<std::endl; 

       _mass->Fill(mass);

     }
    return true;
  }

  bool Pi0Mass::finalize() {

    std::cout<<"Number events: "<<_events<<std::endl;
    std::cout<<"Number of times we have enough showers, sigh: "<<_enough_sh<<std::endl ;

    if(_fout){
      _fout->cd();
      _mass->SetFillStyle(3003);
      _mass->SetFillColor(kBlue);
      _mass->Write();
     }
  
    return true;
  }

}
#endif
