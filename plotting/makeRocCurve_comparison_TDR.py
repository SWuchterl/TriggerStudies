import ROOT


ROOT.gROOT.SetBatch(True)

import ROOTHelp.FancyROOTStyle

from optparse import OptionParser
p = OptionParser()
p.add_option('--input',  type = 'string', default = "outBTag.FTKBtagging.ttbar.mwt2.All.root", dest = 'inFile', help = 'intput File' )
p.add_option('--input2',  type = 'string', default = "outBTag.FTKBtagging.ttbar.mwt2.All.root", dest = 'inFile2', help = 'intput File 2' )
p.add_option('--label',  type = 'string', default = "input", dest = 'label1', help = 'intput File' )
p.add_option('--label2',  type = 'string', default = "input2", dest = 'label2', help = 'intput File 2' )
p.add_option('--output', type = 'string', default = "makeRocCurves", dest = 'outDir', help = 'output dir' )
p.add_option('--cmsText', type = 'string', default = "",  help = '' )
(o,a) = p.parse_args()

# inFile  = ROOT.TFile(o.inFile,  "READ")
# inFile2  = ROOT.TFile(o.inFile2,  "READ")

# inFile =
# inFile2 =
# inFile3 =
# inFile4 =
# inFile5 =
# inFile6 =

label1 = o.label1
label2 =  o.label2


import os
if not os.path.exists(o.outDir):
    os.makedirs(o.outDir)

from rocCurveUtils     import makeRoc
from JetLevelPlotUtils import getCMSText, getText



# def getWorkingPoint(var, bkg, sig, dir, varNorm, inFile=None):
#     sigHist = inFile.Get(dir+"_"+sig+"/"+var)
#     bkgHist = inFile.Get(dir+"_"+bkg+"/"+var)
#
#     sigNormHist = inFile.Get(dir+"_"+sig+"/"+varNorm)
#     bkgNormHist = inFile.Get(dir+"_"+bkg+"/"+varNorm)
#
#
#     rocPlot = makeRoc(sigHist, sigNormHist, bkgHist, bkgNormHist,doErr=False,bkgMode="Rej")
#
#     nbins      = sigHist.GetNbinsX()
#
#     thisSig    = sigHist    .Integral(0,nbins+1)
#     thisSigDen = sigNormHist.Integral(0,nbins+1)
#     if not thisSigDen: thisSigDen = 1
#     sigEff = float(thisSig) / float(thisSigDen)
#
#     thisBkg    = bkgHist    .Integral(0,nbins+1)
#     thisBkgDen = bkgNormHist.Integral(0,nbins+1)
#     if not thisBkgDen: thisBkgDen = 1
#     bkgEff =  float(thisBkg) / float(thisBkgDen)
#     if bkgEff: bkgRej = 1./bkgEff
#     else:      bkgRej = 1
#
#     print bkgRej
#
#     return (sigEff, bkgRej)

# def getWorkingPoint2(var, bkg, sig, dir, varNorm):
#     sigHist = inFile2.Get(dir+"_"+sig+"/"+var)
#     bkgHist = inFile2.Get(dir+"_"+bkg+"/"+var)
#
#     sigNormHist = inFile2.Get(dir+"_"+sig+"/"+varNorm)
#     bkgNormHist = inFile2.Get(dir+"_"+bkg+"/"+varNorm)
#
#
#     rocPlot = makeRoc(sigHist, sigNormHist, bkgHist, bkgNormHist,doErr=False,bkgMode="Rej")
#
#     nbins      = sigHist.GetNbinsX()
#
#     thisSig    = sigHist    .Integral(0,nbins+1)
#     thisSigDen = sigNormHist.Integral(0,nbins+1)
#     if not thisSigDen: thisSigDen = 1
#     sigEff = float(thisSig) / float(thisSigDen)
#
#     thisBkg    = bkgHist    .Integral(0,nbins+1)
#     thisBkgDen = bkgNormHist.Integral(0,nbins+1)
#     if not thisBkgDen: thisBkgDen = 1
#     bkgEff =  float(thisBkg) / float(thisBkgDen)
#     if bkgEff: bkgRej = 1./bkgEff
#     else:      bkgRej = 1
#
#     print bkgRej
#
#     return (sigEff, bkgRej)

def makeRocPlot(name, var, bkg, sig, dir, varNorm=None,debug=False, inFile=None):
    inFile  = ROOT.TFile(inFile,  "READ")
    sigHist = inFile.Get(dir+"_"+sig+"/"+var)
    bkgHist = inFile.Get(dir+"_"+bkg+"/"+var)

    if varNorm:
        sigNormHist = inFile.Get(dir+"_"+sig+"/"+varNorm)
        bkgNormHist = inFile.Get(dir+"_"+bkg+"/"+varNorm)
    else      :
        sigNormHist = sigHist
        bkgNormHist = bkgHist

    rocPlots = []
    for config in [("Rej",1,5e4),("Eff",5e-4,1)]:
        rocPlots.append(makeRoc(sigHist, sigNormHist, bkgHist, bkgNormHist,doErr=False,bkgMode=config[0],cleanNoCut=True,debug=debug))

        can = ROOT.TCanvas(name+"_"+config[0], name+"_"+config[0])
        can.cd().SetLogy(1)
        rocPlots[-1].SetLineWidth(5)
        # rocPlots[-1].SetLineWidth(2)
        rocPlots[-1].GetXaxis().SetTitle("B-Jet  Efficiency")
        rocPlots[-1].GetXaxis().SetRangeUser(0.4,1)
        if config[0] == "Rej":    yTitle ="Light Flavor Rejection"
        elif config[0] == "Eff":  yTitle ="Light Flavor Efficiency"
        rocPlots[-1].GetYaxis().SetTitle(yTitle)
        rocPlots[-1].GetYaxis().SetRangeUser(config[1],config[2])
        rocPlots[-1].Draw("AL")
        # can.SaveAs(o.outDir+"/roc_"+name+"_"+config[0]+"_"+label1+"_"+".pdf")
        # can.SaveAs(o.outDir+"/roc_"+name+"_"+config[0]+"_"+label1+"_"+".png")
    return rocPlots

# def makeRocPlot2(name, var, bkg, sig, dir, varNorm=None,debug=False):
#     sigHist = inFile2.Get(dir+"_"+sig+"/"+var)
#     bkgHist = inFile2.Get(dir+"_"+bkg+"/"+var)
#
#     if varNorm:
#         sigNormHist = inFile2.Get(dir+"_"+sig+"/"+varNorm)
#         bkgNormHist = inFile2.Get(dir+"_"+bkg+"/"+varNorm)
#     else      :
#         sigNormHist = sigHist
#         bkgNormHist = bkgHist
#
#     rocPlots = []
#     for config in [("Rej",1,5e4),("Eff",5e-4,1)]:
#         rocPlots.append(makeRoc(sigHist, sigNormHist, bkgHist, bkgNormHist,doErr=False,bkgMode=config[0],cleanNoCut=True,debug=debug))
#
#         can = ROOT.TCanvas(name+"_"+config[0], name+"_"+config[0])
#         can.cd().SetLogy(1)
#         rocPlots[-1].SetLineWidth(5)
#         rocPlots[-1].GetXaxis().SetTitle("B-Jet  Efficiency")
#         rocPlots[-1].GetXaxis().SetRangeUser(0.4,1)
#         if config[0] == "Rej":    yTitle ="Light Flavor Rejection"
#         elif config[0] == "Eff":  yTitle ="Light Flavor Efficiency"
#         rocPlots[-1].GetYaxis().SetTitle(yTitle)
#         rocPlots[-1].GetYaxis().SetRangeUser(config[1],config[2])
#         rocPlots[-1].Draw("AL")
#         can.SaveAs(o.outDir+"/roc_"+name+"_"+config[0]+"_"+label2+"_"+".pdf")
#         can.SaveAs(o.outDir+"/roc_"+name+"_"+config[0]+"_"+label2+"_"+".png")
#     return rocPlots

def plotSame(name,graphs,colors,styles, plotCaloJet=False, plotPFJet=False, plotOffJet=False,plotCSV=False,plotDeepCSV=False,plotDeepJet=False,workingPts= None,rocType=None):

    can = ROOT.TCanvas(name,name)
    can.cd().SetLogy(1)
    for gItr, g in enumerate(graphs):
        g.SetLineColor(colors[gItr])
        g.SetLineStyle(styles[gItr])
        if not gItr:
            g.Draw("AL")
        else:
            g.Draw("L")

    if not workingPts == None:
        g_wrkPts = ROOT.TGraph(len(workingPts))
        g_wrkPts.SetMarkerSize(2)
        g_wrkPts.SetMarkerColor(colors[1])
        g_wrkPts.SetMarkerStyle(34)
        for wpItr, wp in enumerate(workingPts):
            print wpItr,wp

            g_wrkPts.SetPoint(wpItr, wp[0],wp[1])

        g_wrkPts.Draw("P")

    cmsLines = getCMSText(xStart=0.2,yStart=0.875,subtext=o.cmsText,lumiText="Phase 2 (14 TeV)")
    for cmsl in cmsLines:
        cmsl.Draw("same")

    yStart = 0.75
    # xStart = 0.225
    xStart = 0.185
    if rocType == "Rej":
        xStart = 0.5
        yStart = 0.875

    if plotOffJet:
        offJetText  = getText("PUPPI Jets TRK V0",xStart=xStart,yStart=yStart,size=0.03,color=ROOT.kBlack)
        yStart = yStart - 0.04
        offJetText.Draw("same")

    if plotPFJet:
        pfJetText   = getText("HLT PUPPI Jets"+" TRK V6_{1}",xStart=xStart,yStart=yStart,size=0.03,color=ROOT.kRed+1)
        pfJetText.Draw("same")
        # yStart = yStart - 0.04
        # pfJetText3   = getText("HLT PUPPI Jets"+" TRKv6p1+TICL",xStart=xStart,yStart=yStart,size=0.03,color=ROOT.kGreen+2)
        # pfJetText3.Draw("same")
        yStart = yStart - 0.04
        pfJetText6   = getText("HLT PUPPI Jets"+" TRK V7_{2}",xStart=xStart,yStart=yStart,size=0.03,color=ROOT.kBlue+1)
        pfJetText6.Draw("same")
        # yStart = yStart - 0.04
        # pfJetText8   = getText("HLT PUPPI Jets"+" TRKv7p2+TICL",xStart=xStart,yStart=yStart,size=0.03,color=ROOT.kOrange+1)
        # pfJetText8.Draw("same")
        # yStart = yStart - 0.04
        # pfJetText7   = getText("DeepCSV solid",xStart=xStart,yStart=yStart,size=0.03,color=ROOT.kBlack)
        # pfJetText7.Draw("same")
        # yStart = yStart - 0.04
        # pfJetText8   = getText("DeepJet dotted",xStart=xStart,yStart=yStart,size=0.03,color=ROOT.kBlack)
        # pfJetText8.Draw("same")

    yStart = 0.25
    xStart = 0.6
    if rocType == "Rej":
        xStart = 0.2

    textEtaRegion = ""
    # if "_eta1" in name:
    #     textEtaRegion = " (|#eta|<1.5)"
    # if "_eta2" in name:
    #     textEtaRegion = " (1.5<|#eta|<3)"
    # if "_eta3" in name:
    #     textEtaRegion = " (|#eta|>3)"

    if plotDeepCSV:
        if plotCSV:
            deepCSVText   = getText("DeepCSV (solid)"+textEtaRegion,xStart=xStart,yStart=yStart,size=0.03,color=ROOT.kBlack)
        else:
            deepCSVText   = getText("DeepCSV (solid)"+textEtaRegion,xStart=xStart,yStart=yStart,size=0.03,color=ROOT.kBlack)
        deepCSVText.Draw("same")
        yStart = yStart - 0.04
    if plotDeepJet:

        deepJetText   = getText("DeepJet (dotted)"+textEtaRegion,xStart=xStart,yStart=yStart,size=0.03,color=ROOT.kBlack)
        deepJetText.Draw("same")
        yStart = yStart - 0.04

    if plotCSV:
        if plotDeepCSV:
            CSVText   = getText("CSV      (dashDotted)"+textEtaRegion,xStart=xStart,yStart=yStart,size=0.03,color=ROOT.kBlack)
        else:
            CSVText   = getText("CSV"+textEtaRegion,xStart=xStart,yStart=yStart,size=0.03,color=ROOT.kBlack)
        CSVText.Draw("same")

    #offJetTextDeep.Draw("same")

    can.SaveAs(o.outDir+"/roc_"+name+".pdf")
    can.SaveAs(o.outDir+"/roc_"+name+".png")

def plotSameEtaRanges(name,graphs,colors,styles, plotCaloJet=False, plotPFJet=False, plotOffJet=False,plotCSV=False,plotDeepCSV=False,plotDeepJet=False,workingPts= None,rocType=None):

    can = ROOT.TCanvas(name,name)
    can.cd().SetLogy(1)
    for gItr, g in enumerate(graphs):
        g.SetLineColor(colors[gItr])
        g.SetLineStyle(styles[gItr])
        if not gItr:
            g.Draw("AL")
        else:
            g.Draw("L")

    if not workingPts == None:
        g_wrkPts = ROOT.TGraph(len(workingPts))
        g_wrkPts.SetMarkerSize(2)
        g_wrkPts.SetMarkerColor(colors[1])
        g_wrkPts.SetMarkerStyle(34)
        for wpItr, wp in enumerate(workingPts):
            print wpItr,wp

            g_wrkPts.SetPoint(wpItr, wp[0],wp[1])

        g_wrkPts.Draw("P")

    cmsLines = getCMSText(xStart=0.2,yStart=0.875,subtext=o.cmsText,lumiText="Phase 2 (14 TeV)")
    for cmsl in cmsLines:
        cmsl.Draw("same")

    yStart = 0.75
    # xStart = 0.225
    xStart = 0.185
    if rocType == "Rej":
        xStart = 0.5
        yStart = 0.875

    if plotOffJet:
        offJetText  = getText("PUPPI Jets TRk V0 (dotted)",xStart=xStart,yStart=yStart,size=0.03,color=ROOT.kBlack)
        yStart = yStart - 0.04
        offJetText.Draw("same")

    if plotPFJet:
        pfJetText   = getText("HLT PUPPI Jets (solid)",xStart=xStart,yStart=yStart,size=0.03,color=ROOT.kBlack)
        pfJetText.Draw("same")

    yStart = 0.35
    xStart = 0.7
    if rocType == "Rej":
        xStart = 0.2

    textEtaRegion = ""
    if "_eta1" in name:
        textEtaRegion = " (|#eta|<1.5)"
    if "_eta2" in name:
        textEtaRegion = " (1.5<|#eta|<3)"
    if "_eta3" in name:
        textEtaRegion = " (|#eta|>3)"

    if plotDeepCSV:
        deepCSVText   = getText("DeepCSV (all Jets)"+textEtaRegion,xStart=xStart,yStart=yStart,size=0.03,color=ROOT.kBlack)
        deepCSVText.Draw("same")
        yStart = yStart - 0.04
        deepJetText   = getText("DeepCSV (|#eta|<1.5)"+textEtaRegion,xStart=xStart,yStart=yStart,size=0.03,color=ROOT.kBlue+1)
        deepJetText.Draw("same")
        yStart = yStart - 0.04
        deepJetText2   = getText("DeepCSV (1.5<|#eta|<3)"+textEtaRegion,xStart=xStart,yStart=yStart,size=0.03,color=ROOT.kRed+1)
        deepJetText2.Draw("same")
        yStart = yStart - 0.04
        deepJetText3   = getText("DeepCSV (|#eta|>3)"+textEtaRegion,xStart=xStart,yStart=yStart,size=0.03,color=ROOT.kGreen+1)
        deepJetText3.Draw("same")
        yStart = yStart - 0.04



    can.SaveAs(o.outDir+"/roc_"+name+".pdf")
    can.SaveAs(o.outDir+"/roc_"+name+".png")


#
#
#
def main(etaRegion=""):

    # off_deepcsv_roc   = makeRocPlot("Offline_PUPPI_deepcsv0"+etaRegion, "DeepCSV_l", bkg="matchedPuppi_L"+etaRegion,sig="matchedPuppi_B"+etaRegion,dir="offJets", inFile = "../NtupleAna/run/OUTPUT/29_10_20_Workshop/ttbar_PU200_TrackingV0_default.root")

    # pf_deepcsv_roc_v0_deepcsv   = makeRocPlot("PUPPI_deepcsv1"+etaRegion,     "DeepCSV_l", bkg="matchedPuppiJet_LandPU"+etaRegion,sig="matchedPuppiJet_B"+etaRegion,dir="offJets", inFile = "../NtupleAna/run/OUTPUT/29_10_20_Workshop/ttbar_PU200_TrackingV0_default.root")
    pf_deepcsv_roc_v0_deepcsv   = makeRocPlot("PUPPI_deepcsv1",     "DeepCSV_l", bkg="matchedPuppiJet_L",sig="matchedPuppiJet_B",dir="offJets", inFile = "../NtupleAna/run/OUTPUT/29_10_20_Workshop/ttbar_PU200_TrackingV0_default.root")
    pf_deepcsv_roc_v6_deepcsv   = makeRocPlot("PUPPI_deepcsv1"+etaRegion,     "DeepCSV_l", bkg="matchedJet_L"+etaRegion,sig="matchedJet_B"+etaRegion,dir="offJets", inFile = "../NtupleAna/run/OUTPUT/18_12_TDR/Phase2HLTTDR_TTbar_14TeV_PU200_HLT_TRKv06p1_TICL_cutsV2.root")
    pf_deepcsv_roc_v7_deepcsv   = makeRocPlot("PUPPI_deepcsv1"+etaRegion,     "DeepCSV_l", bkg="matchedJet_L"+etaRegion,sig="matchedJet_B"+etaRegion,dir="offJets", inFile = "../NtupleAna/run/OUTPUT/18_12_TDR/Phase2HLTTDR_TTbar_14TeV_PU200_HLT_TRKv07p2_TICL_cutsV2.root")
    # pf_deepcsv_roc_v0_deepjet   = makeRocPlot("PUPPI_deepcsv1"+etaRegion,     "DeepJet_l", bkg="matchedPuppiJet_LandPU"+etaRegion,sig="matchedPuppiJet_B"+etaRegion,dir="offJets", inFile = "../NtupleAna/run/OUTPUT/29_10_20_Workshop/ttbar_PU200_TrackingV0_default.root")
    pf_deepcsv_roc_v0_deepjet   = makeRocPlot("PUPPI_deepcsv1",     "DeepJet_l", bkg="matchedPuppiJet_L",sig="matchedPuppiJet_B",dir="offJets", inFile = "../NtupleAna/run/OUTPUT/29_10_20_Workshop/ttbar_PU200_TrackingV0_default.root")
    pf_deepcsv_roc_v6_deepjet   = makeRocPlot("PUPPI_deepcsv1"+etaRegion,     "DeepJet_l", bkg="matchedJet_L"+etaRegion,sig="matchedJet_B"+etaRegion,dir="offJets", inFile = "../NtupleAna/run/OUTPUT/18_12_TDR/Phase2HLTTDR_TTbar_14TeV_PU200_HLT_TRKv06p1_TICL_cutsV2.root")
    pf_deepcsv_roc_v7_deepjet   = makeRocPlot("PUPPI_deepcsv1"+etaRegion,     "DeepJet_l", bkg="matchedJet_L"+etaRegion,sig="matchedJet_B"+etaRegion,dir="offJets", inFile = "../NtupleAna/run/OUTPUT/18_12_TDR/Phase2HLTTDR_TTbar_14TeV_PU200_HLT_TRKv07p2_TICL_cutsV2.root")

    for i, rocType in enumerate(["Rej","Eff"]):

        plotSame("PU200_Off_vs_HLTDeepCSV_"+rocType+etaRegion,
              # [off_deepcsv_roc[i], pf_deepcsv_roc[i],  pf_deepcsv_roc2[i], pf_deepcsv_roc3[i],  pf_deepcsv_roc4[i],  pf_deepcsv_roc5[i],  pf_deepcsv_roc6[i],  pf_deepcsv_roc7[i],  pf_deepcsv_roc8[i]],
              # [ROOT.kBlack,      ROOT.kRed,  ROOT.kGreen+2, ROOT.kBlue+2,ROOT.kOrange+1, ROOT.kRed, ROOT.kGreen+2, ROOT.kBlue+2,ROOT.kOrange+1],
              # [ROOT.kSolid,      ROOT.kSolid,  ROOT.kSolid, ROOT.kSolid,ROOT.kSolid, ROOT.kDotted, ROOT.kDotted, ROOT.kDotted, ROOT.kDotted],
              [pf_deepcsv_roc_v0_deepcsv[i], pf_deepcsv_roc_v6_deepcsv[i],  pf_deepcsv_roc_v7_deepcsv[i],  pf_deepcsv_roc_v0_deepjet[i],  pf_deepcsv_roc_v6_deepjet[i], pf_deepcsv_roc_v7_deepjet[i]],
                 [ROOT.kBlack,      ROOT.kRed+1,  ROOT.kBlue+1, ROOT.kBlack, ROOT.kRed+1, ROOT.kBlue+1],
                 [ROOT.kSolid,      ROOT.kSolid,  ROOT.kSolid, ROOT.kDotted, ROOT.kDotted, ROOT.kDotted],
                 plotCaloJet = False,
                 plotPFJet = True,
                 plotOffJet = True,
                 plotCSV = False,
                 plotDeepCSV = True,
                 plotDeepJet = True,
                 rocType = rocType
                 )


    off_deepcsv_roc_v0_allEta   = makeRocPlot("off_deepcsv_roc_v0_allEta", "DeepCSV_l", bkg="matchedPuppiJet_L",sig="matchedPuppiJet_B",dir="offJets", inFile = "../NtupleAna/run/OUTPUT/29_10_20_Workshop/ttbar_PU200_TrackingV0_default.root")
    off_deepcsv_roc_v6_allEta   = makeRocPlot("off_deepcsv_roc_v6_allEta", "DeepCSV_l", bkg="matchedJet_L",sig="matchedJet_B",dir="offJets", inFile = "../NtupleAna/run/OUTPUT/18_12_TDR/Phase2HLTTDR_TTbar_14TeV_PU200_HLT_TRKv06p1_TICL_cutsV2.root")

    pf_deepcsv_roc_v0_eta1   = makeRocPlot("PUPPI_deepcsv1"+"_eta1",     "DeepCSV_l", bkg="matchedPuppiJet_L"+"_eta1",sig="matchedPuppiJet_B"+"_eta1",dir="offJets", inFile = "../NtupleAna/run/OUTPUT/29_10_20_Workshop/ttbar_PU200_TrackingV0_default.root")
    pf_deepcsv_roc_v6_eta1   = makeRocPlot("PUPPI_deepcsv1"+"_eta1",     "DeepCSV_l", bkg="matchedJet_L"+"_eta1",sig="matchedJet_B"+"_eta1",dir="offJets", inFile = "../NtupleAna/run/OUTPUT/18_12_TDR/Phase2HLTTDR_TTbar_14TeV_PU200_HLT_TRKv06p1_TICL_cutsV2.root")
    pf_deepcsv_roc_v0_eta2   = makeRocPlot("PUPPI_deepcsv1"+"_eta2",     "DeepCSV_l", bkg="matchedPuppiJet_L"+"_eta2",sig="matchedPuppiJet_B"+"_eta2",dir="offJets", inFile = "../NtupleAna/run/OUTPUT/29_10_20_Workshop/ttbar_PU200_TrackingV0_default.root")
    pf_deepcsv_roc_v6_eta2   = makeRocPlot("PUPPI_deepcsv1"+"_eta2",     "DeepCSV_l", bkg="matchedJet_L"+"_eta2",sig="matchedJet_B"+"_eta2",dir="offJets", inFile = "../NtupleAna/run/OUTPUT/18_12_TDR/Phase2HLTTDR_TTbar_14TeV_PU200_HLT_TRKv06p1_TICL_cutsV2.root")
    pf_deepcsv_roc_v0_eta3   = makeRocPlot("PUPPI_deepcsv1"+"_eta3",     "DeepCSV_l", bkg="matchedPuppiJet_L"+"_eta3",sig="matchedPuppiJet_B"+"_eta3",dir="offJets", inFile = "../NtupleAna/run/OUTPUT/29_10_20_Workshop/ttbar_PU200_TrackingV0_default.root")
    pf_deepcsv_roc_v6_eta3   = makeRocPlot("PUPPI_deepcsv1"+"_eta3",     "DeepCSV_l", bkg="matchedJet_L"+"_eta3",sig="matchedJet_B"+"_eta3",dir="offJets", inFile = "../NtupleAna/run/OUTPUT/18_12_TDR/Phase2HLTTDR_TTbar_14TeV_PU200_HLT_TRKv06p1_TICL_cutsV2.root")

    for i, rocType in enumerate(["Rej","Eff"]):

        plotSameEtaRanges("PU200_Off_vs_HLTDeepCSV_"+rocType+"_allEta",
              [off_deepcsv_roc_v0_allEta[i], pf_deepcsv_roc_v0_eta1[i], pf_deepcsv_roc_v0_eta2[i],  pf_deepcsv_roc_v0_eta3[i],off_deepcsv_roc_v6_allEta[i],  pf_deepcsv_roc_v6_eta1[i],  pf_deepcsv_roc_v6_eta2[i], pf_deepcsv_roc_v6_eta3[i]],
                 [ROOT.kBlack, ROOT.kBlue+1,      ROOT.kRed+1,  ROOT.kGreen+1, ROOT.kBlack, ROOT.kBlue+1,      ROOT.kRed+1,  ROOT.kGreen+1],
                 [ROOT.kDotted,      ROOT.kDotted,  ROOT.kDotted,ROOT.kDotted, ROOT.kSolid, ROOT.kSolid, ROOT.kSolid, ROOT.kSolid],
                 plotCaloJet = False,
                 plotPFJet = True,
                 plotOffJet = True,
                 plotCSV = False,
                 plotDeepCSV = True,
                 plotDeepJet = True,
                 rocType = rocType
                 )

if __name__ == "__main__":
    # main()
    main(etaRegion="_eta2p4")
    # main(etaRegion="_eta1")
    # main(etaRegion="_eta2")
    # main(etaRegion="_eta3")
