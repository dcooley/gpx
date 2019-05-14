
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
  int file_counter;
  int sfg_objects = 0;

  Rcpp::List sfc( n );
  Rcpp::List properties( n );
  Rcpp::NumericVector list_depths( n );

  Rcpp::NumericVector bbox = gpxsf::sfc::start_bbox();
  Rcpp::NumericVector z_range = gpxsf::sfc::start_range();
  Rcpp::NumericVector m_range = gpxsf::sfc::start_range();

  // keep track of which columns to include
  Rcpp::DataFrame df_cols = Rcpp::DataFrame::create(
    _["name"] = false,
    _["comment"] = false,
    _["description"] = false,
    _["source"] = false,
    _["link"] = false,
    _["number"] = false,
    _["type"] = false,
    _["geometry"] = true
  );

  // loop over each gpx file
  for( file_counter = 0; file_counter < n; file_counter++ ) {

    std::string f = gpx_files[ file_counter ];
    std::ifstream theFile( f );

    std::vector<char> buffer(
        (std::istreambuf_iterator<char>(theFile)),
        std::istreambuf_iterator<char>()
      );

    buffer.push_back('\0');

    doc.parse<0>( &buffer[0] );

    root_node = doc.first_node("gpx");

    // there can be many tracks per gpx file
    //sfc[i] = gpxsf::track::get_track( root_node, sfc, properties, file_counter, sfg_objects, bbox, z_range, m_range, time_format );
    gpxsf::track::get_track(
      root_node, sfc, properties, file_counter, sfg_objects,
      bbox, z_range, m_range, time_format, list_depths, df_cols );
  }

  //return df_cols;

  Rcpp::List res = gpxsf::sfc::construct_sfc( sfg_objects, sfc, bbox, z_range, m_range );

  //return res;

  //Rcpp::Rcout << "returned to main" << std::endl;

  // list_depths keeps track of the number of tracks / rows in each file
  //Rcpp::Rcout << "number tracks: " << list_depths << std::endl;

  int track_counter = 0;
  int element_counter = 0;
  Rcpp::StringVector track_names( sfg_objects );
  Rcpp::StringVector track_comments( sfg_objects );
  Rcpp::StringVector track_descriptions( sfg_objects );
  Rcpp::StringVector track_sources( sfg_objects );
  Rcpp::StringVector track_links( sfg_objects );
  Rcpp::NumericVector track_numbers( sfg_objects );
  Rcpp::StringVector track_types( sfg_objects );

  for( int i = 0; i < list_depths.size(); i++ ) {
    Rcpp::List this_track = properties[ track_counter ];
    Rcpp::StringVector tracks = this_track[ "name" ];
    Rcpp::StringVector comments = this_track[ "cmt" ];
    Rcpp::StringVector descriptions = this_track[ "desc" ];
    Rcpp::StringVector sources = this_track[ "src" ];
    Rcpp::StringVector links = this_track[ "link" ];
    Rcpp::NumericVector numbers = this_track[ "number" ];
    Rcpp::StringVector types = this_track[ "type" ];

    int track_elements = list_depths[ i ];
    for( int j = 0; j < track_elements; j++ ) {

      track_names[ element_counter ] = tracks[ j ];
      track_comments[ element_counter ] = comments[ j ];
      track_descriptions[ element_counter ] = descriptions[ j ];
      track_sources[ element_counter ] = sources[ j ];
      track_links[ element_counter ] = links[ j ];
      track_numbers[ element_counter ] = numbers[ j ];
      track_types[ element_counter ] = types[ j ];

      element_counter++;
    }

    track_counter++;
  }


  Rcpp::DataFrame sf = Rcpp::DataFrame::create(
    _["name"] = track_names,
    _["comment"] = track_comments,
    _["description"] = track_descriptions,
    _["source"] = track_sources,
    _["link"] = track_links,
    _["number"] = track_numbers,
    _["type"] = track_types,
    _["geometry"] = res
  );

  // TODO - remove 'property' cols which are 'false' in df_cols
  //Rcpp::LogicalVector lv = Rcpp::as< Rcpp::LogicalVector >( df_cols );
  //Rcpp::Rcout << "lv sum: " << Rcpp::sum( lv ) << std::endl;

  //return df_cols;


  sf.attr("sf_column") = "geometry";
  sf.attr("class") = Rcpp::CharacterVector::create("sf", "data.frame");
  return sf;

}
