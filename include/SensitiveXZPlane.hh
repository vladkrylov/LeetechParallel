/*
 * SensitivePlane.hh
 *
 * Represents a sensitive plane in parallel to Y axis
 * Intersections are assumed to be checked in SteppingAction
 */

#ifndef INCLUDE_SENSITIVEXZPLANE_HH_
#define INCLUDE_SENSITIVEXZPLANE_HH_

#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "globals.hh"

class G4Step;
class G4String;
class G4VPhysicalVolume;

class SensitiveXZPlane {
public:
	SensitiveXZPlane(G4String my_name, double my_xc, double my_zc, double my_length, G4LogicalVolume* motherLogic);
	virtual ~SensitiveXZPlane();

	bool Crossed(const G4Step* step);
	G4String& GetName();

private:
	G4String name;
	double xc;
	double zc;
	double halfLength;
	G4VPhysicalVolume *phys;
};

class PlaneDetectors {
public:
	PlaneDetectors();
	virtual ~PlaneDetectors();
};

inline G4String& SensitiveXZPlane::GetName() {
	return name;
}

#endif /* INCLUDE_SENSITIVEXZPLANE_HH_ */