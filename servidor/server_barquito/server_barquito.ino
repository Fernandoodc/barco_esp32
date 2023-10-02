
#include <Robojax_L298N_DC_motor.h>
// motor 1 settings
#define CHA 1  //canal de comunicación PWM
#define IN1 5
#define IN2 18
#define ENA 19            // this pin must be PWM enabled pin
const int CCW = 2;        // do not change
const int CW = 1;         // do not change
int motorDirection = CW;  // defaule direction
#define motor1 1          // do not change
Robojax_L298N_DC_motor motor(IN1, IN2, ENA, CHA, true);
const int changeStep = 5;  // 5 is 5% every time button is pushed
int outPutValue = 50;      // variable holding the light output vlaue (initial value) 50 means 50%
const int motorMinimumSpeed = 0;
int motor1StopState = HIGH;  //Stope state of motor HIGH=STOP

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include <ESP32Servo.h>

Servo miServo;
#define servoPin 14
//#define servoAlim 13 //pin de alimentación

int servoAngulo = 90;

#ifndef STASSID
#define STASSID "ESP32 BARQUITO"
#define STAPSK "12345678"
#endif

const int tVida = 20;  //tiempo de vida de la conexión, se tiene que renovar con la url /alive
int vida = tVida;      //vida inicial
const char *ssid = STASSID;
const char *password = STAPSK;

WebServer server(80);


void handleRoot() {
  String HTML_page = "<h1>Hola :)</h1>";
  server.send(200, "text/html", HTML_page);
}

void resetVida() {
  vida = tVida;
  String message = "Ok";
  server.send(200, "text/plain", message);
}

void handleNotFound() {

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}
void setup(void) {

  Serial.begin(115200);
  motor.begin();

  miServo.attach(servoPin);
  miServo.write(servoAngulo);

  //L298N DC Motor by Robojax.com


  WiFi.mode(WIFI_STA);
  //WiFi.begin(ssid, password);
  WiFi.softAP(ssid, password);
  Serial.println("");

  // Wait for connection
  /*while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }*/

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("robojaxESP32")) {
    Serial.print("MDNS responder started at http://");
    Serial.println("robojaxESP32");
  }
  server.enableCORS();
  server.on("/", handleRoot);
  server.on("/alive", resetVida);
  server.on("/speed", HTTP_GET, handleMotorSpeed);
  server.on("/direction", HTTP_GET, handleServoDirection);
  server.on("/stop", HTTP_GET, handleMotorBrake);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}
void loop(void) {
  server.handleClient();
  miServo.write(servoAngulo);
  digitalWrite(servoAlim, LOW);
  //control de vida de la conexión
  if (vida <= 0) {
    motor.brake(motor1);
    Serial.println("Muerto");
  } else {
    vida -= 1;
    if (motor1StopState == HIGH) {
      motor.brake(motor1);
    } else {
      motor.rotate(motor1, outPutValue, motorDirection);
    }
  }
  delay(100);  //trabaja directamente con el tiempo de vida de cada sesión, a mayor valor más tiempo de vida
}

/*
 * handleMotorSpeed()
 * Slows down or speeds up the motor
 * returns nothing
 */
void handleMotorSpeed() {
  if (server.hasArg("porcen")) {
    if (server.arg("porcen").toInt() >= 100) {
      outPutValue = 100;
    } else {
      if (server.arg("porcen").toInt() <= 0) {
        outPutValue = 0;
      } else {
        outPutValue = server.arg("porcen").toInt();
      }
    }
  }
  if (server.hasArg("reverse")) {
    //Serial.println(server.arg("reverse"));
    if (server.arg("reverse") == "true") {
      motorDirection = CCW;

    } else {
      motorDirection = CW;
    }
  }
  String message = "Ok";
  server.send(200, "text/plain", message);
}


void handleServoDirection() {
  if (server.arg("dir") == "l" || server.arg("dir") == "L") {
    servoAngulo = 90 - (server.arg("porcen").toInt() * 0.9);
  } else {
    if (server.arg("dir") == "r" || server.arg("dir") == "R") {
      servoAngulo = 90 + (server.arg("porcen").toInt() * 0.9);
    }
  }
  String message = "Ok";
  server.send(200, "text/plain", message);
  //handleRoot();
}

void handleMotorBrake() {

  if (server.arg("stop") == "false") {
    motor1StopState = LOW;
  } else {
    motor1StopState = HIGH;
  }
  String message = "Ok";
  server.send(200, "text/plain", message);
  //handleRoot();
}

void handleServo() {
  if (server.hasArg("mas")) {
    servoAngulo += server.arg("mas").toInt();
    if (servoAngulo >= 180) {
      servoAngulo = 180;
    }
  } else {
    if (server.hasArg("menos")) {
      servoAngulo -= server.arg("menos").toInt();
      if (servoAngulo <= 0) {
        servoAngulo = 0;
      }
    }
  }
  String message = "Ok";
  server.send(200, "text/plain", message);
  //handleRoot();
}