#include <iostream>
#include <iomanip>
#include <cstdio>
#include <TROOT.h>
#include <boost/bind.hpp>


#include "TriggerStudies/NtupleAna/interface/BTagAnalysis.h"
#include "nTupleAnalysis/baseClasses/interface/helpers.h"

using std::cout; using std::endl;
using namespace TriggerStudies;
using std::vector;  using std::map; using std::string; using std::set;

// 2018
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation102X
// const float OfflineDeepCSVTightCut2018  = 0.7527;
// const float OfflineDeepCSVMediumCut2018 = 0.4184;
// const float OfflineDeepCSVLooseCut2018  = 0.1241;
const float OfflineDeepCSVTightCutPhaseII = 0.9;
const float OfflineDeepCSVMediumCutPhaseII = 0.75;
const float OfflineDeepCSVLooseCutPhaseII  = 0.6;


// const float OfflineDeepFlavourTightCut2018   = 0.7264;
// const float OfflineDeepFlavourMediumCut2018  = 0.2770;
// const float OfflineDeepFlavourLooseCut2018   = 0.0494;
const float OfflineDeepFlavourTightCutPhaseII   = 0.93;
const float OfflineDeepFlavourMediumCutPhaseII  = 0.8;
const float OfflineDeepFlavourLooseCutPhaseII   = 0.6;

// https://cmswbm.cern.ch/cmsdb/servlet/HLTPath?PATHID=2117358
// https://cmswbm.cern.ch/cmsdb/servlet/HLTPath?PATHID=2090546
const float OnlineDeepCSVCutPF    = 0.24;
const float OnlineDeepCSVCutCalo  = 0.17;
// const float OnlineDeepCSVCutPuppi  = 0.17;
const float OnlineDeepCSVCutPuppiLoose  = 0.5;
const float OnlineDeepCSVCutPuppiMedium  = 0.7;
const float OnlineDeepCSVCutPuppiTight  = 0.85;
const float OnlineDeepFlavourCutPuppiLoose  = 0.5;
const float OnlineDeepFlavourCutPuppiMedium  = 0.7;
const float OnlineDeepFlavourCutPuppiTight  = 0.85;
const float OnlineCSVCutPF        = 0.7;
const float OnlineCSVCutCalo      = 0.5;
const float OnlineCSVCutPuppi      = 0.5;


// 2017
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation94X
// const float OfflineDeepCSVTightCut2017  = 0.8001;
// const float OfflineDeepCSVMediumCut2017 = 0.4941;
// const float OfflineDeepCSVLooseCut2017  = 0.1522;
//
// const float OfflineDeepFlavourTightCut2017   = 0.7489;
// const float OfflineDeepFlavourMediumCut2017  = 0.3033;
// const float OfflineDeepFlavourLooseCut2017   = 0.0521;



BTagAnalysis::BTagAnalysis(TChain* _eventsRAW, TChain* _eventsAOD, fwlite::TFileService& fs, bool _isMC, std::string _year, int _histogramming, bool _debug, std::string PUFileName, std::string jetDetailString, const edm::ParameterSet& nnConfig){
  if(_debug) cout<<"In BTagAnalysis constructor"<<endl;
  debug      = _debug;
  isMC       = _isMC;
  year       = _year;
  eventsRAW     = _eventsRAW;
  eventsRAW->SetBranchStatus("*", 0);

  eventsAOD     = _eventsAOD;
  eventsAOD->SetBranchStatus("*", 0);


  histogramming = _histogramming;

  doTracks = jetDetailString.find("Tracks") != std::string::npos;
  doCaloJets = jetDetailString.find("CaloJets") != std::string::npos;
  doPuppiJets = jetDetailString.find("PuppiJets") != std::string::npos;

  event      = new eventData(eventsRAW, eventsAOD, isMC, year, debug, jetDetailString);
  treeEvents = eventsRAW->GetEntries();

  cutflow    = new nTupleAnalysis::cutflowHists("cutflow", fs);
  cutflow->AddCut("all");
  cutflow->AddCut("foundMatch");
  cutflow->AddCut("passMuonCut");
  cutflow->AddCut("passElecCut");
  cutflow->AddCut("passLeptonCut");
  cutflow->AddCut("passNJetCut");
  cutflow->AddCut("passNBJetCut");

  //
  //  Jets
  //
  cutflowJets    = new nTupleAnalysis::cutflowHists("cutflowJets", fs);
  cutflowJets->AddCut("all");
  cutflowJets->AddCut("eta");
  cutflowJets->AddCut("pt");
  cutflowJets->AddCut("muonOlap");
  cutflowJets->AddCut("elecOlap");
  cutflowJets->AddCut("isProbe");
  cutflowJets->AddCut("hasHLTMatchPF");
  cutflowJets->AddCut("hasHLTMatchCalo");
  cutflowJets->AddCut("hasHLTMatchPuppi");


  //
  // hists
  //
  dir = fs.mkdir("BTagAnalysis");

  hAllMuons = new nTupleAnalysis::muonHists("AllMuons", fs, "All Muons");
  hAllElecs = new nTupleAnalysis::elecHists("AllElecs", fs, "All Elecs");
  hSelMuons = new nTupleAnalysis::muonHists("SelMuons", fs, "Sel Muons");
  hSelElecs = new nTupleAnalysis::elecHists("SelElecs", fs, "Sel Elecs");
  hMuons    = new nTupleAnalysis::muonHists("Muons", fs, "Muons");
  hElecs    = new nTupleAnalysis::elecHists("Elecs", fs, "Elecs");

  hEvents                 = new nTupleAnalysis::eventHists("Events", fs);
  hEventsNoPUWeight       = new nTupleAnalysis::eventHists("EventsNoPUWeight", fs);

  hOffJetsPreOLap         = new nTupleAnalysis::jetHists("offJetsPreOLap",        fs, "Pre Overlap");
  hOffJets                = new nTupleAnalysis::jetHists("offJets",               fs, "", jetDetailString);
  hOffJets_matched        = new nTupleAnalysis::jetHists("offJets_matched",       fs, "", jetDetailString);
  hOffJets_matched_eta2p4        = new nTupleAnalysis::jetHists("offJets_matched_eta2p4",       fs, "", jetDetailString);
  hOffJets_matched_eta1        = new nTupleAnalysis::jetHists("offJets_matched_eta1",       fs, "", jetDetailString);
  hOffJets_matched_eta2        = new nTupleAnalysis::jetHists("offJets_matched_eta2",       fs, "", jetDetailString);
  hOffJets_matched_eta3        = new nTupleAnalysis::jetHists("offJets_matched_eta3",       fs, "", jetDetailString);
  hOffJets_matchedJet     = new nTupleAnalysis::jetHists("offJets_matchedJet",    fs, "", jetDetailString);
  hOffJets_matchedJet_eta2p4     = new nTupleAnalysis::jetHists("offJets_matchedJet_eta2p4",    fs, "", jetDetailString);
  hOffJets_matchedJet_eta1     = new nTupleAnalysis::jetHists("offJets_matchedJet_eta1",    fs, "", jetDetailString);
  hOffJets_matchedJet_eta2     = new nTupleAnalysis::jetHists("offJets_matchedJet_eta2",    fs, "", jetDetailString);
  hOffJets_matchedJet_eta3     = new nTupleAnalysis::jetHists("offJets_matchedJet_eta3",    fs, "", jetDetailString);
  if(doCaloJets){
    hOffJets_matchedCalo    = new nTupleAnalysis::jetHists("offJets_matchedCalo",   fs, "", jetDetailString);
    hOffJets_matchedCaloJet = new nTupleAnalysis::jetHists("offJets_matchedCaloJet",fs, "", jetDetailString);
  }
  if(doPuppiJets){
    hOffJetsPuppi                = new nTupleAnalysis::jetHists("offJetsPuppi",               fs, "", jetDetailString);
    hOffJets_matchedPuppi             = new nTupleAnalysis::jetHists("offJets_matchedPuppi",   fs, "", jetDetailString);
    hOffJets_matchedPuppi_eta2p4        = new nTupleAnalysis::jetHists("offJets_matchedPuppi_eta2p4",       fs, "", jetDetailString);
    hOffJets_matchedPuppi_eta1        = new nTupleAnalysis::jetHists("offJets_matchedPuppi_eta1",       fs, "", jetDetailString);
    hOffJets_matchedPuppi_eta2        = new nTupleAnalysis::jetHists("offJets_matchedPuppi_eta2",       fs, "", jetDetailString);
    hOffJets_matchedPuppi_eta3        = new nTupleAnalysis::jetHists("offJets_matchedPuppi_eta3",       fs, "", jetDetailString);
    hOffJets_matchedPuppiJet          = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet",fs, "", jetDetailString);
    hOffJets_matchedPuppiJet_eta2p4     = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_eta2p4",    fs, "", jetDetailString);
    hOffJets_matchedPuppiJet_eta1     = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_eta1",    fs, "", jetDetailString);
    hOffJets_matchedPuppiJet_eta2     = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_eta2",    fs, "", jetDetailString);
    hOffJets_matchedPuppiJet_eta3     = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_eta3",    fs, "", jetDetailString);
  }

  hOffJet_matchedPFcsvTag         = new nTupleAnalysis::jetHists("offJets_matchedPFcsvTag",         fs, "");
  hOffJet_matchedPFcsvTagJet      = new nTupleAnalysis::jetHists("offJets_matchedPFcsvTagJet",      fs, "");
  hOffJet_matchedPFDeepcsvTag     = new nTupleAnalysis::jetHists("offJets_matchedPFDeepcsvTag",     fs, "");
  hOffJet_matchedPFDeepcsvTagJet  = new nTupleAnalysis::jetHists("offJets_matchedPFDeepcsvTagJet",  fs, "");

  hOffJetTightDeepCSV_matchedPFJet       = new nTupleAnalysis::jetHists("offJetsTight_matchedPFJet",       fs, "");
  hOffJetMediumDeepCSV_matchedPFJet      = new nTupleAnalysis::jetHists("offJetsMedium_matchedPFJet",      fs, "");
  hOffJetLooseDeepCSV_matchedPFJet       = new nTupleAnalysis::jetHists("offJetsLoose_matchedPFJet",       fs, "");

  hOffJetMedDeepCSV_matchedPFJet         = new nTupleAnalysis::jetHists("offJetsMedDeepCSV_matchedPFJet",       fs, "", "matchedBJet");
  hOffJetMedDeepCSV_matchedPFDeepCSV  = new nTupleAnalysis::jetHists("offJetsMedDeepCSV_matchedPFDeepCSV",      fs, "", "matchedBJet");
  hOffJetMedDeepCSV_matchedPFCSV      = new nTupleAnalysis::jetHists("offJetsMedDeepCSV_matchedPFCSV",          fs, "", "matchedBJet");

  hOffJetMedDeepFlav_matchedPFJet      = new nTupleAnalysis::jetHists("offJetsMedDeepFlav_matchedPFJet",          fs, "", "matchedBJet");
  hOffJetMedDeepFlav_matchedPFDeepCSV  = new nTupleAnalysis::jetHists("offJetsMedDeepFlav_matchedPFDeepCSV",      fs, "", "matchedBJet");
  hOffJetMedDeepFlav_matchedPFCSV      = new nTupleAnalysis::jetHists("offJetsMedDeepFlav_matchedPFCSV",          fs, "", "matchedBJet");

  if(doCaloJets){
    hOffJet_matchedCalocsvTag         = new nTupleAnalysis::jetHists("offJets_matchedCalocsvTag",         fs, "");
    hOffJet_matchedCalocsvTagJet      = new nTupleAnalysis::jetHists("offJets_matchedCalocsvTagJet",      fs, "");
    hOffJet_matchedCaloDeepcsvTag     = new nTupleAnalysis::jetHists("offJets_matchedCaloDeepcsvTag",     fs, "");
    hOffJet_matchedCaloDeepcsvTagJet  = new nTupleAnalysis::jetHists("offJets_matchedCaloDeepcsvTagJet",  fs, "");

    hOffJetTightDeepCSV_matchedCaloJet       = new nTupleAnalysis::jetHists("offJetsTight_matchedCaloJet",       fs, "");
    hOffJetMediumDeepCSV_matchedCaloJet      = new nTupleAnalysis::jetHists("offJetsMedium_matchedCaloJet",      fs, "");
    hOffJetLooseDeepCSV_matchedCaloJet       = new nTupleAnalysis::jetHists("offJetsLoose_matchedCaloJet",       fs, "");

    hOffJetMedDeepCSV_matchedCaloJet      = new nTupleAnalysis::jetHists("offJetsMedDeepCSV_matchedCaloJet",      fs, "", "matchedBJet");
    hOffJetMedDeepCSV_matchedCaloDeepCSV  = new nTupleAnalysis::jetHists("offJetsMedDeepCSV_matchedCaloDeepCSV",  fs, "", "matchedBJet");
    hOffJetMedDeepCSV_matchedCaloCSV      = new nTupleAnalysis::jetHists("offJetsMedDeepCSV_matchedCaloCSV",      fs, "", "matchedBJet");

    hOffJetMedDeepFlav_matchedCaloJet      = new nTupleAnalysis::jetHists("offJetsMedDeepFlav_matchedCaloJet",      fs, "", "matchedBJet");
    hOffJetMedDeepFlav_matchedCaloDeepCSV  = new nTupleAnalysis::jetHists("offJetsMedDeepFlav_matchedCaloDeepCSV",  fs, "", "matchedBJet");
    hOffJetMedDeepFlav_matchedCaloCSV      = new nTupleAnalysis::jetHists("offJetsMedDeepFlav_matchedCaloCSV",      fs, "", "matchedBJet");
  }
  if(doPuppiJets){
    hOffJet_matchedPuppicsvTag         = new nTupleAnalysis::jetHists("offJets_matchedPuppicsvTag",         fs, "");
    hOffJet_matchedPuppicsvTagJet      = new nTupleAnalysis::jetHists("offJets_matchedPuppicsvTagJet",      fs, "");
    hOffJet_matchedPuppiDeepcsvTag     = new nTupleAnalysis::jetHists("offJets_matchedPuppiDeepcsvTag",     fs, "");
    hOffJet_matchedPuppiDeepcsvTagJet  = new nTupleAnalysis::jetHists("offJets_matchedPuppiDeepcsvTagJet",  fs, "");
    hOffJet_matchedPuppiDeepcsvTagLoose     = new nTupleAnalysis::jetHists("offJets_matchedPuppiDeepcsvTagLoose",     fs, "");
    hOffJet_matchedPuppiDeepcsvTagLooseJet  = new nTupleAnalysis::jetHists("offJets_matchedPuppiDeepcsvTagLooseJet",  fs, "");
    hOffJet_matchedPuppiDeepcsvTagMedium     = new nTupleAnalysis::jetHists("offJets_matchedPuppiDeepcsvTagMedium",     fs, "");
    hOffJet_matchedPuppiDeepcsvTagMediumJet  = new nTupleAnalysis::jetHists("offJets_matchedPuppiDeepcsvTagMediumJet",  fs, "");
    hOffJet_matchedPuppiDeepcsvTagTight     = new nTupleAnalysis::jetHists("offJets_matchedPuppiDeepcsvTagTight",     fs, "");
    hOffJet_matchedPuppiDeepcsvTagTightJet  = new nTupleAnalysis::jetHists("offJets_matchedPuppiDeepcsvTagJetTight",  fs, "");

    hOffJet_matchedPuppiDeepflavTag     = new nTupleAnalysis::jetHists("offJets_matchedPuppiDeepflavTag",     fs, "");
    hOffJet_matchedPuppiDeepflavTagJet  = new nTupleAnalysis::jetHists("offJets_matchedPuppiDeepflavTagJet",  fs, "");
    hOffJet_matchedPuppiDeepflavTagLoose     = new nTupleAnalysis::jetHists("offJets_matchedPuppiDeepflavTagLoose",     fs, "");
    hOffJet_matchedPuppiDeepflavTagLooseJet  = new nTupleAnalysis::jetHists("offJets_matchedPuppiDeepflavTagLooseJet",  fs, "");
    hOffJet_matchedPuppiDeepflavTagMedium     = new nTupleAnalysis::jetHists("offJets_matchedPuppiDeepflavTagMedium",     fs, "");
    hOffJet_matchedPuppiDeepflavTagMediumJet  = new nTupleAnalysis::jetHists("offJets_matchedPuppiDeepflavTagMediumJet",  fs, "");
    hOffJet_matchedPuppiDeepflavTagTight     = new nTupleAnalysis::jetHists("offJets_matchedPuppiDeepflavTagTight",     fs, "");
    hOffJet_matchedPuppiDeepflavTagTightJet  = new nTupleAnalysis::jetHists("offJets_matchedPuppiDeepflavTagJetTight",  fs, "");

    hOffJetTightDeepCSV_matchedPuppiJet       = new nTupleAnalysis::jetHists("offJetsTight_matchedPuppiJet",       fs, "");
    hOffJetMediumDeepCSV_matchedPuppiJet      = new nTupleAnalysis::jetHists("offJetsMedium_matchedPuppiJet",      fs, "");
    hOffJetLooseDeepCSV_matchedPuppiJet       = new nTupleAnalysis::jetHists("offJetsLoose_matchedPuppiJet",       fs, "");

    hOffJetTightDeepFlav_matchedPuppiJet       = new nTupleAnalysis::jetHists("offJetsTightFlav_matchedPuppiJet",       fs, "");
    hOffJetMediumDeepFlav_matchedPuppiJet      = new nTupleAnalysis::jetHists("offJetsMediumFlav_matchedPuppiJet",      fs, "");
    hOffJetLooseDeepFlav_matchedPuppiJet       = new nTupleAnalysis::jetHists("offJetsLooseFlav_matchedPuppiJet",       fs, "");

    hOffJetMedDeepCSV_matchedPuppiJet      = new nTupleAnalysis::jetHists("offJetsMedDeepCSV_matchedPuppiJet",      fs, "", "matchedBJet");
    hOffJetMedDeepCSV_matchedPuppiDeepCSV  = new nTupleAnalysis::jetHists("offJetsMedDeepCSV_matchedPuppiDeepCSV",  fs, "", "matchedBJet");
    hOffJetMedDeepCSV_matchedPuppiCSV      = new nTupleAnalysis::jetHists("offJetsMedDeepCSV_matchedPuppiCSV",      fs, "", "matchedBJet");

    hOffJetMedDeepFlav_matchedPuppiJet      = new nTupleAnalysis::jetHists("offJetsMedDeepFlav_matchedPuppiJet",      fs, "", "matchedBJet");
    hOffJetMedDeepFlav_matchedPuppiDeepCSV  = new nTupleAnalysis::jetHists("offJetsMedDeepFlav_matchedPuppiDeepCSV",  fs, "", "matchedBJet");
    hOffJetMedDeepFlav_matchedPuppiCSV      = new nTupleAnalysis::jetHists("offJetsMedDeepFlav_matchedPuppiCSV",      fs, "", "matchedBJet");
  }

  if(isMC){
    hOffJets_matched_L        = new nTupleAnalysis::jetHists("offJets_matched_L",       fs, "", jetDetailString);
    hOffJets_matched_L_eta2p4        = new nTupleAnalysis::jetHists("offJets_matched_L_eta2p4",       fs, "", jetDetailString);
    hOffJets_matched_L_eta1        = new nTupleAnalysis::jetHists("offJets_matched_L_eta1",       fs, "", jetDetailString);
    hOffJets_matched_L_eta2        = new nTupleAnalysis::jetHists("offJets_matched_L_eta2",       fs, "", jetDetailString);
    hOffJets_matched_L_eta3        = new nTupleAnalysis::jetHists("offJets_matched_L_eta3",       fs, "", jetDetailString);
    hOffJets_matchedJet_L     = new nTupleAnalysis::jetHists("offJets_matchedJet_L",    fs, "", jetDetailString);
    hOffJets_matchedJet_L_eta2p4     = new nTupleAnalysis::jetHists("offJets_matchedJet_L_eta2p4",    fs, "", jetDetailString);
    hOffJets_matchedJet_L_eta1     = new nTupleAnalysis::jetHists("offJets_matchedJet_L_eta1",    fs, "", jetDetailString);
    hOffJets_matchedJet_L_eta2     = new nTupleAnalysis::jetHists("offJets_matchedJet_L_eta2",    fs, "", jetDetailString);
    hOffJets_matchedJet_L_eta3     = new nTupleAnalysis::jetHists("offJets_matchedJet_L_eta3",    fs, "", jetDetailString);

    hOffJets_matched_LandPU        = new nTupleAnalysis::jetHists("offJets_matched_LandPU",       fs, "", jetDetailString);
    hOffJets_matched_LandPU_eta2p4        = new nTupleAnalysis::jetHists("offJets_matched_LandPU_eta2p4",       fs, "", jetDetailString);
    hOffJets_matched_LandPU_eta1        = new nTupleAnalysis::jetHists("offJets_matched_LandPU_eta1",       fs, "", jetDetailString);
    hOffJets_matched_LandPU_eta2        = new nTupleAnalysis::jetHists("offJets_matched_LandPU_eta2",       fs, "", jetDetailString);
    hOffJets_matched_LandPU_eta3        = new nTupleAnalysis::jetHists("offJets_matched_LandPU_eta3",       fs, "", jetDetailString);
    hOffJets_matchedJet_LandPU     = new nTupleAnalysis::jetHists("offJets_matchedJet_LandPU",    fs, "", jetDetailString);
    hOffJets_matchedJet_LandPU_eta2p4     = new nTupleAnalysis::jetHists("offJets_matchedJet_LandPU_eta2p4",    fs, "", jetDetailString);
    hOffJets_matchedJet_LandPU_eta1     = new nTupleAnalysis::jetHists("offJets_matchedJet_LandPU_eta1",    fs, "", jetDetailString);
    hOffJets_matchedJet_LandPU_eta2     = new nTupleAnalysis::jetHists("offJets_matchedJet_LandPU_eta2",    fs, "", jetDetailString);
    hOffJets_matchedJet_LandPU_eta3     = new nTupleAnalysis::jetHists("offJets_matchedJet_LandPU_eta3",    fs, "", jetDetailString);

    hOffJets_matched_B        = new nTupleAnalysis::jetHists("offJets_matched_B",       fs, "", jetDetailString );
    hOffJets_matched_B_eta2p4        = new nTupleAnalysis::jetHists("offJets_matched_B_eta2p4",       fs, "", jetDetailString );
    hOffJets_matched_B_eta1        = new nTupleAnalysis::jetHists("offJets_matched_B_eta1",       fs, "", jetDetailString );
    hOffJets_matched_B_eta2        = new nTupleAnalysis::jetHists("offJets_matched_B_eta2",       fs, "", jetDetailString );
    hOffJets_matched_B_eta3        = new nTupleAnalysis::jetHists("offJets_matched_B_eta3",       fs, "", jetDetailString );
    hOffJets_matchedJet_B     = new nTupleAnalysis::jetHists("offJets_matchedJet_B",    fs, "", jetDetailString );
    hOffJets_matchedJet_B_eta2p4     = new nTupleAnalysis::jetHists("offJets_matchedJet_B_eta2p4",    fs, "", jetDetailString );
    hOffJets_matchedJet_B_eta1     = new nTupleAnalysis::jetHists("offJets_matchedJet_B_eta1",    fs, "", jetDetailString );
    hOffJets_matchedJet_B_eta2     = new nTupleAnalysis::jetHists("offJets_matchedJet_B_eta2",    fs, "", jetDetailString );
    hOffJets_matchedJet_B_eta3     = new nTupleAnalysis::jetHists("offJets_matchedJet_B_eta3",    fs, "", jetDetailString );

    hOffJets_matched_C        = new nTupleAnalysis::jetHists("offJets_matched_C",       fs, "");
    hOffJets_matched_C_eta2p4        = new nTupleAnalysis::jetHists("offJets_matched_C_eta2p4",       fs, "");
    hOffJets_matched_C_eta1        = new nTupleAnalysis::jetHists("offJets_matched_C_eta1",       fs, "");
    hOffJets_matched_C_eta2        = new nTupleAnalysis::jetHists("offJets_matched_C_eta2",       fs, "");
    hOffJets_matched_C_eta3        = new nTupleAnalysis::jetHists("offJets_matched_C_eta3",       fs, "");
    hOffJets_matchedJet_C     = new nTupleAnalysis::jetHists("offJets_matchedJet_C",    fs, "");
    hOffJets_matchedJet_C_eta2p4     = new nTupleAnalysis::jetHists("offJets_matchedJet_C_eta2p4",    fs, "");
    hOffJets_matchedJet_C_eta1     = new nTupleAnalysis::jetHists("offJets_matchedJet_C_eta1",    fs, "");
    hOffJets_matchedJet_C_eta2     = new nTupleAnalysis::jetHists("offJets_matchedJet_C_eta2",    fs, "");
    hOffJets_matchedJet_C_eta3     = new nTupleAnalysis::jetHists("offJets_matchedJet_C_eta3",    fs, "");

    if(doCaloJets){
      hOffJets_matchedCalo_L    = new nTupleAnalysis::jetHists("offJets_matchedCalo_L",   fs, "", jetDetailString);
      hOffJets_matchedCaloJet_L = new nTupleAnalysis::jetHists("offJets_matchedCaloJet_L",fs, "", jetDetailString);
      hOffJets_matchedCalo_LandPU    = new nTupleAnalysis::jetHists("offJets_matchedCalo_LandPU",   fs, "", jetDetailString);
      hOffJets_matchedCaloJet_LandPU = new nTupleAnalysis::jetHists("offJets_matchedCaloJet_LandPU",fs, "", jetDetailString);

      hOffJets_matchedCalo_B    = new nTupleAnalysis::jetHists("offJets_matchedCalo_B",   fs, "", jetDetailString );
      hOffJets_matchedCaloJet_B = new nTupleAnalysis::jetHists("offJets_matchedCaloJet_B",fs, "", jetDetailString );

      hOffJets_matchedCalo_C    = new nTupleAnalysis::jetHists("offJets_matchedCalo_C",   fs, "");
      hOffJets_matchedCaloJet_C = new nTupleAnalysis::jetHists("offJets_matchedCaloJet_C",fs, "");
    }
    if(doPuppiJets){
      hOffJets_matchedPuppi_L             = new nTupleAnalysis::jetHists("offJets_matchedPuppi_L",   fs, "", jetDetailString);
      hOffJets_matchedPuppi_L_eta2p4        = new nTupleAnalysis::jetHists("offJets_matchedPuppi_L_eta2p4",       fs, "", jetDetailString);
      hOffJets_matchedPuppi_L_eta1        = new nTupleAnalysis::jetHists("offJets_matchedPuppi_L_eta1",       fs, "", jetDetailString);
      hOffJets_matchedPuppi_L_eta2        = new nTupleAnalysis::jetHists("offJets_matchedPuppi_L_eta2",       fs, "", jetDetailString);
      hOffJets_matchedPuppi_L_eta3        = new nTupleAnalysis::jetHists("offJets_matchedPuppi_L_eta3",       fs, "", jetDetailString);
      hOffJets_matchedPuppiJet_L          = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_L",fs, "", jetDetailString);
      hOffJets_matchedPuppiJet_L_eta2p4     = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_L_eta2p4",    fs, "", jetDetailString);
      hOffJets_matchedPuppiJet_L_eta1     = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_L_eta1",    fs, "", jetDetailString);
      hOffJets_matchedPuppiJet_L_eta2     = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_L_eta2",    fs, "", jetDetailString);
      hOffJets_matchedPuppiJet_L_eta3     = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_L_eta3",    fs, "", jetDetailString);
      hOffJets_matchedPuppi_LandPU             = new nTupleAnalysis::jetHists("offJets_matchedPuppi_LandPU",   fs, "", jetDetailString);
      hOffJets_matchedPuppi_LandPU_eta2p4        = new nTupleAnalysis::jetHists("offJets_matchedPuppi_LandPU_eta2p4",       fs, "", jetDetailString);
      hOffJets_matchedPuppi_LandPU_eta1        = new nTupleAnalysis::jetHists("offJets_matchedPuppi_LandPU_eta1",       fs, "", jetDetailString);
      hOffJets_matchedPuppi_LandPU_eta2        = new nTupleAnalysis::jetHists("offJets_matchedPuppi_LandPU_eta2",       fs, "", jetDetailString);
      hOffJets_matchedPuppi_LandPU_eta3        = new nTupleAnalysis::jetHists("offJets_matchedPuppi_LandPU_eta3",       fs, "", jetDetailString);
      hOffJets_matchedPuppiJet_LandPU          = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_LandPU",fs, "", jetDetailString);
      hOffJets_matchedPuppiJet_LandPU_eta1     = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_LandPU_eta1",    fs, "", jetDetailString);
      hOffJets_matchedPuppiJet_LandPU_eta2     = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_LandPU_eta2",    fs, "", jetDetailString);
      hOffJets_matchedPuppiJet_LandPU_eta3     = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_LandPU_eta3",    fs, "", jetDetailString);

      hOffJets_matchedPuppi_B             = new nTupleAnalysis::jetHists("offJets_matchedPuppi_B",   fs, "", jetDetailString );
      hOffJets_matchedPuppi_B_eta2p4        = new nTupleAnalysis::jetHists("offJets_matchedPuppi_B_eta2p4",       fs, "", jetDetailString );
      hOffJets_matchedPuppi_B_eta1        = new nTupleAnalysis::jetHists("offJets_matchedPuppi_B_eta1",       fs, "", jetDetailString );
      hOffJets_matchedPuppi_B_eta2        = new nTupleAnalysis::jetHists("offJets_matchedPuppi_B_eta2",       fs, "", jetDetailString );
      hOffJets_matchedPuppi_B_eta3        = new nTupleAnalysis::jetHists("offJets_matchedPuppi_B_eta3",       fs, "", jetDetailString );
      hOffJets_matchedPuppiJet_B          = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_B",fs, "", jetDetailString );
      hOffJets_matchedPuppiJet_B_eta2p4     = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_B_eta2p4",    fs, "", jetDetailString );
      hOffJets_matchedPuppiJet_B_eta1     = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_B_eta1",    fs, "", jetDetailString );
      hOffJets_matchedPuppiJet_B_eta2     = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_B_eta2",    fs, "", jetDetailString );
      hOffJets_matchedPuppiJet_B_eta3     = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_B_eta3",    fs, "", jetDetailString );

      hOffJets_matchedPuppi_C             = new nTupleAnalysis::jetHists("offJets_matchedPuppi_C",   fs, "");
      hOffJets_matchedPuppi_C_eta2p4        = new nTupleAnalysis::jetHists("offJets_matchedPuppi_C_eta2p4",       fs, "");
      hOffJets_matchedPuppi_C_eta1        = new nTupleAnalysis::jetHists("offJets_matchedPuppi_C_eta1",       fs, "");
      hOffJets_matchedPuppi_C_eta2        = new nTupleAnalysis::jetHists("offJets_matchedPuppi_C_eta2",       fs, "");
      hOffJets_matchedPuppi_C_eta3        = new nTupleAnalysis::jetHists("offJets_matchedPuppi_C_eta3",       fs, "");
      //hOffJets_matchedPuppiJet_C          = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_C",fs, "");
      hOffJets_matchedPuppiJet_C          = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_C",    fs, "");
      hOffJets_matchedPuppiJet_C_eta2p4     = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_C_eta2p4",    fs, "");
      hOffJets_matchedPuppiJet_C_eta1     = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_C_eta1",    fs, "");
      hOffJets_matchedPuppiJet_C_eta2     = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_C_eta2",    fs, "");
      hOffJets_matchedPuppiJet_C_eta3     = new nTupleAnalysis::jetHists("offJets_matchedPuppiJet_C_eta3",    fs, "");
    }


  }

  hPfJets          = new nTupleAnalysis::jetHists("pfJets",           fs, "");
  hPfJets_matched  = new nTupleAnalysis::jetHists("pfJets_matched",           fs, "");
  hDeltaROffPf       = dir.make<TH1F>("dR_OffPf",            "BTagAnalysis/dR_OffPf;             DeltaR;   Entries", 100,-0.01, 5);

  if(doCaloJets){
    hCaloJets            = new nTupleAnalysis::jetHists("caloJets",           fs, "");
    hCaloJets_matched    = new nTupleAnalysis::jetHists("caloJets_matched",           fs, "");
  }
  if(doPuppiJets){
    hPuppiJets            = new nTupleAnalysis::jetHists("puppiJets",           fs, "");
    hPuppiJets_matched    = new nTupleAnalysis::jetHists("puppiJets_matched",           fs, "");
  }


  if(doTracks){
    hOffTracks           = new nTupleAnalysis::trackHists("offTracks",fs, "");
    hOffTracks_unmatched = new nTupleAnalysis::trackHists("offTracks_unmatched",fs, "");
    hOffTracks_matched   = new nTupleAnalysis::trackHists("offTracks_matched",fs, "");
    hOffTracks_noV0           = new nTupleAnalysis::trackHists("offTracks_noV0",fs, "");
    hOffTracks_matched_noV0   = new nTupleAnalysis::trackHists("offTracks_matched_noV0",fs, "");

    hPfTracks            = new nTupleAnalysis::trackHists("pfTracks",fs, "");
    hPfTracks_noV0       = new nTupleAnalysis::trackHists("pfTracks_noV0",fs, "");
    hPfTracks_matched    = new nTupleAnalysis::trackHists("pfTracks_matched",fs, "");
    hPfTracks_unmatched  = new nTupleAnalysis::trackHists("pfTracks_unmatched",fs, "");

    if(doCaloJets){
      hOffTracksCalo                = new nTupleAnalysis::trackHists("offTracksCalo",fs, "");
      hOffTracksCalo_unmatched      = new nTupleAnalysis::trackHists("offTracksCalo_unmatched",fs, "");
      hOffTracksCalo_matched        = new nTupleAnalysis::trackHists("offTracksCalo_matched",fs, "");
      hOffTracksCalo_noV0           = new nTupleAnalysis::trackHists("offTracksCalo_noV0",fs, "");
      hOffTracksCalo_matched_noV0   = new nTupleAnalysis::trackHists("offTracksCalo_matched_noV0",fs, "");

      hCaloTracks            = new nTupleAnalysis::trackHists("caloTracks",fs, "");
      hCaloTracks_noV0       = new nTupleAnalysis::trackHists("caloTracks_noV0",fs, "");
      hCaloTracks_matched    = new nTupleAnalysis::trackHists("caloTracks_matched",fs, "");
      hCaloTracks_unmatched  = new nTupleAnalysis::trackHists("caloTracks_unmatched",fs, "");
    }
    if(doPuppiJets){
      hOffTracksPuppi                = new nTupleAnalysis::trackHists("offTracksPuppi",fs, "");
      hOffTracksPuppi_unmatched      = new nTupleAnalysis::trackHists("offTracksPuppi_unmatched",fs, "");
      hOffTracksPuppi_matched        = new nTupleAnalysis::trackHists("offTracksPuppi_matched",fs, "");
      hOffTracksPuppi_noV0           = new nTupleAnalysis::trackHists("offTracksPuppi_noV0",fs, "");
      hOffTracksPuppi_matched_noV0   = new nTupleAnalysis::trackHists("offTracksPuppi_matched_noV0",fs, "");

      hPuppiTracks            = new nTupleAnalysis::trackHists("puppiTracks",fs, "");
      hPuppiTracks_noV0       = new nTupleAnalysis::trackHists("puppiTracks_noV0",fs, "");
      hPuppiTracks_matched    = new nTupleAnalysis::trackHists("puppiTracks_matched",fs, "");
      hPuppiTracks_unmatched  = new nTupleAnalysis::trackHists("puppiTracks_unmatched",fs, "");
    }

    hOffBTagsAll        = new nTupleAnalysis::btaggingHists("offBTagsAll",fs, "");
    hOffBTags           = new nTupleAnalysis::btaggingHists("offBTags",fs, "");
    hOffBTags_matched   = new nTupleAnalysis::btaggingHists("offBTags_matched",fs, "");
    hOffBTags_unmatched = new nTupleAnalysis::btaggingHists("offBTags_unmatched",fs, "");
    hOffBTags_noV0           = new nTupleAnalysis::btaggingHists("offBTags_noV0",fs, "");
    hOffBTags_matched_noV0   = new nTupleAnalysis::btaggingHists("offBTags_matched_noV0",fs, "");

    hPfBTags           = new nTupleAnalysis::btaggingHists("pfBTags",fs, "");
    hPfBTags_matched   = new nTupleAnalysis::btaggingHists("pfBTags_matched",fs, "");
    hPfBTags_unmatched = new nTupleAnalysis::btaggingHists("pfBTags_unmatched",fs, "");

    hPuppiBTags           = new nTupleAnalysis::btaggingHists("puppiBTags",fs, "");
    hPuppiBTags_matched   = new nTupleAnalysis::btaggingHists("puppiBTags_matched",fs, "");
    hPuppiBTags_unmatched = new nTupleAnalysis::btaggingHists("puppiBTags_unmatched",fs, "");

    hDeltaROffPf       = dir.make<TH1F>("dR_OffPf",            "BTagAnalysis/dR_OffPf;             DeltaR;   Entries", 100,-0.01, 5);


    hmttOff           = dir.make<TH1F>("mtt_off",            "BTagAnalysis/mtt_off;             mtt;   Entries", 100,-0.01, 2);
    hmttOff_isFromV0  = dir.make<TH1F>("mtt_off_isFromV0",   "BTagAnalysis/mtt_off_isFromV0;    mtt;   Entries", 100,-0.01, 2);
    hmttPf            = dir.make<TH1F>("mtt_pf",             "BTagAnalysis/mtt_pf;              mtt;   Entries", 100,-0.01, 2);
    hmttPf_isFromV0   = dir.make<TH1F>("mtt_pf_isFromV0",    "BTagAnalysis/mtt_pf_isFromV0;     mtt;   Entries", 100,-0.01, 2);
    if(doCaloJets){
      hmttCalo            = dir.make<TH1F>("mtt_calo",             "BTagAnalysis/mtt_calo;              mtt;   Entries", 100,-0.01, 2);
      hmttCalo_isFromV0   = dir.make<TH1F>("mtt_calo_isFromV0",    "BTagAnalysis/mtt_calo_isFromV0;     mtt;   Entries", 100,-0.01, 2);
    }
    if(doPuppiJets){
      hmttPuppi            = dir.make<TH1F>("mtt_puppi",             "BTagAnalysis/mtt_puppi;              mtt;   Entries", 100,-0.01, 2);
      hmttPuppi_isFromV0   = dir.make<TH1F>("mtt_puppi_isFromV0",    "BTagAnalysis/mtt_puppi_isFromV0;     mtt;   Entries", 100,-0.01, 2);
    }
  }

  //
  // Vertex Hists
  //
  hVtx       = new nTupleAnalysis::vertexHists("hltVtx", fs, "HLT Vtx");
  hVtx       ->makeDiffHists("hltVtx", fs, "HLT Vtx");

  hOffVtx    = new nTupleAnalysis::vertexHists("offVtx", fs, "Off Vtx");


  //
  //  Configure Selection
  //
  OfflineDeepCSVTightCut  = OfflineDeepCSVTightCutPhaseII  ;
  OfflineDeepCSVMediumCut = OfflineDeepCSVMediumCutPhaseII ;
  OfflineDeepCSVLooseCut  = OfflineDeepCSVLooseCutPhaseII  ;
  OfflineDeepFlavourTightCut  = OfflineDeepFlavourTightCutPhaseII ;
  OfflineDeepFlavourMediumCut = OfflineDeepFlavourMediumCutPhaseII ;
  OfflineDeepFlavourLooseCut  = OfflineDeepFlavourLooseCutPhaseII ;
  // OfflineDeepCSVTightCut  = OfflineDeepCSVTightCut2017  ;
  // OfflineDeepCSVMediumCut = OfflineDeepCSVMediumCut2017 ;
  // OfflineDeepCSVLooseCut  = OfflineDeepCSVLooseCut2017  ;
  // OfflineDeepFlavourMediumCut = OfflineDeepFlavourMediumCut2017 ;
  // if(_year == "2018"){
  //   OfflineDeepCSVTightCut  = OfflineDeepCSVTightCut2018  ;
  //   OfflineDeepCSVMediumCut = OfflineDeepCSVMediumCut2018 ;
  //   OfflineDeepCSVLooseCut  = OfflineDeepCSVLooseCut2018 ;
  //   OfflineDeepFlavourMediumCut = OfflineDeepFlavourMediumCut2018 ;
  // }



  //
  //  Init the pile-up function
  //
  if(isMC){
    if( PUFileName != ""){
      pileUpTool = new nTupleAnalysis::pileUpWeightTool(PUFileName);
    }else{
      cout << "Skipping PU reweighting" << endl;
    }
  }

  //
  // Adding the NN
  //
  //
  if(nnConfig.getParameter<bool>("reCalcWeights")){
    edm::FileInPath nnInputFile = nnConfig.getParameter<edm::FileInPath>("NNConfig");
    cout << "Creating Neuralnet from  " << nnInputFile << endl;
    neuralNet = std::make_shared<NeuralNetworkAndConstants>(nnConfig);
  }else{
    neuralNet = nullptr;
  }

}


void BTagAnalysis::monitor(long int e){
  //Monitor progress
  percent        = (e+1)*100/nEvents;
  duration       = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  eventRateAVE      = (e+1)/duration;
  timeRemaining  = (nEvents-e)/eventRateAVE;
  minutes = static_cast<int>(timeRemaining/60);
  seconds = static_cast<int>(timeRemaining - minutes*60);
  getrusage(who, &usage);
  usageMB = usage.ru_maxrss/1024;


  timeStep = (std::clock() - lastTime) / (double) CLOCKS_PER_SEC;
  eventStep = e - lastEvent;
  eventRate = eventStep/timeStep;
  //print status and flush stdout so that status bar only uses one line
  if(isMC){
    fprintf(stdout, "\rProcessed: %8li of %li ( %2li%% | %.0f events/s AVE | %.0f events/s  | done in %02i:%02i | memory usage: %li MB)       ",
	    e+1, nEvents, percent,   eventRateAVE,    eventRate, minutes, seconds,                usageMB);
  }else{
    fprintf(stdout, "\rProcessed: %8li of %li ( %2li%% | %.0f events/s AVE | %.0f events/s | done in %02i:%02i | memory usage: %li MB | LumiBlocks %i  )       ",
	    e+1, nEvents, percent,   eventRateAVE,    eventRate, minutes, seconds,                usageMB,            nls );
  }
  fflush(stdout);
  lastTime = std::clock();
  lastEvent = e+1;
}

int BTagAnalysis::eventLoop(int maxEvents, int nSkipEvents){
  //Set Number of events to process. Take manual maxEvents if maxEvents is > 0 and less than the total number of events in the input files.
  nEvents = (maxEvents > 0 && maxEvents < treeEvents) ? maxEvents : treeEvents;

  cout << "\nProcess " << nEvents << " of " << treeEvents << " events.\n";

  start = std::clock();//2546000 //2546043
  lastTime = std::clock();
  lastEvent = 0;
  for(long int e = 0; e < nEvents; e++){

    if(e < nSkipEvents) continue;

    event->update(e);
    processEvent();
    if(debug) event->dump();

    //periodically update status
    //if( (e+1)%1 == 0 || e+1==nEvents || debug)
    if( (e+1)%10000 == 0 || e+1==nEvents || debug)
      monitor(e);

  }


  cout << endl;
  cout << "BTagAnalysis::End of Event Loop" << endl;
  if(!isMC) cout << "Runs " << firstRun << "-" << lastRun << endl;


  minutes = static_cast<int>(duration/60);
  seconds = static_cast<int>(duration - minutes*60);

  if(isMC){
    fprintf(stdout,"---------------------------\nProcessed in %02i:%02i", minutes, seconds);
  }else{
    fprintf(stdout,"---------------------------\nProcessed in %02i:%02i", minutes, seconds);
  }
  return 0;
}

int BTagAnalysis::processEvent(){
  if(debug) cout << "processEvent start" << endl;

  cutflow->Fill("all", 1.0);

  if(event->run != event->runAOD)
    return 0;

  if(event->event != event->eventAOD)
    return 0;

  cutflow->Fill("foundMatch", 1.0);

  if(debug) cout << "Fill/Select Muons" << endl;
  std::vector<nTupleAnalysis::muonPtr> selMuons;
  for(nTupleAnalysis::muonPtr& muon: event->muons){
    hAllMuons->Fill(muon,1.0);
    if(muon->tightId && muon->isolation_corrected < 0.2){
      hSelMuons->Fill(muon,1.0);
      selMuons.push_back(muon);
    }
  }
  hAllMuons->nMuons->Fill(event->muons.size());
  hSelMuons->nMuons->Fill(selMuons.size());




  if(debug) cout << "Fill/Select Elecs" << endl;

  std::vector<nTupleAnalysis::elecPtr> selElecs;
  for(nTupleAnalysis::elecPtr& elec: event->elecs){
    hAllElecs->Fill(elec,1.0);
    if(elec->tightId && elec->isolation_corrected < 0.2){
      hSelElecs->Fill(elec,1.0);
      selElecs.push_back(elec);
    }
  }
  if(debug) cout << "Done Elec Loop" << endl;
  hAllElecs->nElecs->Fill(event->elecs.size());
  hSelElecs->nElecs->Fill(selElecs.size());
  if(debug) cout << "Done Elec Fill " << endl;

  // if(doLeptonSel){
  //   if(debug) cout << "Doing Lepton Cuts " << endl;
  //   if(selMuons.size() == 1)
  //     cutflow->Fill("passMuonCut", 1.0);
  //
  //   if(selElecs.size() == 1)
  //     cutflow->Fill("passElecCut", 1.0);
  //
  //   if(selMuons.size() != 1){
  //     if(debug) cout << "Fail Muon Cut" << endl;
  //     return 0;
  //   }
  //
  //   if(selElecs.size() != 1){
  //     if(debug) cout << "Fail Elec Cut" << endl;
  //     return 0;
  //   }
  //   cutflow->Fill("passLeptonCut", 1.0);
  // }

  float eventWeight = 1.0;
  float puWeight    = 1.0;
  if(isMC && pileUpTool){
    puWeight = pileUpTool->getWeight(event->offPVs.size());
    eventWeight =  puWeight;
    if(doLeptonSel)
      eventWeight *= (selElecs.at(0)->SF * selMuons.at(0)->SF);
  }




  //
  //  Offline BTags
  //
  if(debug) cout << "Count BTags " << endl;
  unsigned int nOffJetsForCut = 0;
  unsigned int nOffJetsTaggedForCut = 0;
  float totalSFWeight = 1.0;
  for(const nTupleAnalysis::jetPtr& offJet : event->offJets){


    for(const nTupleAnalysis::trkTagVarPtr& trkTag: offJet->trkTagVars) {
      hOffBTagsAll->FillTrkTagVarHists(trkTag, eventWeight);
    }

    if(fabs(offJet->eta) > 4) continue;
    if(offJet->pt       < 30)   continue;


    // if(nTupleAnalysis::failOverlap(offJet->p,event->elecs,0.4)) continue;
    // if(nTupleAnalysis::failOverlap(offJet->p,event->muons,0.4)) continue;

    ++nOffJetsForCut;
    if(offJet->DeepCSV > OfflineDeepCSVTightCut) ++nOffJetsTaggedForCut;

    if(isMC)
      totalSFWeight *= offJet->SF;
  }

  // if(nOffJetsForCut < 2      ){
  //   if(debug) cout << "Fail NJet Cut" << endl;
  //   return 0;
  // }
  cutflow->Fill("passNJetCut", eventWeight);
  if(debug) cout << "Pass NJet Cut " << endl;

  // bool doOfflineBTagCut = false;
  // if(doOfflineBTagCut){
  //
  //   if(nOffJetsTaggedForCut < 1) {
  //     if(debug) cout << "Fail NBJet Cut" << endl;
  //     return 0;
  //   }
  //   cutflow->Fill("passNBJetCut", eventWeight);
  // }

  if(debug) cout << "Pass NBJet Cut " << endl;

  if(isMC)
    eventWeight *= totalSFWeight;


  if(debug) cout << "Fill Leptons " << endl;
  hMuons->nMuons->Fill(event->muons.size());
  for(auto muon: event->muons)
    hMuons->Fill(muon,eventWeight);

  hElecs->nElecs->Fill(event->elecs.size());
  for(auto elec: event->elecs)
    hElecs->Fill(elec,eventWeight);


  //
  // Fill All events
  //
  if(debug) cout << "Fill All Events " << endl;
  hEvents->Fill(event->offPVs.size(),  0.0, eventWeight);
  if(puWeight)
    hEventsNoPUWeight->Fill(event->offPVs.size(),  0.0, eventWeight/puWeight);

  hVtx      ->Fill(event->pvs, eventWeight);
  hVtx      ->FillDiffHists(event->pvs, event->offPVs, eventWeight);
  hOffVtx   ->Fill(event->offPVs, eventWeight);

  //
  //
  //
  unsigned int nOffJetsPreOLap      = 0;
  unsigned int nOffJets             = 0;
  unsigned int nOffJets_matched     = 0;
  unsigned int nOffJets_matchedCalo = 0;
  unsigned int nOffJets_matchedPuppi = 0;

  if(debug) cout << "Starting off jets loop  " << endl;
  for(const nTupleAnalysis::jetPtr& offJet : event->offJets){
    cutflowJets->Fill("all", eventWeight);

    if(fabs(offJet->eta) > 4) continue;
    cutflowJets->Fill("eta", eventWeight);

    if(offJet->pt       < 30)   continue;
    cutflowJets->Fill("pt", eventWeight);

    ++nOffJetsPreOLap;
    hOffJetsPreOLap->Fill(offJet, eventWeight);


    // if(nTupleAnalysis::failOverlap(offJet->p,event->muons,0.4)) continue;
    cutflowJets->Fill("muonOlap", eventWeight);

    // if(nTupleAnalysis::failOverlap(offJet->p,event->elecs,0.4)) continue;
    cutflowJets->Fill("elecOlap", eventWeight);



    //
    // Check if jet is a Probe
    //
    unsigned int nOtherTags = 0;
    float min_dR_all  = 1000;
    float min_dR_bjet = 1000;
    //const JetData* tagJet = nullptr;

    if(debug) cout << "Starting off jets loop for probe check " << endl;
    for(const nTupleAnalysis::jetPtr& offJetOther : event->offJets){
      if(offJetOther == offJet) continue;


      if(offJetOther->pt       < 30)   continue;
      if(fabs(offJetOther->eta) > 4) continue;
      // if(nTupleAnalysis::failOverlap(offJetOther->p,event->elecs,0.4)) continue;
      // if(nTupleAnalysis::failOverlap(offJetOther->p,event->muons,0.4)) continue;
      float thisDr = offJetOther->p.DeltaR(offJet->p);
      if(thisDr < min_dR_all) min_dR_all = thisDr;

      // if(offJetOther->DeepCSV       < OfflineDeepCSVTightCut)   continue;

      if(thisDr < min_dR_bjet) min_dR_bjet = thisDr;

      ++nOtherTags;
      //tagJet = &offJetOther;
    }


    if(min_dR_all  < 1000) offJet->match_dR       = min_dR_all;
    if(min_dR_bjet < 1000) offJet->match_dR_bjet = min_dR_bjet;

    // Require that there only be one other probe
    //    Note: this will suppress LF
    // if(nOtherTags != 1) continue;

    cutflowJets->Fill("isProbe", eventWeight);
    if(debug) cout << "Pass Probe cut " << endl;

    //
    //  Offline Jets
    //
    ++nOffJets;
    hOffJets->Fill(offJet,eventWeight);

    if(doPuppiJets){
      hOffJetsPuppi->Fill(offJet,eventWeight);
    }

    if(debug) cout << "Filling offline BTags " << endl;

    for(const nTupleAnalysis::trkTagVarPtr& trkTag: offJet->trkTagVars) {
      hOffBTagsAll->FillTrkTagVarHists(trkTag, eventWeight);
    }

    if(offJet->DeepCSV > 1)
      cout << "Error " << "Offline" << " DeepCSV is " << offJet->DeepCSV << endl;

    if(offJet->DeepCSVb > 1)
      cout << "Error " << "Offline" << " DeepCSVb is " << offJet->DeepCSVb << endl;

    if(offJet->DeepCSVbb > 1)
      cout << "Error " << "Offline" << " DeepCSVbb is " << offJet->DeepCSVbb << endl;


    // Match offline to online
    float dR = 1e6;
    nTupleAnalysis::jetPtr matchedJet = nullptr;

    if(debug) cout << "Matching to PF jets " << endl;
    for(const nTupleAnalysis::jetPtr& pfJet : event->pfJets){
      if(debug) cout << " new jet " << endl;
      float this_dR = pfJet->p.DeltaR(offJet->p);
      if(debug) cout << " this dR " << this_dR << endl;
      if (this_dR < dR){
	dR = this_dR;
	matchedJet = pfJet;
      }
    }


    hDeltaROffPf->Fill(dR,eventWeight);

    //

    //  Have PF Match
    //
    if( dR < 0.4){

      if(debug) cout << "Have a PF jet match " << endl;
      cutflowJets->Fill("hasHLTMatchPF", eventWeight);
      offJet->matchedJet = matchedJet;

      //
      // Testing Neural Net
      //
      if(neuralNet){
	if(debug) cout << "Testing the NN " << endl;

	lwt::ValueMap nnout = neuralNet->compute(matchedJet);
	float DeepCSV_reCalc = nnout["probb"] + nnout["probbb"];
        matchedJet->DeepCSV_reCalc = DeepCSV_reCalc;

	if(fabs(DeepCSV_reCalc - matchedJet->DeepCSV) > 0.001){
	  cout << "Event: " << event->event << endl;
	  cout << "DeepCSV_reCalc: " << DeepCSV_reCalc << " vs " << matchedJet->DeepCSV << endl;
	  nnout = neuralNet->compute(matchedJet, true);
	}
      }


      if(debug) cout << "Doing PFJetAnalysis " << endl;
      PFJetAnalysis(offJet,matchedJet,eventWeight);

      ++nOffJets_matched;


    }//offJet has match


    //
    //  Calo Jets
    //
    if(doCaloJets){
      float dRCalo = 1e6;
      nTupleAnalysis::jetPtr matchedCaloJet = nullptr;

      for(const nTupleAnalysis::jetPtr& caloJet : event->caloJets){
	float this_dR = caloJet->p.DeltaR(offJet->p);
	if (this_dR < dRCalo){
	  dRCalo = this_dR;
	  matchedCaloJet = caloJet;
	}
      }


      if( dRCalo < 0.4){

	cutflowJets->Fill("hasHLTMatchCalo", eventWeight);

	CaloJetAnalysis(offJet,matchedCaloJet,eventWeight);

	++nOffJets_matchedCalo;
      }
    }
    //
    //  Puppi Jets
    //
    if(doPuppiJets){
      float dRPuppi = 1e6;
      nTupleAnalysis::jetPtr matchedPuppiJet = nullptr;

      for(const nTupleAnalysis::jetPtr& puppiJet : event->puppiJets){
	float this_dR = puppiJet->p.DeltaR(offJet->p);
	if (this_dR < dRPuppi){
	  dRPuppi = this_dR;
	  matchedPuppiJet = puppiJet;
	}
      }


      if( dRPuppi < 0.4){

	cutflowJets->Fill("hasHLTMatchPuppi", eventWeight);
  offJet->matchedJet = matchedPuppiJet;

	PuppiJetAnalysis(offJet,matchedPuppiJet,eventWeight);

	++nOffJets_matchedPuppi;
      }
    }
  }//offJets


  hOffJetsPreOLap    ->nJets->Fill(nOffJetsPreOLap ,eventWeight);
  hOffJets           ->nJets->Fill(nOffJets        ,eventWeight);
  hOffJets_matched   ->nJets->Fill(nOffJets_matched,eventWeight);

  if(doCaloJets){
    hOffJets_matchedCalo->nJets->Fill(nOffJets_matchedCalo,eventWeight);
    hOffJets_matchedCaloJet->nJets->Fill(nOffJets_matchedCalo,eventWeight);
  }
  if(doPuppiJets){
    hOffJetsPuppi           ->nJets->Fill(nOffJets        ,eventWeight);
    //hOffJets_matchedPuppi->nJets->Fill(nOffJets_matchedPuppi,eventWeight);
    hOffJets_matchedPuppi->nJets->Fill(nOffJets_matchedPuppi,eventWeight);
    hOffJets_matchedPuppiJet->nJets->Fill(nOffJets_matchedPuppi,eventWeight);
  }



  //
  //  pf Jets
  //
  for(const nTupleAnalysis::jetPtr& pfJet : event->pfJets){
    if(fabs(pfJet->eta) > 4) continue;
    if(pfJet->pt       < 30)   continue;

    //pfJetHistsPreOLap.Fill(pfJet);

    // if(nTupleAnalysis::failOverlap(pfJet->p,event->elecs, 0.4)) continue;
    // if(nTupleAnalysis::failOverlap(pfJet->p,event->muons, 0.4)) continue;

    hPfJets->Fill(pfJet, eventWeight);

    const nTupleAnalysis::jetPtr pfJetMatchedJet = pfJet->matchedJet.lock();
    if(pfJetMatchedJet){
      hPfJets_matched->Fill(pfJet, eventWeight);

      //if(isMC){
      //	if( pfJetMatchedJet->hadronFlavour == 5){
      //	  hPfJets_matched_B->Fill(pfJet, eventWeight);
      //	}else if( pfJetMatchedJet->hadronFlavour == 4){
      //	  hPfJets_matched_C->Fill(pfJet, eventWeight);
      //	}else if( pfJetMatchedJet->hadronFlavour == 0){
      //	  hPfJets_matched_L->Fill(pfJet, eventWeight);
      //	}
      //}
    }
  }

  //
  //  calo Jets
  //

  if(doCaloJets){
    for(const nTupleAnalysis::jetPtr& caloJet : event->caloJets){
      if(fabs(caloJet->eta) > 4) continue;
      if(caloJet->pt       < 30)   continue;

      //caloJetHistsPreOLap.Fill(caloJet);
      // if(nTupleAnalysis::failOverlap(caloJet->p,event->elecs, 0.4)) continue;
      // if(nTupleAnalysis::failOverlap(caloJet->p,event->muons, 0.4)) continue;

      hCaloJets->Fill(caloJet, eventWeight);

      if(caloJet->DeepCSV > 1){
	       cout << "Error " << "Offline" << " DeepCSV is " << caloJet->DeepCSV << endl;
       }

      if(caloJet->DeepCSVb > 1){
  	     cout << "Error " << "Offline" << " DeepCSVb is " << caloJet->DeepCSVb << endl;
       }

      if(caloJet->DeepCSVbb > 1){
	       cout << "Error " << "Offline" << " DeepCSVbb is " << caloJet->DeepCSVbb << endl;
      }

      if(caloJet->DeepCSVb > 1){
        std::cout << "Error " << "Offline" << " DeepCSVb is " << caloJet->DeepCSVb << std::endl;
      }


      const nTupleAnalysis::jetPtr caloJetMatchedJet = caloJet->matchedJet.lock();
      if(caloJetMatchedJet){
	hCaloJets_matched->Fill(caloJet, eventWeight);

	//if(caloJet->m_matchedJet->m_hadronFlavour == 5)
	//	caloJetHists_matchedB.Fill(caloJet);
	//	else
	//	  caloJetHists_matchedL.Fill(caloJet);
      }
    }
  }

  //
  //  Puppi Jets
  //

  if(doPuppiJets){
    for(const nTupleAnalysis::jetPtr& puppiJet : event->puppiJets){
      if(fabs(puppiJet->eta) > 4) continue;
      if(puppiJet->pt       < 30)   continue;

      //puppiJetHistsPreOLap.Fill(puppiJet);
      // if(nTupleAnalysis::failOverlap(puppiJet->p,event->elecs, 0.4)) continue;
      // if(nTupleAnalysis::failOverlap(puppiJet->p,event->muons, 0.4)) continue;

      hPuppiJets->Fill(puppiJet, eventWeight);

      if(puppiJet->DeepCSV > 1){
	       cout << "Error " << "Offline" << " DeepCSV is " << puppiJet->DeepCSV << endl;
      }

      if(puppiJet->DeepCSVb > 1){
	       cout << "Error " << "Offline" << " DeepCSVb is " << puppiJet->DeepCSVb << endl;
       }

      if(puppiJet->DeepCSVbb > 1){
	       cout << "Error " << "Offline" << " DeepCSVbb is " << puppiJet->DeepCSVbb << endl;
       }

    if(puppiJet->DeepCSVb > 1){
      std::cout << "Error " << "Offline" << " DeepCSVb is " << puppiJet->DeepCSVb << std::endl;
    }


      const nTupleAnalysis::jetPtr puppiJetMatchedJet = puppiJet->matchedJet.lock();
      if(puppiJetMatchedJet){
      	hPuppiJets_matched->Fill(puppiJet, eventWeight);

      	//if(puppiJet->m_matchedJet->m_hadronFlavour == 5)
      	//	puppiJetHists_matchedB.Fill(puppiJet);
      	//	else
      	//	  puppiJetHists_matchedL.Fill(puppiJet);
      }
    }
  }







  //
  //if we are processing data, first apply lumiMask and trigger
  //
  if(!isMC){
    if(!passLumiMask()){
      if(debug) cout << "Fail lumiMask" << endl;
      return 0;
    }
    //cutflow->Fill(event, "lumiMask", true);

    //keep track of total lumi

    //if(!event->passHLT){
    //  if(debug) cout << "Fail HLT: data" << endl;
    //  return 0;
    //}
    //cutflow->Fill(event, "HLT", true);
  }
  //if(allEvents != NULL && event->passHLT) allEvents->Fill(event);





  return 0;
}

bool BTagAnalysis::passLumiMask(){
  // if the lumiMask is empty, then no JSON file was provided so all
  // events should pass
  if(lumiMask.empty()) return true;

  //make lumiID run:lumiBlock
  //edm::LuminosityBlockID lumiID(event->run, event->lumiBlock);

  //define function that checks if a lumiID is contained in a lumiBlockRange
  //bool (*funcPtr) (edm::LuminosityBlockRange const &, edm::LuminosityBlockID const &) = &edm::contains;

  //Loop over the lumiMask and use funcPtr to check for a match
  //std::vector< edm::LuminosityBlockRange >::const_iterator iter = std::find_if (lumiMask.begin(), lumiMask.end(), boost::bind(funcPtr, _1, lumiID) );

  //return lumiMask.end() != iter;
  return true;
}



BTagAnalysis::~BTagAnalysis(){
  cout << "BTagAnalysis::destroyed" << endl;
}


//
//  Set Match tracks
//
void BTagAnalysis::OfflineToOnlineTrackMatching(const nTupleAnalysis::jetPtr& offJet, const nTupleAnalysis::trackPtr& offTrk,
						const nTupleAnalysis::jetPtr& hltJet,
						float dRMatch){

  float dR = 1e6;
  float dR2 = 1e6;

  nTupleAnalysis::trackPtr matchedTrack  = nullptr;
  nTupleAnalysis::trackPtr secondClosest = nullptr;

  for(const nTupleAnalysis::trackPtr& hltTrack: hltJet->tracks){

    //need to check that the track (with matching resolution cone r=0.01) is in region where R=0.3 circles inside the two jets overlap!
    if(hltTrack->dR                  > 0.29) continue; // hltTrack is not in cone of pfJet
    if(hltTrack->p.DeltaR(offJet->p) > 0.29) continue; // hltTrack is not in cone of offJet

    float this_dR = offTrk->p.DeltaR(hltTrack->p);

    if(this_dR > dR && this_dR < dR2){
      dR2 = this_dR;
      secondClosest = hltTrack;
    }

    if(this_dR < dR){
      dR2 = dR;
      secondClosest = matchedTrack;

      dR  = this_dR;
      matchedTrack = hltTrack;
    }

  }// matched pf tracks


  if(dR < dRMatch){
    matchedTrack->matchedTrack = offTrk;
    matchedTrack->nMatches += 1;

    offTrk->matchedTrack     = matchedTrack;
    offTrk->secondClosest    = secondClosest;
    offTrk->nMatches              += 1;
  }

  return;
}

//
//  Set Match Track Tags
//
void BTagAnalysis::OfflineToOnlineTrkTagMatching(const nTupleAnalysis::jetPtr& offJet, const nTupleAnalysis::trkTagVarPtr& offTrkTag,
						 const nTupleAnalysis::jetPtr& hltJet,
						 float dRMatch){

  float dR = 1e6;
  nTupleAnalysis::trkTagVarPtr matchedTrkTag  = nullptr;
  for(const nTupleAnalysis::trkTagVarPtr& hltTrkTag: hltJet->trkTagVars){
    if(hltTrkTag->trackDeltaR                  > 0.29) continue; // hltTrack is not in cone of hltJet
    if(hltTrkTag->p.DeltaR(offJet->p) > 0.29) continue; // hltTrack is not in cone of offJet

    float this_dR = offTrkTag->p.DeltaR(hltTrkTag->p);
    if(this_dR < dR){
      dR  = this_dR;
      matchedTrkTag = hltTrkTag;
    }


  }

  if(dR < dRMatch){
    matchedTrkTag->matchedTrkTagVar = offTrkTag;
    offTrkTag->matchedTrkTagVar     = matchedTrkTag;
  }//else{
    //cout << " dR  " << dR << endl;
    //cout << " \t offtrk  " << offTrkTag->trackEta << " " << offTrkTag->trackPhi << endl;
    //if(matchedTrkTag)
    //  cout << " \t hlttrk  " << matchedTrkTag->trackEta << " " << matchedTrkTag->trackPhi << endl;
  //}

  return;
}



//
//  Set Match Track Tags
//
void BTagAnalysis::OfflineToOnlineSVMatching(const nTupleAnalysis::svPtr& offSV,
					     const nTupleAnalysis::jetPtr& hltJet,
					     float dRMatch){

  float dR2 = 1e6;

  nTupleAnalysis::svPtr matchedSV  = nullptr;
  for(const nTupleAnalysis::svPtr& hltSV: hltJet->svs){

    float dX = (offSV->x - hltSV->x);
    float dY = (offSV->y - hltSV->y);
    float dZ = (offSV->z - hltSV->z);
    float this_dR2 =  dX*dX + dY*dY + dZ*dZ;

    if(this_dR2 < dR2){
      dR2  = this_dR2;
      matchedSV = hltSV;
    }

  }

  if(dR2 < (dRMatch*dRMatch)){
    matchedSV->matchedSV = offSV;
    offSV     ->matchedSV     = matchedSV;
  }

  return;
}



void BTagAnalysis::PFJetAnalysis(const nTupleAnalysis::jetPtr& offJet,const nTupleAnalysis::jetPtr& hltJet, float weight){
  if(debug) cout << "In PFJetAnalysis " << endl;

  if(doTracks){
    if(debug) cout << " ... doing tracks " << endl;

    //
    //  Off tracks
    //
    unsigned int nOffTracks = 0;
    unsigned int nOffTracks_matched = 0;
    unsigned int nOffTracks_noV0 = 0;
    unsigned int nOffTracks_matched_noV0 = 0;


    for(const nTupleAnalysis::trackPtr& offTrack: offJet->tracks){

      //
      //  track mass calculation
      //
      for(const nTupleAnalysis::trackPtr& offTrack_pair: offJet->tracks){
        if(offTrack == offTrack_pair) continue;
        float thisMass = (offTrack->p + offTrack_pair->p).M();

        hmttOff->Fill(thisMass , weight);
        if(offTrack->isfromV0)
	  hmttOff_isFromV0->Fill(thisMass , weight);

      }

      //need to check that the track (with matching resolution cone r=0.01) is in region where R=0.3 circles inside the two jets overlap!
      //if offTrack.dR > 0.29 - offJet.match_dR: continue
      if(offTrack->dR                  > 0.29) continue; // offTrack is not in cone of offJet
      if(offTrack->p.DeltaR(hltJet->p) > 0.29) continue; // offTrack is not in cone of pfJet


      hOffTracks->Fill(offTrack, weight);
      ++nOffTracks;
      if(!offTrack->isfromV0){
        hOffTracks_noV0->Fill(offTrack, weight);
        ++nOffTracks_noV0;
      }

      //
      //  Match offline track with PF Track
      //
      OfflineToOnlineTrackMatching(offJet, offTrack, hltJet, 0.01);

      const nTupleAnalysis::trackPtr offTrackMatchedTrack = offTrack->matchedTrack.lock();

      if(offTrackMatchedTrack){

	if(!offTrack->isfromV0 && !offTrackMatchedTrack->isfromV0){
	  hOffTracks_matched_noV0->Fill(offTrack, weight);
	  ++nOffTracks_matched_noV0;
	}

	++nOffTracks_matched;
	hOffTracks_matched    ->Fill(offTrack, weight);
	hPfTracks_matched ->Fill(offTrackMatchedTrack,weight);

      }else{
        hOffTracks_unmatched->Fill(offTrack, weight);
      }

    }//off Tracks

    hOffTracks             ->nTracks->Fill(nOffTracks, weight);
    hOffTracks_matched     ->nTracks->Fill(nOffTracks_matched, weight);
    hOffTracks_noV0        ->nTracks->Fill(nOffTracks_noV0, weight);
    hOffTracks_matched_noV0->nTracks->Fill(nOffTracks_matched_noV0, weight);


    //
    //  Off BTags trkTagVars
    //
    unsigned int nTrkTags = 0;
    unsigned int nTrkTags_matched = 0;
    unsigned int nTrkTags_noV0 = 0;
    unsigned int nTrkTags_matched_noV0 = 0;

    for(const nTupleAnalysis::trkTagVarPtr& offTrkTag: offJet->trkTagVars){

      //need to check that the track (with matching resolution cone r=0.01) is in region where R=0.3 circles inside the two jets overlap!
      //if offTrack.dR > 0.29 - offJet.match_dR: continue
      if(offTrkTag->trackDeltaR                              > 0.29) continue; // offTrack is not in cone of offJet
      if(offTrkTag->p.DeltaR(hltJet->p) > 0.29) continue; // offTrack is not in cone of pfJet

      hOffBTags->FillTrkTagVarHists(offTrkTag, weight);
      ++nTrkTags;

      if(!offTrkTag->matchIsFromV0){
        hOffBTags_noV0->FillTrkTagVarHists(offTrkTag, weight);
        ++nTrkTags_noV0;
      }

      OfflineToOnlineTrkTagMatching(offJet, offTrkTag, hltJet, 0.01);

      const nTupleAnalysis::trkTagVarPtr offTrackMatchedTrkTag = offTrkTag->matchedTrkTagVar.lock();
      if(offTrackMatchedTrkTag){

	if(!offTrkTag->matchIsFromV0 && !offTrackMatchedTrkTag->matchIsFromV0){
	  hOffBTags_matched_noV0->FillTrkTagVarHists(offTrkTag, weight);
	  ++nTrkTags_matched_noV0;
	}

	++nTrkTags_matched;
	//cout << "Filling offBTag_match" << endl;
	hOffBTags_matched    ->FillTrkTagVarHists(offTrkTag, weight);
	//const nTupleAnalysis::trackPtr offTrackMatchedTrkTag = offTrkTag->matchedTrkTagVar.lock();
	//hPfTracks_matched ->FillTrkTagVarHists(offTrackMatchedTrkTag,weight);
      }else{
	hOffBTags_unmatched    ->FillTrkTagVarHists(offTrkTag, weight);
      }

    }//OffTrkTag

    hOffBTags             ->trkTag_nTracks->Fill(nTrkTags, weight);
    hOffBTags_matched     ->trkTag_nTracks->Fill(nTrkTags_matched, weight);
    hOffBTags_noV0        ->trkTag_nTracks->Fill(nTrkTags_noV0, weight);
    hOffBTags_matched_noV0->trkTag_nTracks->Fill(nTrkTags_matched_noV0, weight);


    //
    //  Off BTags SVs
    //
    for(const nTupleAnalysis::svPtr& offSV: offJet->svs){

      OfflineToOnlineSVMatching(offSV, hltJet, 0.01);

    }//OffTrkTag


    //
    // PF Tracks
    //
    unsigned int nPfTracks = 0;
    unsigned int nPfTracks_matched = 0;
    unsigned int nPfTracks_noV0 = 0;
    for(const nTupleAnalysis::trackPtr& pfTrack: hltJet->tracks){

      //
      //  track mass calculation
      //
      for(const nTupleAnalysis::trackPtr& pfTrack_pair: hltJet->tracks){
        if(pfTrack == pfTrack_pair) continue;
        float thisMass = (pfTrack->p + pfTrack_pair->p).M();

        hmttPf->Fill(thisMass , weight);
        if(pfTrack->isfromV0)
	  hmttPf_isFromV0->Fill(thisMass , weight);

      }

      //need to check that the track (with matching resolution cone r=0.01) is in region where R=0.3 circles inside the two jets overlap!
      if(pfTrack->dR                  > 0.29) continue; // pfTrack is not in cone of pfJet
      if(pfTrack->p.DeltaR(offJet->p) > 0.29) continue; // pfTrack is not in cone of offJet

      hPfTracks->Fill(pfTrack, weight); //all pftracks in matched jets
      hPfTracks->FillMatchStats(pfTrack, weight); //check how often we match pfTracks to more than one offTrack
      ++nPfTracks;

      if(!pfTrack->isfromV0){
        hPfTracks_noV0->Fill(pfTrack, weight); //all pftracks in matched jets
        ++nPfTracks_noV0;
      }

      if(!pfTrack->nMatches){
        hPfTracks_unmatched->Fill(pfTrack, weight); //all unmatched pftracks
        hPfTracks_unmatched->FillMatchStats(pfTrack, weight);
      }else{
        hPfTracks_matched->FillMatchStats(pfTrack, weight);
        ++nPfTracks_matched;
      }
    }// pfTracks

    hPfTracks              ->nTracks->Fill(nPfTracks, weight);
    hPfTracks_matched      ->nTracks->Fill(nPfTracks_matched, weight);
    hPfTracks_noV0         ->nTracks->Fill(nPfTracks_noV0, weight);
    //hOffTracks_matched_noV0->nTracks->Fill(nOffTracks_matched_noV0, weight);


    //
    //  PF BTags
    //
    unsigned int nPfTrkTags = 0;

    for(const nTupleAnalysis::trkTagVarPtr& pfTrkTag: hltJet->trkTagVars){
      //need to check that the track (with matching resolution cone r=0.01) is in region where R=0.3 circles inside the two jets overlap!
      //if offTrack.dR > 0.29 - offJet.match_dR: continue
      if(pfTrkTag->trackDeltaR                              > 0.29) continue; // offTrack is not in cone of offJet
      if(pfTrkTag->p.DeltaR(offJet->p) > 0.29) continue; // offTrack is not in cone of pfJet


      hPfBTags->FillTrkTagVarHists(pfTrkTag, weight);
      const nTupleAnalysis::trkTagVarPtr pfTrkTagMatch = pfTrkTag->matchedTrkTagVar.lock();
      if(pfTrkTagMatch){
	hPfBTags_matched  ->FillTrkTagVarHists(pfTrkTag, weight);
      }else{
	hPfBTags_unmatched->FillTrkTagVarHists(pfTrkTag, weight);
      }

      ++nPfTrkTags;

    }//OffTrkTag

    hPfBTags             ->trkTag_nTracks->Fill(nPfTrkTags, weight);

  }//doTracks


  //
  // Jet info
  //
  if(debug) cout << " ... doing jet info " << endl;
  hOffJets_matched->Fill(offJet,weight);
  hOffJets_matchedJet->Fill(hltJet,weight);
  if(abs(hltJet->p.Eta())<1.5){
      hOffJets_matched_eta1->Fill(offJet,weight);
      hOffJets_matchedJet_eta1->Fill(hltJet,weight);
  }
  if(abs(hltJet->p.Eta())<2.4){
      hOffJets_matched_eta2p4->Fill(offJet,weight);
      hOffJets_matchedJet_eta2p4->Fill(hltJet,weight);
  }
  if(abs(hltJet->p.Eta())>=1.5 && abs(hltJet->p.Eta())<3.){
      hOffJets_matched_eta2->Fill(offJet,weight);
      hOffJets_matchedJet_eta2->Fill(hltJet,weight);
  }
  if(abs(hltJet->p.Eta())>=3){
      hOffJets_matched_eta3->Fill(offJet,weight);
      hOffJets_matchedJet_eta3->Fill(hltJet,weight);
  }

  //
  // Offline Btags
  //
  if((offJet->DeepCSV > OfflineDeepCSVTightCut)){
    hOffJetTightDeepCSV_matchedPFJet->Fill(hltJet, weight);
}
  if((offJet->DeepCSV > OfflineDeepCSVMediumCut)){
    hOffJetMediumDeepCSV_matchedPFJet->Fill(hltJet, weight);
    hOffJetMedDeepCSV_matchedPFJet    ->Fill(offJet, weight);
    if(hltJet->CSVv2   > OnlineCSVCutPF)     hOffJetMedDeepCSV_matchedPFCSV    ->Fill(offJet, weight);
    if(hltJet->DeepCSV > OnlineDeepCSVCutPF) hOffJetMedDeepCSV_matchedPFDeepCSV->Fill(offJet, weight);
  }

  if((offJet->DeepCSV > OfflineDeepCSVLooseCut)){
    hOffJetLooseDeepCSV_matchedPFJet->Fill(hltJet, weight);
    }

  if(offJet->DeepJet > OfflineDeepFlavourMediumCut){
    hOffJetMedDeepFlav_matchedPFJet ->Fill(offJet, weight);
    if(hltJet->CSVv2   > OnlineCSVCutPF)     hOffJetMedDeepFlav_matchedPFCSV    ->Fill(offJet, weight);
    if(hltJet->DeepCSV > OnlineDeepCSVCutPF) hOffJetMedDeepFlav_matchedPFDeepCSV->Fill(offJet, weight);
  }

  //
  // If pass CVS working point
  //
  if(hltJet->CSVv2 >= OnlineCSVCutPF){
    hOffJet_matchedPFcsvTag   ->Fill(offJet, weight);
    hOffJet_matchedPFcsvTagJet->Fill(hltJet, weight);
  }


  //
  // If pass DeepCVS working point
  //
  if(hltJet->DeepCSV >= OnlineDeepCSVCutPF){
    hOffJet_matchedPFDeepcsvTag   ->Fill(offJet, weight);
    hOffJet_matchedPFDeepcsvTagJet->Fill(hltJet, weight);
  }


  if(isMC){
    if(offJet->hadronFlavour == 5){
      hOffJets_matched_B->Fill(offJet, weight);
      hOffJets_matchedJet_B->Fill(hltJet, weight);
      if(abs(hltJet->p.Eta())<1.5){
          hOffJets_matched_B_eta1->Fill(offJet, weight);
          hOffJets_matchedJet_B_eta1->Fill(hltJet,weight);
      }
      if(abs(hltJet->p.Eta())<2.4){
          hOffJets_matched_B_eta2p4->Fill(offJet, weight);
          hOffJets_matchedJet_B_eta2p4->Fill(hltJet,weight);
      }
      if(abs(hltJet->p.Eta())>=1.5 && abs(hltJet->p.Eta())<3.){
          hOffJets_matched_B_eta2->Fill(offJet, weight);
          hOffJets_matchedJet_B_eta2->Fill(hltJet,weight);
      }
      if(abs(hltJet->p.Eta())>=3){
          hOffJets_matched_B_eta3->Fill(offJet, weight);
          hOffJets_matchedJet_B_eta3->Fill(hltJet,weight);
      }
    }else if(offJet->hadronFlavour == 4){
      hOffJets_matched_C->Fill(offJet, weight);
      hOffJets_matchedJet_C->Fill(hltJet, weight);
      if(abs(hltJet->p.Eta())<1.5){
          hOffJets_matched_C_eta1->Fill(offJet, weight);
          hOffJets_matchedJet_C_eta1->Fill(hltJet,weight);
      }
      if(abs(hltJet->p.Eta())<2.4){
          hOffJets_matched_C_eta2p4->Fill(offJet, weight);
          hOffJets_matchedJet_C_eta2p4->Fill(hltJet,weight);
      }
      if(abs(hltJet->p.Eta())>=1.5 && abs(hltJet->p.Eta())<3.){
          hOffJets_matched_C_eta2->Fill(offJet, weight);
          hOffJets_matchedJet_C_eta2->Fill(hltJet,weight);
      }
      if(abs(hltJet->p.Eta())>=3){
          hOffJets_matched_C_eta3->Fill(offJet, weight);
          hOffJets_matchedJet_C_eta3->Fill(hltJet,weight);
      }
    }else if(offJet->hadronFlavour == 0){
      hOffJets_matched_LandPU->Fill(offJet, weight);
      hOffJets_matchedJet_LandPU->Fill(hltJet, weight);
      if(abs(hltJet->p.Eta())<1.5){
          hOffJets_matched_LandPU_eta1->Fill(offJet, weight);
          hOffJets_matchedJet_LandPU_eta1->Fill(hltJet,weight);
      }
      if(abs(hltJet->p.Eta())<2.4){
          hOffJets_matched_LandPU_eta2p4->Fill(offJet, weight);
          hOffJets_matchedJet_LandPU_eta2p4->Fill(hltJet,weight);
      }
      if(abs(hltJet->p.Eta())>=1.5 && abs(hltJet->p.Eta())<3.){
          hOffJets_matched_LandPU_eta2->Fill(offJet, weight);
          hOffJets_matchedJet_LandPU_eta2->Fill(hltJet,weight);
      }
      if(abs(hltJet->p.Eta())>=3){
          hOffJets_matched_LandPU_eta3->Fill(offJet, weight);
          hOffJets_matchedJet_LandPU_eta3->Fill(hltJet,weight);
      }
      if(offJet->flavourCleaned!=0){
        hOffJets_matched_L->Fill(offJet, weight);
        hOffJets_matchedJet_L->Fill(hltJet, weight);
        if(abs(hltJet->p.Eta())<1.5){
            hOffJets_matched_L_eta1->Fill(offJet, weight);
            hOffJets_matchedJet_L_eta1->Fill(hltJet,weight);
        }
        if(abs(hltJet->p.Eta())<2.4){
            hOffJets_matched_L_eta2p4->Fill(offJet, weight);
            hOffJets_matchedJet_L_eta2p4->Fill(hltJet,weight);
        }
        if(abs(hltJet->p.Eta())>=1.5 && abs(hltJet->p.Eta())<3.){
            hOffJets_matched_L_eta2->Fill(offJet, weight);
            hOffJets_matchedJet_L_eta2->Fill(hltJet,weight);
        }
        if(abs(hltJet->p.Eta())>=3){
            hOffJets_matched_L_eta3->Fill(offJet, weight);
            hOffJets_matchedJet_L_eta3->Fill(hltJet,weight);
        }
      }
    }
  }


  return;
}


void BTagAnalysis::CaloJetAnalysis(const nTupleAnalysis::jetPtr& offJet,const nTupleAnalysis::jetPtr& hltJet, float weight){



  if(doTracks){

    //
    //  Off tracks
    //
    unsigned int nOffTracks = 0;
    unsigned int nOffTracks_matched = 0;
    unsigned int nOffTracks_noV0 = 0;
    unsigned int nOffTracks_matched_noV0 = 0;


    for(const nTupleAnalysis::trackPtr& offTrack: offJet->tracks){

      //need to check that the track (with matching resolution cone r=0.01) is in region where R=0.3 circles inside the two jets overlap!
      //if offTrack.dR > 0.29 - offJet.match_dR: continue
      if(offTrack->dR                  > 0.29) continue; // offTrack is not in cone of offJet
      if(offTrack->p.DeltaR(hltJet->p) > 0.29) continue; // offTrack is not in cone of caloJet


      hOffTracksCalo->Fill(offTrack, weight);
      ++nOffTracks;
      if(!offTrack->isfromV0){
	hOffTracksCalo_noV0->Fill(offTrack, weight);
	++nOffTracks_noV0;
      }

      //
      //  Match offline track with CaloJet Track
      //
      OfflineToOnlineTrackMatching(offJet, offTrack, hltJet, 0.01);

      const nTupleAnalysis::trackPtr offTrackMatchedTrack = offTrack->matchedTrack.lock();

      if(offTrackMatchedTrack){

	if(!offTrack->isfromV0 && !offTrackMatchedTrack->isfromV0){
	  hOffTracksCalo_matched_noV0->Fill(offTrack, weight);
	  ++nOffTracks_matched_noV0;
	}

	++nOffTracks_matched;
	hOffTracksCalo_matched    ->Fill(offTrack, weight);
	hCaloTracks_matched ->Fill(offTrackMatchedTrack,weight);

      }else{

	hOffTracksCalo_unmatched->Fill(offTrack, weight);

      }

    }//off Tracks

    hOffTracksCalo             ->nTracks->Fill(nOffTracks, weight);
    hOffTracksCalo_matched     ->nTracks->Fill(nOffTracks_matched, weight);
    hOffTracksCalo_noV0        ->nTracks->Fill(nOffTracks_noV0, weight);
    hOffTracksCalo_matched_noV0->nTracks->Fill(nOffTracks_matched_noV0, weight);


    //
    // Calo Tracks
    //
    unsigned int nCaloTracks = 0;
    unsigned int nCaloTracks_matched = 0;
    unsigned int nCaloTracks_noV0 = 0;
    for(const nTupleAnalysis::trackPtr& caloTrack: hltJet->tracks){

      //
      //  track mass calculation
      //
      for(const nTupleAnalysis::trackPtr& caloTrack_pair: hltJet->tracks){
	if(caloTrack == caloTrack_pair) continue;
	float thisMass = (caloTrack->p + caloTrack_pair->p).M();

	hmttCalo->Fill(thisMass , weight);
	if(caloTrack->isfromV0)
	  hmttCalo_isFromV0->Fill(thisMass , weight);

      }

      //need to check that the track (with matching resolution cone r=0.01) is in region where R=0.3 circles inside the two jets overlap!
      if(caloTrack->dR                  > 0.29) continue; // caloTrack is not in cone of caloJet
      if(caloTrack->p.DeltaR(offJet->p) > 0.29) continue; // caloTrack is not in cone of offJet

      hCaloTracks->Fill(caloTrack, weight); //all calotracks in matched jets
      hCaloTracks->FillMatchStats(caloTrack, weight); //check how often we match caloTracks to more than one offTrack
      ++nCaloTracks;

      if(!caloTrack->isfromV0){
	hCaloTracks_noV0->Fill(caloTrack, weight); //all calotracks in matched jets
	++nCaloTracks_noV0;
      }

      if(!caloTrack->nMatches){
	hCaloTracks_unmatched->Fill(caloTrack, weight); //all unmatched pftracks
	hCaloTracks_unmatched->FillMatchStats(caloTrack, weight);
      }else{
	hCaloTracks_matched->FillMatchStats(caloTrack, weight);
	++nCaloTracks_matched;
      }
    }// caloTracks

    hCaloTracks              ->nTracks->Fill(nCaloTracks, weight);
    hCaloTracks_matched      ->nTracks->Fill(nCaloTracks_matched, weight);
    hCaloTracks_noV0         ->nTracks->Fill(nCaloTracks_noV0, weight);
    //hOffTracks_matched_noV0->nTracks->Fill(nOffTracks_matched_noV0, weight);

  }// doTracks

  //
  // Jet info
  //
  hOffJets_matchedCalo   ->Fill(offJet,weight);
  hOffJets_matchedCaloJet->Fill(hltJet,weight);

  //
  // Offline Btaggs
  //
  if((offJet->DeepCSV > OfflineDeepCSVTightCut))
    hOffJetTightDeepCSV_matchedCaloJet->Fill(hltJet, weight);
  if((offJet->DeepCSV > OfflineDeepCSVMediumCut)){
    hOffJetMediumDeepCSV_matchedCaloJet->Fill(hltJet, weight);
    hOffJetMedDeepCSV_matchedCaloJet    ->Fill(offJet, weight);
    if(hltJet->CSVv2   > OnlineCSVCutPF)     hOffJetMedDeepCSV_matchedCaloCSV    ->Fill(offJet, weight);
    if(hltJet->DeepCSV > OnlineDeepCSVCutPF) hOffJetMedDeepCSV_matchedCaloDeepCSV->Fill(offJet, weight);
  }

  if((offJet->DeepCSV > OfflineDeepCSVLooseCut))
    hOffJetLooseDeepCSV_matchedCaloJet->Fill(hltJet, weight);

  if(offJet->DeepJet > OfflineDeepFlavourMediumCut){
    hOffJetMedDeepFlav_matchedCaloJet ->Fill(offJet, weight);
    if(hltJet->CSVv2   > OnlineCSVCutCalo)     hOffJetMedDeepFlav_matchedCaloCSV    ->Fill(offJet, weight);
    if(hltJet->DeepCSV > OnlineDeepCSVCutCalo) hOffJetMedDeepFlav_matchedCaloDeepCSV->Fill(offJet, weight);
  }


  //
  // If pass CVS working point
  //
  if(hltJet->CSVv2 >= OnlineCSVCutCalo){
    hOffJet_matchedCalocsvTag   ->Fill(offJet, weight);
    hOffJet_matchedCalocsvTagJet->Fill(hltJet, weight);
  }


  //
  // If pass DeepCVS working point
  //
  if(hltJet->DeepCSV >= OnlineDeepCSVCutCalo){
    hOffJet_matchedCaloDeepcsvTag   ->Fill(offJet, weight);
    hOffJet_matchedCaloDeepcsvTagJet->Fill(hltJet, weight);
  }


  if(isMC){
    if(offJet->hadronFlavour == 5){
      hOffJets_matchedCalo_B->Fill(offJet, weight);
      hOffJets_matchedCaloJet_B->Fill(hltJet, weight);
    }else if(offJet->hadronFlavour == 4){
      hOffJets_matchedCalo_C->Fill(offJet, weight);
      hOffJets_matchedCaloJet_C->Fill(hltJet, weight);
    }else if(offJet->hadronFlavour == 0){
      hOffJets_matchedCalo_LandPU->Fill(offJet, weight);
      hOffJets_matchedCaloJet_LandPU->Fill(hltJet, weight);
      if(offJet->flavourCleaned!=0){
        hOffJets_matchedCalo_L->Fill(offJet, weight);
        hOffJets_matchedCaloJet_L->Fill(hltJet, weight);
      }
    }
  }



  return;
}

void BTagAnalysis::PuppiJetAnalysis(const nTupleAnalysis::jetPtr& offJet,const nTupleAnalysis::jetPtr& hltJet, float weight){


      if(doTracks){

        //
        //  Off tracks
        //
        unsigned int nOffTracks = 0;
        unsigned int nOffTracks_matched = 0;
        unsigned int nOffTracks_noV0 = 0;
        unsigned int nOffTracks_matched_noV0 = 0;


        for(const nTupleAnalysis::trackPtr& offTrack: offJet->tracks){

          //
          //  track mass calculation
          //
          for(const nTupleAnalysis::trackPtr& offTrack_pair: offJet->tracks){
            if(offTrack == offTrack_pair) continue;
            // float thisMass = (offTrack->p + offTrack_pair->p).M();
            // hmttOff->Fill(thisMass , weight);
            // if(offTrack->isfromV0)
              // hmttOff_isFromV0->Fill(thisMass , weight);

          }

          //need to check that the track (with matching resolution cone r=0.01) is in region where R=0.3 circles inside the two jets overlap!
          //if offTrack.dR > 0.29 - offJet.match_dR: continue
          if(offTrack->dR                  > 0.29) continue; // offTrack is not in cone of offJet
          if(offTrack->p.DeltaR(hltJet->p) > 0.29) continue; // offTrack is not in cone of puppiJet



          hOffTracksPuppi->Fill(offTrack, weight);
          ++nOffTracks;
          if(!offTrack->isfromV0){
            hOffTracksPuppi_noV0->Fill(offTrack, weight);
            ++nOffTracks_noV0;
          }

          //
          //  Match offline track with PF Track
          //
          OfflineToOnlineTrackMatching(offJet, offTrack, hltJet, 0.01);

          const nTupleAnalysis::trackPtr offTrackMatchedTrack = offTrack->matchedTrack.lock();

          if(offTrackMatchedTrack){

    	if(!offTrack->isfromV0 && !offTrackMatchedTrack->isfromV0){
    	  hOffTracksPuppi_matched_noV0->Fill(offTrack, weight);
    	  ++nOffTracks_matched_noV0;
    	}

    	++nOffTracks_matched;
    	hOffTracksPuppi_matched    ->Fill(offTrack, weight);
    	hPuppiTracks_matched ->Fill(offTrackMatchedTrack,weight);

          }else{
            hOffTracksPuppi_unmatched->Fill(offTrack, weight);
          }

        }//off Tracks

        hOffTracksPuppi             ->nTracks->Fill(nOffTracks, weight);
        hOffTracksPuppi_matched     ->nTracks->Fill(nOffTracks_matched, weight);
        hOffTracksPuppi_noV0        ->nTracks->Fill(nOffTracks_noV0, weight);
        hOffTracksPuppi_matched_noV0->nTracks->Fill(nOffTracks_matched_noV0, weight);


        //
        //  Off BTags trkTagVars
        //
        unsigned int nTrkTags = 0;
        unsigned int nTrkTags_matched = 0;
        unsigned int nTrkTags_noV0 = 0;
        unsigned int nTrkTags_matched_noV0 = 0;

        for(const nTupleAnalysis::trkTagVarPtr& offTrkTag: offJet->trkTagVars){

          //need to check that the track (with matching resolution cone r=0.01) is in region where R=0.3 circles inside the two jets overlap!
          //if offTrack.dR > 0.29 - offJet.match_dR: continue
          if(offTrkTag->trackDeltaR                              > 0.29) continue; // offTrack is not in cone of offJet
          if(offTrkTag->p.DeltaR(hltJet->p) > 0.29) continue; // offTrack is not in cone of puppiJet

          hOffBTags->FillTrkTagVarHists(offTrkTag, weight);
          ++nTrkTags;

          if(!offTrkTag->matchIsFromV0){
            hOffBTags_noV0->FillTrkTagVarHists(offTrkTag, weight);
            ++nTrkTags_noV0;
          }

          OfflineToOnlineTrkTagMatching(offJet, offTrkTag, hltJet, 0.01);

          const nTupleAnalysis::trkTagVarPtr offTrackMatchedTrkTag = offTrkTag->matchedTrkTagVar.lock();
          if(offTrackMatchedTrkTag){

    	if(!offTrkTag->matchIsFromV0 && !offTrackMatchedTrkTag->matchIsFromV0){
    	  hOffBTags_matched_noV0->FillTrkTagVarHists(offTrkTag, weight);
    	  ++nTrkTags_matched_noV0;
    	}

    	++nTrkTags_matched;
    	//cout << "Filling offBTag_match" << endl;
    	hOffBTags_matched    ->FillTrkTagVarHists(offTrkTag, weight);
    	//const nTupleAnalysis::trackPtr offTrackMatchedTrkTag = offTrkTag->matchedTrkTagVar.lock();
    	//hPuppiTracks_matched ->FillTrkTagVarHists(offTrackMatchedTrkTag,weight);
          }else{
    	hOffBTags_unmatched    ->FillTrkTagVarHists(offTrkTag, weight);
          }

        }//OffTrkTag

        hOffBTags             ->trkTag_nTracks->Fill(nTrkTags, weight);
        hOffBTags_matched     ->trkTag_nTracks->Fill(nTrkTags_matched, weight);
        hOffBTags_noV0        ->trkTag_nTracks->Fill(nTrkTags_noV0, weight);
        hOffBTags_matched_noV0->trkTag_nTracks->Fill(nTrkTags_matched_noV0, weight);


        //
        //  Off BTags SVs
        //
        for(const nTupleAnalysis::svPtr& offSV: offJet->svs){

          OfflineToOnlineSVMatching(offSV, hltJet, 0.01);

        }//OffTrkTag


        //
        // PF Tracks
        //
        unsigned int nPuppiTracks = 0;
        unsigned int nPuppiTracks_matched = 0;
        unsigned int nPuppiTracks_noV0 = 0;
        for(const nTupleAnalysis::trackPtr& puppiTrack: hltJet->tracks){

          //
          //  track mass calculation
          //
          for(const nTupleAnalysis::trackPtr& puppiTrack_pair: hltJet->tracks){
            if(puppiTrack == puppiTrack_pair) continue;
            float thisMass = (puppiTrack->p + puppiTrack_pair->p).M();

            hmttPuppi->Fill(thisMass , weight);
            if(puppiTrack->isfromV0)
    	  hmttPuppi_isFromV0->Fill(thisMass , weight);

          }

          //need to check that the track (with matching resolution cone r=0.01) is in region where R=0.3 circles inside the two jets overlap!
          if(puppiTrack->dR                  > 0.29) continue; // puppiTrack is not in cone of puppiJet
          if(puppiTrack->p.DeltaR(offJet->p) > 0.29) continue; // puppiTrack is not in cone of offJet
          // if((1.48<abs(puppiTrack->eta) && abs(puppiTrack->eta)< 3.0 &&puppiTrack-> pt>1.0)) continue; // puppiTrack is not in cone of offJet

          hPuppiTracks->Fill(puppiTrack, weight); //all puppitracks in matched jets
          hPuppiTracks->FillMatchStats(puppiTrack, weight); //check how often we match puppiTracks to more than one offTrack
          ++nPuppiTracks;

          if(!puppiTrack->isfromV0){
            hPuppiTracks_noV0->Fill(puppiTrack, weight); //all puppitracks in matched jets
            ++nPuppiTracks_noV0;
          }

          if(!puppiTrack->nMatches){
            hPuppiTracks_unmatched->Fill(puppiTrack, weight); //all unmatched puppitracks
            hPuppiTracks_unmatched->FillMatchStats(puppiTrack, weight);
          }else{
            hPuppiTracks_matched->FillMatchStats(puppiTrack, weight);
            ++nPuppiTracks_matched;
          }
        }// puppiTracks

        hPuppiTracks              ->nTracks->Fill(nPuppiTracks, weight);
        hPuppiTracks_matched      ->nTracks->Fill(nPuppiTracks_matched, weight);
        hPuppiTracks_noV0         ->nTracks->Fill(nPuppiTracks_noV0, weight);
        //hOffTracks_matched_noV0->nTracks->Fill(nOffTracks_matched_noV0, weight);


        //
        //  Puppi BTags
        //
        unsigned int nPuppiTrkTags = 0;

        for(const nTupleAnalysis::trkTagVarPtr& puppiTrkTag: hltJet->trkTagVars){
          //need to check that the track (with matching resolution cone r=0.01) is in region where R=0.3 circles inside the two jets overlap!
          //if offTrack.dR > 0.29 - offJet.match_dR: continue
          if(puppiTrkTag->trackDeltaR                              > 0.29) continue; // offTrack is not in cone of offJet
          if(puppiTrkTag->p.DeltaR(offJet->p) > 0.29) continue; // offTrack is not in cone of puppiJet


          hPuppiBTags->FillTrkTagVarHists(puppiTrkTag, weight);
          const nTupleAnalysis::trkTagVarPtr puppiTrkTagMatch = puppiTrkTag->matchedTrkTagVar.lock();
          if(puppiTrkTagMatch){
    	hPuppiBTags_matched  ->FillTrkTagVarHists(puppiTrkTag, weight);
          }else{
    	hPuppiBTags_unmatched->FillTrkTagVarHists(puppiTrkTag, weight);
          }

          ++nPuppiTrkTags;

        }//OffTrkTag

        hPuppiBTags             ->trkTag_nTracks->Fill(nPuppiTrkTags, weight);

      }//doTracks

      //
      // Jet info
      //
      hOffJets_matchedPuppi->Fill(offJet,weight);
      hOffJets_matchedPuppiJet->Fill(hltJet,weight);
      if(abs(hltJet->p.Eta())<1.5){
          hOffJets_matchedPuppi_eta1->Fill(offJet,weight);
          hOffJets_matchedPuppiJet_eta1->Fill(hltJet,weight);
      }
      if(abs(hltJet->p.Eta())<2.4){
          hOffJets_matchedPuppi_eta2p4->Fill(offJet,weight);
          hOffJets_matchedPuppiJet_eta2p4->Fill(hltJet,weight);
      }
      if(abs(hltJet->p.Eta())>=1.5 && abs(hltJet->p.Eta())<3.){
          hOffJets_matchedPuppi_eta2->Fill(offJet,weight);
          hOffJets_matchedPuppiJet_eta2->Fill(hltJet,weight);
      }
      if(abs(hltJet->p.Eta())>=3){
          hOffJets_matchedPuppi_eta3->Fill(offJet,weight);
          hOffJets_matchedPuppiJet_eta3->Fill(hltJet,weight);
      }

      //
      // Offline Btaggs
      //
      if((offJet->DeepCSV > OfflineDeepCSVTightCut)){
        hOffJetTightDeepCSV_matchedPuppiJet->Fill(hltJet, weight);
      }
      if((offJet->DeepCSV > OfflineDeepCSVMediumCut)){
        hOffJetMediumDeepCSV_matchedPuppiJet->Fill(hltJet, weight);
        // hOffJetMedDeepCSV_matchedPuppiJet    ->Fill(offJet, weight);
        if(hltJet->CSVv2   > OnlineCSVCutPuppi)     hOffJetMedDeepCSV_matchedPuppiCSV    ->Fill(offJet, weight);
        // if(hltJet->DeepCSV > OnlineDeepCSVCutPuppi) hOffJetMedDeepCSV_matchedPuppiDeepCSV->Fill(offJet, weight);
        if(hltJet->DeepCSV > OnlineDeepCSVCutPuppiMedium) hOffJetMedDeepCSV_matchedPuppiDeepCSV->Fill(offJet, weight);
      }

      if((offJet->DeepCSV > OfflineDeepCSVLooseCut)){
        hOffJetLooseDeepCSV_matchedPuppiJet->Fill(hltJet, weight);
    }

        if((offJet->DeepJet > OfflineDeepFlavourTightCut)){
          hOffJetTightDeepFlav_matchedPuppiJet->Fill(hltJet, weight);
        }
      if(offJet->DeepJet > OfflineDeepFlavourMediumCut){
        hOffJetMediumDeepFlav_matchedPuppiJet->Fill(hltJet, weight);
        // hOffJetMedDeepFlav_matchedPuppiJet ->Fill(offJet, weight);
        // if(hltJet->CSVv2   > OnlineCSVCutPuppi)     hOffJetMedDeepFlav_matchedPuppiCSV    ->Fill(offJet, weight);
        // if(hltJet->DeepJet > OnlineDeepCSVCutPuppi) hOffJetMedDeepFlav_matchedPuppiDeepCSV->Fill(offJet, weight);
        if(hltJet->DeepJet > OnlineDeepCSVCutPuppiMedium) hOffJetMedDeepFlav_matchedPuppiDeepCSV->Fill(offJet, weight);
      }
      if((offJet->DeepJet > OfflineDeepFlavourLooseCut))
        hOffJetLooseDeepFlav_matchedPuppiJet->Fill(hltJet, weight);

      //
      // If pass CVS working point
      //
      // if(hltJet->CSVv2 >= OnlineCSVCutPuppi){
      //   hOffJet_matchedPuppicsvTag   ->Fill(offJet, weight);
      //   hOffJet_matchedPuppicsvTagJet->Fill(hltJet, weight);
      // }


      //
      // If pass DeepCVS working point
      //
      // if(hltJet->DeepCSV >= OnlineDeepCSVCutPuppi){
      //   hOffJet_matchedPuppiDeepcsvTag   ->Fill(offJet, weight);
      //   hOffJet_matchedPuppiDeepcsvTagJet->Fill(hltJet, weight);
      // }
      if(hltJet->DeepCSV >= OnlineDeepCSVCutPuppiLoose){
        hOffJet_matchedPuppiDeepcsvTagLoose   ->Fill(offJet, weight);
        hOffJet_matchedPuppiDeepcsvTagLooseJet->Fill(hltJet, weight);
      }
      if(hltJet->DeepCSV >= OnlineDeepCSVCutPuppiMedium){
        hOffJet_matchedPuppiDeepcsvTagMedium   ->Fill(offJet, weight);
        hOffJet_matchedPuppiDeepcsvTagMediumJet->Fill(hltJet, weight);
      }
      if(hltJet->DeepCSV >= OnlineDeepCSVCutPuppiTight){
        hOffJet_matchedPuppiDeepcsvTagTight   ->Fill(offJet, weight);
        hOffJet_matchedPuppiDeepcsvTagTightJet->Fill(hltJet, weight);
      }

      if(hltJet->DeepJet >= OnlineDeepFlavourCutPuppiLoose){
        hOffJet_matchedPuppiDeepflavTagLoose   ->Fill(offJet, weight);
        hOffJet_matchedPuppiDeepflavTagLooseJet->Fill(hltJet, weight);
      }
      if(hltJet->DeepJet >= OnlineDeepFlavourCutPuppiMedium){
        hOffJet_matchedPuppiDeepflavTagMedium   ->Fill(offJet, weight);
        hOffJet_matchedPuppiDeepflavTagMediumJet->Fill(hltJet, weight);
      }
      if(hltJet->DeepJet >= OnlineDeepFlavourCutPuppiTight){
        hOffJet_matchedPuppiDeepflavTagTight   ->Fill(offJet, weight);
        hOffJet_matchedPuppiDeepflavTagTightJet->Fill(hltJet, weight);
      }


      if(isMC){
        if(offJet->hadronFlavour == 5){
          hOffJets_matchedPuppi_B->Fill(offJet, weight);
          hOffJets_matchedPuppiJet_B->Fill(hltJet, weight);
          if(abs(hltJet->p.Eta())<1.5){
              hOffJets_matchedPuppi_B_eta1->Fill(offJet, weight);
              hOffJets_matchedPuppiJet_B_eta1->Fill(hltJet,weight);
          }
          if(abs(hltJet->p.Eta())<2.4){
              hOffJets_matchedPuppi_B_eta2p4->Fill(offJet, weight);
              hOffJets_matchedPuppiJet_B_eta2p4->Fill(hltJet,weight);
          }
          if(abs(hltJet->p.Eta())>=1.5 && abs(hltJet->p.Eta())<3.){
              hOffJets_matchedPuppi_B_eta2->Fill(offJet, weight);
              hOffJets_matchedPuppiJet_B_eta2->Fill(hltJet,weight);
          }
          if(abs(hltJet->p.Eta())>=3){
              hOffJets_matchedPuppi_B_eta3->Fill(offJet, weight);
              hOffJets_matchedPuppiJet_B_eta3->Fill(hltJet,weight);
          }
        }else if(offJet->hadronFlavour == 4){
          hOffJets_matchedPuppi_C->Fill(offJet, weight);
          hOffJets_matchedPuppiJet_C->Fill(hltJet, weight);
          if(abs(hltJet->p.Eta())<1.5){
              hOffJets_matchedPuppi_C_eta1->Fill(offJet, weight);
              hOffJets_matchedPuppiJet_C_eta1->Fill(hltJet,weight);
          }
          if(abs(hltJet->p.Eta())<2.4){
              hOffJets_matchedPuppi_C_eta2p4->Fill(offJet, weight);
              hOffJets_matchedPuppiJet_C_eta2p4->Fill(hltJet,weight);
          }
          if(abs(hltJet->p.Eta())>=1.5 && abs(hltJet->p.Eta())<3.){
              hOffJets_matchedPuppi_C_eta2->Fill(offJet, weight);
              hOffJets_matchedPuppiJet_C_eta2->Fill(hltJet,weight);
          }
          if(abs(hltJet->p.Eta())>=3){
              hOffJets_matchedPuppi_C_eta3->Fill(offJet, weight);
              hOffJets_matchedPuppiJet_C_eta3->Fill(hltJet,weight);
          }
        }else if(offJet->hadronFlavour == 0){
          hOffJets_matchedPuppi_LandPU->Fill(offJet, weight);
          hOffJets_matchedPuppiJet_LandPU->Fill(hltJet, weight);
          if(abs(hltJet->p.Eta())<1.5){
              hOffJets_matchedPuppi_LandPU_eta1->Fill(offJet, weight);
              hOffJets_matchedPuppiJet_LandPU_eta1->Fill(hltJet,weight);
          }
          if(abs(hltJet->p.Eta())<2.4){
              hOffJets_matchedPuppi_LandPU_eta2p4->Fill(offJet, weight);
              hOffJets_matchedPuppiJet_LandPU_eta2p4->Fill(hltJet,weight);
          }
          if(abs(hltJet->p.Eta())>=1.5 && abs(hltJet->p.Eta())<3.){
              hOffJets_matchedPuppi_LandPU_eta2->Fill(offJet, weight);
              hOffJets_matchedPuppiJet_LandPU_eta2->Fill(hltJet,weight);
          }
          if(abs(hltJet->p.Eta())>=3){
              hOffJets_matchedPuppi_LandPU_eta3->Fill(offJet, weight);
              hOffJets_matchedPuppiJet_LandPU_eta3->Fill(hltJet,weight);
          }
          if(offJet->flavourCleaned!=0){
            hOffJets_matchedPuppi_L->Fill(offJet, weight);
            hOffJets_matchedPuppiJet_L->Fill(hltJet, weight);
            if(abs(hltJet->p.Eta())<1.5){
                hOffJets_matchedPuppi_L_eta1->Fill(offJet, weight);
                hOffJets_matchedPuppiJet_L_eta1->Fill(hltJet,weight);
            }
            if(abs(hltJet->p.Eta())<2.4){
                hOffJets_matchedPuppi_L_eta2p4->Fill(offJet, weight);
                hOffJets_matchedPuppiJet_L_eta2p4->Fill(hltJet,weight);
            }
            if(abs(hltJet->p.Eta())>=1.5 && abs(hltJet->p.Eta())<3.){
                hOffJets_matchedPuppi_L_eta2->Fill(offJet, weight);
                hOffJets_matchedPuppiJet_L_eta2->Fill(hltJet,weight);
            }
            if(abs(hltJet->p.Eta())>=3){
                hOffJets_matchedPuppi_L_eta3->Fill(offJet, weight);
                hOffJets_matchedPuppiJet_L_eta3->Fill(hltJet,weight);
            }
          }
        }
      }


      return;
}
