
# mkdir -p Rates_Eta4/
# rm Rates_Eta4/*
#
# HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/hadded/Phase2HLTTDR_QCD_Pt030to050_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o Rates_Eta4/ -n -1 --histFile qcd_30to50_eta4.root --eta 4.0
# HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/hadded/Phase2HLTTDR_QCD_Pt050to080_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o Rates_Eta4/ -n -1 --histFile qcd_50to80_eta4.root --eta 4.0
# HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/Phase2HLTTDR_QCD_Pt080to120_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o Rates_Eta4/ -n -1 --histFile qcd_80to120_eta4.root --eta 4.0
# HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/Phase2HLTTDR_QCD_Pt120to170_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o Rates_Eta4/ -n -1 --histFile qcd_120to170_eta4.root --eta 4.0
# HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/Phase2HLTTDR_QCD_Pt170to300_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o Rates_Eta4/ -n -1 --histFile qcd_170to300_eta4.root --eta 4.0
# HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/Phase2HLTTDR_QCD_Pt300to470_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o Rates_Eta4/ -n -1 --histFile qcd_300to470_eta4.root --eta 4.0
# HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/Phase2HLTTDR_QCD_Pt470to600_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o Rates_Eta4/ -n -1 --histFile qcd_470to600_eta4.root --eta 4.0
# HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/Phase2HLTTDR_QCD_Pt600toInf_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o Rates_Eta4/ -n -1 --histFile qcd_600toInf_eta4.root --eta 4.0
# HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/Phase2HLTTDR_WJetsToLNu_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o Rates_Eta4/ -n -1 --histFile wjets_eta4.root --eta 4.0
# HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/Phase2HLTTDR_DYJetsToLL_M010to050_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o Rates_Eta4/ -n -1 --histFile dy_10to50_eta4.root --eta 4.0
# HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/Phase2HLTTDR_DYJetsToLL_M050toInf_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o Rates_Eta4/ -n -1 --histFile dy_50toinf_eta4.root --eta 4.0
#
# hadd -f Rates_Eta4.root Rates_Eta4/*.root


mkdir -p Rates_Eta2p4/
rm Rates_Eta2p4/*

# HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/hadded/Phase2HLTTDR_QCD_Pt030to050_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o Rates_Eta2p4/ -n -1 --histFile qcd_30to50_eta4.root --eta 2.4
HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/hadded/Phase2HLTTDR_QCD_Pt050to080_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o Rates_Eta2p4/ -n -1 --histFile qcd_50to80_eta4.root --eta 2.4
HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/Phase2HLTTDR_QCD_Pt080to120_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o Rates_Eta2p4/ -n -1 --histFile qcd_80to120_eta4.root --eta 2.4
HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/Phase2HLTTDR_QCD_Pt120to170_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o Rates_Eta2p4/ -n -1 --histFile qcd_120to170_eta4.root --eta 2.4
HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/Phase2HLTTDR_QCD_Pt170to300_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o Rates_Eta2p4/ -n -1 --histFile qcd_170to300_eta4.root --eta 2.4
HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/Phase2HLTTDR_QCD_Pt300to470_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o Rates_Eta2p4/ -n -1 --histFile qcd_300to470_eta4.root --eta 2.4
HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/Phase2HLTTDR_QCD_Pt470to600_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o Rates_Eta2p4/ -n -1 --histFile qcd_470to600_eta4.root --eta 2.4
HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/Phase2HLTTDR_QCD_Pt600toInf_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o Rates_Eta2p4/ -n -1 --histFile qcd_600toInf_eta4.root --eta 2.4
HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/Phase2HLTTDR_WJetsToLNu_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o Rates_Eta2p4/ -n -1 --histFile wjets_eta4.root --eta 2.4
HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/Phase2HLTTDR_DYJetsToLL_M010to050_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o Rates_Eta2p4/ -n -1 --histFile dy_10to50_eta4.root --eta 2.4
HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/Phase2HLTTDR_DYJetsToLL_M050toInf_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o Rates_Eta2p4/ -n -1 --histFile dy_50toinf_eta4.root --eta 2.4

hadd -f Rates_Eta2p4.root Rates_Eta2p4/*.root


# HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/Phase2HLTTDR_HH4b_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o . -n -1 --histFile HH4b_Eta2p4.root --eta 2.4
HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/18_12_20_TDR/Online/Phase2HLTTDR_HH4b_14TeV_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o . -n -1 --histFile HH4b_Eta2p4.root --eta 2.4
# HH4bTriggerStudy TriggerStudies/NtupleAna/scripts/HH4bStudy_cfg.py --inputRAW TriggerStudies/NtupleAna/run/INPUT/07_12_20_NewTraining/Online/Phase2HLTTDR_HH4b_14TeV_PU200_PU200_HLT_TRKv06p1_TICL_cutsV2.root -o . -n -1 --histFile HH4b_Eta4.root --eta 4.0
