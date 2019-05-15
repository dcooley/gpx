#ifndef R_GPX_SFG_H
#define R_GPX_SFG_H

#include <Rcpp.h>

namespace gpx {
namespace sfg {

  inline Rcpp::CharacterVector sfg_attributes( std::string& dimension, std::string& geom_type ) {
    return Rcpp::CharacterVector::create( dimension, geom_type, "sfg" );
  }

} // sf
} // gpx

#endif
