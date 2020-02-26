import ROOT


ROOT.gROOT.SetBatch(True)

import sys
sys.path.insert(0, '../../')
import ROOTHelp.FancyROOTStyle


from optparse import OptionParser
p = OptionParser()
#p.add_option('--inputData',  type = 'string', dest = 'inFileData', help = 'intput File' )
p.add_option('--inputMC',  type = 'string', dest = 'inFileMC', help = 'intput File' )
p.add_option('--output', type = 'string', default = "jetLevelPlots", dest = 'outDir', help = 'output dir' )
p.add_option('--doAlgoStudy', action="store_true" )
(o,a) = p.parse_args()

#from rocCurveUtils            import drawWaterMarks
#import rebinning
#from Rebinning import rebinningDB

#inFileData  = ROOT.TFile(o.inFileData,  "READ")
inFileMC    = ROOT.TFile(o.inFileMC,  "READ")

import os
if not os.path.exists(o.outDir):
    os.makedirs(o.outDir)


from JetLevelPlotUtils import makeEff, drawComp, getHist, drawStackCompRatio, makeStack, makeInverseTurnOn, make2DComp, makeInverseTurnOnAll,plotRatio

#plotRatio("nPV",    "Events",inFileData, "Data", inFileMC, "MC",xTitle="nPV", outDir=o.outDir,binning=1)


#
#  Offline Turnon curves:
#
#effBinning=5
vars = [
    "phi"            ,
#        "eta"            ,
        "Chi2",
        "DecayLenVal"    ,
        "DecayLenVal_l"  ,
        "DeltaR"         ,
        "DeltaR_l"       ,
        "Eta"            ,
         "HasInnerPixHit",
        "IsFromSV"       ,
        "IsFromV0"       ,
        "JetDistVal"     ,
        "Momentum"       ,
        "NPixelHits"     ,
        "NStripHits"     ,
        "NTotalHits"     ,
        "PPar"           ,
        "PParRatio"      ,
        "PV"             ,
        "PVweight"       ,
        "Phi"            ,
        "PtRatio"        ,
        "PtRel"          ,
        "Pt_logx"        ,
        "SV"             ,
        "SVweight"       ,
        "algo"           ,
        "origAlgo"           ,
        "charge"         ,
         "eta"           ,
        "ip2d"           ,
        "ip2d_err"       ,
        "ip2d_err_l"     ,
        "ip2d_l"         ,
        "ip2d_sig"       ,
        "ip2d_sig_l"     ,
        "ip3d"           ,
        "ip3d_err"       ,
        "ip3d_err_l"     ,
        "ip3d_l"         ,
        "ip3d_sig"       ,
        "ip3d_sig_l"     ,


#        "trackMomentum"       ,
#        "ip2d",
#        "ip2d_l",
#        "ip3d",
#        "ip3d_l",
#        "ip2d_sig",
#        "ip2d_sig_l",
#        "ip3d_sig",
#        "ip3d_sig_l",
#        "ip2d_err",
#        "ip3d_err",
#        "trackHasInnerPixHit",
#        "trackNPixelHits",

#        "trackNTotalHits",

        ]

if o.doAlgoStudy:
    for i in range(30):
        vars += ["Eta_forAlgo"+str(i)]
        vars += ["Pt_forAlgo"+str(i)]


for v in vars:
    binning = 1
    if not v.find("Eta_forAlgo") == -1:
        binning = 4
    if not v.find("Pt_forAlgo") == -1:
        binning = [0,1,2,3,4,5,6,7,8,9,10,12,14,16,18,20,25,30,40,50]

    eff_Matched = makeEff(v ,         ["offTracks_matched","offTracks"],inFileMC,binning=binning)
    eff_Matched_noV0 = makeEff(v ,    ["offTracks_matched_noV0","offTracks_noV0"],inFileMC,binning=binning)

    effCalo_Matched = makeEff(v ,         ["offTracksCalo_matched","offTracksCalo"],inFileMC,binning=binning)
    effCalo_Matched_noV0 = makeEff(v ,    ["offTracksCalo_matched_noV0","offTracksCalo_noV0"],inFileMC,binning=binning)

    vBtag = "track"+v
    if v=="phi": vBtag = "trackPhi"
    if v=="eta": vBtag = "trackEta"
    if v in ["HasInnerPixHit","IsFromSV","NStripHits","PV","PVweight","SV","SVweight","algo","charge"]: pass #continue
    if v in ["ip2d","ip2d_l","ip2d_err","ip2d_err_l","ip2d_sig","ip2d_sig_l",
             "ip3d","ip3d_l","ip3d_err","ip3d_err_l","ip3d_sig","ip3d_sig_l",
             ]: vBtag = v


    print "Doing ",vBtag
    #eff_Matched_BTag = makeEff(vBtag ,    ["offBTags_matched","offBTags"],inFileMC,binning=1)
    #eff_Matched_BTag_noV0 = makeEff(vBtag ,    ["offBTags_matched_noV0","offBTags_noV0"],inFileMC,binning=1)

    yLeg = 0.93
    xLeg = 0.5
    if v in  ["algo","origAlgo"]:
        yLeg = 0.4
        xLeg = 0.6

    drawComp("Eff_"+v,[(eff_Matched,"t#bar{t} MC (All Tracks)",ROOT.kBlack),
                       # (eff_Matched_noV0,"t#bar{t} MC (After V0 veto)",ROOT.kRed),
                       (eff_Matched_noV0,"t#bar{t} MC (After V0/K_{S}^{0} veto)",ROOT.kRed),
                       #(eff_Matched_BTag,"t#bar{t} MC ",ROOT.kBlue),
                       #(eff_Matched_BTag_noV0,"t#bar{t} MC ",ROOT.kGreen)
                       ]
             ,yTitle="Online Track Efficiency Relative to Offline",xTitle=eff_Matched.GetXaxis().GetTitle(),outDir=o.outDir,yMax=1.2,yLeg=yLeg,xLeg=xLeg,
             xMax=eff_Matched.GetXaxis().GetXmax(),
             xMin=eff_Matched.GetXaxis().GetXmin()
             )


    drawComp("EffCalo_"+v,[(effCalo_Matched,"t#bar{t} MC (All Tracks)",ROOT.kBlack),
                           # (effCalo_Matched_noV0,"t#bar{t} MC (After V0 veto)",ROOT.kRed),
                           (effCalo_Matched_noV0,"t#bar{t} MC (After V0/K_{S}^{0} veto)",ROOT.kRed),
                           #(eff_Matched_BTag,"t#bar{t} MC ",ROOT.kBlue),
                           #(eff_Matched_BTag_noV0,"t#bar{t} MC ",ROOT.kGreen)
                       ]
             ,yTitle="Online Track Efficiency Relative to Offline",xTitle=effCalo_Matched.GetXaxis().GetTitle(),outDir=o.outDir,yMax=1.2,yLeg=yLeg,xLeg=xLeg,
             xMax=effCalo_Matched.GetXaxis().GetXmax(),
             xMin=effCalo_Matched.GetXaxis().GetXmin()
             )


    drawComp("Eff_PVvsCalo_"+v,[(eff_Matched,"t#bar{t} MC (PF Jets)",ROOT.kBlack),
                                (effCalo_Matched,"t#bar{t} MC (Calo Jets)",ROOT.kRed),
                                #(eff_Matched_BTag,"t#bar{t} MC ",ROOT.kBlue),
                                #(eff_Matched_BTag_noV0,"t#bar{t} MC ",ROOT.kGreen)
                       ]
             ,yTitle="Online Track Efficiency Relative to Offline",xTitle=eff_Matched.GetXaxis().GetTitle(),outDir=o.outDir,yMax=1.2,yLeg=yLeg,xLeg=xLeg,
             xMax=eff_Matched.GetXaxis().GetXmax(),
             xMin=eff_Matched.GetXaxis().GetXmin()
             )



    fake_Matched = makeEff(v ,        ["pfTracks_unmatched","pfTracks"],inFileMC,binning=1)
    fakeCalo_Matched = makeEff(v ,    ["caloTracks_unmatched","caloTracks"],inFileMC,binning=1)

    drawComp("Fake_"+v,[(fake_Matched,"t#bar{t} MC ",ROOT.kBlack)]
             ,yTitle="Online Track Fake-Rate Relative to Offline",xTitle=fake_Matched.GetXaxis().GetTitle(),outDir=o.outDir,yMax=0.4,yLeg=0.9,xLeg=0.6,
             xMax=fake_Matched.GetXaxis().GetXmax(),
             xMin=fake_Matched.GetXaxis().GetXmin()
             )

    drawComp("FakeCalo_"+v,[(fake_Matched,"t#bar{t} MC ",ROOT.kBlack)]
             ,yTitle="Online Track Fake-Rate Relative to Offline",xTitle=fakeCalo_Matched.GetXaxis().GetTitle(),outDir=o.outDir,yMax=0.4,yLeg=0.9,xLeg=0.6,
             xMax=fakeCalo_Matched.GetXaxis().GetXmax(),
             xMin=fakeCalo_Matched.GetXaxis().GetXmin()
             )



#eff_CaloCSV_MC   = makeEff("csv" ,    ["offJets_matchedCalocsvTag","offJets_matchedCalo"],inFileMC  ,binning=effBinning)
#eff_PFCSV_Data   = makeEff("csv" ,    ["offJets_matchedPFcsvTag",  "offJets_matchedPF"]  ,inFileData,binning=effBinning)
#eff_PFCSV_MC     = makeEff("csv" ,    ["offJets_matchedPFcsvTag",  "offJets_matchedPF"]  ,inFileMC  ,binning=effBinning)

#
#eff_CaloCSVvsDeepCSV_Data = makeEff("deepcsv" ,    ["offJets_matchedCalocsvTag","offJets_matchedCalo"],inFileData,binning=effBinning)
#eff_CaloCSVvsDeepCSV_MC   = makeEff("deepcsv" ,    ["offJets_matchedCalocsvTag","offJets_matchedCalo"],inFileMC  ,binning=effBinning)
#eff_PFCSVvsDeepCSV_Data   = makeEff("deepcsv" ,    ["offJets_matchedPFcsvTag",  "offJets_matchedPF"]  ,inFileData,binning=effBinning)
#eff_PFCSVvsDeepCSV_MC     = makeEff("deepcsv" ,    ["offJets_matchedPFcsvTag",  "offJets_matchedPF"]  ,inFileMC  ,binning=effBinning)
#
#
#eff_CaloDeepCSV_Data = makeEff("deepcsv" ,    ["offJets_matchedCaloDeepcsvTag","offJets_matchedCalo"],inFileData,binning=effBinning)
#eff_CaloDeepCSV_MC   = makeEff("deepcsv" ,    ["offJets_matchedCaloDeepcsvTag","offJets_matchedCalo"],inFileMC  ,binning=effBinning)
#eff_PFDeepCSV_Data   = makeEff("deepcsv" ,    ["offJets_matchedPFDeepcsvTag",  "offJets_matchedPF"]  ,inFileData,binning=effBinning)
#eff_PFDeepCSV_MC     = makeEff("deepcsv" ,    ["offJets_matchedPFDeepcsvTag",  "offJets_matchedPF"]  ,inFileMC  ,binning=effBinning)
#
#
#reveff_CSV_MC = {}
#reveff_CSV_Data = {}
#for jetType in ["Calo","PF"]:
#    reveff_CSV_MC[jetType] = {}
#    reveff_CSV_Data[jetType] = {}
#    for op in ["Loose","Medium","Tight"]:
#        reveff_CSV_MC  [jetType][op]   = makeEff("csv" ,    ["offJets"+op+"_matched"+jetType+"Jet",  "offJets_matched"+jetType+"Jet"]  ,inFileMC    ,binning=effBinning)
#        reveff_CSV_Data[jetType][op]   = makeEff("csv" ,    ["offJets"+op+"_matched"+jetType+"Jet",  "offJets_matched"+jetType+"Jet"]  ,inFileData  ,binning=effBinning)
#
#
#    drawComp("RevEff_"+jetType+"CSV_MC",[(reveff_CSV_MC[jetType]["Loose"], "Loose",ROOT.kBlue),
#                                         (reveff_CSV_MC[jetType]["Medium"],"Medium",ROOT.kGreen),
#                                         (reveff_CSV_MC[jetType]["Tight"], "Tight",ROOT.kRed),]
#             ,yTitle="Efficiency",xTitle="CSV Value of Jets", otherText=""+jetType+" Jets (MC)",outDir=o.outDir)
#
#    drawComp("RevEff_"+jetType+"CSV_Data",[(reveff_CSV_Data[jetType]["Loose"], "Loose",ROOT.kBlue),
#                                           (reveff_CSV_Data[jetType]["Medium"],"Medium",ROOT.kGreen),
#                                           (reveff_CSV_Data[jetType]["Tight"], "Tight",ROOT.kRed),]
#             ,yTitle="Efficiency",xTitle="CSV Value of Jets", otherText=""+jetType+" Jets (Data)",outDir=o.outDir)
#
#
#    drawComp("RevEff_"+jetType+"CSV_All",[(reveff_CSV_Data[jetType]["Loose"], "Loose (Data)",ROOT.kBlue),
#                                          (reveff_CSV_MC[jetType]["Loose"], "Loose (MC)",ROOT.kBlue,24),
#                                          (reveff_CSV_Data[jetType]["Medium"],"Medium (Data)",ROOT.kGreen+1),
#                                          (reveff_CSV_MC[jetType]["Medium"],"Medium (MC)",ROOT.kGreen+1,24),
#                                          (reveff_CSV_Data[jetType]["Tight"], "Tight (Data)",ROOT.kRed),
#                                          (reveff_CSV_MC[jetType]["Tight"], "Tight (MC)",ROOT.kRed,24),
#                                          ]
#             ,yTitle="Efficiency",xTitle="Online ("+jetType+"-Jet) CSV Value", otherText="",outDir=o.outDir,leg="special")
#
#
#
#
##   Data vs MC
#drawComp("Eff_CaloCSV_DataVSMC",[(eff_CaloCSV_Data,"Data",ROOT.kBlue),(eff_CaloCSV_MC,"MC",ROOT.kRed)]
#         ,yTitle="Efficiency",xTitle="CSV Value of Jets", otherText="Online WP: CSV > 0.8484",outDir=o.outDir)
#drawComp("Eff_PFCSV_DataVSMC",[(eff_PFCSV_Data,"Data",ROOT.kBlue),(eff_PFCSV_MC,"MC",ROOT.kRed)]
#         ,yTitle="Efficiency",xTitle="CSV Value of Jets", otherText="Online WP: CSV > 0.8484",outDir=o.outDir)
#
#drawComp("Eff_CaloCSVvsDeepCSV_DataVSMC",[(eff_CaloCSVvsDeepCSV_Data,"Data",ROOT.kBlue),(eff_CaloCSVvsDeepCSV_MC,"MC",ROOT.kRed)]
#         ,yTitle="Efficiency",xTitle="DeepCSV Value of Jets", otherText="Online WP: CSV > 0.8484",outDir=o.outDir)
#drawComp("Eff_PFCSVvsDeepCSV_DataVSMC",[(eff_PFCSVvsDeepCSV_Data,"Data",ROOT.kBlue),(eff_PFCSVvsDeepCSV_MC,"MC",ROOT.kRed)]
#         ,yTitle="Efficiency",xTitle="DeepCSV Value of Jets", otherText="Online WP: CSV > 0.8484",outDir=o.outDir)
#
#
#drawComp("Eff_CaloDeepCSV_DataVSMC",[(eff_CaloDeepCSV_Data,"Data",ROOT.kBlue),(eff_CaloDeepCSV_MC,"MC",ROOT.kRed)]
#         ,yTitle="Efficiency",xTitle="DeepCSV Value of Jets", otherText="Online WP: DeepCSV > 0.6324",outDir=o.outDir)
#drawComp("Eff_PFDeepCSV_DataVSMC",[(eff_PFDeepCSV_Data,"Data",ROOT.kBlue),(eff_PFDeepCSV_MC,"MC",ROOT.kRed)]
#         ,yTitle="Efficiency",xTitle="DeepCSV Value of Jets", otherText="Online WP: DeepCSV > 0.6324",outDir=o.outDir)
#
#
##
##   Calo vs PF
##
#drawComp("Eff_CaloVsPF_CSV_Data",[(eff_CaloCSV_Data,"Calo (Data)",ROOT.kBlue),(eff_PFCSV_Data,"PF (Data)",ROOT.kRed)]
#         ,yTitle="Efficiency",xTitle="CSV Value of Jets", otherText="Online WP: CSV > 0.8484",outDir=o.outDir)
#drawComp("Eff_CaloVsPF_CSV_MC",[(eff_CaloCSV_MC,"Calo (MC)",ROOT.kBlue),(eff_PFCSV_MC,"PF (MC)",ROOT.kRed)]
#         ,yTitle="Efficiency",xTitle="CSV Value of Jets", otherText="Online WP: CSV > 0.8484",outDir=o.outDir)
#
#drawComp("Eff_CaloVsPF_CSVvsDeepCSV_Data",[(eff_CaloCSVvsDeepCSV_Data,"Calo (Data)",ROOT.kBlue),(eff_PFCSVvsDeepCSV_Data,"PF (Data)",ROOT.kRed)]
#         ,yTitle="Efficiency",xTitle="DeepCSV Value of Jets", otherText="Online WP: CSV > 0.8484",outDir=o.outDir)
#drawComp("Eff_CaloVsPF_CSVvsDeepCSV_MC",[(eff_CaloCSVvsDeepCSV_MC,"Calo (MC)",ROOT.kBlue),(eff_PFCSVvsDeepCSV_MC,"PF (MC)",ROOT.kRed)]
#         ,yTitle="Efficiency",xTitle="DeepCSV Value of Jets", otherText="Online WP: CSV > 0.8484",outDir=o.outDir)
#
#
#drawComp("Eff_CaloVsPF_DeepCSV_Data",[(eff_CaloDeepCSV_Data,"Calo (Data)",ROOT.kBlue),(eff_PFDeepCSV_Data,"PF (Data)",ROOT.kRed)]
#         ,yTitle="Efficiency",xTitle="DeepCSV Value of Jets", otherText="Online WP: DeepCSV > 0.6324",outDir=o.outDir)
#drawComp("Eff_CaloVsPF_DeepCSV_MC",[(eff_CaloDeepCSV_MC,"Calo (MC)",ROOT.kBlue),(eff_PFDeepCSV_MC,"PF (MC)",ROOT.kRed)]
#         ,yTitle="Efficiency",xTitle="DeepCSV Value of Jets", otherText="Online WP: DeepCSV > 0.6324",outDir=o.outDir)
#
#
##   (Calo vs PF) x (Data vs MC )
#drawComp("Eff_CaloVsPF_CSV_DataVsMC",
#         [(eff_CaloCSV_Data,"Calo-Jet (Data)",ROOT.kBlue),
#          (eff_CaloCSV_MC,  "Calo-Jet (MC)  ",ROOT.kBlue, 24),
#          (eff_PFCSV_Data,  "PF-Jet   (Data)",ROOT.kRed),
#          (eff_PFCSV_MC,    "PF-Jet   (MC)  ",ROOT.kRed, 24),
#          ]
#         ,yTitle="Efficiency",xTitle="Offline CSV Value", otherText="Online WP: CSV > 0.8484",outDir=o.outDir)
#
#drawComp("Eff_CaloVsPF_CSVvsDeepCSV_DataVsMC",
#         [(eff_CaloCSVvsDeepCSV_Data,"Calo-Jet (Data)",ROOT.kBlue),
#          (eff_CaloCSVvsDeepCSV_MC,  "Calo-Jet (MC)  ",ROOT.kBlue, 24),
#          (eff_PFCSVvsDeepCSV_Data,  "PF-Jet   (Data)",ROOT.kRed),
#          (eff_PFCSVvsDeepCSV_MC,    "PF-Jet   (MC)  ",ROOT.kRed, 24),
#          ]
#         ,yTitle="Efficiency",xTitle="Offline DeepCSV Value", otherText="Online WP: CSV > 0.8484",outDir=o.outDir)
#
#
##
## Offline vs Online
##
#csvBinning=3
## Deep CSV
#offDeepCSV_Calo    = getHist(inFileData,"offJets_matchedCalo",     "deepcsv",binning=csvBinning,norm=1)
#caloDeepCSV        = getHist(inFileData,"offJets_matchedCaloJet",  "deepcsv",binning=csvBinning,norm=1)
#offDeepCSV_Calo_MC = getHist(inFileMC,  "offJets_matchedCalo",     "deepcsv",binning=csvBinning,norm=1)
#caloDeepCSV_MC     = getHist(inFileMC,  "offJets_matchedCaloJet",  "deepcsv",binning=csvBinning,norm=1)
#
#offDeepCSV_PF      = getHist(inFileData,"offJets_matchedPF",     "deepcsv",binning=csvBinning,norm=1)
#pfDeepCSV          = getHist(inFileData,"offJets_matchedPFJet",  "deepcsv",binning=csvBinning,norm=1)
#offDeepCSV_PF_MC   = getHist(inFileMC,  "offJets_matchedPF",     "deepcsv",binning=csvBinning,norm=1)
#pfDeepCSV_MC       = getHist(inFileMC,  "offJets_matchedPFJet",  "deepcsv",binning=csvBinning,norm=1)
#
## CSV
#offCSV_Calo    = getHist(inFileData,"offJets_matchedCalo",     "csv",binning=csvBinning,norm=1)
#caloCSV        = getHist(inFileData,"offJets_matchedCaloJet",  "csv",binning=csvBinning,norm=1)
#offCSV_Calo_MC = getHist(inFileMC,  "offJets_matchedCalo",     "csv",binning=csvBinning,norm=1)
#caloCSV_MC     = getHist(inFileMC,  "offJets_matchedCaloJet",  "csv",binning=csvBinning,norm=1)
#
#offCSV_PF      = getHist(inFileData,"offJets_matchedPF",       "csv",binning=csvBinning,norm=1)
#pfCSV          = getHist(inFileData,"offJets_matchedPFJet",    "csv",binning=csvBinning,norm=1)
#offCSV_PF_MC   = getHist(inFileMC,  "offJets_matchedPF",       "csv",binning=csvBinning,norm=1)
#pfCSV_MC       = getHist(inFileMC,  "offJets_matchedPFJet",    "csv",binning=csvBinning,norm=1)
#
####
##### Off Vs HLT
##drawCompRatio("DeepCSV_CaloVsOff",[(caloDeepCSV,"Calo Jets (Data)"),(offDeepCSV_Calo,"Offline Jets (Data)")]
##              ,yTitle="Normalized",xTitle="DeepCSV Value of Jets",rTitle="Calo/Offline",setLogy=0)
##drawCompRatio("DeepCSV_CaloVsOff_MC",[(caloDeepCSV_MC,"Calo Jets (MC)"),(offDeepCSV_Calo_MC,"Offline Jets (MC)")]
##              ,yTitle="Normalized",xTitle="DeepCSV Value of Jets",rTitle="Calo/Offline",setLogy=0)
##drawCompRatio("DeepCSV_PFVsOff",[(pfDeepCSV,"PF Jets (Data)"),(offDeepCSV_PF,"Offline Jets (Data)")]
##              ,yTitle="Normalized",xTitle="DeepCSV Value of Jets",rTitle="PF/Offline",setLogy=0)
##drawCompRatio("DeepCSV_PFVsOff_MC",[(pfDeepCSV_MC,"PF Jets (MC)"),(offDeepCSV_PF_MC,"Offline Jets (MC)")]
##              ,yTitle="Normalized",xTitle="DeepCSV Value of Jets",rTitle="PF/Offline",setLogy=0)
##
### Data Vs MC
##drawCompRatio("CSV_Calo_DataVsMC",[(caloCSV,"Data"),(caloCSV_MC,"MC")]
##              ,yTitle="Normalized",xTitle="Calo CSV",rTitle="Data/MC",setLogy=0)
##drawCompRatio("DeepCSV_Calo_DataVsMC",[(caloDeepCSV,"Data"),(caloDeepCSV_MC,"MC")]
##              ,yTitle="Normalized",xTitle="Calo DeepCSV",rTitle="Data/MC",setLogy=0)
##drawCompRatio("CSV_Off_DataVsMC",[(offCSV_PF,"Data"),(offCSV_PF_MC,"MC")]
##              ,yTitle="Normalized",xTitle="Offline CSV",rTitle="Data/MC",setLogy=0)
##drawCompRatio("DeepCSV_Off_DataVsMC",[(offDeepCSV_PF,"Data"),(offDeepCSV_PF_MC,"MC")]
##              ,yTitle="Normalized",xTitle="Offline DeepCSV",rTitle="Data/MC",setLogy=0)
##drawCompRatio("CSV_PF_DataVsMC",[(pfCSV,"Data"),(pfCSV_MC,"MC")]
##              ,yTitle="Normalized",xTitle="PF CSV",rTitle="Data/MC",setLogy=0)
##drawCompRatio("DeepCSV_PF_DataVsMC",[(pfDeepCSV,"Data"),(pfDeepCSV_MC,"MC")]
##              ,yTitle="Normalized",xTitle="PF DeepCSV",rTitle="Data/MC",setLogy=0)
#
#
#offCSV_B_PF_MC      = getHist(inFileMC,"offJets_B_matchedPF",     "csv",binning=csvBinning,norm=0)
#offCSV_C_PF_MC      = getHist(inFileMC,"offJets_C_matchedPF",     "csv",binning=csvBinning,norm=0)
#offCSV_L_PF_MC      = getHist(inFileMC,"offJets_L_matchedPF",     "csv",binning=csvBinning,norm=0)
#
#offDeepCSV_B_PF_MC  = getHist(inFileMC,"offJets_B_matchedPF",     "deepcsv",binning=csvBinning,norm=0)
#offDeepCSV_C_PF_MC  = getHist(inFileMC,"offJets_C_matchedPF",     "deepcsv",binning=csvBinning,norm=0)
#offDeepCSV_L_PF_MC  = getHist(inFileMC,"offJets_L_matchedPF",     "deepcsv",binning=csvBinning,norm=0)
#
#
#offDeepCSV_B_Calo_MC  = getHist(inFileMC,"offJets_B_matchedCalo",     "deepcsv",binning=csvBinning,norm=0)
#offDeepCSV_C_Calo_MC  = getHist(inFileMC,"offJets_C_matchedCalo",     "deepcsv",binning=csvBinning,norm=0)
#offDeepCSV_L_Calo_MC  = getHist(inFileMC,"offJets_L_matchedCalo",     "deepcsv",binning=csvBinning,norm=0)
#
#caloCSV_B_MC      = getHist(inFileMC,"offJets_B_matchedCaloJet",     "csv",binning=csvBinning,norm=0)
#caloCSV_C_MC      = getHist(inFileMC,"offJets_C_matchedCaloJet",     "csv",binning=csvBinning,norm=0)
#caloCSV_L_MC      = getHist(inFileMC,"offJets_L_matchedCaloJet",     "csv",binning=csvBinning,norm=0)
#
#caloDeepCSV_B_MC  = getHist(inFileMC,"offJets_B_matchedCaloJet",     "deepcsv",binning=csvBinning,norm=0)
#caloDeepCSV_C_MC  = getHist(inFileMC,"offJets_C_matchedCaloJet",     "deepcsv",binning=csvBinning,norm=0)
#caloDeepCSV_L_MC  = getHist(inFileMC,"offJets_L_matchedCaloJet",     "deepcsv",binning=csvBinning,norm=0)
#
#pfCSV_B_MC        = getHist(inFileMC,"offJets_B_matchedPFJet",       "csv",binning=csvBinning,norm=0)
#pfCSV_C_MC        = getHist(inFileMC,"offJets_C_matchedPFJet",       "csv",binning=csvBinning,norm=0)
#pfCSV_L_MC        = getHist(inFileMC,"offJets_L_matchedPFJet",       "csv",binning=csvBinning,norm=0)
#
#pfDeepCSV_B_MC    = getHist(inFileMC,"offJets_B_matchedPFJet",     "deepcsv",binning=csvBinning,norm=0)
#pfDeepCSV_C_MC    = getHist(inFileMC,"offJets_C_matchedPFJet",     "deepcsv",binning=csvBinning,norm=0)
#pfDeepCSV_L_MC    = getHist(inFileMC,"offJets_L_matchedPFJet",     "deepcsv",binning=csvBinning,norm=0)
#
#
#drawStackCompRatio("Calo_CSV_FlavorComp",(caloCSV,"Data"),
#                   [(caloCSV_L_MC,"Light Flavor",ROOT.kAzure-9),
#                    (caloCSV_C_MC,"Charm Jets",ROOT.kGreen+1),
#                    (caloCSV_B_MC,"B Jets",ROOT.kYellow)]
#                   ,yTitle="Normalized",xTitle="Online (Calo-Jet) CSV",rTitle="Data/MC",setLogy=0,outDir=o.outDir)
#
#drawStackCompRatio("Calo_DeepCSV_FlavorComp",(caloDeepCSV,"Data"),
#                   [(caloDeepCSV_L_MC,"Light Flavor",ROOT.kAzure-9),
#                    (caloDeepCSV_C_MC,"Charm Jets",ROOT.kGreen+1),
#                    (caloDeepCSV_B_MC,"B Jets",ROOT.kYellow)]
#                   ,yTitle="Normalized",xTitle="Calo DeepCSV",rTitle="Data/MC",setLogy=0,outDir=o.outDir)
#
#
#drawStackCompRatio("PF_CSV_FlavorComp",(pfCSV,"Data"),
#                   [(pfCSV_L_MC,"Light Flavor",ROOT.kAzure-9),
#                    (pfCSV_C_MC,"Charm Jets",ROOT.kGreen+1),
#                    (pfCSV_B_MC,"B Jets",ROOT.kYellow)]
#                   ,yTitle="Normalized",xTitle="Online (PF-jet) CSV",rTitle="Data/MC",setLogy=0,outDir=o.outDir)
#
#drawStackCompRatio("PF_DeepCSV_FlavorComp",(pfDeepCSV,"Data"),
#                   [(pfDeepCSV_L_MC,"Light Flavor",ROOT.kAzure-9),
#                    (pfDeepCSV_C_MC,"Charm Jets",ROOT.kGreen+1),
#                    (pfDeepCSV_B_MC,"B Jets",ROOT.kYellow)]
#                   ,yTitle="Normalized",xTitle="PF DeepCSV",rTitle="Data/MC",setLogy=0,outDir=o.outDir)
#
#
#drawStackCompRatio("Off_CSV_FlavorComp",(offCSV_PF,"Data"),
#                   [(offCSV_L_PF_MC,"Light Flavor",ROOT.kAzure-9),
#                    (offCSV_C_PF_MC,"Charm Jets",ROOT.kGreen+1),
#                    (offCSV_B_PF_MC,"B Jets",ROOT.kYellow)]
#                   ,yTitle="Normalized",xTitle="Offline CSV",rTitle="Data/MC",setLogy=0,outDir=o.outDir)
#
#drawStackCompRatio("Off_DeepCSV_FlavorComp",(offDeepCSV_PF,"Data"),
#                   [(offDeepCSV_L_PF_MC,"Light Flavor",ROOT.kAzure-9),
#                    (offDeepCSV_C_PF_MC,"Charm Jets",ROOT.kGreen+1),
#                    (offDeepCSV_B_PF_MC,"B Jets",ROOT.kYellow)]
#                   ,yTitle="Normalized",xTitle="Offline DeepCSV",rTitle="Data/MC",setLogy=0,outDir=o.outDir)
#
#
#
##plot("pt","offJets",norm=1,logy=1)offJets_matchedPF
#
#
#makeStack("OffJet_Pt",     "pt",     "offJets_matchedPF",binning=2,xTitle="Offline Jet Pt", rTitle="Data/MC",logy=1,inFileData=inFileData,inFileMC=inFileMC,outDir=o.outDir,min=20)
#makeStack("OffJet_Eta",    "eta",    "offJets_matchedPF",binning=2,xTitle="Offline Jet Eta",rTitle="Data/MC",logy=0,inFileData=inFileData,inFileMC=inFileMC,outDir=o.outDir)
#makeStack("OffJet_Phi",    "phi",    "offJets_matchedPF",binning=2,xTitle="Offline Jet Phi",rTitle="Data/MC",logy=0,inFileData=inFileData,inFileMC=inFileMC,outDir=o.outDir)
#makeStack("OffJet_DeepCSV","deepcsv","offJets_matchedPF",binning=csvBinning,xTitle="Offline Jet DeepCSV",rTitle="Data/MC",logy=0,inFileData=inFileData,inFileMC=inFileMC,outDir=o.outDir)
#makeStack("OffJet_CSV",    "csv",    "offJets_matchedPF",binning=csvBinning,xTitle="Offline Jet CSV",rTitle="Data/MC",logy=0,inFileData=inFileData,inFileMC=inFileMC,outDir=o.outDir)
#
#
#
#ROOT.gStyle.SetPalette(1)
#oldMargin =  ROOT.gStyle.GetPadRightMargin()
#ROOT.gStyle.SetPadRightMargin(0.2)
#
#
#
##make2DComp("PFCompDeepCSV_Data",inFileData,"offJets_matchedPFJet","deepcsv_vs_matched_deepcsv",xTitle="PF Jet Deep CSV",yTitle="Offline Jet Deep CSV")
##make2DComp("PFCompDeepCSV_MC",  inFileMC  ,"offJets_matchedPFJet","deepcsv_vs_matched_deepcsv",xTitle="PF Jet Deep CSV",yTitle="Offline Jet Deep CSV")
#
#make2DComp("PFCompCSVvsDeepCSV_Data",inFileData,"offJets_matchedPFJet","csv_vs_matched_deepcsv",xTitle="Online (PF-Jet) CSV",yTitle="Offline DeepCSV",outDir=o.outDir)
#make2DComp("PFCompCSVvsDeepCSV_MC",  inFileMC,  "offJets_matchedPFJet","csv_vs_matched_deepcsv",xTitle="Online (PF-Jet) CSV",yTitle="Offline DeepCSV",outDir=o.outDir)
#
#make2DComp("PFCompCSV_Data",inFileData,"offJets_matchedPFJet","csv_vs_matched_csv",xTitle="Online (PF-Jet) CSV",yTitle="Offline CSV",outDir=o.outDir)
#make2DComp("PFCompCSV_MC",  inFileMC,  "offJets_matchedPFJet","csv_vs_matched_csv",xTitle="Online (PF-Jet) CSV",yTitle="Offline CSV",outDir=o.outDir)
#
#
##make2DComp("CaloCompDeepCSV_Data",inFileData,"offJets_matchedCaloJet","deepcsv_vs_matched_deepcsv",xTitle="Calo Jet Deep CSV",yTitle="Offline Jet Deep CSV")
##make2DComp("CaloCompDeepCSV_MC",  inFileMC,  "offJets_matchedCaloJet","deepcsv_vs_matched_deepcsv",xTitle="Calo Jet Deep CSV",yTitle="Offline Jet Deep CSV")
#
#make2DComp("CaloCompCSV_Data",inFileData,"offJets_matchedCaloJet","csv_vs_matched_csv",xTitle="Online (Calo-Jet) CSV",yTitle="Offline CSV",outDir=o.outDir)
#make2DComp("CaloCompCSV_MC",  inFileMC,  "offJets_matchedCaloJet","csv_vs_matched_csv",xTitle="Online (Calo-Jet) CSV",yTitle="Offline CSV",outDir=o.outDir)
#
#make2DComp("CaloCompCSVvsDeepCSV_Data",inFileData,"offJets_matchedCaloJet","csv_vs_matched_deepcsv",xTitle="Online (Calo-Jet) CSV",yTitle="Offline DeepCSV",outDir=o.outDir)
#make2DComp("CaloCompCSVvsDeepCSV_MC",  inFileMC,  "offJets_matchedCaloJet","csv_vs_matched_deepcsv",xTitle="Online (Calo-Jet) CSV",yTitle="Offline DeepCSV",outDir=o.outDir)
#
#
#ROOT.gStyle.SetPadRightMargin(oldMargin)
#
#
#makeInverseTurnOn("CaloCSVEffwrtOff_MC",   "csv","offJetsWORKINGPOINT_matchedCaloJet", inFileMC,  binning=1, otherText="Online Calo CSV Eff wrt Offline (MC)",outDir=o.outDir)
#makeInverseTurnOn("CaloCSVEffwrtOff_Data", "csv","offJetsWORKINGPOINT_matchedCaloJet", inFileData,binning=1, otherText="Online Calo CSV Eff wrt Offline (Data)",outDir=o.outDir)
#makeInverseTurnOnAll("CaloCSVEffwrtOff_All", "csv","offJetsWORKINGPOINT_matchedCaloJet", inFileData,"Data",inFileMC,"MC",binning=1, otherText="Online Calo CSV Eff wrt Offline",outDir=o.outDir)
#
#
#makeInverseTurnOn("PFCSVEffwrtOff_MC",   "csv","offJetsWORKINGPOINT_matchedPFJet", inFileMC,  binning=1, otherText="Online PF CSV Eff wrt Offline (MC)",outDir=o.outDir)
#makeInverseTurnOn("PFCSVEffwrtOff_Data", "csv","offJetsWORKINGPOINT_matchedPFJet", inFileData,binning=1, otherText="Online PF CSV Eff wrt Offline (Data)",outDir=o.outDir)
#makeInverseTurnOnAll("PFCSVEffwrtOff_All", "csv","offJetsWORKINGPOINT_matchedPFJet", inFileData,"Data",inFileMC,"MC",binning=1, otherText="Online PF CSV Eff wrt Offline",outDir=o.outDir)
#
#for op in ["Loose","Medium","Tight"]:
#    makeInverseTurnOnAll("CaloCSVEffwrtOff_"+op, "csv","offJetsWORKINGPOINT_matchedCaloJet", inFileData,"Data",inFileMC,"MC",binning=1, otherText="Online Calo CSV Eff wrt Offline",outDir=o.outDir,
#                         wps=[op])
#
#
#    makeInverseTurnOnAll("PFCSVEffwrtOff_"+op, "csv","offJetsWORKINGPOINT_matchedPFJet", inFileData,"Data",inFileMC,"MC",binning=1, otherText="Online PF CSV Eff wrt Offline",outDir=o.outDir,
#                         wps=[op])
#
#
