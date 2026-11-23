CXX      = g++
# We let the ROOT version decide the C++ standard
# All source code conforms strictly to C++17 and is forward-compatible with C++20/C++23 ROOT builds.
CXXFLAGS = -O2 $(shell root-config --cflags) -I$(PYTHIA8)/include
LDFLAGS  = $(shell root-config --libs) -L$(PYTHIA8)/lib -lpythia8 -Wl,-rpath,$(PYTHIA8)/lib

generator: src/generator.cc
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)
analysis: src/analysis.cc
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)
