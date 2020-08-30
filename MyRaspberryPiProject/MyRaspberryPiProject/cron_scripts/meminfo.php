<html>
<head>
<meta http-equiv="cache-control" content="max-age=0" />
<meta http-equiv="cache-control" content="no-cache" />
<meta http-equiv="expires" content="0" />
<meta http-equiv="expires" content="Tue, 01 Jan 1980 1:00:00 GMT" />
<meta http-equiv="pragma" content="no-cache" />
</head>
<body>
<h1>Analyse /proc/meminfo</h1>
<?php

$mem_total = 0;
$mem_free = 0;
$swap_total = 0;
$swap_free = 0;

$regex = "/(?P<name>\S+):[\ ]+(?P<quantity>\d+) (?P<uom>\w+)/";
// "/(\S+):[\ ]+(\d+) (\w+)/";

/*Ouverture du fichier en lecture seule*/
$handle = fopen('/proc/meminfo', 'r');

/*Si on a réussi à ouvrir le fichier*/
if ($handle)
{
	/*Tant que l'on est pas à la fin du fichier*/
	while (!feof($handle))
	{
		/*On lit la ligne courante*/
		$buffer = fgets($handle);
		/*On l'affiche*/
		echo $buffer;
		AnalyzeLine($buffer);
	}
	/*On ferme le fichier*/
	fclose($handle);
}

function AnalyzeLine($line)
{
	global $regex;
	global $mem_total, $mem_free, $swap_total, $swap_free;
	preg_match($regex, $line, $chars);
	echo '<br />';
	echo '<font color=red>';
	print_r($chars);
	echo '</font>';
	echo '<br />';
	echo '<font color=green>';
	echo $chars['name'].'=';
	switch ($chars['uom'])
	{
		case 'kB': $multiplicator = 1024; break;
		case 'mB': $multiplicator = 1048576; break;
		case 'gB': $multiplicator = 1073741824; break;
		default: $multiplicator = 1; break;
	}
	$quantity = $chars['quantity'] * $multiplicator;
	echo $quantity;
	echo '</font>';
	echo '<br />';

	switch ($chars['name'])
	{
		case 'MemTotal': $mem_total = $quantity; break;
		case 'MemFree': $mem_free = $quantity; break;
		case 'SwapTotal': $swap_total = $quantity; break;
		case 'SwapFree': $swap_free = $quantity; break;
	}
}

$mem_total = $mem_total / 1048576; //Conversion in mB
$mem_free = $mem_free / 1048576; //Conversion in mB
$swap_total = $swap_total / 1048576; //Conversion in mB
$swap_free = $swap_free / 1048576; //Conversion in mB

echo '<br />';
echo '<br />';
echo 'mem_total(mB)='. $mem_total;
echo '<br />';
echo 'mem_free(mB)='. $mem_free;
echo '<br />';

// Record in database
require_once("/var/www/database_use_start.php");

$query = sprintf("insert into memory (mem_total, mem_free, swap_total, swap_free)
	values (%f, %f, %f, %f)",
	$mem_total,
	$mem_free,
	$swap_total,
	$swap_free);
$result = mysql_query($query) or printf('Erreur SQL ! '.$query.'<br />'.mysql_error());

require_once("/var/www/database_use_stop.php");
?>
</body>
</html>