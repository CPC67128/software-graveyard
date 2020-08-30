<?php
require_once("database_use_start.php");

$query = 'select * from memory where creation_date > (CURDATE() - INTERVAL 6 DAY) ';
$result = mysql_query($query) or die('Erreur SQL ! '.$query.'<br />'.mysql_error());

$mem_free = array();
$x_mem_free = array();
$mem_total = 1;

$swap_free= array();
$x_swap_free = array();
$swap_total = 1;

$i=0;

while ($row = mysql_fetch_assoc($result))
{
	$mem_free[$i] = $row['mem_free'];
	if ($mem_total < $row['mem_total'])
		$mem_total = $row['mem_total'];
	$x_mem_free[$i] = strtotime($row['creation_date']);
	//echo $data['mem_free'].'<br />';

	$swap_free[$i] = $row['swap_free'];
	if ($swap_total < $row['swap_total'])
		$swap_total = $row['swap_total'];
	$x_swap_free[$i] = strtotime($row['creation_date']);

	++$i;
}

require_once("jpgraph/src/jpgraph.php");
require_once("jpgraph/src/jpgraph_line.php");
require_once("jpgraph/src/jpgraph_date.php");

// Create the new graph
$graph = new Graph(900,450);
 
// Slightly larger than normal margins at the bottom to have room for
// the x-axis labels
$graph->SetMargin(40,10,30,30);
 
// Fix the Y-scale to go between [0,100] and use date for the x-axis
$graph->SetScale('datint', 0, ($mem_total > $swap_total ? $mem_total : $swap_total));
$graph->title->Set("Memory usage over last 6 days (in mB)");
$graph->legend->SetPos(0, 0, 'right','top');

// Set the angle for the labels to 90 degrees
$graph->xaxis->SetLabelAngle(90);
 
$line = new LinePlot($mem_free,$x_mem_free);
$line->SetLegend('Physical Memory / Total: '.floor($mem_total));
$line->SetFillColor('lightblue@0.5');
$graph->Add($line);

$line = new LinePlot($swap_free,$x_swap_free);
$line->SetLegend('Swap / '.floor($swap_total));
$line->SetFillColor('lightred@0.5');
$graph->Add($line);

$graph->Stroke();

require_once("database_use_stop.php");

?>