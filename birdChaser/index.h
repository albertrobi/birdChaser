const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    body {
      text-align: center;
      font-family: "Trebuchet MS", Arial;
      margin-left:auto;
      margin-right:auto;
    }
    .slider {
      width: 300px;
    }
  </style>
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
</head>
<body onload="getStartXvalue()">
  <h1>ESP32 Bird Chaser</h1>
  <p>Position: <span id="servoPos"></span></p>
   <p>Servo X-Axis: <span id="servoPosX"></span></p>
  <input type="range" min="0" max="180" class="slider" id="servoSliderX" onchange="servoX(this.value)"/>
  <p>Servo Y-Axis: <span id="servoPosX"></span></p>
  <input type="range" min="0" max="180" class="slider" id="servoSliderY" onchange="servoY(this.value)"/>
  <br>
  <br>
  <button onclick="startXaxis()">X-Axis Start</button>
  <button onclick="endXaxis()">X-Axis End</button>
  <br>
  <button onclick="startYaxis()">Y-Axis Start</button>
  <button onclick="endYaxis()">Y-Axis End</button>
  <br>
  <br>
  <button onclick="playBeam()">Play</button>
  <br>
  <button onclick="stopBeam()">Stop</button>

  <script>
    var servoP = document.getElementById("servoPos");
    
    function servoX(pos) {
      $.get("/moveServoX?newServoPos=" + pos + "&");
      {Connection: close};
    }
    
    function servoY(pos) {
      $.get("/moveServoY?newServoPos=" + pos + "&");
      {Connection: close};
    }

      
    function startXaxis() {
      $.get("/startXaxis");
      {Connection: close};
    }

    function startYaxis() {
      $.get("/startYaxis");
      {Connection: close};
    }

    function endXaxis() {
      $.get("/endXaxis");
      {Connection: close};
    }

    function endYaxis() {
      $.get("/endYaxis");
      {Connection: close};
    }

    function playBeam() {
      $.get("/playBeam");
      {Connection: close};
    }
        
    function stopBeam() {
      $.get("/stopBeam");
      {Connection: close};
    }

    function getStartXvalue() {
      $.get("/getStartXvalue", function(data) {
      });
      {Connection: close};
    }
    

  </script>
</body>
</html>
)=====";
