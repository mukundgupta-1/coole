#include <Servo.h>
#include <ESP8266WiFi.h>

/* define port */
WiFiClient client;
WiFiServer server(80);

/* WIFI settings */
const char* ssid = "coole";
const char* password = "capstoneg47";

/* data received from application */
String  data = "";


Servo servo1;
Servo servo2;
Servo servo3;

void setup() {
  Serial.begin(115200);
  delay(10);
  // Attach the servos to pins 9, 10, and 11
  servo1.attach(2);
  servo2.attach(4);
  servo3.attach(16);

  // Set the initial position of the servos to 90 degrees
  servo1.write(165);
  servo2.write(0);
  servo3.write(130);
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

void loop() {
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
    void MotorForward(void)
    {
      servo1.write(0);
      servo2.write(0);
      servo3.write(30);
    }
    // Move the arm to the second point
    void MotorBackward(void){
      servo1.write(45);
      servo2.write(45);
      servo3.write(30);
    }
    // Move the arm to the third point
    void TurnLeft(void) {
      servo1.write(45);
      servo2.write(45);
      servo3.write(45);
    }
  
    void TurnRight(void){
      servo1.write(45);
      servo2.write(90);
      servo3.write(45);
    }
    void MotorStop(void){
      servo1.write(90);
      servo2.write(90);
      servo3.write(0);}

      String checkClient (void)
{
  while (!client.available()) delay(1);
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length() - 9, 9);
  return request;
}
