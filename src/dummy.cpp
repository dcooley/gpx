
#include <iostream>
#include <fstream>
#include <rapidxml.hpp>

#include "gpxsf/track/track.hpp"

#include <Rcpp.h>
using namespace Rcpp;

using namespace rapidxml;

// [[Rcpp::depends(rapidxmlr)]]

// [[Rcpp::export]]
Rcpp::NumericMatrix test() {

  xml_document<> doc;
  xml_node<> *root_node;

  std::vector< double > lons;
  std::vector< double > lats;
  std::vector< double > elev;
  std::vector< double > time;

  //std::ifstream theFile ("/Users/dave/Documents/Data/Strava/Bethan/activities/1007343724.gpx");

  std::ifstream theFile( "/Users/dave/Downloads/run.gpx");

  std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
  buffer.push_back('\0');
  // Parse the buffer using the xml file parsing library into doc
  doc.parse<0>( &buffer[0] );

  root_node = doc.first_node("gpx");

  gpxsf::track::get_track( root_node, lons, lats, elev, time );

  //gpxsf::track::string_to_datetime( time );

  int n = lons.size();

  Rcpp::NumericVector nv_lons = Rcpp::wrap( lons );
  Rcpp::NumericVector nv_lats = Rcpp::wrap( lats );
  Rcpp::NumericVector nv_elev = Rcpp::wrap( elev );
  Rcpp::NumericVector nv_time = Rcpp::wrap( time );

  Rcpp::NumericMatrix linestring( n, 4 );

  linestring(_, 0) = nv_lons;
  linestring(_, 1) = nv_lats;
  linestring(_, 2) = nv_elev;
  linestring(_, 3) = nv_time;

  linestring.attr("class") = Rcpp::CharacterVector::create("XYZM", "LINESTRING", "sfg");

  return linestring;
}
