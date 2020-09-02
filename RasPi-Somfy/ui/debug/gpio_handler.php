<?php

$action = $_POST['action'];
$gpio = $_POST['gpio'];
$value = $_POST['value'];

if ($action == 'write')
{
	system('gpio mode '.$gpio.' out');
	system('gpio write '.$gpio.' '.$value);
}
else if ($action == 'pulse')
{
	system('gpio mode '.$gpio.' out');
	system('gpio write '.$gpio.' 0');
	system('gpio write '.$gpio.' 1');
	system('sleep 2');
	system('gpio write '.$gpio.' 0');
}
