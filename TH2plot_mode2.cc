// Macro to read output file of exampleB01b and write file of TH2
// Different range values for analog simulation
// Use: root -l TH2plot_mode2.cc'(input, output, TTree directory)'

#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TBranch.h"
#include "TH1D.h"
#include <iostream>
#include <string>

//-------------------------------------------------------
const Int_t GLOBAL_NCELLS = 20;   // number of cells
std::array <Double_t, GLOBAL_NCELLS> fCellArray;  // Container to fill cell values

//-------ooo000ooo-------ooo000ooo-------ooo000ooo-------

void TH2plot(const char *, const char *, const char *);

TH2F *histfill(TTree*, const char*, Int_t);

TH2F *hist_layer (TTree*, Int_t, const char*, const char*, const char*,
                                                 Int_t nbinx = 20,
                                                 Double_t binx_low = 0,
                                                 Double_t binx_high = 0);

TH2F *hist_weights(TTree*, Int_t, const char*, const char*);

TH2F *hist_process(TTree*, Int_t, const char*);

//-------ooo000ooo-------ooo000ooo-------ooo000ooo-------

void TH2plot_mode2 (const char *input_file = "B01b.root",
              const char *output_file = "TH2_mode2.root",
              const char *tree_name = "events")
{
  // Open input file
  TFile *f1 = TFile::Open(input_file, "READ");
  if (!f1) return;
  
  // Access TTree
  TTree *t1 = (TTree*)f1->Get(tree_name);
  
  // Get branches in treereader
  TObjArray *tarr = t1->GetListOfBranches(); // List of TBranches
  auto num_branches = tarr->GetEntries(); // # of TBranches
  
  // Get number of events (rows) in treereader
  Int_t num_events = (Int_t) t1->GetEntries();
  
  /* Create list of canvases & histograms for each branch
    * First column always contains the layer number of detector
    * One axis will represent the layer number
    */
  
  // TH2D *hist[num_branches - 1];
  
  /* For each branch:
    *   Set TTreeReaderValue to read each entry
    *   Set histogram name & title
    *   Use TTreeReaderValue to fill each histogram with entries
    *   SAVE to output file
    */
  
  TFile *f2 = TFile::Open(output_file, "UPDATE");
  
  for (Int_t col = 0; col < num_branches; ++col)
  // Read over each branch
  {
    const char *branchname = tarr->At(col)->GetName();
    if (std::strcmp(branchname, "Layer_Number") == 0) continue;
    std::cout << "Accessing branch: " << branchname << std::endl;

    auto dir = Form("%s/%s", tree_name, branchname);
    gDirectory->cd("../../");

    if (!gDirectory->GetDirectory(dir)) gDirectory->mkdir(dir);
    gDirectory->cd(dir);
    
    // Histogram
    auto hist = histfill(t1, branchname, num_events); // Array of TH1
      
    // Add to existing histogram if possible
    if ( gDirectory->Get(hist->GetTitle()) ) hist->Add( (TH2*) gDirectory->Get(hist->GetTitle()) );
    // Write histogram to file
    hist->Write(0, TObject::kOverwrite);
  }
  
  f2->Close();
  f1->Close();
}

//--------ooo000ooo---------ooo000ooo---------ooo000ooo---------
// This function receives the branchname and automatically fills a TH2D histogram with correct labels.
// Check how you created the ntuples in the G4 fill in order to change the cases.
TH2F *histfill (TTree *t, const char *branchname, Int_t num_events)
{  
  if (std::strcmp(branchname, "Limiting_Process") == 0)
  {
    // Create histogram
    TH2F *hist = hist_process(t, num_events, branchname);
    return hist;
  }
  
  else if (std::strcmp(branchname, "NSecondaries(unweighted)") == 0)
  {
    // Create histogram
    TH2F *hist = hist_layer(t, num_events, branchname, "", "# of secondaries", 30, 0, 30);
    return hist;
  }
  
  else if (std::strcmp(branchname, "NSecondaries(weighted)") == 0)
  {
    // Create histogram
    TH2F *hist = hist_layer(t, num_events, branchname, "", "# of secondaries", 25, 0, 25);
    return hist;
  }
  
  else if (std::strcmp(branchname, "Energy_Deposit(unweighted)") == 0)
  {   
    // Create histogram
    TH2F *hist = hist_layer(t, num_events, branchname, "", "energy deposit [MeV]", 30, 0, 60);
    return hist;
  }
  
  else if (std::strcmp(branchname, "Energy_Deposit(weighted)") == 0)
  {
    // Create histogram
    TH2F *hist = hist_layer(t, num_events, branchname, "", "energy deposit [MeV]", 30, 0, 60);
    return hist;
  }
  
  else if (std::strcmp(branchname, "Weight_PreStepPoint") == 0)
  {   
    // Create histogram
    TH2F *hist = hist_weights(t, num_events, branchname, "");
    return hist;
  }
  
  else if (std::strcmp(branchname, "Weight_PostStepPoint") == 0)
  {   
    // Create histogram
    TH2F *hist = hist_weights(t, num_events, branchname, "");
    return hist;
  }
  
  else if (std::strcmp(branchname, "Step_Length(unweighted)") == 0 || std::strcmp(branchname, "Step_Length(weighted)") == 0)
  {
    // Create histogram
    TH2F *hist = hist_layer(t, num_events, branchname, "", "average step length (mm)");
    return hist;
  }
  
  else if (std::strcmp(branchname, "Is_Enter(unweighted)") == 0)
  {
    // Create histogram
    TH2F *hist = hist_layer(t, num_events, branchname, "", "# tracks entering", 20, 0, 20);
    return hist;
  }
  
  else if (std::strcmp(branchname, "Is_Enter(weighted)") == 0)
  {
    // Create histogram
    TH2F *hist = hist_layer(t, num_events, branchname, "", "# tracks entering", 10, 0, 10);
    return hist;
  }
  
  else if (std::strcmp(branchname, "Is_Exit(unweighted)") == 0)
  {
    // Create histogram
    TH2F *hist = hist_layer(t, num_events, branchname, "", "# tracks exiting", 10, 0, 10);
    return hist;
  }
  
  else if (std::strcmp(branchname, "Is_Exit(weighted)") == 0)
  {
    // Create histogram
    TH2F *hist = hist_layer(t, num_events, branchname, "", "# tracks exiting", 10, 0, 10);
    return hist;    
  }
  
  else if (std::strcmp(branchname, "Num_Hits(unweighted)") == 0)
  {
    // Create histogram
    TH2F *hist = hist_layer(t, num_events, branchname, "", "# hits", 20, 0, 200);
    return hist;
  }
  
  else if (std::strcmp(branchname, "Num_Hits(weighted)") == 0)
  {
    // Create histogram
    TH2F *hist = hist_layer(t, num_events, branchname, "", "# hits", 20, 0, 200);
    return hist;
  }
  
  else return 0;
}

//--------ooo000ooo---------ooo000ooo---------ooo000ooo---------
// Array of 1D Histograms in which each hist represents one quantity in each layer for each event.
TH2F *hist_layer (TTree* t,
                  Int_t  num_events,
             const char* branchname,
             const char* hist_title,
             const char* y_title,
                  Int_t  nbiny,
               Double_t  biny_low,
               Double_t  biny_high)
{ 
  // Set output histogram
  auto hist = new TH2F(branchname, branchname, GLOBAL_NCELLS, 0, GLOBAL_NCELLS, nbiny, biny_low, biny_high);
  hist->SetTitle(hist_title);
  hist->GetXaxis()->SetTitle("cell ID");
  hist->GetYaxis()->SetTitle(y_title);
  hist->GetZaxis()->SetTitle("events");
  hist->SetOption("COLZ");
  hist->SetStats(0);

  // Set up pointers to access branches
  TBranch *br_data = 0;
  std::vector<Double_t> *vec_data = 0;
  
  t->SetBranchAddress(branchname, &vec_data, &br_data);
  
  // Read over each event
  for (Int_t row = 0; row < num_events; ++row)
  {
    br_data->GetEntry(row);
    
    // Fill histograms
    for (auto cell = 0; cell < GLOBAL_NCELLS; ++cell)
    {
      if (vec_data->at(cell) < 0.00000001) continue;
      hist->Fill(cell, vec_data->at(cell) );
    }
  }
  
  t->ResetBranchAddresses();
  vec_data = {}; delete vec_data;

  return hist;
}

//--------ooo000ooo---------ooo000ooo---------ooo000ooo---------
// 2D Histogram to record weights
TH2F *hist_weights (TTree* t,
                    Int_t  num_events,
               const char* branchname,
               const char* hist_title)
{
  // Set container for histograms
  auto hist = new TH2F(branchname, branchname, GLOBAL_NCELLS, 0, GLOBAL_NCELLS, GLOBAL_NCELLS, -GLOBAL_NCELLS + 1, 1);
  hist->SetTitle(hist_title);
  hist->GetXaxis()->SetTitle("cell ID");
  hist->GetYaxis()->SetTitle("log_{2}[weight]");
  hist->GetZaxis()->SetTitle("# hits");
  hist->SetOption("LEGO2");
  hist->SetTitleOffset(1.8, "X");
  hist->SetTitleOffset(1.8, "Y");
  hist->SetStats(0);
  
  // Set up pointers to access branches
  TBranch *br_weights = 0;
  TBranch *br_layernum = 0;
  
  std::vector<Double_t> *vec_weights = 0;
  std::vector<Int_t> *vec_layernum = 0;

  t->SetBranchAddress(branchname, &vec_weights, &br_weights);
  t->SetBranchAddress("Layer_Number", &vec_layernum, &br_layernum);
  
  // Create histograms

  // Read over each event
  for (Int_t row = 0; row < num_events; ++row)
  {
    br_weights->GetEntry(row);
    br_layernum->GetEntry(row);

    for (Int_t i = 0; i < vec_layernum->size(); ++i)
    {
      // Cell number
      Int_t cell = (Int_t) vec_layernum->at(i);
    
      Double_t weight = (Double_t) vec_weights->at(i);
      hist->Fill( cell, TMath::Log2(weight) );
    }
  }
  
  t->ResetBranchAddresses();
  
  // Free memory
  vec_weights = {}; vec_layernum = {};
  delete vec_weights, vec_layernum;
  
  return hist;
}


//--------ooo000ooo---------ooo000ooo---------ooo000ooo---------
// 2D Histogram to display count of processes in each cell.
TH2F *hist_process (TTree* t,
                    Int_t  num_events,
               const char* branchname)
{
  // Set container for histograms
  TH2F *output;
  auto hist = new TH2F(branchname, branchname, GLOBAL_NCELLS, 0, GLOBAL_NCELLS, 10, 0, 1);
  hist->SetCanExtend(TH1::kAllAxes);
  hist->SetTitle("");
  hist->GetXaxis()->SetTitle("cell ID");
  hist->GetYaxis()->SetTitle("process");
  hist->GetZaxis()->SetTitle("# hits");
  hist->SetOption("COLZ");
  hist->SetStats(0);
  
  // Array of (index value) = (process id)
  const char *processlist[404] = {"",  // 0
  "CoulombScattering", "Ionisation", "Bremsstrahlung", "PairProdByCharged", "Annihilation", "AnnihilationToMuMu", "AnnihilationToHadrons", "NuclearStopping", // 1-8
  "", // 9
  "MultipleScattering", "Rayleigh", "PhotoElectricEffect", "ComptonScattering", "GammaConversion", "GammaConversionToMuMu", // 10-15
  "", "", "", "", "", // 16-20
  "Cerenkov", "ScInt_tillation", "SynchrotronRadiation", "TransitionRadiation", // 21-24
  "", "", "", "", "", "", // 25-30
  "OpAbsorption", "OpBoundary", "OpRayleigh", "OpWLS", "OpMieHG", // 31-35
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", // 36-90
  "TRANSPORTATION", "COUPLED_TRANSPORTATION", // 91-92
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", // 93-110
  "HadronElastic", // 111
  "", "", "", "", "", "", "", "", "", // 112-120
  "HadronInelastic", // 121
  "", "", "", "", "", "", "", "", "", // 122-130
  "Capture", // 131
  "", "", "", "", "", "", "", "", "", // 132-140
  "Fission", // 141
  "", "", "", "", "", "", "", "", "", // 142-150
  "HadronAtRest", "LeptonAtRest", // 151-152
  "", "", "", "", "", "", "", "", // 153-160
  "ChargeExchange", // 161
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", // 162-200
  "DECAY", // 201
  "", "", "", "", "", "", "", "", // 202-209
  "RadioactiveDecay", "UnknownDecay", // 210-211
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", // 212-230
  "ExternalDecay", // 231
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", // 232-300
  "FastSIM_ManagerProcess", // 301
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", // 302-400
  "STEPLIMITER", "USER_SPECIAL_CUTS", "NEUTRON_KILLER" // 401-403
  }; // End of list
  
  // Set up pointers to access branches
  TBranch *br_data = 0;
  TBranch *br_layernum = 0;
  TBranch *br_prestepweight = 0;

  std::vector<Int_t> *vec_data = 0;
  std::vector<Int_t> *vec_layernum = 0;
  std::vector<Int_t> *vec_prestepweight = 0;

  t->SetBranchAddress(branchname, &vec_data, &br_data);
  t->SetBranchAddress("Layer_Number", &vec_layernum, &br_layernum);
//   if (weighted) t->SetBranchAddress("Weight_PreStepPoint", &vec_prestepweight, &br_prestepweight);

  // Read over each event
  for (auto row = 0; row < num_events; ++row)
  {
    //std::cout << "PoInt_t i" << std::endl;
    br_data->GetEntry(row);
    br_layernum->GetEntry(row);
  //     if (weighted) br_prestepweight->GetEntry(row);

    for (auto i = 0; i < vec_layernum->size(); ++i)
    {
      // Cell number
      Int_t cell = vec_layernum->at(i);
      
      // Convert process id to const char
      Int_t procid = vec_data->at(i);
      const char *process = processlist[procid];
      if (process[0] == '\0') process = Form("%d", procid);
      
      // Fill
      hist->Fill(cell, process, 1);
    }
      
//       if (!weighted) output[cell].Fill(process, 1);
//       else output[cell].Fill( process, vec_prestepweight->at(i) );
  }

  
  t->ResetBranchAddresses();
  
  vec_data = {}; vec_layernum = {}; vec_prestepweight = {};
  delete vec_data, vec_layernum, vec_prestepweight;

  return hist;
}
