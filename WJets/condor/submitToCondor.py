#!/usr/bin/env python
############################################################################
#
# Author: Francisco Yumiceva
# yumiceva@fnal.gov
#
# Fermilab, 2007
############################################################################

"""
   Submit jobs to condor. It will create directories in the output path
   to store configuration files and log files.
   
   e.g. submitToCondor.py -c myAnalyzer.cfg -l datasets.cff -o OutputDirectory

   where myAnalyzer.cfg is the configuration file. The configuration file
   needs to be modified a bit: remove all input files in the PoolSource and add a
   keyword {FILENAME} you need also the brackets.
   
   usage: %prog
   -c, --cfg    = CFG: configuration file
   -l, --list   = LIST: file with list of files (dataset)
   -o, --output = OUTPUT: output path
   -t, --test : do not submit anything just show what I would do

   Original Author: Francisco Yumiceva (yumiceva@fnal.gov)
   Modified version: Kalanand Mishra (kalanand@fnal.gov)
"""


import os,sys
try:
    import random
except:
    # now fix PYTHONPATH for the damn cmslpc python configuration
    atemp = os.getenv("PYTHONPATH")
    atempl = atemp.split(":")
    natemp = ""
    for iatemp in atempl:
        if iatemp.find("python2.3") == -1:
            natemp = natemp + iatemp
    print "\n FIX PYTHONPATH environment variable, try:\n"
    print "setenv PYTHONPATH "+ natemp
    sys.exit()
    #os.putenv("PYTHONPATH",natemp)
    # PYTHONPATH fixed

import string, re
from time import gmtime, localtime, strftime

#_______________OPTIONS__________________________________
import optparse
USAGE = re.compile(r'(?s)\s*usage: (.*?)(\n[ \t]*\n|$)')
def nonzero(self): # will become the nonzero method of optparse.Values
    "True if options were given"
    for v in self.__dict__.itervalues():
        if v is not None: return True
    return False

optparse.Values.__nonzero__ = nonzero # dynamically fix optparse.Values

class ParsingError(Exception): pass

optionstring=""

def exit(msg=""):
    raise SystemExit(msg or optionstring.replace("%prog",sys.argv[0]))

def parse(docstring, arglist=None):
    global optionstring
    optionstring = docstring
    match = USAGE.search(optionstring)
    if not match: raise ParsingError("Cannot find the option string")
    optlines = match.group(1).splitlines()
    try:
        p = optparse.OptionParser(optlines[0])
        for line in optlines[1:]:
            opt, help=line.split(':')[:2]
            short,long=opt.split(',')[:2]
            if '=' in opt:
                action='store'
                long=long.split('=')[0]
            else:
                action='store_true'
            p.add_option(short.strip(),long.strip(),
                         action = action, help = help.strip())
    except (IndexError,ValueError):
        raise ParsingError("Cannot parse the option string correctly")
    return p.parse_args(arglist)

#______________________________________________________________________

#
# path to this scripts and the used template file:
#
TemplatesPATH =  "/uscms_data/d2/kalanand/condor/"
#
# path to the output that is created by this script
#
main_path = os.getcwd() + "/"

out_path      = main_path+"condorjobs/"

scripts_path  = main_path+"condorjobs/"
cfg_path      = scripts_path+"cfg/"
csh_path      = scripts_path+"csh/"
logs_path     = scripts_path+"logs/"


istest = 0
Nevents = "-1"

#
# Path to the input/output data:
#


cfg_tags        = ["{FILENAME}",
                   "{OUTPUT_FILENAME}"]

scripts_tags    = ["{PATHTOOUT}",
                   "{CONFIGFILE}",
                   "{OUTDIR}"]

condor_tags     = ["{EXECUTABLE}",
                   "{OUTPUT}",
                   "{ERROR}",
                   "{LOG}",
                   "{NOTIFY}"]


template_fnames = {}
template_fnames["condor"]       =  TemplatesPATH+"/template.condor"
template_fnames["csh"]          =  TemplatesPATH+"/runcms_new2.csh"


def _mkdir(newdir):
    """works the way a good mkdir should :)
        - already exists, silently complete
        - regular file in the way, raise an exception
        - parent directory(ies) does not exist, make them as well
    """
    if os.path.isdir(newdir):
        pass
    elif os.path.isfile(newdir):
        raise OSError("a file with the same name as the desired " \
                      "dir, '%s', already exists." % newdir)
    else:
        head, tail = os.path.split(newdir)
        if head and not os.path.isdir(head):
            _mkdir(head)
        if tail:
            os.mkdir(newdir)

def change(infile,outfile,changearray):
    fin  = open(infile)
    fout = open(outfile,"w")
    for line in fin.readlines():
        for change in changearray:              
            line=line.replace(change[0] ,change[1])        
        fout.write(line)


def submit_jobs(array,ini_cfgfile,launch_path, input_file, output_dir):
    
    cfgfile           = ini_cfgfile.replace("_template","")
    outfilename_log   = logs_path + input_file.replace(".DAT", ".log")
    outfilename_csh   = csh_path  + input_file.replace(".DAT", ".csh")
    outfilename_condor= csh_path  + input_file.replace(".DAT", ".condor")
                
    #
    # now create the script to process the file:
    #
    changearray    =[]
    changearray.append((scripts_tags[0],launch_path+"/"))
    changearray.append((scripts_tags[1],input_file))
    changearray.append((scripts_tags[2],output_dir))
    
    change(template_fnames["csh"],outfilename_csh,changearray)
    os.chmod(outfilename_csh,0775)
    print outfilename_csh + " has been written.\n"


    
    #
    # finally create the condor job description file:
    #
    stdout         = outfilename_log.replace(".log",".stdout")
    stderr         = outfilename_log.replace(".log",".stderr")
    logfiles       = outfilename_log.replace(".log",".log")
    changearray    =[]
    changearray.append((condor_tags[0],outfilename_csh))
    changearray.append((condor_tags[1],stdout))
    changearray.append((condor_tags[2],stderr))
    changearray.append((condor_tags[3],logfiles))
    changearray.append((condor_tags[4],os.getenv("USER")+"@fnal.gov"))
    change(template_fnames["condor"],outfilename_condor,changearray)

    print outfilename_condor + " has been written.\n"

    submitcommand ="/opt/condor/bin/condor_submit  "+outfilename_condor
    print submitcommand+"\n"
    
    if not istest:
        child   = os.popen(submitcommand)
    else:
        print "Not submitting into condor batch farm. This is only a test"
        


#________________________________________________________________    
if __name__ =='__main__':


    #import optionparse
    option,args = parse(__doc__)
    if not args and not option: exit()

    if not option.cfg:
        print " you need to provide configuration file"
        optionparse.exit()
        
    ini_cfgfile = option.cfg
    input_file = ini_cfgfile
    
    template_fnames["cfg"]          =  ini_cfgfile
                
    
    istest = option.test
    
    launch_path = "condor/"
    output_dir  = "condor/"
    
    if option.output:
        output_dir = option.output+"/"

    cfg_path  = launch_path+ "cfg/"
    csh_path  = launch_path+ "csh/"
    logs_path = launch_path+ "logs/"

    subset = []

    #make directories
    if not os.path.exists(launch_path):
        _mkdir(launch_path)
    if not os.path.exists(cfg_path):
        _mkdir(cfg_path)
    if not os.path.exists(csh_path):
        _mkdir(csh_path)
    if not os.path.exists(logs_path):
        _mkdir(logs_path)
    

    submit_jobs(subset,ini_cfgfile,launch_path, input_file, output_dir)
    



