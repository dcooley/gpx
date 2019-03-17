#ifndef GPXSF_TIME_ANYTIME_H
#define GPXSF_TIME_ANYTIME_H

// anytime.hpp uses code from Dirk Eddelbuettel's anytime library, specifically anytime.cpp
// https://github.com/eddelbuettel/anytime/blob/master/src/anytime.cpp

#if !defined(BOOST_NO_AUTO_PTR)
#define BOOST_NO_AUTO_PTR
#endif
#include <boost/date_time.hpp>

#include <Rcpp.h>

const std::string sformats[] = {

  // See the Boost documentation, tz specifications (%q %Q %z %Z) are _ignored_ on input
  // http://www.boost.org/doc/libs/1_62_0/doc/html/date_time/date_time_io.html#date_time.time_input_facet
  "%Y-%m-%d %H:%M:%S%Z",

  "%Y-%m-%d %H:%M:%S%f",
  "%Y-%m-%d %H%M%S%f",
  "%Y-%b-%d %H:%M:%S%f",
  "%Y-%B-%d %H:%M:%S%f"

};

const size_t nsformats = sizeof(sformats)/sizeof(sformats[0]);

class TimeFormats {
private:
  std::vector<std::string> formats;
  std::vector<std::locale> locales;
public:
  TimeFormats() {
    for (size_t i=0; i<nsformats; i++) {

      formats.push_back(sformats[i]);
      locales.push_back(std::locale(std::locale::classic(),
                                    new boost::posix_time::time_input_facet(sformats[i])));
    }
  }

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
  inline double ptToDoubleUTC(const boost::posix_time::ptime & pt) {

    const boost::posix_time::ptime timet_start(boost::gregorian::date(1970,1,1));

    boost::posix_time::time_duration tdiff = pt - timet_start;
    double totsec = tdiff.total_microseconds() / 1.0e6;
    return totsec;
  }


  // https://github.com/eddelbuettel/anytime/blob/master/src/anytime.cpp#L197
  // given a string with a (date)time object, try all formats til we parse one
  // conversion of ptime object to double done by ptToDouble()
  double stringToTime( const std::string s ) {

    boost::posix_time::ptime pt, ptbase;

    // loop over formats and try them til one fits
    for (size_t i = 0; pt == ptbase && i < timeformats.getN(); ++i) {
      std::istringstream is(s);

      is.imbue(timeformats.getLocale(i));
      is >> pt;
    }

    if (pt == ptbase) return NA_REAL; // NA for non-parsed dates

    return ptToDoubleUTC( pt );
  }

} // namespace anytime
} // namespace gpxsf


#endif
