import argparse
import ROOT

parser = argparse.ArgumentParser()
parser.add_argument('-f', '--file', nargs="*", default=['mc.root'], help='Filename(s) including TTree')
parser.add_argument('-t', '--tree', nargs="*", default=['tree'], help='TTree name(s)')
args = parser.parse_args()
filenames = args.file
treenames = args.tree


if len(filenames) != len(treenames):
    print('File and Tree are not match.')
    exit(-1)
n_targets = len(treenames);

f = open('new_analysis.C', 'w')

f.write('''
// --------------------------------------- //
// new_analysis.C 
// generated with make_skelton.py
// Usage : root -l -b -q new_analysis.C
// --------------------------------------- //

#include <iostream>
''')
f.write('int new_analysis(')
for i in range(n_targets):
    f.write('TString filename'+str(i)+'="' + filenames[i] + '"' + (',' if i != n_targets-1 else ''))
f.write('){\n')

for i in range(n_targets):
    f.write('    auto f'+str(i)+' = TFile::Open(filename'+str(i)+');\n')
    f.write('    auto '+ treenames[i] +' = f'+str(i)+'->Get<TTree>("'+treenames[i]+'");\n')

f.write('    ULong64_t n_entries = '+ treenames[0] +'->GetEntries();\n')
f.write('    std::cout << "Entries : " << n_entries << std::endl;\n')

f.write('\n\n')
for i in range(n_targets):
    df = ROOT.RDataFrame(treenames[i], filenames[i])

    for name in df.GetColumnNames():
        bn = str(name)
        c_type = df.GetColumnType(bn)
        c_type = c_type.replace("ROOT::VecOps::RVec", "std::vector")
        if c_type.find('vector') == -1:
            f.write('    ' + c_type + ' ' + bn + ' = 0;' + '\n')
        else:
            f.write('    ' + c_type + ' *' + bn + ' = nullptr;' + '\n')
        f.write('    ' + 'TBranch *b_' + bn + ';' + '\n')
        f.write('    ' + treenames[i] + '->SetBranchAddress("' + bn + '", &' + bn + ', &b_' + bn + ');' + '\n')


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
for i in range(n_targets):
    f.write('        ' + treenames[i] + '->GetEntry(i_entry);\n')

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
