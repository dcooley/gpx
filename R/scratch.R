#
#
# fp <- path.expand("~/Documents/Data/Strava/Bethan/activities/")
# l <- list.files(fp, patter = "gpx$")
#
# gpx <- paste0(fp, l)
#
# library(sf)
#
# sf <- gpxsf::gpx_sf( gpx[1] )
#
# library(plotKML)
#
# library(microbenchmark)
#
# microbenchmark::microbenchmark(
#   gpxsf = {
#     sf <- gpxsf:::rcpp_gpx_to_sf( gpx[1:3], "default" )
#   },
#   # preallocate = {
#   #   gpxsf:::preallocate( gpx[1] )
#   # },
#   # push_back = {
#   #   gpxsf:::push_back( gpx[1] )
#   # },
#   kml = {
#     kml <- plotKML::readGPX( gpx[1] )
#   },
#   times = 1
# )
#

# sf <- gpxsf::gpx_sf( "~/Downloads/ATB-Heerenveen.gpx")

#
#
# l1 <- sf::st_linestring( x = matrix(c(0,0,1,1), ncol = 2))
# l2 <- sf::st_linestring( x = matrix(c(0,0,1,1), ncol = 2))
#
# sfc <- sf::st_sfc( list(l1, l2))
#
# str( sfc )
# str( sf::st_sf( geometry = sfc ))
#
# str( sf )

