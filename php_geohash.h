#define PHP_GEOHASH_EXTNAME "geohash"
#define PHP_GEOHASH_VERSION "0.1.0"

#ifndef GEOHASH_H
#define GEOHASH_H

#define geohash_t   zval
#define GEOHASH_PROPERTY_NAME_INSTANCE "_instance"
#define GEOHASH_PROPERTY_NAME_LATMAX "latitude_range_max"
#define GEOHASH_PROPERTY_NAME_LATMIN "latitude_range_min"
#define GEOHASH_PROPERTY_NAME_LONMAX "longtitude_range_max"
#define GEOHASH_PROPERTY_NAME_LONMIN "longtitude_range_min"

PHP_MINIT_FUNCTION(geohash);

void geohash_init_geohash(TSRMLS_D);
void geohash_init_exception(TSRMLS_D);
 
PHP_METHOD(Geohash, __construct);
PHP_METHOD(Geohash, getInstance);
PHP_METHOD(Geohash, encode);
PHP_METHOD(Geohash, fastEncode);
PHP_METHOD(Geohash, decode);
PHP_METHOD(Geohash, fastDecode);
PHP_METHOD(Geohash, neighbors);
PHP_METHOD(Geohash, stepInRadius);
PHP_METHOD(Geohash, radiusSearch);
 
#endif


 




