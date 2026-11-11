#include <WiFi.h>
#include <WebServer.h>

// ===============================
// Wi-Fi Access Point
// ===============================
const char* ssid = "ESP32_RC_CAR";
const char* password = "12345678";

WebServer server(80);

// ===============================
// L298N Motor Pins
// ===============================

// Channel A - Left motor
const int IN1 = 12;
const int IN2 = 14;

// Channel B - Right motor
const int IN3 = 26;
const int IN4 = 25;

// ===============================
// Motor Functions
// ===============================

void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void forward() {
  // Left motor forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // Right motor forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward() {
  // Left motor backward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  // Right motor backward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void left() {
  // Left motor backward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  // Right motor forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void right() {
  // Left motor forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // Right motor backward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// ===============================
// Webpage
// ===============================

void handleRoot() {

  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>

<meta name="viewport" content="width=device-width, initial-scale=1">

<title>ESP32 RC Car</title>

<style>

body {
  text-align: center;
  font-family: Arial;
  background: #222;
  color: white;
}

h1 {
  margin-top: 30px;
}

button {
  width: 120px;
  height: 70px;
  margin: 8px;
  font-size: 22px;
  font-weight: bold;
  border-radius: 15px;
  border: none;
}

.forward {
  background: green;
  color: white;
}

.backward {
  background: orange;
  color: white;
}

.left {
  background: blue;
  color: white;
}

.right {
  background: blue;
  color: white;
}

.stop {
  background: red;
  color: white;
}

</style>

</head>

<body>

<h1>🚗 ESP32 RC Car</h1>

<div>

<button class="forward"
onclick="sendCommand('forward')">
⬆ Forward
</button>

</div>

<div>

<button class="left"
onclick="sendCommand('left')">
⬅ Left
</button>

<button class="stop"
onclick="sendCommand('stop')">
⏹ Stop
</button>

<button class="right"
onclick="sendCommand('right')">
➡ Right
</button>

</div>

<div>

<button class="backward"
onclick="sendCommand('backward')">
⬇ Backward
</button>

</div>

<script>

function sendCommand(command) {
  fetch("/" + command);
}

</script>

</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}

// ===============================
// Setup
// ===============================

void setup() {

  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopCar();

  // Create ESP32 Wi-Fi
  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.println("ESP32 RC Car Started!");
  Serial.print("Wi-Fi Name: ");
  Serial.println(ssid);

  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Webpage
  server.on("/", handleRoot);

  server.on("/forward", []() {
    forward();
    server.send(200, "text/plain", "FORWARD");
  });

  server.on("/backward", []() {
    backward();
    server.send(200, "text/plain", "BACKWARD");
  });

  server.on("/left", []() {
    left();
    server.send(200, "text/plain", "LEFT");
  });

  server.on("/right", []() {
    right();
    server.send(200, "text/plain", "RIGHT");
  });

  server.on("/stop", []() {
    stopCar();
    server.send(200, "text/plain", "STOP");
  });

  server.begin();

  Serial.println("Web server started!");
}

// ===============================
// Loop
// ===============================

void loop() {
  server.handleClient();
}