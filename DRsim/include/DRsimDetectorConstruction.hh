#ifndef DRsimDetectorConstruction_h
#define DRsimDetectorConstruction_h 1

#include "DRsimMagneticField.hh"
#include "DRsimMaterials.hh"
#include "DRsimSiPMHit.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4Trap.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VSensitiveDetector.hh"
#include "G4VisAttributes.hh"
#include "G4GenericMessenger.hh"
#include "G4FieldManager.hh"
#include "G4ThreeVector.hh"

#include "dimensionCalc.hh"

using namespace std;

class DRsimMagneticField;

class DRsimDetectorConstruction : public G4VUserDetectorConstruction {
public:
  DRsimDetectorConstruction();
  virtual ~DRsimDetectorConstruction();

  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();

  static int fNofModules;
  static int fNofRow;
  static int fNofModules_2022_M1;
  static int fNofRow_2022_M1;
  static int fNofModules_2022_M2;
  static int fNofRow_2022_M2;

private:
  void DefineCommands();
  void DefineMaterials();
  G4Material* FindMaterial(G4String matName) { return fMaterials->GetMaterial(matName); }
  G4OpticalSurface* FindSurface(G4String surfName) { return fMaterials->GetOpticalSurface(surfName); }

  void ModuleBuild(G4LogicalVolume* ModuleLogical_[], G4LogicalVolume* PMTGLogical_[], G4LogicalVolume* PMTfilterLogical_[], G4LogicalVolume* PMTcellLogical_[], G4LogicalVolume* PMTcathLogical_[],
                    G4LogicalVolume* ReflectorMirrorLogical_[],
                    std::vector<G4LogicalVolume*> fiberUnitIntersection_[], std::vector<G4LogicalVolume*> fiberCladIntersection_[], std::vector<G4LogicalVolume*> fiberCoreIntersection_[],
                    std::vector<G4LogicalVolume*> AirGapIntersection_[], 
                    std::vector<DRsimInterface::DRsimModuleProperty>& towerProps_);

  void FiberImplement(G4int i, G4LogicalVolume* ModuleLogical__[], 
                   std::vector<G4LogicalVolume*> fiberUnitIntersection__[], std::vector<G4LogicalVolume*> fiberCladIntersection__[], std::vector<G4LogicalVolume*> fiberCoreIntersection__[],
                   std::vector<G4LogicalVolume*> AirGapIntersection__[]);

  G4bool checkOverlaps;
  G4GenericMessenger* fMessenger;
  DRsimMaterials* fMaterials;

  static G4ThreadLocal DRsimMagneticField* fMagneticField;
  static G4ThreadLocal G4FieldManager* fFieldMgr;

  G4VisAttributes* fVisAttrOrange;
  G4VisAttributes* fVisAttrOrange_PMT;
  G4VisAttributes* fVisAttrBlue;
  G4VisAttributes* fVisAttrBlue_PMT;
  G4VisAttributes* fVisAttrGray;
  G4VisAttributes* fVisAttrGreen;
  G4VisAttributes* fVisAttrInvisible;

  G4double fFrontL;
  G4double fTowerDepth;
  G4double fModuleH;
  G4double fModuleW;
  G4double fFiberUnitH;
  G4double fAirgapH;
  G4int fRandomSeed;

  G4double PMTT;
  G4double filterT;
  G4double reflectorT;

  G4bool doFiber;
  G4bool doReflector;
  G4bool doPMT;
  G4bool doAirgap;

  dimensionCalc* dimCalc;

  char name[20];
  G4String moduleName;
  G4Box* module;
  G4Box* reflector;
  G4Box* pmtg;
  G4Box* pmtcath;

  G4Box* fiberAirgap;
  G4Box* fiberUnit;
  G4Tubs* fiberClad;
  G4Tubs* fiberCladS;
  G4Tubs* fiberCoreS;
  G4Tubs* fiberCoreC;
  
  //G4VSolid* tfiberUnitIntersection;
  //G4VSolid* tfiberCladIntersection;
  //G4VSolid* tfiberCoreIntersection;
  //G4VSolid* tAirGapIntersection;

  G4LogicalVolume* ModuleLogical[600];

  G4LogicalVolume* PMTGLogical[600];
  G4LogicalVolume* PMTcathLogical[600];
  G4LogicalVolume* PMTcellLogical[600];
  G4LogicalVolume* PMTfilterLogical[600];
  G4LogicalVolume* ReflectorMirrorLogical[600];

  vector<G4LogicalVolume*> fiberUnitIntersection[600];
  vector<G4LogicalVolume*> fiberCladIntersection[600];
  vector<G4LogicalVolume*> fiberCoreIntersection[600];
  vector<G4LogicalVolume*> AirGapIntersection[600];
  vector<G4LogicalVolume*> fiberinnerCladIntersection;

  DRsimInterface::hitXY fTowerXY;
  std::vector<DRsimInterface::DRsimModuleProperty> fModuleProp;

  G4double clad_C_rMin;
  G4double clad_C_rMax;
  G4double clad_C_Dz  ;
  G4double clad_C_Sphi;
  G4double clad_C_Dphi;

  G4double core_C_rMin;
  G4double core_C_rMax;
  G4double core_C_Dz  ;
  G4double core_C_Sphi;
  G4double core_C_Dphi;

  G4double clad_S_rMin;
  G4double clad_S_rMax;
  G4double clad_S_Dz  ;
  G4double clad_S_Sphi;
  G4double clad_S_Dphi;

  G4double clad_inner_S_rMin;
  G4double clad_inner_S_rMax;
  G4double clad_inner_S_Dz  ;
  G4double clad_inner_S_Sphi;
  G4double clad_inner_S_Dphi;

  G4double core_S_rMin;
  G4double core_S_rMax;
  G4double core_S_Dz  ;
  G4double core_S_Sphi;
  G4double core_S_Dphi;

  G4double core_inner_S_rMin;
  G4double core_inner_S_rMax;
  G4double core_inner_S_Dz  ;
  G4double core_inner_S_Sphi;
  G4double core_inner_S_Dphi;

  std::vector<G4float> fFiberX;
  std::vector<G4float> fFiberY;
  std::vector<G4bool> fFiberWhich;

  G4LogicalVolume* worldLogical;

  G4String setModuleName(int i) {
    return std::to_string(i);
  }
};

#endif