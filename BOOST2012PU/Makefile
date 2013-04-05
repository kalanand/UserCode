
GCC=$(shell which g++)

lib:
	$(GCC) -c $(shell fastjet-install/bin/fastjet-config --cxxflags) \
	$(shell root-config --cflags) \
	-I fastjet-3.0.3/include  \
	-I fastjet-3.0.3/tools  \
	-I fastjet-install/include  \
	-I /uscms_data/d2/kalanand/software/boost/include \
	-I . \
	MB_Py8.C Event.cxx RandomEngine.cxx SubtractorWithMass.cc

anal01:
	$(GCC) -I  fastjet-3.0.3/include  \
	-I fastjet-3.0.3/tools  \
	-I fastjet-install/include  \
	-I /uscms_data/d2/kalanand/software/boost/include \
	-I . \
	-L fastjet-install/lib  \
	-lgcc \
	$(shell root-config --cflags) \
	$(shell fastjet-install/bin/fastjet-config --cxxflags) \
	anal01.C MB_Py8.C Event.cxx RandomEngine.cxx  SubtractorWithMass.cc -o anal01.exe \
	$(shell  root-config --libs) $(shell fastjet-install/bin/fastjet-config --libs)

