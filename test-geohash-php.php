<?php
// program mark - basic test
$start_m = memory_get_usage();

$hashbit_1 = Geohash::getInstance()->encode(52.53261, 13.3777, 24);
echo "Geohash::encode(52.53261, 13.3777, 24) : ".($hashbit_1)."\n";

$hashbit_2 = Geohash::getInstance()->fastEncode(52.53261, 13.3777);
echo "Geohash::fastEncode(52.53261, 13.3777) : ".($hashbit_2)."\n";

$area_1 = Geohash::getInstance()->decode($hashbit_1, 24);
echo "Geohash::decode($hashbit_1, 24):\n";
var_dump($area_1);

$area_2 = Geohash::getInstance()->fastDecode($hashbit_2);
echo "Geohash::fastDecode($hashbit_2):\n";
var_dump($area_2);

$neighbors = Geohash::getInstance()->neighbors($hashbit_1, 24);
echo "Geohash::neighbors($hashbit_1, 24):\n";
var_dump($neighbors);

// program mark - real search test 52.53261	
$steps = Geohash::getInstance()->stepInRadius(10000);
echo "Geohash::stepInRadius(10000) : ".($steps)."\n";

$hashbit = Geohash::getInstance()->encode(52.53261, 13.3777, $steps);
$hashbit_1 = $hashbit + 1;
echo "Geohash::encode(52.53261, 13.3777, $steps) : ".($hashbit)."\n";
echo "$hashbit + 1 : ".($hashbit_1)."\n";

$hashbit_shift = $hashbit << (52-$steps*2);
$hashbit_1_shift = $hashbit_1 << (52-$steps*2);
echo "$hashbit << (52-$steps*2) : ".($hashbit_shift)."\n";
echo "$hashbit_1 << (52-$steps*2) : ".($hashbit_1_shift)."\n";

$ret = Geohash::getInstance()->radiusSearch(52.53261, 13.3777, 10000, true);
var_dump($ret);

$hashbit_2 = Geohash::getInstance()->encode(52.53261, 13.3777);
echo "Geohash::encode(52.53261, 13.3777) : ".($hashbit_2)."\n";

$end_m = memory_get_usage();
echo ($end_m-$start_m) . "\n";

