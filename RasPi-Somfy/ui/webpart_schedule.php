<?php
include 'DB.php';
include_once 'enums.php';

$result = GetScheduledCommands();
while ($row = $result->fetch()) {
?>
<div class="col-sm-6 col-md-4 col-lg-3">
<p class="bg-<?= $commandsClass[$row['command']] ?>" style="border-radius: 0.2em; padding-left: 0.2em; padding-right: 0.2em; text-align: center;">
<button onclick="if (confirm('Etes-vous sûr de vouloir supprimer cette entrée ?')) { DeleteSchedule(<?= $row['id'] ?>); }" type="button" class="close" aria-label="Close"><span aria-hidden="true">&times;</span></button>
<?= $days[$row['day']] ?> - <?= $row['time'] ?><br />
<?= $commandsText[$row['command']] ?><br />
<?php if (isset($row['last_execution'])) { ?>Exécuté <?= $row['last_execution'] ?><br /><?php } ?>
<?php if (isset($row['next_execution'])) { ?>Execution <?= $row['next_execution'] ?><br /><?php } ?>
</p>
</div>
<?php } ?>
