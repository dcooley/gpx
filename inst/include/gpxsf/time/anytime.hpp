#ifndef GPXSF_TIME_ANYTIME_H
#define GPXSF_TIME_ANYTIME_H


#if !defined(BOOST_NO_AUTO_PTR)
#define BOOST_NO_AUTO_PTR
#endif
#include <boost/date_time.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>

#include <Rcpp.h>

const std::string sformats[] = {
  "%Y-%m-%d %H:%M:%S%f",
  "%Y-%m-%d %H%M%S%f",
  "%Y/%m/%d %H:%M:%S%f",
  "%Y%m%d %H%M%S%f",
  "%Y%m%d %H:%M:%S%f",
  "%m/%d/%Y %H:%M:%S%f",
  "%m-%d-%Y %H:%M:%S%f",
  // "%d.%m.%Y %H:%M:%S%f",

  "%Y-%b-%d %H:%M:%S%f",
  "%Y/%b/%d %H:%M:%S%f",
  "%Y%b%d %H%M%S%F",
  "%Y%b%d %H:%M:%S%F",
  "%b/%d/%Y %H:%M:%S%f",
  "%b-%d-%Y %H:%M:%S%f",

  "%d.%b.%Y %H:%M:%S%f",
  "%d%b%Y %H%M%S%f",
  "%d%b%Y %H:%M:%S%f",
  "%d-%b-%Y %H%M%S%f",
  "%d-%b-%Y %H:%M:%S%f",

  "%Y-%B-%d %H:%M:%S%f",
  "%Y/%B/%d %H:%M:%S%f",
  "%Y%B%d %H%M%S%f",
  "%Y%B%d %H:%M:%S%f",
  "%B/%d/%Y %H:%M:%S%f",
  "%B-%d-%Y %H:%M:%S%f",
  "%d.%B.%Y %H:%M:%S%f",

  // see http://stackoverflow.com/questions/39259184/formatting-dates-with-r for next one
  "%a %b %d %H:%M:%S%F %Y",

  // see RFC 822 and standard Unix use eg mail headers (but no TZ or UTC offset on input :-/ )
  "%a %d %b %Y %H:%M:%S%F",

  // See the Boost documentation, tz specifications (%q %Q %z %Z) are _ignored_ on input
  // http://www.boost.org/doc/libs/1_62_0/doc/html/date_time/date_time_io.html#date_time.time_input_facet
  "%Y-%m-%d %H:%M:%S%Z",

  // Issue 47: support formats like "Thu Jan 17 09:29:10 EST 2013" by ignoring the three-char TZ
  // also support fractional seconds if present
  "%a %b %d %H:%M:%S%F xxx %Y",

  "%Y-%m-%d",
  "%Y%m%d",
  "%m/%d/%Y",
  "%m-%d-%Y",

  "%Y-%b-%d",
  "%Y%b%d",
  "%b/%d/%Y",
  "%b-%d-%Y",

  "%d%b%Y"
  "%d-%b-%Y"

  "%Y-%B-%d",
  "%Y%B%d",
  "%B/%d/%Y",
  "%B-%d-%Y"

};

const size_t nsformats = sizeof(sformats)/sizeof(sformats[0]);

// somewhat simplistic C++ class -- but enough to allow us to hold (and hide)
// two vectors of (string) formats and locale using the strings -- so that users
// can retrieve the strings, and add to them
class TimeFormats {
private:
  std::vector<std::string> formats;
  std::vector<std::locale> locales;
public:
  TimeFormats() {
    for (size_t i=0; i<nsformats; i++) {
      //Rcpp::Rcout << i << std::endl;
      formats.push_back(sformats[i]);
      locales.push_back(std::locale(std::locale::classic(),
                                    new boost::posix_time::time_input_facet(sformats[i])));
    }
  }
  void addFormat(const char *txt) {
    formats.insert(formats.begin(), txt);
    locales.insert(locales.begin(), std::locale(std::locale::classic(),
                                 new boost::posix_time::time_input_facet(txt)));
  };
  std::locale getLocale(int i) { return locales[i]; }
  std::string getFormat(int i) { return formats[i]; }
  size_t getN() { return formats.size(); }
  std::vector<std::string> getFormats() { return formats; }
};

static TimeFormats timeformats;

namespace gpxsf {
namespace anytime {

  // https://github.com/eddelbuettel/anytime/blob/master/src/anytime.cpp#L184
  // given a ptime object, return (fractional) seconds since epoch -- at UTC
  inline double ptToDoubleUTC(const boost::posix_time::ptime & pt, const bool asDate=false) {
    const boost::posix_time::ptime timet_start(boost::gregorian::date(1970,1,1));

    if (asDate) {
      return static_cast<double>(pt.date().day_number()) - static_cast<double>(timet_start.date().day_number());
    }

    boost::posix_time::time_duration tdiff = pt - timet_start;
    double totsec = tdiff.total_microseconds()/1.0e6;
    return totsec;
  }


  // https://github.com/eddelbuettel/anytime/blob/master/src/anytime.cpp#L197
  // given a string with a (date)time object, try all formats til we parse one
  // conversion of ptime object to double done by ptToDouble()
  double stringToTime(const std::string s, const bool asUTC=false, const bool asDate=false) {

    boost::posix_time::ptime pt, ptbase;

    // loop over formats and try them til one fits
    for (size_t i=0; pt == ptbase && i < timeformats.getN(); ++i) {
      std::istringstream is(s);
      //Rcpp::Rcout << i << " " << timeformats.getFormat(i) << std::endl;
      is.imbue(timeformats.getLocale(i));
      is >> pt;
    }

    if (pt == ptbase) return NA_REAL; // NA for non-parsed dates

    //if (asUTC) {
      return ptToDoubleUTC(pt, asDate);
    //} else {
    //  return ptToDouble(pt, asDate);
    //}
  }




} // namespace anytime
} // namespace gpxsf


#endif
