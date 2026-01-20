#include "DRsimRunAction.hh"
#include "DRsimEventAction.hh"
#include "G4AutoLock.hh"
#include "G4Threading.hh"

#include <vector>

using namespace std;

namespace { G4Mutex DRsimRunActionMutex = G4MUTEX_INITIALIZER; }
RootInterface<DRsimInterface::DRsimEventData>* DRsimRunAction::sRootIO = 0;
int DRsimRunAction::sNumEvt = 0;

DRsimRunAction::DRsimRunAction(G4int seed, G4String filename, G4bool useHepMC)
: G4UserRunAction()
{
  fSeed = seed;
  fFilename = filename;
  fUseHepMC = useHepMC;

  G4AutoLock lock(&DRsimRunActionMutex);

  if (!sRootIO) {
    sRootIO = new RootInterface<DRsimInterface::DRsimEventData>(fFilename+"_"+std::to_string(fSeed)+".root", true);
    sRootIO->create("DRsim","DRsimEventData");
  }
}

DRsimRunAction::~DRsimRunAction() {
  if (IsMaster()) {
    G4AutoLock lock(&DRsimRunActionMutex);

    if (sRootIO) {
      sRootIO->write();
      sRootIO->close();
      delete sRootIO;
      sRootIO = 0;
    }
  }
}

void DRsimRunAction::BeginOfRunAction(const G4Run*) {

}

void DRsimRunAction::EndOfRunAction(const G4Run*) {

}
