import ROOT
ROOT.gROOT.SetBatch(True)
import ROOTHelp.FancyROOTStyle
import numpy as np


from optparse import OptionParser
p = OptionParser()
p.add_option('--inputMC',  type = 'string', dest = 'inFileMC', help = 'intput File' )
p.add_option('--output', type = 'string', default = "jetLevelPlots", dest = 'outDir', help = 'output dir' )
p.add_option('--cmsText', type = 'string', default = "Work in Progress",  help = '' )
p.add_option('--lumiText', default = "",  help = '' )
(o,a) = p.parse_args()

inFileMC    = ROOT.TFile(o.inFileMC,  "READ")

import os
if not os.path.exists(o.outDir):
    os.makedirs(o.outDir)

from JetLevelPlotUtils import makeEff, drawComp, getHist, drawStackCompRatio, makeStack, makeInverseTurnOn, make2DComp, makeInverseTurnOnAll,plotRatio

effBinning=1

eff_Reco_HT330_ht_gen = makeEff("ht_gen" , ["RECO_PFHT330PT30", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_ht_gen = makeEff("ht_gen" , ["L1PlusRECO_PFHT330PT30", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_ht_gen = makeEff("ht_gen" , ["L1_PFHT330PT30", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_Reco_HT330_ht_reco = makeEff("ht_reco" , ["RECO_PFHT330PT30", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_ht_reco = makeEff("ht_reco" , ["L1PlusRECO_PFHT330PT30", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_ht_reco = makeEff("ht_reco" , ["L1_PFHT330PT30", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_Reco_HT330_m4j_gen = makeEff("m4j_gen" , ["RECO_PFHT330PT30", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_m4j_gen = makeEff("m4j_gen" , ["L1PlusRECO_PFHT330PT30", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_m4j_gen = makeEff("m4j_gen" , ["L1_PFHT330PT30", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)

drawComp("Eff_HT330_ht_gen",
         [(eff_Reco_HT330_ht_gen,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_ht_gen,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_ht_gen,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 1000
         ,yTitle="Efficiency",xTitle="H_{T}^{gen}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)
drawComp("Eff_HT330_ht_reco",
         [(eff_Reco_HT330_ht_reco,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_ht_reco,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_ht_reco,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 1000
         ,yTitle="Efficiency",xTitle="H_{T}^{reco}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)
drawComp("Eff_HT330_m4j_gen",
         [(eff_Reco_HT330_m4j_gen,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_m4j_gen,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_m4j_gen,"L1",ROOT.kGreen),
          ]
          ,xMin = 200., xMax = 1000
         ,yTitle="Efficiency",xTitle="m_{jjjj}^{gen}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)

eff_Reco_HT330_4JetPt_ht_gen = makeEff("ht_gen" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_ht_gen = makeEff("ht_gen" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_ht_gen = makeEff("ht_gen" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)

eff_Reco_HT330_4JetPt_pt4_gen = makeEff("pt4_gen" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_pt4_gen = makeEff("pt4_gen" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_pt4_gen = makeEff("pt4_gen" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_Reco_HT330_4JetPt_pt3_gen = makeEff("pt3_gen" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_pt3_gen = makeEff("pt3_gen" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_pt3_gen = makeEff("pt3_gen" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_Reco_HT330_4JetPt_pt2_gen = makeEff("pt2_gen" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_pt2_gen = makeEff("pt2_gen" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_pt2_gen = makeEff("pt2_gen" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_Reco_HT330_4JetPt_pt1_gen = makeEff("pt1_gen" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_pt1_gen = makeEff("pt1_gen" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_pt1_gen = makeEff("pt1_gen" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)

eff_Reco_HT330_4JetPt_pt4_reco = makeEff("pt4_reco" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_pt4_reco = makeEff("pt4_reco" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_pt4_reco = makeEff("pt4_reco" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_Reco_HT330_4JetPt_pt3_reco = makeEff("pt3_reco" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_pt3_reco = makeEff("pt3_reco" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_pt3_reco = makeEff("pt3_reco" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_Reco_HT330_4JetPt_pt2_reco = makeEff("pt2_reco" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_pt2_reco = makeEff("pt2_reco" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_pt2_reco = makeEff("pt2_reco" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_Reco_HT330_4JetPt_pt1_reco = makeEff("pt1_reco" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_pt1_reco = makeEff("pt1_reco" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_pt1_reco = makeEff("pt1_reco" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)

eff_Reco_HT330_4JetPt_m4j_gen = makeEff("m4j_gen" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_m4j_gen = makeEff("m4j_gen" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_m4j_gen = makeEff("m4j_gen" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)

eff_Reco_HT330_4JetPt_tag4 = makeEff("deepCSV_jet4" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_tag4 = makeEff("deepCSV_jet4" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_tag4 = makeEff("deepCSV_jet4" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_Reco_HT330_4JetPt_tag3 = makeEff("deepCSV_jet3" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_tag3 = makeEff("deepCSV_jet3" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_tag3 = makeEff("deepCSV_jet3" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_Reco_HT330_4JetPt_tag2 = makeEff("deepCSV_jet2" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_tag2 = makeEff("deepCSV_jet2" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_tag2 = makeEff("deepCSV_jet2" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_Reco_HT330_4JetPt_tag1 = makeEff("deepCSV_jet1" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_tag1 = makeEff("deepCSV_jet1" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_tag1 = makeEff("deepCSV_jet1" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)


drawComp("Eff_HT330_4JetPt_ht_gen",
         [(eff_Reco_HT330_4JetPt_ht_gen,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_ht_gen,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_ht_gen,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 1000
         ,yTitle="Efficiency",xTitle="H_{T}^{gen}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)

drawComp("Eff_HT330_4JetPt_pt4_gen",
         [(eff_Reco_HT330_4JetPt_pt4_gen,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_pt4_gen,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_pt4_gen,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 400
         ,yTitle="Efficiency",xTitle="p_{T}^{4th gen jet}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)
drawComp("Eff_HT330_4JetPt_pt3_gen",
         [(eff_Reco_HT330_4JetPt_pt3_gen,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_pt3_gen,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_pt3_gen,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 400
         ,yTitle="Efficiency",xTitle="p_{T}^{3rd gen jet}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)
drawComp("Eff_HT330_4JetPt_pt2_gen",
         [(eff_Reco_HT330_4JetPt_pt2_gen,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_pt2_gen,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_pt2_gen,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 400
         ,yTitle="Efficiency",xTitle="p_{T}^{2nd gen jet}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)
drawComp("Eff_HT330_4JetPt_pt1_gen",
         [(eff_Reco_HT330_4JetPt_pt1_gen,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_pt1_gen,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_pt1_gen,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 400
         ,yTitle="Efficiency",xTitle="p_{T}^{1st gen jet}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)

drawComp("Eff_HT330_4JetPt_pt4_reco",
         [(eff_Reco_HT330_4JetPt_pt4_reco,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_pt4_reco,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_pt4_reco,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 400
         ,yTitle="Efficiency",xTitle="p_{T}^{4th reco jet}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)
drawComp("Eff_HT330_4JetPt_pt3_reco",
         [(eff_Reco_HT330_4JetPt_pt3_reco,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_pt3_reco,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_pt3_reco,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 400
         ,yTitle="Efficiency",xTitle="p_{T}^{3rd reco jet}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)
drawComp("Eff_HT330_4JetPt_pt2_reco",
         [(eff_Reco_HT330_4JetPt_pt2_reco,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_pt2_reco,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_pt2_reco,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 400
         ,yTitle="Efficiency",xTitle="p_{T}^{2nd reco jet}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)
drawComp("Eff_HT330_4JetPt_pt1_reco",
         [(eff_Reco_HT330_4JetPt_pt1_reco,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_pt1_reco,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_pt1_reco,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 400
         ,yTitle="Efficiency",xTitle="p_{T}^{1st reco jet}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)

drawComp("Eff_HT330_4JetPt_m4j_gen",
         [(eff_Reco_HT330_4JetPt_m4j_gen,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_m4j_gen,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_m4j_gen,"L1",ROOT.kGreen),
          ]
          ,xMin = 200., xMax = 800
         ,yTitle="Efficiency",xTitle="m_{jjjj}^{gen}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)
drawComp("Eff_HT330_4JetPt_DeepCSV4_gen",
         [(eff_Reco_HT330_4JetPt_tag4,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_tag4,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_tag4,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 400
         ,yTitle="Efficiency",xTitle="DeepCSV_{4th gen jet}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)
drawComp("Eff_HT330_4JetPt_DeepCSV3_gen",
         [(eff_Reco_HT330_4JetPt_tag3,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_tag3,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_tag3,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 400
         ,yTitle="Efficiency",xTitle="DeepCSV_{3rd gen jet}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)
drawComp("Eff_HT330_4JetPt_DeepCSV2_gen",
         [(eff_Reco_HT330_4JetPt_tag2,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_tag2,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_tag2,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 400
         ,yTitle="Efficiency",xTitle="DeepCSV_{2nd gen jet}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)
drawComp("Eff_HT330_4JetPt_DeepCSV1_gen",
         [(eff_Reco_HT330_4JetPt_tag1,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_tag1,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_tag1,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 400
         ,yTitle="Efficiency",xTitle="DeepCSV_{1st gen jet}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)

eff_Reco_HT330_4JetPt_3Tag_ht_gen = makeEff("ht_gen" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_3Tag_ht_gen = makeEff("ht_gen" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_3Tag_ht_gen = makeEff("ht_gen" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)

eff_Reco_HT330_4JetPt_3Tag_pt4_gen = makeEff("pt4_gen" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_3Tag_pt4_gen = makeEff("pt4_gen" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_3Tag_pt4_gen = makeEff("pt4_gen" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_Reco_HT330_4JetPt_3Tag_pt3_gen = makeEff("pt3_gen" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_3Tag_pt3_gen = makeEff("pt3_gen" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_3Tag_pt3_gen = makeEff("pt3_gen" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_Reco_HT330_4JetPt_3Tag_pt2_gen = makeEff("pt2_gen" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_3Tag_pt2_gen = makeEff("pt2_gen" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_3Tag_pt2_gen = makeEff("pt2_gen" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_Reco_HT330_4JetPt_3Tag_pt1_gen = makeEff("pt1_gen" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_3Tag_pt1_gen = makeEff("pt1_gen" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_3Tag_pt1_gen = makeEff("pt1_gen" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)

eff_Reco_HT330_4JetPt_3Tag_tag4 = makeEff("deepCSV_jet4" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_3Tag_tag4 = makeEff("deepCSV_jet4" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_3Tag_tag4 = makeEff("deepCSV_jet4" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_Reco_HT330_4JetPt_3Tag_tag3 = makeEff("deepCSV_jet3" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_3Tag_tag3 = makeEff("deepCSV_jet3" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_3Tag_tag3 = makeEff("deepCSV_jet3" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_Reco_HT330_4JetPt_3Tag_tag2 = makeEff("deepCSV_jet2" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_3Tag_tag2 = makeEff("deepCSV_jet2" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_3Tag_tag2 = makeEff("deepCSV_jet2" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_Reco_HT330_4JetPt_3Tag_tag1 = makeEff("deepCSV_jet1" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_3Tag_tag1 = makeEff("deepCSV_jet1" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_3Tag_tag1 = makeEff("deepCSV_jet1" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)

eff_Reco_HT330_4JetPt_3Tag_m4j_gen = makeEff("m4j_gen" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1PlusReco_HT330_4JetPt_3Tag_m4j_gen = makeEff("m4j_gen" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)
eff_L1_HT330_4JetPt_3Tag_m4j_gen = makeEff("m4j_gen" , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMC, binning=effBinning)



drawComp("Eff_HT330_4JetPt_3tag_ht_gen",
         [(eff_Reco_HT330_4JetPt_3Tag_ht_gen,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_3Tag_ht_gen,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_3Tag_ht_gen,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 1000
         ,yTitle="Efficiency",xTitle="H_{T}^{gen}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)
drawComp("Eff_HT330_4JetPt_3tag_pt4_gen",
         [(eff_Reco_HT330_4JetPt_3Tag_pt4_gen,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_3Tag_pt4_gen,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_3Tag_pt4_gen,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 400
         ,yTitle="Efficiency",xTitle="p_{T}^{4th gen jet}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)
drawComp("Eff_HT330_4JetPt_3tag_pt3_gen",
         [(eff_Reco_HT330_4JetPt_3Tag_pt3_gen,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_3Tag_pt3_gen,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_3Tag_pt3_gen,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 400
         ,yTitle="Efficiency",xTitle="p_{T}^{3rd gen jet}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)
drawComp("Eff_HT330_4JetPt_3tag_pt2_gen",
         [(eff_Reco_HT330_4JetPt_3Tag_pt2_gen,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_3Tag_pt2_gen,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_3Tag_pt2_gen,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 400
         ,yTitle="Efficiency",xTitle="p_{T}^{2nd gen jet}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)
drawComp("Eff_HT330_4JetPt_3tag_pt1_gen",
         [(eff_Reco_HT330_4JetPt_3Tag_pt1_gen,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_3Tag_pt1_gen,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_3Tag_pt1_gen,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 400
         ,yTitle="Efficiency",xTitle="p_{T}^{1st gen jet}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)
drawComp("Eff_HT330_4JetPt_3tag_DeepCSV4_gen",
         [(eff_Reco_HT330_4JetPt_3Tag_tag4,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_3Tag_tag4,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_3Tag_tag4,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 400
         ,yTitle="Efficiency",xTitle="DeepCSV_{4th gen jet}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)
drawComp("Eff_HT330_4JetPt_3tag_DeepCSV3_gen",
         [(eff_Reco_HT330_4JetPt_3Tag_tag3,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_3Tag_tag3,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_3Tag_tag3,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 400
         ,yTitle="Efficiency",xTitle="DeepCSV_{3rd gen jet}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)
drawComp("Eff_HT330_4JetPt_3tag_DeepCSV2_gen",
         [(eff_Reco_HT330_4JetPt_3Tag_tag2,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_3Tag_tag2,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_3Tag_tag2,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 400
         ,yTitle="Efficiency",xTitle="DeepCSV_{2nd gen jet}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)
drawComp("Eff_HT330_4JetPt_3tag_DeepCSV1_gen",
         [(eff_Reco_HT330_4JetPt_3Tag_tag1,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_3Tag_tag1,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_3Tag_tag1,"L1",ROOT.kGreen),
          ]
          ,xMin = 0., xMax = 400
         ,yTitle="Efficiency",xTitle="DeepCSV_{1st gen jet}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)
drawComp("Eff_HT330_4JetPt_3tag_m4j_gen",
         [(eff_Reco_HT330_4JetPt_3Tag_m4j_gen,"HT",ROOT.kBlue),
          (eff_L1PlusReco_HT330_4JetPt_3Tag_m4j_gen,"L1+HT",ROOT.kRed),
          (eff_L1_HT330_4JetPt_m4j_gen,"L1",ROOT.kGreen),
          ]
          ,xMin = 200., xMax = 800
         ,yTitle="Efficiency",xTitle="m_{jjjj}^{gen}", otherText=[""], outDir=o.outDir,
         xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8)
