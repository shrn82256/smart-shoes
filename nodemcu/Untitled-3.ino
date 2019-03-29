#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

unsigned long myChannelNumber = 741375;
const char *myWriteAPIKey = "MGFJICVEYPMMFAEP";

const char *ssid = "AndroidAP";    // your wifi SSID name
const char *password = "mojojojo"; // wifi pasword

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
      float calories = 0.063 * steps;

      Serial.print(steps);
      Serial.print('\t');
      Serial.println(calories);

      int httpCode1 = ThingSpeak.writeField(myChannelNumber, 1, steps, myWriteAPIKey);
      int httpCode2 = ThingSpeak.writeField(myChannelNumber, 3, calories, myWriteAPIKey);

      steps = 0;
    }
  }
}