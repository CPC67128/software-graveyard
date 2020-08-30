<?php
require_once("functions.php");
RecordConnection();

if (!isset($_GET['type']))
	return;
if ($_GET['type'] == 1)
{
	$last_line = system('gpio mode 0 out', $retval);
	$last_line = system('gpio write 0 1', $retval);
}
elseif ($_GET['type'] == 2)
{
	$last_line = system('gpio mode 0 out', $retval);
	$last_line = system('gpio write 0 0', $retval);
}
elseif ($_GET['type'] == 3)
{
	$last_line = system('touch /tmp/webcam', $retval);
}
elseif ($_GET['type'] == 666)
{
	$last_line = system('touch /tmp/shutdown', $retval);
}
