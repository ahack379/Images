
void plots(){

    TFile * f = new TFile("out.root");

    TTree * t = (TTree*)f->Get("tree") ;

    TCanvas * c4 = new TCanvas("c4","c4");
    TH1D * h4 = new TH1D("h4","3D Length Difference ",100,-100,100);
    TH1D * h5 = new TH1D("h5","3D Length Difference ",100,-80,80);
    t->Draw("(mc_length - calc_length)>>h4",""); //,"_misID==0");
    t->Draw("(mc_length - calc_length)>>h5","reco_energy > 10"); //,"_misID==0");
    h4->SetFillStyle( 3001);
    h5->SetFillStyle( 3001);
    h4->SetFillColorAlpha( kBlue,0.35);
    h5->SetFillColorAlpha( kRed,0.35);
    h5->SetLineColor(kRed);
    h4->GetXaxis()->SetTitle("Delta L [cm]");
    h5->DrawCopy() ;
    //h5->Draw("same");
    gStyle->SetOptStat(1);

    

//
//    TCanvas * c6 = new TCanvas("c6","c6");
//    TH1D * h6 = new TH1D("h6","Time MCFlash",100,-3200,4000);
//    t->Draw("(ft)>>h6",""); //,"_misID==0");
//    h6->SetFillStyle( 3001);
//    h6->SetFillColor( kBlue);
//    h6->GetXaxis()->SetTitle("Delta T [us]");
//    h6->DrawCopy() ;
//    gStyle->SetOptStat(1);

}
