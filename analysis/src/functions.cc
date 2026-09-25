#include "functions.h"
#include "TLorentzVector.h"

float functions::E_DR(float E_C, float E_S, float chi) {
  return (E_S - chi*E_C)/(1 - chi);
}

float functions::E_DR291(float E_C, float E_S) {
  float chi = 0.291;

  return (E_S - chi*E_C)/(1 - chi);
}
