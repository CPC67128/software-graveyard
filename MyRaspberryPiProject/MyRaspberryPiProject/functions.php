<?php

function String2StringForSprintfQueryBuilder($String)
{
	if (get_magic_quotes_gpc())
		return $String;
	else
		return mysql_real_escape_string($String);
}

function RecordConnection()
{
	global $_SERVER;

	include 'database_use_start.php';

	$escaped_browser = String2StringForSprintfQueryBuilder($_SERVER['HTTP_USER_AGENT']);

	$query = sprintf("insert into connection (connection_date_time, ip_address, browser, request_uri) values(now(), '%s', '%s', '%s')",
		$_SERVER['REMOTE_ADDR'],
		$escaped_browser,
		$_SERVER['REQUEST_URI']);
	$result = mysql_query($query) or printf('Erreur SQL ! '.$query.'<br />'.mysql_error());

	include 'database_use_stop.php';
}

