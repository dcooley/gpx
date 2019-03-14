#ifndef GPXSF_SFG_H
#define GPXSF_SFG_H

#include <Rcpp.h>

namespace gpxsf {
namespace sfg {

  inline Rcpp::CharacterVector sfg_attributes( std::string& dimension, std::string& geom_type ) {
    return Rcpp::CharacterVector::create( dimension, geom_type, "sfg" );
  }

} // sf
} // gpxsf

#endif
