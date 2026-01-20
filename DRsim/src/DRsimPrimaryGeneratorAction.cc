#include "DRsimPrimaryGeneratorAction.hh"
#include "DRsimRunAction.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4AutoLock.hh"
#include "Randomize.hh"
// #include "PhysicalConstants.h"
#include <cmath>

namespace { G4Mutex DRsimPrimaryGeneratorMutex = G4MUTEX_INITIALIZER; }
int DRsimPrimaryGeneratorAction::sNumEvt = 0;
G4ThreadLocal int DRsimPrimaryGeneratorAction::sIdxEvt = 0;

using namespace std;
DRsimPrimaryGeneratorAction::DRsimPrimaryGeneratorAction(G4int seed, G4bool useHepMC, G4bool useCalib, G4bool useGPS)
: G4VUserPrimaryGeneratorAction()
{
  fSeed = seed;
  fUseHepMC = useHepMC;
  fUseCalib = useCalib;
  fUseGPS = useGPS;

  if (!fUseHepMC) {
    if (fUseGPS) initGPS();
    else initPtcGun();
  }
}

void DRsimPrimaryGeneratorAction::initPtcGun() {
  fTheta = -0.01111;
  fPhi = 0.;
  fRandX = 10.*mm;
  fRandY = 10.*mm;
  fY_0 = 0.;
  fZ_0 = 0.;
  fParticleGun = new G4ParticleGun(1);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  fElectron = particleTable->FindParticle(particleName="e-");
  fPositron = particleTable->FindParticle(particleName="e+");
  fMuon = particleTable->FindParticle(particleName="mu+");
  fPion = particleTable->FindParticle(particleName="pi+");
  fKaon0L = particleTable->FindParticle(particleName="kaon0L");
  fProton = particleTable->FindParticle(particleName="proton");
  fOptGamma = particleTable->FindParticle(particleName="opticalphoton");

  // define commands for this class
  DefineCommands();
}

void DRsimPrimaryGeneratorAction::initGPS() {
  fGPS = new G4GeneralParticleSource();
}

DRsimPrimaryGeneratorAction::~DRsimPrimaryGeneratorAction() {
  if (!fUseHepMC) {
    if (fUseGPS) delete fGPS;
    else {
      if (fParticleGun) delete fParticleGun;
      if (fMessenger) delete fMessenger;
    }
  }
}

void DRsimPrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {

  if (fUseGPS) {
    G4AutoLock lock(&DRsimPrimaryGeneratorMutex);
    fGPS->GeneratePrimaryVertex(event);
    sIdxEvt = sNumEvt;
    sNumEvt++;

    return;
  }

  G4double x = (G4UniformRand()-0.5)*fRandX + fX_0;
  G4double y = (G4UniformRand()-0.5)*fRandY + fY_0;
  G4double z = 0;
  fOrg.set(x,y,z);

  fParticleGun->SetParticlePosition(fOrg); // http://www.apc.univ-paris7.fr/~franco/g4doxy/html/classG4VPrimaryGenerator.html

  fDirection.setREtaPhi(1.,0.,0.);
  fDirection.rotateY( -M_PI * ((90 - fTheta)/180) );
  fDirection.rotateX( M_PI * (fPhi/180.) );

  fParticleGun->SetParticleMomentumDirection(fDirection);

  G4AutoLock lock(&DRsimPrimaryGeneratorMutex);
  fParticleGun->GeneratePrimaryVertex(event);
  sIdxEvt = sNumEvt;
  sNumEvt++;
}

void DRsimPrimaryGeneratorAction::DefineCommands() {
  // Define /DRsim/generator command directory using generic messenger class
  fMessenger = new G4GenericMessenger(this, "/DRsim/generator/", "Primary generator control");

  G4GenericMessenger::Command& etaCmd = fMessenger->DeclareMethodWithUnit("theta","rad",&DRsimPrimaryGeneratorAction::SetTheta,"theta of beam");
  etaCmd.SetParameterName("theta",true);
  etaCmd.SetDefaultValue("0.");

  G4GenericMessenger::Command& phiCmd = fMessenger->DeclareMethodWithUnit("phi","rad",&DRsimPrimaryGeneratorAction::SetPhi,"phi of beam");
  phiCmd.SetParameterName("phi",true);
  phiCmd.SetDefaultValue("0.");

  G4GenericMessenger::Command& x0Cmd = fMessenger->DeclareMethodWithUnit("x0","cm",&DRsimPrimaryGeneratorAction::SetX0,"x_0 of beam");
  x0Cmd.SetParameterName("x0",true);
  x0Cmd.SetDefaultValue("0.");

  G4GenericMessenger::Command& y0Cmd = fMessenger->DeclareMethodWithUnit("y0","cm",&DRsimPrimaryGeneratorAction::SetY0,"y_0 of beam");
  y0Cmd.SetParameterName("y0",true);
  y0Cmd.SetDefaultValue("0.");

  G4GenericMessenger::Command& z0Cmd = fMessenger->DeclareMethodWithUnit("z0","cm",&DRsimPrimaryGeneratorAction::SetZ0,"z_0 of beam");
  z0Cmd.SetParameterName("z0",true);
  z0Cmd.SetDefaultValue("0.");

  G4GenericMessenger::Command& randxCmd = fMessenger->DeclareMethodWithUnit("randx","mm",&DRsimPrimaryGeneratorAction::SetRandX,"x width of beam");
  randxCmd.SetParameterName("randx",true);
  randxCmd.SetDefaultValue("0.");

  G4GenericMessenger::Command& randyCmd = fMessenger->DeclareMethodWithUnit("randy","mm",&DRsimPrimaryGeneratorAction::SetRandY,"y width of beam");
  randyCmd.SetParameterName("randy",true);
  randyCmd.SetDefaultValue("0.");
}
