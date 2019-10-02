#include <Arduino.h>

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

const byte rows[] = {ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8 };
const byte cols[] = {COL_1,COL_2, COL_3, COL_4, COL_5, COL_6, COL_7, COL_8 };
unsigned long time = 0;

void clear();
void Display(unsigned char dat[8][8]);
void drawScreen(byte buffer2[]);
void drawhi();
void clearState(bool state[8][8]);
void display(boolean state[8][8]);
void drawAlert();
void drawCircle(bool state[8][8]);
void drawCircleSquare();
bool shouldAlert(int sound);

void setup() {
  for(int i = 0;i<8;i++)  
  {  
    pinMode(rows[i],OUTPUT);  
    pinMode(cols[i],OUTPUT);  
  }  
  Serial.begin(9600);
  clear();
}

void loop(){
  int sound = analogRead(A0);
  if(shouldAlert(sound)){
    for(int k = 0; k < 3; k++){
      drawAlert();
    }
    Serial.println(sound);
  }
}

bool shouldAlert(int sound){
  if(sound > 512){
    if(millis() > time + 512 ){
      time = millis();
      return true;
    }
  }
  return false;
}

struct Pair
{
  public:
    int x, y;
    Pair(){

    }
    Pair(int x1, int y1){
      x = x1;
      y = y1;
    }
};

void drawAlert(){
  // drawCircle();
  drawCircleSquare();
}

void drawCircleSquare(){
  bool state[8][8];
  clearState(state);
  Pair squareLoc[36];
  for(int i = 0; i < 8; i++){
    squareLoc[4*i] = Pair(0,i);
    squareLoc[4*i+1] = Pair(i,0);
    squareLoc[4*i+2] = Pair(i,7);
    squareLoc[4*i+3] = Pair(7,i);
  }
  squareLoc[32] = Pair(3,3);
  squareLoc[33] = Pair(3,4);
  squareLoc[34] = Pair(4,3);
  squareLoc[35] = Pair(4,4);
  Pair locations[12] = {Pair(1,3), Pair(1,4), Pair(2,5), Pair(3,6), Pair(4,6), Pair(5,5), Pair(6,4),
    Pair(6,3), Pair(5,2), Pair(4,1), Pair(3,1), Pair(2,2)};
  for(int j = 0; j < 36; j++){
    int x = squareLoc[j].x;
    int y = squareLoc[j].y;
    state[x][y] = true;
  }
  drawCircle(state);
  for(int j = 0; j < 36; j++){
    int x = squareLoc[j].x;
    int y = squareLoc[j].y;
    state[x][y] = false;
  }
}


void drawCircle(bool state[8][8]){
  Pair locations[12] = {Pair(1,3), Pair(1,4), Pair(2,5), Pair(3,6), Pair(4,6), Pair(5,5), Pair(6,4),
    Pair(6,3), Pair(5,2), Pair(4,1), Pair(3,1), Pair(2,2)};
  for(int i = 0; i < 12; i++){
    for(int j = 0; j < 4; j++){
      int x = locations[(i+j)%12].x;
      int y = locations[(i+j)%12].y;
      state[x][y] = true;
    }
    display(state);
    for(int j = 0; j < 4; j++){
      int x = locations[(i+j)%12].x;
      int y = locations[(i+j)%12].y;
      state[x][y] = false;
    }
  }
}

void clearState(bool state[8][8]){
    for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      state[i][j] = false;
    }
  }
}

void turnon(int x, int y){
    digitalWrite(cols[y], HIGH);
    digitalWrite(rows[x], LOW);
}

void turnoff(int x, int y){
    digitalWrite(cols[y], LOW);
    digitalWrite(rows[x], HIGH);
}

void display(bool state[8][8]){
  for(int k = 0; k < 5; k++){
    for(int i = 0; i < 8; i++){
      for(int j = 0; j < 8; j++){
        for(int l = 0; l < 40; l++){
          if(state[i][j]){
            turnon(7- j, 7 - i);
          }
        }
        turnoff(7- j, 7 - i);
      }
    }
  }
  for(int i = 0; i < 8; i++){
      for(int j = 0; j < 8; j++){
        turnoff(7- j, 7 - i);
      }
  }
}

void clear(){
  for(int i = 0;i<8;i++){
    digitalWrite(cols[i],LOW);  
    digitalWrite(rows[i],HIGH);  
  }  
}  
