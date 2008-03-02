#ifndef MuonIsolation_Direction_H
#define MuonIsolation_Direction_H 

/** \class muonisolation::Direction
 *  Simple eta-phi direction
 *  \author M. Zanetti
 */

#ifndef USE_MUISODEPOSIT_REQUIRED
#error THIS FILE IS NOT SUPPOSED TO BE INCLUDED: USE "DataFormats/RecoCandidate/interface/IsoDepositDirection.h" instead
#error THIS FILE IS HERE FOR BKW COMPATIBILITY ONLY
//#include "DataFormats/RecoCandidate/interface/IsoDepositDirection.h"
//namespace muonisolation { typedef reco::isodeposit::Direction Direction; }
#else

#include <cmath>
#include <sstream>
#include <iostream>

#include "PhysicsTools/Utilities/interface/deltaR.h"
#include "PhysicsTools/Utilities/interface/deltaPhi.h"


namespace muonisolation {

class  Direction {
public:

  struct Distance { 
    float deltaR; 
    float relativeAngle; 
    bool operator < (const Distance & rd2) const { return deltaR < rd2.deltaR; }; 
  };
  
  Direction(double eta = 0., double phi = 0.) : theEta(eta), thePhi(phi) {
    while( thePhi < 0.0 ) thePhi += 2*M_PI; 
    while( thePhi >= 2*M_PI ) thePhi -= 2*M_PI;
  };

  double eta() const { return theEta; }
  double phi() const { return thePhi; } 
  double theta() const { return acos(tanh(theEta)); } 

  inline bool operator==(const Direction & d2) {
    if ( this == &d2 ) return true;
    if ( deltaR(d2) < 1.e-4) return true;
    return false; 
  }

  inline double deltaR2(const Direction & dir2) const { 
    return reco::deltaR2(*this, dir2);
  } 
  inline double deltaR(const Direction & dir2) const { 
    return reco::deltaR(*this, dir2);
  } 
  
  Distance operator- (const Direction & dir2) const {
    Distance result;
    double dR    = deltaR(dir2);
    double dEta = theEta-dir2.eta();
    double dPhi = reco::deltaPhi(thePhi,dir2.phi());

    result.relativeAngle = (dR > 1.e-4) ? atan2(dPhi,dEta) : 0.;
    result.deltaR = dR;
    return result;
  }

  Direction operator+ (const Distance & relDir) const {
    double eta = theEta + relDir.deltaR*cos(relDir.relativeAngle);
    double phi = thePhi + relDir.deltaR*sin(relDir.relativeAngle);
    return Direction(eta,phi);
  }

 std::string print() const {
   std::ostringstream str;
   str<<" (Eta="<<theEta<< "," << "Phi=" <<thePhi<<")";
   return str.str();
 }

private:
  float  theEta;
  float  thePhi;
};

}

#endif //USE_MUISODEPOSIT_REQUIRED
#endif
