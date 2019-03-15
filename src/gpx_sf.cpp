
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

std::size_t count_points(rapidxml::xml_node<> *trk_seg_node) {
  size_t counter = 0;
  for(
    rapidxml::xml_node<> *trk_pt_node = trk_seg_node -> first_node("trkpt");
    trk_pt_node;
    trk_pt_node = trk_pt_node -> next_sibling()
  ) {

    counter++;
  }
  return counter;
}

// [[Rcpp::export]]
void preallocate( std::string f ) {

  rapidxml::xml_document<> doc;
  rapidxml::xml_node<> *root_node;

  //std::string f = "/Users/david/Downloads/run.gpx";
  std::ifstream theFile( f );

  std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
  buffer.push_back('\0');

  doc.parse<0>( &buffer[0] );

  root_node = doc.first_node("gpx");

  size_t n_points = 0;

  for(
    rapidxml::xml_node<> *trk_node = root_node -> first_node("trk");
    trk_node;
    trk_node = trk_node -> next_sibling()
  ) {

    for(
      rapidxml::xml_node<> *trk_seg_node = trk_node -> first_node("trkseg");
      trk_seg_node;
      trk_seg_node = trk_seg_node -> next_sibling()
    ) {

      for(
        rapidxml::xml_node<> *trk_pt_node = trk_seg_node -> first_node("trkpt");
        trk_pt_node;
        trk_pt_node = trk_pt_node -> next_sibling()
      ) {
        n_points = count_points( trk_pt_node );
      }

      Rcpp::NumericVector lons( n_points );
      Rcpp::NumericVector lats( n_points );

      int counter = 0;
      for(
        rapidxml::xml_node<> *trk_pt_node = trk_seg_node -> first_node("trkpt");
        trk_pt_node;
        trk_pt_node = trk_pt_node -> next_sibling()
      ) {
        const char* clon = trk_pt_node -> first_attribute("lon") -> value();
        const char* clat = trk_pt_node -> first_attribute("lat") -> value();

        lons[counter] = atof( clon );
        lats[counter] = atof( clat );
      }

    }
  }
}



// [[Rcpp::export]]
void push_back( std::string f) {

  rapidxml::xml_document<> doc;
  rapidxml::xml_node<> *root_node;

  //std::string f = "/Users/david/Downloads/run.gpx";
  std::ifstream theFile( f );

  std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
  buffer.push_back('\0');

  doc.parse<0>( &buffer[0] );

  root_node = doc.first_node("gpx");

  std::vector< double > lons;
  std::vector< double > lats;

  for(
    rapidxml::xml_node<> *trk_node = root_node -> first_node("trk");
    trk_node;
    trk_node = trk_node -> next_sibling()
  ) {

    for(
      rapidxml::xml_node<> *trk_seg_node = trk_node -> first_node("trkseg");
      trk_seg_node;
      trk_seg_node = trk_seg_node -> next_sibling()
    ) {

      for(
        rapidxml::xml_node<> *trk_pt_node = trk_seg_node -> first_node("trkpt");
        trk_pt_node;
        trk_pt_node = trk_pt_node -> next_sibling()
      ) {

        const char* clon = trk_pt_node -> first_attribute("lon") -> value();
        const char* clat = trk_pt_node -> first_attribute("lat") -> value();

        lons.push_back( atof( clon ) );
        lats.push_back( atof( clat ) );
      }
    }
  }
}



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

    // datetime - default
    if( time_format == "counter" ) {
      gpxsf::counter::counter( nv_time );
    } else if ( time_format == "normalise" ) {
      gpxsf::scale::rescale( nv_time );
    }

    // I'm making it a XYZM object
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
