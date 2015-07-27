#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class TTree;
class TFile;
class TString;
class G4Run;
class RunActionMessenger;

struct HitData;

class RunAction : public G4UserRunAction
{
public:
	RunAction();
	virtual ~RunAction();

	void   BeginOfRunAction(const G4Run*); //set writing parameters
	void   EndOfRunAction(const G4Run*);	//write seted parameters

	TTree* tree;
	HitData HitInfo;

	void SetRootFile(G4String newValue);

private:
	TFile* hfile;
	TString _RootFile;
	RunActionMessenger* runMessenger;
	time_t systime;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

