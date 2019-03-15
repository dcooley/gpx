#ifndef GPXSF_TRACK_H
#define GPXSF_TRACK_H

#include <Rcpp.h>

#include "gpxsf/time/anytime.hpp"
#include "gpxsf/sf/sfc.hpp"

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

  inline void get_track(
      xml_node<> *root_node,
      Rcpp::NumericVector& bbox,
      std::vector< double >& lons,
      std::vector< double >& lats,
      std::vector< double >& elev,
      std::vector< double >& time ) {

    for(
       xml_node<> *trk_node = root_node -> first_node("trk");
       trk_node;
       trk_node = trk_node -> next_sibling()
       ) {
      gpxsf::track::get_segments( trk_node, bbox, lons, lats, elev, time );
    }
  }


} // namespace track
} // namespace gpxsf

#endif
