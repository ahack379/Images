#ifndef LARLITE_MCSHOWERTEST_CXX
#define LARLITE_MCSHOWERTEST_CXX

#include "MCShowerTest.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/shower.h"
#include "DataFormat/cluster.h"
#include "LArUtil/GeometryUtilities.h"
#include "math.h"

namespace larlite {

  bool MCShowerTest::initialize() {

   if(!_tree){
     _tree = new TTree("tree","tree");
     _tree->Branch("mc_length",&_mc_length,"_mc_length/D");
     _tree->Branch("calc_length",&_calc_length,"_calc_length/D");
     _tree->Branch("mc_angle",&_mc_angle,"_mc_angle/D");
     _tree->Branch("calc_angle",&_calc_angle,"_calc_angle/D");

     _tree->Branch("mc_energy",&_mc_energy,"_mc_energy/D");
     _tree->Branch("reco_energy",&_reco_energy,"_reco_energy/D");
     }

    return true;
  }
  
  bool MCShowerTest::analyze(storage_manager* storage) {

    _mc_length   = -10;
    _calc_length = -10;
    _mc_angle    = -1000;
    _calc_angle  = -1000;
    _mc_energy   = -1000;
    _reco_energy = -1000;

    auto geo_util = larutil::GeometryUtilities::GetME() ;

   auto ev_mc = storage->get_data<event_mcshower>("mcreco");
   auto ev_reco = storage->get_data<event_shower>("showerreco");
   auto ev_clus = storage->get_data<event_cluster>("mergeall");

   if(!ev_mc->size() || !ev_reco->size() || !ev_clus->size()) {std::cout<<"We coming in here? "<<std::endl; return false; }

   auto & reco_sh = ev_reco->at(0);
   auto & mc_sh = ev_mc->at(0);
   auto & m_st = mc_sh.Start();
   auto & m_end = mc_sh.End();
   auto length = pow( pow(m_st.X() - m_end.X(),2) + pow(m_st.Y() - m_end.Y(),2) + pow(m_st.Z()-m_end.Z(),2),0.5 );
   auto theta = acos( fabs (m_st.Y() - m_end.Y())/length) ;
   _mc_length = length ; //  sin ( theta ) * length ; 
   _mc_angle = atan2(m_end.X() - m_st.X(),m_end.Z() - m_st.Z())*180/3.142 ;  
   _mc_energy = mc_sh.DetProfile().E();

   std::cout<<"Length and lengthsin(theta) : "<<length<<", "<<length*sin(theta) <<std::endl ;


   int max_hits = 0;
   int max_it   = 0;
   for(int i=0; i<ev_clus->size(); i++){
     auto & cl = ev_clus->at(i); 
     if( cl.Plane().Plane !=2 ) continue;

     if(cl.NHits() > max_hits){
       max_hits = cl.NHits();
       max_it = i;
       }
     }

   //This is a plane 2 cluster with the max number of hits from merge-most
   auto cl = ev_clus->at(max_it);
   auto w_st = cl.StartWire() / geo_util->WireToCm(); 
   auto w_end = cl.EndWire() / geo_util->WireToCm(); 
   auto t_st = cl.StartTick() / geo_util->TimeToCm(); 
   auto t_end = cl.EndTick() / geo_util->TimeToCm(); 

   auto length2d_reco = pow( pow(w_st - w_end,2) + pow(t_st - t_end,2),0.5 );

   auto energy = reco_sh.Energy(2);
   _reco_energy = energy ;

   // This is a calcualted 3d length, ideally from cluster energy info-- 
   // not sure how to get energy from cluster yet, so for now use reco energy
   _calc_length = 13.8874 + 0.121734*energy - (3.75571e-05)*energy*energy;
   _calc_angle = (length2d_reco / _calc_length) * 180/3.142 ; 
   //atan2( reco_sh.Direction().Y(), reco_sh.Direction().X()) * 180./3.142; 
   
  
    _tree->Fill();

    return true;
  }

  bool MCShowerTest::finalize() {
     
     if(_fout){
       _fout->cd();
       _tree->Write();
       }
  
    return true;
  }

}
#endif
