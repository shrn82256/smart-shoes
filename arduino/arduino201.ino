#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);

char junk;
String inputString = "";
const int xpin = A2;
int ypin = A3;
int zpin = A4;
int powerpin = A0;
int gnd = A1;
float threshhold = 80.0;
float xval[100] = {0};
float yval[100] = {0};
float zval[100] = {0};
float xavg;
float yavg;
float zavg;
float calories;
int steps, flag = 0;

const int trigPin = 7;
const int echoPin = 8;

void setup()
{
  Serial.begin(115200);
  mySerial.begin(115200);
  pinMode(powerpin, OUTPUT);
  pinMode(gnd, OUTPUT);
  digitalWrite(powerpin, HIGH);
  digitalWrite(gnd, LOW);
  calibrate();
}
void loop()
{
  long duration, inches, cm;

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:

  pinMode(trigPin, OUTPUT);

  digitalWrite(trigPin, LOW);

  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);

  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose

  // duration is the time (in microseconds) from the sending

  // of the ping to the reception of its echo off of an object.

  pinMode(echoPin, INPUT);

  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance

  inches = microsecondsToInches(duration);

  cm = microsecondsToCentimeters(duration);

  Serial.print(inches);

  Serial.print("in, ");

  Serial.print(cm);

  Serial.print("cm");

  Serial.println();

  delay(100);

  int acc = 0;
  float totvect[100] = {0};
  float totave[100] = {0};
  float xaccl[100] = {0};
  float yaccl[100] = {0};
  float zaccl[100] = {0};
  for (int i = 0; i < 100; i++)
  {
    xaccl[i] = float(analogRead(xpin));
    delay(1);
    yaccl[i] = float(analogRead(ypin));
    delay(1);
    zaccl[i] = float(analogRead(zpin));
    delay(1);
    totvect[i] = sqrt(((xaccl[i] - xavg) * (xaccl[i] - xavg)) + ((yaccl[i] - yavg) * (yaccl[i] - yavg)) + ((zval[i] - zavg) * (zval[i] - zavg)));
    totave[i] = (totvect[i] + totvect[i - 1]) / 2;
    Serial.println(totave[i]);
    //mySerial.write(totave[i]);
    delay(200);
    if (totave[i] > threshhold && flag == 0)
    {
      steps = steps + 1;
      flag = 1;
    }
    else if (totave[i] > threshhold && flag == 1)
    {
    }
    if (totave[i] < threshhold && flag == 1)
    {
      flag = 0;
    }
    Serial.println('\n');
    Serial.print("steps=");
    Serial.println(steps);
    //mySerial.print("steps=");
    mySerial.println(steps);
    calories = 0.063 * steps;
    Serial.print("calories=");
    Serial.println(calories);

    //mySerial.print("calories=");
    //mySerial.print(calories);
    //mySerial.println('\n');
    Serial.print(inches);

    Serial.print("in, ");

    Serial.print(cm);

    Serial.print("cm");

    Serial.println();
  }
  delay(1000);
}
void calibrate()
{
  digitalWrite(13, HIGH);
  float sum = 0;
  float sum1 = 0;
  float sum2 = 0;
  for (int i = 0; i < 100; i++)
  {
    xval[i] = float(analogRead(xpin));

    sum = xval[i] + sum;
  }
  delay(100);
  xavg = sum / 100.0;

  Serial.println(xavg);

  for (int j = 0; j < 100; j++)
  {
    xval[j] = float(analogRead(xpin));

    sum1 = xval[j] + sum1;
  }
  yavg = sum1 / 100.0;

  Serial.println(yavg);
  delay(100);
  for (int i = 0; i < 100; i++)
  {
    zval[i] = float(analogRead(zpin));

    sum2 = zval[i] + sum2;
  }
  zavg = sum2 / 100.0;
  delay(100);
  Serial.println(zavg);
}

long microsecondsToInches(long microseconds)
{
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}
