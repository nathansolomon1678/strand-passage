/* 
 * File:   bfacfProbabilitiesFromZ.cpp
 * Author: kmo
 * 
 * Created on January 19, 2013, 3:19 PM
 */

#include "bfacfProbabilitiesFromZ.h"

bfacfProbabilitiesFromZ::bfacfProbabilitiesFromZ() { }

bfacfProbabilitiesFromZ::bfacfProbabilitiesFromZ(const bfacfProbabilitiesFromZ& orig) { }

bfacfProbabilitiesFromZ::~bfacfProbabilitiesFromZ() { }

// The formulas for P(-2), P(0), & P(+2) come from equations 3.14, 3.15, & 3.16 of
// https://link.springer.com/content/pdf/10.1007/BF01013668.pdf

double bfacfProbabilitiesFromZ::p2() const {
  return z() * z() * m2();
}

double bfacfProbabilitiesFromZ::p0() const {
  // return 0.5 - p_plus2();
  return 0.5 * (p2() + m2());
}

double bfacfProbabilitiesFromZ::m2() const {
  double zSquared = z() * z();
  //   return zSquared / (1.0 + 3.0 * zSquared);
  return 1. / (1. + 3. * zSquared);
}

// IDK what these other functions are supposed to calculate

double bfacfProbabilitiesFromZ::p4p2() const
{
   return 4.0 * p2();
}

double bfacfProbabilitiesFromZ::p03p2() const
{
   return p0() + 3.0 * p2();
}

double bfacfProbabilitiesFromZ::m23p2() const
{
   return m2() + 3.0 * p2();
}

double bfacfProbabilitiesFromZ::p2p0() const
{
   return 2.0 * p0();
}

double bfacfProbabilitiesFromZ::p2p02p2() const
{
   return 2.0 * p0() + 2.0 * p2();
}

