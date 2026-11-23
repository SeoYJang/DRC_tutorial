#include "RootInterface.h"
#include "DRsimInterface.h"
#include "RecoInterface.h"

template <typename T>
RootInterface<T>::RootInterface(const std::string& filename, bool key)
: fChain(nullptr), fFile(nullptr), fTree(nullptr), fFilename(filename), fEventData(nullptr), fNumEvt(0) {
  if (key) init();
  if (!key) PrepareChain();
}

template <typename T>
RootInterface<T>::~RootInterface() {}

template <typename T>
void RootInterface<T>::PrepareChain() {
  fEventData = new T();
}

template <typename T>
void RootInterface<T>::GetChain(const std::string& treename) {
  fChain = new TChain(treename.c_str());
  fChain->Add((fFilename+"/*.root").c_str());
  fTree = fChain;
  fTree->SetBranchAddress((treename+"EventData").c_str(),&fEventData);
}

template <typename T>
void RootInterface<T>::init() {
  fEventData = new T();
  fFile = TFile::Open(fFilename.c_str(),"UPDATE");
}

template <typename T>
void RootInterface<T>::create(const std::string& name, const std::string& title) {
  fTree = new TTree(name.c_str(),name.c_str());
  fTree->Branch(title.c_str(),fEventData);
}

template <typename T>
void RootInterface<T>::set(const std::string& name, const std::string& title) {
  fTree = (TTree*)fFile->Get(name.c_str());
  fTree->SetBranchAddress(title.c_str(),&fEventData);
}

template <typename T>
void RootInterface<T>::fill(const T* evt) {
  *fEventData = *evt;
  fTree->Fill();
}

template <typename T>
void RootInterface<T>::read(T& evt) {
  fTree->GetEntry(fNumEvt);
  evt = *fEventData;
  fNumEvt++;
}

template <typename T>
void RootInterface<T>::write() {
  fFile->WriteTObject(fTree);
}

template <typename T>
void RootInterface<T>::close() {
  if (fFile) fFile->Close();
  if (fEventData) delete fEventData;
  if (fChain) delete fChain;
}

template <typename T>
TTree* RootInterface<T>::getTree() {
  return fTree;
}

template class RootInterface<DRsimInterface::DRsimEventData>;
template class RootInterface<RecoInterface::RecoEventData>;
