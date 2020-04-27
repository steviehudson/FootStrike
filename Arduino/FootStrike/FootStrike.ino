#include <Arduino_FreeRTOS.h>
#include <SPI.h>
#include <SD.h>

void FootStrike(); 

const int PS1 = A3; //pressure sensor 1
const int PS2 = A4; //pressure sensor 2
const int button = 2; 
const int CS = 4; 

File newFile;
String fileName;

void setup() { 

  Serial.begin(9600);

  pinMode(PS1, INPUT);
  pinMode(PS2, INPUT);
  pinMode(button, INPUT);
  pinMode(CS, OUTPUT);

  Sd2Card card;
  SdVolume volume;
  SdFile root;
 
  Serial.print("Initializing SD card...");
 
  if (!card.init(SPI_HALF_SPEED, CS)) {
    Serial.println("Initialisation failed. Check card inserted, wiring correct and CS using pin 4.");
    return;
  } else {
    Serial.println("Initialising complete.");
  }
 
  Serial.print("\nCard type: ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }
 
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }
  
  uint32_t volumesize;
  Serial.print("\nVolume type is FAT");
  Serial.println(volume.fatType(), DEC);
  Serial.println();
 
  volumesize = volume.blocksPerCluster();    
  volumesize *= volume.clusterCount();       
  volumesize *= 512;                            
  Serial.print("Volume size (bytes): ");
  Serial.println(volumesize);
  Serial.print("Volume size (Kbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  
  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);
  root.ls(LS_R | LS_DATE | LS_SIZE);
  
  delay(100);

  fileName = "FSTRIKE.TXT";
  newFile = SD.open(fileName, FILE_WRITE);

  if (newFile) {
    Serial.println(fileName + " open");
  }
  else {
    Serial.println(fileName + " closed");
  }

  delay(100);
  
  xTaskCreate(
    footStrike
    ,  "FootStrike"
    ,  128
    ,  NULL
    ,  1
    ,  NULL );
}

void footStrike() // task
{
  int buttonState = digitalRead(button);
  
  if (newFile) {
    Serial.println(fileName + " open");
    for (;;) 
    {
      if (buttonState == HIGH) {
        Serial.println("Saving File");
        while (newFile.available()) {
          Serial.write(newFile.read());
        }
        Serial.println("Closing File");
        newFile.close();
        break;    
      }
        // pressure sensor ADC reading
        int ps1ADC = analogRead(PS1);
        int ps2ADC = analogRead(PS2);
  
      // if above at rest threshold, write to console  
      if (ps1ADC > 20){
        newFile.println("ADC 1: " + String(ps1ADC));
        newFile.println();
        Serial.println("ADC 1: " + String(ps1ADC));
        Serial.println();
      }
      if (ps2ADC > 20){
        newFile.println("ADC 2: " + String(ps2ADC));
        newFile.println();
        Serial.println("ADC 2: " + String(ps2ADC));
        Serial.println();
      }
      delay(100);
    }
  }
  else {
    Serial.println("Error opening FSTRIKE.TXT");
  }
delay(100);
}

void loop()
{
  //tasks are used instead
}
