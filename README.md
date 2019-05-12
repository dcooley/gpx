
# What is gpxsf?

Very simple package which converts GPX files to `sf` objects.

``` r
library(sf) ## for print methods of sf objects
#> Linking to GEOS 3.7.1, GDAL 2.4.1, PROJ 6.0.0
gpx <- system.file("./gpx/city_trail.gpx", package = "gpxsf")
sf <- gpxsf::gpx_sf( gpx )

sf
#> Simple feature collection with 1 feature and 7 fields
#> geometry type:  LINESTRING
#> dimension:      XYZM
#> bbox:           xmin: 144.9356 ymin: -37.84586 xmax: 145.0274 ymax: -37.77897
#> z_range:        zmin: -1.8 zmax: 54.9
#> m_range:        mmin: 1499569000 mmax: 1499581000
#> epsg (SRID):    4326
#> proj4string:    +proj=longlat +datum=WGS84 +no_defs
#>                                  name comment description source link
#> 1 Sunday afternoon Capital City trail                                
#>   number type                       geometry
#> 1     NA    1 LINESTRING ZM (144.9998 -37...
```

And that’s it.

However, the real benefit, and the reason I built this package, is so we
can plot them in mapdeck’s trip layer

### documentation notes

<https://www.topografix.com/GPX/1/1/#type_trkType>
