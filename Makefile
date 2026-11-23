CXX      = g++
# We let the root version decide the C++ standardq:q
CXXFLAGS = -O2 $(shell root-config --cflags) -I$(PYTHIA8)/include
LDFLAGS  = $(shell root-config --libs) -L$(PYTHIA8)/lib -lpythia8 -Wl,-rpath,$(PYTHIA8)/lib

generator: src/generator.cc
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)
analysis: src/analysis.cc
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)
