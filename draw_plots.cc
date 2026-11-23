#include "TFile.h"
#include <TCanvas.h>
#include <TH1D.h>
#include <TLegend.h>
#include <TROOT.h>
#include <TStyle.h>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using namespace std;
void draw_plots(const char *outDirArg = "") {
  // Output dir management
  const fs::path OutDir =
      (std::string(outDirArg) != "") ? fs::path(outDirArg) : fs::current_path();
  const fs::path pngDir = OutDir / "png";
  const fs::path pdfDir = OutDir / "pdf";

  fs::create_directories(pngDir);
  fs::create_directories(pdfDir);
  TFile *f = TFile::Open("analysis.root");
  // Render canvases offscreen
  gROOT->SetBatch(kTRUE);

  // Null ptr handling
  if (!f) {
    cerr << "Failed to open \"analysis.root\"";
    return;
  }

  // Predefined histograms
  const vector<string> species = {
      "charged", "piPlus", "piMinus", "kPlus", "kMinus", "proton", "pbar",
  };
  const vector<string> histTypes = {"hN", "hPt", "hEta", "hPhi"};

  // Removes default statbox
  gStyle->SetOptStat(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetPadBottomMargin(0.20);
  gROOT->ForceStyle();

  // Read checker for future pdf
  bool ok = 1;
  for (const string &h : histTypes) {
    for (const string &s : species) {
      const string name = h + "_" + s;
      TH1D *hist = f->Get<TH1D>(name.c_str());
      if (!hist) {
        cerr << "Histogram not found: " << name << "\n";
        ok = 0;
        continue;
      }
      // Canvas loop
      TCanvas *c = new TCanvas(("c_" + name).c_str(), name.c_str(), 1600, 900);
      // Hist title
      const string title = string(hist->GetTitle()) + " - " + h.substr(1);
      hist->SetTitle(title.c_str());
      // Log scale for
      if (h == "hN" && s == "charged")
        gPad->SetLogy();
      hist->Draw("HIST");
      c->SaveAs((pngDir / (name + ".png")).string().c_str());
      c->SaveAs((pdfDir / (name + ".pdf")).string().c_str());
      delete c;
    }
  }

  if (ok) {
    // Big pdf in current directory
    const string allPdf = (OutDir / "presentation.pdf").string();
    // Customizable for future use
    const size_t perPage = 4;
    const size_t total = species.size() * histTypes.size();

    TCanvas *big = new TCanvas("big", "all plots", 1600, 1200);
    // Open multi=page document
    big->Print((allPdf + "[").c_str());

    size_t k = 0;
    for (const string &s : species) {
      for (const string &h : histTypes) {
        if (k % perPage == 0) {
          // new slide
          big->Clear();
          big->Divide(2, 2);
        }
        big->cd(k % perPage + 1);

        if (h == "hN")
          gPad->SetLogy();
        TH1D *hist = f->Get<TH1D>((h + "_" + s).c_str());
        hist->Draw("HIST");
        ++k;
        if (k % perPage == 0 || k == total)
          big->Print(allPdf.c_str());
      }
    }
    // Close multi-page document
    big->Print((allPdf + "]").c_str());
    delete big;
  } else {
    cerr << "Some histograms were missing, skipping generation of the "
            "combined PDF\n";
  }
  f->Close();
}
