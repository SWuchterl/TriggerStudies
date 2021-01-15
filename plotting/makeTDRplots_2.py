import ROOT
ROOT.gROOT.SetBatch(True)
import ROOTHelp.FancyROOTStyle
import numpy as np
import math

from optparse import OptionParser
p = OptionParser()
p.add_option('--inSignal',  type = 'string', dest = 'inFileMCSignal', help = 'intput File' )
p.add_option('--inRates',  type = 'string', dest = 'inFileMCRates', help = 'intput File' )
p.add_option('--out', type = 'string', default = "jetLevelPlots", dest = 'outDir', help = 'output dir' )
# p.add_option('--cmsText', type = 'string', default = "Work in Progress",  help = '' )
p.add_option('--cmsText', type = 'string', default = "",  help = '' )
p.add_option('--lumiText', default = "Phase 2 (14 TeV)",  help = '' )
p.add_option('--doTurnOn', action="store_true",  help = '' )
p.add_option('--doEffs', action="store_true",  help = '' )
p.add_option('--doHHEff', action="store_true",  help = '' )
p.add_option('--doRates', action="store_true",  help = '' )
p.add_option('--doShapes', action="store_true",  help = '' )
(o,a) = p.parse_args()

inFileMCSignal    = ROOT.TFile(o.inFileMCSignal,  "READ")
inFileMCRates    = ROOT.TFile(o.inFileMCRates,  "READ")

import os
if not os.path.exists(o.outDir):
    os.makedirs(o.outDir)

from JetLevelPlotUtils import makeEff, drawComp, getHist, drawStackCompRatio, makeStack, makeInverseTurnOn, make2DComp, makeInverseTurnOnAll,plotRatio,getInverseTurnOn, getCMSText

xTitlesAndBinnings = {
    "ht_gen":       ["H_{T}^{GEN}",             [0,10,20,30,40,50,60,70,80,90,100,125,150,175,200,250,300]],
    "ht_reco":      ["H_{T}^{HLT}",             [0,10,20,30,40,50,60,70,80,90,100,125,150,175,200,250,300]],
    "ht_l1":        ["H_{T}^{L1T}",             [0,10,20,30,40,50,60,70,80,90,100,125,150,175,200,250,300]],
    "m4j_gen":      ["m_{4b}^{GEN}",          [0,10,20,30,40,50,60,70,80,90,100,125,150,175,200,250,300]],
    "m4j_reco":     ["m_{4b}^{HLT}",           [0,10,20,30,40,50,60,70,80,90,100,125,150,175,200,250,300]],
    "m4j_l1":       ["m_{4b}^{L1T}",          [0,10,20,30,40,50,60,70,80,90,100,125,150,175,200,250,300]],
    "m2j_gen":      ["m_{2b}^{GEN}",            [0,10,20,30,40,50,60,70,80,90,100,125,150,175,200,250,300]],
    "m2j_reco":     ["m_{2b}^{HLT}",            [0,10,20,30,40,50,60,70,80,90,100,125,150,175,200,250,300]],
    "m2j_l1":       ["m_{2b}^{L1T}",            [0,10,20,30,40,50,60,70,80,90,100,125,150,175,200,250,300]],
    "pt1_gen":      ["p_{T}^{1st jet, gen}",    [0,10,20,30,40,50,60,70,80,90,100,125,150,175,200,250,300]],
    "pt1_reco":     ["p_{T}^{1st jet, HLT}",    [0,10,20,30,40,50,60,70,80,90,100,125,150,175,200,250,300]],
    "pt1_l1":       ["p_{T}^{1st jet, L1T}",    [0,10,20,30,40,50,60,70,80,90,100,125,150,175,200,250,300]],
    "pt2_gen":      ["p_{T}^{2nd jet, gen}",    [0,10,20,30,40,50,60,70,80,90,100,125,150,175,200,250,300]],
    "pt2_reco":     ["p_{T}^{2nd jet, HLT}",    [0,10,20,30,40,50,60,70,80,90,100,125,150,175,200,250,300]],
    "pt2_l1":       ["p_{T}^{2nd jet, L1T}",    [0,10,20,30,40,50,60,70,80,90,100,125,150,175,200,250,300]],
    "pt3_gen":      ["p_{T}^{3rd jet, gen}",    [0,10,20,30,40,50,60,70,80,90,100,125,150,175,200,250,300]],
    "pt3_reco":     ["p_{T}^{3rd jet, HLT}",    [0,10,20,30,40,50,60,70,80,90,100,125,150,175,200,250,300]],
    "pt3_l1":       ["p_{T}^{3rd jet, L1T}",    [0,10,20,30,40,50,60,70,80,90,100,125,150,175,200,250,300]],
    "pt4_gen":      ["p_{T}^{4th jet, gen}",    [0,10,20,30,40,50,60,70,80,90,100,125,150,175,200,250,300]],
    "pt4_reco":     ["p_{T}^{4th jet, HLT}",    [0,10,20,30,40,50,60,70,80,90,100,125,150,175,200,250,300]],
    "pt4_l1":       ["p_{T}^{4th jet, L1T}",    [0,10,20,30,40,50,60,70,80,90,100,125,150,175,200,250,300]],
    "deepCSV_jet1": ["DeepCSV_{1st jet}",       [-0.5,0.,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.5,0.6,0.7,0.8,0.9,1.0]],
    "deepCSV_jet2": ["DeepCSV_{2nd jet}",       [-0.5,0.,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.5,0.6,0.7,0.8,0.9,1.0]],
    "deepCSV_jet3": ["DeepCSV_{3rd jet}",       [-0.5,0.,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.5,0.6,0.7,0.8,0.9,1.0]],
    "deepCSV_jet4": ["DeepCSV_{1st jet}",       [-0.5,0.,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.5,0.6,0.7,0.8,0.9,1.0]],
    "deepJet_jet1": ["DeepJet_{4th jet}",       [-0.5,0.,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.5,0.6,0.7,0.8,0.9,1.0]],
    "deepJet_jet2": ["DeepJet_{2nd jet}",       [-0.5,0.,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.5,0.6,0.7,0.8,0.9,1.0]],
    "deepJet_jet3": ["DeepJet_{3rd jet}",       [-0.5,0.,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.5,0.6,0.7,0.8,0.9,1.0]],
    "deepJet_jet4": ["DeepJet_{4th jet}",       [-0.5,0.,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.5,0.6,0.7,0.8,0.9,1.0]],
    "n_ev":         ["Total Rate",              [0.,1.]],
}

variables=[
    "ht_gen","ht_reco","ht_l1",
    "m4j_gen","m4j_reco","m4j_l1",
    "m2j_gen","m2j_reco","m2j_l1",
    "pt1_gen","pt1_reco","pt1_l1",
    "pt2_gen","pt2_reco","pt2_l1",
    "pt3_gen","pt3_reco","pt3_l1",
    "pt4_gen","pt4_reco","pt4_l1",
    "deepCSV_jet1","deepCSV_jet2","deepCSV_jet3","deepCSV_jet4",
    "deepJet_jet1","deepJet_jet2","deepJet_jet3","deepJet_jet4",
]
xLeg = 0.6
yLeg = 0.8

if o.doShapes:
    for var in variables:
        hGen = getHist(inFileMCSignal, "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4", var, binning=2 ,norm=False, color=ROOT.kGreen+1)
        hL1 = getHist(inFileMCSignal, "L1_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4", var, binning=2 ,norm=False, color=ROOT.kBlack)
        hL1plushloose = getHist(inFileMCSignal, "L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30", var, binning=2 ,norm=False, color=ROOT.kBlue+1)
        hL1plush = getHist(inFileMCSignal, "L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", var, binning=2 ,norm=False, color=ROOT.kRed+1)
        hL1plushlooseWithTag = getHist(inFileMCSignal, "L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4", var, binning=2 ,norm=False, color=ROOT.kBlue+1)
        hL1plushWithTag = getHist(inFileMCSignal, "L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4", var, binning=2 ,norm=False, color=ROOT.kRed+1)
        drawComp("Shapes_HT4Jet_"+var,
            [
                (hGen,"GEN",ROOT.kGreen+1),
                (hL1,"L1T",ROOT.kBlack),
                (hL1plush,"L1T+HLT Run 2",ROOT.kBlue+1),
                (hL1plushloose,"L1T+HLT b-tag only",ROOT.kRed+1),
                (hL1plushWithTag,"L1T+HLT Run 2 with BTag",ROOT.kBlue+1, 24),
                (hL1plushlooseWithTag,"L1T+HLT b-tag only with BTag",ROOT.kRed+1, 24),
            ]
            ,xMax=1,xMin = -0.2
            ,yTitle="a.u.",xTitle=xTitlesAndBinnings[var][0], outDir=o.outDir,
            yMax=2700,
            setLogy=0,
            yLeg=yLeg,xLeg=xLeg-0.1
            ,cmsText=o.cmsText, lumiText=o.lumiText
            ,drawOpt="histe"
            ,otherText=["PU200"]
            ,xStartOther=1600,yStartOther=2700
            # ,xLumiStart=0.75
            # xMax=eff_Matched.GetXaxis().GetXmax(),
            # xMin=eff_Matched.GetXaxis().GetXmin()
        )

    for var in variables:
        hGen = getHist(inFileMCRates, "GEN_Dijet", var, binning=2 ,norm=False, color=ROOT.kGreen+1)
        # hL1 = getHist(inFileMCRates, "rate_L1_SingleJet50Hz", var, binning=2 ,norm=False, color=ROOT.kBlack)
        hL1plusHLTSingle = getHist(inFileMCRates, "rate_L1PlusRECO_SingleJet50Hz", var, binning=2 ,norm=False, color=ROOT.kBlue+1)
        hL1plushHLTDoubleBTag = getHist(inFileMCRates, "rate_L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6_DeepCSVCuts", var, binning=2 ,norm=False, color=ROOT.kRed+1)
        # HLTSingle = getHist(inFileMCRates, "rate_RECO_SingleJet50Hz", var, binning=1 ,norm=False, color=ROOT.kBlue+1)
        HLTDoubleBTag = getHist(inFileMCRates, "rate_L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6", var, binning=2 ,norm=False, color=ROOT.kRed+1)
        drawComp("Shapes_Double50Hz_"+var,
            [
                (hGen,"GEN",ROOT.kGreen+1),
                # (hL1,"L1T",ROOT.kBlack),
                (hL1plusHLTSingle,"L1T+HLT Single Jet",ROOT.kBlue+1),
                (hL1plushHLTDoubleBTag,"L1T+HLT DiJet+BTags",ROOT.kRed+1),
                # (HLTSingle,"HLT Run 2 with BTag",ROOT.kBlue+1, 24),
                (HLTDoubleBTag,"L1T+HLT DiJet",ROOT.kRed+1, 24),
            ]
            ,xMax=1,xMin = -0.2
            ,yTitle="a.u.",xTitle=xTitlesAndBinnings[var][0], outDir=o.outDir,
            yMax=1000000000,
            setLogy=1,
            yLeg=yLeg+0.12,xLeg=xLeg-0.1
            ,cmsText=o.cmsText, lumiText=o.lumiText
            ,drawOpt="histe"
            # xMax=eff_Matched.GetXaxis().GetXmax(),
            # xMin=eff_Matched.GetXaxis().GetXmin()
        )
    for var in variables:
        hGen = getHist(inFileMCRates, "noFilter_PFDeepCSVPuppi", var, binning=2 ,norm=False, color=ROOT.kGreen+1)
        # hL1 = getHist(inFileMCRates, "rate_L1_SingleJet50Hz", var, binning=2 ,norm=False, color=ROOT.kBlack)
        hL1plusHLTSingle = getHist(inFileMCRates, "rate_L1PlusRECO_SingleJet75Hz", var, binning=2 ,norm=False, color=ROOT.kBlue+1)
        hL1plushHLTDoubleBTag = getHist(inFileMCRates, "rate_L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_DeepCSVCuts", var, binning=2 ,norm=False, color=ROOT.kRed+1)
        # HLTSingle = getHist(inFileMCRates, "rate_RECO_SingleJet50Hz", var, binning=1 ,norm=False, color=ROOT.kBlue+1)
        HLTDoubleBTag = getHist(inFileMCRates, "rate_L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", var, binning=2 ,norm=False, color=ROOT.kRed+1)
        drawComp("Shapes_Double75Hz_"+var,
            [
                (hGen,"GEN",ROOT.kGreen+1),
                # (hL1,"L1T",ROOT.kBlack),
                (hL1plusHLTSingle,"L1T+HLT Single Jet",ROOT.kBlue+1),
                (hL1plushHLTDoubleBTag,"L1T+HLT DiJet+BTags",ROOT.kRed+1),
                # (HLTSingle,"HLT Run 2 with BTag",ROOT.kBlue+1, 24),
                (HLTDoubleBTag,"L1T+HLT DiJet",ROOT.kRed+1, 24),
            ]
            ,xMax=1,xMin = -0.2
            ,yTitle="a.u.",xTitle=xTitlesAndBinnings[var][0], outDir=o.outDir,
            yMax=1000000000,
            setLogy=1,
            yLeg=yLeg+0.12,xLeg=xLeg-0.1
            ,cmsText=o.cmsText, lumiText=o.lumiText
            ,drawOpt="histe"
            # xMax=eff_Matched.GetXaxis().GetXmax(),
            # xMin=eff_Matched.GetXaxis().GetXmin()
        )



if o.doHHEff:

    def calcEff(var,dirs,inFile,binning,bayesRatio=1,histForXBarycenterCalc=None,val0=0.,val1=1000.):
        if not isinstance(var,list) and isinstance(dirs,list):
            numHist = getHist(inFile,dirs[0],          var,binning,color=ROOT.kBlue)
            denHist = getHist(inFile,dirs[1],          var,binning,color=ROOT.kBlue)
        elif isinstance(var,list) and not isinstance(dirs,list):
            numHist = getHist(inFile,dirs,          var[0],binning,color=ROOT.kBlue)
            denHist = getHist(inFile,dirs,          var[1],binning,color=ROOT.kBlue)
        else:
            print "ERROR",var,dirs


        bin0 = numHist.FindFixBin(val0)
        bin1 = numHist.FindFixBin(val1)
        if(val0>-1 and val1>-1):
            passed = int(numHist.Integral(bin0, bin1))
            total = int(denHist.Integral(bin0, bin1))
        else:
            passed = int(numHist.Integral())
            total = int(denHist.Integral())
        e = 1. * passed / total
        if passed <= total:
            conf = ROOT.TEfficiency().GetConfidenceLevel()
            e_up = ROOT.TEfficiency.ClopperPearson(total, passed, conf, True)
            e_dn = ROOT.TEfficiency.ClopperPearson(total, passed, conf, False)

        print dirs[0]
        return np.round(e*100.,2), np.round((e-e_up)*100.,2), np.round((e-e_dn)*100.,2)

    # eff_Reco_HT330_ht_gen = makeEff("m4j_gen" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=4)
    eff_L1PlusReco_HT330_ht_gen = makeEff("m4j_gen" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=5)
    eff_L1PlusReco_HT330_ht_gen140 = makeEff("m4j_gen" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=5)
    # eff_Reco_HT100_ht_gen = makeEff("m4j_gen" , ["RECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=4)
    eff_L1PlusReco_HT100_ht_gen = makeEff("m4j_gen" , ["L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=5)
    eff_L1PlusReco_HT100_ht_gen140 = makeEff("m4j_gen" , ["L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=5)
    eff_L1_HT330_ht_gen = makeEff("m4j_gen" , ["L1_PFHT330PT30", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=4)

    drawComp("Eff_HTAll_"+"m4j_gen",
        # [(eff_Reco_HT330_ht_gen,"HLT Run 2",ROOT.kBlue+1,24),
        [(eff_L1PlusReco_HT330_ht_gen,"L1T+HLT Run 2 (PU 200)",ROOT.kBlue+1),
        (eff_L1PlusReco_HT330_ht_gen140,"L1T+HLT Run 2 (PU 140)",ROOT.kBlue+1,26),
        # (eff_Reco_HT100_ht_gen,"HLT  Phase 2",ROOT.kBlue+1),
        (eff_L1PlusReco_HT100_ht_gen,"L1T+HLT b-tag only (PU 200)",ROOT.kRed+1),
        (eff_L1PlusReco_HT100_ht_gen140,"L1T+HLT b-tag only (PU 140)",ROOT.kRed+1,26),
        (eff_L1_HT330_ht_gen,"L1T",ROOT.kBlack),
        ]
        ,xMin = 0., xMax = 1500
        ,yMax=1.2
        # ,otherText=["PU140-PU200"]
        ,yTitle="L1T+HLT Efficiency",xTitle=xTitlesAndBinnings["m4j_gen"][0], outDir=o.outDir,
        xStartOther=0.7*1500, yStartOther=1.1, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.6,xLeg = 0.55)

    print "0-350"
    print calcEff("m4j_gen" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=1, val0=0., val1=350.)
    print calcEff("m4j_gen" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=2, val0=0., val1=350.)
    print calcEff("m4j_gen" , ["RECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=2, val0=0., val1=350.)
    print calcEff("m4j_gen" , ["L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=2, val0=0., val1=350.)
    print calcEff("m4j_gen" , ["L1_PFHT330PT30", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=2, val0=0., val1=350.)
    print "0-inf"
    print calcEff("m4j_gen" , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=1, val0=-1., val1=-1.)
    print calcEff("m4j_gen" , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=2, val0=0., val1=-1.)
    print calcEff("m4j_gen" , ["RECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=2, val0=-1., val1=-1.)
    print calcEff("m4j_gen" , ["L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=2, val0=-1., val1=-1.)
    print calcEff("m4j_gen" , ["L1_PFHT330PT30", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=2, val0=-1., val1=-1.)

if o.doTurnOn:
    for var in variables:
        eff_Reco_HT330_ht_gen = makeEff(var , ["RECO_PFHT330PT30", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=1)
        eff_L1PlusReco_HT330_ht_gen = makeEff(var , ["L1PlusRECO_PFHT330PT30", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=1)
        eff_L1_HT330_ht_gen = makeEff(var , ["L1_PFHT330PT30", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=1)

        drawComp("Eff_HT330_"+var,
                 [(eff_Reco_HT330_ht_gen,"HLT",ROOT.kBlack),
                  (eff_L1PlusReco_HT330_ht_gen,"L1T+HLT",ROOT.kRed+1),
                  (eff_L1_HT330_ht_gen,"L1T",ROOT.kBlue+1),
                  ]
                  # ,xMin = 0., xMax = 1000
                  ,yMax=1.2
                 ,yTitle="Efficiency",xTitle=xTitlesAndBinnings[var][0], otherText=[""], outDir=o.outDir,
                 xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8,xLeg = 0.6)

    for var in variables:
        eff_Reco_HT330_4JetPt_ht_gen = makeEff(var , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=1)
        eff_L1PlusReco_HT330_4JetPt_ht_gen = makeEff(var , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=1)
        eff_L1_HT330_4JetPt_ht_gen = makeEff(var , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=1)

        drawComp("Eff_HT330_4JetPt_"+var,
                 [(eff_Reco_HT330_4JetPt_ht_gen,"HLT",ROOT.kBlack),
                  (eff_L1PlusReco_HT330_4JetPt_ht_gen,"L1T+HLT",ROOT.kRed+1),
                  (eff_L1_HT330_4JetPt_ht_gen,"L1T",ROOT.kBlue+1),
                  ]
                  # ,xMin = 0., xMax = 1000
                  ,yMax=1.2
                 ,yTitle="Efficiency",xTitle=xTitlesAndBinnings[var][0], otherText=[""], outDir=o.outDir,
                 xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8,xLeg = 0.6)

    for var in variables:
        eff_Reco_HT330_4JetPt_3Tag_ht_gen = makeEff(var , ["RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=1)
        eff_L1PlusReco_HT330_4JetPt_3Tag_ht_gen = makeEff(var , ["L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=1)
        eff_L1_HT330_4JetPt_3Tag_ht_gen = makeEff(var , ["L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4", "GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=1)

        drawComp("Eff_HT330_4JetPt_3tag_"+var,
                 [(eff_Reco_HT330_4JetPt_3Tag_ht_gen,"HLT",ROOT.kBlack),
                  (eff_L1PlusReco_HT330_4JetPt_3Tag_ht_gen,"L1T+HLT",ROOT.kRed+1),
                  (eff_L1_HT330_4JetPt_3Tag_ht_gen,"L1T",ROOT.kBlue+1),
                  ]
                  # ,xMin = 0., xMax = 1000
                  ,yMax=1.2
                 ,yTitle="Efficiency",xTitle=xTitlesAndBinnings[var][0], otherText=[""], outDir=o.outDir,
                 xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8,xLeg = 0.6)

    # loose edition

    for var in variables:
        eff_Reco_HT100_ht_gen = makeEff(var , ["RECO_PFHT100PT30", "GEN_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=1)
        eff_L1PlusReco_HT100_ht_gen = makeEff(var , ["L1PlusRECO_PFHT100PT30", "GEN_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=1)
        eff_L1_HT100_ht_gen = makeEff(var , ["L1_PFHT100PT30", "GEN_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=1)

        drawComp("Eff_HT100_"+var,
                 [(eff_Reco_HT100_ht_gen,"HLT",ROOT.kBlack),
                  (eff_L1PlusReco_HT100_ht_gen,"L1T+HLT",ROOT.kRed+1),
                  (eff_L1_HT100_ht_gen,"L1T",ROOT.kBlue+1),
                  ]
                  # ,xMin = 0., xMax = 1000
                  ,yMax=1.2
                 ,yTitle="Efficiency",xTitle=xTitlesAndBinnings[var][0], otherText=[""], outDir=o.outDir,
                 xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8,xLeg = 0.6)

    for var in variables:
        eff_Reco_HT100_4JetPt_ht_gen = makeEff(var , ["RECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30", "GEN_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=1)
        eff_L1PlusReco_HT100_4JetPt_ht_gen = makeEff(var , ["L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30", "GEN_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=1)
        eff_L1_HT100_4JetPt_ht_gen = makeEff(var , ["L1_PFHT100PT30_QuadPFPuppiJet_30_30_30_30", "GEN_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=1)

        drawComp("Eff_HT100_4JetPt_"+var,
                 [(eff_Reco_HT100_4JetPt_ht_gen,"HLT",ROOT.kBlack),
                  (eff_L1PlusReco_HT100_4JetPt_ht_gen,"L1T+HLT",ROOT.kRed+1),
                  (eff_L1_HT100_4JetPt_ht_gen,"L1T",ROOT.kBlue+1),
                  ]
                  # ,xMin = 0., xMax = 1000
                  ,yMax=1.2
                 ,yTitle="Efficiency",xTitle=xTitlesAndBinnings[var][0], otherText=[""], outDir=o.outDir,
                 xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8,xLeg = 0.6)

    for var in variables:
        eff_Reco_HT100_4JetPt_3Tag_ht_gen = makeEff(var , ["RECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4", "GEN_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=1)
        eff_L1PlusReco_HT100_4JetPt_3Tag_ht_gen = makeEff(var , ["L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4", "GEN_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=1)
        eff_L1_HT100_4JetPt_3Tag_ht_gen = makeEff(var , ["L1_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4", "GEN_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4"], inFileMCSignal, binning=1)

        drawComp("Eff_HT100_4JetPt_3tag_"+var,
                 [(eff_Reco_HT100_4JetPt_3Tag_ht_gen,"HLT",ROOT.kBlack),
                  (eff_L1PlusReco_HT100_4JetPt_3Tag_ht_gen,"L1T+HLT",ROOT.kRed+1),
                  (eff_L1_HT100_4JetPt_3Tag_ht_gen,"L1T",ROOT.kBlue+1),
                  ]
                  ,yMax=1.2
                  # ,xMin = 0., xMax = 1000
                 ,yTitle="Efficiency",xTitle=xTitlesAndBinnings[var][0], otherText=[""], outDir=o.outDir,
                 xStartOther=0.3, yStartOther=0.85, cmsText=o.cmsText, lumiText=o.lumiText, yLeg=0.8,xLeg = 0.6)



def getInverseTurnOnTDR(name,var,dir1, dir2,inFile,binning):
    histSig = getHist(inFile,dir1,var,binning)
    histCut = getHist(inFile,dir2,var,binning)

    histEff = histSig.Clone(name)
    histEff.Reset()

    totalIntegralSig = histSig.Integral()
    totalIntegralCut = histCut.Integral()

    nBinsX = histSig.GetNbinsX()

    for iBin in range(nBinsX+1):
        # numberTotal = histSig.Integral(-1,iBin)
        # numberFailed = histSig.Integral(-1,iBin)
        numberFailed = histCut.Integral(-1,iBin)
        # numberRemain= histCut.Integral(iBin,nBinsX+1)

        # fractionFailed = (totalIntegralSig - numberFailed)/totalIntegralSig
        # error = math.sqrt(fractionFailed*(1-fractionFailed)/totalIntegralSig)
        # fractionFailed = (numberRemain)/totalIntegralSig
        fractionFailed = (totalIntegralCut-numberFailed)/totalIntegralSig
        error = math.sqrt(fractionFailed*(1-fractionFailed)/totalIntegralSig)
        # error = 0.

        histEff.SetBinContent(iBin, fractionFailed)
        histEff.SetBinError(iBin, error)

    return histEff


def makeInverseTurnOnTDR(name,var,dirs,inFile,binning, otherText, outDir,cmsText="", lumiText="",xTitle=""):
    histTight  = getInverseTurnOnTDR(name,var,dirs[0],dirs[2], inFile,binning)
    histTight.SetMarkerColor(ROOT.kRed+1)
    histTight.SetLineColor(ROOT.kRed+1)
    #
    histMedium = getInverseTurnOnTDR(name,var,dirs[0],dirs[3],inFile,binning)
    histMedium.SetMarkerColor(ROOT.kBlack)
    histMedium.SetLineColor(ROOT.kBlack)

    histLoose  = getInverseTurnOnTDR(name,var,dirs[0],dirs[1], inFile,binning)
    histLoose.SetMarkerColor(ROOT.kBlue+1)
    histLoose.SetLineColor(ROOT.kBlue+1)

    #hist = getHist(inFile,dir,var,binning)

    # histLoose.GetXaxis().SetTitle("Online DeepCSV Cut Value")
    histLoose.GetXaxis().SetTitle(xTitle)
    histLoose.GetYaxis().SetTitle("Relative Efficiency")
    # histLoose.GetXaxis().SetRangeUser(0,1)
    histLoose.GetYaxis().SetRangeUser(0,1.2)

    can = ROOT.TCanvas(name, name,600,500)#x, 700, 500)
    can.cd()

    histLoose .Draw()

    histMedium.Draw("same")
    histTight .Draw("same")
    histLoose .Draw("axis, same")
    #
    #  CMS Text
    #
    cmsLines = getCMSText(xStart=0.2,yStart=0.875,subtext=cmsText,lumiText=lumiText,xLumiStart=0.7,yLumiStart=0.96)
    for cmsl in cmsLines:
        cmsl.Draw("same")

    # xpos = 0.2
    xpos = 0.6
    # ypos = 0.3
    ypos = 0.6
    xwidth = 0.2
    ywidth = 0.2

    leg = ROOT.TLegend(xpos, ypos, xpos+xwidth, ypos+ywidth)
    # leg.AddEntry(histLoose, "GEN","PEL")
    leg.AddEntry(histLoose, "HLT","PEL")
    # leg.AddEntry(histMedium,"HLT","PEL")
    leg.AddEntry(histMedium,"L1T","PEL")
    leg.AddEntry(histTight, "L1T+HLT","PEL")
    #leg.AddEntry(,histInfo[1][1] ,"F")

    leg.Draw("same")

    if otherText:
        xStartOther=0.3*histLoose.GetBinCenter(histLoose.GetNbinsX())
        yStartOther=1.1
        textsize=0.045
        otherLabel = ROOT.TLatex(xStartOther, yStartOther, '#scale['+str(0.7)+']{'+otherText+'}')
        otherLabel.Draw("same")

    can.SaveAs(outDir+"/"+name+".pdf")
    can.SaveAs(outDir+"/"+name+".png")

if o.doEffs:

    for jet in ["jet1","jet2","jet3","jet4"]:
        makeInverseTurnOnTDR("HT330_4JetPt_EffwrtGEN_"+jet,   "deepCSV_"+jet,["GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4","RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40","L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40","L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40"], inFileMCSignal,  binning=1,
            otherText="Online Puppi DeepCSV Eff wrt GEN", outDir=o.outDir,cmsText=o.cmsText,lumiText=o.lumiText,xTitle=xTitlesAndBinnings["deepCSV_"+jet][0])
        makeInverseTurnOnTDR("HT100_4JetPt_EffwrtGEN_"+jet,   "deepCSV_"+jet,["GEN_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4","RECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30","L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30","L1_PFHT100PT30_QuadPFPuppiJet_30_30_30_30"], inFileMCSignal,  binning=1,
            otherText="Online Puppi DeepCSV Eff wrt GEN", outDir=o.outDir,cmsText=o.cmsText,lumiText=o.lumiText,xTitle=xTitlesAndBinnings["deepCSV_"+jet][0])
    for var in ["pt1_reco","pt2_reco","pt3_reco","pt4_reco"]:
        makeInverseTurnOnTDR("HT330_EffwrtGEN_"+var,   var,["GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4","RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40","L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40","L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40"], inFileMCSignal,  binning=1,
            otherText="Online Puppi DeepCSV Eff wrt GEN", outDir=o.outDir,cmsText=o.cmsText,lumiText=o.lumiText,xTitle=xTitlesAndBinnings[var][0])
        makeInverseTurnOnTDR("HT100_EffwrtGEN_"+var,   var,["GEN_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4","RECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30","L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30","L1_PFHT100PT30_QuadPFPuppiJet_30_30_30_30"], inFileMCSignal,  binning=1,
            otherText="Online Puppi DeepCSV Eff wrt GEN", outDir=o.outDir,cmsText=o.cmsText,lumiText=o.lumiText,xTitle=xTitlesAndBinnings[var][0])

    makeInverseTurnOnTDR("HT330_EffwrtGEN_"+"ht_reco",   "ht_reco",
        ["GEN_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_TriplePFPuppiBTagDeepCSV0p5_2p4","RECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40","L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40","L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40"],
        inFileMCSignal,  binning=1,
        otherText="Online Puppi DeepCSV Eff wrt GEN", outDir=o.outDir,cmsText=o.cmsText,lumiText=o.lumiText,xTitle=xTitlesAndBinnings["ht_reco"][0])
    makeInverseTurnOnTDR("HT100_EffwrtGEN_"+"ht_reco",   "ht_reco",
        ["GEN_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_TriplePFPuppiBTagDeepCSV0p5_2p4","RECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30","L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30","L1_PFHT100PT30_QuadPFPuppiJet_30_30_30_30"],
        inFileMCSignal,  binning=1,
        otherText="Online Puppi DeepCSV Eff wrt GEN", outDir=o.outDir,cmsText=o.cmsText,lumiText=o.lumiText,xTitle=xTitlesAndBinnings["ht_reco"][0])


    for jet in ["jet1","jet2"]:
        makeInverseTurnOnTDR("Double128_EffwrtGEN_"+jet,   "deepCSV_"+jet,
        ["GEN_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4","RECO_DoublePFPuppiJets128MaxDeta1p6","L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6","L1_DoublePFPuppiJets128MaxDeta1p6"],
        inFileMCSignal,  binning=1,
            otherText="Online Puppi DeepCSV Eff wrt GEN", outDir=o.outDir,cmsText=o.cmsText,lumiText=o.lumiText,xTitle=xTitlesAndBinnings["deepCSV_"+jet][0])
        makeInverseTurnOnTDR("Double30_EffwrtGEN_"+jet,   "deepCSV_"+jet,
        ["GEN_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4","RECO_DoublePFPuppiJets30MaxDeta1p6","L1PlusRECO_DoublePFPuppiJets30MaxDeta1p6","L1_DoublePFPuppiJets30MaxDeta1p6"]
        , inFileMCSignal,  binning=1,
            otherText="Online Puppi DeepCSV Eff wrt GEN", outDir=o.outDir,cmsText=o.cmsText,lumiText=o.lumiText,xTitle=xTitlesAndBinnings["deepCSV_"+jet][0])
    for var in ["pt1_reco","pt2_reco"]:
        makeInverseTurnOnTDR("Double128_EffwrtGEN_"+var,   var,["GEN_DoublePFPuppiJets128MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4","RECO_DoublePFPuppiJets128MaxDeta1p6","L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6","L1_DoublePFPuppiJets128MaxDeta1p6"], inFileMCSignal,  binning=1,
            otherText="Online Puppi DeepCSV Eff wrt GEN", outDir=o.outDir,cmsText=o.cmsText,lumiText=o.lumiText,xTitle=xTitlesAndBinnings[var][0])
        makeInverseTurnOnTDR("Double30_EffwrtGEN_"+var,   var,["GEN_DoublePFPuppiJets30MaxDeta1p6_DoublePFPuppiBTagDeepCSV_p5_2p4","RECO_DoublePFPuppiJets30MaxDeta1p6","L1PlusRECO_DoublePFPuppiJets30MaxDeta1p6","L1_DoublePFPuppiJets30MaxDeta1p6"], inFileMCSignal,  binning=1,
            otherText="Online Puppi DeepCSV Eff wrt GEN", outDir=o.outDir,cmsText=o.cmsText,lumiText=o.lumiText,xTitle=xTitlesAndBinnings[var][0])



# xLeg = 0.6
# yLeg = 0.8
def convertToRate(histo):
    nbins = histo.GetNbinsX()
    outHisto = histo.Clone()
    for binx in range(1,nbins+1):
        err = ROOT.Double(0)
        int = histo.IntegralAndError(binx,nbins+1,err)
        outHisto.SetBinContent(binx,int)
        outHisto.SetBinError(binx,err)
        # outHisto.SetBinContent(binx,histo.Integral(binx,nbins+1))
        # outHisto.SetBinError(binx,histo.Integral(binx,nbins+1))
    return outHisto
def printCutValueForRate(histo, rateMax):
    nbins = histo.GetNbinsX()
    cutted = False
    for binx in range(1,nbins+1):
        if histo.GetBinContent(binx)<rateMax:
            if(cutted==False):
                print histo.GetBinLowEdge(binx), histo.GetBinContent(binx),"+-",histo.GetBinError(binx)
                cutted = True


if o.doRates:

    vars=["n_ev","deepCSV_jet1","deepCSV_jet2","deepCSV_jet3"]
    for var in vars:
        # hL1 = getHist(inFileMCRates, "rate_L1_DoublePFPuppiJets128MaxDeta1p6", var, binning=2 ,norm=False, color=ROOT.kBlack)
        hL1plushloose = getHist(inFileMCRates, "rate_L1PlusRECO_DoublePFPuppiJets30MaxDeta1p6", var, binning=1 ,norm=False, color=ROOT.kBlue+1)
        hL1plush = getHist(inFileMCRates, "rate_L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6", var, binning=1 ,norm=False, color=ROOT.kRed+1)
        hL1plushloose140 = getHist(inFileMCRates, "rate_L1PlusRECO_DoublePFPuppiJets30MaxDeta1p6", var, binning=1 ,norm=False, color=ROOT.kBlue+1)
        hL1plush140 = getHist(inFileMCRates, "rate_L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6", var, binning=1 ,norm=False, color=ROOT.kRed+1)
        # hL1 = convertToRate(hL1)
        hL1plushloose = convertToRate(hL1plushloose)
        hL1plush = convertToRate(hL1plush)
        hL1plushloose140 = convertToRate(hL1plushloose140)
        hL1plush140 = convertToRate(hL1plush140)
        # print ("Cut for rate_L1PlusRECO_DoublePFPuppiJets30MaxDeta1p6")
        # printCutValueForRate(hL1plushloose,50)
        print ("Cut for rate_L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6")
        printCutValueForRate(hL1plush,50)
        printCutValueForRate(hL1plush,75)
        drawComp("Rates_Double_"+var,
            [
                # (hL1,"L1T",ROOT.kBlack),
                (hL1plush,   "b-tag only (PU200)",ROOT.kBlue+1),
                # (hL1plushloose,"L1T+HLT Phase 2 (PU200)",ROOT.kRed+1),
                (hL1plush140,"Run 2-like (PU140)",ROOT.kBlue+1,26),
                # (hL1plushloose140,"L1T+HLT Phase 2 (PU140)",ROOT.kRed+1,26),
            ]
            ,xMax=1.0,xMin = -0.1 if not "DeepCSV" in var else 0.
            ,yTitle="L1T+HLT Rate [Hz]",xTitle=xTitlesAndBinnings[var][0], outDir=o.outDir,
            yMax=1000000,
            setLogy=1,
            yLeg=yLeg,xLeg=xLeg-0.1
            ,cmsText=o.cmsText, lumiText=o.lumiText
            # ,otherText=["PU200"]
            ,xStartOther=0.5
            ,yStartOther=0.1*1000000
            # xMax=eff_Matched.GetXaxis().GetXmax(),
            # xMin=eff_Matched.GetXaxis().GetXmin()
        )

    for var in vars:
        # hL1 = getHist(inFileMCRates, "rate_L1_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", var, binning=1 ,norm=False, color=ROOT.kBlack)
        hL1plushloose = getHist(inFileMCRates, "rate_L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30", var, binning=1 ,norm=False, color=ROOT.kBlue+1)
        hL1plush = getHist(inFileMCRates, "rate_L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", var, binning=1 ,norm=False, color=ROOT.kRed+1)
        hL1plushloose140 = getHist(inFileMCRates, "rate_L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30", var, binning=1 ,norm=False, color=ROOT.kBlue+1)
        hL1plush140 = getHist(inFileMCRates, "rate_L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40", var, binning=1 ,norm=False, color=ROOT.kRed+1)
        # hL1 = convertToRate(hL1)
        hL1plushloose = convertToRate(hL1plushloose)
        hL1plush = convertToRate(hL1plush)
        hL1plushloose140 = convertToRate(hL1plushloose140)
        hL1plush140 = convertToRate(hL1plush140)
        print ("Cut for rate_L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40")
        printCutValueForRate(hL1plush,50)
        printCutValueForRate(hL1plush,75)
        print ("Cut for rate_L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30")
        printCutValueForRate(hL1plushloose,50)
        printCutValueForRate(hL1plushloose,75)
        drawComp("Rates_HT4Jet_"+var,
            [
                # (hL1,"L1T",ROOT.kBlack),
                (hL1plush,        "Run 2-like (PU200)",ROOT.kBlue+1),
                (hL1plushloose,   "b-tag only (PU200)",ROOT.kRed+1),
                (hL1plush140,     "Run 2-like (PU140)",ROOT.kBlue+1,26),
                (hL1plushloose140,"b-tag only (PU140)",ROOT.kRed+1,26),
            ]
            ,xMax=1.,xMin = -0.1
            ,yTitle="L1T+HLT Rate [Hz]",xTitle=xTitlesAndBinnings[var][0], outDir=o.outDir,
            yMax=100000,
            setLogy=1,
            yLeg=yLeg,xLeg=xLeg-0.1
            ,cmsText=o.cmsText, lumiText=o.lumiText
            # ,otherText=["PU200"]
            ,xStartOther=0.5
            ,yStartOther=100000*0.1
            ,drawOpt="ehist"
            # xMax=eff_Matched.GetXaxis().GetXmax(),
            # xMin=eff_Matched.GetXaxis().GetXmin()
        )



    # printing rates after cuts

    def printRate(foldername,histoname):
        histo =  getHist(inFileMCRates, foldername, histoname, binning=1 ,norm=False, color=ROOT.kBlack)
        c = histo.GetBinContent(1)
        e = histo.GetBinError(1)
        print "Rate for",foldername,"=",c,"+-",e

    printRate("rate_L1PlusRECO_DoublePFPuppiJets128MaxDeta1p6_DeepCSVCuts","n_ev")
    # printRate("rate_L1PlusRECO_DoublePFPuppiJets30MaxDeta1p6_DeepCSVCuts","n_ev")
    printRate("rate_L1PlusRECO_PFHT330PT30_QuadPFPuppiJet_75_60_45_40_DeepCSVCuts","n_ev")
    printRate("rate_L1PlusRECO_PFHT100PT30_QuadPFPuppiJet_30_30_30_30_DeepCSVCuts","n_ev")
