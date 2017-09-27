#include <DHT.h>

#include <U8glib.h>

#define DHTTYPE DHT11

int D0_PIN = 2;
int D1_PIN = 3;
int CS_PIN = 4;
int DC_PIN = 5;
int RES_PIN = 6;
int DHT_PIN = 7;
int SOIL_PIN = A0;
int SOIL_POWER_PIN = 8;

int sensorValue = 0;  
int percent = 0;
int val = 0;

U8GLIB_SH1106_128X64 u8g(D0_PIN, D1_PIN, CS_PIN, DC_PIN, RES_PIN);
DHT dht(DHT_PIN, DHTTYPE);

int convertToPercent(int value) {
  int percentValue = 0;
  percentValue = map(value, 1004, 330, 0, 100);
  return percentValue;
}

int readSoil() {
    digitalWrite(SOIL_POWER_PIN, HIGH);//turn D7 "On"
    delay(10);//wait 10 milliseconds 
    val = analogRead(SOIL_PIN);//Read the SIG value form sensor 
    digitalWrite(SOIL_POWER_PIN, LOW);//turn D7 "Off"
    return val;//send current moisture value
}

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.drawStr( 0, 16, "Plant Master 2000");

  int temp = dht.readTemperature();
  
  int hygro = dht.readHumidity();

  enum {BufSize=19}; // If a is short use a smaller number, eg 5 or 6 
  char buf[BufSize];
  snprintf (buf, BufSize, "AIR TEMP: %d C", temp);

  char bufH[BufSize];
  snprintf (bufH, BufSize, "AIR HUM:  %d pct", hygro);

  char bufS[BufSize];
  snprintf (bufS, BufSize, "SOIL HUM:  %d pct", convertToPercent(readSoil()));

  u8g.drawStr(0, 32, buf);
  u8g.drawStr(0, 48, bufH);
  u8g.drawStr(0, 64, bufS);

}

void setup() {
  Serial.begin(9600);
  Serial.println("Plant Master 2000");
  u8g.setColorIndex(1);
  u8g.setFont(u8g_font_6x13r);
  dht.begin();

  pinMode(SOIL_POWER_PIN, OUTPUT);
  digitalWrite(SOIL_POWER_PIN, LOW);
  
  delay(1500);
}

void loop() {
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );

  delay(1000);
}
