
## What is gpx?

Very simple package which converts
[GPX](https://www.topografix.com/GPX/1/1/) files to `sf` objects.

``` r
library(gpx)
library(sf) ## for print methods of sf objects
# Linking to GEOS 3.7.1, GDAL 2.4.1, PROJ 6.0.0
gpx <- system.file("gpx/city_trail.gpx", package = "gpxsf")
sf <- gpx::gpx_sf( gpx )

sf
# Simple feature collection with 1 feature and 2 fields
# geometry type:  LINESTRING
# dimension:      XYZM
# bbox:           xmin: 144.9356 ymin: -37.84586 xmax: 145.0274 ymax: -37.77897
# z_range:        zmin: -1.8 zmax: 54.9
# m_range:        mmin: 1499569000 mmax: 1499581000
# epsg (SRID):    4326
# proj4string:    +proj=longlat +datum=WGS84 +no_defs
#                                  name type                       geometry
# 1 Sunday afternoon Capital City trail    1 LINESTRING ZM (144.9998 -37...
```

And that’s it.

However, the real benefit, and the reason I built this package, is so we
can plot them in mapdeck’s trip layer

## How are timestamps handled?

The timestamp can specified as one of

  - datetime (default) - the numeric representation of POSIX dates
  - counter - every track starts from 0 and counts to the maximum time
  - normalise - every track’s time is rescaled to be \[0,1\]

Here are some examples

``` r

sf_time <- gpx_sf( gpx, time = "datetime" )
sf_count <- gpx_sf( gpx, time = "counter" )
sf_norm <- gpx_sf( gpx, time = "normalise" )

d <- getOption("digits")
options(digits = 12)

track_time <- unclass( sf_time[, "geometry"][[1]][[1]] )
head( track_time )
#            [,1]       [,2] [,3]       [,4]
# [1,] 144.999816 -37.845118 21.8 1499569160
# [2,] 144.999838 -37.845079 21.8 1499569161
# [3,] 144.999832 -37.845019 21.4 1499569162
# [4,] 144.999874 -37.845010 21.2 1499569163
# [5,] 144.999931 -37.844929 20.9 1499569164
# [6,] 144.999853 -37.844893 20.7 1499569165

summary( track_time[, 4] )
#       Min.    1st Qu.     Median       Mean    3rd Qu.       Max. 
# 1499569160 1499570978 1499573624 1499574804 1499578474 1499581021

track_count <- unclass( sf_count[, "geometry"][[1]][[1]] )
head( track_count )
#            [,1]       [,2] [,3] [,4]
# [1,] 144.999816 -37.845118 21.8    0
# [2,] 144.999838 -37.845079 21.8    1
# [3,] 144.999832 -37.845019 21.4    2
# [4,] 144.999874 -37.845010 21.2    3
# [5,] 144.999931 -37.844929 20.9    4
# [6,] 144.999853 -37.844893 20.7    5

summary( track_count[, 4] )
#        Min.     1st Qu.      Median        Mean     3rd Qu.        Max. 
#     0.00000  1818.00000  4464.00000  5644.41681  9314.00000 11861.00000

track_norm <- unclass( sf_norm[, "geometry"][[1]][[1]] )
head( track_norm )
#            [,1]       [,2] [,3]              [,4]
# [1,] 144.999816 -37.845118 21.8 0.000000000000000
# [2,] 144.999838 -37.845079 21.8 0.000084309923278
# [3,] 144.999832 -37.845019 21.4 0.000168619846556
# [4,] 144.999874 -37.845010 21.2 0.000252929769834
# [5,] 144.999931 -37.844929 20.9 0.000337239693112
# [6,] 144.999853 -37.844893 20.7 0.000421549616390

summary( track_norm[, 4] )
#        Min.     1st Qu.      Median        Mean     3rd Qu.        Max. 
# 0.000000000 0.153275441 0.376359498 0.475880348 0.785262625 1.000000000

options(digits = d)
```

### Surely there’s something else?

Oh yes, it’s quite quick too\!

This example compares reading 364 gpx files using this library, compared
to reading a single gpx file using `library(plotKML)`

``` r

library(plotKML)
library(microbenchmark)

fp <- path.expand(".../.../")
l <- list.files(fp, patter = "gpx$")

gpx <- paste0(fp, l)

length( gpx )
# 364

microbenchmark::microbenchmark(
  gpx = {
    sf <- gpx:::rcpp_gpx_to_sf( gpx, time = "default" )
  },
  kml = {
    kml <- plotKML::readGPX( gpx[1] )
  },
  times = 1
)

# Unit: seconds
#  expr      min       lq     mean   median       uq      max neval
#   gpx 5.450537 5.450537 5.450537 5.450537 5.450537 5.450537     1
#   kml 4.036555 4.036555 4.036555 4.036555 4.036555 4.036555     1
```
