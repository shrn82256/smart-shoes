#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

unsigned long myChannelNumber = 0;
const char *myWriteAPIKey = "lalala";

const char *ssid = "AndroidAP";    // your wifi SSID name
const char *password = "mojojojo"; // wifi pasword
float val;
int pin = A0;

WiFiClient client;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    ;
  }

  WiFi.disconnect();
  delay(10);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("NodeMcu connected to wifi ");
  Serial.println(ssid);
  Serial.println();

  ThingSpeak.begin(client);
}

int steps = 0;
void loop()
{
  if (Serial.available())
  {
    char ch = Serial.read();
    if (ch >= '0' && ch <= '9')
    {
      steps = steps * 10 + ch - '0';
    }
    else if (ch == '\n')
    {
      val = analogRead(pin);
      float calories = 0.063 * steps;

      Serial.print(steps);
      Serial.print('\t');
      Serial.print(val);
      Serial.print('\t');
      Serial.println(calories);

      ThingSpeak.setField(1, steps);
      ThingSpeak.setField(3, (float)calories);
      ThingSpeak.setField(2, (float)val);
      ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

      steps = 0;
    }
  }
}