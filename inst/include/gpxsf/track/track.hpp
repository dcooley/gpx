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
      Rcpp::NumericVector& z_range,
      Rcpp::NumericVector& m_range,
      std::vector< double >& lons,
      std::vector< double >& lats,
      std::vector< double >& elev,
      std::vector< double >& time,
      Rcpp::DataFrame& df_cols ) {

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
      gpxsf::utils::get_optional_element( trk_pt_node, "ele", elev, df_cols );
      //gpxsf::utils::get_optional_element( trk_pt_node, "time", time ); -- TIME is converted

      if( trk_pt_node -> first_node("time") ) {
        const char* ctim = trk_pt_node -> first_node("time") -> value();
        std::string stim = std::string( ctim );
        double d = gpxsf::anytime::stringToTime( stim );
        time.push_back( d );
      } else {
        time.push_back( NA_REAL );
      }

      gpxsf::sfc::calculate_range( z_range, elev.back() );
      // m_range is done AFTER scaling
      //gpxsf::sfc::calculate_range( m_range, time.back() );
    }
  }

  inline void get_segments(
      xml_node<> *trk_node,
      Rcpp::NumericVector& bbox,
      Rcpp::NumericVector& z_range,
      Rcpp::NumericVector& m_range,
      std::vector< double >& lons,
      std::vector< double >& lats,
      std::vector< double >& elev,
      std::vector< double >& time,
      Rcpp::DataFrame& df_cols ) {

    for(
      xml_node<> *trk_seg_node = trk_node -> first_node("trkseg");
      trk_seg_node;
      trk_seg_node = trk_seg_node -> next_sibling()
      ) {
      gpxsf::track::get_points( trk_seg_node, bbox, z_range, m_range, lons, lats, elev, time, df_cols );
    }
  }


  inline void get_track(
      xml_node<> *root_node,
      Rcpp::List& sfc,
      Rcpp::List& properties,
      int& file_counter,
      int& sfg_objects,
      Rcpp::NumericVector& bbox,
      Rcpp::NumericVector& z_range,
      Rcpp::NumericVector& m_range,
      std::string& time_format,
      Rcpp::NumericVector& list_depths,
      Rcpp::DataFrame& df_cols
  ) {

    std::vector< double > lons;
    std::vector< double > lats;
    std::vector< double > elev;
    std::vector< double > time;
    std::vector< std::string > name;
    std::vector< std::string > cmt; // comments
    std::vector< std::string > desc;
    std::vector< std::string > src;
    std::vector< std::string > link;
    std::vector< double > number;
    std::vector< std::string > type;

    size_t trk_counter = 0;

    size_t n_trk = gpxsf::utils::xml_size( root_node, "trk" );
    Rcpp::List sfgs( n_trk );

    // n_trk is the number of tracks in this file
    for(
       xml_node<> *trk_node = root_node -> first_node("trk");
       trk_node;
       trk_node = trk_node -> next_sibling()
       ) {

      gpxsf::utils::get_optional_element( trk_node, "name", name, df_cols );
      gpxsf::utils::get_optional_element( trk_node, "cmt", cmt, df_cols );
      gpxsf::utils::get_optional_element( trk_node, "desc", desc, df_cols );
      gpxsf::utils::get_optional_element( trk_node, "src", src, df_cols );
      gpxsf::utils::get_optional_element( trk_node, "link", link, df_cols );
      gpxsf::utils::get_optional_element( trk_node, "number", number, df_cols );
      gpxsf::utils::get_optional_element( trk_node, "type", type, df_cols );

      // TODO
      // every sibling must be a new LINESTRING
      gpxsf::track::get_segments( trk_node, bbox, z_range, m_range, lons, lats, elev, time, df_cols );

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

      // Need to calcualte the time (m) ranges AFTER scaling the time
      double min_time = Rcpp::min( nv_time );
      double max_time = Rcpp::max( nv_time );

      gpxsf::sfc::calculate_range( m_range, min_time );
      gpxsf::sfc::calculate_range( m_range, max_time );

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

    sfc[ file_counter ] = sfgs;
    list_depths[ file_counter ] = trk_counter;

    Rcpp::StringVector sv_name = Rcpp::wrap( name );
    Rcpp::StringVector sv_cmt = Rcpp::wrap( cmt );
    Rcpp::StringVector sv_desc = Rcpp::wrap( desc );
    Rcpp::StringVector sv_src = Rcpp::wrap( src );
    Rcpp::StringVector sv_link = Rcpp::wrap( link );
    Rcpp::NumericVector nv_number = Rcpp::wrap( number );
    Rcpp::StringVector sv_type = Rcpp::wrap( type );

    Rcpp::List lst_properties = Rcpp::List::create(
      _["name"] = sv_name,
      _["cmt"] = cmt,
      _["desc"] = desc,
      _["src"] = src,
      _["link"] = link,
      _["number"] = number,
      _["type"] = type
    );

    properties[ file_counter ] = lst_properties;
  }


} // namespace track
} // namespace gpxsf

#endif
