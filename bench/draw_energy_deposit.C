
int draw_energy_deposit(TString filename="config_kk3jkq4sbd_10000"){

    //gROOT->SetStyle("ATLAS");

    auto file = TFile::Open(filename+".root");
    auto tree = dynamic_cast<TTree*>(file->Get("tree"));
    Int_t           nHit;
    vector<double>  *x = 0;
    vector<double>  *y = 0;
    vector<double>  *z = 0;
    vector<double>  *time = 0;
    vector<double>  *eIn = 0;
    vector<double>  *eDep = 0;
    vector<int>     *TrackID = 0;
    vector<int>     *copyNo = 0;
    vector<int>     *particle = 0;
    TBranch        *b_nHit;
    TBranch        *b_x;
    TBranch        *b_y;
    TBranch        *b_z;
    TBranch        *b_time;
    TBranch        *b_eIn;
    TBranch        *b_eDep;
    TBranch        *b_TrackID;
    TBranch        *b_copyNo;
    TBranch        *b_particle;
    tree->SetBranchAddress("nHit", &nHit, &b_nHit);
    tree->SetBranchAddress("x", &x, &b_x);
    tree->SetBranchAddress("y", &y, &b_y);
    tree->SetBranchAddress("z", &z, &b_z);
    tree->SetBranchAddress("time", &time, &b_time);
    tree->SetBranchAddress("eIn", &eIn, &b_eIn);
    tree->SetBranchAddress("eDep", &eDep, &b_eDep);
    tree->SetBranchAddress("TrackID", &TrackID, &b_TrackID);
    tree->SetBranchAddress("copyNo", &copyNo, &b_copyNo);
    tree->SetBranchAddress("particle", &particle, &b_particle);

    ULong64_t entries = tree->GetEntries();
    const int n_bins = 200;
    const double hist_min = 0.0;
    const double hist_max = 20.0;  // MeV
    auto C = new TCanvas();
    auto hist = new TH1D("hist", "Energy Deposit", n_bins, hist_min, hist_max);
    hist->SetTitle(filename+";Deposit Energy (MeV);Counts/bins");

    for(int i_event=0; i_event<entries; ++i_event){
        tree->GetEntry(i_event);

        double total_edep = 0.;
        for(size_t i=0; i<eDep->size(); ++i){
            if(TrackID->at(i) != 1) continue;
            total_edep += eDep->at(i);
        }

        if(total_edep > 0) hist->Fill(total_edep);
    }
    hist->Draw();
    C->SaveAs(filename + ".pdf");

    return 0;
}
