#include "RecoInterface.h"
#include "DRsimMirrorParameterisation.hh"
#include "DRsimCellParameterisation.hh"

#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

DRsimMirrorParameterisation::DRsimMirrorParameterisation(const G4int numx, const G4int numy)
: G4VPVParameterisation()
{
  for ( G4int copyNo = 0; copyNo < numx*numy; copyNo++ ) {

    G4int column = copyNo / numy;
    G4int row = copyNo % numy;

    if ( RecoInterface::IsCerenkov(column,row) ) {
      fXMirror.push_back( -99.*mm/2 + column*1.5*mm + 0.75*mm );
      fYMirror.push_back( -99.*mm/2 + row*1.5*mm + 0.75*mm );
    }
  }
  fNumx = numx;
  fNumy = numy;
}

DRsimMirrorParameterisation::~DRsimMirrorParameterisation() {}

void DRsimMirrorParameterisation::ComputeTransformation(const G4int copyNo, G4VPhysicalVolume* physVol) const {
  physVol->SetTranslation(G4ThreeVector(fXMirror[copyNo],fYMirror[copyNo],0.));
}
