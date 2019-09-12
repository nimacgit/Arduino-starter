// #include <Arduino.h>
#include <MFRC522.h>
 
#define SDA_PIN 10
#define RST_PIN 9
#define NR_KNOWN_KEYS   8
// Known keys, see: https://code.google.com/p/mfcuk/wiki/MifareClassicDefaultKeys
byte knownKeys[NR_KNOWN_KEYS][MFRC522::MF_KEY_SIZE] =  {
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, // FF FF FF FF FF FF = factory default
    {0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5}, // A0 A1 A2 A3 A4 A5
    {0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5}, // B0 B1 B2 B3 B4 B5
    {0x4d, 0x3a, 0x99, 0xc3, 0x51, 0xdd}, // 4D 3A 99 C3 51 DD
    {0x1a, 0x98, 0x2c, 0x7e, 0x45, 0x9a}, // 1A 98 2C 7E 45 9A
    {0xd3, 0xf7, 0xd3, 0xf7, 0xd3, 0xf7}, // D3 F7 D3 F7 D3 F7
    {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff}, // AA BB CC DD EE FF
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // 00 00 00 00 00 00
};
MFRC522 rfid(SDA_PIN, RST_PIN);   // Create MFRC522 instance.
MFRC522::MIFARE_Key key; 
byte nuidPICC[4];

// SDA	Digital 10
// SCK	Digital 13
// MOSI	Digital 11
// MISO	Digital 12
// IRQ	unconnected
// RST	Digital 9 

void printDec(byte *buffer, byte bufferSize);
void printHex(byte *buffer, byte bufferSize);
String printUID(MFRC522 rfid);
void writeData();
bool try_key(MFRC522::MIFARE_Key *key);
MFRC522::MIFARE_Key findKey();


void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  rfid.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
   
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);  
}

void loop() 
{
  // Look for new cards
  if (!rfid.PICC_IsNewCardPresent()) 
  {
    Serial.println ("no card");
    return;
  }
  // Select one of the cards
  if (!rfid.PICC_ReadCardSerial()) 
  {
    Serial.println("Select one");
    return;
  }
  
  String content = printUID(rfid);
  Serial.print("Message : ");
  rfid.PICC_DumpToSerial(&(rfid.uid));
  Serial.println();
  content.toUpperCase();
  if (content.substring(1) == "97 A3 E5 4E" || content.substring(1) == "E7 AD 67 33") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    
    Serial.print(F("PICC type: "));
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    Serial.println(rfid.PICC_GetTypeName(piccType));
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
      Serial.println(F("Your tag is not of type MIFARE Classic."));
      return;
    }
    key = findKey();
    writeData();

    // Halt PICC
    rfid.PICC_HaltA();
    // Stop encryption on PCD
    rfid.PCD_StopCrypto1();
  }
  else   {
    Serial.println(" Access denied");
  }
  delay(10000);
} 

void writeData(){
  byte sector         = 1;
  byte blockAddr      = 4;
  byte dataBlock[]    = {
      0x01, 0x02, 0x03, 0x04, //  1,  2,   3,  4,
      0x05, 0x06, 0x07, 0x08, //  5,  6,   7,  8,
      0x09, 0x0a, 0xff, 0x0b, //  9, 10, 255, 11,
      0x0c, 0x0d, 0x0e, 0x0f  // 12, 13, 14, 15
  };
  byte trailerBlock   = 7;
  MFRC522::StatusCode status;
  byte buffer[18];
  byte size = sizeof(buffer);

  // Authenticate using key A
  Serial.println(F("Authenticating using key A..."));
  status = (MFRC522::StatusCode) rfid.PCD_Authenticate(&key, trailerBlock, &key, &(rfid.uid));
  if (status != MFRC522::STATUS_OK) {
      Serial.print(F("PCD_Authenticate() failed: "));
      Serial.println(rfid.GetStatusCodeName(status));
      return;
  }

  // Show the whole sector as it currently is
  Serial.println(F("Current data in sector:"));
  rfid.PICC_DumpMifareClassicSectorToSerial(&(rfid.uid), &key, sector);
  Serial.println();

  // Read data from the block
  Serial.print(F("Reading data from block ")); Serial.print(blockAddr);
  Serial.println(F(" ..."));
  status = (MFRC522::StatusCode) rfid.MIFARE_Read(blockAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Read() failed: "));
      Serial.println(rfid.GetStatusCodeName(status));
  }
  Serial.print(F("Data in block ")); Serial.print(blockAddr); Serial.println(F(":"));
  printHex(buffer, 16); 
  Serial.println();
  Serial.println();

  // Authenticate using key B
  Serial.println(F("Authenticating again using key B..."));
  status = (MFRC522::StatusCode) rfid.PCD_Authenticate(&key, trailerBlock, &key, &(rfid.uid));
  if (status != MFRC522::STATUS_OK) {
      Serial.print(F("PCD_Authenticate() failed: "));
      Serial.println(rfid.GetStatusCodeName(status));
      return;
  }

  // Write data to the block
  Serial.print(F("Writing data into block ")); Serial.print(blockAddr);
  Serial.println(F(" ..."));
  printHex(dataBlock, 16);
  Serial.println();
  status = (MFRC522::StatusCode) rfid.MIFARE_Write(blockAddr, dataBlock, 16);
  if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Write() failed: "));
      Serial.println(rfid.GetStatusCodeName(status));
  }
  Serial.println();

  // Read data from the block (again, should now be what we have written)
  Serial.print(F("Reading data from block ")); Serial.print(blockAddr);
  Serial.println(F(" ..."));
  status = (MFRC522::StatusCode) rfid.MIFARE_Read(blockAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Read() failed: "));
      Serial.println(rfid.GetStatusCodeName(status));
  }
  Serial.print(F("Data in block ")); Serial.print(blockAddr); Serial.println(F(":"));
  printHex(buffer, 16);
  Serial.println();

  // Check that data in block is what we have written
  // by counting the number of bytes that are equal
  Serial.println(F("Checking result..."));
  byte count = 0;
  for (byte i = 0; i < 16; i++) {
      // Compare buffer (= what we've read) with dataBlock (= what we've written)
      if (buffer[i] == dataBlock[i])
          count++;
  }
  Serial.print(F("Number of bytes that match = ")); Serial.println(count);
  if (count == 16) {
      Serial.println(F("Success :-)"));
  } else {
      Serial.println(F("Failure, no match :-("));
      Serial.println(F("  perhaps the write didn't work properly..."));
  }
  Serial.println();

  // Dump the sector data
  Serial.println(F("Current data in sector:"));
  rfid.PICC_DumpMifareClassicSectorToSerial(&(rfid.uid), &key, sector);
  Serial.println();
}

/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
  Serial.println();
}

void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

String printUID(MFRC522 rfid){
  String content= "";
  Serial.print(F("UID tag in hex: "));
  printHex(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();
  Serial.print(F("UID tag in dec: "));
  printDec(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();
  for (byte i = 0; i < rfid.uid.size; i++) 
  {
     content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(rfid.uid.uidByte[i], HEX));
     nuidPICC[i] = rfid.uid.uidByte[i];
  }
  Serial.println();
  return content;
}

MFRC522::MIFARE_Key findKey(){
  MFRC522::MIFARE_Key key;
  for (byte k = 0; k < NR_KNOWN_KEYS; k++) {
      // Copy the known key into the MIFARE_Key structure
      for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++) {
          key.keyByte[i] = knownKeys[k][i];
      }
      // Try the key
      Serial.println("Try key: ");
      printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
      if (try_key(&key)) {
          // Found and reported on the key and block,
          // no need to try other keys for this PICC
          Serial.print("Found Key: ");
          printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
          return key;
      }
      Serial.println("Key not found");
  }
}

bool try_key(MFRC522::MIFARE_Key *key){
  bool result = false;
  byte buffer[18];
  byte block = 0;
  MFRC522::StatusCode status;

  // Serial.println(F("Authenticating using key A..."));
  status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, key, &(rfid.uid));
  if (status != MFRC522::STATUS_OK) {
      Serial.println(F("PCD_Authenticate() failed: "));
      printHex((*key).keyByte, MFRC522::MF_KEY_SIZE);
      Serial.println();
      // Serial.println(rfid.GetStatusCodeName(status));
      return false;
  }

  // Read block
  byte byteCount = sizeof(buffer);
  status = rfid.MIFARE_Read(block, buffer, &byteCount);
  if (status != MFRC522::STATUS_OK) {
      // Serial.print(F("MIFARE_Read() failed: "));
      // Serial.println(rfid.GetStatusCodeName(status));
  }
  else {
      // Successful read
      result = true;
      Serial.print(F("Success with key:"));
      printHex((*key).keyByte, MFRC522::MF_KEY_SIZE);
      Serial.println();
      // Dump block data
      Serial.print(F("Block ")); Serial.print(block); Serial.print(F(":"));
      printHex(buffer, 16);
      Serial.println();
  }
  Serial.println();

  rfid.PICC_HaltA();       // Halt PICC
  rfid.PCD_StopCrypto1();  // Stop encryption on PCD
  return result;
}