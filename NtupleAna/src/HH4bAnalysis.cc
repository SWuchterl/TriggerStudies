#include <iostream>
#include <iomanip>
#include <cstdio>
#include <TROOT.h>
#include <boost/bind.hpp>


#include "TriggerStudies/NtupleAna/interface/HH4bAnalysis.h"
#include "nTupleAnalysis/baseClasses/interface/helpers.h"

using std::cout; using std::endl;
using namespace TriggerStudies;
using std::vector;  using std::map; using std::string; using std::set; using std::pair;


HH4bAnalysis::HH4bAnalysis(TChain* _eventsRAW, fwlite::TFileService& fs, bool _debug){


  if(_debug) cout<<"In HH4bAnalysis constructor"<<endl;
  debug      = _debug;

  eventsRAW     = _eventsRAW;
  // eventsRAW->SetBranchStatus("*", 0);
  eventsRAW->SetBranchStatus("PFJet.*", 0);

  event      = new eventData(eventsRAW, NULL, true, "2018", debug, "PuppiJets");
  treeEvents = eventsRAW->GetEntries();

  cutflow    = new nTupleAnalysis::cutflowHists("cutflow", fs);

  crossSections14TeV["QCD15to20"] = 923300000.0;
  crossSections14TeV["QCD20to30"] = 436000000.0;
  crossSections14TeV["QCD30to50"] = 118400000.0;
  crossSections14TeV["QCD50to80"] = 17650000.0;
  crossSections14TeV["QCD80to120"] = 2671000.0;
  crossSections14TeV["QCD120to170"] = 469700.0;
  crossSections14TeV["QCD170to300"] = 121700.0;
  crossSections14TeV["QCD300to470"] = 8251.0;
  crossSections14TeV["QCD470to600"] = 686.4;
  crossSections14TeV["QCD600toInf"] = 244.8;
  crossSections14TeV["WJetsToLNu"] = 56990.0;
  crossSections14TeV["DYToLL-M10to50"] = 16880.0;
  crossSections14TeV["DYToLL-M50"] = 5795.0;
  crossSections14TeV["MinBias"] = 1.0;
  crossSections14TeV["TTbar"] = 1.0;
  crossSections14TeV["HH4b"] = 1.0;
  // crossSections14TeV["QCD15to20"] = 923300000.0/1.;
  // crossSections14TeV["QCD20to30"] = 436000000.0/996386.;
  // crossSections14TeV["QCD30to50"] = 118400000.0/(483498. + 499401.);
  // crossSections14TeV["QCD50to80"] = 17650000.0/(300000. + 299401.);
  // crossSections14TeV["QCD80to120"] = 2671000.0/100000.;
  // crossSections14TeV["QCD120to170"] = 469700.0/49601.;
  // crossSections14TeV["QCD170to300"] = 121700.0/50000.;
  // crossSections14TeV["QCD300to470"] = 8251.0/50000.;
  // crossSections14TeV["QCD470to600"] = 686.4/50000.;
  // crossSections14TeV["QCD600toInf"] = 244.8/50000.;
  // crossSections14TeV["WJetsToLNu"] = 56990.0/85778.;
  // crossSections14TeV["DYToLL-M10to50"] = 16880.0/96923.;
  // crossSections14TeV["DYToLL-M50"] = 5795.0/299300.;
  // crossSections14TeV["MinBias"] = 1.0;
  // crossSections14TeV["TTbar"] = 1.0;

  // basic
  triggers_HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4_v1 = new nTupleAnalysis::triggers("HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4_v1", fs);
  triggers_HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_2p4_v1 = new nTupleAnalysis::triggers("HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_2p4_v1", fs);
  triggers_HLT_QuadPFPuppiJet_75_60_45_40_2p4_v1 = new nTupleAnalysis::triggers("HLT_QuadPFPuppiJet_75_60_45_40_2p4_v1", fs);
  triggers_HLTNoL1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_2p4_v1 = new nTupleAnalysis::triggers("HLTNoL1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_2p4_v1", fs);
  triggers_HLTNoL1_QuadPFPuppiJet_75_60_45_40_2p4_v1 = new nTupleAnalysis::triggers("HLTNoL1_QuadPFPuppiJet_75_60_45_40_2p4_v1", fs);
  triggers_L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV_2p4_v1 = new nTupleAnalysis::triggers("L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV_2p4_v1", fs);
  triggers_L1_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV_2p4_v1 = new nTupleAnalysis::triggers("L1_PFHT330PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV_2p4_v1", fs);

  triggers_HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4_v1 = new nTupleAnalysis::triggers("HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4_v1", fs);
  triggers_HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppi_2p4_v1 = new nTupleAnalysis::triggers("HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppi_2p4_v1", fs);
  triggers_HLTNoL1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppi_2p4_v1 = new nTupleAnalysis::triggers("HLTNoL1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppi_2p4_v1", fs);
  triggers_L1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p71_2p4_v1 = new nTupleAnalysis::triggers("L1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p71_2p4_v1", fs);
  triggers_L1_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p71_2p4_v1 = new nTupleAnalysis::triggers("L1_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p71_2p4_v1", fs);

  triggers_GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4 = new nTupleAnalysis::triggers("GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4", fs);
  triggers_GEN_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4 = new nTupleAnalysis::triggers("GEN_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4", fs);

  triggers_L1PlusRECO_PFHT330PT30 = new nTupleAnalysis::triggers("L1PlusRECO_PFHT330PT30", fs);
  triggers_L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40 = new nTupleAnalysis::triggers("L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", fs);
  triggers_L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4 = new nTupleAnalysis::triggers("L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4", fs);
  triggers_L1_PFHT330PT30 = new nTupleAnalysis::triggers("L1_PFHT330PT30", fs);
  triggers_L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40 = new nTupleAnalysis::triggers("L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", fs);
  triggers_L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4 = new nTupleAnalysis::triggers("L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4", fs);
  triggers_RECO_PFHT330PT30 = new nTupleAnalysis::triggers("RECO_PFHT330PT30", fs);
  triggers_RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40 = new nTupleAnalysis::triggers("RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", fs);
  triggers_RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4 = new nTupleAnalysis::triggers("RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4", fs);

  triggers_L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6 = new nTupleAnalysis::triggers("L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6", fs);
  triggers_L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4 = new nTupleAnalysis::triggers("L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4", fs);
  triggers_L1_DoublePFPuppiJets128MaxDeta1p6 = new nTupleAnalysis::triggers("L1_DoublePFPuppiJets128MaxDeta1p6", fs);
  triggers_L1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4 = new nTupleAnalysis::triggers("L1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4", fs);
  triggers_RECO_DoublePFPuppiJets128MaxDeta1p6 = new nTupleAnalysis::triggers("RECO_DoublePFPuppiJets128MaxDeta1p6", fs);
  triggers_RECO_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4 = new nTupleAnalysis::triggers("RECO_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4", fs);

  // loose edition
  triggers_GEN_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4 = new nTupleAnalysis::triggers("GEN_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4", fs);
  triggers_GEN_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4 = new nTupleAnalysis::triggers("GEN_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4", fs);

  triggers_L1PlusRECO_PFHT100PT30 = new nTupleAnalysis::triggers("L1PlusRECO_PFHT100PT30", fs);
  triggers_L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30 = new nTupleAnalysis::triggers("L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30", fs);
  triggers_L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4 = new nTupleAnalysis::triggers("L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4", fs);
  triggers_L1_PFHT100PT30 = new nTupleAnalysis::triggers("L1_PFHT100PT30", fs);
  triggers_L1_PFHT100PT30_QuadPFPuppiJet_30_30_30_30 = new nTupleAnalysis::triggers("L1_PFHT100PT30_QuadPFPuppiJet_30_30_30_30", fs);
  triggers_L1_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4 = new nTupleAnalysis::triggers("L1_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4", fs);
  triggers_RECO_PFHT100PT30 = new nTupleAnalysis::triggers("RECO_PFHT100PT30", fs);
  triggers_RECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30 = new nTupleAnalysis::triggers("RECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30", fs);
  triggers_RECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4 = new nTupleAnalysis::triggers("RECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4", fs);

  triggers_L1PlusRECO_DoublePFPuppiJets30MaxDeta1p6 = new nTupleAnalysis::triggers("L1PlusRECO_DoublePFPuppiJets30MaxDeta1p6", fs);
  triggers_L1PlusRECO_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4 = new nTupleAnalysis::triggers("L1PlusRECO_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4", fs);
  triggers_L1_DoublePFPuppiJets30MaxDeta1p6 = new nTupleAnalysis::triggers("L1_DoublePFPuppiJets30MaxDeta1p6", fs);
  triggers_L1_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4 = new nTupleAnalysis::triggers("L1_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4", fs);
  triggers_RECO_DoublePFPuppiJets30MaxDeta1p6 = new nTupleAnalysis::triggers("RECO_DoublePFPuppiJets30MaxDeta1p6", fs);
  triggers_RECO_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4 = new nTupleAnalysis::triggers("RECO_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4", fs);

  triggers_noFilter_PFDeepCSVPuppi = new nTupleAnalysis::triggers("noFilter_PFDeepCSVPuppi", fs);

  rates_L1_SingleJet50Hz = new nTupleAnalysis::triggers("rate_L1_SingleJet50Hz", fs);
  rates_L1PlusReco_SingleJet50Hz = new nTupleAnalysis::triggers("rate_L1PlusRECO_SingleJet50Hz", fs);
  rates_Reco_SingleJet50Hz = new nTupleAnalysis::triggers("rate_RECO_SingleJet50Hz", fs);
  rates_L1_SingleJet75Hz = new nTupleAnalysis::triggers("rate_L1_SingleJet75Hz", fs);
  rates_L1PlusReco_SingleJet75Hz = new nTupleAnalysis::triggers("rate_L1PlusRECO_SingleJet75Hz", fs);
  rates_Reco_SingleJet75Hz = new nTupleAnalysis::triggers("rate_RECO_SingleJet75Hz", fs);
  GEN_Dijet = new nTupleAnalysis::triggers("GEN_Dijet", fs);

  rates_L1_DoublePFPuppiJets128MaxDeta1p6 = new nTupleAnalysis::triggers("rate_L1_DoublePFPuppiJets128MaxDeta1p6", fs);
  rates_L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6 = new nTupleAnalysis::triggers("rate_L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6", fs);
  rates_L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6_DeepCSVCuts = new nTupleAnalysis::triggers("rate_L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6_DeepCSVCuts", fs);
  rates_L1PlusRECO_DoublePFPuppiJets30MaxDeta1p6 = new nTupleAnalysis::triggers("rate_L1PlusRECO_DoublePFPuppiJets30MaxDeta1p6", fs);
  rates_L1PlusRECO_DoublePFPuppiJets30MaxDeta1p6_DeepCSVCuts = new nTupleAnalysis::triggers("rate_L1PlusRECO_DoublePFPuppiJets30MaxDeta1p6_DeepCSVCuts", fs);
  rates_L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40 = new nTupleAnalysis::triggers("rate_L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", fs);
  rates_L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40 = new nTupleAnalysis::triggers("rate_L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", fs);
  rates_L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30 = new nTupleAnalysis::triggers("rate_L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30", fs);
  rates_L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_DeepCSVCuts = new nTupleAnalysis::triggers("rate_L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_DeepCSVCuts", fs);
  rates_L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_DeepCSVCuts = new nTupleAnalysis::triggers("rate_L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_DeepCSVCuts", fs);

  // triggerMap[]
    triggerMap["MC_JME"] = std::make_pair(0,0);
    triggerMap["QCDMuon"] = std::make_pair(0, 1);
    triggerMap["noFilter_PFDeepCSVPuppi"] = std::make_pair(0,2);
    triggerMap["noFilter_PFDeepFlavourPuppi"] = std::make_pair(0,3);
    triggerMap["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV_2p4_v1"] = std::make_pair(0, 4);
    triggerMap["L1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p71_2p4_v1"] = std::make_pair(0, 5);
    triggerMap["HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_4p5_v1"] = std::make_pair(0, 6);
    triggerMap["HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_4p5_v1"] = std::make_pair(0, 7);
    triggerMap["HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepFlavour0p5_4p5_v1"] = std::make_pair(0, 8);
    triggerMap["HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepFlavour_p5_4p5_v1"] = std::make_pair(0, 9);
    triggerMap["HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p7_4p5_v1"] = std::make_pair(0, 10);
    triggerMap["HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p7_4p5_v1"] = std::make_pair(0, 11);
    triggerMap["HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepFlavour0p7_4p5_v1"] = std::make_pair(0, 12);
    triggerMap["HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepFlavour_p7_4p5_v1"] = std::make_pair(0, 13);
    triggerMap["HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p85_4p5_v1"] = std::make_pair(0, 14);
    triggerMap["HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p85_4p5_v1"] = std::make_pair(0, 15);
    triggerMap["HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepFlavour0p85_4p5_v1"] = std::make_pair(0, 16);
    triggerMap["HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepFlavour_p85_4p5_v1"] = std::make_pair(0, 17);

    triggerMap["HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4_v1"] = std::make_pair(0, 18);
    triggerMap["HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4_v1"] = std::make_pair(0, 19);
    triggerMap["HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepFlavour0p5_2p4_v1"] = std::make_pair(0, 20);
    triggerMap["HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepFlavour_p5_2p4_v1"] = std::make_pair(0, 21);
    triggerMap["HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p7_2p4_v1"] = std::make_pair(0, 22);
    triggerMap["HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p7_2p4_v1"] = std::make_pair(0, 23);
    triggerMap["HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepFlavour0p7_2p4_v1"] = std::make_pair(0, 24);
    triggerMap["HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepFlavour_p7_2p4_v1"] = std::make_pair(0, 25);
    triggerMap["HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p85_2p4_v1"] = std::make_pair(0, 26);
    triggerMap["HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p85_2p4_v1"] = std::make_pair(0, 27);
    triggerMap["HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepFlavour0p85_2p4_v1"] = std::make_pair(0, 28);
    triggerMap["HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepFlavour_p85_2p4_v1"] = std::make_pair(0, 29);

    triggerMap["Offline_BadPFMuon" ]= std::make_pair(0, 30);
    triggerMap["Offline_BadPFMuonDz"] = std::make_pair(0, 31);
    triggerMap["Offline_BadChargedCandidate"] = std::make_pair(1, 0);
    triggerMap["L1T_SinglePFPuppiJet200off"] = std::make_pair(1, 1);
    triggerMap["HLT_AK4PFJet550"] = std::make_pair(1, 2);
    triggerMap["HLT_AK4PFCHSJet550"] = std::make_pair(1, 3);
    triggerMap["HLT_AK4PFPuppiJet550"] = std::make_pair(1, 4);
    triggerMap["L1T_PFPuppiHT450off"] = std::make_pair(1, 5);
    triggerMap["HLT_PFPuppiHT1050"] = std::make_pair(1, 6);
    triggerMap["L1T_PFPuppiMET200off"] = std::make_pair(1, 7);
    triggerMap["L1T_PFPuppiMET245off"] = std::make_pair(1, 8);
    triggerMap["HLT_PFMET250"] = std::make_pair(1, 9);
    triggerMap["HLT_PFCHSMET250"] = std::make_pair(1, 10);
    triggerMap["HLT_PFPuppiMET250"] = std::make_pair(1, 11);
    triggerMap["HLT_PFPuppiMET120"] = std::make_pair(1, 12);
    triggerMap["HLT_PFPuppiMET120_PFPuppiMHT120"] = std::make_pair(1, 13);
    triggerMap["HLT_PFPuppiMET120_PFPuppiMHT120_PFPuppiHT60"] = std::make_pair(1, 14);

    triggerMap["HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppi_2p4_v1"] = std::make_pair(1, 15);
    triggerMap["HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppi_4p5_v1"] = std::make_pair(1, 16);
    triggerMap["HLTNoL1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppi_2p4_v1"] = std::make_pair(1, 17);
    triggerMap["HLTNoL1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppi_4p5_v1"] = std::make_pair(1, 18);

    triggerMap["HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_2p4_v1"] = std::make_pair(1, 19);
    triggerMap["HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_4p5_v1"] = std::make_pair(1, 20);
    triggerMap["HLT_QuadPFPuppiJet_75_60_45_40_2p4_v1"] = std::make_pair(1, 21);
    triggerMap["HLT_QuadPFPuppiJet_75_60_45_40_4p5_v1"] = std::make_pair(1, 22);
    triggerMap["HLTNoL1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_2p4_v1"] = std::make_pair(1, 23);
    triggerMap["HLTNoL1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_4p5_v1"] = std::make_pair(1, 24);
    triggerMap["HLTNoL1_QuadPFPuppiJet_75_60_45_40_2p4_v1"] = std::make_pair(1, 25);
    triggerMap["HLTNoL1_QuadPFPuppiJet_75_60_45_40_4p5_v1"] = std::make_pair(1, 26);


  cutflow->AddCut("all");

  for(map<string, pair<int,int> >::iterator it = triggerMap.begin(); it != triggerMap.end(); it++ ){
      cutflow->AddCut(it->first);
  }

  dir = fs.mkdir("HH4bAnalysis");

  hEvents = new nTupleAnalysis::eventHists("Events", fs);

}


void HH4bAnalysis::monitor(long int e){
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
  fprintf(stdout, "\rProcessed: %8li of %li ( %2li%% | %.0f events/s AVE | %.0f events/s  | done in %02i:%02i | memory usage: %li MB)       ",
	  e+1, nEvents, percent,   eventRateAVE,    eventRate, minutes, seconds,                usageMB);
  fflush(stdout);
  lastTime = std::clock();
  lastEvent = e+1;
}

int HH4bAnalysis::eventLoop(int maxEvents, int nSkipEvents, double etaCut){
  //Set Number of events to process. Take manual maxEvents if maxEvents is > 0 and less than the total number of events in the input files.
  nEvents = (maxEvents > 0 && maxEvents < treeEvents) ? maxEvents : treeEvents;

  std::cout<<"Using eta cut of: "<<etaCut<<std::endl;

  jetEtaCut = etaCut;

  cout << "\nProcess " << nEvents << " of " << treeEvents << " events.\n";

  start = std::clock();
  lastTime = std::clock();
  lastEvent = 0;
  for(long int e = 0; e < nEvents; e++){
    if(e < nSkipEvents) continue;
    event->update(e);
    processEvent();
    if(debug) event->dump();
    if( (e+1)%10000 == 0 || e+1==nEvents || debug)
      monitor(e);
  }
  cout << endl;
  cout << "HH4bAnalysis::End of Event Loop" << endl;

  minutes = static_cast<int>(duration/60);
  seconds = static_cast<int>(duration - minutes*60);

  fprintf(stdout,"---------------------------\nProcessed in %02i:%02i", minutes, seconds);

  return 0;
}






int HH4bAnalysis::processEvent(){
  // float eta_cut     = 2.4;
  // float eta_cut     = 4.;
  float eta_cut     = jetEtaCut;
  float pt_cut      = 30. ;
  const char *current_file_name = eventsRAW->GetCurrentFile()->GetName();
  TString currentFilename(current_file_name);

  float factor=1.;

  if(currentFilename.Contains("PU200")){
      factor=0.075;
  }
  else if(currentFilename.Contains("PU140")){
      factor=0.053;
  }

  if(currentFilename.Contains("DYJetsToLL_M010to050")){
      xSec =crossSections14TeV["DYToLL-M10to50"]*factor/ (double)nEvents;
  }else if(currentFilename.Contains("DYJetsToLL_M050toInf")){
      xSec =crossSections14TeV["DYToLL-M50"]*factor/ (double)nEvents;
  }else if(currentFilename.Contains("WJetsToLNu")){
      xSec =crossSections14TeV["WJetsToLNu"]*factor/ (double)nEvents;
  }else if(currentFilename.Contains("QCD_Pt600toInf")){
      xSec =crossSections14TeV["QCD600toInf"]*factor/ (double)nEvents;
  }else if(currentFilename.Contains("QCD_Pt470to600")){
      xSec =crossSections14TeV["QCD470to600"]*factor/ (double)nEvents;
  }else if(currentFilename.Contains("QCD_Pt300to470")){
      xSec =crossSections14TeV["QCD300to470"]*factor/ (double)nEvents;
  }else if(currentFilename.Contains("QCD_Pt170to300")){
      xSec =crossSections14TeV["QCD170to300"]*factor/ (double)nEvents;
  }else if(currentFilename.Contains("QCD_Pt120to170")){
      xSec =crossSections14TeV["QCD120to170"]*factor/ (double)nEvents;
  }else if(currentFilename.Contains("QCD_Pt080to120")){
      xSec =crossSections14TeV["QCD80to120"]*factor/ (double)nEvents;
  }else if(currentFilename.Contains("QCD_Pt050to080")){
      xSec =crossSections14TeV["QCD50to80"]*factor/ (double)nEvents;
  }else if(currentFilename.Contains("QCD_Pt030to050")){
      xSec =crossSections14TeV["QCD30to50"]*factor/ (double)nEvents;
  }else if(currentFilename.Contains("QCD_Pt020to030")){
      xSec =crossSections14TeV["QCD20to30"]*factor/ (double)nEvents;
  }else if(currentFilename.Contains("QCD_Pt015to020")){
      xSec =crossSections14TeV["QCD15to20"]*factor/ (double)nEvents;
  }else if(currentFilename.Contains("TTbar")){
      xSec =crossSections14TeV["TTbar"];
  }else if(currentFilename.Contains("HH4b")){
      xSec =crossSections14TeV["HH4b"];
  }else if(currentFilename.Contains("MinBias")){
      // xSec =crossSections14TeV["MinBias"]/ 30000000.;
      // xSec =crossSections14TeV["MinBias"]* 30900000./4928613.;
      // xSec =crossSections14TeV["MinBias"]* 30900000./ (double)nEvents;
      xSec =crossSections14TeV["MinBias"]* 30000000./ (double)nEvents;
  }else{
      std::cout<<"ERROR: NO XSEC FOUND!!!"<<std::endl;
      std::cout<<currentFilename<<std::endl;
      xSec=0.;
  }

  // double weight = xSec * intLumi / (double)nEvents;
  // double weight = xSec / (double)nEvents;
  double weight = xSec;
  // std::cout<<weight<<std::endl;

  cutflow->Fill("all", weight);

  float eventWeight = 1.0;
  eventWeight = 1.0 * weight;

  float qcd140 = event->qcdWeightPU140;
  float qcd200 = event->qcdWeightPU200;

  if(currentFilename.Contains("QCD_Pt") || currentFilename.Contains("MinBias")){
      if (currentFilename.Contains("PU140")){
          eventWeight = eventWeight * qcd140;
      }
      if (currentFilename.Contains("PU200")){
          eventWeight = eventWeight * qcd200;
      }
  }


  int nBits = 2;
  int nTrigsperBit[nBits] = {32,26};
  std::vector<bool> convertedBits;
  convertedBits.reserve(nBits*32);
  for (int i=0;i<nBits;++i){
      for(int j=0; j<nTrigsperBit[i];++j){
          convertedBits.push_back(passTrigBit(i,j));
      }
  }

  for(map<string, pair<int,int> >::iterator it = triggerMap.begin(); it != triggerMap.end(); it++ ){
      if(getConvertedBit(it->first, convertedBits)){
          cutflow->Fill(it->first, weight);
      }
  }

  // match genJets to reco jets myself
  TLorentzVector temp(0.,0.,0.,0.);
  for(const nTupleAnalysis::jetPtr& jet : event->puppiJets){
      float minDR(1000.);
      int index = -99;
      for(int ijet=0; ijet<event->nGenJets; ++ijet){
          temp.SetPtEtaPhiM(event->genJet_pt[ijet], event->genJet_eta[ijet], event->genJet_phi[ijet], event->genJet_mass[ijet]);
          if(event->genJet_pt[ijet]>15. && temp.DeltaR(jet->p)<minDR){
              minDR = temp.DeltaR(jet->p);
              index = ijet;
          }
      }
      if(minDR<0.1){
          jet->hasDRMatchedGenJet = 1;
          jet->DRGenJet_pt = event->genJet_pt[index];
          jet->DRGenJet_eta = event->genJet_eta[index];
          jet->DRGenJet_phi = event->genJet_phi[index];
          jet->DRGenJet_mass = event->genJet_mass[index];
      }
  }



  float genHT_temp = 0.;
  for(int ijet=0; ijet<event->nGenJets; ++ijet){
      if(event->genJet_pt[ijet]>pt_cut && fabs(event->genJet_eta[ijet])<eta_cut){
          genHT_temp = genHT_temp + event->genJet_pt[ijet];
      }
  }
  event->genHT = genHT_temp;


  bool emulaterBit_GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4 = false;
  bool emulaterBit_GEN_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4 = false;
  bool emulaterBit_GEN_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4 = false;
  bool emulaterBit_GEN_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4 = false;

  bool L1DEtaCut = false;
  bool L1HT4JetCut = false;

  bool L1SingleJetSeed = false;
  bool HLTSingleJet50Hz = false;
  bool HLTSingleJet75Hz = false;

  bool hasRecoHTGreater330 = false;
  bool hasRecoHTGreater100 = false;

  bool has4jetsWithPTCuts = false;
  bool has4jetsWithPTCutsLoose = false;
  bool has2jetsWithPTCuts = false;
  bool has2jetsWithPTCutsLoose = false;

  bool has2jetswithMaxDEta = false;
  bool has2jetswithMaxDEtaLoose = false;

  bool has3jetsWithBTagsNew = false;
  bool has2jetsWithBTagsNew = false;
  bool has3jetsWithBTagsOld = false;
  bool has2jetsWithBTagsOld = false;

  vector<nTupleAnalysis::jetPtr> selectedjets;
  vector<nTupleAnalysis::jetPtr> selectedjets128;
  vector<nTupleAnalysis::jetPtr> selectedjets30;
  vector<nTupleAnalysis::jetPtr> selectedjets_forBTags;
  vector<nTupleAnalysis::jetPtr> selectedjets_gen;
  vector<nTupleAnalysis::jetPtr> selectedjets_genMatched;
  vector<nTupleAnalysis::jetPtr> selectedjets_forSingleJet;
  int countTrueBJets = 0;

  float recoHTTemp = 0;

  for(const nTupleAnalysis::jetPtr& jet : event->puppiJets){
      // perform HLT like selection of jets

     if(fabs(jet->eta)<2.4 && jet->pt>30.){
         selectedjets_forSingleJet.push_back(jet);
     }

    if(fabs(jet->eta) > eta_cut) continue;
    if(jet->pt       < pt_cut)       continue; // 40 ?
    recoHTTemp = recoHTTemp + jet->pt;

    selectedjets_forBTags.push_back(jet);
    selectedjets.push_back(jet);

    if(!jet->hasMatch) continue;
    if(fabs(jet->GenJet_eta) > eta_cut) continue;
    // if(jet->GenJet_pt       < pt_cut)       continue; //
    // if(jet->GenJet_pt       < 15)       continue; //
    if(jet->GenJet_pt       < 20)       continue; //
    if(jet->p.DeltaR(jet->p_gen)>0.1) continue;
    if(jet->isUndefined) continue;
    selectedjets_genMatched.push_back(jet);
    if(jet->GenJet_pt       < pt_cut)       continue; //
    if(jet->isB || jet->isLeptonicB || jet->isLeptonicB_C || jet->isBB || jet->isGBB){
        ++countTrueBJets;
    }
  }

  vector<int> L1JetIndices;
  vector<int> L1JetIndicesEta2p4Pt30;
  vector<int> L1JetIndicesEta2p4Pt112;
  float tempL1Ht=0.;
  for(int i=0; i<event->nL1_PuppiJets; ++i){
      if(triggers_noFilter_PFDeepCSVPuppi->rescaleL1TpT(event->L1_PuppiJets_pt[i], event->L1_PuppiJets_eta[i])>30. && fabs(event->L1_PuppiJets_eta[i])<5.){
          L1JetIndices.push_back(i);
          tempL1Ht=tempL1Ht+event->L1_PuppiJets_pt[i];
      }
      if(triggers_noFilter_PFDeepCSVPuppi->rescaleL1TpT(event->L1_PuppiJets_pt[i], event->L1_PuppiJets_eta[i])>30. && fabs(event->L1_PuppiJets_eta[i])<2.4){
          L1JetIndicesEta2p4Pt30.push_back(i);
          // tempL1Ht=tempL1Ht+event->L1_PuppiJets_pt[i];
      }
      if(triggers_noFilter_PFDeepCSVPuppi->rescaleL1TpT(event->L1_PuppiJets_pt[i], event->L1_PuppiJets_eta[i])>112. && fabs(event->L1_PuppiJets_eta[i])<2.4){
          L1JetIndicesEta2p4Pt112.push_back(i);
      }
  }
  if(L1JetIndicesEta2p4Pt112.size()>1){
      int n(0);
      for(uint i1=0; i1<L1JetIndicesEta2p4Pt112.size();++i1){
          for(uint i2=i1+1; i2<L1JetIndicesEta2p4Pt112.size();++i2){
              if(fabs(event->L1_PuppiJets_eta[i1]-event->L1_PuppiJets_eta[i2])<1.6) ++n;
          }
      }
      if(n>0) L1DEtaCut=true;
  }
  // tempL1Ht = triggers_noFilter_PFDeepCSVPuppi->rescaleL1THT(tempL1Ht);
  if(L1JetIndicesEta2p4Pt30.size()>3){
      if(triggers_noFilter_PFDeepCSVPuppi->rescaleL1TpT(event->L1_PuppiJets_pt[L1JetIndicesEta2p4Pt30.at(0)],event->L1_PuppiJets_eta[L1JetIndicesEta2p4Pt30.at(0)])>70.){
          if(triggers_noFilter_PFDeepCSVPuppi->rescaleL1TpT(event->L1_PuppiJets_pt[L1JetIndicesEta2p4Pt30.at(1)],event->L1_PuppiJets_eta[L1JetIndicesEta2p4Pt30.at(1)])>55.){
              if(triggers_noFilter_PFDeepCSVPuppi->rescaleL1TpT(event->L1_PuppiJets_pt[L1JetIndicesEta2p4Pt30.at(2)],event->L1_PuppiJets_eta[L1JetIndicesEta2p4Pt30.at(2)])>40.){
                  if(triggers_noFilter_PFDeepCSVPuppi->rescaleL1TpT(event->L1_PuppiJets_pt[L1JetIndicesEta2p4Pt30.at(3)],event->L1_PuppiJets_eta[L1JetIndicesEta2p4Pt30.at(3)])>40.){
                      // if(tempL1Ht>400.) L1HT4JetCut = true;
                      // if(triggers_noFilter_PFDeepCSVPuppi->rescaleL1THT(tempL1Ht)>400.){L1HT4JetCut = true;}
                      if(triggers_noFilter_PFDeepCSVPuppi->rescaleL1THT(event->L1_HT_sumEt)>400.){L1HT4JetCut = true;}
                  }
              }
          }
      }
  }

  if(L1JetIndices.size()>0){
      if(triggers_noFilter_PFDeepCSVPuppi->rescaleL1TpT(event->L1_PuppiJets_pt[L1JetIndices.at(0)], event->L1_PuppiJets_eta[L1JetIndices.at(0)])>200.){
          L1SingleJetSeed = true;
      }
  }
  if(selectedjets_forSingleJet.size()>0){
      if(selectedjets_forSingleJet.at(0)->pt>530.)  HLTSingleJet75Hz=true;
      if(selectedjets_forSingleJet.at(0)->pt>560.)  HLTSingleJet50Hz=true;
  }


  for(const nTupleAnalysis::jetPtr& jet : event->puppiJets){
      // perform for GEN a HLT like selection of jets
    if(!jet->hasMatch) continue;
    if(fabs(jet->GenJet_eta) > eta_cut) continue;
    if(jet->p.DeltaR(jet->p_gen)>0.1) continue;
    if(jet->isUndefined) continue;
    if(jet->GenJet_pt       < pt_cut)       continue; // 40 ?
    selectedjets_gen.push_back(jet);
  }

  if(selectedjets_gen.size()>3){
      if (selectedjets_gen.at(0)->pt>30){
          if (selectedjets_gen.at(1)->pt>30){
              if (selectedjets_gen.at(2)->pt>30){
                  if (selectedjets_gen.at(3)->pt>30){
                      if(event->genHT > 100.){
                          // if(countTrueBJets>2){
                          if(countTrueBJets>3){
                              emulaterBit_GEN_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4 = true;
                              // same signal def
                              emulaterBit_GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4 = true;
                          }
                      }
                  }
              }
          }
      }
  }

  if(selectedjets_gen.size()>1){
      if (selectedjets_gen.at(0)->pt>30){
          if (selectedjets_gen.at(1)->pt>30){
              if(countTrueBJets>1){
                  emulaterBit_GEN_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4 = true;
                  // same signal def
                  emulaterBit_GEN_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4 = true;
              }
          }
      }
  }


  event->HLT_HT_2p4_sumEt = recoHTTemp;

  if(event->HLT_HT_2p4_sumEt> 330.){
      hasRecoHTGreater330 = true;
  }
  if(event->HLT_HT_2p4_sumEt> 100.){
  // if(event->HLT_HT_2p4_sumEt> 200.){
      hasRecoHTGreater100 = true;
  }
  if(selectedjets.size()>3){
      if(selectedjets.at(0)->pt>75. && selectedjets.at(1)->pt>60. && selectedjets.at(2)->pt>45. && selectedjets.at(3)->pt>40.){
          has4jetsWithPTCuts = true;
    }
      if(selectedjets.at(0)->pt>30. && selectedjets.at(1)->pt>30. && selectedjets.at(2)->pt>30. && selectedjets.at(3)->pt>30.){
      // if(selectedjets.at(0)->pt>70. && selectedjets.at(1)->pt>50. && selectedjets.at(2)->pt>30. && selectedjets.at(3)->pt>30.){
          has4jetsWithPTCutsLoose = true;
    }
  }

  // doublejets
  for(uint i=0; i<selectedjets.size();++i){
      if(selectedjets.at(i)->pt>128.) selectedjets128.push_back(selectedjets.at(i));
      if(selectedjets.at(i)->pt>30.) selectedjets30.push_back(selectedjets.at(i));
  }
  if(selectedjets128.size()>1) has2jetsWithPTCuts = true;
  if(selectedjets30.size()>1) has2jetsWithPTCutsLoose = true;

// max deta
    if(selectedjets128.size()>1){
      int n(0);
      for(uint i1=0; i1<selectedjets128.size();++i1){
          for(uint i2=i1+1; i2<selectedjets128.size();++i2){
              if(fabs(selectedjets128.at(i1)->eta-selectedjets128.at(i2)->eta)<=1.6) ++n;
          }
      }
      if(n>0) has2jetswithMaxDEta=true;
    }
    if(selectedjets30.size()>1){
      int n(0);
      for(uint i1=0; i1<selectedjets30.size();++i1){
          for(uint i2=i1+1; i2<selectedjets30.size();++i2){
              if(fabs(selectedjets30.at(i1)->eta-selectedjets30.at(i2)->eta)<=1.6) ++n;
          }
      }
      if(n>0)has2jetswithMaxDEtaLoose=true;
    }



// btags

  int countRecoBTagsGT0p5(0);
  for(const nTupleAnalysis::jetPtr& jet : selectedjets){
      if(jet->DeepCSV>0.5){
          ++countRecoBTagsGT0p5;
      }
  }


  float cutToUseNew = 0.27;
  float cutToUseDouble= 0.75;
  float cutToUseOld = 0.17;
  if (eta_cut>3.){
      cutToUseNew = 0.37;
      cutToUseOld = 0.32;
      cutToUseDouble = 0.75;
  }

  // if(countRecoBTagsGT0p5>2){
  //     has3jetsWithBTags = true;
  // }
  // if(countRecoBTagsGT0p5>1){
  //     has2jetsWithBTags = true;
  // }

  std::vector<float> btagScores;
  for (const nTupleAnalysis::jetPtr& jet : selectedjets){
      btagScores.push_back(jet->DeepCSV);
  }
  std::sort(btagScores.begin(), btagScores.end(), std::greater<float>());
  // more final
    if(btagScores.size()>3){
        if(btagScores.at(2)>cutToUseNew){
            has3jetsWithBTagsNew = true;
        }
        if(btagScores.at(2)>cutToUseOld){
            has3jetsWithBTagsOld = true;
        }
    }
    if(btagScores.size()>1){
        if(btagScores.at(1)>cutToUseDouble){
            has2jetsWithBTagsNew = true;
        }
        if(btagScores.at(1)>cutToUseDouble){
            has2jetsWithBTagsOld = true;
        }
    }









  if(emulaterBit_GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4){
      if(getConvertedBit("HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4_v1", convertedBits)){
          triggers_HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4_v1->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      if(getConvertedBit("HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_2p4_v1", convertedBits)){
          triggers_HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_2p4_v1->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      if(getConvertedBit("HLT_QuadPFPuppiJet_75_60_45_40_2p4_v1", convertedBits)){
          triggers_HLT_QuadPFPuppiJet_75_60_45_40_2p4_v1->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      if(getConvertedBit("HLTNoL1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_2p4_v1", convertedBits)){
          triggers_HLTNoL1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_2p4_v1->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      if(getConvertedBit("HLTNoL1_QuadPFPuppiJet_75_60_45_40_2p4_v1", convertedBits)){
          triggers_HLTNoL1_QuadPFPuppiJet_75_60_45_40_2p4_v1->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      // if(getConvertedBit("L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV_2p4_v1", convertedBits)){
      if(L1HT4JetCut){
          triggers_L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV_2p4_v1->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }

      // if(hasRecoHTGreater330 && getConvertedBit("L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV_2p4_v1", convertedBits)){
      if(hasRecoHTGreater330 && L1HT4JetCut){
          triggers_L1PlusRECO_PFHT330PT30->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      // if(hasRecoHTGreater330 && has4jetsWithPTCuts && getConvertedBit("L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV_2p4_v1", convertedBits)){
      if(hasRecoHTGreater330 && has4jetsWithPTCuts && L1HT4JetCut){
          triggers_L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      // if(hasRecoHTGreater330 && has4jetsWithPTCuts && has3jetsWithBTags && getConvertedBit("L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV_2p4_v1", convertedBits)){
      // if(hasRecoHTGreater330 && has4jetsWithPTCuts && has3jetsWithBTagsOld && getConvertedBit("L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV_2p4_v1", convertedBits)){
      if(hasRecoHTGreater330 && has4jetsWithPTCuts && has3jetsWithBTagsOld && L1HT4JetCut){
          triggers_L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      // if(getConvertedBit("L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV_2p4_v1", convertedBits)){
      if(L1HT4JetCut){
          triggers_L1_PFHT330PT30->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
          triggers_L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
          triggers_L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      if(hasRecoHTGreater330){
          triggers_RECO_PFHT330PT30->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      if(hasRecoHTGreater330 && has4jetsWithPTCuts){
          triggers_RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      // if(hasRecoHTGreater330 && has4jetsWithPTCuts && has3jetsWithBTags){
      if(hasRecoHTGreater330 && has4jetsWithPTCuts && has3jetsWithBTagsOld){
          triggers_RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }

      triggers_GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4 ->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
  }

  // loose edition
  if(emulaterBit_GEN_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4){

      // if(getConvertedBit("L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV_2p4_v1", convertedBits)){
      if(L1HT4JetCut){
          triggers_L1_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV_2p4_v1->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      // if(hasRecoHTGreater100 && getConvertedBit("L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV_2p4_v1", convertedBits)){
      if(hasRecoHTGreater100 && L1HT4JetCut){
          triggers_L1PlusRECO_PFHT100PT30->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      // if(hasRecoHTGreater100 && has4jetsWithPTCutsLoose && getConvertedBit("L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV_2p4_v1", convertedBits)){
      if(hasRecoHTGreater100 && has4jetsWithPTCutsLoose && L1HT4JetCut){
          triggers_L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      // if(hasRecoHTGreater100 && has4jetsWithPTCutsLoose && has3jetsWithBTags && getConvertedBit("L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV_2p4_v1", convertedBits)){
      // if(hasRecoHTGreater100 && has4jetsWithPTCutsLoose && has3jetsWithBTagsNew && getConvertedBit("L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV_2p4_v1", convertedBits)){
      if(hasRecoHTGreater100 && has4jetsWithPTCutsLoose && has3jetsWithBTagsNew && L1HT4JetCut){
          triggers_L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      // if(getConvertedBit("L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV_2p4_v1", convertedBits)){
      if(L1HT4JetCut){
          triggers_L1_PFHT100PT30->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
          triggers_L1_PFHT100PT30_QuadPFPuppiJet_30_30_30_30->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
          triggers_L1_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      if(hasRecoHTGreater100){
          triggers_RECO_PFHT100PT30->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      if(hasRecoHTGreater100 && has4jetsWithPTCutsLoose){
          triggers_RECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      // if(hasRecoHTGreater100 && has4jetsWithPTCutsLoose && has3jetsWithBTags){
      if(hasRecoHTGreater100 && has4jetsWithPTCutsLoose && has3jetsWithBTagsNew){
          triggers_RECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }

      triggers_GEN_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4 ->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
  }



    // Rates

    // if(getConvertedBit("L1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p71_2p4_v1", convertedBits)){
    if(L1DEtaCut){
        rates_L1_DoublePFPuppiJets128MaxDeta1p6->Fill(event, selectedjets, selectedjets_forBTags, eventWeight);
        if(has2jetsWithPTCuts) rates_L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6->Fill(event, selectedjets, selectedjets_forBTags, eventWeight);
        if(has2jetsWithPTCuts){
            if(has2jetsWithBTagsOld) rates_L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6_DeepCSVCuts->Fill(event, selectedjets, selectedjets_forBTags, eventWeight);
        }
        if(has2jetsWithPTCutsLoose && has2jetswithMaxDEtaLoose) rates_L1PlusRECO_DoublePFPuppiJets30MaxDeta1p6->Fill(event, selectedjets, selectedjets_forBTags, eventWeight);
        if(has2jetsWithPTCutsLoose && has2jetswithMaxDEtaLoose){
             if(has2jetsWithBTagsNew) rates_L1PlusRECO_DoublePFPuppiJets30MaxDeta1p6_DeepCSVCuts->Fill(event, selectedjets, selectedjets_forBTags, eventWeight);
         }
    }
    // if(getConvertedBit("L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV_2p4_v1", convertedBits)){
    if(L1HT4JetCut){
        rates_L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40->Fill(event, selectedjets, selectedjets_forBTags, eventWeight);
        if(hasRecoHTGreater330 && has4jetsWithPTCuts) rates_L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40->Fill(event, selectedjets, selectedjets_forBTags, eventWeight);
        if(hasRecoHTGreater330 && has4jetsWithPTCuts && has3jetsWithBTagsOld) rates_L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_DeepCSVCuts->Fill(event, selectedjets, selectedjets_forBTags, eventWeight);
        if(hasRecoHTGreater100 && has4jetsWithPTCutsLoose) rates_L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30->Fill(event, selectedjets, selectedjets_forBTags, eventWeight);
        if(hasRecoHTGreater100 && has4jetsWithPTCutsLoose && has3jetsWithBTagsNew) rates_L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_DeepCSVCuts->Fill(event, selectedjets, selectedjets_forBTags, eventWeight);
    }

    if(selectedjets_gen.size()>1){
        GEN_Dijet->Fill(event, selectedjets, selectedjets_forBTags, eventWeight);
    }
    if(L1SingleJetSeed){
        rates_L1_SingleJet50Hz->Fill(event, selectedjets, selectedjets_forBTags, eventWeight);
        rates_L1_SingleJet75Hz->Fill(event, selectedjets, selectedjets_forBTags, eventWeight);
        if(HLTSingleJet50Hz) rates_L1PlusReco_SingleJet50Hz->Fill(event, selectedjets, selectedjets_forBTags, eventWeight);
        if(HLTSingleJet75Hz) rates_L1PlusReco_SingleJet75Hz->Fill(event, selectedjets, selectedjets_forBTags, eventWeight);
    }
    if(HLTSingleJet50Hz) rates_Reco_SingleJet50Hz->Fill(event, selectedjets, selectedjets_forBTags, eventWeight);
    if(HLTSingleJet75Hz) rates_Reco_SingleJet75Hz->Fill(event, selectedjets, selectedjets_forBTags, eventWeight);






  if(emulaterBit_GEN_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4){
      if(getConvertedBit("HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4_v1", convertedBits)){
          triggers_HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4_v1->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      if(getConvertedBit("HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppi_2p4_v1", convertedBits)){
          triggers_HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppi_2p4_v1->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      if(getConvertedBit("HLTNoL1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppi_2p4_v1", convertedBits)){
          triggers_HLTNoL1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppi_2p4_v1->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      // if(getConvertedBit("L1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p71_2p4_v1", convertedBits)){
      if(L1DEtaCut){
          triggers_L1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p71_2p4_v1->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }

      // if(has2jetsWithPTCuts && getConvertedBit("L1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p71_2p4_v1", convertedBits)){
      if(has2jetsWithPTCuts && has2jetswithMaxDEta && L1DEtaCut){
          triggers_L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      // if(has2jetsWithPTCuts && has2jetsWithBTags && getConvertedBit("L1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p71_2p4_v1", convertedBits)){
      // if(has2jetsWithPTCuts && has2jetsWithBTagsOld && getConvertedBit("L1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p71_2p4_v1", convertedBits)){
      if(has2jetsWithPTCuts && has2jetswithMaxDEta && has2jetsWithBTagsOld && L1DEtaCut){
          triggers_L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      // if(getConvertedBit("L1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p71_2p4_v1", convertedBits)){
      if(L1DEtaCut){
          triggers_L1_DoublePFPuppiJets128MaxDeta1p6->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
          triggers_L1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      if(has2jetsWithPTCuts){
          triggers_RECO_DoublePFPuppiJets128MaxDeta1p6->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      // if(has2jetsWithPTCuts && has2jetsWithBTags){
      if(has2jetsWithPTCuts && has2jetswithMaxDEta && has2jetsWithBTagsOld){
          triggers_RECO_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }

      triggers_GEN_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4 ->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
  }

// loose edition
  if(emulaterBit_GEN_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4){
      // if(getConvertedBit("HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4_v1", convertedBits)){
      //     triggers_HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4_v1->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      // }
      // if(getConvertedBit("HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppi_2p4_v1", convertedBits)){
      //     triggers_HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppi_2p4_v1->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      // }
      // if(getConvertedBit("HLTNoL1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppi_2p4_v1", convertedBits)){
      //     triggers_HLTNoL1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppi_2p4_v1->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      // }
      // if(getConvertedBit("L1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p71_2p4_v1", convertedBits)){
      if(L1DEtaCut){
          triggers_L1_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p71_2p4_v1->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }

      // if(has2jetsWithPTCutsLoose && getConvertedBit("L1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p71_2p4_v1", convertedBits)){
      if(L1DEtaCut){
          triggers_L1PlusRECO_DoublePFPuppiJets30MaxDeta1p6->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      // if(has2jetsWithPTCutsLoose && has2jetsWithBTagsNew && getConvertedBit("L1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p71_2p4_v1", convertedBits)){
      if(has2jetsWithPTCutsLoose && has2jetswithMaxDEtaLoose && has2jetsWithBTagsNew && L1DEtaCut){
          triggers_L1PlusRECO_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      // if(getConvertedBit("L1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p71_2p4_v1", convertedBits)){
      if(L1DEtaCut){
          triggers_L1_DoublePFPuppiJets30MaxDeta1p6->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
          triggers_L1_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      if(has2jetsWithPTCutsLoose&&has2jetswithMaxDEtaLoose){
          triggers_RECO_DoublePFPuppiJets30MaxDeta1p6->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }
      // if(has2jetsWithPTCutsLoose && has2jetsWithBTags){
      if(has2jetsWithPTCutsLoose && has2jetswithMaxDEtaLoose && has2jetsWithBTagsNew){
          triggers_RECO_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
      }

      triggers_GEN_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4 ->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);
  }








  // inclusive studies
  triggers_noFilter_PFDeepCSVPuppi->Fill(event, selectedjets_genMatched, selectedjets_forBTags, eventWeight);













  return 0;
}

bool HH4bAnalysis::getConvertedBit(const string& pathname, const std::vector<bool>& convertedBits){
    int nBit = triggerMap[pathname].first;
    int nIndex = triggerMap[pathname].second;
    return convertedBits[nBit*32+nIndex];
}

bool HH4bAnalysis::passTrigBit(const unsigned int nBit, const unsigned int trigIndex){
    int mask =  1 << trigIndex;
    int masked_n = event->BitTrigger[nBit] & mask;
    int thebit = masked_n >> trigIndex;
    // return bool(event->BitTrigger[nBit] & (1 << trigIndex));
    return bool(thebit);
}

HH4bAnalysis::~HH4bAnalysis(){
  cout << "HH4bAnalysis::destroyed" << endl;
}
