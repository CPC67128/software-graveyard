<!DOCTYPE html>
<?php
include_once '../configuration/configuration.php';
include_once 'enums.php';
include_once 'DB.php';
?>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">
    <link rel="icon" href="favicon.ico">

    <title>Commande Volets Somfy</title>

<!-- Latest compiled and minified CSS -->
<link rel="stylesheet" href="bootstrap-3.3.7-dist/css/bootstrap.min.css">

<!-- Optional theme -->
<link rel="stylesheet" href="bootstrap-3.3.7-dist/css/bootstrap-theme.min.css">

    <!-- Custom styles for this template -->
    <link href="index.css" rel="stylesheet">

    <!-- HTML5 shim and Respond.js IE8 support of HTML5 elements and media queries -->
    <!--[if lt IE 9]>
      <script src="https://oss.maxcdn.com/html5shiv/3.7.2/html5shiv.min.js"></script>
      <script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
    <![endif]-->

    <script src="index.js"></script>
  </head>

  <body>

    <div class="navbar navbar-inverse navbar-fixed-top" role="navigation">
      <div class="container-fluid">
        <div class="navbar-header">
          <a class="navbar-brand" href="#">Commande Volets Somfy</a>
        </div>
        <div class="navbar-collapse collapse">
          <ul class="nav navbar-nav navbar-right">
            <li><a href="http://stevefuchs.fr">@ Steve Fuchs</a></li>
            <!--  <li><a href="https://projects.drogon.net/raspberry-pi/wiringpi/the-gpio-utility/">GPIO @ Gordons Projects</a></li>  -->
          </ul>
        </div>
      </div>
    </div>

<div class="container">

<div class="row">

<!-- ---------------------------------------------------------------- -->
<a name="commands"></a>
<h1>Actions</h1>
<?php foreach ($commands as $command) { ?>
<button type="button" class="btn btn-<?= $commandsClass[$command] ?> btn-lg" onclick="CreateCommand('<?= $command ?>');"><?= $commandsText[$command] ?></button>
<?php } ?>
<br />


</div>

<div class="row">
<!-- ---------------------------------------------------------------- -->
<a name="planning"></a>
<h1>Planning</h1>
</div>

<div class="row">
<div id="webpartSchedule"></div>
</div>

<div class="row">
<form class="form-inline">
  <div class="form-group">
    <label for="scheduleDay">Jour</label>
    <select id="scheduleDay" class="form-control">
		<?php foreach ($days as $dayNumber => $day) { ?>
		<option value="<?= $dayNumber ?>"><?= $day ?></option>
		<?php } ?>
	</select>
  </div>
  <div class="form-group">
    <label for="scheduleTime">Heure</label>
	<select id="scheduleTime" class="form-control">
		<?php for ($hour = 0; $hour <= 23; $hour++) {
			$time = $hour.':00'; ?>
		<option value="<?= $time ?>" <?= $time=='8:00' ? 'selected' : '' ?>><?= $time ?></option>
		<?php } ?>
	</select>
  </div>
  <div class="form-group">
    <label for="scheduleCommand">Action</label>
	<select id="scheduleCommand" class="form-control">
		<?php foreach ($commands as $command) { ?>
		<option value="<?= $command ?>"><?= $commandsText[$command] ?></option>
		<?php } ?>
	</select>
  </div>
  <button type="button" class="btn btn-default" onclick="CreateSchedule();" id="btnCreateSchedule">Ajouter</button>
</form>

<div id="createScheduleResult"></div>

        </div>
</div>

    <!-- Bootstrap core JavaScript
    ================================================== -->
<!-- jQuery (necessary for Bootstrap's JavaScript plugins) -->
<script src="js/jquery-3.1.1.min.js"></script>

<!-- Latest compiled and minified JavaScript -->
<script src="bootstrap-3.3.7-dist/js/bootstrap.min.js" ></script>

    <script src="js/docs.min.js"></script>
    <!-- IE10 viewport hack for Surface/desktop Windows 8 bug -->
    <script src="js/ie10-viewport-bug-workaround.js"></script>
  </body>
  <script>
  RefreshListOfSchedules();
  </script>
</html>
