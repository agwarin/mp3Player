/*************************************************** 
  This code was written by Niki Selken for Adafruit VS1053 Codec Breakout
  It creates a playlist feature for the music maker shield.
  It also works with the sharp distance sensor to trigger the track. 
  
  Designed specifically to work with the Adafruit VS1053 Codec Breakout 
  ----> https://www.adafruit.com/products/1381

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
bool playsong = false;
int thisPin;
int val = 1;

//import the library in the sketch
#include <SharpIR.h>

//Create a new instance of the library
//Call the sensor "sensor"
//The model of the sensor is "GP2YA41SK0F"
//The sensor output pin is attached to the pin A0
SharpIR sensor( SharpIR::GP2Y0A41SK0F, A0    );

// define the pins used
//#define CLK 13       // SPI Clock, shared with SD card
//#define MISO 12      // Input data, from VS1053/SD card
//#define MOSI 11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins. 
// See http://arduino.cc/en/Reference/SPI "Connections"

// These are the pins used for the breakout example
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
  //Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
  // create shield-example object!
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);
  
void setup() {
  Serial.begin(9600);
  Serial.println("Adafruit VS1053 Simple Test");
playsong =  false;
  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
   if (!SD.begin(CARDCS)) {
    Serial.println(("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  // list files
  printDirectory(SD.open("/"), 0);
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(20,20);

  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
 // musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  
//int songNumber = 1;
//IsAnySongPlaying= musicPlayer.playingMusic();
thisPin = 0;

}

void loop() {
//Distance sensor reading
int distance = sensor.getDistance(); //Calculate the distance in centimeters and store the value in a variable
Serial.println( distance ); //Print the value to the serial monitor
delay(10);

if (distance < 4){
  playsong = true;
 
  if (playsong ==  true){

//track number
    val;
    
//play each song and increase track number (val) by one after playing.
//Just add new numbered cases for each song.
switch (val) {

      case 1:
      Serial.println(" Ready Track");
      Serial.println(val);
          // Play one file, don't return until complete
      musicPlayer.playFullFile("1.mp3");
      delay(500);
      val = val + 1;
      Serial.println(" Ready Track");
      Serial.println (val); 
      break;

      case 2:
      musicPlayer.playFullFile("2.mp3");
      val = val + 1;
      Serial.println(" Ready Track");
      Serial.println (val);
      break;

      case 3:
      musicPlayer.playFullFile("3.mp3");
      val = val + 1;
      Serial.println(" Ready Track");
      Serial.println (val);
      break;

      case 4:
      musicPlayer.playFullFile("4.mp3");
      val = 1;
      Serial.println(" Ready Track");
      Serial.println (val);
      break;
      
      default:
      Serial.println("I shouldn't be here.");
      break;
    }
  }


//if (playingMusic==false) { SongNumber++; musicPlayer.playTrack(SongNumber); }
  // Play another file in the background, REQUIRES interrupts!
 // Serial.println(F("Playing track 002"));
 // musicPlayer.startPlayingFile("/track002.mp3");

    }
  // File is playing in the background
//  if (musicPlayer.stopped()) {
//    Serial.println("Done playing music");
//    while (1) {
//      delay(10);  // we're done! do nothing...
//    }
//  }
  if (Serial.available()) {
    char c = Serial.read();
    
    // if we get an 's' on the serial console, stop!
    if (c == 's') {
      musicPlayer.stopPlaying();
    }
    
    // if we get an 'p' on the serial console, pause/unpause!
    if (c == 'p') {
      if (! musicPlayer.paused()) {
        Serial.println("Paused");
        musicPlayer.pausePlaying(true);
      } else { 
        Serial.println("Resumed");
        musicPlayer.pausePlaying(false);
      }
    }
  }

  delay(100);
}


/// File listing helper
void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}
