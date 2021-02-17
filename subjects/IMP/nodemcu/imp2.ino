#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "abcd.h"

// #define rotateLeft(value, shift) ((value << shift)|(value >> (sizeof(value)*8 - shift)))
// #define rotateRight(value, shift) ((value >> shift)|(value << (sizeof(value)*8 - shift)))

/*
 * D0 - D7 = col
 * D8, D9(RX), D10(TX) = row
 */

#define MAX_NUMBER_OF_SCREENS 70 // Maximalni pocet snimku
uint8_t selectScreen = 0;       // Index vybraneho snimku
uint8_t numberOfScreens = 6;    // Pocet dostupnych snimku
uint16_t screenDuration = 0;    // Aktualni pocet opakovani/trvani
// Seznam trvani snimku
uint16_t screenDurations[MAX_NUMBER_OF_SCREENS] = {
  50, 50, 50, 50, 50, 50
};
// Seznam snimku
uint8_t screens[MAX_NUMBER_OF_SCREENS][8] = {
  {
    0b00010000, 
    0b00101000, 
    0b00010000, 
    0b11111110, 
    0b00010000, 
    0b00010000, 
    0b00101000, 
    0b00101000
  },
  {
    0b00010000, 
    0b10101010, 
    0b01010100, 
    0b00111000, 
    0b00010000, 
    0b00010000, 
    0b00101000, 
    0b00101000
  },
  {
    0b00010000, 
    0b00101000, 
    0b00010000, 
    0b11111110, 
    0b00010000, 
    0b00010000, 
    0b00101000, 
    0b00101000
  },
  {
    0b00010000, 
    0b00101000, 
    0b00010000, 
    0b00111000, 
    0b01010100, 
    0b10010010, 
    0b00101000, 
    0b00101000
  },
  {
    0b00010000, 
    0b00101000, 
    0b00010000, 
    0b00111000, 
    0b00111000, 
    0b00111000, 
    0b01101100, 
    0b00101000
  },
  {
    0b00010000, 
    0b00101000, 
    0b00010000, 
    0b00111000, 
    0b01010100, 
    0b10010010, 
    0b00101000, 
    0b00101000
  }
};

unsigned int refreshRate = 250; // doba mikrosekund pro sviceni led

// Orientace radku displeje
uint8_t rowsOrientaion = 0;
uint8_t rowsValue[2][2] = { {LOW, HIGH},{HIGH, LOW} };

// Orientace sloupce displeje
uint8_t colsOrientaion = 0;
uint8_t colsPin[2][8] = {
  {D7,D6,D5,D4,D3,D2,D1,D0},
  {D0,D1,D2,D3,D4,D5,D6,D7}
};

// Rotovani hodnoty v rozsahu <0,max> pro kroky z rozsahu <-max,max>.
#define NextNumber(value,step,max) ((value+max+step)%max)

uint8_t row = 0; // Radek displeje.
uint8_t col = 0; // Sloupec displeje. 
uint8_t colData = 128; // Pomocna promena pro vytahnuti dat o hodnote sloupce snimku.
uint8_t srow = 0; // Hodnota posunuti vykreslovaneho radku snimku oproti displeji.
uint8_t scol = 0; // Hodnota posunuti vykreslovaneho sloupce snimku oproti displeji.
uint8_t srowStep = 0; // Krok posunu pro srow.
uint8_t scolStep = 1; // Krok posunu pro scol.

void RestartAnimation(){
  row=0;
  col=0;
  selectScreen = 0;
  screenDuration = 0;
}

/**
 * Funkce nacte snimky dle textu.
 * @param text Text ktery se prevede na snimky.
 */
void LoadTextIntoScreens(String text, uint8_t duration) {
  uint8_t numOfScr =  text.length();
  char charInString = ' ';
  // Upraveni dle maximalni velikosti.
  numOfScr = (numOfScr > MAX_NUMBER_OF_SCREENS)?MAX_NUMBER_OF_SCREENS:numOfScr;
  for(int i=0; i<numOfScr; i++) {
    charInString = text.charAt(i);
    // A-Z
    if(charInString >= 'A' && charInString <= 'Z') {
      memcpy(screens[i], CHARS_ABCD[ABC2index(charInString)], sizeof(uint8_t)*8);
    } 
    // a-z
    else if(charInString >= 'a' && charInString <= 'z') {
      memcpy(screens[i], CHARS_ABCD[abc2index(charInString)], sizeof(uint8_t)*8);
    } 
    // 0-9
    else if(charInString >= '0' && charInString <= '9') {
      memcpy(screens[i], CHARS_NUMBER[number2index(charInString)], sizeof(uint8_t)*8);
    } 
    // ' '
    else if(charInString == ' ') {
      memcpy(screens[i], CHAR_SPACE, sizeof(uint8_t)*8);
    } 
    // .
    else if(charInString == '.') {
      memcpy(screens[i], CHAR_DOT, sizeof(uint8_t)*8);
    } 
    // ,
    else if(charInString == ',') {
      memcpy(screens[i], CHAR_COMMA, sizeof(uint8_t)*8);
    } 
    // !
    else if(charInString == '!') {
      memcpy(screens[i], CHAR_EXCLAMATION, sizeof(uint8_t)*8);
    } 
    // ?
    else if(charInString == '?') {
      memcpy(screens[i], CHAR_QUESTION, sizeof(uint8_t)*8);
    } 
    // ;
    else if(charInString == ';') {
      memcpy(screens[i], CHAR_SEMICOLON, sizeof(uint8_t)*8);
    } 
    // +
    else if(charInString == '+') {
      memcpy(screens[i], CHAR_PLUS, sizeof(uint8_t)*8);
    } 
    // -
    else if(charInString == '-') {
      memcpy(screens[i], CHAR_MINUS, sizeof(uint8_t)*8);
    } 
    // *
    else if(charInString == '*') {
      memcpy(screens[i], CHAR_MUL, sizeof(uint8_t)*8);
    } 
    // /
    else if(charInString == '/') {
      memcpy(screens[i], CHAR_DIV, sizeof(uint8_t)*8);
    } 
    // =
    else if(charInString == '=') {
      memcpy(screens[i], CHAR_EQUAL, sizeof(uint8_t)*8);
    } 
    // Unknown
    else {
      memcpy(screens[i], CHAR_UNKNOWN, sizeof(uint8_t)*8);
    }

    screenDurations[i]=duration;
  }
  numberOfScreens = numOfScr;
  RestartAnimation();
}
/**
 * Funkce nacte ulozene snimky animace do aktivnich snimku, ktere jsou zobrazovany.
 * @param animationScreens Pole obsahujici snimky.
 * @param animationLen     Velikost pole obsahujici snimky.
 * @param duration         Trvani mezi snimky.
 */
void LoadAnimation(uint8_t animationScreens[][8], uint8_t animationLen, uint8_t duration) {
  uint8_t numOfScr =  animationLen;
  // Upraveni dle maximalni velikosti.
  numOfScr = (numOfScr > MAX_NUMBER_OF_SCREENS)?MAX_NUMBER_OF_SCREENS:numOfScr;
  for(int i=0; i<numOfScr; i++) {
    memcpy(screens[i], animationScreens[i], sizeof(uint8_t)*8);
    screenDurations[i]=duration;
  }
  numberOfScreens = numOfScr;
  RestartAnimation();
}
/**
 * Funkce zmeni typ pouzivane animace zobrazovani.
 * @param type 0-Preblikavani snimku, 1-posouvani z leva do prava, 2-posouvani z hora dolu.
 */
void SetAnimationType(uint8_t type) {
  if(type==0) {
    // Preblikavani snimku
    srowStep = 0;
    scolStep = 0;
  } else if (type==1) {
    // Posouvani z leva do prava
    srowStep = 0;
    scolStep = 1;
  } else if (type==2) {
    // Posouvani z hora dolu
    srowStep = 1;
    scolStep = 0;
  } else {
    srowStep = 0;
    scolStep = 0;
  }
  // Vymazat predchoziho stavu
  srow = 0;
  scol = 0;
}
/**
 * Funkce nastavuje orientaci radku displeje ku snimkum.
 * @param rowsOrientaion 0=Orientace1 nebo 0!=Orientace2
 */
void SetRowsOrientation(uint8_t orientaion) {
  rowsOrientaion = (orientaion)?0:1;
}
/**
 * Funkce nastavuje orientaci sloupcu displeje ku snimkum.
 * @param colsOrientaion 0=Orientace1 nebo 0!=Orientace2
 */
void SetColsOrientation(uint8_t orientaion) {
  colsOrientaion = (orientaion)?0:1;
}

// ========================================================================================================================

// WebServer naslouchajici na portu 80
ESP8266WebServer server(80); 

void initWifi() {
  // 192.168.4.1
  WiFi.mode(WIFI_AP);
  //WiFi.softAP("xxx", "xxxxxxxx");
  WiFi.softAP("xvalka05");
  WiFi.begin(); 
   
  server.on("/", routeHello);
  server.on("/args/", routeArgs);
  server.on("/set/", routeSetContent);
  server.on("/opt/", routeSetScreen);
  server.on("/text/", routeSetText);
  server.on("/load/", routeLoadAnimation);
  server.onNotFound([](){
    server.send(404, "text/plain", "404 Not Found");
  });
  
  server.begin();
}

void routeHello(){
  server.send(200,"text/plain","Hello");  
}

void routeArgs(){
  String message = "Args: \n";
  for (int i = 0; i < server.args(); i++) {
    message += server.argName(i) + ": ";
    message += server.arg(i) + "\n"; 
  }
  server.send(200, "text/plain", message); 
}

void routeSetScreen(){
  uint8_t tmpByte = 0;
  int tmpInt = 0;

  // rate : obnovovaci frekvence
  // cols : Zmeni orientaci spoupcu displeje
  // rows : Zmeni orientaci radku displeje
  // style: Zmeni styl animace snimku
  
  String message = "Args: \n";
  for (int i = 0; i < server.args(); i++) {
    message += server.argName(i) + ": ";
    message += server.arg(i) + "\n"; 

    if(server.argName(i)=="rate"){
      tmpInt = server.arg(i).toInt();
      if(tmpInt > 0) {
        refreshRate = tmpInt;
      }
    }
    else if(server.argName(i)=="cols"){
      SetColsOrientation(colsOrientaion);
      //colsOrientaion = (colsOrientaion)?0:1;
    }
    else if(server.argName(i)=="rows"){
      SetRowsOrientation(rowsOrientaion);
      //rowsOrientaion = (rowsOrientaion)?0:1;
    }
    else if(server.argName(i)=="style"){
      tmpInt = server.arg(i).toInt();
      SetAnimationType(tmpInt);
    }
    
  }
  server.send(200, "text/plain", message); 
}

// Nastaveni obsahu animace
void routeSetContent(){
  uint8_t numOfScr = 0;
  uint8_t tmpByte = 0;
  int tmpInt = 0;
  
  // s : cislo snimku, ktery se dale bude upravovat.
  // d : Trvani jednoho snimku
  // r0 - r7 : 8 radku screenu
  // l : pocet pouzivanych snimku
  // style : Zmeni styl animace snimku
  
  String message = "Args: \n";
  for (int i = 0; i < server.args(); i++) {
    message += server.argName(i) + ": ";
    message += server.arg(i) + "\n"; 

    if(server.argName(i)=="s"){
      tmpByte = server.arg(i).toInt();
      if(tmpByte < MAX_NUMBER_OF_SCREENS) {
        numOfScr = tmpByte;
      }
    }
    else if(server.argName(i)=="d"){
      tmpInt = server.arg(i).toInt();
      screenDurations[numOfScr] = tmpInt;
    }
    else if(server.argName(i)=="l"){
      tmpByte = server.arg(i).toInt();
      if(tmpByte <= MAX_NUMBER_OF_SCREENS) {
        numberOfScreens = tmpByte;
      }
      else {
        numberOfScreens = 1;  
      }
    }
    else if(server.argName(i)=="r0"){
      tmpByte = server.arg(i).toInt();
      screens[numOfScr][0] = tmpByte;
    }
    else if(server.argName(i)=="r1"){
      tmpByte = server.arg(i).toInt();
      screens[numOfScr][1] = tmpByte;
    }
    else if(server.argName(i)=="r2"){
      tmpByte = server.arg(i).toInt();
      screens[numOfScr][2] = tmpByte;
    }
    else if(server.argName(i)=="r3"){
      tmpByte = server.arg(i).toInt();
      screens[numOfScr][3] = tmpByte;
    }
    else if(server.argName(i)=="r4"){
      tmpByte = server.arg(i).toInt();
      screens[numOfScr][4] = tmpByte;
    }
    else if(server.argName(i)=="r5"){
      tmpByte = server.arg(i).toInt();
      screens[numOfScr][5] = tmpByte;
    }
    else if(server.argName(i)=="r6"){
      tmpByte = server.arg(i).toInt();
      screens[numOfScr][6] = tmpByte;
    }
    else if(server.argName(i)=="r7"){
      tmpByte = server.arg(i).toInt();
      screens[numOfScr][7] = tmpByte;
    }
    else if(server.argName(i)=="style"){
      tmpInt = server.arg(i).toInt();
      SetAnimationType(tmpInt);
    }
    
  }
  server.send(200, "text/plain", message); 
}

void routeSetText() {
  uint8_t tmpByte = 0;
  int tmpInt = 0;

  // t : text pro zobrazeni
  // d : doba animace prechodu mezi pismeny
  // style: Zmeni styl animace snimku
  
  String text = "?";
  uint8_t duration = 25;
  
  String message = "Args: \n";
  for (int i = 0; i < server.args(); i++) {
    message += server.argName(i) + ": ";
    message += server.arg(i) + "\n"; 

    if(server.argName(i)=="t"){
      text = server.arg(i);
    }
    else if(server.argName(i)=="d"){
      tmpInt = server.arg(i).toInt();
      duration = tmpInt;
    }
    else if(server.argName(i)=="style"){
      tmpInt = server.arg(i).toInt();
      SetAnimationType(tmpInt);
    }
    
  }
  LoadTextIntoScreens(text, duration);
  server.send(200, "text/plain", message); 
}

void routeLoadAnimation() {
  uint8_t tmpByte = 0;
  int tmpInt = 0;

  // n : oznaceni animace
  // d : doba animace prechodu mezi snimky
  // style: Zmeni styl animace snimku

  String n = "{";
  uint8_t duration = 30;
  
  String message = "Args: \n";
  for (int i = 0; i < server.args(); i++) {
    message += server.argName(i) + ": ";
    message += server.arg(i) + "\n"; 

    if(server.argName(i)=="n"){
      n = server.arg(i);
    }
    else if(server.argName(i)=="d"){
      tmpInt = server.arg(i).toInt();
      duration = tmpInt;
    }
    else if(server.argName(i)=="style"){
      tmpInt = server.arg(i).toInt();
      SetAnimationType(tmpInt);
    }
    
  }
  // Vyber animace
  if(n=="0") {
    LoadAnimation(SCR_DIAGONAL_LIGHT, NUMBER_OF_DIAGONAL_LIGHT, duration);
  }
  else if(n=="1") {
    LoadAnimation(SCR_DIAGONAL_LIGHT_LINE, NUMBER_OF_DIAGONAL_LIGHT_LINE, duration);
  }
  else if(n=="2") {
    LoadAnimation(SCR_GROW_RECT_FILL, NUMBER_OF_GROW_RECT_FILL, duration);
  }
  else if(n=="3") {
    LoadAnimation(SCR_GROW_RECT, NUMBER_OF_GROW_RECT, duration);
  }
  else if(n=="4") {
    LoadAnimation(SCR_FILLING, NUMBER_OF_FILLING, duration);
  }
  else if(n=="5") {
    LoadAnimation(SCR_STICKMAN, NUMBER_OF_STICKMAN, duration);
  }
  else if(n=="6") {
    LoadAnimation(SCR_MOVE_RECT, NUMBER_OF_MOVE_RECT, duration);
  }
  else if(n=="7") {
    LoadAnimation(SCR_ROTATE_LINE, NUMBER_OF_ROTATE_LINE, duration);
  }
  else if(n=="8") {
    LoadAnimation(SCR_ROT_FILL, NUMBER_OF_ROT_FILL, duration);
  }
  else if(n=="9") {
    LoadAnimation(SCR_LINE_PULS, NUMBER_OF_LINE_PULS, duration);
  }
  else {
    LoadTextIntoScreens(n, duration);
  }

  server.send(200, "text/plain", message); 
}
// ========================================================================================================================


/**
 * Funkce pro vykreslovani na display po jednotlivych diodach. 
 */
void refreshNextCell_v2() {
  // Posunuti sloupce displeje na kterem se ma pracovat s ledkou.
  col = NextNumber(col, 1, 8);
  if(col==0) {
    // Posunuti radku displeje na kterem se ma pracovat s ledkou.
    row = NextNumber(row, 1, 8);
    if(row==0) {
      // Posunuti casovace pro setrvani snimku/posuvne animace.
      screenDuration = NextNumber(screenDuration, 1, screenDurations[selectScreen]);
      if(screenDuration==0){
        // Hodnota posunuti sloupce, ktery se ma vykreslit.
        scol = NextNumber(scol, scolStep, 8);
        // Hodnota posunuti radku, ktery se ma vykreslit.
        srow = NextNumber(srow, srowStep, 8);
        if(scol==0 && srow==0){
          // Posunuti vybraneho snimku
          selectScreen = NextNumber(selectScreen, 1, numberOfScreens);
        } 
      }
    }
  }
  // Sloupec snimku, ktery bude vykreslen.
  uint8_t tmpcol = NextNumber(col, scol, 8);
  colData = 128 >> tmpcol;
  // Radek snimku, ktery bude vykreslen.
  uint8_t tmprow = NextNumber(row, srow, 8);
  // Index snimku ze ktereho maji byt cteny data.
  uint8_t tmpSelectScreen;
  if(col+scol<8 && row+srow<8){
    tmpSelectScreen = selectScreen;
  } else {
    // Posunuti snimku saha do dalsiho snimku.
    tmpSelectScreen = NextNumber(selectScreen, 1, numberOfScreens);
  }
  // Snimek ze ktereho maji byt cteny data.
  uint8_t *scr = screens[tmpSelectScreen];
  
  // row - Vybrani aktivniho radku na displeji.
  digitalWrite( D8, (row&1)?rowsValue[rowsOrientaion][0]:rowsValue[rowsOrientaion][1] );
  digitalWrite( D9, (row&2)?rowsValue[rowsOrientaion][0]:rowsValue[rowsOrientaion][1] );
  digitalWrite(D10, (row&4)?rowsValue[rowsOrientaion][0]:rowsValue[rowsOrientaion][1] );

  // coll - Vybrani aktivniho sloupce na displeji.
  digitalWrite( colsPin[colsOrientaion][col], (scr[tmprow]&colData)?LOW:HIGH );
}

/**
 * STARA
 * Funkce pro vykreslovani na display po jednotlivych diodach. 
 */
void refreshNextCell() {

  if(col<7){
    col++;
    colData = colData >> 1;
  }
  else {
    col = 0;
    colData = 128;
    //row = (row<7)?row+1:0;
    if(row<7){
      row++;
    }
    else {
      row = 0;
      screenDuration++;
      if(screenDuration >= screenDurations[selectScreen]) {
        screenDuration = 0;
        selectScreen++;
        if(selectScreen >= numberOfScreens) {
          selectScreen=0;  
        }
      }// if(screenDuration ...
    }// if(row<7)
  }// if(col<7)
  
  uint8_t *scr = screens[selectScreen];
  // row
  digitalWrite( D8, (row&1)?rowsValue[rowsOrientaion][0]:rowsValue[rowsOrientaion][1] );
  digitalWrite( D9, (row&2)?rowsValue[rowsOrientaion][0]:rowsValue[rowsOrientaion][1] );
  digitalWrite(D10, (row&4)?rowsValue[rowsOrientaion][0]:rowsValue[rowsOrientaion][1] );

  // coll
  digitalWrite( colsPin[colsOrientaion][col], (scr[row]&colData)?LOW:HIGH );
  //delayMicroseconds(250);
  //digitalWrite( colsPin[colsOrientaion][col], HIGH );

}

void cleanCell(){
  digitalWrite( colsPin[colsOrientaion][col], HIGH );
}

/*
// Prekreslovani displeje - 3 x 3
void scrRefresh() {
  uint8_t row;
  uint8_t col;
  uint8_t colData;
  uint8_t *scr = screens[selectScreen];
  
  for(row=0; row<8; row++){ 
    // rows
    digitalWrite( D8, (row&1)?rowsValue[rowsOrientaion][0]:rowsValue[rowsOrientaion][1] );
    digitalWrite( D9, (row&2)?rowsValue[rowsOrientaion][0]:rowsValue[rowsOrientaion][1] );
    digitalWrite(D10, (row&4)?rowsValue[rowsOrientaion][0]:rowsValue[rowsOrientaion][1] );

    // cols
    colData = 128;
    for(col=0; col<8; col++){
      digitalWrite( colsPin[colsOrientaion][col], (scr[row]&colData)?LOW:HIGH );
      delay(0);
      delayMicroseconds(250);
      digitalWrite( colsPin[colsOrientaion][col], HIGH );
      colData = colData >> 1;
    }
    
    screenDuration++;
    if(screenDuration >= screenDurations[selectScreen]) {
      screenDuration = 0;
      selectScreen++;
      if(selectScreen >= numberOfScreens) {
        selectScreen=0;  
      }
    }

    
  }
}
*/

void setup() {
  // put your setup code here, to run once:
  initWifi();
  //Serial.begin(115200);
  // cols
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  // rows
  pinMode(D8, OUTPUT);
  pinMode(D9, OUTPUT);
  pinMode(D10, OUTPUT);
  
  LoadTextIntoScreens("Ahoj kamo.", 15);
}

unsigned long timerTmp = 0, timer1 = 0;
uint8_t versionRefresh = 1;

void loop() {
  //Serial.print("Loop: ");
  //Serial.println(count++);
  server.handleClient();
  //scrRefresh();
  //refreshNextCell2();

  timerTmp = micros();

  if((unsigned long)(timerTmp - timer1) > refreshRate){
    timer1 = timerTmp;
    if(versionRefresh==0) {
      cleanCell();
      refreshNextCell_v2();
    } else {
      refreshNextCell_v2();
      delayMicroseconds(refreshRate);
      cleanCell();
    }
    //cleanCell(); // v1
    //refreshNextCell();
    //refreshNextCell_v2(); // v1 v2
    //delayMicroseconds(refreshRate); // v2
    //cleanCell(); // v2
  }
  
  //refreshNextCell();
  //delayMicroseconds(light);
  //cleanCell();
}
