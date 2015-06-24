--TEST--
Check for extenion
--SKIPIF--
<?php if (!extension_loaded("geohash")) print "skip"; ?>
--FILE--
<?php 
echo "geohash extension is available";
?>
--EXPECT--
geohash extension is available