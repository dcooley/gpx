
#include <iostream>
#include <fstream>
#include <rapidxml.hpp>

#include "gpxsf/track/track.hpp"

#include "gpxsf/sf/sfg.hpp"
#include "gpxsf/sf/sfc.hpp"
#include "gpxsf/sf/sf.hpp"

#include <Rcpp.h>

using namespace Rcpp;
using namespace rapidxml;


// [[Rcpp::depends(rapidxmlr)]]

// std::size_t xml_size(rapidxml::xml_node<> *trk_seg_node, const char* node) {
//   size_t counter = 0;
//   for(
//     rapidxml::xml_node<> *trk_pt_node = trk_seg_node -> first_node( node );
//     trk_pt_node;
//     trk_pt_node = trk_pt_node -> next_sibling()
//   ) {
//
//     counter++;
//   }
//   return counter;
// }

// [[Rcpp::export]]
Rcpp::List rcpp_gpx_to_sf( std::vector< std::string > gpx_files, std::string time_format ) {

  xml_document<> doc;
  xml_node<> *root_node;

  int n = gpx_files.size();
  int i;
  int sfg_objects = 0;

  Rcpp::List sf( 1 );
  Rcpp::List sfc( n );

  Rcpp::NumericVector bbox = gpxsf::sfc::start_bbox();

  for( i = 0; i < n; i++ ) {

    std::string f = gpx_files[i];
    std::ifstream theFile( f );

    std::vector<char> buffer(
        (std::istreambuf_iterator<char>(theFile)),
        std::istreambuf_iterator<char>()
      );

    buffer.push_back('\0');

    doc.parse<0>( &buffer[0] );

    root_node = doc.first_node("gpx");

    sfc[i] = gpxsf::track::get_track( root_node, sfg_objects, bbox, time_format );
  }

  Rcpp::List res = gpxsf::sfc::construct_sfc( sfg_objects, sfc, bbox );

  sf[0] = res;

  // properties / column names
  std::vector< std::string >sv( 1 );
  sv[0] = "geometry";
  sf.names() = sv;

  Rcpp::IntegerVector rn = Rcpp::seq( 1, sfg_objects );
  sf.attr("row.names") = rn;
  sf.attr("sf_column") = "geometry";
  sf.attr("class") = Rcpp::CharacterVector::create("sf", "data.frame");
  return sf;
}
