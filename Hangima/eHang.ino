// https://www.sampleism.com/hephaestussounds/product/hangdrum-free

// *********** PROG ************//
#define TRUE 1
#define FALSE 0

#define NOTE_ON 1
#define NOTE_OFF 0


// *********** CONFIG ************//
const byte nbSensors = 9;    // Number of pads
const byte sensorsPins[] = {0, 1, 3, 4, 15, 18, 19, 22, 23};    // Capacitive pins used for sensors (pads)
bool notesStatus[] = {NOTE_OFF,NOTE_OFF,NOTE_OFF,NOTE_OFF,NOTE_OFF,NOTE_OFF,NOTE_OFF,NOTE_OFF,NOTE_OFF};    // All notes set to Off at the begining
//const byte noteList[] = {60, 62, 64, 67, 69, 72, 74, 76, 79};    // Notes for each sensor. Values : 0-127. Each number codes for a semitone, middle C of the piano is 60.
const byte noteList[] = {43, 45, 48, 50, 52, 55, 57, 60, 62};    // Notes for each sensor. Values : 0-127. Each number codes for a semitone, middle C of the piano is 60.

const byte midiChannel = 1;   // Choose a midi channel between 1 and 16
const byte midiVolume = 127;   // Value : 0-127. Max volume (127) allows you to use the full range of your speakers.

const unsigned int touchReadLimit = 1600;   // Decrease this value to increase the sensitivity of sensors
bool isSensorTouched = FALSE;

void setup() {
  
}

void loop() {
  // For each sensor
  for (int i = 0; i < nbSensors; i++) {
    if( touchRead(sensorsPins[i]) > touchReadLimit ) {
      isSensorTouched = TRUE;
    }
    else {
      isSensorTouched = FALSE;
    }
    
    if( isSensorTouched == TRUE && notesStatus[i] == NOTE_OFF ) {  // If this value is > to the limit that means that the sensor is touched
      usbMIDI.sendNoteOn(noteList[i], midiVolume, midiChannel);   // Play the note that correspond to this sensor
      notesStatus[i] = NOTE_ON;   // And save the new status
    }
    else if(isSensorTouched == FALSE && notesStatus[i] == NOTE_ON) {  
      usbMIDI.sendNoteOff(noteList[i], midiVolume, midiChannel);
      notesStatus[i] = NOTE_OFF;   // And save the new status      
    }     
  }
  delay(10);
}
