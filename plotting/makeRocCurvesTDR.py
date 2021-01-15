import ROOT


ROOT.gROOT.SetBatch(True)

import ROOTHelp.FancyROOTStyle

from optparse import OptionParser
p = OptionParser()
p.add_option('--input',  type = 'string', default = "outBTag.FTKBtagging.ttbar.mwt2.All.root", dest = 'inFile', help = 'intput File' )
p.add_option('--output', type = 'string', default = "makeRocCurves", dest = 'outDir', help = 'output dir' )
p.add_option('--cmsText', type = 'string', default = "Work in Progress",  help = '' )
(o,a) = p.parse_args()

inFile  = ROOT.TFile(o.inFile,  "READ")


import os
if not os.path.exists(o.outDir):
    os.makedirs(o.outDir)

from rocCurveUtils     import makeRoc
from JetLevelPlotUtils import getCMSText, getText


def makeRocPlot(name, varBKG,varSIG, folder, dir, varNorm=None,debug=False):
    # print dir+"_"+folder+"/"+varSIG
    # print dir+"_"+folder+"/"+varBKG
    # sigHist = inFile.Get(dir+"_"+folder+"/"+varSIG)
    # bkgHist = inFile.Get(dir+"_"+folder+"/"+varBKG)
    sigHist = inFile.Get(folder+"/"+varSIG)
    bkgHist = inFile.Get(folder+"/"+varBKG)

    if varNorm:
        sigNormHist = inFile.Get(dir+"_"+folder+"/"+varNorm)
        bkgNormHist = inFile.Get(dir+"_"+folder+"/"+varNorm)
    else      :
        sigNormHist = sigHist
        bkgNormHist = bkgHist

    rocPlots = []
    for config in [("Rej",1,5e4),("Eff",5e-4,1)]:
        rocPlots.append(makeRoc(sigHist, sigNormHist, bkgHist, bkgNormHist,doErr=False,bkgMode=config[0],cleanNoCut=True,debug=debug))

        can = ROOT.TCanvas(name+"_"+config[0], name+"_"+config[0])
        can.cd().SetLogy(1)
        rocPlots[-1].SetLineWidth(5)
        rocPlots[-1].GetXaxis().SetTitle("B-Jet  Efficiency")
        rocPlots[-1].GetXaxis().SetRangeUser(0.4,1)
        if config[0] == "Rej":    yTitle ="Light Flavor Rejection"
        elif config[0] == "Eff":  yTitle ="Light Flavor Efficiency"
        rocPlots[-1].GetYaxis().SetTitle(yTitle)
        rocPlots[-1].GetYaxis().SetRangeUser(config[1],config[2])
        rocPlots[-1].Draw("AL")
        # can.SaveAs(o.outDir+"/roc_"+name+"_"+config[0]+".pdf")
        # can.SaveAs(o.outDir+"/roc_"+name+"_"+config[0]+".png")
    return rocPlots

def plotSame(name,graphs,colors,styles, plotCaloJet=False, plotPFJet=False, plotOffJet=False, plotPuppiJet=False, plotCSV=False,plotDeepCSV=False,workingPts= None,rocType=None,plotDeepJet=False):

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

    cmsLines = getCMSText(xStart=0.2,yStart=0.875,subtext=o.cmsText)
    for cmsl in cmsLines:
        cmsl.Draw("same")

    yStart = 0.75
    xStart = 0.2
    if rocType == "Rej":
        xStart = 0.5
        yStart = 0.875

    if plotPuppiJet:
        puppiJetText   = getText("HLT Puppi Jets  ",xStart=xStart,yStart=yStart,size=0.04,color=ROOT.kBlack)
        puppiJetText.Draw("same")

        #offJetTextDeep  = getText("Offline DeepCSV (Dashed)  ",xStart=0.6,yStart=0.36,size=0.03,color=ROOT.kBlack)

        #offJetText  = getText("Offline Jet  ",xStart=0.6,yStart=0.4,size=0.03,color=ROOT.kBlack)

    yStart = 0.3
    xStart = 0.6
    if rocType == "Rej":
        xStart = 0.2


    if plotDeepCSV:
        if plotCSV:
            deepCSVText   = getText("DeepCSV (solid)  ",xStart=xStart,yStart=yStart,size=0.04,color=ROOT.kBlack)
        elif plotDeepJet:
            deepCSVText   = getText("DeepJet (dotted)  ",xStart=xStart,yStart=yStart,size=0.04,color=ROOT.kBlack)
        else:
            deepCSVText   = getText("DeepCSV",xStart=xStart,yStart=yStart,size=0.04,color=ROOT.kBlack)
        deepCSVText.Draw("same")
        yStart = yStart - 0.05

    if plotDeepJet:
        if plotCSV:
            deepJetText   = getText("DeepJet (solid)  ",xStart=xStart,yStart=yStart,size=0.04,color=ROOT.kBlack)
        elif plotDeepCSV:
            deepJetText   = getText("DeepCSV (solid)  ",xStart=xStart,yStart=yStart,size=0.04,color=ROOT.kBlack)
        else:
            deepJetText   = getText("DeepJet",xStart=xStart,yStart=yStart,size=0.04,color=ROOT.kBlack)
        deepJetText.Draw("same")
        yStart = yStart - 0.05

    if plotCSV:
        if plotDeepCSV:
            # CSVText   = getText("CSV      (dashed)  ",xStart=xStart,yStart=yStart,size=0.04,color=ROOT.kBlack)
            CSVText   = getText("JetBProb      (dashed)  ",xStart=xStart,yStart=yStart,size=0.04,color=ROOT.kBlack)
        else:
            # CSVText   = getText("CSV",xStart=xStart,yStart=yStart,size=0.04,color=ROOT.kBlack)
            CSVText   = getText("JetBProb",xStart=xStart,yStart=yStart,size=0.04,color=ROOT.kBlack)
        CSVText.Draw("same")




    #offJetTextDeep.Draw("same")

    can.SaveAs(o.outDir+"/roc_"+name+".pdf")
    can.SaveAs(o.outDir+"/roc_"+name+".png")


def plotEtaRangesSame(name,graphs,colors,styles, plotCaloJet=False, plotPFJet=False, plotPuppiJet=False, plotOffJet=False,plotDeepCSV=False,workingPts= None,rocType=None,plotDeepJet=False):

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

    cmsLines= getCMSText(xStart=0.2,yStart=0.875,subtext=o.cmsText)
    for cmsl in cmsLines:
        cmsl.Draw("same")

    yStart = 0.75
    xStart = 0.2
    if rocType == "Rej":
        xStart = 0.5
        yStart = 0.875

    if plotOffJet:
        offJetText  = getText("Offline Jets  (Solid)",xStart=xStart,yStart=yStart,size=0.04,color=ROOT.kBlack)
        yStart = yStart - 0.05
        offJetText.Draw("same")

    if plotPFJet:
        pfJetText   = getText("HLT PFCHS Jets  (Dashed) ",xStart=xStart,yStart=yStart,size=0.04,color=ROOT.kBlack)
        pfJetText.Draw("same")
    if plotPuppiJet:
        puppiJetText   = getText("HLT Puppi Jets  (Dashed) ",xStart=xStart,yStart=yStart,size=0.04,color=ROOT.kBlack)
        puppiJetText.Draw("same")

    yStart = 0.35
    # xStart = 0.6
    xStart = 0.67
    if rocType == "Rej":
        xStart = 0.2

    if plotDeepCSV:
        deepCSVText   = getText("DeepCSV (all Jets)",xStart=xStart,yStart=yStart,size=0.04,color=ROOT.kBlack)
        deepCSVText.Draw("same")
        yStart = yStart - 0.05

        eta1text  = getText("DeepCSV (|#eta|<1.5)",xStart=xStart,yStart=yStart,size=0.04,color=ROOT.kBlue+1)
        yStart = yStart - 0.05
        eta2text  = getText("DeepCSV (1.5<|#eta|<3)",xStart=xStart,yStart=yStart,size=0.04,color=ROOT.kRed)
        yStart = yStart - 0.05
        eta3text  = getText("DeepCSV (|#eta|>3)",xStart=xStart,yStart=yStart,size=0.04,color=ROOT.kGreen+2)
        eta1text.Draw("same")
        eta2text.Draw("same")
        eta3text.Draw("same")
    elif plotDeepJet:
        deepCSVText   = getText("DeepJet (all Jets)",xStart=xStart,yStart=yStart,size=0.04,color=ROOT.kBlack)
        deepCSVText.Draw("same")
        yStart = yStart - 0.05

        eta1text  = getText("DeepJet (|#eta|<1.5)",xStart=xStart,yStart=yStart,size=0.04,color=ROOT.kBlue+1)
        yStart = yStart - 0.05
        eta2text  = getText("DeepJet (1.5<|#eta|<3)",xStart=xStart,yStart=yStart,size=0.04,color=ROOT.kRed)
        yStart = yStart - 0.05
        eta3text  = getText("DeepJet (|#eta|>3)",xStart=xStart,yStart=yStart,size=0.04,color=ROOT.kGreen+2)
        eta1text.Draw("same")
        eta2text.Draw("same")
        eta3text.Draw("same")
    can.SaveAs(o.outDir+"/roc_"+name+".pdf")
    can.SaveAs(o.outDir+"/roc_"+name+".png")



def main():

    # puppi_off_deepcsv_roc        = makeRocPlot("Puppi_Offline_deepcsv",      "DeepCSV_l", bkg="matchedPuppi_L",sig="matchedPuppi_B",dir="offJets")
    # puppi_off_deepcsv_roc_eta1   = makeRocPlot("Puppi_Offline_deepcsv_eta1", "DeepCSV_l", bkg="matchedPuppi_L_eta1",sig="matchedPuppi_B_eta1",dir="offJets")
    # puppi_off_deepcsv_roc_eta2   = makeRocPlot("Puppi_Offline_deepcsv_eta2", "DeepCSV_l", bkg="matchedPuppi_L_eta2",sig="matchedPuppi_B_eta2",dir="offJets")
    # puppi_off_deepcsv_roc_eta3   = makeRocPlot("Puppi_Offline_deepcsv_eta3", "DeepCSV_l", bkg="matchedPuppi_L_eta3",sig="matchedPuppi_B_eta3",dir="offJets")
    # puppi_off_deepjet_roc_eta1   = makeRocPlot("Puppi_Offline_deepjet_eta1", "DeepJet_l", bkg="matchedPuppi_L_eta1",sig="matchedPuppi_B_eta1",dir="offJets")
    # puppi_off_deepjet_roc_eta2   = makeRocPlot("Puppi_Offline_deepjet_eta2", "DeepJet_l", bkg="matchedPuppi_L_eta2",sig="matchedPuppi_B_eta2",dir="offJets")
    # puppi_off_deepjet_roc_eta3   = makeRocPlot("Puppi_Offline_deepjet_eta3", "DeepJet_l", bkg="matchedPuppi_L_eta3",sig="matchedPuppi_B_eta3",dir="offJets")
    # puppi_off_deepjet_roc        = makeRocPlot("Puppi_Offline_deepjet",      "DeepJet_l",   bkg="matchedPuppi_L",sig="matchedPuppi_B",dir="offJets")

    # puppi_deepjet_roc        = makeRocPlot("Puppi_deepjet",      "DeepJet_l", bkg="matchedPuppiJet_L",sig="matchedPuppiJet_B",dir="offJets")
    # puppi_deepcsv_roc        = makeRocPlot("Puppi_deepcsv",      "DeepCSV_l", bkg="matchedPuppiJet_L",sig="matchedPuppiJet_B",dir="offJets")
    puppi_deepcsv_roc        = makeRocPlot("Puppi_deepcsv",      varBKG="deepCSV_discr_l", varSIG="deepCSV_discr_b",folder="noFilter_PFDeepCSVPuppi",dir="",debug=True)
    puppi_deepjet_roc        = makeRocPlot("Puppi_deepjet",      varBKG="deepJet_discr_l", varSIG="deepJet_discr_b",folder="noFilter_PFDeepCSVPuppi",dir="")
    # puppi_deepcsv_roc_eta1   = makeRocPlot("Puppi_deepcsv_eta1", "DeepCSV_l", bkg="matchedPuppiJet_L_eta1",sig="matchedPuppiJet_B_eta1",dir="offJets")
    # puppi_deepcsv_roc_eta2   = makeRocPlot("Puppi_deepcsv_eta2", "DeepCSV_l", bkg="matchedPuppiJet_L_eta2",sig="matchedPuppiJet_B_eta2",dir="offJets")
    # puppi_deepcsv_roc_eta3   = makeRocPlot("Puppi_deepcsv_eta3", "DeepCSV_l", bkg="matchedPuppiJet_L_eta3",sig="matchedPuppiJet_B_eta3",dir="offJets")
    # puppi_deepjet_roc_eta1   = makeRocPlot("Puppi_deepjet_eta1", "DeepJet_l", bkg="matchedPuppiJet_L_eta1",sig="matchedPuppiJet_B_eta1",dir="offJets")
    # puppi_deepjet_roc_eta2   = makeRocPlot("Puppi_deepjet_eta2", "DeepJet_l", bkg="matchedPuppiJet_L_eta2",sig="matchedPuppiJet_B_eta2",dir="offJets")
    # puppi_deepjet_roc_eta3   = makeRocPlot("Puppi_deepjet_eta3", "DeepJet_l", bkg="matchedPuppiJet_L_eta3",sig="matchedPuppiJet_B_eta3",dir="offJets")



    for i, rocType in enumerate(["Rej","Eff"]):

        # plotEtaRangesSame("PuppiOff_vs_HLTDeepCSV_"+rocType+"_etaRanges",
        #          [puppi_off_deepcsv_roc[i], puppi_deepcsv_roc[i],
        #           puppi_off_deepcsv_roc_eta1[i], puppi_deepcsv_roc_eta1[i],
        #           puppi_off_deepcsv_roc_eta2[i], puppi_deepcsv_roc_eta2[i],
        #           puppi_off_deepcsv_roc_eta3[i], puppi_deepcsv_roc_eta3[i]],
        #          [ROOT.kBlack,      ROOT.kBlack,
        #           ROOT.kBlue+1,      ROOT.kBlue+1,
        #           ROOT.kRed,      ROOT.kRed,
        #           ROOT.kGreen+2,      ROOT.kGreen+2],
        #          [ROOT.kSolid,      ROOT.kDotted,
        #           ROOT.kSolid,      ROOT.kDotted,
        #           ROOT.kSolid,      ROOT.kDotted,
        #           ROOT.kSolid,      ROOT.kDotted],
        #           plotCaloJet = False,
        #           plotPFJet = False,
        #           plotOffJet = True,
        #           plotPuppiJet=True,
        #           plotDeepCSV = True,
        #           rocType = rocType
        #           )
        # plotEtaRangesSame("PuppiOff_vs_HLTDeepJet_"+rocType+"_etaRanges",
        #          [puppi_off_deepjet_roc[i], puppi_deepjet_roc[i],
        #           puppi_off_deepjet_roc_eta1[i], puppi_deepjet_roc_eta1[i],
        #           puppi_off_deepjet_roc_eta2[i], puppi_deepjet_roc_eta2[i],
        #           puppi_off_deepjet_roc_eta3[i], puppi_deepjet_roc_eta3[i]],
        #          [ROOT.kBlack,      ROOT.kBlack,
        #           ROOT.kBlue+1,      ROOT.kBlue+1,
        #           ROOT.kRed,      ROOT.kRed,
        #           ROOT.kGreen+2,      ROOT.kGreen+2],
        #          [ROOT.kSolid,      ROOT.kDotted,
        #           ROOT.kSolid,      ROOT.kDotted,
        #           ROOT.kSolid,      ROOT.kDotted,
        #           ROOT.kSolid,      ROOT.kDotted],
        #           plotCaloJet = False,
        #           plotPFJet = False,
        #           plotOffJet = True,
        #           plotPuppiJet=True,
        #           plotDeepCSV = False,
        #           rocType = rocType,
        #           plotDeepJet = True
        #           )


        # plotSame("PuppiOff_vs_HLTDeepCSV_"+rocType+"_eta1",
        #          [puppi_off_deepcsv_roc_eta1[i], puppi_deepcsv_roc_eta1[i]],
        #          [ROOT.kBlack,      ROOT.kBlue],
        #          [ROOT.kSolid,      ROOT.kSolid],
        #          plotCaloJet = False,
        #          plotPFJet = False,
        #          plotOffJet = True,
        #          plotPuppiJet=True,
        #          plotCSV = True,
        #          plotDeepCSV = True,
        #          rocType = rocType
        #          )
        # plotSame("PuppiOff_vs_HLTDeepCSV_"+rocType+"_eta2",
        #          [puppi_off_deepcsv_roc_eta2[i], puppi_deepcsv_roc_eta2[i]],
        #          [ROOT.kBlack,      ROOT.kBlue],
        #          [ROOT.kSolid,      ROOT.kSolid],
        #          plotCaloJet = False,
        #          plotPFJet = False,
        #          plotOffJet = True,
        #          plotPuppiJet=True,
        #          plotCSV = True,
        #          plotDeepCSV = True,
        #          rocType = rocType
        #          )
        # plotSame("PuppiOff_vs_HLTDeepCSV_"+rocType+"_eta3",
        #          [puppi_off_deepcsv_roc_eta3[i], puppi_deepcsv_roc_eta3[i]],
        #          [ROOT.kBlack,      ROOT.kBlue],
        #          [ROOT.kSolid,      ROOT.kSolid],
        #          plotCaloJet = False,
        #          plotPFJet = False,
        #          plotOffJet = True,
        #          plotPuppiJet=True,
        #          plotCSV = True,
        #          plotDeepCSV = True,
        #          rocType = rocType
        #          )


        # plotSame("PuppiOff_vs_HLT_All_"+rocType,
        #          [puppi_off_deepcsv_roc[i], puppi_off_deepjet_roc[i], puppi_deepcsv_roc[i],  puppi_deepjet_roc[i]],
        #          [ROOT.kBlack,       ROOT.kBlack,     ROOT.kBlue,         ROOT.kBlue],
        #          [ROOT.kSolid,      ROOT.kDashed,     ROOT.kSolid,        ROOT.kDashed],
        #          plotCaloJet = False,
        #          plotPFJet = False,
        #          plotOffJet = True,
        #          plotPuppiJet=True,
        #          plotCSV = False,
        #          plotDeepCSV = True,
        #          rocType = rocType,
        #          plotDeepJet=True
        #          )
        plotSame("PuppiOff_vs_HLT_All_"+rocType,
                 [puppi_deepcsv_roc[i],  puppi_deepjet_roc[i]],
                 [ROOT.kBlack,  ROOT.kBlack],
                 [ROOT.kSolid,   ROOT.kDotted],
                 plotCaloJet = False,
                 plotPFJet = False,
                 plotOffJet = False,
                 plotPuppiJet=True,
                 plotCSV = False,
                 plotDeepCSV = True,
                 rocType = rocType,
                 plotDeepJet=True
                 )


        # plotSame("PuppiOff_vs_HLTDeepCSV_"+rocType,
        #          [puppi_off_deepcsv_roc[i], puppi_deepcsv_roc[i]],
        #          [ROOT.kBlack,        ROOT.kBlue],
        #          [ROOT.kSolid,        ROOT.kSolid],
        #          plotCaloJet = False,
        #          plotPFJet = False,
        #          plotOffJet = True,
        #          plotPuppiJet=True,
        #          plotCSV = False,
        #          plotDeepCSV = True,
        #          rocType = rocType
        #          )





if __name__ == "__main__":
    main()
