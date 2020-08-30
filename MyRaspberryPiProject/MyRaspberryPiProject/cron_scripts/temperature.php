<html>
<head>
<meta http-equiv="cache-control" content="max-age=0" />
<meta http-equiv="cache-control" content="no-cache" />
<meta http-equiv="expires" content="0" />
<meta http-equiv="expires" content="Tue, 01 Jan 1980 1:00:00 GMT" />
<meta http-equiv="pragma" content="no-cache" />
</head>
<body>
<h1>Analyse w1_slave</h1>
<?php

$temperature = 0.0;

$buffer = file_get_contents('/sys/bus/w1/devices/10-000802a0b2f3/w1_slave');

echo $buffer;

$string_to_search = 't=';
$pos = strpos($buffer, $string_to_search);
$buffer = substr($buffer, $pos + strlen($string_to_search));
echo '<br />';
echo $buffer;

echo '<br />';

$temperature = $buffer;
$temperature = $temperature / 1000;
echo '<font color=green>';
echo $temperature;
echo '&nbsp;milli degrés Celsius</font>';
echo '<br />';

echo '<br />';

// Record in database
require_once("/var/www/database_use_start.php");

$query = sprintf("insert into temperature (temperature)
	values (%f)",
	$temperature);
$result = mysql_query($query) or printf('Erreur SQL ! '.$query.'<br />'.mysql_error());

require_once("/var/www/database_use_stop.php");
?>
</body>
</html>