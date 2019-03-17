#ifndef GPXSF_RAPIDXML_UTILS_H
#define GPXSF_RAPIDXML_UTILS_H

#include <rapidxml.hpp>

namespace gpxsf {
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

} // utils
} // gpxsf

#endif
