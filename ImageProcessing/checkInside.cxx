#ifndef LARLITE_CHECKINSIDE_CXX
#define LARLITE_CHECKINSIDE_CXX

#include "checkInside.h"
#include "math.h"

namespace larlite {

  bool checkInside::initialize() { 

    _file = TFile::Open("contours.root","READ");
    _event = 0;

    return true; 
    }

  void checkInside::newTree(const int & event, const int & plane ){
 
//   _contour_array->clear();

   _contour_tree = (TTree*)(_file->Get(Form("event_%i_plane_%i",event,plane)));
   _contour_tree->SetBranchAddress("contours",&_contour_array);
   _contour_tree->GetEntry(0);

   //std::cout<<"event plane size: "<<event<<", "<<plane<<", "<<_contour_array->size()<<std::endl;

   _hit_tree=new TTree(Form("hits_event_%d_plane_%d",event,plane), "Hit Tree");
   _hit_tree->Branch("hits","std::vector<std::vector<std::pair<double,double>>>",&_hits);

  }
  
  bool checkInside::analyze(storage_manager* storage) {

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

   for(int plane=0; plane<3; plane++){
     newTree(_event,plane);
     _hits.resize(_contour_array->size()) ;
     for(size_t j=0; j<_contour_array->size(); j++ ){  

       _hits[j].reserve(ev_hit->size());
       _contour_tree->GetEntry(j);
 
       for(size_t i=0; i < ev_hit->size(); i++){
          auto const & h = ev_hit->at(i);
          if ( h.View() == plane ){
            std::pair<double,double> point ( h.WireID().Wire, h.PeakTime() ) ;
            if ( InPolygon((*_contour_array)[j], point) )
              _hits[j].push_back(point);
            }
	  else if (h.View() > plane)
	    break;
           }
         }
	 _hit_tree->Fill();
	if(_fout){
	 _fout->cd();
         _hit_tree->Write();
	}
       }

   _event++; 

    return true;
  }

  int checkInside::InPolygon(std::vector<std::pair<double,double>> & points, std::pair<double,double> & p)
  {
     double angle=0;
     double x1, y1, x2, y2;
     double pi = atan(1)*4 ;

     auto const & n = points.size()/2;
     points.push_back(std::make_pair(points[0].first,points[0].second));

     for (size_t i = 0 ; i < n ; i+=10) {
        x1 = points[i].first  - p.first;
        y1 = points[i].second - p.second;
        x2 = points[(i+10)].first - p.first;
        y2 = points[(i+10)].second - p.second;
        angle += Angle2D(x1,y1,x2,y2);
     }   

       std::cout<<"angle is: "<<angle<<std::endl ;

   if (fabs(angle) < 1.1*pi) 
      return false;
   else
      return true;
   }

  double checkInside::Angle2D(double & x1, double & y1, double & x2, double & y2)
  {
     double dtheta,theta1,theta2;
     double pi = atan(1)*4 ;
  
     theta1 = atan2(y1,x1);
     theta2 = atan2(y2,x2);
     dtheta = theta2 - theta1;
     while (dtheta > pi)
        dtheta -= 2*pi ;
     while (dtheta < -pi)
        dtheta += 2*pi ;
  
     return dtheta ;
  }

  bool checkInside::finalize() { 

      return true; 
    }

}
#endif
