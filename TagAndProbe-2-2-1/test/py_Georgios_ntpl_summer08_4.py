import FWCore.ParameterSet.Config as cms

from PhysicsTools.HepMCCandAlgos.genParticles_cfi import *
from Geometry.CMSCommonData.cmsIdealGeometryXML_cff import *
from Geometry.CaloEventSetup.CaloGeometry_cff import *
from Configuration.EventContent.EventContent_cff import *

process = cms.Process("ElectronEff")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("CondCore.DBCommon.CondDBSetup_cfi")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('IDEAL_V11::All')

process.source = cms.Source("PoolSource",
                            skipEvents = cms.untracked.uint32(150000),
                            fileNames = cms.untracked.vstring(
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0007/D408618C-AA00-DE11-BACE-001A64789CF0.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/04222547-2400-DE11-A852-00151715C58C.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/04A24CC8-0F00-DE11-BCF3-00161725E485.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/06C8C3FB-0F00-DE11-B092-00E08179176B.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/0840D3A9-A000-DE11-ABF8-00161725E4EB.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/085F6AB3-C200-DE11-AEBC-0013D3DE2683.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/0AB7D4CF-0F00-DE11-A046-00E081791797.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/0C99661D-1000-DE11-AFDE-0015170AE2A4.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/0E650360-3A00-DE11-8593-003048673E9C.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/0EB54E60-3A00-DE11-91AF-003048673F1E.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/0EF964A8-AB00-DE11-BB3A-00161725E522.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/10267B13-A100-DE11-AC93-00161725E504.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/1258FA50-A100-DE11-ABF9-0015170ACBD4.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/16028BC1-AB00-DE11-9883-00304866C674.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/1A58F3EA-C400-DE11-A2B6-0013D32281C5.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/1CB042D1-3500-DE11-9258-003048635C78.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/1E3AC01F-2E00-DE11-8ABB-0013D3DE269B.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/1E6E3D30-CB00-DE11-BC25-003048674010.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/22C26AC5-1000-DE11-A2A6-0013D32281BF.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/2CECEDA4-1000-DE11-9A13-0013D322812F.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/2E761120-3600-DE11-8FC7-0015170AE4EC.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/32FD7C33-3600-DE11-B206-003048673F7E.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/34567AB3-1C00-DE11-BD26-003048673F74.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/36312739-3A00-DE11-9AFE-003048674016.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/386C3923-1000-DE11-A1E7-001A64789500.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/3C7B170A-AB00-DE11-9DCE-0015170ACB38.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/3CC8FE1B-2E00-DE11-93FC-0015170AC494.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/3E385ABF-1C00-DE11-A069-00E08178C18F.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/46956B23-2400-DE11-872E-00161725E4D5.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/4CCCC0AD-0200-DE11-9DAB-00E08178C021.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/4E9A87AB-C200-DE11-B22F-0015170AC464.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/54EB6FBE-0200-DE11-846E-001A64789E04.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/5A30C179-A000-DE11-818B-0015170ACBF8.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/5A8C7B76-2E00-DE11-A124-0013D3228149.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/62208D19-AB00-DE11-98DB-001A649747B0.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/62ED761E-3600-DE11-A43E-00E08178C179.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/6A595D39-2500-DE11-9AF0-00161725E485.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/6C0C0C3B-2400-DE11-B675-00E081791805.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/78227DB0-0200-DE11-9898-00E081791753.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/7873DCBC-C200-DE11-985F-0015170AA798.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/78D58EE0-1C00-DE11-84E7-0013D3D85A8B.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/7C1C645E-2E00-DE11-A885-0015170AC468.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/7C4E2B61-C300-DE11-917C-0015170AE518.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/7C972022-C400-DE11-9261-0013D322816F.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/826CE63A-C400-DE11-987C-0015170AE340.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/86EA44FE-3900-DE11-8068-00304832691A.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/8E255818-AB00-DE11-B7D2-001A64789E28.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/9270497D-AB00-DE11-A6C0-001A64789E14.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/9A63BECB-AB00-DE11-9F91-00161725E4C1.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/9C152144-3600-DE11-A897-0015170AE790.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/9C2239AD-0200-DE11-8EE3-00E0817918B7.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/A0BF6F2F-CB00-DE11-B1CC-00E081791769.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/A0E9E523-1900-DE11-8B45-003048673EA8.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/A6271E3F-3600-DE11-AF8C-003048670B14.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/B29B7C95-AB00-DE11-BAA2-0013D3DE26D3.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/B2B52721-2E00-DE11-B88D-00161725E51C.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/B48F94DD-AA00-DE11-8762-00161725E4B5.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/B809802E-3200-DE11-B400-003048673E82.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/B8E5A253-3600-DE11-83D4-00E08178C149.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/BC505C65-AB00-DE11-A57A-0011251E30DD.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/BC678C46-2400-DE11-9507-00E08178C0CF.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/BE47877D-AB00-DE11-AA05-001A64789D98.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/BEAE6F36-CB00-DE11-BF66-003048673F9E.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/C0D1D246-2400-DE11-B372-0015170AE6E4.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/C802F930-CB00-DE11-9CC4-00304863623C.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/CA87FE75-3600-DE11-89DB-001A64789DBC.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/CAE2C09E-9F00-DE11-AE53-00304866C674.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/CCE225AA-A000-DE11-BF6C-00161725E477.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/D8343527-2500-DE11-9F91-00E08178C0CF.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/DCA7D5C1-3600-DE11-9C6D-0015170AE768.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/E037AB90-3500-DE11-B304-00E08178C13F.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/E0625D5B-1000-DE11-A3AB-0013D3DE2667.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/E0EF8AAF-AB00-DE11-A143-001A64789D0C.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/E2B17CC2-C400-DE11-9CD4-0015170AA798.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/E67EE1B5-AA00-DE11-9A44-0015170ACB68.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/EC839CBE-3100-DE11-A1B8-00161725E51C.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/F07A91AD-0200-DE11-8F11-00E0817918C5.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0008/FC5163B5-C200-DE11-8AF2-00161725E4A3.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/000B1E31-CF00-DE11-9A0A-00161725E510.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/0036A87B-CF00-DE11-BF44-0015170AB27C.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/02217669-8600-DE11-842F-0013D3DE267B.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/0285C44D-8600-DE11-AABF-0013D3541F66.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/04056181-CF00-DE11-B4C5-00E081791849.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/04B56A1B-4500-DE11-9A34-003048635E3C.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/063D3AC8-CF00-DE11-8246-0013D3DE2699.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/0814579B-CF00-DE11-BF37-0015170AE558.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/0C20F231-CF00-DE11-BCF3-0013D3541F4E.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/0E1BF20F-4500-DE11-85DD-001A64789E68.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/1207B012-D000-DE11-A7CD-003048673F08.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/14DF642D-8600-DE11-B293-0013D3542030.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/14E1E644-4500-DE11-9E9D-00E08178C121.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/188BE818-D000-DE11-8AE6-001A64789D40.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/18B33293-CF00-DE11-B760-00161725E510.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/1C298E39-4500-DE11-9632-0015170AAD90.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/200F0D03-4500-DE11-B645-00E0817918A7.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/20750E33-D000-DE11-B2E7-00E0817917F3.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/2474B915-4500-DE11-B4DD-00E08179188F.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/263C85D6-CF00-DE11-9B47-001A647894C4.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/26963A88-CD00-DE11-B2DE-0030486361DC.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/286111FE-3100-DE11-ADDE-0013D3541F20.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/28FD8D0E-4500-DE11-AF84-001A647894DC.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/2A648B20-4500-DE11-AD60-001A64789CF8.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/2EBDD92F-D000-DE11-83CC-003048670B36.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/30C93448-4500-DE11-81A6-00E0817917A3.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/32322CEC-CF00-DE11-9428-003048670B66.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/32D2BC8E-CF00-DE11-828A-001A64789E00.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/38D13EE9-CF00-DE11-A2CF-003048670ADA.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/38FEF0F5-CF00-DE11-B4F8-00E08179182B.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/3EA56ECE-3600-DE11-BC48-003048673F9C.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/3EC41C21-D000-DE11-AC7A-003048670B66.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/4A7199B9-CF00-DE11-AEBF-0013D3DE2699.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/4AD8D07F-CF00-DE11-A5A9-003048673F1E.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/4C657039-2400-DE11-8969-00E081791801.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/5097300E-4500-DE11-9013-0015170ACC24.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/5466C4D5-CF00-DE11-A364-001A64789DC8.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/5895C18C-CF00-DE11-99CC-001A64789464.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/5A6CD68A-CF00-DE11-848C-003048673EA4.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/5C275C79-9E00-DE11-8DC4-0013D3228211.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/5C2C095C-4600-DE11-A938-001A64789DDC.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/5CAC5B53-9F00-DE11-947A-0015170AD1E4.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/5CB9DA23-3E00-DE11-9842-001A64789CE4.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/5E005B7D-CF00-DE11-9E1B-00E08178C129.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/5E2A1665-D000-DE11-B147-001A64789488.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/6650E625-D000-DE11-A8F1-001A647894E0.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/66B14411-D000-DE11-AE20-00E0817917DF.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/684F90AD-CF00-DE11-BD76-001A64789E08.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/6C3C6961-CF00-DE11-89AC-00304866C51E.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/6E73FC8B-0600-DE11-96AD-00304866C53C.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/6EDC4C9F-CF00-DE11-AC40-001A64789E00.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/709ECE03-D000-DE11-AAD5-003048673E9A.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/70C8259B-D000-DE11-978E-00151715C58C.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/7406E8D3-CF00-DE11-9AD6-00E08179176F.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/76CC629F-D000-DE11-8725-0015170AE694.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/78C1D3D3-CF00-DE11-815E-00E08178C097.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/7AA57014-D000-DE11-923E-003048673F1E.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/7C713E60-CF00-DE11-A9E3-001A6478ABB4.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/84078213-4500-DE11-92C0-001A6478945C.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/840E3E60-4900-DE11-91EF-00E08179185D.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/8AE1DDA1-CF00-DE11-9EC0-00E081791893.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/948137EF-0600-DE11-8F1A-001A6478945C.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/94C9580B-8600-DE11-BEBC-0013D3228105.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/9866638F-0600-DE11-8A59-00304863623C.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/98D3118B-D000-DE11-9B64-001A64789DF4.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/9A94DEF2-0600-DE11-8D84-00E08178C091.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/9E69F847-4B00-DE11-88DD-00161725E508.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/A0460FFF-CF00-DE11-AE81-00E0817918A7.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/A65D3BAC-CF00-DE11-BD91-003048673EAA.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/A6994843-4500-DE11-B7AC-00E0817918A7.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/A83D0136-8500-DE11-8FDC-0013D3541F66.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/A8742366-2E00-DE11-A1F2-00161725E51C.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/AA12E515-4500-DE11-A678-00E08178C159.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/AE46E312-D000-DE11-85E8-003048673F9C.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/B266D438-4500-DE11-B9F3-001A64789D64.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/B420867C-CF00-DE11-86F9-003048635E2C.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/B46BFA2A-D000-DE11-9E4A-0013D3541F4E.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/B4E4556B-CF00-DE11-8E26-00E081B08BE9.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/B60730FD-CF00-DE11-93F0-00E081791849.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/B6573A44-3E00-DE11-A563-00E08178C037.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/B6BC7421-D000-DE11-A607-003048635E2C.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/B840372F-CF00-DE11-ABAD-0015170AE6C4.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/BC4B1EB0-4400-DE11-80EF-001A647894EC.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/C073FC33-CF00-DE11-BB0A-00161725E4E3.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/C2853BEA-CF00-DE11-9786-001A64787070.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/C628459E-CF00-DE11-B49F-001A64789DF4.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/CADB382E-4500-DE11-8664-00161725E465.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/CEA0CBB0-CF00-DE11-9DE4-00E08178C10F.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/D04B7E64-D000-DE11-BE7D-003048635E32.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/D0B4E88C-CF00-DE11-881D-001A64789354.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/D21834C2-CF00-DE11-BFE2-001A64789DC8.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/DEAF574A-4500-DE11-9791-001A64789DA4.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/E035435F-4900-DE11-AAB6-00E0817918A7.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/E83EF8C5-3A00-DE11-A65C-001A64789D78.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/EA183A20-4500-DE11-8549-001A64789D8C.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/EAD82431-CF00-DE11-924B-00151715C610.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/EAE8EC9E-CF00-DE11-BE40-001A64789D54.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/F0A09E7A-CF00-DE11-8F4E-003048635E2C.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/F227E39C-D000-DE11-BA8B-0015170AE63C.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/F2501122-4500-DE11-A22F-003048673E70.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0009/FAEF812A-CF00-DE11-9010-001A6478AB7C.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0010/007DE1DF-D100-DE11-90AA-001A64787060.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0010/06F0C794-D100-DE11-8213-00161725E465.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0010/0C45A59D-D100-DE11-B17E-00E081791841.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0010/6AE28E44-E600-DE11-9398-00E08178C067.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0010/8042759D-D100-DE11-9945-003048674016.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0010/90A1E3DD-D100-DE11-9F67-001A64789E4C.root',
        '/store/mc/Summer08/Zee/GEN-SIM-RECO/IDEAL_V11_redigi_v2/0010/DCFA0E8C-D000-DE11-83FA-001A64789D50.root'
        )
)


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(200000)
)

process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000



#####################################################################################
#####################################################################################
#####################################################################################
#####################################################################################

#  electron isolation  ################
# 

process.eleIsoFromDepsTk = cms.EDFilter("CandIsolatorFromDeposits",
   deposits = cms.VPSet(cms.PSet(
       mode = cms.string('sum'),
       src = cms.InputTag("eleIsoDepositTk"),
       weight = cms.string('1'),
       deltaR = cms.double(0.3),
       vetos = cms.vstring('0.015','Threshold(1.0)'),
       skipDefaultVeto = cms.bool(True)
   ))
)

process.eleIsoFromDepsEcalFromHits = cms.EDFilter("CandIsolatorFromDeposits",
   deposits = cms.VPSet(cms.PSet(
       mode = cms.string('sum'),
       src = cms.InputTag("eleIsoDepositEcalFromHits"),
       weight = cms.string('1'),
       deltaR = cms.double(0.4),      ### DR cone
       vetos = cms.vstring('EcalBarrel:0.045',   ### inner cone

                           'EcalBarrel:RectangularEtaPhiVeto(-0.02,0.02,-0.5,0.5)',
                           ## Jurassic parameters in eta and phi
                           'EcalBarrel:ThresholdFromTransverse(0.08)',   ## ET cut
                           'EcalEndcaps:ThresholdFromTransverse(0.3)',
                           'EcalEndcaps:0.070',
                           'EcalEndcaps:RectangularEtaPhiVeto(-0.02,0.02,-0.5,0.5)'),
       skipDefaultVeto = cms.bool(True)
       ))
)
process.eleIsoFromDepsHcalFromHits = cms.EDFilter("CandIsolatorFromDeposits",
   deposits = cms.VPSet(cms.PSet(
       src = cms.InputTag("eleIsoDepositHcalFromHits"),
       deltaR = cms.double(0.4),
       weight = cms.string('1'),
       vetos = cms.vstring('0.0'),
       skipDefaultVeto = cms.bool(True),
       mode = cms.string('sum')
   ))
)




#  SuperClusters  ################
# 
process.HybridSuperClusters = cms.EDProducer("ConcreteEcalCandidateProducer",
    src = cms.InputTag("correctedHybridSuperClusters"),
    particleType = cms.string('gamma')
)
process.EBSuperClusters = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("HybridSuperClusters"),
    cut = cms.string('abs( eta ) < 1.4442')
)



process.EndcapSuperClusters = cms.EDProducer("ConcreteEcalCandidateProducer",
    src = cms.InputTag("correctedMulti5x5SuperClustersWithPreshower"),
    particleType = cms.string('gamma')
)
process.EESuperClusters = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("EndcapSuperClusters"),
    cut = cms.string('abs( eta ) > 1.560 & abs( eta ) < 3.0')
)


process.allSuperClusters = cms.EDFilter("CandViewMerger",
   src = cms.VInputTag(cms.InputTag("EBSuperClusters"), cms.InputTag("EESuperClusters"))
)


#  all probes: ET > 30
process.AllProbe30 = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("allSuperClusters"),
    cut = cms.string('et  > 30.0 & ((abs( eta ) < 1.4442) | (abs( eta ) > 1.560 & abs( eta ) < 2.5))')
)


#  all probes: ET> 20
process.AllProbe20 = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("allSuperClusters"),
    cut = cms.string('et  > 20.0 & ((abs( eta ) < 1.4442) | (abs( eta ) > 1.560 & abs( eta ) < 2.5))')
)




## #  all probes: ET > 30
## process.AllProbe30 = cms.EDFilter("GenericElectronSelection",
##                                src = cms.untracked.string('pixelMatchGsfElectrons'),
##                                etMin = cms.untracked.double(30.0)
##                                )


## #  all probes: ET> 20
## process.AllProbe20 = cms.EDFilter("GenericElectronSelection",
##                                src = cms.untracked.string('pixelMatchGsfElectrons'),
##                                etMin = cms.untracked.double(20.0)
##                                )


#  ZeeOffline
process.ZeeOffline = cms.EDFilter("GenericElectronSelection",
                                  src = cms.untracked.string('pixelMatchGsfElectrons'),
                                  etMin = cms.untracked.double(20.0),
                                  tkIsoCutBarrel = cms.untracked.double(7.2),
                                  tkIsoCutEndcaps = cms.untracked.double(5.1),
                                  ecalIsoCutBarrel = cms.untracked.double(5.7),
                                  ecalIsoCutEndcaps = cms.untracked.double(5.0),
                                  hcalIsoCutBarrel  = cms.untracked.double(8.1),
                                  hcalIsoCutEndcaps = cms.untracked.double(3.4),
                                  deltaEtaCutBarrel  = cms.untracked.double(0.0071),
                                  deltaEtaCutEndcaps = cms.untracked.double(0.0066),
                                  sigmaEtaEtaCutBarrel = cms.untracked.double(0.010),
                                  sigmaEtaEtaCutEndcaps = cms.untracked.double(0.028),
                                  eleIsoTk = cms.InputTag("eleIsoFromDepsTk"),
                                  eleIsoEcal = cms.InputTag("eleIsoFromDepsEcalFromHits"),
                                  eleIsoHcal = cms.InputTag("eleIsoFromDepsHcalFromHits")
                                  )


#  ZeeOnline
process.ZeeOnline = cms.EDFilter("GenericElectronSelection",
                                 src = cms.untracked.string('pixelMatchGsfElectrons'),
                                 etMin = cms.untracked.double(20.0),
                                 requireTrigMatch = cms.untracked.bool(True),
                                 tkIsoCutBarrel = cms.untracked.double(7.2),
                                 tkIsoCutEndcaps = cms.untracked.double(5.1),
                                 ecalIsoCutBarrel = cms.untracked.double(5.7),
                                 ecalIsoCutEndcaps = cms.untracked.double(5.0),
                                 hcalIsoCutBarrel  = cms.untracked.double(8.1),
                                 hcalIsoCutEndcaps = cms.untracked.double(3.4),
                                 deltaEtaCutBarrel  = cms.untracked.double(0.0071),
                                 deltaEtaCutEndcaps = cms.untracked.double(0.0066),
                                 sigmaEtaEtaCutBarrel = cms.untracked.double(0.010),
                                 sigmaEtaEtaCutEndcaps = cms.untracked.double(0.028),
                                 eleIsoTk = cms.InputTag("eleIsoFromDepsTk"),
                                 eleIsoEcal = cms.InputTag("eleIsoFromDepsEcalFromHits"),
                                 eleIsoHcal = cms.InputTag("eleIsoFromDepsHcalFromHits")
                                 )




#  WenuOffline
process.WenuOffline = cms.EDFilter("GenericElectronSelection",
                                   src = cms.untracked.string('pixelMatchGsfElectrons'),
                                   etMin = cms.untracked.double(30.0),
                                   tkIsoCutBarrel = cms.untracked.double(2.2),
                                   tkIsoCutEndcaps = cms.untracked.double(1.1),
                                   ecalIsoCutBarrel = cms.untracked.double(4.2),
                                   ecalIsoCutEndcaps = cms.untracked.double(3.4),
                                   hcalIsoCutBarrel  = cms.untracked.double(2.0),
                                   hcalIsoCutEndcaps = cms.untracked.double(1.3),
                                   deltaEtaCutBarrel  = cms.untracked.double(0.0040),
                                   deltaEtaCutEndcaps = cms.untracked.double(0.0066),
                                   deltaPhiCutBarrel  = cms.untracked.double(0.025),
                                   deltaPhiCutEndcaps = cms.untracked.double(0.020),
                                   sigmaEtaEtaCutBarrel = cms.untracked.double(0.0099),
                                   sigmaEtaEtaCutEndcaps = cms.untracked.double(0.028),
                                   eleIsoTk = cms.InputTag("eleIsoFromDepsTk"),
                                   eleIsoEcal = cms.InputTag("eleIsoFromDepsEcalFromHits"),
                                   eleIsoHcal = cms.InputTag("eleIsoFromDepsHcalFromHits")
                                   )


#  WenuOnline
process.WenuOnline = cms.EDFilter("GenericElectronSelection",
                                  src = cms.untracked.string('pixelMatchGsfElectrons'),
                                  etMin = cms.untracked.double(30.0),
                                  requireTrigMatch = cms.untracked.bool(True),
                                  tkIsoCutBarrel = cms.untracked.double(2.2),
                                  tkIsoCutEndcaps = cms.untracked.double(1.1),
                                  ecalIsoCutBarrel = cms.untracked.double(4.2),
                                  ecalIsoCutEndcaps = cms.untracked.double(3.4),
                                  hcalIsoCutBarrel  = cms.untracked.double(2.0),
                                  hcalIsoCutEndcaps = cms.untracked.double(1.3),
                                  deltaEtaCutBarrel  = cms.untracked.double(0.0040),
                                  deltaEtaCutEndcaps = cms.untracked.double(0.0066),
                                  deltaPhiCutBarrel  = cms.untracked.double(0.025),
                                  deltaPhiCutEndcaps = cms.untracked.double(0.020),
                                  sigmaEtaEtaCutBarrel = cms.untracked.double(0.0099),
                                  sigmaEtaEtaCutEndcaps = cms.untracked.double(0.028),
                                  eleIsoTk = cms.InputTag("eleIsoFromDepsTk"),
                                  eleIsoEcal = cms.InputTag("eleIsoFromDepsEcalFromHits"),
                                  eleIsoHcal = cms.InputTag("eleIsoFromDepsHcalFromHits")
                                  )



####################################################
#  All Tag / Probe Association Maps  ###############
####################################################



process.tpMap20 = cms.EDProducer("TagProbeProducer",
                                 MassMaxCut = cms.untracked.double(120.0),
                                 TagCollection = cms.InputTag("ZeeOnline"),
                                 MassMinCut = cms.untracked.double(60.0),
                                 ProbeCollection = cms.InputTag("AllProbe20")
                                 )


process.tpMap30 = cms.EDProducer("TagProbeProducer",
                                 MassMaxCut = cms.untracked.double(120.0),
                                 TagCollection = cms.InputTag("ZeeOnline"),
                                 MassMinCut = cms.untracked.double(60.0),
                                 ProbeCollection = cms.InputTag("AllProbe30")
                                 )

process.tpMapZoff = cms.EDProducer("TagProbeProducer",
                                   MassMaxCut = cms.untracked.double(120.0),
                                   TagCollection = cms.InputTag("ZeeOnline"),
                                   MassMinCut = cms.untracked.double(60.0),
                                   ProbeCollection = cms.InputTag("ZeeOffline")
                                   )

process.tpMapWoff = cms.EDProducer("TagProbeProducer",
                                   MassMaxCut = cms.untracked.double(120.0),
                                   TagCollection = cms.InputTag("ZeeOnline"),
                                   MassMinCut = cms.untracked.double(60.0),
                                   ProbeCollection = cms.InputTag("WenuOffline")
                                   )

#
#  All Truth-matched collections  ###################
#
# find generator particles matching by DeltaR

process.WenuOfflineMatch = cms.EDFilter("MCTruthDeltaRMatcherNew",
                                        pdgId = cms.vint32(11),
                                        src = cms.InputTag("WenuOffline"),
                                        distMin = cms.double(0.3),
                                        matched = cms.InputTag("genParticles")
                                        )

process.WenuOnlineMatch = cms.EDFilter("MCTruthDeltaRMatcherNew",
                                       pdgId = cms.vint32(11),
                                       src = cms.InputTag("WenuOnline"),
                                       distMin = cms.double(0.3),
                                       matched = cms.InputTag("genParticles")
                                       )


process.ZeeOfflineMatch = cms.EDFilter("MCTruthDeltaRMatcherNew",
                                       pdgId = cms.vint32(11),
                                       src = cms.InputTag("ZeeOffline"),
                                       distMin = cms.double(0.3),
                                       matched = cms.InputTag("genParticles")
                                       )

process.ZeeOnlineMatch = cms.EDFilter("MCTruthDeltaRMatcherNew",
                                   pdgId = cms.vint32(11),
                                   src = cms.InputTag("ZeeOnline"),
                                   distMin = cms.double(0.3),
                                   matched = cms.InputTag("genParticles")
                                   )

process.probeMatchAll20 = cms.EDFilter("MCTruthDeltaRMatcherNew",
                                       pdgId = cms.vint32(11),
                                       src = cms.InputTag("AllProbe20"),
                                       distMin = cms.double(0.3),
                                       matched = cms.InputTag("genParticles")
                                       )


process.probeMatchAll30 = cms.EDFilter("MCTruthDeltaRMatcherNew",
                                       pdgId = cms.vint32(11),
                                       src = cms.InputTag("AllProbe30"),
                                       distMin = cms.double(0.3),
                                       matched = cms.InputTag("genParticles")
                                       )

###########################################################################
###########################################################################
###########################################################################
###########################################################################
###########################################################################


process.TPEdm = cms.EDProducer("TagProbeEDMNtuple",
                               allProbeTruthMatchMapTags =
                               cms.untracked.VInputTag(cms.InputTag("probeMatchAll30"),
                                                       cms.InputTag("WenuOfflineMatch"),
                                                       cms.InputTag("probeMatchAll20"),
                                                       cms.InputTag("ZeeOfflineMatch")),
                               checkExactOverlap = cms.untracked.bool(False),
                               triggerDelRMatch = cms.untracked.double(0.3),
                               triggerDelPtRelMatch = cms.untracked.double(1.0),
                               allProbeCandTags =
                               cms.untracked.VInputTag(cms.InputTag("AllProbe30"),
                                                       cms.InputTag("WenuOffline"),
                                                       cms.InputTag("AllProbe20"),
                                                       cms.InputTag("ZeeOffline")),
                               # Truth Matching tags
                               passProbeTruthMatchMapTags =
                               cms.untracked.VInputTag(cms.InputTag("WenuOfflineMatch"),
                                                       cms.InputTag("WenuOnlineMatch"),
                                                       cms.InputTag("ZeeOfflineMatch"),
                                                       cms.InputTag("ZeeOnlineMatch")),
                               # Tag & Probe Electron Candidate Collections
                               tagCandTags =
                               cms.untracked.VInputTag(cms.InputTag("ZeeOnline"),
                                                       cms.InputTag("ZeeOnline"),
                                                       cms.InputTag("ZeeOnline"),
                                                       cms.InputTag("ZeeOnline")),
                               # Tag & Probe Association Map
                               tagProbeMapTags =
                               cms.untracked.VInputTag(cms.InputTag("tpMap30"),
                                                       cms.InputTag("tpMapWoff"),
                                                       cms.InputTag("tpMap20"),
                                                       cms.InputTag("tpMapZoff")),
                               # Type of tag-probe candidates, use "Muon" or "Electron"
                               # For the moment this only affects the kind of particle
                               # used for storing MC truth information.
                               tagProbeType = cms.untracked.string('Electron'),
                               # Truth Map Tags
                               tagTruthMatchMapTags =
                               cms.untracked.VInputTag(cms.InputTag("ZeeOnlineMatch"),
                                                       cms.InputTag("ZeeOnlineMatch"),
                                                       cms.InputTag("ZeeOnlineMatch"),
                                                       cms.InputTag("ZeeOnlineMatch")),
                               # Store some generic information about the event
                               # in case we want it
                               mcParticles = cms.untracked.vint32(23, 11, 22),
                               trackTags = cms.untracked.VInputTag(cms.InputTag("generalTracks")),
                               # Pass Probe Electron Candidate Collections
                               passProbeCandTags =
                               cms.untracked.VInputTag(cms.InputTag("WenuOffline"),
                                                       cms.InputTag("WenuOnline"),
                                                       cms.InputTag("ZeeOffline"),
                                                       cms.InputTag("ZeeOnline")),
                               verticesTag = cms.untracked.InputTag("offlinePrimaryVertices"),
                               mcParents = cms.untracked.vint32(0, 0, 0),
                               BestProbeCriteria =
                               cms.untracked.vstring("OneProbe","OneProbe",
                                                     "OneProbe","OneProbe"),
                               BestProbeInvMass  =
                               cms.untracked.vdouble(91.1876,91.1876,91.1876,91.1876)
                               )



process.iso_sequence = cms.Sequence( process.eleIsoFromDepsTk *
                                     process.eleIsoFromDepsEcalFromHits *
                                     process.eleIsoFromDepsHcalFromHits)
    
process.sc_sequence = cms.Sequence( (process.HybridSuperClusters *
                                     process.EBSuperClusters +
                                     process.EndcapSuperClusters *
                                     process.EESuperClusters) *
                                    process.allSuperClusters )

process.electron_sequence = cms.Sequence( process.AllProbe20 * process.AllProbe30 * 
                                          process.ZeeOffline * process.ZeeOnline *
                                          process.WenuOffline * process.WenuOnline )

process.tpMap_sequence = cms.Sequence( process.tpMap20 + process.tpMap30 +
                                       process.tpMapWoff + process.tpMapZoff)

process.truthMatch_sequence = cms.Sequence( process.WenuOfflineMatch + 
                                            process.WenuOnlineMatch +
                                            process.ZeeOfflineMatch +
                                            process.ZeeOnlineMatch +
                                            process.probeMatchAll20 +
                                            process.probeMatchAll30 )

process.lepton_cands = cms.Sequence( process.sc_sequence * process.iso_sequence *
                                     process.electron_sequence *
                                     process.tpMap_sequence *
                                     process.truthMatch_sequence)

process.outpath = cms.OutputModule("PoolOutputModule",
                                  outputCommands = cms.untracked.vstring('drop *','keep *_TPEdm_*_*'),
                                   fileName = cms.untracked.string('/uscms_data/d2/kalanand/Summer08-Zee/Georgios/ntuple_Summer08_ridigi_4.root')
                                   )

process.p1 = cms.Path( process.lepton_cands *  process.TPEdm) 
process.the_end = cms.EndPath(process.outpath)
