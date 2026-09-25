#include "DRsimDetectorConstruction.hh"
#include "DRsimCellParameterisation.hh"
#include "DRsimFilterParameterisation.hh"
#include "DRsimMirrorParameterisation.hh"
#include "DRsimSiPMSD.hh"

#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"

#include "G4IntersectionSolid.hh"
#include "G4SDManager.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4GeometryManager.hh"

#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"

using namespace std;

G4ThreadLocal DRsimMagneticField* DRsimDetectorConstruction::fMagneticField = 0;
G4ThreadLocal G4FieldManager* DRsimDetectorConstruction::fFieldMgr = 0;

int DRsimDetectorConstruction::fNofRow = 6;
int DRsimDetectorConstruction::fNofModules = fNofRow * fNofRow;

DRsimDetectorConstruction::DRsimDetectorConstruction()
: G4VUserDetectorConstruction(), fMessenger(0), fMaterials(NULL) {
  DefineCommands();
  DefineMaterials();

  clad_C_rMin = 0.49*mm;
  clad_C_rMax = 0.50*mm;
  clad_C_Dz   = 2.5*m;
  clad_C_Sphi = 0.;
  clad_C_Dphi = 2.*M_PI;

  core_C_rMin = 0.*mm;
  core_C_rMax = 0.49*mm;
  core_C_Dz   = 2.5*m;
  core_C_Sphi = 0.;
  core_C_Dphi = 2.*M_PI;

  clad_S_rMin = 0.48*mm;
  clad_S_rMax = 0.50*mm;
  clad_S_Dz   = 2.5*m;
  clad_S_Sphi = 0.;
  clad_S_Dphi = 2.*M_PI;

  clad_inner_S_rMin = 0.46*mm;
  clad_inner_S_rMax = 0.48*mm;
  clad_inner_S_Dz   = 2.5*m;
  clad_inner_S_Sphi = 0.;
  clad_inner_S_Dphi = 2.*M_PI;

  core_S_rMin = 0.*mm;
  core_S_rMax = 0.48*mm;
  core_S_Dz   = 2.5*m;
  core_S_Sphi = 0.;
  core_S_Dphi = 2.*M_PI;

  core_inner_S_rMin = 0.*mm;
  core_inner_S_rMax = 0.46*mm;
  core_inner_S_Dz   = 2.5*m;
  core_inner_S_Sphi = 0.;
  core_inner_S_Dphi = 2.*M_PI;

  PMTT = 0.3*mm;
  filterT = 0.01*mm;
  reflectorT = 0.03*mm;

  fVisAttrOrange = new G4VisAttributes(G4Colour(1.0,0.5,0.,1.0));
  fVisAttrOrange->SetVisibility(true);
  fVisAttrBlue = new G4VisAttributes(G4Colour(0.,0.,1.0,1.0));
  fVisAttrBlue->SetVisibility(true);
  fVisAttrGray = new G4VisAttributes(G4Colour(0.3,0.3,0.3,0.3));
  fVisAttrGray->SetVisibility(true);
  fVisAttrGreen = new G4VisAttributes(G4Colour(0.3,0.7,0.3));
  fVisAttrGreen->SetVisibility(true);
  fVisAttrInvisible = new G4VisAttributes(G4Colour(1.0,1.0,1.0,0.0));
  fVisAttrInvisible->SetVisibility(false);
}

DRsimDetectorConstruction::~DRsimDetectorConstruction() {
  delete fMessenger;
  delete fMaterials;

  delete fVisAttrOrange;
  delete fVisAttrBlue;
  delete fVisAttrGray;
  delete fVisAttrGreen;
}

void DRsimDetectorConstruction::DefineMaterials() {
  fMaterials = DRsimMaterials::GetInstance();
}

G4VPhysicalVolume* DRsimDetectorConstruction::Construct() {
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  checkOverlaps = false;

  G4VSolid* worldSolid             = new G4Box("worldBox",10.*m,10.*m,10.*m);
  worldLogical                     = new G4LogicalVolume(worldSolid,FindMaterial("G4_Galactic"),"worldLogical");
  G4VPhysicalVolume* worldPhysical = new G4PVPlacement(0,G4ThreeVector(),worldLogical,"worldPhysical",0,false,0,checkOverlaps);

  fFrontL     = 1500.;     // NOTE :: Length from the center of world box to center of module
  fTowerDepth = 2500.; 
  fModuleH    = 48;
  fModuleW    = 48;
  fAirgapH    = 1.09377;
  fFiberUnitH = 1.;

  // fRandomSeed = 1;

  doFiber     = true;
  doPMT       = true;
  doAirgap    = true;

  fiberAirgap = new G4Box("fiber_Air", (fAirgapH/2.) *mm, (fAirgapH/2.) *mm, (fTowerDepth/2) *mm);
  fiberUnit   = new G4Box("fiber_SQ", (fFiberUnitH/2) *mm, (1./2) *mm, (fTowerDepth/2) *mm);
  fiberClad   = new G4Tubs("fiberclad_C",  0, clad_C_rMax, fTowerDepth/2., 0 *deg, 360. *deg);
  fiberCladS  = new G4Tubs("fiberclad_S", 0, clad_S_rMax, fTowerDepth/2., 0 *deg, 360. *deg);
  fiberCoreC  = new G4Tubs("fiberC", 0, core_C_rMax, fTowerDepth/2., 0 *deg, 360. *deg);
  fiberCoreS  = new G4Tubs("fiberS", 0, core_S_rMax, fTowerDepth/2., 0 *deg, 360. *deg);

  dimCalc = new dimensionCalc();
  dimCalc->SetFrontL(fFrontL);
  dimCalc->SetTower_height(fTowerDepth);
  dimCalc->SetModule_height(fModuleH);
  dimCalc->SetModule_width(fModuleW);
  dimCalc->SetPMTT(PMTT+filterT);
  dimCalc->SetNofModules(fNofModules);
  dimCalc->SetNofRow(fNofRow);

  ModuleBuild(ModuleLogical,PMTGLogical,PMTfilterLogical,PMTcellLogical,PMTcathLogical,ReflectorMirrorLogical,fiberUnitIntersection,fiberCladIntersection,fiberCoreIntersection,AirGapIntersection,fModuleProp);

  delete dimCalc;
  return worldPhysical;
}

void DRsimDetectorConstruction::ConstructSDandField() {
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4String SiPMName = "SiPMSD";

  // ! Not a memory leak - SDs are deleted by G4SDManager. Deleting them manually will cause double delete!
  if ( doPMT ) {
    for (int i = 0; i < fNofModules; i++) {
      DRsimSiPMSD* SiPMSDmodule = new DRsimSiPMSD("Module"+std::to_string(i), "ModuleC"+std::to_string(i), fModuleProp.at(i));
      SDman->AddNewDetector(SiPMSDmodule);
      PMTcathLogical[i]->SetSensitiveDetector(SiPMSDmodule);
    }
  }
}

void DRsimDetectorConstruction::ModuleBuild(G4LogicalVolume* ModuleLogical_[], 
                                            G4LogicalVolume* PMTGLogical_[], G4LogicalVolume* PMTfilterLogical_[], G4LogicalVolume* PMTcellLogical_[], G4LogicalVolume* PMTcathLogical_[], 
                                            G4LogicalVolume* ReflectorMirrorLogical_[],
                                            std::vector<G4LogicalVolume*> fiberUnitIntersection_[], std::vector<G4LogicalVolume*> fiberCladIntersection_[], std::vector<G4LogicalVolume*> fiberCoreIntersection_[],
                                            std::vector<G4LogicalVolume*> AirGapIntersection_[],  
                                            std::vector<DRsimInterface::DRsimModuleProperty>& ModuleProp_) {

  for (int i = 0; i < fNofModules; i++) {    
    moduleName = setModuleName(i);
    
    dimCalc->SetisModule(true);
    module = new G4Box("Module", (fModuleH/2.) *mm, (fModuleW/2.) *mm, (fTowerDepth/2.) *mm );
    ModuleLogical_[i] = new G4LogicalVolume(module,FindMaterial("Copper"),moduleName);
    //ModuleLogical_[i] = new G4LogicalVolume(module,FindMaterial("Brass"),moduleName);
    //ModuleLogical_[i] = new G4LogicalVolume(module,FindMaterial("Lead"),moduleName);
    //ModuleLogical_[i] = new G4LogicalVolume(module,FindMaterial("Iron"),moduleName);
    //ModuleLogical_[i] = new G4LogicalVolume(module,FindMaterial("Tungsten"),moduleName);
    // G4VPhysicalVolume* modulePhysical = new G4PVPlacement(0,dimCalc->GetOrigin(i),ModuleLogical_[i],moduleName,worldLogical,false,0,checkOverlaps);
    new G4PVPlacement(0,dimCalc->GetOrigin(i),ModuleLogical_[i],moduleName,worldLogical,false,0,checkOverlaps);

    if ( doPMT ) {
      dimCalc->SetisModule(false);  
      pmtg = new G4Box("PMTG", (fModuleH/2.) *mm, (fModuleW/2.) *mm, (PMTT+filterT)/2. *mm );
      PMTGLogical_[i]  = new G4LogicalVolume(pmtg,FindMaterial("G4_AIR"),moduleName);
      new G4PVPlacement(0,dimCalc->GetOrigin_PMTG(i),PMTGLogical_[i],moduleName,worldLogical,false,0,checkOverlaps);
    }

    FiberImplement(i,ModuleLogical_,fiberUnitIntersection_,fiberCladIntersection_,fiberCoreIntersection_,AirGapIntersection_);

    DRsimInterface::DRsimModuleProperty ModulePropSingle;
    ModulePropSingle.towerXY   = fTowerXY;
    ModulePropSingle.ModuleNum = i;
    ModuleProp_.push_back(ModulePropSingle);
    ModuleLogical_[i]->SetVisAttributes(fVisAttrOrange);

    if ( doPMT ) {
      G4VSolid* SiPMlayerSolid = new G4Box("SiPMlayerSolid", (fModuleH/2.) *mm, (fModuleW/2.) *mm, (PMTT/2.) *mm );
      G4LogicalVolume* SiPMlayerLogical = new G4LogicalVolume(SiPMlayerSolid,FindMaterial("G4_AIR"),"SiPMlayerLogical");
      new G4PVPlacement(0,G4ThreeVector(0.,0.,filterT/2.),SiPMlayerLogical,"SiPMlayerPhysical",PMTGLogical_[i],false,0,checkOverlaps);

      G4VSolid* filterlayerSolid = new G4Box("filterlayerSolid", (fModuleH/2.) *mm, (fModuleW/2.) *mm, (filterT/2.) *mm );
      G4LogicalVolume* filterlayerLogical = new G4LogicalVolume(filterlayerSolid,FindMaterial("Glass"),"filterlayerLogical");
      new G4PVPlacement(0,G4ThreeVector(0.,0.,-PMTT/2.),filterlayerLogical,"filterlayerPhysical",PMTGLogical_[i],false,0,checkOverlaps);

      G4VSolid* PMTcellSolid = new G4Box("PMTcellSolid", 1.2/2. *mm, 1.2/2. *mm, PMTT/2. *mm );
      PMTcellLogical_[i] = new G4LogicalVolume(PMTcellSolid,FindMaterial("Glass"),"PMTcellLogical_");

      DRsimCellParameterisation* PMTcellParam = new DRsimCellParameterisation(fTowerXY.first,fTowerXY.second, fModuleH, fModuleW);
      G4PVParameterised* PMTcellPhysical = new G4PVParameterised("PMTcellPhysical",PMTcellLogical_[i],SiPMlayerLogical,kXAxis,fTowerXY.first*fTowerXY.second,PMTcellParam);

      G4VSolid* PMTcathSolid = new G4Box("PMTcathSolid", 1.2/2. *mm, 1.2/2. *mm, filterT/2. *mm );
      PMTcathLogical_[i] = new G4LogicalVolume(PMTcathSolid,FindMaterial("Silicon"),"PMTcathLogical_");
      new G4PVPlacement(0,G4ThreeVector(0.,0.,(PMTT-filterT)/2.*mm),PMTcathLogical_[i],"PMTcathPhysical",PMTcellLogical_[i],false,0,checkOverlaps);
      if (i==15) {
        new G4LogicalSkinSurface("Photocath_surf",PMTcathLogical_[i],FindSurface("MCPPMTSurf"));
      } else {
        new G4LogicalSkinSurface("Photocath_surf",PMTcathLogical_[i],FindSurface("PMTSurf"));
      }     

      G4VSolid* filterSolid = new G4Box("filterSolid", 1.2/2. *mm, 1.2/2. *mm, filterT/2. *mm );
      PMTfilterLogical_[i] = new G4LogicalVolume(filterSolid,FindMaterial("Gelatin"),"PMTfilterLogical_");

      DRsimFilterParameterisation* filterParam = new DRsimFilterParameterisation(fTowerXY.first,fTowerXY.second, fModuleH, fModuleW);
      G4PVParameterised* filterPhysical = new G4PVParameterised("filterPhysical",PMTfilterLogical_[i],filterlayerLogical,kXAxis,fTowerXY.first*fTowerXY.second/2,filterParam);
      new G4LogicalBorderSurface("filterSurf",filterPhysical,PMTcellPhysical,FindSurface("FilterSurf"));
          
      PMTcathLogical_[i]->SetVisAttributes(fVisAttrGreen);
      PMTfilterLogical_[i]->SetVisAttributes(fVisAttrOrange);
    }
  }
}

void DRsimDetectorConstruction::DefineCommands() {}

void DRsimDetectorConstruction::FiberImplement(G4int i, G4LogicalVolume* ModuleLogical__[], 
                                              std::vector<G4LogicalVolume*> fiberUnitIntersection__[], std::vector<G4LogicalVolume*> fiberCladIntersection__[], 
                                              std::vector<G4LogicalVolume*> fiberCoreIntersection__[], std::vector<G4LogicalVolume*> AirGapIntersection__[]) {

  fFiberX.clear();
  fFiberY.clear();
  fFiberWhich.clear();

  int NofFiber;
  int NofPlate;

  NofFiber = fModuleW*2/3;   
  NofPlate = fModuleH*2/3; 

  fTowerXY = std::make_pair(NofPlate,NofFiber);
  
  G4bool fWhich = false;  
  for (int k = 0; k < NofPlate; k++) {
    for (int j = 0; j < NofFiber; j++) { 
      /*
        ? fX : # of plate , fY : # of fiber in the plate
      */
      G4float fX;      
      G4float fY;

      fX = -fModuleH*mm/2 + k*1.5*mm + 0.75*mm;
      fY = -fModuleW*mm/2 + j*1.5*mm + 0.75*mm;

      fWhich = !fWhich;
      fFiberX.push_back(fX);
      fFiberY.push_back(fY);
      fFiberWhich.push_back(fWhich);
    }
    if ( NofFiber%2==0 ) { fWhich = !fWhich; }   
  }
  
  if ( doFiber && doAirgap) {
    for (unsigned int j = 0; j<fFiberX.size(); j++) {
      
      //tAirGapIntersection = new G4IntersectionSolid("fiberAirgap",fiberAirgap,module,0,G4ThreeVector(-fFiberX.at(j),-fFiberY.at(j),0.));
      AirGapIntersection__[i].push_back(new G4LogicalVolume(fiberAirgap,FindMaterial("G4_Galactic"),name));
      new G4PVPlacement(0,G4ThreeVector(fFiberX.at(j),fFiberY.at(j),0),AirGapIntersection__[i].at(j),name,ModuleLogical__[i],false,j,checkOverlaps);

      //AirGapIntersection__[i].at(j)->SetVisAttributes(fVisAttrGray);

      if ( !fFiberWhich.at(j) ) { //c fibre

        fiberCladIntersection__[i].push_back(new G4LogicalVolume(fiberClad,FindMaterial("FluorinatedPolymer"),name));
        new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),fiberCladIntersection__[i].at(j),name,AirGapIntersection__[i].at(j),false,j,checkOverlaps);

        fiberCoreIntersection__[i].push_back(new G4LogicalVolume(fiberCoreC,FindMaterial("PMMA"),name));
        new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),fiberCoreIntersection__[i].at(j),name,fiberCladIntersection__[i].at(j),false,j,checkOverlaps);

        //fiberCladIntersection__[i].at(j)->SetVisAttributes(fVisAttrInvisible);
        fiberCladIntersection__[i].at(j)->SetVisAttributes(fVisAttrGray);
        //fiberCoreIntersection__[i].at(j)->SetVisAttributes(fVisAttrInvisible);
        fiberCoreIntersection__[i].at(j)->SetVisAttributes(fVisAttrBlue);
      } else { // s fibre

        fiberCladIntersection__[i].push_back(new G4LogicalVolume(fiberCladS,FindMaterial("PMMA"),name));
        new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),fiberCladIntersection__[i].at(j),name,AirGapIntersection__[i].at(j),false,j,checkOverlaps);

        fiberCoreIntersection__[i].push_back(new G4LogicalVolume(fiberCoreS,FindMaterial("Polystyrene"),name));
        new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),fiberCoreIntersection__[i].at(j),name,fiberCladIntersection__[i].at(j),false,j,checkOverlaps);
      

        fiberCladIntersection__[i].at(j)->SetVisAttributes(fVisAttrGray);
        //fiberCladIntersection__[i].at(j)->SetVisAttributes(fVisAttrGray);
        fiberCoreIntersection__[i].at(j)->SetVisAttributes(fVisAttrOrange);
        //fiberCoreIntersection__[i].at(j)->SetVisAttributes(fVisAttrOrange);
      }
    }
  }
}

