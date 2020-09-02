<!DOCTYPE html>
<?php include_once '../configuration.php'; ?>
<html lang="fr">
<head>
<meta charset="utf-8">
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta name="description" content="Page de diagnostic pour lire l'état des GPIO">
<meta name="author" content="Steve Fuchs">
<link rel="icon" href="../favicon.ico">

<script src="../js/iohttpcommander.js"></script>

<title>RasPi-Somfy : Actions GPIO</title>
</head>

<body>

<?php if ($ON_RASPBERRY) { ?>
<h1 class="page-header">Actions GPIO</h1>
<div class="table-responsive">
<table class="table table-striped">
<thead>
<tr>
<th>Name</th>
<th>Value</th>
</tr>
</thead>
<tbody>
<?php for ($i = 1; $i <= 7; $i++) { ?>
<tr>
<td>GPIO. <?= $i ?></td>
<td><button onclick="WriteGPIO(<?= $i ?>, 1);">1</button>&nbsp;<button onclick="WriteGPIO(<?= $i ?>, 0);">0</button>&nbsp;<button onclick="PulseGPIO(<?= $i ?>, 0);">Pulse</button></td>
</tr>
<?php } ?>
</tbody>
</table>
</div>

<?php } else { ?>
<p>Not on a Pi !</p>
<?php } ?>
</body>
</html>