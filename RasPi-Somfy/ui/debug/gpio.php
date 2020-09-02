<!DOCTYPE html>
<?php include_once '../configuration.php'; ?>
<html lang="fr">
<head>
<meta charset="utf-8">
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta name="description" content="Page de diagnostic pour lire l'état des IO">
<meta name="author" content="Steve Fuchs">
<link rel="icon" href="../favicon.ico">

<title>RasPi-Somfy : GPIO</title>
</head>

<body>

<?php if ($ON_RASPBERRY) { ?>
<h1 class="page-header">Situation en direct</h1>

<font face="Courier New">
<?php
// Needs to buffer output of command gpio readall
ob_start();
system('gpio readall');

$readall = ob_get_clean();

$readall = str_replace(array("\r\n", "\n", "\r"), "<br/>", $readall);
$readall = str_replace(" ", "&nbsp;", $readall);
echo($readall);
?>
</font>

<?php } else { ?>
<p>Not on a Pi !</p>
<?php } ?>
</body>
</html>