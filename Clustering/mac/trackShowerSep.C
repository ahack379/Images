
void trackShowerSep(){

    TFile * f = new TFile("eminus_v4.root"); //minus_v3.root");
    TFile * f2 = new TFile("muMinus_v4.root"); //Minus_v3.root");

    TTree * s = (TTree*)f->Get("area_tree") ;
    TTree * t = (TTree*)f2->Get("area_tree") ;

    TTree * s_params = (TTree*)f->Get("dpa_tree") ;
    TTree * t_params = (TTree*)f2->Get("dpa_tree") ;

    TCanvas * c0 = new TCanvas("c0","c0");
    TH2D * h0 = new TH2D("h0","Length vs Area",200,0,800,200,0,2000);
    TH2D * h1 = new TH2D("h1","L v A",200,0,800,200,0,2000);
    s->Draw("area:length>>h0","");
    h0->SetMarkerColor(4);
    t->Draw("area:length>>h1","");
    h1->SetMarkerColor(2);
    h1->GetXaxis()->SetTitle("Length");
    h1->GetYaxis()->SetTitle("Area");
    gStyle->SetOptStat(0);
    c0->SetGrid();
    h1->DrawCopy();
    h0->Draw("same");

//    h0->DrawCopy("COLZ") ;
//    c0->Print("hitpe_v_g4pe.png","png");

    TCanvas * c1 = new TCanvas("c1","c1");
    TH1D * h2 = new TH1D("h2","Ratio of Contour Area to Bounding Box Area",75,0,0.8);
    TH1D * h3 = new TH1D("h3","Ratio of Contour Area to Bounding Box Area",75,0,0.8);
    s->Draw("extent>>h2"); //"npe_flash > 10 ");
    t->Draw("extent>>h3"); //"npe_flash > 10 ");
    h2->GetXaxis()->SetTitle("Ratio of Areas");
    h2->SetFillColorAlpha(kBlue,0.35);
    h3->SetFillColorAlpha(kRed,0.35);
    h2->SetLineColor(kBlue);
    h3->SetLineColor(kRed) ;
    h2->Scale(100./h2->GetEntries());
    h3->Scale(100./h3->GetEntries());
    gStyle->SetOptStat(0);
    c1->SetGrid();
    h3->DrawCopy() ;
    h2->Draw("same");

    TCanvas * c2 = new TCanvas("c2","c2");
    TH1D * h4 = new TH1D("h4","Bounding Box Height Over Width",50,0,90);
    TH1D * h5 = new TH1D("h5","Aspect Ratios",50,0,90);
    s->Draw("height*(height/area)>>h4"); //"npe_flash > 10 ");
    t->Draw("height*(height/area)>>h5"); //"npe_flash > 10 ");
    h4->GetXaxis()->SetTitle("Height / Width");
    h4->SetFillColorAlpha(kBlue,0.35);
    h4->Scale(1./h4->GetEntries());
    h5->Scale(1./h5->GetEntries());
    h5->SetFillColorAlpha(kRed,0.35);
    h4->SetLineColor(kBlue);
    h5->SetLineColor(kRed) ;
    gStyle->SetOptStat(0);
    c2->SetGrid();
    h4->DrawCopy() ;
    h5->Draw("same");

    TCanvas * c3 = new TCanvas("c3","c3");
    TH1D * h6 = new TH1D("h6","Bounding Box Height Over Min Contour Width",40,0,120);
    TH1D * h7 = new TH1D("h7","Aspect Ratios",40,0,120);
    s->Draw("height/mindist>>h6"); //"npe_flash > 10 ");
    t->Draw("height/mindist>>h7"); //"npe_flash > 10 ");
    h6->GetXaxis()->SetTitle("Length / Width");
    h6->SetFillColorAlpha(kBlue,0.35);
    h6->Scale(1./h6->GetEntries());
    h7->Scale(1./h7->GetEntries());
    h7->SetFillColorAlpha(kRed,0.35);
    h6->SetLineColor(kBlue);
    h7->SetLineColor(kRed) ;
    gStyle->SetOptStat(0);
    c3->SetGrid();
//    c3->SetLogy();
    h6->DrawCopy() ;
    h7->Draw("same");


}
