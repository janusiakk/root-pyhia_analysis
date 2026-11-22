#include "Pythia8/Pythia.h"
#include "TFile.h"
#include "TTree.h"
#include <string>
#include <vector>

using namespace Pythia8;

int main(int argc, char *argv[]) {
  const std::string cfg = (argc > 1) ? argv[1] : "config1.cmnd";

  Pythia pythia;
  if (!pythia.readFile(cfg))
    return 1;
  if (!pythia.init())
    return 1;

  const int nEvents = pythia.mode("Main:numberOfEvents");

  // Recreate - overrides the output file
  TFile file("data.root", "RECREATE");
  TTree tree("events", "pp-13.6 TeV final-state particles");

  //Scope of data collection
  int evNo = 0;
  std::vector<int> id, isCharged;
  std::vector<float> px, py, pz, e, pt, eta, phi;

  tree.Branch("id", &id);
  tree.Branch("isCharged", &isCharged); //for future faster analysis
  tree.Branch("px", &px);
  tree.Branch("py", &py);
  tree.Branch("pz", &pz);
  tree.Branch("e", &e);
  tree.Branch("pt", &pt);
  tree.Branch("eta", &eta);
  tree.Branch("phi", &phi);

  for (int iEvent = 0; iEvent < nEvents; ++iEvent) {
    if (!pythia.next())
      continue;
    //Emptying vectors before next event
    id.clear();
    isCharged.clear();
    px.clear();
    py.clear();
    pz.clear();
    e.clear();
    pt.clear();
    eta.clear();
    phi.clear();
    evNo = iEvent;

    for (int i = 0; i < pythia.event.size(); ++i) {
      const Particle &p = pythia.event[i];
      if (!p.isFinal())
        continue;
      id.push_back(p.id());
      isCharged.push_back(p.isCharged() ? 1 : 0);
      px.push_back(p.px());
      py.push_back(p.py());
      pz.push_back(p.pz());
      e.push_back(p.e());
      pt.push_back(p.pT());
      eta.push_back(p.eta());
      phi.push_back(p.phi());
    }
    tree.Fill();
  }


  tree.Write();
  file.Close();
  return 0;
}
