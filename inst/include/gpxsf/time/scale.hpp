#ifndef GPXSF_TIME_SCALE_H
#define GPXSF_TIME_SCALE_H

#include <Rcpp.h>

namespace gpxsf {
namespace scale {

  inline Rcpp::NumericVector minmax( Rcpp::NumericVector& x ) {
    int n = x.size();

    double min = 0;
    double max = 0;
    bool first_value = false;

    for( int i = 0; i < n; i++) {

      if( !ISNAN( x[i] ) ) {
        if( !first_value ) {
          min = x[i];
          max = x[i];
          first_value = true;
        } else {
          if( x[i] < min ) {
            min = x[i];
          } else if ( x[i] > max) {
            max = x[i];
          }
        }
      }
    }
    return Rcpp::NumericVector::create(min, max);
  }

  // Always rescales to (0, 1)
  inline void rescale( Rcpp::NumericVector& x ) {
    int n = x.size();

    Rcpp::NumericVector rng = minmax( x );
    Rcpp::NumericVector diff_from = Rcpp::diff( rng ); // should only be one value!

    double this_diff = diff_from[0] == 0 ? 1.0 : diff_from[0];

    this_diff = 1 / this_diff;
    int i = 0;
    double this_range = rng[0];

    for (i = 0; i < n; i++) {
      x[i] = ( x[i] - this_range ) * this_diff;
    }
    //return rescaled;
  }

} // scale
} // gpxsf

#endif
