<?php
// program mark - basic test
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

// program mark - initialize geo_hash bits in db
// try {
//     $dbh = new PDO('mysql:dbname=test;host=127.0.0.1;','root','');

//     //$sth = $dbh->prepare("select id,lat,lon, geo_id from users_geo_test where geo_hash=0 and lat<>0 and lon<>0 group by geo_id;");
//     $sth = $dbh->prepare("select id,lat,lon, geo_id from users_geo_test where geo_hash=0 and geo_id>0 group by geo_id;");
//     $sth->execute();

//     while($row = $sth->fetch(PDO::FETCH_ASSOC))
//     {
//     	$hashbit = Geohash::getInstance()->encode($row['lat'], $row['lon']);
//     	if($hashbit)
//     	{
//     		error_log("update users_geo_test set geo_hash={$hashbit} where geo_id={$row['geo_id']};\n", 3, "test_update_sql.sql");
//     	}

//     	unset($hashbit);
//     	unset($row);
//     }

// } catch (PDOException $e) {
//     echo 'Connection failed: ' . $e->getMessage();
// }

// program mark - test range search
// try {
//     $dbh = new PDO('mysql:dbname=test;host=127.0.0.1;','root','');

//     $sth =$dbh->prepare("select id,lat,lon from users_geo_test where id=1971;");
//     $sth->execute();
//     $user=$sth->fetch(PDO::FETCH_ASSOC);
//     if($user)
//     {
//     	radiusSearch(5000, $user);
//     	radiusSearch(10000, $user);
//     	radiusSearch(20000, $user);
//     	radiusSearch(30000, $user);
//     	radiusSearch(50000, $user);
//     	radiusSearch(100000, $user);
//     	radiusSearch(200000, $user);
//     	radiusSearch(500000, $user);
//     }

// } catch (PDOException $e) {
//     echo 'Connection failed: ' . $e->getMessage();
// }

// function radiusSearch($meter, $params)
// {
// 	$steps = Geohash::getInstance()->stepInRadius($meter);
// 	echo "{$steps} with meter {$meter} \r\n";
// 	$hashbit = Geohash::getInstance()->encode($params['lat'], $params['lon'], $steps);
// 	$hashbit_1 = $hashbit + 1;
// 	$hashbit_shift = $hashbit << (52-$steps*2);
// 	$hashbit_1_shift = $hashbit_1 << (52-$steps*2);
// 	echo "select * from users_geo_test where geo_hash>={$hashbit_shift} and geo_hash<={$hashbit_1_shift};\r\n";
// }
