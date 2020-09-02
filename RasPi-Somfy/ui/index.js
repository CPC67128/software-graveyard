function WriteGPIO(pGpio, pValue)
{
	$.post (
			'gpio_handler.php',
			{
				action: 'write',
				gpio: pGpio,
				value: pValue
			},
			function(response, status) {
			}
	);
}

function PulseGPIO(pGpio, pValue)
{
	$.post (
			'gpio_handler.php',
			{
				action: 'pulse',
				gpio: pGpio,
				value: pValue
			},
			function(response, status) {
			}
	);
}

function SendCommand(pCommand)
{
	$.post ('controller.php',
			{
				action: 'command',
				command: pCommand
			},
			function(response, status) {
			}
	);
}

function CreateCommand(pCommand)
{
	$.post ('controller.php',
			{
				action: 'command',
				command: pCommand
			},
			function(response, status) {
			}
	);
}

function DeleteSchedule(id)
{
	$.post ('controller.php',
			{
				action: 'delete_schedule',
				id: id
			},
			function(response, status) {
				RefreshListOfSchedules();
			}
	);
}

function CreateSchedule() {
	//document.getElementById("btnCreateSchedule").disabled = true;

	var scheduleDay = document.getElementById("scheduleDay").value;
	var scheduleTime = document.getElementById("scheduleTime").value;
	var scheduleCommand = document.getElementById("scheduleCommand").value;

	$.post ('controller.php',
			{
				action: 'schedule_command',
				day: scheduleDay,
				time: scheduleTime, 
				command: scheduleCommand
			},
		function(response, status) {
			$("#createScheduleResult").stop().show();
			$("#createScheduleResult").html("");

			if (status == 'success') {
				if (response.indexOf("<!-- ERROR -->") >= 0) {
					$("#createScheduleResult").html(response);
				}
				else {
					RefreshListOfSchedules();
				}
			}
			else {
				$("#createScheduleResult").html("UNEXPECTED ERROR / Status = " + status);
			}
			document.getElementById("btnCreateSchedule").disabled = false;
		}
	);
}

function RefreshListOfSchedules()
{
	$.ajax({
		type : 'POST',
		url : 'webpart_schedule.php',
		data: {},
		dataType: 'html',
		success : function(data) {
			$('#webpartSchedule').html(data);
		}
	});
}

function SetCommandConfiguration(command, gpio)
{
	$.post ('controller.php',
			{
				action: 'command_configuration',
				command: command,
				gpio: gpio
			},
			function(response, status) {
			}
	);
}

