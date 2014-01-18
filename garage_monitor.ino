
#include <LiquidCrystal.h>

int relay = 0;          // ATMega 328P pin 2
int motion_sensor = 1;  // ATMega 328P pin 3
int door_sensor = 7;    // ATMega 328P pin 13
int red_led = 8;        // ATMega 328P pin 14
int green_led = 9;       // ATMega 328P pin 15
int blue_led = 10;     // ATMega 328P pin 16
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // ATMega 328P pins {18, 17, 11, 6, 5, 4}

int counter = 0;  // each increment of counter will represent 1 second.
int pulse_time = 100;
boolean message_sent = false;  // will help keep track if door_close was called.

void setup()
{
  pinMode (motion_sensor, INPUT);
  pinMode (door_sensor, INPUT);
  pinMode (red_led, OUTPUT);
  pinMode (green_led, OUTPUT);
  pinMode (blue_led, OUTPUT);
  digitalWrite (red_led, HIGH);
  digitalWrite (green_led, HIGH);
  digitalWrite (blue_led, HIGH);

  lcd.begin (16, 2);
  lcd.print ("Garage Monior");
  lcd.setCursor (0, 1);
  lcd.print ("Version 1.0");
  delay (2000);
}

void loop ()
{
  lcd.clear ();
  lcd.setCursor (0, 0);
  lcd.print (counter);
  // while motion detected and door is open, set RGB to yellow and reset counter.
  while (motion () && !door_closed ())
  {
    lcd.clear ();
    lcd.print ("Motion!         ");
    pulse (red_led, 10);
    delay (1000);
    lcd.clear ();
    counter = 0;
  }
  
  // If door is closed, set RGB to green, reset message_sent and wait till door opens.
  while (door_closed ())
  {
    lcd.clear ();
    lcd.setCursor (0, 1);
    lcd.print ("Door Closed     ");
    lcd.setCursor (0, 0);
    setColor (255, 127, 255);
    message_sent = false;
    counter = 0;
    delay (500);
  }
  // door open, set RGB to red.
  if (!door_closed ())
 {
    lcd.setCursor (0, 1);
    lcd.print ("Door Open!  ");
    lcd.setCursor (0, 0);
    setColor (127, 255, 255);
 }
  // If door is open, counter has reached 30 (~30 seconds) and message has not been sent, then close door.  
  if (!door_closed () && counter == 30 && !message_sent)
  {
    pulse (red_led, 5);
    close_door (); 
  }
  
  while (message_sent && !door_closed)
  {
    lcd.clear ();
    lcd.print ("**Closing Door**");
    delay (500);
    lcd.clear ();
    lcd.setCursor (0, 1);
    lcd.print ("**Closing Door**");
    delay (500);
  }
  delay(1000);
  
  if (counter != 30) // Wait ~30 seconds before reseting back to zero.
    ++counter;
  else
    counter = 0;
  
}

// return true if door closed.
boolean door_closed ()
{
 return (digitalRead (door_sensor) == 0); 
}

// return true if motion detected.
boolean motion ()
{
  return (digitalRead (motion_sensor) == 1); 
}

// close relay for 2 seconds to simulate pushing garage door button.
void close_door ()
{
  
  digitalWrite (relay, HIGH);
  delay (2000);
  digitalWrite (relay, LOW);
  message_sent = true; 
}

void setColor(int red, int green, int blue)
{
  analogWrite(red_led, red);
  analogWrite(green_led, green);
  analogWrite(blue_led, blue);  
}

void pulse (int led, int times)
{
   do {
    digitalWrite(led, LOW);
    delay(pulse_time);
    digitalWrite(led, HIGH);
    delay(pulse_time);
  } 
  while (times--);
}
