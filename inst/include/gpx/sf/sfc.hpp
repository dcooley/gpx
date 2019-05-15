#ifndef R_GPX_SFC_H
#define R_GPX_SFC_H

#include <Rcpp.h>
#include "gpx/gpx.hpp"

namespace gpx {
namespace sfc {

  inline Rcpp::NumericVector start_bbox() {
    Rcpp::NumericVector bbox(4);  // xmin, ymin, xmax, ymax
    bbox(0) = bbox(1) = bbox(2) = bbox(3) = NA_REAL;
    return bbox;
  }

  inline Rcpp::NumericVector start_range() {
    Rcpp::NumericVector range(2);
    range(0) = range(1) = NA_REAL;
    return range;
  }

  inline void calculate_bbox(Rcpp::NumericVector& bbox, double& lon, double& lat) {
    //xmin, ymin, xmax, ymax
    bbox[0] = std::min(lon, bbox[0]);
    bbox[2] = std::max(lon, bbox[2]);

    bbox[1] = std::min(lat, bbox[1]);
    bbox[3] = std::max(lat, bbox[3]);
  }


  inline void calculate_range(Rcpp::NumericVector& rnge, double& val) {
    //xmin, ymin, xmax, ymax
    rnge[0] = std::min(val, rnge[0]);
    rnge[1] = std::max(val, rnge[1]);
  }

  inline void attach_sfc_attributes(
      Rcpp::List& sfc,
      Rcpp::NumericVector& bbox,
      Rcpp::NumericVector& z_range,
      Rcpp::NumericVector& m_range
      ) {

    //std::string geometry_class = attach_class(sfc, type, geometry_types);
    sfc.attr("class") = Rcpp::CharacterVector::create("sfc_LINESTRING", "sfc");

    double prec = 0;

    // attribute::crs
    Rcpp::List crs = Rcpp::List::create(
      Rcpp::Named("epsg") = gpx::EPSG,
      Rcpp::Named("proj4string") = gpx::PROJ4STRING
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

    z_range.attr("class") = Rcpp::CharacterVector::create("z_range");
    z_range.attr("names") = Rcpp::CharacterVector::create("zmin","zmax");
    sfc.attr("z_range") = z_range;

    m_range.attr("class") = Rcpp::CharacterVector::create("m_range");
    m_range.attr("names") = Rcpp::CharacterVector::create("mmin","mmax");
    sfc.attr("m_range") = m_range;

  }

  inline void fetch_geometries(Rcpp::List& sf, Rcpp::List& res, int& sfg_counter) {

    std::string geom_attr;

    for (Rcpp::List::iterator it = sf.begin(); it != sf.end(); it++) {

      switch( TYPEOF(*it) ) {

      case VECSXP: {
        Rcpp::List tmp = Rcpp::as< Rcpp::List >( *it );
        if(Rf_isNull(tmp.attr("class"))) {
          fetch_geometries(tmp, res, sfg_counter);
        } else {
          res[sfg_counter] = tmp;
          sfg_counter++;
        }
        break;
      }
      case REALSXP: {
        Rcpp::NumericVector tmp = Rcpp::as< Rcpp::NumericVector >( *it );
        if(Rf_isNull(tmp.attr("class"))) {
          Rcpp::stop("Geometry could not be determined");
        } else {
          res[sfg_counter] = tmp;
          sfg_counter++;
        }
        break;
      }
      case INTSXP: {
        Rcpp::IntegerVector tmp = Rcpp::as< Rcpp::IntegerVector >( *it );
        if(Rf_isNull( tmp.attr( "class" ) ) ){
          Rcpp::stop("Geometry could not be determined");
        } else {
          res[sfg_counter] = tmp;
          sfg_counter++;
        }
        break;
      }
      case STRSXP: {
        Rcpp::StringVector tmp = Rcpp::as< Rcpp::StringVector >( *it );
        if(Rf_isNull( tmp.attr( "class" ) ) ) {
          Rcpp::stop("Geometry could not be determined");
        } else {
          res[sfg_counter] = tmp;
          sfg_counter++;
        }
        break;
      }
      default: {
        //res[0] = create_null_sfc();
        Rcpp::stop("Geometry could not be determined");
      }
      }
    }
  }

  inline Rcpp::List construct_sfc(
      int& sfg_objects,
      Rcpp::List& sf,
      Rcpp::NumericVector& bbox,
      Rcpp::NumericVector& z_range,
      Rcpp::NumericVector& m_range
    ) {

    Rcpp::List sfc_output( sfg_objects );

    int sfg_counter = 0;

    fetch_geometries( sf, sfc_output, sfg_counter );
    attach_sfc_attributes( sfc_output, bbox, z_range, m_range );

    return sfc_output;
  }


} // namespace sfc
} // namespace geojsonsf

#endif
