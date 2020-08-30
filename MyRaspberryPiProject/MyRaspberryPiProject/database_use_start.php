<?php

$DB_HOST = "localhost";
$DB_NAME = "raspberry";
$DB_USER = "root";
$DB_PASSWORD = "";

$link = mysql_connect ($DB_HOST,$DB_USER,$DB_PASSWORD) or die ('ERREUR '.mysql_error());
mysql_select_db ($DB_NAME) or die ('ERREUR '.mysql_error()); 
mysql_query("SET NAMES 'utf8'");

