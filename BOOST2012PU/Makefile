
GCC=$(shell which g++)

all:	lib anal02lib clean

lib:
	$(GCC) -c $(shell fastjet-install/bin/fastjet-config --cxxflags) \
	$(shell root-config --cflags) \
	-I fastjet-3.0.3/include  \
	-I fastjet-3.0.3/tools  \
	-I fastjet-install/include  \
	-I /uscms_data/d2/kalanand/software/boost/include \
	-I . \
	MB_Py8.C Zprime_Py8.C Event.cxx RandomEngine.cxx Selectors.cxx SubtractorWithMass.cc 
	ar r libanal.a *.o

anal01:
	$(GCC) -I  fastjet-3.0.3/include  \
	-I fastjet-3.0.3/tools  \
	-I fastjet-install/include  \
	-I /uscms_data/d2/kalanand/software/boost/include \
	-I. \
	-L fastjet-install/lib  \
	-lgcc \
	$(shell root-config --cflags) \
	$(shell fastjet-install/bin/fastjet-config --cxxflags) \
	anal01.C MB_Py8.C Event.cxx RandomEngine.cxx  Selectors.cxx SubtractorWithMass.cc -o anal01.exe \
	$(shell  root-config --libs) $(shell fastjet-install/bin/fastjet-config --libs)

clean:
	rm *.o

anal02:
	$(GCC) -I  fastjet-3.0.3/include  \
	-I fastjet-3.0.3/tools  \
	-I fastjet-install/include  \
	-I /uscms_data/d2/kalanand/software/boost/include \
	-I. \
	-L fastjet-install/lib  \
	-lgcc \
	$(shell root-config --cflags) \
	$(shell fastjet-install/bin/fastjet-config --cxxflags) \
	anal02.C Zprime_Py8.C MB_Py8.C Event.cxx RandomEngine.cxx  Selectors.cxx SubtractorWithMass.cc -o anal02.exe \
	$(shell  root-config --libs) $(shell fastjet-install/bin/fastjet-config --libs)

anal02lib:
	$(GCC) -I  fastjet-3.0.3/include  \
	-I fastjet-3.0.3/tools  \
	-I fastjet-install/include  \
	-I /uscms_data/d2/kalanand/software/boost/include \
	-I . \
	-L fastjet-install/lib  \
	-lgcc \
	$(shell root-config --cflags) \
	$(shell fastjet-install/bin/fastjet-config --cxxflags) \
	anal02.C -o anal02.exe \
	-L. -lanal \
	$(shell  root-config --libs) $(shell fastjet-install/bin/fastjet-config --libs)

