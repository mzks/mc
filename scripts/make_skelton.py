import ROOT

filename = "./mc.root"
treename = "tree"
df = ROOT.RDataFrame("tree", filename)

f = open('new_analysis.C', 'w')

f.write('''
// ------------------------------ //
// new_analysis.C 
// generated with make_skelton.py
// Usage : root -l new_analysis.C
// ------------------------------ //

#include <iostream>
''')
f.write('int new_analysis(TString filename="' + filename + '"){\n')
f.write('    auto f = TFile::Open(filename);\n')
f.write('    auto '+ treename +' = f->Get<TTree>("'+treename+'");\n')
f.write('    ULong64_t n_entries = '+ treename +'->GetEntries();\n')
f.write('''
    std::cout << "Entries : " << n_entries << std::endl;

''')


for name in df.GetColumnNames():
    bn = str(name)
    c_type = df.GetColumnType(bn)
    c_type = c_type.replace("ROOT::VecOps::RVec", "std::vector")
    if c_type.find('vector') == -1:
        f.write('    ' + c_type + ' ' + bn + ' = 0;' + '\n')
    else:
        f.write('    ' + c_type + ' *' + bn + ' = nullptr;' + '\n')
    f.write('    ' + 'TBranch *b_' + bn + ';' + '\n')
    f.write('    ' + treename + '->SetBranchAddress("' + bn + '", &' + bn + ', &b_' + bn + ');' + '\n')


f.write('''
    auto new_f = TFile::Open("new_file.root", "RECREATE");
    auto new_t = new TTree("new_tree", "new_tree");
    Int_t dummy = 0;
    std::vector<Float_t> dummy_vec;
    new_t->Branch("dummy", &dummy, "dummy/I");
    new_t->Branch("dummy_vec", &dummy_vec);

    // B/b (Char_t/UChar_t)
    // S/s (Short_t/UShort_t)
    // I/i (Int_t/UInt_t)
    // F/f (Float_t/UFloat_t)
    // D/d (Double_t/UDouble_t)
    // L/l (Long64_t/ULong64_t)
    // O : [the letter o, not a zero] a boolean (Bool_t)

    for(ULong64_t i_entry=0; i_entry<n_entries; ++i_entry){
    ''')
f.write('        ' + treename + '->GetEntry(i_entry);\n')

f.write('''
        // if (your_cut) continue;
        // Insert your code...

        dummy = 0;
        dummy_vec.clear();
        dummy_vec = {1,2,3};
        new_t->Fill();
    }
    new_t->Write();
    new_f->Close();
    return 0;
}
        ''')

f.close()
