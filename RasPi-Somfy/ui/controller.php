<?php
include 'DB.php';

$action = $_POST['action'];

if ($action == 'command')
{
	InsertCommand(-1, null, $_POST['command']);
}
else if ($action == 'schedule_command')
{
	InsertCommand($_POST['day'], $_POST['time'], $_POST['command']);
}
else if ($action == 'delete_schedule')
{
	DeleteCommand($_POST['id']);
}
else if ($action == 'command_configuration')
{
	SetCommandConfiguration($_POST['command'], $_POST['gpio']);
}