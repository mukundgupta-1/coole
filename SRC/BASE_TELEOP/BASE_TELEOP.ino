/* include library */
#include <ESP8266WiFi.h>

/* define port */
WiFiClient client;
WiFiServer server(80);

/* WIFI settings */
const char* ssid = "coole";
const char* password = "capstoneg47";

/* data received from application */
String  data = "";

/* define MD10C motor control pins */
int motorA1 = 2; //GPIO2(D4) -> PWM A
int motorA2 = 12; //GPIO12(D3) -> Direction A
int motorB1 = 4; //GPIO4(D2) -> PWM B
int motorB2 = 16; //GPIO6(D1) -> Direction B

void setup()
{
  Serial.begin(115200);
  delay(10);
  /* initialize motor control pins as output */
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  // Attempt to connect to WiFi network:
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    // Connect to WPA/WPA2 network. Change this line if using open or WEP  network:
    // Wait 3 seconds for connection:
    delay(3000);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   //You can get IP address assigned to ESP

  /* start server communication */
  server.begin();
}

void loop()
{
  /* If the server available, run the "checkClient" function */
  client = server.available();
  if (!client) return;
  data = checkClient ();

  /******** Run function according to incoming data from application *********/

  /* If the incoming data is "forward", run the "MotorForward" function */
  if (data == "forward") MotorForward();
  /* If the incoming data is "backward", run the "MotorBackward" function */
  else if (data == "backward") MotorBackward();
  /* If the incoming data is "left", run the "TurnLeft" function */
  else if (data == "left") TurnLeft();
  /* If the incoming data is "right", run the "TurnRight" function */
  else if (data == "right") TurnRight();
  /* If the incoming data is "stop", run the "MotorStop" function */
  else if (data == "stop") MotorStop();
}

/*************** FORWARD *******************/
void MotorForward(void)
{
  analogWrite(motorA1, 255);
  digitalWrite(motorA2, LOW);
  analogWrite(motorB1, 255);
  digitalWrite(motorB2, HIGH);
}

/*************** BACKWARD *******************/
void MotorBackward(void)
{
  analogWrite(motorA1, 255);
  digitalWrite(motorA2, HIGH);
  analogWrite(motorB1, 255);
  digitalWrite(motorB2, LOW);
}

/*************** TURN LEFT *******************/
void TurnLeft(void)
{
  analogWrite(motorA1, 150);
  digitalWrite(motorA2, HIGH);
  analogWrite(motorB1, 150);
  digitalWrite(motorB2, HIGH);
}

/*************** TURN RIGHT *******************/
void TurnRight(void)
{
  analogWrite(motorA1, 150);
  digitalWrite(motorA2, LOW);
  analogWrite(motorB1, 150);
  digitalWrite(motorB2, LOW);
}

void MotorStop(void)
{
  analogWrite(motorA1, 0);
  digitalWrite(motorA2, LOW);
  analogWrite(motorB1, 0);
  digitalWrite(motorB2, LOW);
}

String checkClient (void)
{
  while (!client.available()) delay(1);
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length() - 9, 9);
  return request;
}
