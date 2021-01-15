// -*- C++ -*-
#if !defined(HH4bAnalysis_H)
#define HH4bAnalysis_H

#include <ctime>
#include <sys/resource.h>

#include <TChain.h>
#include <TString.h>
#include <TTree.h>
#include <TSpline.h>
#include "DataFormats/FWLite/interface/InputSource.h" //for edm::LuminosityBlockRange
#include "nTupleAnalysis/baseClasses/interface/brilCSV.h"
#include "nTupleAnalysis/baseClasses/interface/initBranch.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "TriggerStudies/NtupleAna/interface/eventData.h"
#include "nTupleAnalysis/baseClasses/interface/cutflowHists.h"
#include "nTupleAnalysis/baseClasses/interface/eventHists.h"
#include "nTupleAnalysis/baseClasses/interface/triggers.h"
#include "nTupleAnalysis/baseClasses/interface/mass.h"
using std::vector;  using std::map; using std::string; using std::set; using std::pair;

namespace TriggerStudies {

  class HH4bAnalysis {
  public:

    map<string, pair<int,int> > triggerMap;

    TChain* eventsRAW;

    bool debug = false;
    TFileDirectory dir;

    int histogramming = 1e6;
    int treeEvents;
    eventData* event;

    nTupleAnalysis::eventHists* hEvents;
    nTupleAnalysis::cutflowHists* cutflow;

    nTupleAnalysis::triggers* triggers_HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4_v1;
    nTupleAnalysis::triggers* triggers_HLT_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_2p4_v1;
    nTupleAnalysis::triggers* triggers_HLT_QuadPFPuppiJet_75_60_45_40_2p4_v1;
    nTupleAnalysis::triggers* triggers_HLTNoL1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_2p4_v1;
    nTupleAnalysis::triggers* triggers_HLTNoL1_QuadPFPuppiJet_75_60_45_40_2p4_v1;
    nTupleAnalysis::triggers* triggers_L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV_2p4_v1;
    nTupleAnalysis::triggers* triggers_L1_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV_2p4_v1;

    nTupleAnalysis::triggers* triggers_HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4_v1;
    nTupleAnalysis::triggers* triggers_HLT_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppi_2p4_v1;
    nTupleAnalysis::triggers* triggers_HLTNoL1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppi_2p4_v1;
    nTupleAnalysis::triggers* triggers_L1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p71_2p4_v1;
    nTupleAnalysis::triggers* triggers_L1_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p71_2p4_v1;

    nTupleAnalysis::triggers* triggers_GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4;
    nTupleAnalysis::triggers* triggers_GEN_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4;

    nTupleAnalysis::triggers* triggers_L1PlusRECO_PFHT330PT30;
    nTupleAnalysis::triggers* triggers_L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40;
    nTupleAnalysis::triggers* triggers_L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4;
    nTupleAnalysis::triggers* triggers_L1_PFHT330PT30;
    nTupleAnalysis::triggers* triggers_L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40;
    nTupleAnalysis::triggers* triggers_L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4;
    nTupleAnalysis::triggers* triggers_RECO_PFHT330PT30;
    nTupleAnalysis::triggers* triggers_RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40;
    nTupleAnalysis::triggers* triggers_RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4;

    nTupleAnalysis::triggers* triggers_L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6;
    nTupleAnalysis::triggers* triggers_L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4;
    nTupleAnalysis::triggers* triggers_L1_DoublePFPuppiJets128MaxDeta1p6;
    nTupleAnalysis::triggers* triggers_L1_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4;
    nTupleAnalysis::triggers* triggers_RECO_DoublePFPuppiJets128MaxDeta1p6;
    nTupleAnalysis::triggers* triggers_RECO_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4;

    // loose edition
    nTupleAnalysis::triggers* triggers_GEN_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4;
    nTupleAnalysis::triggers* triggers_GEN_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4;

    nTupleAnalysis::triggers* triggers_L1PlusRECO_PFHT100PT30;
    nTupleAnalysis::triggers* triggers_L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30;
    nTupleAnalysis::triggers* triggers_L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4;
    nTupleAnalysis::triggers* triggers_L1_PFHT100PT30;
    nTupleAnalysis::triggers* triggers_L1_PFHT100PT30_QuadPFPuppiJet_30_30_30_30;
    nTupleAnalysis::triggers* triggers_L1_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4;
    nTupleAnalysis::triggers* triggers_RECO_PFHT100PT30;
    nTupleAnalysis::triggers* triggers_RECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30;
    nTupleAnalysis::triggers* triggers_RECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4;

    nTupleAnalysis::triggers* triggers_L1PlusRECO_DoublePFPuppiJets30MaxDeta1p6;
    nTupleAnalysis::triggers* triggers_L1PlusRECO_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4;
    nTupleAnalysis::triggers* triggers_L1_DoublePFPuppiJets30MaxDeta1p6;
    nTupleAnalysis::triggers* triggers_L1_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4;
    nTupleAnalysis::triggers* triggers_RECO_DoublePFPuppiJets30MaxDeta1p6;
    nTupleAnalysis::triggers* triggers_RECO_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4;

    nTupleAnalysis::triggers* rates_L1_SingleJet50Hz;
    nTupleAnalysis::triggers* rates_L1PlusReco_SingleJet50Hz;
    nTupleAnalysis::triggers* rates_Reco_SingleJet50Hz;
    nTupleAnalysis::triggers* rates_L1_SingleJet75Hz;
    nTupleAnalysis::triggers* rates_L1PlusReco_SingleJet75Hz;
    nTupleAnalysis::triggers* rates_Reco_SingleJet75Hz;
    nTupleAnalysis::triggers* GEN_Dijet;

    nTupleAnalysis::triggers* rates_L1_DoublePFPuppiJets128MaxDeta1p6;
    nTupleAnalysis::triggers* rates_L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6;
    nTupleAnalysis::triggers* rates_L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6_DeepCSVCuts;
    nTupleAnalysis::triggers* rates_L1PlusRECO_DoublePFPuppiJets30MaxDeta1p6;
    nTupleAnalysis::triggers* rates_L1PlusRECO_DoublePFPuppiJets30MaxDeta1p6_DeepCSVCuts;
    nTupleAnalysis::triggers* rates_L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40;
    nTupleAnalysis::triggers* rates_L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40;
    nTupleAnalysis::triggers* rates_L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30;
    nTupleAnalysis::triggers* rates_L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_DeepCSVCuts;
    nTupleAnalysis::triggers* rates_L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_DeepCSVCuts;


    nTupleAnalysis::triggers* triggers_noFilter_PFDeepCSVPuppi;

    nTupleAnalysis::mass* mass_preCut;

    long int nEvents = 0;

    double intLumi = 75.0e33;

    double xSec = 0.;

    std::map <TString, float> crossSections14TeV;

    float jetEtaCut = 0.;

    //Monitoring Variables
    long int percent;
    std::clock_t start;
    std::clock_t lastTime;
    double duration;
    double timeStep;
    double eventRateAVE;
    double eventRate;
    long int    eventStep;
    long int    lastEvent;
    double timeRemaining;
    int minutes;
    int seconds;
    int who = RUSAGE_SELF;
    struct rusage usage;
    long int usageMB;


    HH4bAnalysis(TChain* _eventsRAW, fwlite::TFileService& fs, bool _debug);
    void monitor(long int);
    int eventLoop(int, int nSkipEvents = 0, double etaCut = 2.4);
    int processEvent();
    bool getConvertedBit(const string& pathname, const std::vector<bool>& convertedBits);
    bool passTrigBit(const unsigned int nBit,const unsigned int trigIndex);

    ~HH4bAnalysis();

  };

}
#endif // HH4bAnalysis_H
