<?php include 'common_head_top.php'; ?>
<script src="http://code.jquery.com/jquery-1.9.1.min.js"></script>
<script>
$(function() {
});

function LedOn()
{
	$("#action_text").text('Demande d\'allumage de la LED... Patientez que la caméra se rafraichisse');
	$.ajax({
		type: "GET",
        url : 'action.php',
        data: { type: 1 }
    });
}

function LedOff()
{
	$("#action_text").text('Demande d\'extinction de la LED... Patientez que la caméra se rafraichisse');
	$.ajax({
		type: "GET",
        url : 'action.php',
        data: { type: 2 }
    });
}

function StartWebCam()
{
	$("#startcam_text").text('Démarrage de la cam dans la minute pour 4 minutes... merci de rafraîchir dans 1 minute');
	$.ajax({
		type: "GET",
        url : 'action.php',
        data: { type: 3 }
    });
}

function Refresh()
{
	$.ajax({
        type : 'GET',
        url : 'webcam_live.php',
        dataType: 'html',
        success : function(data) {
            $('#webcamLive').html(data);
        }
    });
}

</script>
<?php include 'common_head_bottom.php'; ?>
<?php include 'common_body_top.php'; ?>

<h2>Actions <a href="#up">(^)</a></h2>
<button onclick="LedOn()">LED on</button>
<button onclick="LedOff()">LED off</button>
<span id="action_text"></span>
<br />

<h2>WebCam Live <a href="#up">(^)</a></h2>
You must be able to reach my port 1935 to access my Pi cam...<br />
<img src="http://raspberrypi.stevefuchs.fr:1935/?action=stream" />
<!-- 
<div id="webcamLive">
</div>
<button onclick="StartWebCam()">Start camera</button>
<span id="startcam_text"></span>
<br />
<button onclick="Refresh()">Refresh</button>
 -->
<?php include 'common_body_bottom.php'; ?>
