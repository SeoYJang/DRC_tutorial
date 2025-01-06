#ifndef dimensionCalc_h
#define dimensionCalc_h 1

#include "G4Types.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

#include "globals.hh"
#include <string.h>
#include <vector>

using namespace std;

class dimensionCalc {
public:
  dimensionCalc();
  ~dimensionCalc();

  void SetNofModules(G4int NofMondules) { fNofModules = NofMondules; }
  void SetNofRow(G4int NofRow) { fNofRow = NofRow; }
  void SetFrontL(G4double frontL) { fFrontL = frontL; }
  void SetTower_height(G4double tower_height) { ftower_height = tower_height; }
  void SetModule_height(G4double module_height) {fmodule_height = module_height; }
  void SetModule_width(G4double module_width) {fmodule_width = module_width; }
  void SetPMTT(G4double PMTT) { fPMTT = PMTT; }
  void SetReflectorT(G4double ReflectorT) { fReflectorT = ReflectorT; }
  void SetisModule(G4bool isModule) { fisModule = isModule; }

  G4ThreeVector GetOrigin(G4int i);
  G4double GetX(G4int i);
  G4double GetY(G4int i);
  G4double GetZ(G4int i);
  G4ThreeVector GetOrigin_PMTG(G4int i);
  G4double GetX_PMTG(G4int i);
  G4double GetY_PMTG(G4int i);
  G4double GetZ_PMTG(G4int i);
  G4ThreeVector GetOrigin_Reflector(G4int i);

private:

  G4int fNofModules;
  G4int fNofRow;
  G4double ftower_front;
  G4double ftower_height;
  G4double fmodule_height;
  G4double fmodule_width;
  G4double fFrontL;
  G4double fPMTT;
  G4double fReflectorT;
  G4bool fisModule;

  G4double x,y,z;

protected:
};

#endif
