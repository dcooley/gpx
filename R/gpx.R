#' gpx sf
#'
#' Converts GPX files to simple feature objects.
#'
#' @param gpx path to gpx files
#' @param time_format one of default, counter or normalise. See Details
#'
#' @return \code{sf} object wtih XYZM dimensions
#'
#' @details
#'
#' Use \code{time_format} to specify how to handle the time component of the GPX file (if available)
#'
#' \itemize{
#'   \item{datetime - numeric representation of the date }
#'   \item{counter - the first timestamp of a track is set to zero. Subsequent timestamps
#'   are offset from zero}
#'   \item{normalise - the timestamps are set to the range [0,1]}
#' }
#'
#' Times are assumed to be in UTC and conforming to ISO 8601, as per the specification
#' \url{https://www.topografix.com/gpx_manual.asp#time}
#'
#' @examples
#'
#' f <- system.file("data/run.gpx", package = "gpxsf")
#' sf <- gpx_sf( f )
#' str( sf )
#'
#'
#' # load sf library for print method
#' # library(sf)
#' # sf
#'
#' @export
gpx_sf <- function( gpx, time_format = c("datetime","counter","normalise") ) {
  time_format <- match.arg( time_format )
  rcpp_gpx_to_sf( gpx, time_format)
}
