Int_t           nHit;
std::vector<double>  *x = 0;
std::vector<double>  *y = 0;
std::vector<double>  *z = 0;
std::vector<double>  *time = 0;
std::vector<double>  *eIn = 0;
std::vector<double>  *eDep = 0;
std::vector<int>     *TrackID = 0;
std::vector<int>     *copyNo = 0;
std::vector<int>     *particle = 0;

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
