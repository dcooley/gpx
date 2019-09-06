
#include <iostream>
#include <fstream>
#include <rapidxml.hpp>

#include "gpx/track/track.hpp"

// #include "gpx/sf/sfg.hpp"
// #include "gpx/sf/sfc.hpp"

#include "sfheaders/sfc/sfc_types.hpp"

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


  Rcpp::NumericVector bbox = sfheaders::bbox::start_bbox();
  Rcpp::NumericVector z_range = sfheaders::zm::start_z_range();
  Rcpp::NumericVector m_range = sfheaders::zm::start_m_range();

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

    //Rcpp::List this_track;
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
    // sfc[file_counter] = gpx::track::get_track(
    //   root_node, sfc, properties, file_counter, sfg_objects,
    //     bbox, z_range, m_range, time_format, list_depths, df_cols
    //   );
    gpx::track::get_track(
      root_node, sfc, properties, file_counter, sfg_objects,
      bbox, z_range, m_range, time_format, list_depths, df_cols
      );
  }

  //return df_cols;

  //Rcpp::List res = gpx::sfc::construct_sfc( sfg_objects, sfc, bbox, z_range, m_range );

  Rcpp::List res = sfheaders::sfc::make_sfc( sfc, sfheaders::sfc::SFC_LINESTRING, bbox, z_range, m_range );

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

  // TODO: the geometries will also be nested, so we need to unnest them too?
  Rcpp::List geometries( sfg_objects );

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

  int n_cols = 0;
  Rcpp::StringVector df_col_names = df_cols.names();

  for( int i = 0; i < df_cols.ncol(); i++ ) {
    const char * this_col = df_col_names[i];
    Rcpp::LogicalVector lv = df_cols[ this_col ];
    bool keep_col = lv[0];
    if( keep_col == true && strcmp( this_col , "ele" ) != 0) {  // not including ele or time
      n_cols++;
    }
  }

  Rcpp::List sf( n_cols );
  Rcpp::StringVector sf_names( n_cols );
  int col_index = 0;

  for( int i = 0; i < df_cols.ncol(); i++ ) {
    const char * this_col = df_col_names[i];
    Rcpp::LogicalVector lv = df_cols[ this_col ];
    bool keep_col = lv[0];

    if( keep_col == true & strcmp( this_col, "ele" ) != 0 ) {
      sf_names[col_index] = this_col;
      if( strcmp( this_col, "name" ) == 0 ) {
        sf[col_index] = track_names;
      } else if ( strcmp( this_col, "comment" ) == 0 ) {
        sf[col_index] = track_comments;
      } else if ( strcmp( this_col, "description" ) == 0 ) {
        sf[col_index] = track_descriptions;
      } else if ( strcmp( this_col, "source" ) == 0 ) {
        sf[col_index] = track_sources;
      } else if ( strcmp( this_col, "link" ) == 0 ) {
        sf[col_index] = track_links;
      } else if ( strcmp( this_col, "number" ) == 0 ) {
        sf[col_index] = track_numbers;
      } else if ( strcmp( this_col, "type" ) == 0 ) {
        sf[col_index] = track_types;
      }
      col_index++;
    }
  }

  sf.names() = sf_names;

  sf[ "geometry" ] = res;



  /*
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
  */

  if (sfg_objects > 0 ) {
    Rcpp::IntegerVector nv = Rcpp::seq( 1, sfg_objects );
    sf.attr("row.names") = nv;
  } else {
    sf.attr("row.names") = Rcpp::IntegerVector(0);
  }


  sf.attr("sf_column") = "geometry";
  sf.attr("class") = Rcpp::CharacterVector::create("sf", "data.frame");
  return sf;

}
