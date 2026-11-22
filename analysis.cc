#include "TFile.h"
#include "TH1.h"
#include "TMath.h"
#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include <string>
#include <vector>

//Structure supporting histogram filling
struct Species {
  std::string name;
  std::string title;
  int pdg;
  int n = 0;
  TH1D *hN = nullptr, *hPt = nullptr, *hEta = nullptr, *hPhi = nullptr;
};

int main(int argc, char *argv[]) {
  const std::string inName = (argc > 1) ? argv[1] : "data.root";
  const std::string outName = (argc > 2) ? argv[2] : "analysis.root";

  // File reading, TTree atribute reading
  TFile in(inName.c_str(), "READ");

  TTreeReader reader("events", &in);
  TTreeReaderArray<int> id(reader, "id");
  TTreeReaderArray<int> isCharged(reader, "isCharged");
  TTreeReaderArray<float> pt(reader, "pt");
  TTreeReaderArray<float> eta(reader, "eta");
  TTreeReaderArray<float> phi(reader, "phi");

  // Particle types with adequate pdg codes
  // Code 0 is used as a placeholder for grouping all charged particles
  std::vector<Species> sp = {
      {"charged", "charged", 0},    {"piPlus", "#pi^{+}", 211},
      {"piMinus", "#pi^{-}", -211}, {"kPlus", "K^{+}", 321},
      {"kMinus", "K^{-}", -321},    {"proton", "p", 2212},
      {"pbar", "#bar{p}", -2212},
  };

  TFile out(outName.c_str(), "RECREATE");

  //Initialisating all 28 histograms
  for (auto &s : sp) {
    const std::string n = s.name;
    s.hN = new TH1D(("hN_" + n).c_str(), (s.title + ";N;P(N)").c_str(), 400, 0,
                    400);
    s.hPt = new TH1D(("hPt_" + n).c_str(),
                     (s.title + ";p_{T} [GeV/c];1/N_{ev} dN/dp_{T}").c_str(),
                     100, 0, 10);
    s.hEta =
        new TH1D(("hEta_" + n).c_str(),
                 (s.title + ";#eta;1/N_{ev} dN/d#eta").c_str(), 100, -5, 5);
    s.hPhi = new TH1D(("hPhi_" + n).c_str(),
                      (s.title + ";#phi [rad];1/N_{ev} dN/d#phi").c_str(), 64,
                      0, TMath::Pi());
  }

  while (reader.Next()) {
    for (Species &s : sp)
      s.n = 0; //particle counter per event
    for (size_t i = 0; i < id.GetSize(); ++i) {
      const int pid = id[i];
      for (Species &s : sp) {
        //Sorting
        const bool match = (s.pdg == 0) ? (isCharged[i] != 0) : (pid == s.pdg);
        if (!match)
          continue;
        ++s.n;
        s.hPt->Fill(pt[i]);
        s.hEta->Fill(eta[i]);
        s.hPhi->Fill(phi[i]);
      }
    }
    for (Species &s : sp)
      s.hN->Fill(s.n);
  }

  out.Write();
  out.Close();
  return 0;
}
