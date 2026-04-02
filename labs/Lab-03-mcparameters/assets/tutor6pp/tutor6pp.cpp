/*****************************************************************************
 *
 * EGSnrc application tutor6pp
 *
 *****************************************************************************/

#include "egs_advanced_application.h"
#include "egs_scoring.h"
#include "egs_interface2.h"
#include "egs_input.h"

#include <fstream>
#include <iomanip>

typedef double EGS_Float;

using namespace std;


class APP_EXPORT tutor6_Application : public EGS_AdvancedApplication {

    EGS_Float        Emin, Emax, Ebin;  // spectrum minimum and maximum, and bin size
    int              nbins;             // number of bins to score spectra
    vector<int>      spectrum_regions;  // regions in which spectrum is scored
    double           Etot;              // total energy that has entered the geometry
    int              nreg;              // number of regions in the geometry

    EGS_ScoringArray *score;            // scoring array with energies deposited
    EGS_ScoringArray *spectrum;         // scoring array for spectrum data
    EGS_Float        current_weight;    // the weight of the initial particle that
                                        // is currently being simulated

public:

    // constructor
    tutor6_Application(int argc, char **argv) :
            EGS_AdvancedApplication(argc,argv), spectrum(0), score(0), nbins(100),
            nreg(0), Etot(0), current_weight(1) {}


    // destructor
    ~tutor6_Application() {
        if (score)    delete score;
        if (spectrum) delete spectrum;
    }

    // describe the application
    void describeUserCode() const;

    // initialize scoring
    int initScoring();

    // simulate a single shower
    int simulateSingleShower();

    // accumulate quantities of interest at run time
    int ausgab(int iarg);

    // output intermediate results to the .egsdat file
    int outputData();

    // read results from a .egsdat file
    int readData();

    // reset the variables used for accumulating results
    void resetCounter();

    // add simulation results
    int addState(istream &data);

    // output the results of a simulation
    void outputResults();

    // get the current simulation result
    void getCurrentResult(double &sum, double &sum2, double &norm,
            double &count);

    // write spectrum to file
    void outputResponse(EGS_ScoringArray *spec, const string& ext);

protected:

    // start a new shower
    int startNewShower();
};


// describeUserCode
void tutor6_Application::describeUserCode() const {
   egsInformation(
     "\n***************************************************"
     "\n*                                                 *"
     "\n*                     tutor6pp                    *"
     "\n*                                                 *"
     "\n***************************************************"
     "\n\n");
}


// initScoring
int tutor6_Application::initScoring() {

    // parse scoring options
    EGS_Input *options = input->takeInputItem("scoring options");
    if (!options) {
        egsFatal("ERROR: no :start scoring options: input block found.\nAborting.");
    }

    // parse spectrum input options
    options = options->takeInputItem("spectrum");
    if (!options) {
        egsFatal("ERROR: no :start spectrum: input block found in scoring options.\nAborting.");
    }

    // label of regions to score spectrum
    string mylabel;
    if (!options->getInput("label", mylabel)) {}
    else {
        egsFatal("ERROR: label = undefined.\nAborting.");
    }

    // energy window
    Emin = 0;
    Emax = source->getEmax();
    EGS_Float myE;
    if (!options->getInput("Emin", myE)) {
        Emin = myE;
    }
    if (!options->getInput("Emax", myE)) {
        Emax = myE;
    }

    // number of bins for scoring spectra
    int mybins = 100;
    if (!options->getInput("bins", mybins)) {
        nbins = mybins;
    }

    // radiative splitting
    int nbrsplit;
    if (!options->getInput("radiative splitting", nbrsplit) && nbrsplit > 1) {
        the_egsvr->nbr_split = nbrsplit;
        the_egsvr->i_play_RR = 1;
        the_egsvr->prob_RR = 1.0/nbrsplit;
    }

    if (options) delete options;

    // set energy bin size
    Ebin = (Emax-Emin)/(double)nbins;

    // allocate scoring arrays
    nreg     = geometry->regions();
    score    = new EGS_ScoringArray(nreg+2);
    spectrum = new EGS_ScoringArray(nbins);

    // get list of spectrum scoring regions by label
    geometry->getLabelRegions(mylabel, spectrum_regions);

    // return
    return 0;
}


// ausgab
int tutor6_Application::ausgab(int iarg) {

    // index of current particle and index of current region
    int np = the_stack->np - 1;
    int ir = the_stack->ir[np] - 2;

    // score energy reflected and transmitted
    if (ir < 0) {
        if (the_stack->w[np] < 0) {
            score->score(nreg,the_epcont->edep*the_stack->wt[np]);
        }
        else {
            score->score(nreg+1,the_epcont->edep*the_stack->wt[np]);
        }
    }

    // score energy deposited in regions
    else {
        if (iarg <= 4) {
            score->score(ir,the_epcont->edep*the_stack->wt[np]);
        }
    }

    return 0;
}


// simulate one shower
int tutor6_Application::simulateSingleShower() {

    // call base class function
    int err = EGS_AdvancedApplication::simulateSingleShower();

    // score the spectrum (accumulate energy deposited by this shower)
    EGS_Float myEnergy = 0.0;
    for (int k=0; k<spectrum_regions.size(); k++) {
        myEnergy += score->thisHistoryScore(spectrum_regions[k]);
    }

    // calculate spectrum energy bin, and add one count to that bin (statistically weighted)
    if (myEnergy > 1e-6) {
        int mybin = (int) (myEnergy/Emax*nbins);
        if (mybin == nbins) mybin--;
        if (mybin >= 0 && mybin < nbins) {
            spectrum->score(mybin,current_weight);
        }
    }

    return err;
}

// outputData
int tutor6_Application::outputData() {
    int err = EGS_AdvancedApplication::outputData();
    if (err) return err;
    (*data_out) << "  " << Etot << endl;
    if (!score->storeState(*data_out))  return 101;
    if (!spectrum->storeState(*data_out)) return 102;
    return 0;
}


// readData
int tutor6_Application::readData() {
    int err = EGS_AdvancedApplication::readData();
    if (err) return err;
    (*data_in) >> Etot;
    if (!score->setState(*data_in)) return 101;
    if (!spectrum->setState(*data_in)) return 102;
    return 0;
}

// resetCounter
void tutor6_Application::resetCounter() {
    EGS_AdvancedApplication::resetCounter();
    score->reset();
    spectrum->reset();
    Etot = 0;
}

// addState
int tutor6_Application::addState(istream &data) {
    int err = EGS_AdvancedApplication::addState(data);
    if (err) return err;
    double etot_tmp;
    data >> etot_tmp;
    Etot += etot_tmp;
    EGS_ScoringArray tmp_score(nreg+2);
    if (!tmp_score.setState(data)) return 101;
    (*score) += tmp_score;
    EGS_ScoringArray tmp_spectrum(nreg+2);
    if (!tmp_spectrum.setState(data)) return 102;
    (*spectrum) += tmp_spectrum;
    return 0;
}


// outputResults
void tutor6_Application::outputResults() {

    egsInformation("\n\n  last case = %d\n  Etot = %g\n", (int)current_case, Etot);

    // energy fractions
    egsInformation("\n\nEnergy fractions\n");
    egsInformation("======================================================\n");
    double f, df, rdf;
    double norm = ((double)current_case)/Etot;

    score->currentResult(nreg,f,df);
    rdf = 100;
    if (f > 0) rdf = df/f*100.0;
    egsInformation("   Reflected:   %8.6f +/- %8.6f   (%10.6f \%)\n", f*norm, df*norm, rdf);

    score->currentResult(1,f,df);
    rdf = 100;
    if (f > 0) rdf = df/f*100.0;
    egsInformation("   Deposited:   %8.6f +/- %8.6f   (%10.6f \%)\n", f*norm, df*norm, rdf);

    score->currentResult(nreg+1,f,df);
    rdf = 100;
    if (f > 0) rdf = df/f*100.0;
    egsInformation("   Transmitted: %8.6f +/- %8.6f   (%10.6f \%)\n", f*norm, df*norm, rdf);

    // spectrum
    egsInformation("\n\nSpectrum\n");
    egsInformation("======================================================\n");
    egsInformation("   spectrum scoring regions: ");
    for (int k=0; k<spectrum_regions.size(); k++) {
        egsInformation("%d ", spectrum_regions[k]);
    }
    egsInformation("\n");
    outputResponse (spectrum, "_response");
    egsInformation("\n\n");

}


// outputResponse
void tutor6_Application::outputResponse(EGS_ScoringArray *spec, const string &ext) {

    // open raw spectrum file
    fstream spectrum_file;
    string fname = output_file + ext + ".dat";
    spectrum_file.open(fname.c_str(), fstream::out);

    // print spectrum
    spectrum_file << "@xaxis  label \"energy (MeV)\"\n";
    spectrum_file << "@yaxis  label \"count per source particle\"\n";
    spectrum_file << "#" << setw(15) << "x" << setw(16) << "y" << setw(16) << "dy" << endl;
    spectrum_file << scientific;
    spectrum_file << setprecision(6);
    spectrum_file << setw(16) << Emin << setw(16) << 0 << setw(16) << 0 << endl;
    double Espectrum = 0.0;
    for(int i=0; i<nbins; i++) {
        double y, dy;
        double x = Emin + (i+1)*Ebin;
        spectrum->currentResult(i,y,dy);
        Espectrum += y*(Emin+(i+0.5)*Ebin);
        y = y;
        dy = dy;
        spectrum_file << setw(16) << x
                      << setw(16) << y
                      << setw(16) << dy
                      << endl;
    }

    // report total energy fraction recorded in spectrum
    Espectrum = Espectrum * current_case / Etot ;
    egsInformation ("   energy fraction recorded in spectrum: %g\n", Espectrum);
    egsInformation ("   spectrum data written to file %s", fname.c_str());

    // close spectrum file
    spectrum_file.close();
}


// getCurrentResult
void tutor6_Application::getCurrentResult (double &sum, double &sum2, double &norm, double &count) {
    score->currentScore(0,sum,sum2);
}


// startNewShower
int tutor6_Application::startNewShower() {
    Etot += p.E*p.wt;
    current_weight = p.wt;
    int res = EGS_Application::startNewShower();
    if (res) return res;
    if (current_case != last_case) {
        score->setHistory(current_case);
        spectrum->setHistory(current_case);
        last_case = current_case;
    }
    return 0;
}


// main
#ifdef BUILD_APP_LIB
    APP_LIB(tutor6_Application);
#else
    APP_MAIN(tutor6_Application);
#endif
