///////////////////////////////////////////////////////////////////////
/// f(x) = [0]*((1-x/7000.+[3]*(x/7000)^2)^[1])/(x^[2])

double QCDFitUncertainty( TF1* f, TMatrixDSym COV, double x)
{
  int nRows=COV.GetNrows();
  int nColumns=COV.GetNcols();
  assert(nRows==nColumns);
  const int npar=nRows;
  double PartialDerivative[2*npar],Parameter[2*npar];
  int N = f->GetNumberFreeParameters();

  if (  N != npar)
    {
      cout<<"ERROR: wrong number of parameters !!!!"<<endl;
      return(-1);
    }  
  for(int i=0;i<npar;i++) Parameter[i] = f->GetParameter(i);
  double z = x/7000.; 
  double y = f->Eval(x);
 
  PartialDerivative[0] = y/Parameter[0];
  if(npar==4) {
    PartialDerivative[1] = y * log(1-z+Parameter[3]*z*z);
    PartialDerivative[2] = -y * log(x);
    PartialDerivative[3] = y * Parameter[1]*z*z / (1-z+Parameter[3]*z*z);
  }
  else if(npar==2) {
    PartialDerivative[1] = -y * log(x);
  }
  double sum = 0.;
  for(int i=0;i<npar;i++)
    for(int j=0;j<npar;j++)
      {
        sum+= PartialDerivative[i]*PartialDerivative[j]*COV(i,j);
      }
  
  return  sqrt(sum);
}
