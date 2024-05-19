/*****************************************************************************************************************************
**********************************    Author  : Ehab Magdy Abdullah                      *************************************
**********************************    Linkedin: https://www.linkedin.com/in/ehabmagdyy/  *************************************
**********************************    Youtube : https://www.youtube.com/@EhabMagdyy      *************************************
******************************************************************************************************************************/

#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#define MP3_TX_PIN              17
#define MP3_RX_PIN              16

#define NEXT_PIN                18
#define PAUSE_PIN               19
#define PREV_PIN                21
#define NO_OF_AUDIO             3

#define POTENTIOMETER_PIN       4     /* ADC2 Pin */

unsigned char audioNumber = 1;
unsigned char isPaused = 0;
unsigned int ADCReadCounter = 0;
unsigned int analogValue = 0;
unsigned char volume = 0;

SoftwareSerial softwareSerial(MP3_RX_PIN, MP3_TX_PIN);

DFRobotDFPlayerMini player;

void setup()
{
  softwareSerial.begin(9600);

  player.begin(softwareSerial);
  
  /* Set mode of the three push buttons */
  pinMode(NEXT_PIN, INPUT);
  pinMode(PAUSE_PIN, INPUT);
  pinMode(PREV_PIN, INPUT);

  player.volume(30);
  player.play(1);
}

void loop() 
{
    ADCReadCounter++;
    /* Reading Potentiometer analog value to set the volume */
    if(ADCReadCounter > 10000)
    {
        analogValue = analogRead(POTENTIOMETER_PIN);
        volume = map(analogValue, 0, 4095, 0, 30);
        player.volume(volume);
        ADCReadCounter = 0;
    }
    /* if current played audio is finished, play it again */
    if(DFPlayerPlayFinished == player.readType())
    {
        player.play(audioNumber);
    }
    /* play the next audio */
    if(digitalRead(NEXT_PIN))
    {
        /* You can use the function player.previous(). i used this to play just 3 specific audios from SD card */
        audioNumber = ((audioNumber + 1) > NO_OF_AUDIO)? 1 : (audioNumber + 1);
        while(digitalRead(NEXT_PIN));
        player.play(audioNumber);
    }
    /* Pause/Resume next audio */
    if(digitalRead(PAUSE_PIN))
    {
        if(isPaused)
        {
            while(digitalRead(PAUSE_PIN));
            player.start();
            isPaused = 0;
        }
        else
        {
            while(digitalRead(PAUSE_PIN));
            player.pause();
            isPaused = 1;
        }
    }
    /* play the previous audio */
    if(digitalRead(PREV_PIN))
    {
        /* You can use the function player.previous(). i used this to play just 3 specific audios from SD card */
        audioNumber = ((audioNumber - 1) == 0)? NO_OF_AUDIO : (audioNumber - 1); 
        while(digitalRead(PREV_PIN));
        player.play(audioNumber);
    }
}
