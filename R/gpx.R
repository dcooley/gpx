#' gpx sf
#'
#' Converts GPX files to simple feature objects.
#'
#' @param gpx path to gpx files
#' @param time the format of the timestamp element returned to the sf object.
#' One of datetime, counter or normalise. See Details
#'
#' @return \code{sf} object wtih XYZM dimensions
#'
#' @details
#'
#' Use \code{time} to specify how to handle the time component of the GPX file (if available)
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
#' f <- system.file("data/run.gpx", package = "gpx")
#' sf <- gpx_sf( f )
#' str( sf )
#'
#'
#' # load sf library for print method
#' # library(sf)
#' # sf
#'
#' @export
gpx_sf <- function( gpx, time = c("datetime","counter","normalise") ) {
  UseMethod("gpx_sf")
}

#' @export
gpx_sf.character <- function( gpx, time = c("datetime", "counter", "normalise")) {

  time <- match.arg( time )

  if( all( file.exists( gpx ) ) ) {
    return( rcpp_gpx_to_sf( normalizePath( gpx ), time ) )
  } else {
    stop("Can't find the gpx file")
  }
}



is_url <- function(gpx) grepl("^https?://", geojson, useBytes=TRUE)

read_url <- function(con) {
  out <- tryCatch({
    paste0(readLines(con), collapse = "")
  },
  error = function(cond){
    stop("There was an error downloading the gpx")
  },
  finally = {
    close(con)
  })
}
