#include <php.h>
#include <zend_exceptions.h>

#include "php_geohash.h"
#include "geohash_int.h"

zend_module_entry geohash_module_entry = {
  STANDARD_MODULE_HEADER,
  PHP_GEOHASH_EXTNAME,
  NULL,
  PHP_MINIT(geohash),
  NULL,
  NULL,
  NULL,
  NULL,
  PHP_GEOHASH_VERSION,
  STANDARD_MODULE_PROPERTIES
};
 
// install module
ZEND_GET_MODULE(geohash);

PHP_MINIT_FUNCTION(geohash) {
  geohash_init_geohash(TSRMLS_C);
  geohash_init_exception(TSRMLS_C);
}
 
zend_class_entry *geohash_ce;
zend_class_entry *geohash_exception_ce;
 
static zend_function_entry geohash_methods[] = {
  PHP_ME(Geohash, __construct, NULL, ZEND_ACC_CTOR|ZEND_ACC_PROTECTED)
  PHP_ME(Geohash, getInstance, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Geohash, encode, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(Geohash, fastEncode, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(Geohash, decode, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(Geohash, fastDecode, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(Geohash, neighbors, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(Geohash, stepInRadius, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(Geohash, radiusSearch, NULL, ZEND_ACC_PUBLIC)
  {NULL, NULL, NULL}
};

/** {{{ static geohash_t * geohash_instance(TSRMLS_D)
*/
static geohash_t * geohash_instance(TSRMLS_D) {
  geohash_t *instance;

  MAKE_STD_ZVAL(instance);
  object_init_ex(instance, geohash_ce);

  zend_update_static_property(geohash_ce, ZEND_STRL(GEOHASH_PROPERTY_NAME_INSTANCE), instance TSRMLS_CC);
  zval_ptr_dtor(&instance);
  return instance;
}
/* }}} */

/** {{{ static int steps_in_meters(double radius)
*/
static int steps_in_meters(double radius) {
  int step;

  if(radius<1)
  {
    step = 52/2;
  }
  else if(radius<=2)//1.1943
  {
    step = 50/2;
  }
  else if(radius<=3)//2.3889
  {
    step = 48/2;
  }
  else if(radius<=5)//4.7774
  {
    step = 46/2;
  }
  else if(radius<=10)//9.5547
  {
    step = 44/2;
  }
  else if(radius<=20)//19.1095
  {
    step = 42/2;
  }
  else if(radius<=40)//38.2189
  {
    step = 40/2;
  }
  else if(radius<=80)//76.4378
  {
    step = 38/2;
  }
  else if(radius<152.8757)
  {
    step = 36/2;
  }
  else if(radius<305.751)
  {
    step = 34/2;
  }
  else if(radius<611.5028)
  {
    step = 32/2;
  }
  else if(radius<1223.0056)
  {
    step = 30/2;
  }
  else if(radius<2446.0112)
  {
    step = 28/2;
  }
  else if(radius<=5000)//4892.0224
  {
    step = 26/2;
  }
  else if(radius<=10000)//9784.0449
  {
    step = 24/2;
  }
  else if(radius<=20000)//19568.0898
  {
    step = 22/2;
  }
  else if(radius<=40000)//39136.1797
  {
    step = 20/2;
  }
  else if(radius<80000)//78272.35938
  {
    step = 18/2;
  }
  else if(radius<156544.7188)
  {
    step = 16/2;
  }
  else if(radius<313089.4375)
  {
    step = 14/2;
  }
  else if(radius<626178.875)
  {
    step = 12/2;
  }
  else if(radius<1252357.75)
  {
    step = 10/2;
  }
  else if(radius<2504715.5)
  {
    step = 8/2;
  }
  else if(radius<5009431)
  {
    step = 6/2;
  }
  else// if(radius<10018863)
  {
    step = 4/2;
  }

  return step;
}
/* }}} */
 
void geohash_init_geohash(TSRMLS_D) {
  zend_class_entry ce;
 
  INIT_CLASS_ENTRY(ce, "Geohash", geohash_methods);

  geohash_ce = zend_register_internal_class(&ce TSRMLS_CC);

  zend_declare_property_null(geohash_ce, ZEND_STRL(GEOHASH_PROPERTY_NAME_INSTANCE), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);
  zend_declare_property_double(geohash_ce, ZEND_STRL(GEOHASH_PROPERTY_NAME_LATMAX), 90, ZEND_ACC_PROTECTED TSRMLS_CC);
  zend_declare_property_double(geohash_ce, ZEND_STRL(GEOHASH_PROPERTY_NAME_LATMIN), -90, ZEND_ACC_PROTECTED TSRMLS_CC);
  zend_declare_property_double(geohash_ce, ZEND_STRL(GEOHASH_PROPERTY_NAME_LONMAX), 180, ZEND_ACC_PROTECTED TSRMLS_CC);
  zend_declare_property_double(geohash_ce, ZEND_STRL(GEOHASH_PROPERTY_NAME_LONMIN), -180, ZEND_ACC_PROTECTED TSRMLS_CC);
}

void geohash_init_exception(TSRMLS_D) {
  zend_class_entry e;
 
  INIT_CLASS_ENTRY(e, "GeohashException", NULL);
  geohash_exception_ce = zend_register_internal_class_ex(&e, (zend_class_entry*)zend_exception_get_default(TSRMLS_C), NULL TSRMLS_CC);
}

/** {{{ proto protected Geohash::__construct(void)
*/
PHP_METHOD(Geohash, __construct) {
}
/* }}} */

/** {{{ proto public Geohash::getInstance(void)
*/
PHP_METHOD(Geohash, getInstance) {
  geohash_t *instance = zend_read_static_property(geohash_ce, ZEND_STRL(GEOHASH_PROPERTY_NAME_INSTANCE), 1 TSRMLS_CC);

  if (Z_TYPE_P(instance) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(instance), geohash_ce TSRMLS_CC)) {
    if ((instance = geohash_instance(TSRMLS_C))) {
      RETURN_ZVAL(instance, 1, 0);
    } else {
      RETURN_NULL();
    }
  } else {
    RETURN_ZVAL(instance, 1, 0);
  }
}
/* }}} */

/** {{{ proto public Geohash::encode(lat, long, step=26)
*/
PHP_METHOD(Geohash, encode) {
	double latitude;
	double longtitude;
  long step = 26;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd|l", &latitude, &longtitude, &step) == FAILURE) {
    return;
  }

  GeoHashRange lat_range, lon_range;
  lat_range.max = Z_DVAL_P(zend_read_property(geohash_ce, getThis(), ZEND_STRL(GEOHASH_PROPERTY_NAME_LATMAX), 1 TSRMLS_CC));
  lat_range.min = Z_DVAL_P(zend_read_property(geohash_ce, getThis(), ZEND_STRL(GEOHASH_PROPERTY_NAME_LATMIN), 1 TSRMLS_CC));
  lon_range.max = Z_DVAL_P(zend_read_property(geohash_ce, getThis(), ZEND_STRL(GEOHASH_PROPERTY_NAME_LONMAX), 1 TSRMLS_CC));
  lon_range.min = Z_DVAL_P(zend_read_property(geohash_ce, getThis(), ZEND_STRL(GEOHASH_PROPERTY_NAME_LONMIN), 1 TSRMLS_CC));

  GeoHashBits hash;
  if(geohash_encode(lat_range, lon_range, latitude, longtitude, step, &hash) == -1)
  {
  	zend_throw_exception(geohash_exception_ce, "expect `encode` failed", 500 TSRMLS_CC);
    return;
  }

  RETURN_LONG(hash.bits);
}
/* }}} */

/** {{{ proto public Geohash::fastEncode(lat, long, step=26)
*/
PHP_METHOD(Geohash, fastEncode) {
  double latitude;
  double longtitude;
  long step = 26;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd|l", &latitude, &longtitude, &step) == FAILURE) {
    return;
  }

  GeoHashRange lat_range, lon_range;
  lat_range.max = Z_DVAL_P(zend_read_property(geohash_ce, getThis(), ZEND_STRL(GEOHASH_PROPERTY_NAME_LATMAX), 1 TSRMLS_CC));
  lat_range.min = Z_DVAL_P(zend_read_property(geohash_ce, getThis(), ZEND_STRL(GEOHASH_PROPERTY_NAME_LATMIN), 1 TSRMLS_CC));
  lon_range.max = Z_DVAL_P(zend_read_property(geohash_ce, getThis(), ZEND_STRL(GEOHASH_PROPERTY_NAME_LONMAX), 1 TSRMLS_CC));
  lon_range.min = Z_DVAL_P(zend_read_property(geohash_ce, getThis(), ZEND_STRL(GEOHASH_PROPERTY_NAME_LONMIN), 1 TSRMLS_CC));

  GeoHashBits fast_hash;
  if(geohash_fast_encode(lat_range, lon_range, latitude, longtitude, step, &fast_hash) == -1)
  {
    zend_throw_exception(geohash_exception_ce, "expect `fast encode` failed", 500 TSRMLS_CC);
    return;
  }

  RETURN_LONG(fast_hash.bits);
}
/* }}} */

/** {{{ proto public Geohash::decode(hash, step=26)
*/
PHP_METHOD(Geohash, decode) {
  long hashbit;
  long step = 26;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|l", &hashbit, &step) == FAILURE) {
    return;
  }

  GeoHashBits hash;
  hash.step = step;
  hash.bits = hashbit;

  GeoHashRange lat_range, lon_range;
  lat_range.max = Z_DVAL_P(zend_read_property(geohash_ce, getThis(), ZEND_STRL(GEOHASH_PROPERTY_NAME_LATMAX), 1 TSRMLS_CC));
  lat_range.min = Z_DVAL_P(zend_read_property(geohash_ce, getThis(), ZEND_STRL(GEOHASH_PROPERTY_NAME_LATMIN), 1 TSRMLS_CC));
  lon_range.max = Z_DVAL_P(zend_read_property(geohash_ce, getThis(), ZEND_STRL(GEOHASH_PROPERTY_NAME_LONMAX), 1 TSRMLS_CC));
  lon_range.min = Z_DVAL_P(zend_read_property(geohash_ce, getThis(), ZEND_STRL(GEOHASH_PROPERTY_NAME_LONMIN), 1 TSRMLS_CC));

  GeoHashArea area;
  if(geohash_decode(lat_range, lon_range, hash, &area) == -1)
  {
    zend_throw_exception(geohash_exception_ce, "expect `decode` failed", 500 TSRMLS_CC);
    return;
  }

  array_init(return_value);
  add_assoc_double(return_value, "latitude", (area.latitude.max+area.latitude.min)/2);
  add_assoc_double(return_value, "longitude", (area.longitude.max+area.longitude.min)/2);
  add_assoc_double(return_value, "lat_min", area.latitude.min);
  add_assoc_double(return_value, "lat_max", area.latitude.max);
  add_assoc_double(return_value, "lon_min", area.longitude.min);
  add_assoc_double(return_value, "lon_max", area.longitude.max);
}
/* }}} */

/** {{{ proto public Geohash::fastDecode(hash, step=26)
*/
PHP_METHOD(Geohash, fastDecode) {
  long hashbit;
  long step = 26;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|l", &hashbit, &step) == FAILURE) {
    return;
  }

  GeoHashBits hash;
  hash.step = step;
  hash.bits = hashbit;

  GeoHashRange lat_range, lon_range;
  lat_range.max = Z_DVAL_P(zend_read_property(geohash_ce, getThis(), ZEND_STRL(GEOHASH_PROPERTY_NAME_LATMAX), 1 TSRMLS_CC));
  lat_range.min = Z_DVAL_P(zend_read_property(geohash_ce, getThis(), ZEND_STRL(GEOHASH_PROPERTY_NAME_LATMIN), 1 TSRMLS_CC));
  lon_range.max = Z_DVAL_P(zend_read_property(geohash_ce, getThis(), ZEND_STRL(GEOHASH_PROPERTY_NAME_LONMAX), 1 TSRMLS_CC));
  lon_range.min = Z_DVAL_P(zend_read_property(geohash_ce, getThis(), ZEND_STRL(GEOHASH_PROPERTY_NAME_LONMIN), 1 TSRMLS_CC));

  GeoHashArea area;
  if(geohash_fast_decode(lat_range, lon_range, hash, &area) == -1)
  {
    zend_throw_exception(geohash_exception_ce, "expect `fastDecode` failed", 500 TSRMLS_CC);
    return;
  }

  array_init(return_value);
  add_assoc_double(return_value, "latitude", (area.latitude.max+area.latitude.min)/2);
  add_assoc_double(return_value, "longitude", (area.longitude.max+area.longitude.min)/2);
  add_assoc_double(return_value, "lat_min", area.latitude.min);
  add_assoc_double(return_value, "lat_max", area.latitude.max);
  add_assoc_double(return_value, "lon_min", area.longitude.min);
  add_assoc_double(return_value, "lon_max", area.longitude.max);
}
/* }}} */

/** {{{ proto public Geohash::neighbors(hash, step=26)
*/
PHP_METHOD(Geohash, neighbors) {
  long hashbit;
  long step = 26;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|l", &hashbit, &step) == FAILURE) {
    return;
  }

  GeoHashBits hash;
  hash.step = step;
  hash.bits = hashbit;

  GeoHashNeighbors neighbors;
  if(geohash_get_neighbors(hash, &neighbors) == -1)
  {
    zend_throw_exception(geohash_exception_ce, "expect `neighbors` failed", 500 TSRMLS_CC);
    return;
  }

  array_init(return_value);
  add_assoc_long(return_value, "center", hash.bits);
  add_assoc_long(return_value, "east", neighbors.east.bits);
  add_assoc_long(return_value, "west", neighbors.west.bits);
  add_assoc_long(return_value, "south", neighbors.south.bits);
  add_assoc_long(return_value, "north", neighbors.north.bits);
  add_assoc_long(return_value, "north_west", neighbors.north_west.bits);
  add_assoc_long(return_value, "north_east", neighbors.north_east.bits);
  add_assoc_long(return_value, "south_east", neighbors.south_east.bits);
  add_assoc_long(return_value, "south_west", neighbors.south_west.bits);
}
/* }}} */

/** {{{ proto public Geohash::stepInRadius(meter)
*/
PHP_METHOD(Geohash, stepInRadius) {
  double radius;
  int step;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &radius) == FAILURE) {
    return;
  }

  step = steps_in_meters(radius);

  RETURN_LONG(step);
}
/* }}} */

/** {{{ proto public Geohash::radiusSearch(lat, lon, meter, neighbor=false)
*/
PHP_METHOD(Geohash, radiusSearch) {
  double latitude;
  double longtitude;
  double radius;
  zend_bool neighbor = 0;
  int step;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddd|b", &latitude, &longtitude, &radius, &neighbor) == FAILURE) {
    return;
  }

  step = steps_in_meters(radius);

  GeoHashRange lat_range, lon_range;
  lat_range.max = Z_DVAL_P(zend_read_property(geohash_ce, getThis(), ZEND_STRL(GEOHASH_PROPERTY_NAME_LATMAX), 1 TSRMLS_CC));
  lat_range.min = Z_DVAL_P(zend_read_property(geohash_ce, getThis(), ZEND_STRL(GEOHASH_PROPERTY_NAME_LATMIN), 1 TSRMLS_CC));
  lon_range.max = Z_DVAL_P(zend_read_property(geohash_ce, getThis(), ZEND_STRL(GEOHASH_PROPERTY_NAME_LONMAX), 1 TSRMLS_CC));
  lon_range.min = Z_DVAL_P(zend_read_property(geohash_ce, getThis(), ZEND_STRL(GEOHASH_PROPERTY_NAME_LONMIN), 1 TSRMLS_CC));

  GeoHashBits fast_hash;
  if(geohash_fast_encode(lat_range, lon_range, latitude, longtitude, step, &fast_hash) == -1) {
    zend_throw_exception(geohash_exception_ce, "expect `fast encode` failed", 500 TSRMLS_CC);
    return;
  }

  array_init(return_value);
  add_assoc_long(return_value, "min", fast_hash.bits << (52-step*2));
  add_assoc_long(return_value, "max", (fast_hash.bits+1) << (52-step*2));

  if(0 == neighbor) {
    return;
  }

  GeoHashNeighbors neighbors;
  if(geohash_get_neighbors(fast_hash, &neighbors) == -1)
  {
    zend_throw_exception(geohash_exception_ce, "expect `neighbors` failed", 500 TSRMLS_CC);
    return;
  }

  add_assoc_long(return_value, "e-min", neighbors.east.bits << (52-step*2));
  add_assoc_long(return_value, "e-max", (neighbors.east.bits+1) << (52-step*2));
  add_assoc_long(return_value, "w-min", neighbors.west.bits << (52-step*2));
  add_assoc_long(return_value, "w-max", (neighbors.west.bits+1) << (52-step*2));
  add_assoc_long(return_value, "s-min", neighbors.south.bits << (52-step*2));
  add_assoc_long(return_value, "s-max", (neighbors.south.bits+1) << (52-step*2));
  add_assoc_long(return_value, "n-min", neighbors.north.bits << (52-step*2));
  add_assoc_long(return_value, "n-max", (neighbors.north.bits+1) << (52-step*2));
  add_assoc_long(return_value, "nw-min", neighbors.north_west.bits << (52-step*2));
  add_assoc_long(return_value, "nw-max", (neighbors.north_west.bits+1) << (52-step*2));
  add_assoc_long(return_value, "ne-min", neighbors.north_east.bits << (52-step*2));
  add_assoc_long(return_value, "ne-max", (neighbors.north_east.bits+1) << (52-step*2));
  add_assoc_long(return_value, "se-min", neighbors.south_east.bits << (52-step*2));
  add_assoc_long(return_value, "se-max", (neighbors.south_east.bits+1) << (52-step*2));
  add_assoc_long(return_value, "sw-min", neighbors.south_west.bits << (52-step*2));
  add_assoc_long(return_value, "sw-max", (neighbors.south_west.bits+1) << (52-step*2));
}
/* }}} */
