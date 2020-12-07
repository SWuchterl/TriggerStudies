import os
import subprocess
import filecmp
import glob
import multiprocessing

def subprocessWrapper(c):
    subprocess.call(c, shell=True)



INPUTPATH = "INPUT/30_11_PostWorkshop/"
OffFile_PU200 = "ttbar_PU200.root"
OffFile_NoPU = "ttbar_NoPU.root"

OnFile = "ttbar_PUSCENARIO_TRACKING_CUTSSCENARIO.root"

OutFile = "ttbar_PUSCENARIO_TRACKING_CUTSSCENARIO.root"

PU200List = [
    "ttbar_PU200_HLT_TRKv06p1_TICL_default.root",
    "ttbar_PU200_HLT_TRKv06p1_TICL_cutsV2.root",
    "ttbar_PU200_HLT_TRKv06p1_default.root",
    "ttbar_PU200_HLT_TRKv06p1_cutsV2.root",
    "ttbar_PU200_HLT_TRKv06p3_TICL_default.root",
    "ttbar_PU200_HLT_TRKv06p3_TICL_cutsV2.root",
    "ttbar_PU200_HLT_TRKv06p3_default.root",
    "ttbar_PU200_HLT_TRKv06p3_cutsV2.root",
    "ttbar_PU200_HLT_TRKv07p2_TICL_default.root",
    "ttbar_PU200_HLT_TRKv07p2_TICL_cutsV2.root",
    "ttbar_PU200_HLT_TRKv07p2_default.root",
    "ttbar_PU200_HLT_TRKv07p2_cutsV2.root",
    ]

NoPUList = [
    "ttbar_NoPU_HLT_TRKv0p1_TICL_default.root",
    "ttbar_NoPU_HLT_TRKv0p1_TICL_cutsV2.root",
    "ttbar_NoPU_HLT_TRKv0p1_default.root",
    "ttbar_NoPU_HLT_TRKv0p1_cutsV2.root",
    "ttbar_NoPU_TrackingV7p2_TICL_default.root",
    "ttbar_NoPU_TrackingV7p2_TICL_cutsV2.root",
    "ttbar_NoPU_TrackingV7p2_default.root",
    "ttbar_NoPU_TrackingV7p2_cutsV2.root",
    ]

commandList = []
for INPUTFILERAW in NoPUList:
    INPUTFILEAOD = INPUTPATH+"/Offline/"+OffFile_NoPU
    commandString = "BTagAnalyzer ../scripts/BTagAnalyzer_cfg.py --inputAOD "+INPUTFILEAOD+" --inputRAW "+INPUTPATH+"/Online/"+INPUTFILERAW+" -o $PWD -y phase2 --histogramming 1 --histFile OUTPUT/30_11_PostWorkshop/"+INPUTFILERAW+" --isMC --doTracks --doPuppiJets --nevents -1"
    # subprocess.call(commandString, shell=True)
    # commandList.append(commandString)
for INPUTFILERAW in PU200List:
    INPUTFILEAOD = INPUTPATH+"/Offline/"+OffFile_PU200
    commandString = "BTagAnalyzer ../scripts/BTagAnalyzer_cfg.py --inputAOD "+INPUTFILEAOD+" --inputRAW "+INPUTPATH+"/Online/"+INPUTFILERAW+" -o $PWD -y phase2 --histogramming 1 --histFile OUTPUT/30_11_PostWorkshop/"+INPUTFILERAW+" --isMC --doTracks --doPuppiJets --nevents -1"
    # subprocess.call(commandString, shell=True)
    commandList.append(commandString)

print commandList
print len(commandList)


p = multiprocessing.Pool(21)
p.map(subprocessWrapper, commandList)
