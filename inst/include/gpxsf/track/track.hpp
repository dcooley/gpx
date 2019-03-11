#ifndef GPXSF_TRACK_H
#define GPXSF_TRACK_H

#include <Rcpp.h>

#include "gpxsf/anytime.hpp"

#include <rapidxml.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time.hpp>

using namespace Rcpp;
using namespace rapidxml;

// [[Rcpp::depends(BH)]]

namespace gpxsf {
namespace track {


  // std::vector< std::string > times
  inline void string_to_datetime(  ) {


    /*
    int n = times.size();
    int i;
    for( i = 0; i < n; i++ ) {
      std::string t = times[i];
      boost::posix_time::ptime pt( boost::posix_time::time_from_string( t ) );
      Rcpp::Rcout << pt << std::endl;
    }
    */

    std::string ts("2002-01-20 23:59:59.000");
    boost::posix_time::ptime pt( boost::posix_time::time_from_string( ts ) );
  }


  inline void get_points(
      xml_node<> *trk_seg_node,
      std::vector< double >& lons,
      std::vector< double >& lats,
      std::vector< double >& elev,
      std::vector< double >& time ) {

    // iterate over pts
    for( xml_node<> *trk_pt_node = trk_seg_node -> first_node("trkpt"); trk_pt_node; trk_pt_node = trk_pt_node -> next_sibling()) {

      const char* clon = trk_pt_node -> first_attribute("lon") -> value();
      const char* clat = trk_pt_node -> first_attribute("lat") -> value();

      const char* cele = trk_pt_node -> first_node("ele") -> value();
      const char* ctim = trk_pt_node -> first_node("time") -> value();

      // TODO convert time to numeric
      //Rcpp::Rcout << "time: " << ctim << std::endl;
      std::string stim = std::string( ctim );

      double d = gpxsf::anytime::stringToTime( stim );

      //Rcpp::Rcout << "time: " << d << std::endl;


      //std::string ts("2002-01-20 23:59:59.000");
      //boost::posix_time::ptime pt( boost::posix_time::time_from_string( ts ) );

      // boost::posix_time::ptime pt( boost::posix_time::time_from_string( ts ) ) ;
      // boost::posix_time::ptime pt( boost::posix_time::time_from_string( stim ) );

      //boost::posix_time::ptime pt;


      //string_to_datetime();

      lons.push_back( atof( clon ) );
      lats.push_back( atof( clat ) );
      elev.push_back( atof( cele ) );
      time.push_back( d );
    }

    //string_to_datetime();

  }

  inline void get_segments(
      xml_node<> *trk_node,
      std::vector< double >& lons,
      std::vector< double >& lats,
      std::vector< double >& elev,
      std::vector< double >& time ) {

    for( xml_node<> *trk_seg_node = trk_node -> first_node("trkseg"); trk_seg_node; trk_seg_node = trk_seg_node -> next_sibling()) {

      gpxsf::track::get_points( trk_seg_node, lons, lats, elev, time );

    }
  }

  inline void get_track(
      xml_node<> *root_node,
      std::vector< double >& lons,
      std::vector< double >& lats,
      std::vector< double >& elev,
      std::vector< double >& time ) {

    // iterate over the track
    for (xml_node<> *trk_node = root_node -> first_node("trk"); trk_node; trk_node = trk_node -> next_sibling()) {

      // iterate over segements
      gpxsf::track::get_segments( trk_node, lons, lats, elev, time );

    }
  }


} // namespace track
} // namespace gpxsf

#endif
