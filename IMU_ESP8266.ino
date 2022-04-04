#include "ESP8266WiFi.h"
#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

long timer = 0;
int counter = 0;
int counter2 = 0;
int counter3 = 0;
float x1 = 0;
float x2 = 0;
int times[] = {0, 0, 0, 0};

// WiFi parameters to be configured
const char* ssid = "SSID";
const char* password = "PASS";

int status = WL_IDLE_STATUS;

char server[] = "Server";

String postData;
String postVariable = "value=";

WiFiClient client;

int dataSend = 0;
bool trigger = false;

void setup() {
  Serial.begin(9600);
  // Connect to WiFi
  WiFi.begin(ssid, password);

  // while wifi not connected yet, print '.'
  // then after it connected, get out of the loop
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  //print a new line, then print WiFi connected and the IP address
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address
  Serial.println(WiFi.localIP());

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  postData = postVariable + dataSend;
  if (client.connect(server, 80)) {
      Serial.println("Connected");
      client.println("POST /post2.php HTTP/1.1");
      client.println("Host: your_url");
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(postData.length());
      client.println();
      client.print(postData);
    }
    
    if (client.connected()) {
      client.stop();
    }
    Serial.println(postData);
    trigger = false;
}

void loop() {
  mpu6050.update();
  x1 = mpu6050.getAccZ();
  delay(10);
  mpu6050.update();
  x2 = mpu6050.getAccZ();
  
  if (abs(x1 - x2) > 0.15){
    Serial.println(counter2);
    counter2++;
    counter3++;
    times[counter2 % 2] = millis();
    Serial.println(millis());
  }

  if (abs(times[counter2 % 2] - times[(counter2 - 1) % 2]) <= 200 && abs(times[counter2 % 2] - times[(counter2 - 1) % 2]) >= 100){
    Serial.println("Pass");
    Serial.println(times[0]);
    Serial.println(times[1]);
    delay(2000);
    times[counter2 % 2] = millis();
    dataSend += 1;
    postData = postVariable + dataSend;
    trigger = true;
  }
  counter++;
  delay(25);

  if (trigger == true){
    if (client.connect(server, 80)) {
      Serial.println("Connected");
      client.println("POST /post2.php HTTP/1.1");
      client.println("Host: your_url");
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(postData.length());
      client.println();
      client.print(postData);
    }
    
    if (client.connected()) {
      client.stop();
    }
    Serial.println(postData);
    trigger = false;
  }
}
