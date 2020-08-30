<?php include 'common_head_top.php'; ?>
<?php include 'common_head_bottom.php'; ?>
<?php include 'common_body_top.php'; ?>

<h2>Top 15 Processes / Memory usage <a href="#up">(^)</a></h2>
<div class="output">
<?php
$handle = fopen('cron_scripts/ps_mem.out', 'r');
$row_number = 0;
if ($handle)
{
	/*Tant que l'on est pas à la fin du fichier*/
	while (!feof($handle) && $row_number < 16)
	{
		/*On lit la ligne courante*/
		$buffer = fgets($handle);
		/*On l'affiche*/
		$buffer = str_replace(' ', '&nbsp;', $buffer);
		echo $buffer;
		echo '<br />';
		++$row_number;
	}
	/*On ferme le fichier*/
	fclose($handle);
}
?>
</div>

<?php include 'common_body_bottom.php'; ?>