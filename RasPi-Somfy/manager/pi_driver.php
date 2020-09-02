<?php
include '../configuration/configuration.php';

$dns = 'mysql:host=' . $DB_HOST . ';dbname=' . $DB_NAME;
$utilisateur = $DB_USER;
$motDePasse = $DB_PASSWORD;
$connection = new PDO( $dns, $utilisateur, $motDePasse );
$connection->exec("SET CHARACTER SET utf8");

function CalculateNextExecutionDatesOfScheduledActions()
{
	global $connection;
	$sql = sprintf("update pi_schedule set next_execution = CONCAT_WS(' ', date( date_add(now(), interval ((7-weekday(now())-1) + (day)) day) ), time ) where next_execution is null and day > 0");
	$result = $connection->query($sql) or die('Erreur SQL ! '.$query.'<br />'.mysql_error());

	/**
	select `id`, `creation_date`, `day`, `time`, `command`, `last_execution`, `next_execution`,
	CONCAT_WS(' ', date( date_add(now(), interval ((7-weekday(now())-1) + (day)) day) ), time ) as CalculatedNextExecution
	from pi_schedule
	where next_execution is null and day > 0
	*/
}

function InvalidateUnnecessaryActions()
{
	global $connection;
	$sql = sprintf("update pi_schedule set next_execution = CONCAT_WS(' ', date( date_add(now(), interval ((7-weekday(now())-1) + (day)) day) ), time ) where next_execution is null and day > 0");
	$result = $connection->query($sql) or die('Erreur SQL ! '.$query.'<br />'.mysql_error());

	/**
	 select `id`, `creation_date`, `day`, `time`, `command`, `last_execution`, `next_execution`,
	 CONCAT_WS(' ', date( date_add(now(), interval ((7-weekday(now())-1) + (day)) day) ), time ) as CalculatedNextExecution
	 from pi_schedule
	 where next_execution is null and day > 0
	*/
}

function GetFirstActivity()
{
	global $connection;
	$sql = "select *
			from pi_schedule
			where (day = -1 and last_execution is null)
			or next_execution < now()
			order by creation_date desc
			limit 1";
	$result = $connection->query($sql) or die('Erreur SQL ! '.$query.'<br />'.mysql_error());

	$row = $result->fetch();
	return $row;
}

function AknowledgeActivity()
{
	global $connection;
	$sql = sprintf("update pi_schedule
			set next_execution = null, last_execution = now()
			where (day = -1 and last_execution is null)
			or next_execution < now()");
	$connection->query($sql) or die('Erreur SQL ! '.$query.'<br />'.mysql_error());
}

function Stop()
{
	$connection = null;
	exit(0);
}

function DoCommand($command)
{
	global $GPIO, $ON_RASPBERRY;

	$gpio = $GPIO[$command];

	echo 'Running GPIO '.$gpio.'<br />';

	if ($ON_RASPBERRY)
	{
		system('gpio mode '.$gpio.' out');
		system('gpio write '.$gpio.' 0');
		system('gpio write '.$gpio.' 1');
		system('sleep 1');
		system('gpio write '.$gpio.' 0');
	}
}

// ------- Processing

echo "Calculate next execution dates of scheduled actions...<br />";
CalculateNextExecutionDatesOfScheduledActions();

echo "Get first activity...<br/>";
$row = GetFirstActivity();

if (empty($row))
{
	echo "Nothing to do<br/>";
	Stop();
}

echo "Doing ";
echo 'id='.$row['id'].', command='.$row['command'].'<br/>';

DoCommand($row['command']);

echo "Acknowledging...<br/>";
AknowledgeActivity();

echo "Done";