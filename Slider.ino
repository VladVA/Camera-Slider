#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

#define CLK 2
#define DT 3
#define TRIGGER_PIN 9
#define ECHO_PIN 10

int counter = 0;
//int lungime=40;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;
int dmin=9999;
int start=0;
//int viteza=0;

const int dirPin = 4;
const int stepPin = 5;
const int stepsPerRevolution = 200;

#define MAX_DISTANCE 400  



class Slider{
  
  int lungime;
  int viteza;
  int durata;
  
  public:
  Slider();
  Slider(int, int, int);
  void Miscare_simpla();
  void Miscare_auto();
  int getlungime();
  int setlungime(int a);
  int setdurata(int a);
  int setviteza(int a);
  };

Slider::Slider():lungime(0),viteza(0),durata(0)
{
 
}

Slider::Slider(int lungime, int viteza, int durata):lungime(lungime),viteza(viteza),durata(durata)
{

}

void Slider::Miscare_simpla()
{
     digitalWrite(dirPin, HIGH);
           for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(this->viteza);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(this->viteza);
  }
}

void Slider::Miscare_auto()
{
    this->viteza=250*(this->lungime/10)*abs(this->durata);
    Serial.print(viteza);
    for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(this->viteza);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(this->viteza);
  }
}

int Slider::getlungime()
{
  return this->lungime;
}

int Slider::setlungime(int a)
{
  this->lungime=a;
}

int Slider::setdurata(int a)
{
  this->durata=a;
}

int Slider::setviteza(int a)
{
  this->viteza=a;
}

Slider slide(40,0,0);

LiquidCrystal_I2C lcd(0x27,16,2); 

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
float duration, distance;

void setup() 
{
  lcd.init();
  lcd.clear();  
  lcd.setCursor(0,1);
  lcd.print("Durata:0min");
  
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);

  Serial.begin(9600);

  lastStateCLK = digitalRead(CLK);
  
    pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
    digitalWrite(dirPin, LOW);
}

void loop() 
{
  int x;
  x=analogRead(0);

  if(dmin > 5)
  {
  lcd.setCursor(0,0);
  lcd.print("Lungime sina:");
  lcd.print(slide.getlungime());
  }
  if(dmin>5)
  {
  currentStateCLK = digitalRead(CLK);

  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
    if (digitalRead(DT) != currentStateCLK) {
      counter --;
      currentDir ="CCW";
    } else {
      counter ++;
      currentDir ="CW";
    }

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(slide.getlungime());
    lcd.setCursor(0,1);
    lcd.print("Durata:");
    lcd.setCursor(7,1);
    lcd.print(abs(counter));
    slide.setdurata(counter);
    Serial.print(abs(counter));
    if(abs(counter)<10)
    {
    lcd.setCursor(8,1);
    lcd.print("min");
    }
    else if(abs(counter)<100)
    {
    lcd.setCursor(9,1);
    lcd.print("min");
    }
    else if(abs(counter)<1000)
    {
    lcd.setCursor(10,1);
    lcd.print("min");
    }
  }
  lastStateCLK = currentStateCLK;
  }
   if (x > 60 && x<200) {
   slide.setlungime(slide.getlungime()+10);
   delay(500);
 }else if(x > 200 && x<400) {
   slide.setlungime(slide.getlungime()-10);
   delay(500);
 }
 
  delay(1);

  distance = sonar.ping_cm();

 if(distance < 5)
  dmin=distance;

   if (x > 600 && x<800) {
   start++;
   digitalWrite(dirPin, LOW);
   delay(500);
   }

   if (x > 400 && x<600) {
    slide.setviteza(500);
    slide.Miscare_simpla();
   /*    digitalWrite(dirPin, HIGH);
           for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }
  */
   }

   if(start > 1 && dmin > 5)
   {
    if (counter==0)
    {slide.setviteza(1000);
    slide.Miscare_simpla();
    }
    else slide.Miscare_auto();
    /*viteza=250*(lungime/10)*abs(counter);
    Serial.print(viteza);
    for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(viteza);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(viteza);
  }*/
   }

  if (dmin < 5)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Stop");
    delay(500);
  }
}
