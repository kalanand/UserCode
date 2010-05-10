import sys
if len(sys.argv)!=3:
    print "specify file name and folder!"
    sys.exit(2)

filename=sys.argv[1]
folder  =sys.argv[2]

import ROOT

fff=ROOT.TFile(filename,"READONLY")

if fff.cd(folder)!=1:
    print "can't cd() to folder ",folder
    fff.Close()
    sys.exit(2)

ttt=ROOT.gDirectory.Get("fitter_tree")
ttt.AddFriend("tag_tree")

cond1 = "passingALL"
#cond1 = "!passing"
ttt.Draw("mass>>pass_BB(600,60,120)",cond1+" && (abs(eta)<1.5 && abs(tag_tree.eta)<1.5)","goff")
pass_BB = ROOT.gDirectory.Get("pass_BB")

ttt.Draw("mass>>pass_BE(600,60,120)",cond1+" && ((abs(eta)<1.5 && abs(tag_tree.eta)>1.5) || (abs(eta)>1.5 && abs(tag_tree.eta)<1.5))","goff")
pass_BE = ROOT.gDirectory.Get("pass_BE")

ttt.Draw("mass>>pass_EE(600,60,120)",cond1+" && (abs(eta)>1.5 && abs(tag_tree.eta)>1.5)","goff")
pass_EE = ROOT.gDirectory.Get("pass_EE")



cond1 = "!passingALL"
ttt.Draw("mass>>fail_BB(120,60,120)",cond1+" && (abs(eta)<1.5 && abs(tag_tree.eta)<1.5)","goff")
fail_BB = ROOT.gDirectory.Get("fail_BB")

ttt.Draw("mass>>fail_BE(120,60,120)",cond1+" && ((abs(eta)<1.5 && abs(tag_tree.eta)>1.5) || (abs(eta)>1.5 && abs(tag_tree.eta)<1.5))","goff")
fail_BE = ROOT.gDirectory.Get("fail_BE")

ttt.Draw("mass>>fail_EE(120,60,120)",cond1+" && (abs(eta)>1.5 && abs(tag_tree.eta)>1.5)","goff")
fail_EE = ROOT.gDirectory.Get("fail_EE")



outf = ROOT.TFile("Zlineshapes.root","RECREATE")
pass_BB.SetDirectory(outf.GetDirectory(""))
pass_BE.SetDirectory(outf.GetDirectory(""))
pass_EE.SetDirectory(outf.GetDirectory(""))
fail_BB.SetDirectory(outf.GetDirectory(""))
fail_BE.SetDirectory(outf.GetDirectory(""))
fail_EE.SetDirectory(outf.GetDirectory(""))


pass_BB.Write()
pass_BE.Write()
pass_EE.Write()
fail_BB.Write()
fail_BE.Write()
fail_EE.Write()


fff.Close()
