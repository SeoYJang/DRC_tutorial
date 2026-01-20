#include <iostream>
#include "DRsimDetectorConstruction.hh"
#include "DRsimActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4OpticalPhysics.hh"
#include "G4OpticalParameters.hh"
#include "FTFP_BERT.hh"
#include "Randomize.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

int main(int argc, char** argv) {
  // Detect interactive mode (if no arguments) and define UI session
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) ui = new G4UIExecutive(argc, argv);

  G4int seed = 0;
  G4String filename;
  if (argc > 2) seed = atoi(argv[2]);
  if (argc > 3) filename = argv[3];

  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  CLHEP::HepRandom::setTheSeed(seed);

  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;

  // Mandatory user initialization classes
  runManager->SetUserInitialization(new DRsimDetectorConstruction());

  // physics module
  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  physicsList->RegisterPhysics(opticalPhysics);

  auto* opticalParams = G4OpticalParameters::Instance();
  opticalParams->SetBoundaryInvokeSD(true);
  opticalParams->SetProcessActivation("Cerenkov",true);
  opticalParams->SetProcessActivation("Scintillation",true);
  opticalParams->SetCerenkovTrackSecondariesFirst(true);
  opticalParams->SetScintTrackSecondariesFirst(true);

  runManager->SetUserInitialization(physicsList);

  // User action initialization
  runManager->SetUserInitialization(new DRsimActionInitialization(seed,filename));

  // Visualization manager construction
  G4VisManager* visManager = new G4VisExecutive;
  if ( argc == 1 ) visManager->Initialize();
  
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if ( argc != 1 ) {
    // execute an argument macro file if exist
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  } else {
    if ( argc == 1 ) UImanager->ApplyCommand("/control/execute init_vis.mac");
    if ( !(argc == 1) ) UImanager->ApplyCommand("/control/execute init.mac");

    if (ui->IsGUI()) { UImanager->ApplyCommand("/control/execute gui.mac"); }
    // start interactive session
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete visManager;
  delete runManager;

  return 0;
}
