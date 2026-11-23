# root-pythia_analysis

Generation of particle collision data in pythia with connection to the Root framework

### Requirments :
- Pythia 8.318
- gnu make
- g++ compiler of C++17
- Root 6

## SHELL :


### Build :

```bash
make generator analysis
```
### Usage :

Run all commands from this cloned repository.

```bash
# Read config1.cmnd and write to data.root
./generator

# You can specify different .cmnd file
./generator yourconfig.cmnd

# Generating root histograms from the data.root. It writes analysis.root
./analysis
# You can specify input path
./analysis yourdata.root
# Or input and output paths
./analysis yourdata.root youroutput.root

# Printing to pdf/png via root macro.
root
.x draw_plots.cc
# It creates all the histograms in ./png, ./pdf
# If it succeds it generatos presentation.pdf in ./ with all the histograms
# You can specify different output directory than the current one
root
.x draw_plots.cc("yourdirectory")

```

### Methodology notes

Histograms are divided by the number of events to give per-event densities independent of statistics and binning. `hN` uses a log y-axis because P(N) spans several orders of magnitude.

#### The repo contains example analysis.root file so the user can run it with only root framework.
