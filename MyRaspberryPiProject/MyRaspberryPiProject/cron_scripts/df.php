<html>
<head>
<meta http-equiv="cache-control" content="max-age=0" />
<meta http-equiv="cache-control" content="no-cache" />
<meta http-equiv="expires" content="0" />
<meta http-equiv="expires" content="Tue, 01 Jan 1980 1:00:00 GMT" />
<meta http-equiv="pragma" content="no-cache" />
<meta charset="UTF-8">
</head>
<body>
<h1>Analyse df.out</h1>
<?php

$file_system_total = 0;
$file_system_free = 0;

$regex = "/(?P<filesys>\S+)[\ ]+(?P<blocks>\d+)[\ ]+(?P<used>\d+)[\ ]+(?P<available>\d+)[\ ]+(?P<usedpercentage>\S+)[\ ]+(?P<mount>\S+)/";

/*Ouverture du fichier en lecture seule*/
$handle = fopen('/var/www/cron_scripts/df.out', 'r');

/*Si on a réussi à ouvrir le fichier*/
$first_line_read = false;
if ($handle)
{
	/*Tant que l'on est pas à la fin du fichier*/
	while (!feof($handle))
	{
		/*On lit la ligne courante*/
		$buffer = fgets($handle);

		/*On l'affiche*/
		echo $buffer;

		if ($first_line_read)
		{
			AnalyzeLine($buffer);
		}
		else
		{
			$first_line_read = true;
			echo '<br />';
		}
	}
	/*On ferme le fichier*/
	fclose($handle);
}

function AnalyzeLine($line)
{
	global $regex;
	global $file_system_total, $file_system_free;
	preg_match($regex, $line, $chars);
	echo '<br />';
	echo '<font color=red>';
	print_r($chars);
	echo '</font>';
	echo '<br />';
	echo '<font color=green>';
	echo $chars['filesys'].':';
	echo $chars['used'].' used - ';
	echo $chars['available'].' available /';
	echo $chars['blocks'];
	echo '</font>';
	echo '<br />';

	switch ($chars['filesys'])
	{
		case 'rootfs': $file_system_total = $chars['blocks']; $file_system_free = $chars['available']; break;
	}
}

echo '<br />';
echo '<br />';
echo 'total(kB)='. $file_system_total;
echo '<br />';
echo 'free(kB)='. $file_system_free;
echo '<br />';

// Record in database
require_once("/var/www/database_use_start.php");

$query = sprintf("insert into disk (disk_total, disk_free)
	values (%d, %d)",
	$file_system_total,
	$file_system_free);
echo $query;
echo '<br />';
$result = mysql_query($query) or printf('Erreur SQL ! '.$query.'<br />'.mysql_error());

require_once("/var/www/database_use_stop.php");
?>
</body>
</html>