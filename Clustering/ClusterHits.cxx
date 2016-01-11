#ifndef LARLITE_CLUSTERHITS_CXX
#define LARLITE_CLUSTERHITS_CXX

#include "ClusterHits.h"
#include "DataFormat/cluster.h"
#include "DataFormat/event_ass.h"
#include "Base/GeoTypes.h"
#include "Base/GeoConstants.h"

namespace larlite {

  bool ClusterHits::initialize() { return true; }

  bool ClusterHits::analyze(storage_manager* storage) {
    std::cout<<"\nIn analyze!! "<<std::endl ;
    
    auto const ev_hit = storage->get_data<event_hit>("gaushit");

    auto out_cluster = storage->get_data<event_cluster>("imageCluster");
    auto out_ass = storage->get_data<event_ass>(out_cluster->name());

    AssSet_t my_ass;

    // Returns all planes of all contours in the plane of interest
    std::cout<<"\n\nLet's get some contours..."<<std::endl;
    auto const & contours = _ImageMaker.GetClusters(ev_hit);
    std::cout<<"Just obtained contours! "<<ev_hit->size()<<" possible hits. "<<std::endl;

    int offset = _ImageMaker.GetOffset();
    //int bins   = _ImageMaker.GetBins();

    for(int plane=0; plane<contours.size(); plane++){
      std::cout<<"\nOn plane: "<<plane<<std::endl;

      geo::PlaneID pID(0,0,plane);

      // Create cluster + association for each contour assuming each contour get at least 1 hit
      const size_t ass_set_index_offset = my_ass.size();
      //For first plane, this is the number of contours in the plane + 0 (my_ass size is currently 0)
      my_ass.reserve(contours[plane].size() + ass_set_index_offset ); //my_ass.size());

      std::map<size_t,larlite::hit> used_hits ;
      std::cout<<"And there are how many used hits? "<<used_hits.size()<<std::endl ;

      for(int contour_index=0; contour_index<contours[plane].size(); contour_index++){ 
	  auto const& c = contours[plane][contour_index];
          std::cout<<"Contour size: "<<c.size() <<std::endl; 

        for(size_t hit_index=0; hit_index<ev_hit->size(); hit_index++) {
          //std::cout<<"On hit index: "<<hit_index<<std::endl ; 
          // Check that hit has not been used before proceeding
          if(used_hits.find(hit_index) != used_hits.end()) continue;

	  auto const& h = (*ev_hit)[hit_index];

          //std::cout<<"hit and plane: "<<h.WireID().Plane<<", "<<plane<<std::endl;
	  if( h.WireID().Plane != plane) continue; // skip for hit that's not on this plane
          
	  int wire = ( h.WireID().Wire - (_ImageMaker.GetWireMin(plane) - offset) )/_ImageMaker.GetDivWire(plane) ;
          int time = ( int(h.PeakTime()) - (_ImageMaker.GetTimeMin(plane) - offset) )/_ImageMaker.GetDivTime(plane);

          std::pair<int,int> point ( wire, time ) ;

	  if (! _InsideCheck.InContour(c, point) ) continue;

          // At this point we've found a hit inside this contour!
          // Save this contour-hit combo as an association (once per contour)
          if( used_hits.size() == 0 ){
            cluster cl;
            cl.set_id(out_cluster->size());
            cl.set_original_producer("imageCluster");
            cl.set_planeID(pID);
            cl.set_view(geo::View_t(plane));
            cl.set_is_merged(true);

            out_cluster->emplace_back(cl);

            AssUnit_t one_ass;
            one_ass.reserve(ev_hit->size());
            my_ass.emplace_back(one_ass);

            std::cout<<"So far have "<<out_cluster->size()<<" outgoing clusters, and assoc size of "<<my_ass.size()<<" (they should match)."<<std::endl;
            } 

          used_hits[hit_index] = h ;

	  //auto& this_ass = my_ass[ass_set_index_offset + contour_index];
	  auto& this_ass = my_ass[ out_cluster->size() - 1] ; 
	  this_ass.push_back(hit_index);

// break;
        }
      }
    }

    // Store association
    out_ass->set_association( out_cluster->id(), ev_hit->id(), my_ass );
    std::cout<<"asociation size: "<<my_ass.size()<<std::endl ;

    storage->set_id(storage->run_id(),
		    storage->subrun_id(),
		    storage->event_id());

    //
    // Now we have filled association
    // Example: loop over cluster of hits:
    for(auto const& hit_index_v : my_ass) {
      std::cout<<"hitv 1: "<<hit_index_v.size()<<std::endl ;
//      int wire_max(0), time_max(0), time_min(1e8), wire_min(1e8);
    // Looping over cluster of hits
//    for(auto const& hit_index : hit_index_v) {
//      //std::cout<<"\nDid we save any hits? Let's see: "<<h.StartTick()<<std::endl ;
//      auto const& h = (*ev_hit)[hit_index];
//      if ( h.WireID().Wire > wire_max )
//        wire_max = h.WireID().Wire ;
//      if ( h.WireID().Wire < wire_min )
//        wire_min = h.WireID().Wire ;
//      if ( h.PeakTime() > time_max )
//        time_max = h.PeakTime() ;
//      if ( h.PeakTime() < time_min )
//        time_min = h.PeakTime() ;
//        }
      }


    return true;
  }

  bool ClusterHits::finalize() { return true; }

}
#endif
