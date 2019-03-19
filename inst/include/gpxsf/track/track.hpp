#ifndef GPXSF_TRACK_H
#define GPXSF_TRACK_H

#include <Rcpp.h>

#include "gpxsf/time/anytime.hpp"
#include "gpxsf/sf/sfc.hpp"

#include "gpxsf/utils.hpp"


#include "gpxsf/time/scale.hpp"
#include "gpxsf/time/counter.hpp"

#include <rapidxml.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time.hpp>

using namespace Rcpp;
using namespace rapidxml;

// [[Rcpp::depends(BH)]]

namespace gpxsf {
namespace track {

  inline void get_points(
      xml_node<> *trk_seg_node,
      Rcpp::NumericVector& bbox,
      std::vector< double >& lons,
      std::vector< double >& lats,
      std::vector< double >& elev,
      std::vector< double >& time ) {

    for(
      xml_node<> *trk_pt_node = trk_seg_node -> first_node("trkpt");
      trk_pt_node;
      trk_pt_node = trk_pt_node -> next_sibling()
      ) {

      const char* clon = trk_pt_node -> first_attribute("lon") -> value();
      const char* clat = trk_pt_node -> first_attribute("lat") -> value();

      double dlon = atof( clon );
      double dlat = atof( clat );

      gpxsf::sfc::calculate_bbox( bbox, dlon, dlat );

      lons.push_back( dlon );
      lats.push_back( dlat );

      // ele & time are optional
      if( trk_pt_node -> first_node("ele") ) {
        const char* cele = trk_pt_node -> first_node("ele") -> value();
        elev.push_back( atof( cele ) );
      } else {
        elev.push_back( NA_REAL );
      }

      if( trk_pt_node -> first_node("time") ) {
        const char* ctim = trk_pt_node -> first_node("time") -> value();
        std::string stim = std::string( ctim );
        double d = gpxsf::anytime::stringToTime( stim );
        time.push_back( d );
      } else {
        time.push_back( NA_REAL );
      }
    }
  }

  inline void get_segments(
      xml_node<> *trk_node,
      Rcpp::NumericVector& bbox,
      std::vector< double >& lons,
      std::vector< double >& lats,
      std::vector< double >& elev,
      std::vector< double >& time ) {

    for(
      xml_node<> *trk_seg_node = trk_node -> first_node("trkseg");
      trk_seg_node;
      trk_seg_node = trk_seg_node -> next_sibling()
      ) {
      gpxsf::track::get_points( trk_seg_node, bbox, lons, lats, elev, time );
    }
  }

  inline Rcpp::List get_track(
      xml_node<> *root_node,
      int& sfg_objects,
      Rcpp::NumericVector& bbox,
      std::string& time_format
  ) {

    Rcpp::List sfc( 1 );

    std::vector< double > lons;
    std::vector< double > lats;
    std::vector< double > elev;
    std::vector< double > time;

    size_t trk_counter = 0;

    size_t n_trk = gpxsf::utils::xml_size( root_node, "trk" );
    Rcpp::List sfgs( n_trk );

    for(
       xml_node<> *trk_node = root_node -> first_node("trk");
       trk_node;
       trk_node = trk_node -> next_sibling()
       ) {

      // trk name
      //const char* trk_name = trk_node -> first_attribute("name") -> value();
      //Rcpp::Rcout << "trk_name: " << trk_name << std::endl;
      if( trk_node -> first_node("name") ) {
        const char* trk_name = trk_node -> first_node("name") -> value();
        Rcpp::Rcout << "name: " << trk_name << std::endl;

      } else {
        Rcpp::Rcout << "no trk name" << std::endl;
        //elev.push_back( NA_REAL );
      }



      if( trk_node -> first_node("cmt") ) {
        const char* trk_cmt = trk_node -> first_node("cmt") -> value();
        Rcpp::Rcout << "cmt: " << trk_cmt << std::endl;

      } else {
        Rcpp::Rcout << "no trk cmt" << std::endl;
        //elev.push_back( NA_REAL );
      }

      // TODO
      // every sibling must be a new LINESTRING
      gpxsf::track::get_segments( trk_node, bbox, lons, lats, elev, time );

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

      // sfg = linestring
      linestring.attr("class") = Rcpp::CharacterVector::create("XYZM", "LINESTRING", "sfg");
      sfgs[ trk_counter ] = linestring;
      trk_counter++;
      sfg_objects++;
    }
    sfc[0] = sfgs;
    return sfc;
  }


} // namespace track
} // namespace gpxsf

#endif
