#ifndef AnalyzeHGCOctTB_H
#define AnalyzeHGCOctTB_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "RecHit.h"
#include "HGCNtupleVariables.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TFile.h"
#include "TProfile.h"
#include "TLorentzVector.h"
#include "TDirectory.h"

class AnalyzeHGCOctTB : public HGCNtupleVariables{

 public:
  AnalyzeHGCOctTB(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data", const char *config="alpha");
  ~AnalyzeHGCOctTB();
  //Bool_t   FillChain(TChain *chain, TChain *chain2, const TString &inputFileList);
  Bool_t   FillChain(TChain *chain, const TString &inputFileList);
  Long64_t LoadTree(Long64_t entry);
  void     EventLoop(const char *);
  void     BookHistogram(const char *, const char *);

  void moduleMap_init();
  
  TFile *oFile;
  TDirectory *d_Nrechit_layer;
  TDirectory *d_rechit_en_layer;
  TDirectory *d_adcHG;
  TDirectory *d_cluster;
  TDirectory *d_cluster_layer[28];
  
  TDirectory *d_EE[28];
  TDirectory *d_FH[12][7];
  
  const char *conf_;  
  /* TH1F *h_ADChg[128]; */
  TH1F *h_adcHG_EE[28][128];
  TH1F *h_adcHG_FH[12][7][128];
  TH1F *h_NRechits_EE[28];
  TH1F *h_NRechits_FH[12][7];
  // TH1F* h_nrechits;
  // TH1F* h_nrechit_1evt[5];	// Tracking hits in a single event
  TH1F* h_particleID;
  TH1F* h_beamEnergy;
  TH1F* h_runNumber;
  TH1F* h_moduleID;
  TH1F* h_nRechits_layer[28];
  TH1F* h_rechit_energy_all;
  TH1F* h_Nrechit_EE;
  TH1F* h_Nrechit_FH;
  TH1F* h_rechit_energy_EE_only;
  TH1F* h_rechit_energy_FH_only;
  TH2F* h_rechit_energy_EE_vs_FH;
  TH2F* h_Nrechit_EE_vs_FH;
  TH1F* h_ERechits_layer[28];
  //TH1F* h_shower_profile[100];
  TH1F* h_ERechits_full_official_calib;
  TH1F* h_ERechits_full_my_calib;

  TH2F* h_rechit_density[28];
  TProfile* g_density_delta[28];
  // TH1F* h_ERechits_MIP;
  // TH2F* h_rechit_En_N[28];		// histogram of cumulative rechits
  //TH2F* h_rechit_En_Nrechits;		// histogram of cumulative rechits
  TH2F* h_rechit_En_NRechits;
  ofstream myfile;
};
#endif

#ifdef AnalyzeHGCOctTB_cxx

void AnalyzeHGCOctTB::BookHistogram(const char *outFileName, const char* conf) {
  myfile.open("clusters.txt");
  char* hname = new char[200];
  char* dir_name = new char[200];
  conf_ = conf;
  oFile = new TFile(outFileName, "recreate");
  h_particleID = new TH1F("h_particleID","particleID",500,0,500);
  h_beamEnergy = new TH1F("h_beamEnergy","beamEnergy",500,0,500);
  h_runNumber = new TH1F("h_runNumber","RunNumber",2000,0,1000);
  h_moduleID = new TH1F("h_moduleID","moduleID",4,-2,2);
  h_rechit_energy_all = new TH1F("h_rechit_energy_all","h_rechit_energy_all",500,0.0,20000.0);
  h_rechit_energy_EE_only = new TH1F("h_rechit_energy_EE_only","h_rechit_energy_EE_only",500,0.0,20000.0);
  h_rechit_energy_FH_only = new TH1F("h_rechit_energy_FH_only","h_rechit_energy_FH_only",500,0.0,20000.0);
  h_rechit_energy_EE_vs_FH = new TH2F("h_rechit_energy_EE_vs_FH","Rechit energy EE vs FH",500,0.0,20000.0,500,0.0,20000.0);
  h_rechit_energy_EE_vs_FH->GetXaxis()->SetTitle("energy in EE(MIPs)");
  h_rechit_energy_EE_vs_FH->GetYaxis()->SetTitle("energy in FH(MIPs)");

  h_Nrechit_EE = new TH1F("h_Nrechit_EE","NRechits in EE",500,0.0,1000.0);
  h_Nrechit_FH = new TH1F("h_Nrechit_FH","NRechits in FH",500,0.0,1000.0);
  h_Nrechit_EE_vs_FH = new TH2F("h_Nrechit_EE_vs_FH","NRechits EE vs FH",500,0.0,1000.0,500,0.0,1000.0);
  h_Nrechit_EE_vs_FH->GetXaxis()->SetTitle("NRechits in EE");
  h_Nrechit_EE_vs_FH->GetYaxis()->SetTitle("NRechits in FH");



  h_rechit_En_NRechits = new TH2F("h_rechit_En_NRechits","h_rechit_En_NRechits",100,0,1000,500,0.0,20000.0);

  d_cluster = oFile->mkdir("clusters");
  d_cluster->cd();
  for(int i = 0; i < 28; i++){
    sprintf(dir_name,"Layer_%d",i+1);
    d_cluster_layer[i] =   d_cluster->mkdir(dir_name);
    d_cluster_layer[i]->cd();
    sprintf(hname,"rechit_density_%d",i+1);
    h_rechit_density[i] = new TH2F(hname,hname,16,-8.0,8.0,16,-8.0,8.0);
    sprintf(hname,"density_delta_%d",i+1);
    //g_density_delta[i] = new TProfile(hname, hname, 100.0, 0.0, 100.0, 170.0, 0.0, 17.0);
    g_density_delta[i] = new TProfile(hname, hname, 100.0, 0.0, 1000.0);
    //g_density_delta[i] = new TGraph(hname,hname);
  }

  
  d_adcHG = oFile->mkdir("ADC_distributions");
  d_adcHG->cd();
  for(int i = 0; i < 28; i++) {
    sprintf(dir_name,"EE_%d",i+1);
    d_EE[i] = d_adcHG->mkdir(dir_name);
    d_EE[i]->cd();
    sprintf(hname,"h_Nrechits_EE_L%d",i+1);
    h_NRechits_EE[i] = new TH1F(hname, hname, 50, 0, 50);
    int chan = 0;
    for(int chip = 0; chip < 4; chip++) {
      for(int cc = 0; cc < 64; cc+=2,chan++) {
	sprintf(hname,"h_adcHG_EE_L%d_P0_chip%d_chan%d",i+1,chip,cc);
	h_adcHG_EE[i][chan] = new TH1F(hname, hname, 100, 0, 400);
      }
    }
  }
  for(int i = 0; i < 12; i++){
    for(int j = 0; j < 7; j++) {
      if(!strcmp(conf,"alpha") && (i+1)>9 ) {
	  if((j+1) != 4) continue;
	}

	sprintf(dir_name,"FH_L%d_P%d",i+1,j+1);
	d_FH[i][j] = d_adcHG->mkdir(dir_name);
	d_FH[i][j]->cd();

	sprintf(hname,"h_Nrechits_FH_L%d_P%d",i+1,j+1);
	h_NRechits_FH[i][j] = new TH1F(hname, hname, 50, 0, 50);
	
	int chan = 0;
	for(int chip = 0; chip < 4; chip++) {
	  for(int cc = 0; cc < 64; cc+=2,chan++) {
	    sprintf(hname,"h_adcHG_FH_L%d_P%d_chip%d_chan%d",i+1,j+1,chip,cc);
	    h_adcHG_FH[i][j][chan] = new TH1F(hname, hname, 100, 0, 400);

	  }
	}
      for(int chan = 0; chan < 128; chan++) {
      }
    }
  }
  // sprintf(hname,"h_rechit_E_full_official");
  // h_ERechits_full_official_calib = new TH1F(hname,hname,375.0,0.0,25000.0);

  // sprintf(hname,"h_rechit_E_full_my");
  // h_ERechits_full_my_calib = new TH1F(hname,hname,375.0,0.0,25000.0);


  // d_Nrechit_layer = oFile->mkdir("Nrechit_layer");
  // d_Nrechit_layer->cd();

  // for (int i=0;i<28;i++) {
  //   sprintf(hname,"h_rechit_layer_%d",i+1);
  //   h_nRechits_layer[i] = new TH1F(hname,hname,3000,0.0,3000.0);
  // }



  // d_rechit_en_layer = oFile->mkdir("rechit_energys");
  // d_rechit_en_layer->cd();
  
  
  // for (int i=0;i<28;i++) {
  //   sprintf(hname,"h_rechit_E_layer_%d",i);
  //   h_ERechits_layer[i] = new TH1F(hname,hname,250.0,0.0,5000.0);
  // }


  
}


void AnalyzeHGCOctTB::moduleMap_init() {
  cout<<"\n\nINFO: Mapping module configuration ALPHA (oct10-oct17) "<<endl;
  cout<<"INFO: Mapping EE[0]/FH[1]::Layer #[1-40]::Position on Layer[0 for EE]&[1-7 for FH] consult figure for Daisy structure configuration!!!"<<endl;
  char *f_name = new char[200];
  sprintf(f_name,"./moduleMAP_config1.txt");
  std::ifstream in(f_name);
  if(!in){
    cout<<"Could not find "<<f_name<<endl;
    return;
  }
  int modID_, part_, layer_, pos_;
  while(in>>modID_>>part_>>layer_>>pos_){
    std::pair<int, std::vector<int>> temp_pair;
    std::vector<int> temp_vector;
    temp_vector.push_back(part_);
    temp_vector.push_back(layer_);
    temp_vector.push_back(pos_);
    temp_pair = std::make_pair(modID_,temp_vector);
    module_map.insert(temp_pair);
  }

  cout<<"INFO: Module Mapping Done!!! "<<endl<<endl;


}
// void AnalyzeHGCOctTB::offical_calib_init() {
//   char *f_name = new char[200];
//   sprintf(f_name,"/home/shubham/work/HGCAL/CERNTB/CERN_5_oct_2018/txt_files/official_calib.txt");
//   std::ifstream in(f_name);
//   if(!in){
//     cout<<"Could not find "<<f_name<<endl;
//     return;
//   }
//   int layer_, module_, chip_, channel_;
//   long en_chan;
//   float adc_;
//   while(in>>layer_>>module_>>chip_>>channel_>>adc_){
//     en_chan = chip_*1000+channel_;
//     std::pair<int, int> temp;
//     temp = std::make_pair(layer_,en_chan);
//     std::pair<std::pair<int,int>, float> temp1;
//     temp1 = std::make_pair(temp, adc_);
//     offical_calib_map.insert(temp1);
//   }
// }

// void AnalyzeHGCOctTB::my_calib_init() {
//   char *f_name = new char[200];
//   sprintf(f_name,"/home/shubham/work/HGCAL/CERNTB/CERN_5_oct_2018/txt_files/ADC_MIP_v11.txt");
//   std::ifstream in(f_name);
//   if(!in){
//     cout<<"Could not find "<<f_name<<endl;
//     return;
//   }
//   int layer_, chip_, channel_,entry;
//   long en_chan;
//   float adc_,chi2,mip_err;
//   while(in>>layer_>>chip_>>channel_>>adc_>>chi2>>mip_err>>entry){
//     en_chan = chip_*1000+channel_;
//     std::pair<int,int> temp;
//     temp = std::make_pair(layer_+1,en_chan);
//     std::pair<std::pair<int,int>, float> temp1;
//     temp1 = std::make_pair(temp, adc_);
//     my_calib_map.insert(temp1);
//   }
// }

AnalyzeHGCOctTB::AnalyzeHGCOctTB(const TString &inputFileList, const char *outFileName, const char* dataset, const char* config) {

  TChain *tree = new TChain("rechitntupler/hits");
  // TChain *tree2 = new TChain("trackimpactntupler/impactPoints");

  // if( ! FillChain(tree, tree2, inputFileList) ) {
  //   std::cerr << "Cannot get the tree " << std::endl;
  // } else {
  //   std::cout << "Initiating analysis of dataset " << dataset << std::endl;
  // }

  if( ! FillChain(tree, inputFileList) ) {
    std::cerr << "Cannot get the tree " << std::endl;
  } else {
    std::cout << "Initiating analysis of dataset " << dataset << std::endl;
  }

  // HGCNtupleVariables::Init(tree, tree2);
  HGCNtupleVariables::Init(tree);

  BookHistogram(outFileName, config);
  moduleMap_init();
  // offical_calib_init();
  // my_calib_init();
  
}

//Bool_t AnalyzeHGCOctTB::FillChain(TChain *chain, TChain *chain2, const TString &inputFileList) {
Bool_t AnalyzeHGCOctTB::FillChain(TChain *chain, const TString &inputFileList) {

  ifstream infile(inputFileList, ifstream::in);
  std::string buffer;

  if(!infile.is_open()) {
    std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input" << std::endl;
    return kFALSE;
  }

  std::cout << "TreeUtilities : FillChain " << std::endl;
  while(1) {
    infile >> buffer;
    if(!infile.good()) break;
    //std::cout << "Adding tree from " << buffer.c_str() << std::endl;                                                              
    chain->Add(buffer.c_str());
    //chain2->Add(buffer.c_str());
  }
  std::cout << "No. of Entries in chain  : " << chain->GetEntries() << std::endl;
  //std::cout << "No. of Entries in chain2 : " << chain2->GetEntries() << std::endl;
  return kTRUE;
}

Long64_t AnalyzeHGCOctTB::LoadTree(Long64_t entry) {
  // Set the environment to read one entry                                                                                          
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (!fChain->InheritsFrom(TChain::Class()))  return centry;
  TChain *chain = (TChain*)fChain;
  if (chain->GetTreeNumber() != fCurrent) {
    fCurrent = chain->GetTreeNumber();
    //    Notify();
  }

  // if (!fChain2) return -5;
  // Long64_t centry2 = fChain2->LoadTree(entry);
  // if (centry2 < 0) return centry2;
  // if (!fChain2->InheritsFrom(TChain::Class()))  return centry2;
  // TChain *chain2 = (TChain*)fChain2;
  // if (chain2->GetTreeNumber() != fCurrent) {
  //   fCurrent = chain->GetTreeNumber();
  //   //    Notify();
  // }

  
  //if (centry==centry2)
  return centry;
  // cout<<"centry = "<<centry<<endl;
  // if(centry>0)
  //   return centry;
  // else return -1;
}

AnalyzeHGCOctTB::~AnalyzeHGCOctTB() { 

  // if (!fChain || !fChain2) return;
  // delete fChain->GetCurrentFile();
  // delete fChain2->GetCurrentFile();
  // oFile->cd();
  // oFile->Write();
  // oFile->Close();


  if (!fChain) return;
  delete fChain->GetCurrentFile();
  oFile->cd();
  oFile->Write();
  oFile->Close();
  myfile.close();
}

#endif
