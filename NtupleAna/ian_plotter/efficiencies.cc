#include "TChain.h"

#include "TriggerStudies/NtupleAna/interface/TrackHists.h"
#include "TriggerStudies/NtupleAna/interface/TrackData.h"



using namespace std;
using namespace NtupleAna;

//TrackHists::TrackHists(std::string name, fwlite::TFileService& fs) {
Efficiencies::Efficiencies(std::string name, fwlite::TFileService& fs) {

  TFileDirectory dir = fs.mkdir(name);
  m_name = name;

  m_ptM         = dir.make<TH1F>("ptEffM",         "Efficiency vs pt;Efficiency;pt",                 100,0,400); //M matched; T total

  m_etaM        = dir.make<TH1F>("etaEffM",        "Efficiency vs eta;Efficiency;eta",               100,-3,3);
  m_phiM        = dir.make<TH1F>("phiEffM",        "Efficiency vs phi;Efficiency;phi",               100,-3,3);
  m_massM       = dir.make<TH1F>("massEffM",       "Efficiency vs mass;Efficiency;mass",             100,0,200);
  m_deepcsvM    = dir.make<TH1F>("deepcsvEffM",    "Efficiency vs deepcsv;Efficiency;deepcsv",       100,0,100);
  m_deepcsv_bbM = dir.make<TH1F>("deepcsv_bbEffM", "Efficiency vs deepcsv_bb;Efficiency;deepcsv_bb", 100,-1,1);

  m_ptT         = dir.make<TH1F>("ptEffT",         "Efficiency vs pt;Efficiency;pt",                 100,0,400); //M matched; T total
  m_etaT        = dir.make<TH1F>("etaEffT",        "Efficiency vs eta;Efficiency;eta",               100,-3,3);
  m_phiT        = dir.make<TH1F>("phiEffT",        "Efficiency vs phi;Efficiency;phi",               100,-3,3);
  m_massT       = dir.make<TH1F>("massEffT",       "Efficiency vs mass;Efficiency;mass",             100,0,200);
  m_deepcsvT    = dir.make<TH1F>("deepcsvEffT",    "Efficiency vs deepcsv;Efficiency;deepcsv",       100,0,100);
  m_deepcsv_bbT = dir.make<TH1F>("deepcsv_bbEffT", "Efficiency vs deepcsv_bb;Efficiency;deepcsv_bb", 100,-1,1);
} 

Efficiencies::~Efficiencies() {} 


void
JetHists::Fill (const JetData& jetInfo){
  
  //  m_ip3d  ->Fill(track.m_ip3dVal);
  m_ptT         -> Fill(jetInfo.m_pt);
  m_etaT        -> Fill(jetInfo.m_eta);
  m_phiT        -> Fill(jetInfo.m_phi);
  m_massT       -> Fill(jetInfo.m_mass);
  m_deepcsvT    -> Fill(jetInfo.m_deepcsv);
  m_deepcsv_bbT -> Fill(jetInfo.m_deepcsv_bb);

  if(jetInfo.m_matchedJet){
    m_ptM         -> Fill(jetInfo.m_pt);
    m_etaM        -> Fill(jetInfo.m_eta);
    m_phiM        -> Fill(jetInfo.m_phi);
    m_massM       -> Fill(jetInfo.m_mass);
    m_deepcsvM    -> Fill(jetInfo.m_deepcsv);
    m_deepcsv_bbM -> Fill(jetInfo.m_deepcsv_bb);
  }
  return;
}

Efficie
