#ifndef AUTOCORR_H
#define	AUTOCORR_H

#include <iostream>
#include <vector>

/* This file originally contained a class that could predict the entire autocorrelation
 * function by estimating it exponential decay. However, I don't think that's necessary
 * for this project. Also, there were unit tests for the autocorrelation, which I
 * removed out of laziness, but should probably add back sometime.
 */

double autocorrelation(std::vector<double> signal, int lag);

#endif	/* AUTOCORR_H */
