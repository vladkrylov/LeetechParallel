#include <cmath>

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4VPhysicalVolume.hh"

#include "DetectorConstruction.hh"
#include "SteppingAction.hh"
#include "SensitiveXZPlane.hh"

SteppingAction::SteppingAction()
{
	DetectorConstruction* geometry = (DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction();
	detectors = geometry->GetPlaneDetectorList();

    // Get analysis manager
    analysisManager = G4AnalysisManager::Instance();
}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
    for(int i=0; i < detectors->size(); i++)
    {
		if (detectors->at(i)->Crossed(aStep)) {
			Px = aStep->GetPostStepPoint()->GetMomentum().getX();
			Py = aStep->GetPostStepPoint()->GetMomentum().getY();
			Pz = aStep->GetPostStepPoint()->GetMomentum().getZ();
			P = sqrt(Px*Px + Py*Py + Pz*Pz);

			theta = acos(Pz/P) * 180 / M_PI;

			// save the parameters of the step
			analysisManager->FillNtupleIColumn(i, BranchId=0, 1);
			analysisManager->FillNtupleIColumn(i, BranchId=1, aStep->GetTrack()->GetTrackID());
			analysisManager->FillNtupleIColumn(i, BranchId=2, aStep->GetTrack()->GetParentID());
			analysisManager->FillNtupleIColumn(i, BranchId=3, aStep->GetTrack()->GetDefinition()->GetPDGEncoding());
			analysisManager->FillNtupleDColumn(i, BranchId=4, aStep->GetTrack()->GetKineticEnergy());
			analysisManager->FillNtupleDColumn(i, BranchId=5, aStep->GetPostStepPoint()->GetGlobalTime());
			analysisManager->FillNtupleDColumn(i, BranchId=6, aStep->GetPostStepPoint()->GetPosition().getX());
			analysisManager->FillNtupleDColumn(i, BranchId=7, aStep->GetPostStepPoint()->GetPosition().getY());
			analysisManager->FillNtupleDColumn(i, BranchId=8, aStep->GetPostStepPoint()->GetPosition().getZ());
			analysisManager->FillNtupleDColumn(i, BranchId=9, Px);
			analysisManager->FillNtupleDColumn(i, BranchId=10, Py);
			analysisManager->FillNtupleDColumn(i, BranchId=11, Pz);
			analysisManager->FillNtupleDColumn(i, BranchId=12, P);
			analysisManager->FillNtupleDColumn(i, BranchId=13, theta);
			analysisManager->FillNtupleDColumn(i, BranchId=14, aStep->GetStepLength());
			analysisManager->AddNtupleRow(i);

//			aStep->GetTrack()->SetTrackStatus(fStopAndKill);
			break;
		}
    }
}

