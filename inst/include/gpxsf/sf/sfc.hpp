#ifndef GPXSF_SFC_H
#define GPXSF_SFC_H

#include <Rcpp.h>
#include "gpxsf/gpxsf.hpp"

namespace gpxsf {
namespace sfc {

  inline Rcpp::NumericVector start_bbox() {
    Rcpp::NumericVector bbox(4);  // xmin, ymin, xmax, ymax
    bbox(0) = bbox(1) = bbox(2) = bbox(3) = NA_REAL;
    return bbox;
  }

  inline void calculate_bbox(Rcpp::NumericVector& bbox, double& lon, double& lat) {
    //xmin, ymin, xmax, ymax
    bbox[0] = std::min(lon, bbox[0]);
    bbox[2] = std::max(lon, bbox[2]);

    bbox[1] = std::min(lat, bbox[1]);
    bbox[3] = std::max(lat, bbox[3]);
  }


  inline void attach_sfc_attributes(
      Rcpp::List& sfc,
      Rcpp::NumericVector& bbox
      ) {

    //std::string geometry_class = attach_class(sfc, type, geometry_types);
    sfc.attr("class") = Rcpp::CharacterVector::create("sfc_LINESTRING", "sfc");

    double prec = 0;

    // attribute::crs
    Rcpp::List crs = Rcpp::List::create(
      Rcpp::Named("epsg") = gpxsf::EPSG,
      Rcpp::Named("proj4string") = gpxsf::PROJ4STRING
    );

    crs.attr("class") = Rcpp::CharacterVector::create("crs");
    sfc.attr("crs") = crs;

    // attribute::precision
    sfc.attr("precision") = prec;

    // attribute::n_empty
    sfc.attr("n_empty") = 0;

    // attribute::bbox
    bbox.attr("class") = Rcpp::CharacterVector::create("bbox");
    bbox.attr("names") = Rcpp::CharacterVector::create("xmin", "ymin", "xmax", "ymax");
    sfc.attr("bbox") = bbox;
  }

/*
  inline Rcpp::List construct_sfc(
      int& sfg_objects,
      Rcpp::List& sf,
      Rcpp::NumericVector& bbox,
      std::unordered_set< std::string >& geometry_types,
      int& nempty
    ) {

    Rcpp::List sfc_output( sfg_objects );
    std::string geom_attr;

    int sfg_counter = 0;

    geojsonsf::sfc::utils::fetch_geometries( sf, sfc_output, sfg_counter );
    attach_sfc_attributes( sfc_output, geom_attr, bbox, geometry_types, nempty );

    return sfc_output;
  }
*/

} // namespace sfc
} // namespace geojsonsf

#endif
