<?php
require_once("database_use_start.php");

$query = 'select * from temperature where creation_date > (CURDATE() - INTERVAL 6 DAY) ';
$result = mysql_query($query) or die('Erreur SQL ! '.$query.'<br />'.mysql_error());

$disk_free = array();
$x_disk_free = array();

$i=0;

while ($row = mysql_fetch_assoc($result))
{
	$disk_free[$i] = $row['temperature'];
	$x_disk_free[$i] = strtotime($row['creation_date']);

	++$i;
}

require_once("jpgraph/src/jpgraph.php");
require_once("jpgraph/src/jpgraph_line.php");
require_once("jpgraph/src/jpgraph_date.php");

// Create the new graph
$graph = new Graph(900,450);
 
// Slightly larger than normal margins at the bottom to have room for
// the x-axis labels
$graph->SetMargin(50,10,30,130);

// Fix the Y-scale to go between [0,100] and use date for the x-axis
$graph->SetScale('datint', 0, 50);
$graph->title->Set("Temperature over last 6 days (in Celsius degree)");
$graph->legend->SetPos(0, 0, 'right','top');

// Set the angle for the labels to 90 degrees
$graph->xaxis->SetLabelAngle(90);
 
$line = new LinePlot($disk_free,$x_disk_free);
//$line->SetLegend('Available Space / Total: '.floor($disk_total));
$line->SetFillColor('lightred@0.5');
$graph->Add($line);

$graph->Stroke();

require_once("database_use_stop.php");

?>