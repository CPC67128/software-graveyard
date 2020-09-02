<?php
function InsertCommand($day, $time, $command)
{
	include '../configuration/configuration.php';

	$dns = 'mysql:host=' . $DB_HOST . ';dbname=' . $DB_NAME;
	$utilisateur = $DB_USER;
	$motDePasse = $DB_PASSWORD;
	$connection = new PDO( $dns, $utilisateur, $motDePasse );
	$connection->exec("SET CHARACTER SET utf8");

	$sql = sprintf("insert into pi_schedule (creation_date, day, time, command) values (now(), %s, %s, '%s')",
			$day,
			empty($time) ? 'null' : "'".$time."'",
			$command);
	$connection->query($sql) or die('Erreur SQL ! '.$query.'<br />'.mysql_error());
	
	$connection = null;
}

function GetScheduledCommands()
{
	include '../configuration/configuration.php';

	$dns = 'mysql:host=' . $DB_HOST . ';dbname=' . $DB_NAME;
	$utilisateur = $DB_USER;
	$motDePasse = $DB_PASSWORD;
	$connection = new PDO( $dns, $utilisateur, $motDePasse );
	$connection->exec("SET CHARACTER SET utf8");

	$sql = sprintf("select * from pi_schedule where day >= 0");
	$result = $connection->query($sql) or die('Erreur SQL ! '.$query.'<br />'.mysql_error());

	return $result;
}

function DeleteCommand($id)
{
	include '../configuration/configuration.php';

	$dns = 'mysql:host=' . $DB_HOST . ';dbname=' . $DB_NAME;
	$utilisateur = $DB_USER;
	$motDePasse = $DB_PASSWORD;
	$connection = new PDO( $dns, $utilisateur, $motDePasse );
	$connection->exec("SET CHARACTER SET utf8");

	$sql = sprintf("delete from pi_schedule where id = %s",
			$id);
	$connection->query($sql) or die('Erreur SQL ! '.$query.'<br />'.mysql_error());

	$connection = null;
}

function SetCommandConfiguration($command, $gpio)
{
	include '../configuration/configuration.php';

	$dns = 'mysql:host=' . $DB_HOST . ';dbname=' . $DB_NAME;
	$utilisateur = $DB_USER;
	$motDePasse = $DB_PASSWORD;
	$connection = new PDO( $dns, $utilisateur, $motDePasse );
	$connection->exec("SET CHARACTER SET utf8");

	$sql = sprintf("delete from pi_command_configuration where command = '%s'",
			$command);
	$connection->query($sql) or die('Erreur SQL ! '.$query.'<br />'.mysql_error());
	$sql = sprintf("insert into pi_command_configuration (command, gpio) values ('%s', %s)",
			$command,
			$gpio);
	$connection->query($sql) or die('Erreur SQL ! '.$query.'<br />'.mysql_error());
	
	$connection = null;
}

function GetCommandConfiguration($command)
{
	include '../../configuration.php';

	$dns = 'mysql:host=' . $DB_HOST . ';dbname=' . $DB_NAME;
	$utilisateur = $DB_USER;
	$motDePasse = $DB_PASSWORD;
	$connection = new PDO( $dns, $utilisateur, $motDePasse );
	$connection->exec("SET CHARACTER SET utf8");

	$sql = sprintf("select * from pi_command_configuration where command = '%s'",
			$command);
	$result = $connection->query($sql) or die('Erreur SQL ! '.$query.'<br />'.mysql_error());
	
	$returnValue = 1;
	if (isset($result))
	{
		$row = $result->fetch();
		$returnValue = $row['gpio'];
	}
	$connection = null;

	return $returnValue;
}

/*update pi_schedule
set next_execution = CONCAT_WS(' ', date( date_add(now(), interval ((7-weekday(now())-1) + (day)) day) ), time )
where next_execution is null*/
