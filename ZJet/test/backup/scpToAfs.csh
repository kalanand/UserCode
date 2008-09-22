#!/bin/tcsh 
scp Fig13*.root kalanand@flxi02.fnal.gov:public_html/Zjet/$dir/
scp Z*.root kalanand@flxi02.fnal.gov:public_html/Zjet/$dir/
scp E*.root kalanand@flxi02.fnal.gov:public_html/Zjet/$dir/
scp dR*.root kalanand@flxi02.fnal.gov:public_html/Zjet/$dir/
scp Fig13*.gif kalanand@flxi02.fnal.gov:public_html/Zjet/$dir/
scp Z*.gif kalanand@flxi02.fnal.gov:public_html/Zjet/$dir/
scp E*.gif kalanand@flxi02.fnal.gov:public_html/Zjet/$dir/
scp dR*.gif kalanand@flxi02.fnal.gov:public_html/Zjet/$dir/
scp Fig13*.eps kalanand@flxi02.fnal.gov:public_html/Zjet/$dir/
scp Z*.eps kalanand@flxi02.fnal.gov:public_html/Zjet/$dir/
scp E*.eps kalanand@flxi02.fnal.gov:public_html/Zjet/$dir/
scp dR*.eps kalanand@flxi02.fnal.gov:public_html/Zjet/$dir/

rm Fig13*.root
rm Z*.root
rm E*.root
rm dR*.root
rm Fig13*.gif
rm Z*.gif
rm E*.gif
rm dR*.gif
rm Fig13*.eps
rm Z*.eps
rm E*.eps
rm dR*.eps
