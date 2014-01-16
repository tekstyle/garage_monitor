
#include "pitches.h"
int motion_sensor = 2;  // IC pin 4
int door_sensor = 4;    // IC pin 6
int red_led = 3;        // IC pin 5
int green_led = 5;      // IC pin 11
int blue_led = 6;     // IC pin 12
int relay = 9;         // IC pin 15
int power = 13;         // IC pin 19
int interrupt = 0;
int counter = 0;
int WAIT_TIME = 20; // 20 secs
int pulse_time = 100;
boolean message_sent = false;
volatile int wait_time = 300000; // 300000 = 5 min timer.

int melody[] = {NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};
int noteDurations[] = {4, 8, 8, 4,4,4,4,4 };
void setup()
{
  pinMode (motion_sensor, INPUT);
  pinMode (door_sensor, INPUT);
  pinMode (power, OUTPUT);
  pinMode (red_led, OUTPUT);
  pinMode (green_led, OUTPUT);
  pinMode (blue_led, OUTPUT);
  digitalWrite (red_led, HIGH);
  digitalWrite (green_led, HIGH);
  digitalWrite (blue_led, HIGH);
  //attachInterrupt (interrupt, reset_timer, RISING); // motion sensor pin goes high when motion detected.
}

void loop ()
{
  if (motion () && !door_closed ())
  {
    setColor (127, 127, 255);
    counter = 0;
  }
  
  if (door_closed ())
  {
    setColor (255, 127, 255);
    message_sent = false; 
  }
  else 
    setColor (127, 255, 255);
    
  if (!door_closed () && counter == 30 && !message_sent)
  {
    pulse (red_led, 5);
    close_door (); 
  }
  
  if (counter != 30)
    ++counter;
  else
    counter = 0;
  delay(1000);
}

boolean door_closed ()
{
 return (digitalRead (door_sensor) == 0); 
}

boolean motion ()
{
  return (digitalRead (motion_sensor) == 1); 
}

void close_door ()
{
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(8, melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }
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
