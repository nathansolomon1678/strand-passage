#include "bfacfProbabilitiesFromZFixed.h"
#include "clkConformationBfacf3.h"

bfacfProbabilitiesFromZFixed::bfacfProbabilitiesFromZFixed(): zValue(DEFAULT_Z) { }

bfacfProbabilitiesFromZFixed::bfacfProbabilitiesFromZFixed(double z): zValue(z) { }

bfacfProbabilitiesFromZFixed::bfacfProbabilitiesFromZFixed(const bfacfProbabilitiesFromZ& orig): zValue(orig.z()) { }

bfacfProbabilitiesFromZFixed::~bfacfProbabilitiesFromZFixed() { }

double bfacfProbabilitiesFromZFixed::z() const
{
   return zValue;
}
