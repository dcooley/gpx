#
# fp <- path.expand("~/Documents/Data/Strava/Bethan/activities/")
# l <- list.files(fp, patter = "gpx$")
#
# gpx <- paste0(fp, l)
#
# #library(sf)
#
# sf <- gpx::gpx_sf( gpx[1:10] )


#
#
# library(mapdeck)
#
# set_token( read.dcf("~/Documents/.googleAPI", fields = "MAPBOX"))
#
# mapdeck(
# 	style = mapdeck_style("dark")
# 	, location = c(144.5, -37.9)
# 	, zoom = 8
# 	) %>%
# 	add_trips(
# 		data = sf
# 		, loop_length = 11861
# 		, animation_speed = 200
# 		, stroke_colour = "#FFFFFF"
# 	)


#
# library(plotKML)
#
# library(microbenchmark)
#
# microbenchmark::microbenchmark(
#   gpx = {
#     sf <- gpx:::rcpp_gpx_to_sf( gpx, "default" )
#   },
#   # preallocate = {
#   #   gpx:::preallocate( gpx[1] )
#   # },
#   # push_back = {
#   #   gpx:::push_back( gpx[1] )
#   # },
#   kml = {
#     kml <- plotKML::readGPX( gpx[1] )
#   },
#   times = 1
# )



# sf <- gpx::gpx_sf( "~/Downloads/ATB-Heerenveen.gpx")

# sf <- gpx::gpx_sf( c("~/Downloads/ATB-Heerenveen.gpx","~/Downloads/ATB-Heerenveen.gpx") )

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


# sf <- gpx::gpx_sf("~/Downloads/St_Louis_Zoo_sample.gpx", time = "normalise" )


# https://software.frankingermann.de/gpxtrackmap/gtm-examples
#sf <- gpx::gpx_sf("~/Downloads/kuhkopfsteig-fv.gpx")

