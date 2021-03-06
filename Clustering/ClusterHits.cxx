#ifndef LARLITE_CLUSTERHITS_CXX
#define LARLITE_CLUSTERHITS_CXX

#include "ClusterHits.h"
#include "DataFormat/cluster.h"
#include "DataFormat/event_ass.h"
#include "Base/GeoTypes.h"
#include "Base/GeoConstants.h"

namespace larlite {

  bool ClusterHits::initialize() { 
    _nsteps = 2;
    _scale  = 1 ;
    
    if(!_area_tree){
      _area_tree = new TTree("area_tree","Area");
      _area_tree->Branch("area",&_area,"area/D");
      _area_tree->Branch("length",&_length,"length/D");
      _area_tree->Branch("height",&_height,"height/D");
      _area_tree->Branch("width",&_width,"width/D");
      _area_tree->Branch("max_width",&_max_width,"max_width/D");
      _area_tree->Branch("min_width",&_min_width,"min_width/D");
      _area_tree->Branch("mindist",&_mindist,"mindist/D");
      _area_tree->Branch("maxdist",&_maxdist,"maxdist/D");
      _area_tree->Branch("hit_dens",&_hit_dens,"hit_dens/D");
      _area_tree->Branch("n_hits",&_n_hits,"n_hits/D");
      }
     if(!_dpa_tree){
      _dpa_tree = new TTree("dpa_tree","Params");
      _dpa_tree->Branch("dpa_length",&_dpa_length,"dpa_length/D");
      _dpa_tree->Branch("dpa_width",&_dpa_width,"dpa_width/D");
      _dpa_tree->Branch("sh",&_sh,"sh/D");
      _dpa_tree->Branch("trk",&_trk,"trk/D");
      _dpa_tree->Branch("open_angle",&_open_angle,"open_angle/D");
      }
  
    return true; }

  bool ClusterHits::analyze(storage_manager* storage) {
//    std::cout<<"\nIn analyze!! "<<std::endl ;
    
    auto const ev_hit = storage->get_data<event_hit>("gaushit");
    auto out_cluster = storage->get_data<event_cluster>("imageCluster");
    auto out_ass = storage->get_data<event_ass>(out_cluster->name());

    AssSet_t my_ass;


//std::cout<<"Scale is: "<<_scale<<std::endl ;
    // Returns all planes of all contours in the plane of interest
    auto const & contours = _ConMaker.GetContours(ev_hit);

    int offset = _ConMaker.GetOffset();
    std::vector<int> filled_contours ;
    std::vector<int> hit_v;

    for(int plane=0; plane<contours.size(); plane++){
      filled_contours.reserve(contours[plane].size() + filled_contours.size()) ;
      hit_v.reserve(contours[plane].size() + filled_contours.size()); 

      geo::PlaneID pID(0,0,plane);

      // Create cluster + association for each contour that contains at least 1 hit
      const size_t ass_set_index_offset = my_ass.size();
      my_ass.reserve(contours[plane].size() + ass_set_index_offset ); 

      std::map<size_t,larlite::hit> used_hits ;

      int temp_contour_index = 0;

      for(int contour_index=0; contour_index<contours[plane].size(); contour_index++){ 
	  auto const& c = contours[plane][contour_index];
	  int n_hits = 0;

        for(size_t hit_index=0; hit_index<ev_hit->size(); hit_index++) {

          // Check that hit has not been used before proceeding
          if(used_hits.find(hit_index) != used_hits.end()) continue;

	  auto const& h = (*ev_hit)[hit_index];

          // Continue for hit that's not on this plane
	  if( h.WireID().Plane != plane) continue;
          
	  int wire = ( h.WireID().Wire - (_ConMaker.GetWireMin(plane) - offset) )/_ConMaker.GetDivWire(plane) ;
          int time = ( h.PeakTime() - (_ConMaker.GetTimeMin(plane) - offset) )/_ConMaker.GetDivTime(plane);

          std::pair<int,int> point ( wire, time); 

	  if (! _ConMaker.InContour(c, point, _nsteps,_scale) ) continue;

          // At this point we've found at least 1 hit inside this contour!
          // Create a cluster to store association info for this contour
          if( used_hits.size() == 0 || (temp_contour_index != contour_index ) ){

	    filled_contours.emplace_back(contour_index) ; 
	    
            cluster cl;
            cl.set_id(out_cluster->size());
            cl.set_original_producer("imageCluster");
            cl.set_planeID(pID);
            cl.set_view(geo::View_t(plane));
            //cl.set_is_merged(true);

            out_cluster->emplace_back(cl);

            AssUnit_t one_ass;
            one_ass.reserve(ev_hit->size());
            my_ass.emplace_back(one_ass);
            temp_contour_index = contour_index ;

            //std::cout<<"So far have "<<out_cluster->size()<<" outgoing clusters, and assoc size of "<<my_ass.size()<<" (they should match)."<<std::endl;
            } 

          used_hits[hit_index] = h ;

	  auto& this_ass = my_ass[ out_cluster->size() - 1] ; 
	  this_ass.push_back(hit_index);
	  n_hits++;

        }
	if(filled_contours.size() > hit_v.size())
	  hit_v.emplace_back(n_hits);

      }

    }// plane loop

    // Store association
    out_ass->set_association( out_cluster->id(), ev_hit->id(), my_ass );

    storage->set_id(storage->run_id(),
		    storage->subrun_id(),
		    storage->event_id());

    //std::cout<<"asociation size: "<<my_ass.size()<<std::endl ;

    std::vector<::cluster::cluster_params> params_v;
    _CRUHelper.GenerateParams(storage, "imageCluster", params_v);

    for ( auto & c : params_v){
      _DPA.FillParams(c);
      _dpa_width = c.width;
      _dpa_length = c.length;
      _sh = c.showerness ;
      _trk = c.trackness;
      _open_angle = c.opening_angle;
      _dpa_tree->Fill();
      }

    // Now we have filled association
    // Example: loop over cluster of hits:
//    for(auto const& hit_index_v : my_ass) {
//      std::cout<<"hitv 1: "<<hit_index_v.size()<<std::endl ;
//    for(auto const& hit_index : hit_index_v) {
//        }
//      }

    auto a = _ConMaker.GetAreas() ;
    auto l = _ConMaker.GetLengths();
    auto min_w = _ConMaker.GetMinWidths();
    auto max_w = _ConMaker.GetMaxWidths();
    auto h = _ConMaker.GetHeights();
    auto w = _ConMaker.GetWidths();
    auto min_width = _ConMaker.GetMin();
    auto max_width = _ConMaker.GetMax();

//  if ( _ConMaker.GetAreas().size() != hit_v.size() )
//   std::cout<<"Size of area: "<<_ConMaker.GetAreas().size()<<", "<<hit_v.size()<<std::endl ;

    for(size_t k=0; k<a.size(); k++){
    //for(size_t k=0; k<hit_v.size(); k++){
      _area = a[k] ;
      _hit_dens = hit_v[k] / a[k] ;
      _length = l[k] ;
      _height= h[k] ;
      _width= w[k] ;
      _max_width = max_w[k];
      _min_width = min_w[k];
      _mindist= min_width[k] ;
      _maxdist= max_width[k] ;
      _n_hits = hit_v[k];

//      std::cout<<"Filled contour index vs k : "<<k <<", "<<filled_contours[k]<<std::endl ;

      _area_tree->Fill();
      }

    return true;
  }

  bool ClusterHits::finalize() {
  
    if(_fout){
     _fout->cd();
     _area_tree->Write();
     _dpa_tree->Write();
      }

    return true; }

}
#endif
