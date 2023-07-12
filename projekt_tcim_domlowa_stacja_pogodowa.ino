//#include <RTClib.h>
#include <LiquidCrystal.h>
#include <virtuabotixRTC.h>
#include "DHT.h"

#define DHT11_PIN 9
#define red 2
#define green 3
#define blue 13
#define COMMON_ANODE

const int greenLED = 8;
const int switchPIN = 10;
const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
virtuabotixRTC myRTC(14, 15, 16);
DHT dht;

int buttonCounter = 0;              // licznik naciśnięć przycisku
bool buttonState = LOW;         // zakładamy, że przycisk w stanie LOW
bool lastButtonState = LOW;      // analogicznie
 
void setup()
{
  Serial.begin(9600);

  //myRTC.setDS1302Time(29, 7, 21, 3, 31, 5, 2023);

  lcd.begin(16, 2);
  //lcd.print("dzien dobry!");

  //zielona dioda led
  pinMode(greenLED, OUTPUT);
  digitalWrite(greenLED, LOW);

  //przycisk
  pinMode(switchPIN, INPUT_PULLUP);

  //dioda rgb
  pinMode(red, OUTPUT);
  //digitalWrite(red, LOW);
  pinMode(green, OUTPUT);
  //digitalWrite(green, LOW);
  pinMode(blue, OUTPUT);
  //digitalWrite(blue, LOW);

  //czujnik temperatury i wilgotności
  dht.setup(DHT11_PIN);
}

void loop()
{
  myRTC.updateTime();

  int hum = dht.getHumidity();
  int tem = dht.getTemperature();

  //temperatura i wilgotność na port szeregowy
  /*Serial.print("humidity: ");
  Serial.print(hum);
  Serial.print("\n");
  Serial.print("temperature: ");
  Serial.print(tem);
  Serial.print("\n");
  delay(2000);
  */

  buttonState = digitalRead(switchPIN);
  //Serial.println(buttonCounter);

  //zmiana opcji wyświetlacza przez naciśnięcie przycisku
  switch (buttonCounter)
  {
    case 0:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(myRTC.hours);
      lcd.print(":");
      for(int i=0;i<10;i++)
      {
        if(myRTC.minutes==i)
        {
          lcd.print("0");
          lcd.print(myRTC.minutes);
        }
      }
      if(myRTC.minutes>9)
      {
        lcd.print(myRTC.minutes);
      }
      lcd.print(":");
      for(int i=0;i<10;i++)
      {
        if(myRTC.seconds==i)
        {
          lcd.print("0");
          lcd.print(myRTC.seconds);
        }
      }
      if(myRTC.seconds>9)
      {
        lcd.print(myRTC.seconds);
      }
      lcd.setCursor(0, 1);
      lcd.print("temp: ");
      lcd.print(tem);
      lcd.print(" .C");
      delay(1000);
      break;

    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(myRTC.dayofmonth);
      lcd.print(".");
      for(int i=0;i<10;i++)
      {
        if(myRTC.month==i)
        {
          lcd.print("0");
          lcd.print(myRTC.month);
        }
      }
      if(myRTC.month>9)
      {
        lcd.print(myRTC.month);
      }
      lcd.print(".");
      lcd.print(myRTC.year);
      lcd.setCursor(0, 1);
      lcd.print("wilgotnosc: ");
      lcd.print(hum);
      lcd.print("%");
      delay(1000);
      break;

    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      switch (myRTC.dayofweek)
      {
        case 1:
        lcd.print("poniedzialek");
        break;

        case 2:
        lcd.print("wtorek");
        break;

        case 3:
        lcd.print("sroda");
        break;

        case 4:
        lcd.print("czwartek");
        break;

        case 5:
        lcd.print("piatek");
        break;
        
        case 6:
        lcd.print("sobota");
        break;

        case 7:
        lcd.print("niedziela");
        break;
      }
      lcd.setCursor(0, 1);
      lcd.print(myRTC.hours);
      lcd.print(":");
      for(int i=0;i<10;i++)
      {
        if(myRTC.minutes==i)
        {
          lcd.print("0");
          lcd.print(myRTC.minutes);
        }
      }
      if(myRTC.minutes>9)
      {
        lcd.print(myRTC.minutes);
      }
      lcd.print(" ");
      lcd.print(myRTC.dayofmonth);
      lcd.print(".");
      for(int i=1;i<10;i++)
      {
        if(myRTC.month==i)
        {
          lcd.print("0");
          lcd.print(myRTC.month);
        }
      }
      if(myRTC.month>9)
      {
        lcd.print(myRTC.month);
      }
      lcd.print(".");
      lcd.print(myRTC.year);
      delay(1000);
      break;
  }

  if (buttonState != lastButtonState)
  {
    delay(1000);
    if (buttonState == HIGH)
    {
      //jesli status jest wysoki, to przycisk przeszedł ze stamu off to on
      buttonCounter++;
      lcd.noDisplay();
      lcd.display();
      if (buttonCounter > 2)
      {
        buttonCounter = 0;
      }

    }
    lastButtonState = buttonState;
  }

  //warunki dla diody rgb
  if(tem>=15&&tem<20)
  {
    setColor(0,153,153); // niebieski
  }
  else if(tem>=20&&tem<25)
  {
    setColor(0,204,0); // zielen
  }
  else if(tem<15)
  {
    setColor(255, 0, 255); // fiolet
  }
  else if(tem>=25)
  {
    setColor(255, 128, 0);  // pomarancz
  }

  //warunek dla diody led
  if(hum>=60)
  {
    digitalWrite(greenLED, HIGH);
  }
}


// funkcja ustawiająca kolor diody rgb
void setColor(int r, int g, int b)
{
  #ifdef COMMON_ANODE
    r = 255 - r;
    g = 255 - g;
    b = 255 - b;
  #endif
  analogWrite(red, r);
  analogWrite(green, g);
  analogWrite(blue, b);  
}
