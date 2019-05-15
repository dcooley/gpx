#ifndef R_GPX_TIME_COUNTER_H
#define R_GPX_TIME_COUNTER_H

#include <Rcpp.h>

namespace gpx {
namespace counter {

  inline void counter( Rcpp::NumericVector& x ) {
    int m = Rcpp::min( x );
    x = x - m;
  }

}
} // gpx

#endif
