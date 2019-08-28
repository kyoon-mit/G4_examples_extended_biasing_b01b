// Simple Macro to read .root output from extended/biasing/exampleB01
// Use: root -l 'readfile.C'("file.root")'

#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TBranch.h"
#include "TH1D.h"

void readfile(const char *input_file,
              const char *output_file = "histograms.root",
              const char *tree_name = "B01b")
  {
    
    // Open input file
    TFile *f1 = TFile::Open(input_file, "READ");
    
    // Access TTree
    TTreeReader reader(tree_name, f1);
    TTree *t1 = dynamic_cast<TTree*>(f1->Get(tree_name));
    
    // Get branches in treereader
    TObjArray *tarr = t1->GetListOfBranches(); // List of TBranches
    Int_t num_branches = tarr->GetEntries(); // # of TBranches
    
    // Get number of entries in treereader
    Int_t num_entries = t1->GetEntries();
    
    // Create list of canvases & histograms for each branch
    // TH1D *hist[num_branches];
    
    /* For each branch:
     *   Set TTreeReaderValue to read each entry
     *   Set histogram name & title
     *   Use TTreeReaderValue to fill each histogram with entries
     *   SAVE to output file
     */
    
    TFile *f2 = TFile::Open(output_file, "RECREATE");
    
    for (Int_t col = 0; col < num_branches; col++)
      // Read over each branch
      {       
        const char *branchname = tarr->At(col)->GetName();
        TBranch *branch = t1->GetBranch(branchname);
        Double_t entry_value;
        branch->SetAddress(&entry_value);
        TH1D *hist = new TH1D(Form("h%d", col+1), branchname, num_entries, 0, num_entries);
        
        // Read over each entry
        // Int_t row = 0;
        // while (reader.Next())
        for (Int_t row = 0; row < num_entries; row++)
          {
            branch->GetEvent(row);
            hist->SetBinContent(row, entry_value);
            hist->SetTitle(branchname);
            hist->GetXaxis()->SetTitle("Cell ID");
            hist->GetYaxis()->SetTitle(branchname);
          }
          
        hist->Write(); // Current gDirectory is histograms.root:/
      }
      
    f2->Close();
    
    f1->Close();
  }

