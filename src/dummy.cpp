
#include <iostream>
#include <fstream>
#include <rapidxml.hpp>

#include "gpxsf/track/track.hpp"
#include "gpxsf/time/scale.hpp"
#include "gpxsf/time/counter.hpp"

#include "gpxsf/sf/sfg.hpp"
#include "gpxsf/sf/sfc.hpp"

#include <Rcpp.h>

using namespace Rcpp;
using namespace rapidxml;

// [[Rcpp::depends(rapidxmlr)]]

// [[Rcpp::export]]
Rcpp::List rcpp_gpx_to_sf( std::vector< std::string > gpx_files, std::string time_format ) {

  xml_document<> doc;
  xml_node<> *root_node;

  std::vector< double > lons;
  std::vector< double > lats;
  std::vector< double > elev;
  std::vector< double > time;

  int n = gpx_files.size();
  int i;
  Rcpp::List sf(1);
  Rcpp::List sfc( n );

  Rcpp::NumericVector bbox = gpxsf::sfc::start_bbox();

  for( i = 0; i < n; i++ ) {

    std::string f = gpx_files[i];
    std::ifstream theFile( f );

    std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');

    doc.parse<0>( &buffer[0] );

    root_node = doc.first_node("gpx");

    gpxsf::track::get_track( root_node, bbox, lons, lats, elev, time );

    int n = lons.size();

    Rcpp::NumericVector nv_lons = Rcpp::wrap( lons );
    Rcpp::NumericVector nv_lats = Rcpp::wrap( lats );
    Rcpp::NumericVector nv_elev = Rcpp::wrap( elev );
    Rcpp::NumericVector nv_time = Rcpp::wrap( time );

    // datetime
    // default
    if( time_format == "counter" ) {
      gpxsf::counter::counter( nv_time );

    } else if ( time_format == "normalise" ) {
      gpxsf::scale::rescale( nv_time );
    }

    Rcpp::NumericMatrix linestring( n, 4 );

    linestring(_, 0) = nv_lons;
    linestring(_, 1) = nv_lats;
    linestring(_, 2) = nv_elev;
    linestring(_, 3) = nv_time;

    linestring.attr("class") = Rcpp::CharacterVector::create("XYZM", "LINESTRING", "sfg");
    sfc[i] = linestring;
  }

  gpxsf::sfc::attach_sfc_attributes( sfc, bbox );
  sf[0] = sfc;

  // properties / column names
  std::vector< std::string >sv( 1 );
  sv[0] = "geometry";
  sf.names() = sv;

  Rcpp::IntegerVector rn = Rcpp::seq( 1, n );
  sf.attr("row.names") = rn;
  sf.attr("sf_column") = "geometry";


  sf.attr("class") = Rcpp::CharacterVector::create("sf", "data.frame");
  return sf;
}
