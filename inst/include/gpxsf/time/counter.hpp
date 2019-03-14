#ifndef GPXSF_TIME_COUNTER_H
#define GPXSF_TIME_COUNTER_H

#include <Rcpp.h>

namespace gpxsf {
namespace counter {

  inline void counter( Rcpp::NumericVector& x ) {
    int m = Rcpp::min( x );
    x = x - m;
  }

}
} // gpxsf

#endif
