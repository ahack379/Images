#ifndef LARLITE_IMAGE_CXX
#define LARLITE_IMAGE_CXX

#include "Image.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"

namespace larlite {

  bool Image::initialize() {

  if(!_event_tree){
    _event_tree = new TTree("event_tree","");
    _event_tree->Branch("x_max","std::vector<double>",&_xmax);
    _event_tree->Branch("x_min","std::vector<double>",&_xmin);
    _event_tree->Branch("y_max","std::vector<double>",&_ymax);
    _event_tree->Branch("y_min","std::vector<double>",&_ymin);
    _event_tree->Branch("q_max","std::vector<double>",&_qmax);
    _event_tree->Branch("wire_v","std::vector<std::vector<double>>",&_wires);
    _event_tree->Branch("time_v","std::vector<std::vector<double>>",&_times);
    _event_tree->Branch("charge_v","std::vector<std::vector<double>>",&_charge);
  }

    return true;
  }

  void Image::Reset(){

    _xmax.resize(_nplanes);
    _xmin.resize(_nplanes);
    _ymax.resize(_nplanes);
    _ymin.resize(_nplanes);

    _qmax.resize(_nplanes);

    for(size_t i=0; i < _nplanes ; i++){
      _xmax[i] = 0;
      _xmin[i] = 1e12;
      _ymax[i] = 0;
      _ymin[i] = 1e12;
      _qmax[i] = 0;
      }
    
    _wires.clear();
    _times.clear();
    _charge.clear();
    
    _wires.resize(_nplanes);
    _times.resize(_nplanes);
    _charge.resize(_nplanes);

    for(size_t i=0; i < _nplanes; i++){
      _wires[i].clear();
      _times[i].clear();
      _charge[i].clear();
       }
   }
  
  bool Image::analyze(storage_manager* storage) {
    
    Reset();

    auto const & ev_cl = storage->get_data<larlite::event_cluster>("cccluster");
    
    if (!ev_cl->size()){
      std::cout << "Did not find cluster data product!" << std::endl;
      return false;
      }

    ::larlite::event_hit* ev_hit = nullptr;
    auto const& hit_index_v = storage->find_one_ass(ev_cl->id(), ev_hit, "cccluster");

    if (!ev_hit) {
      std::cout << "Did not find hit data product!" << std::endl;
      return false;
      }

    for(size_t k=0; k<_nplanes; k++){
      _wires[k].reserve(ev_hit->size()-1);
      _times[k].reserve(ev_hit->size()-1);
      _charge[k].reserve(ev_hit->size()-1);
      }


    /// Store hit and wire information for each hit per event
    for(size_t i=0; i < ev_hit->size(); i++){ 
        auto const & h = ev_hit->at(i);
        int p = h.View() ;

        _wires[p].emplace_back(h.WireID().Wire); 
        _times[p].emplace_back(h.PeakTime()   ); 
        _charge[p].emplace_back(h.Integral()   ); 

          if(_wires[p].back() > _xmax[p] )
            _xmax[p] = _wires[p].back();
          if(_wires[p].back() < _xmin[p] )
            _xmin[p] = _wires[p].back();

          if(_times[p].back() > _ymax[p] )
            _ymax[p] = _times[p].back();
          if(_times[p].back() < _ymin[p] )
            _ymin[p] = _times[p].back();

          if(_charge[p].back() > _qmax[p] )
            _qmax[p] = _charge[p].back();

        }
      
    _event_tree->Fill();
  
    return true;
  }

  bool Image::finalize() {

    if(_fout){
      _fout->cd();
      _event_tree->Write();
      }

    return true;
  }

}
#endif
