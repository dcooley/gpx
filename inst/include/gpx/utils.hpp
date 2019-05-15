#ifndef R_GPX_RAPIDXML_UTILS_H
#define R_GPX_RAPIDXML_UTILS_H

#include <rapidxml.hpp>

namespace gpx {
namespace utils {

  inline std::size_t xml_size(rapidxml::xml_node<> *outer_node, const char* node) {
    size_t counter = 0;
    for(
      rapidxml::xml_node<> *inner_node = outer_node -> first_node( node );
      inner_node;
      inner_node = inner_node -> next_sibling()
    ) {
      counter++;
    }
    return counter;
  }

  inline void get_optional_element(
      rapidxml::xml_node<> * xml_node,
      const char* node,
      std::vector< std::string >& vec,
      Rcpp::DataFrame& df_cols
  ) {

    if( xml_node -> first_node( node ) ) {
      const char* trk_name = xml_node  -> first_node( node ) -> value();
      vec.push_back( trk_name );
      //Rcpp::Rcout << "keeping node: " << node << std::endl;
      df_cols[ node ] = true;
    } else {
      // TODO( push_back( NA_STRING ) ) -- this doesn't work
      vec.push_back( "" );
    }
  }


  inline void get_optional_element(
      rapidxml::xml_node<> * xml_node,
      const char* node,
      std::vector< double >& vec,
      Rcpp::DataFrame& df_cols
  ) {

    if( xml_node -> first_node( node ) ) {
      const char* trk_name = xml_node -> first_node( node ) -> value();
      vec.push_back( atof( trk_name ) );
      df_cols[ node ] = true;
    } else {
      vec.push_back( NA_REAL );
    }
  }

} // utils
} // gpx

#endif
