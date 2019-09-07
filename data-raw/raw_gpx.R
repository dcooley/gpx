
## examples from
## https://bivouac.com/pgxPg.asp?pgxId=3428

gpx <- '<?xml version="1.0" encoding="ISO-8859-1"?>
<gpx version="1.1"
creator="Bivouac.com 2015.12.13 https://bivouac.com"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xmlns="http://www.topografix.com/GPX/1/1"
xsi:schemaLocation="http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd">
<wpt lat="49.60907" lon="-123.60884">
<name>Steele Cabin</name>
</wpt>
</gpx>'

writeLines(gpx, con = "./inst/gpx/wpt.gpx")

gpx::gpx_sf( "./inst/gpx/wpt.gpx" )

gpx <- '<?xml version="1.0" encoding="ISO-8859-1"?>
<gpx version="1.1"
creator="Bivouac.com 2015.12.13 https://bivouac.com"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xmlns="http://www.topografix.com/GPX/1/1"
xsi:schemaLocation="http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd">
<wpt lat="49.60500" lon="-123.60500">
<ele>1659</ele>
<name>Mount Steele 1659m</name>
</wpt>
<wpt lat="49.60778" lon="-123.59889">
<ele>1647</ele>
<name>Raven Dance Peak 1647m</name>
</wpt>
<wpt lat="49.60907" lon="-123.60884">
<name>Steele Cabin</name>
</wpt>
</gpx>'

writeLines(gpx, con = "./inst/gpx/wpts.gpx")

gpx::gpx_sf( "./inst/gpx/wpts.gpx" )


gpx <- '<?xml version="1.0" encoding="ISO-8859-1"?>
<gpx version="1.1"
creator="Bivouac.com 2015.12.13 https://bivouac.com"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xmlns="http://www.topografix.com/GPX/1/1"
xsi:schemaLocation="http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd">
<trk>
<name>Edwards Lake Trail</name>
<trkseg>
<trkpt lat="49.598920" lon="-123.666710"></trkpt>
<trkpt lat="49.598920" lon="-123.666710"></trkpt>
<trkpt lat="49.598480" lon="-123.665310"></trkpt>
<trkpt lat="49.598350" lon="-123.664740"></trkpt>
<trkpt lat="49.598160" lon="-123.664390"></trkpt>
<trkpt lat="49.598210" lon="-123.662520"></trkpt>
<trkpt lat="49.599180" lon="-123.661190"></trkpt>
<trkpt lat="49.600240" lon="-123.658990"></trkpt>
<trkpt lat="49.600200" lon="-123.658370"></trkpt>
</trkseg>
</trk>
</gpx>'

writeLines(gpx, con = "./inst/gpx/trk.gpx")

gpx::gpx_sf( "./inst/gpx/trk.gpx" )


gpx <- '<?xml version="1.0" encoding="ISO-8859-1"?>
<gpx version="1.1"
creator="Bivouac.com 2015.12.13 https://bivouac.com"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xmlns="http://www.topografix.com/GPX/1/1"
xsi:schemaLocation="http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd">
<trk>
<name>Mount Steele Trail</name>
<desc>Steep but continuous beaten path. From the Edwards Lake cabin continue east.
</desc>
<trkseg>
<trkpt lat="49.595850" lon="-123.620950"></trkpt>
<trkpt lat="49.596100" lon="-123.620570"></trkpt>
<trkpt lat="49.597150" lon="-123.620220"></trkpt>
<trkpt lat="49.597710" lon="-123.620290"></trkpt>
<trkpt lat="49.598070" lon="-123.620460"></trkpt>
<trkpt lat="49.598180" lon="-123.619990"></trkpt>
<trkpt lat="49.598350" lon="-123.619520"></trkpt>
</trkseg>
</trk>
</gpx>'

writeLines(gpx, con = "./inst/gpx/trk_desc.gpx")

gpx::gpx_sf( "./inst/gpx/trk_desc.gpx" )


gpx <- '<?xml version="1.0" encoding="ISO-8859-1"?>
<gpx version="1.1"
creator="Bivouac.com 2015.12.13 https://bivouac.com"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xmlns="http://www.topografix.com/GPX/1/1"
xsi:schemaLocation="http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd">
<wpt lat="49.60500" lon="-123.60500">
<ele>1659</ele>
<name>Mount Steele 1659m</name>
</wpt>
<wpt lat="49.60778" lon="-123.59889">
<ele>1647</ele>
<name>Raven Dance Peak 1647m</name>
</wpt>
<trk>
<name>Mount Steele Trail</name>
<trkseg>
<trkpt lat="49.595850" lon="-123.620950"></trkpt>
<trkpt lat="49.596100" lon="-123.620570"></trkpt>
<trkpt lat="49.597150" lon="-123.620220"></trkpt>
<trkpt lat="49.597710" lon="-123.620290"></trkpt>
<trkpt lat="49.598070" lon="-123.620460"></trkpt>
<trkpt lat="49.598180" lon="-123.619990"></trkpt>
</trkseg>
</trk>
<wpt lat="49.60907" lon="-123.60884">
<name>Steele Cabin</name>
</wpt>
</gpx>'

writeLines(gpx, con = "./inst/gpx/wpt_trk.gpx")

gpx::gpx_sf( "./inst/gpx/wpt_trk.gpx" )

plotKML::readGPX( "./inst/gpx/wpt_trk.gpx" )


