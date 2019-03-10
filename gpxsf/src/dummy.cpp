
#include <iostream>
#include <fstream>
#include <rapidxml.hpp>

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

  std::ifstream theFile ("/Users/dave/Documents/Data/Strava/Bethan/activities/1007343724.gpx");

  std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
  buffer.push_back('\0');
  // Parse the buffer using the xml file parsing library into doc
  doc.parse<0>(&buffer[0]);

  root_node = doc.first_node("gpx");

  // iterate over the trkPts
  for (xml_node<> *trk_node = root_node -> first_node("trk"); trk_node; trk_node = trk_node -> next_sibling()) {

    // iterate over segements
    for( xml_node<> *trk_seg_node = trk_node -> first_node("trkseg"); trk_seg_node; trk_seg_node = trk_seg_node -> next_sibling()) {

        //std::size_t t = trk_pt_node->value_size();
        //Rcpp::Rcout << "size: " << t << std::endl;

      // iterate over pts
      for( xml_node<> *trk_pt_node = trk_seg_node -> first_node("trkpt"); trk_pt_node; trk_pt_node = trk_pt_node -> next_sibling()) {

        const char* clon = trk_pt_node -> first_attribute("lon") -> value();
        const char* clat = trk_pt_node -> first_attribute("lat") -> value();

        lons.push_back( atof( clon ) );
        lats.push_back( atof( clat ) );
      }

    }
  }

  int n = lons.size();

  Rcpp::NumericVector nv_lons = Rcpp::wrap( lons );
  Rcpp::NumericVector nv_lats = Rcpp::wrap( lats );

  Rcpp::NumericMatrix linestring( n, 2 );

  linestring(_, 0) = nv_lons;
  linestring(_, 1) = nv_lats;

  linestring.attr("class") = Rcpp::CharacterVector::create("XY", "LINESTRING", "sfg");


  return linestring;
}
