#include <Arduino.h>

void Clear();
void Display(unsigned char dat[8][8]);
void drawScreen(byte buffer2[]);
void drawhi();
void display(boolean state[8][8]);
void drawCircle();

#define ROW_1 6
#define ROW_2 3
#define ROW_3 8
#define ROW_4 2
#define ROW_5 A1
#define ROW_6 9
#define ROW_7 12
#define ROW_8 A4

#define COL_1 13
#define COL_2 4
#define COL_3 5
#define COL_4 A3
#define COL_5 7
#define COL_6 A2
#define COL_7 11
#define COL_8 10

const byte rows[] = {
    ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8
};
const byte cols[] = {
  COL_1,COL_2, COL_3, COL_4, COL_5, COL_6, COL_7, COL_8
};

byte ALL[] = {B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111};
byte EX[] = {B00000000,B00010000,B00010000,B00010000,B00010000,B00000000,B00010000,B00000000};
byte A[] = {  B00000000,B00111100,B01100110,B01100110,B01111110,B01100110,B01100110,B01100110};
byte B[] = {B01111000,B01001000,B01001000,B01110000,B01001000,B01000100,B01000100,B01111100};
byte C[] = {B00000000,B00011110,B00100000,B01000000,B01000000,B01000000,B00100000,B00011110};
byte D[] = {B00000000,B00111000,B00100100,B00100010,B00100010,B00100100,B00111000,B00000000};
byte E[] = {B00000000,B00111100,B00100000,B00111000,B00100000,B00100000,B00111100,B00000000};
byte F[] = {B00000000,B00111100,B00100000,B00111000,B00100000,B00100000,B00100000,B00000000};
byte G[] = {B00000000,B00111110,B00100000,B00100000,B00101110,B00100010,B00111110,B00000000};
byte H[] = {B00000000,B00100100,B00100100,B00111100,B00100100,B00100100,B00100100,B00000000};
byte I[] = {B00000000,B00111000,B00010000,B00010000,B00010000,B00010000,B00111000,B00000000};
byte J[] = {B00000000,B00011100,B00001000,B00001000,B00001000,B00101000,B00111000,B00000000};
byte K[] = {B00000000,B00100100,B00101000,B00110000,B00101000,B00100100,B00100100,B00000000};
byte L[] = {B00000000,B00100000,B00100000,B00100000,B00100000,B00100000,B00111100,B00000000};
byte M[] = {B00000000,B00000000,B01000100,B10101010,B10010010,B10000010,B10000010,B00000000};
byte N[] = {B00000000,B00100010,B00110010,B00101010,B00100110,B00100010,B00000000,B00000000};
byte O[] = {B00000000,B00111100,B01000010,B01000010,B01000010,B01000010,B00111100,B00000000};
byte P[] = {B00000000,B00111000,B00100100,B00100100,B00111000,B00100000,B00100000,B00000000};
byte Q[] = {B00000000,B00111100,B01000010,B01000010,B01000010,B01000110,B00111110,B00000001};
byte R[] = {B00000000,B00111000,B00100100,B00100100,B00111000,B00100100,B00100100,B00000000};
byte S[] = {B00000000,B00111100,B00100000,B00111100,B00000100,B00000100,B00111100,B00000000};
byte T[] = {B00000000,B01111100,B00010000,B00010000,B00010000,B00010000,B00010000,B00000000};
byte U[] = {B00000000,B01000010,B01000010,B01000010,B01000010,B00100100,B00011000,B00000000};
byte V[] = {B00000000,B00100010,B00100010,B00100010,B00010100,B00010100,B00001000,B00000000};
byte W[] = {B00000000,B10000010,B10010010,B01010100,B01010100,B00101000,B00000000,B00000000};
byte X[] = {B00000000,B01000010,B00100100,B00011000,B00011000,B00100100,B01000010,B00000000};
byte Y[] = {B00000000,B01000100,B00101000,B00010000,B00010000,B00010000,B00010000,B00000000};
byte Z[] = {B00000000,B00111100,B00000100,B00001000,B00010000,B00100000,B00111100,B00000000};

float timeCount = 0;

unsigned char biglove[8][8] =     //the big "heart"   
{  
  0,0,0,0,0,0,0,0,  
  0,1,1,0,0,1,1,0,  
  1,1,1,1,1,1,1,1,  
  1,1,1,1,1,1,1,1,  
  1,1,1,1,1,1,1,1,  
  0,1,1,1,1,1,1,0,  
  0,0,1,1,1,1,0,0,  
  0,0,0,1,1,0,0,0,  
};  
  
unsigned char smalllove[8][8] =      //the small "heart" 
{  
  0,0,0,0,0,0,0,0,  
  0,0,0,0,0,0,0,0,  
  0,0,1,0,0,1,0,0,  
  0,1,1,1,1,1,1,0,  
  0,1,1,1,1,1,1,0,  
  0,0,1,1,1,1,0,0,  
  0,0,0,1,1,0,0,0,  
  0,0,0,0,0,0,0,0,  
};  

void setup() {
  // iterate over the pins:
  for(int i = 0;i<8;i++)  
  // initialize the output pins:
  {  
    pinMode(R[i],OUTPUT);  
    pinMode(C[i],OUTPUT);  
  }  
  Serial.begin(9600); // setup serial
  for (byte i = 2; i <= 13; i++)
      pinMode(i, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A1, OUTPUT);
}
 
void loop() { 
  // for(int i = 0 ; i < 100 ; i++)        //Loop display 100 times 
  // {  
  //   Display(biglove);                   //Display the "Big Heart"  
  // }  
  // for(int i = 0 ; i < 50 ; i++)         //Loop display 50 times
  // {     
  //   Display(smalllove);                 //Display the "small Heart" 
  // }
  bool state[8][8] = {
      0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,
      0,0,1,1,0,0,0,1,
      0,0,1,0,1,0,0,1,
      0,1,0,0,1,0,1,0,
      0,1,0,0,0,1,1,0,
      1,0,0,0,0,1,0,0
  };
  int sound = analogRead(A0);
  // Clear();
  if(sound > 100){
    Clear();
    // drawScreen(A);
    // drawhi();
    // display(state);
    drawCircle();
    Serial.println(sound);
  }
}

struct Pair
{
  public:
    int x, y;
    Pair(int x1, int y1){
      x = x1;
      y = y1;
    }
};


void drawCircle(){
  bool state[8][8];
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      state[i][j] = false;
    }
  }
  Pair locations[12] = {Pair(2,4), Pair(2,5), Pair(3,6), Pair(4,7), Pair(5,7), Pair(6,6), Pair(7,5), Pair(7,4), Pair(6,3),
   Pair(5,2), Pair(4,2), Pair(3,3)};
  for(int i = 0; i < 12; i++){
    int x = locations[i].x;
    int y = locations[i].y;
    state[x][y] = true;
    for(int j = 0; j < 10; j++){  
      display(state);
    }
    state[x][y] = false;
  }
}


void turnon(int x, int y){
    digitalWrite(cols[y], HIGH);
    digitalWrite(rows[x], LOW);
    delay(1);
    digitalWrite(cols[y], LOW);
    digitalWrite(rows[x], HIGH);
    delay(1);
}

void drawhi(){
  for(int i = 0;i < 8; i++){
    digitalWrite(cols[i], LOW);
    digitalWrite(rows[i], HIGH);
  }
  for(int i = 0; i < 1; i++){
    turnon(1,5);
    turnon(5,5);
    turnon(1,1);
    turnon(5,1);
  }
}

void display(bool state[8][8]){
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      if(state[i][j]){
        turnon(7 - j, 7 - i);
      }
    }
  }
}

void Display(unsigned char dat[8][8]){
  for(int c = 0; c<8;c++){
    digitalWrite(C[c],LOW);//use thr column 
    //loop
    for(int r = 0;r<8;r++){
      digitalWrite(R[r],dat[r][c]);  
    }  
    delay(1);  
    Clear();  //Remove empty display light
  }  
}  

void Clear(){
  for(int i = 0;i<8;i++){
    digitalWrite(cols[i],LOW);  
    digitalWrite(rows[i],HIGH);  
  }  
}  



void drawScreen(byte buffer2[])
{ 
  // Turn on each row in series
  for (byte i = 0; i < 8; i++)        // count next row
    {
      digitalWrite(rows[i], HIGH);    //initiate whole row
      for (byte a = 0; a < 8; a++)    // count next row
      {
        // if You set (~buffer2[i] >> a) then You will have positive
        // digitalWrite(col[a], (buffer2[i] >> a) & 0x01); // initiate whole column
        
        // delayMicroseconds(100);       // uncoment deley for diferent speed of display
        //delayMicroseconds(1000);
        //delay(10);
        //delay(100);
        
        digitalWrite(cols[a], 1);      // reset whole column
      }
      digitalWrite(rows[i], LOW);     // reset whole row
      // otherwise last row will intersect with next row
  }
}