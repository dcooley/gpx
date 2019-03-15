

# fp <- path.expand("~/Documents/Data/Strava/Bethan/activities/")
# l <- list.files(fp, patter = "gpx$")
#
# gpx <- paste0(fp, l)
#
# library(plotKML)
#
# library(microbenchmark)
#
# microbenchmark::microbenchmark(
#   gpxsf = {
#     sf <- gpxsf:::rcpp_gpx_to_sf( gpx[1], "default" )
#   },
#   preallocate = {
#     gpxsf:::preallocate( gpx[1] )
#   },
#   push_back = {
#     gpxsf:::push_back( gpx[1] )
#   },
#   kml = {
#     kml <- plotKML::readGPX( gpx[1] )
#   },
#   times = 1
# )


