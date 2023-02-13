/*
  Test of 6 x 6 Reed Switches
  using input pins 2-7 for columns
  and input pins 8-13 for rows
*/

#define BOARD_SIZE 2               // number of columns and rows in the board
#define N_LEDS 8 * 8               // number of individual LEDs in one neopixel strip
#define BOARD_PIN 12               // pin for the neopixel strip
int columnPins[] = {8, 9, 10, 11}; // pins for columns
int rowPins[] = {2, 3, 4, 5};      // pins for rows

const int inputButtons[] = {3,5,6,7,8,9,10,11,12,A1};
const int buttonDebounceTime  = 300;

int currentBoard[8][8]; //an array to track the current state of the board
String piStarted = "No";

String humansMove;
String pisMove;
String pisSuggestedBestMove;
String gameMode = "0";  // can be either 'Stockfish' or 'OnlineHuman' once set
String colourChoice; // can be either 'black' or 'white'
bool legalOrNot;

void setup()
{
    // initialize serial communication
    Serial.begin(9600);

    // initialize columns to output
    for (int i = 0; i < sizeof(columnPins) / sizeof(int); i++)
        pinMode(columnPins[i], OUTPUT);

    // initialize rows to input
    for (int i = 0; i < sizeof(rowPins) / sizeof(int); i++)
        pinMode(rowPins[i], INPUT);

    /*
      initialize neopixel strip here // TODO
    */
    setUpBoard();
    waitForPiToStart();
    setUpGame();
}

void loop()
{
    // constantly check for changes in the grid
    // checkGrid();
    // delay(1000);
    humansMove = humansGo();
    sendToPi(humansMove, "M");
    printBoard();
    legalOrNot = checkPiForError();
    Serial.println("Checking legality");
    if (legalOrNot == false ){
      // do nothing and start the human request again
      Serial.println("Move discarded, please return the pieces and try another move");
      printBoard();
    } else {
      Serial.println("Move is legal");
      updateBoard(humansMove);
      printBoard();
      pisMove = receiveMoveFromPi();
      if (gameMode != "OnlineHuman"){
        pisSuggestedBestMove = pisMove.substring(5);
        Serial.println("Suggested next move = " + pisSuggestedBestMove);
      }
      Serial.println(pisMove);
      updateBoard(pisMove);
      printBoard();
    }
}

String receiveMoveFromPi(){
  Serial.print("Function:receiveMoveFromPi...   ");
    Serial.println("Waiting for response from Raspberry Pi");
    while (true){
      if (Serial.available() > 0){
        String data = Serial.readStringUntil('\n');
        Serial.println(data);
        if (data.startsWith("heyArduinom")){
          Serial.print("Move received from Pi: ");
          data = data.substring(11);
          Serial.println(data);
          return data;
        } else if (data.startsWith("heyArduinoerror")){
          errorFromPi();
          return "error";
        }
      }
    }
  }

bool checkPiForError(){  //check five times during the next 03 seconds to see if we received an error from maxchess on the pi - if so run errorFromPi()
    Serial.print("Function:checkPiForError...   ");
    Serial.println("Waiting for response from Raspberry Pi");
    int checked = 0;
    while (checked<30){
      if (Serial.available() > 0){
        String data = Serial.readStringUntil('\n');
        Serial.println(data);
        if (data.startsWith("heyArduinoerror")){
          Serial.print("Error received from Pi: ");
          Serial.println(data);
          errorFromPi();
          return false;
         }
       } else {
        delay(100);
        Serial.println(checked);
        checked++;
      }
    }
    return true;
  }

void errorFromPi(){
  //Serial.println("Error received from Raspberry Pi");
  int times = 0;
  while (times < 3){
    delay(500);
    delay(500);
    times++;   
  }
}

void printBoard(){
  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
      Serial.print(currentBoard[i][j]);
      Serial.print(",");
    }
    Serial.println();
  }
}

void sendToPi(String message, String messageType){
  String messageToSend = message;
  Serial.println("heypi" + messageType + messageToSend);
}

void checkGrid()
{
    for (int i = 0; i < sizeof(columnPins) / sizeof(int); i++)
    {
        digitalWrite(columnPins[i], HIGH);
        for (int j = 0; j < sizeof(rowPins) / sizeof(int); j++)
        {
            if (digitalRead(rowPins[j]) == HIGH)
                setLED(i, j); // TODO: update to setLED(x, y) function
            else
                clearLED(i, j); // TODO: update to clearLED(x, y) function
        }
        digitalWrite(columnPins[i], LOW);
    }
}

String humansGo(){
  bool moveConfirmed = false;
  int buttonPressed = 0;
  String moveFrom;
  String moveTo;
  String humansMove;

  while (moveConfirmed == false){ 
    moveFrom = getCoordinates(buttonPressed); //get move from the player via the buttons
    moveTo = getCoordinates(0);
    buttonPressed = 0; //clear the variable for another time around this while loop if required
    Serial.println("You're moving the piece on " + moveFrom + " to " + moveTo);
    humansMove = moveFrom + moveTo; //combine the moves into one four character command
    
    return humansMove;
    
  }
  
}

int columnNumber(char column){
  Serial.println("Function: columnNumber");
  //Serial.println(column);
  switch (column){
    case 'a':
    //Serial.println("Column A converted to number 0.");
    return 0;
    case 'b':
    return 1;
    case 'c':
    return 2;
    case 'd':
    //Serial.println("Column D converted to number 3.");
    return 3;
    case 'e':
    //Serial.println("Column E converted to number 4.");
    return 4;
    case 'f':
    return 5;
    case 'g':
    return 6;
    case 'h':
    return 7;
    default:
    Serial.println("No case statement found!");
  }
}

void updateBoard(String moveToUpdate){
  Serial.print("Function:updateBoard - Piece to update: ");
  Serial.println(moveToUpdate);
  
  int columnMovedFrom = columnNumber(moveToUpdate[0]);
  char rowMovedFrom = moveToUpdate[1];
  int irowMovedFrom = 7-(rowMovedFrom - '1');
  currentBoard[irowMovedFrom][columnMovedFrom] = 0;

  int columnMovedTo = columnNumber(moveToUpdate[2]);
  char rowMovedTo = moveToUpdate[3];
  int irowMovedTo = 7-(rowMovedTo - '1');
  currentBoard[irowMovedTo][columnMovedTo] = 1;
}

void setUpBoard(){
    //set up the inital starting positions of chess pieces in the array
  for(int i = 0; i < 2; i++) {
    for(int j = 0; j < 8; j++) {
      currentBoard[i][j] = 1;
    }
  }
  for(int i = 2; i < 6; i++) {
    for(int j = 0; j < 8; j++) {
      currentBoard[i][j] = 0;
    }
  }
   for(int i = 6; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
      currentBoard[i][j] = 1;
    }
  }
}

void waitForPiToStart(){
    Serial.println("Function:waitForPiToStart...   ");
    int chessSquaresLit = 0;
    while (true){
      delay(1000);
      if (Serial.available() > 0){
        String data = Serial.readStringUntil('\n');
        Serial.println(data);
        if (data.startsWith("heyArduinoChooseMode")){
          while (chessSquaresLit < 64){
            delay(15);
          }
          //turn on the control panels lights
          
          while (true) {
              
            gameMode = "Stockfish";
            sendToPi(gameMode, "G");
            Serial.print("Pi is going to start a game with Stockfish.");
            //chessboardLEDS.drawPixel(0,0,GREEN);
            //chessboardLEDS.show();
            delay(500);
            //chessboardLEDS.drawPixel(0,0,BLACK);
            //chessboardLEDS.show();
            delay(500);
            //chessboardLEDS.drawPixel(0,0,GREEN);
            //chessboardLEDS.show();
            delay(500);
            //chessboardLEDS.drawPixel(0,0,BLACK);
            //chessboardLEDS.show();
            delay(500);
            break;
              
            }
          }
          if (gameMode != "0"){
            break;
          }
        }
      }
    }

void setLED(int x, int y)
{
    Serial.print("LED is on at: ");
    Serial.print(x);
    Serial.print(',');
    Serial.print(y);
    Serial.println();
    // strip.setPixelColor(matrix[x][y], c);
    // strip.show();
}

// turn off a single LED
void clearLED(int x, int y)
{
    Serial.print("LED is off at: ");
    Serial.print(x);
    Serial.print(',');
    Serial.print(y);
    Serial.println();
    // strip.setPixelColor(matrix[x][y], strip.Color(0, 0, 0, 0));
    // strip.show();
}

void setUpGame(){
  //turn on the control panels lights
 if (gameMode == "Stockfish"){
  String gameDifficulty;
  String gameTimeout;
  
  //Serial.print("Set difficulty level: ");
  
  gameDifficulty = "2";
  delay(buttonDebounceTime);
  if (gameDifficulty.length() < 2){
    Serial.println("0" + gameDifficulty);
    gameDifficulty = ("0" + gameDifficulty);
  }
  Serial.println(gameDifficulty);
  sendToPi(gameDifficulty, "-");

  Serial.print("Set computers move timout setting: ");
  
  gameTimeout = "7";
  delay(buttonDebounceTime);
  //Serial.println(gameTimeout);
  sendToPi(gameTimeout, "-");
 } else if (gameMode == "OnlineHuman"){
  while (true){
    colourChoice = "1";
    delay(buttonDebounceTime);
    if (colourChoice == "1"){
      colourChoice = "White";
      Serial.println("Chossen colour is white");
      sendToPi(colourChoice, "C");
      break;
    } else if (colourChoice == "2"){
      colourChoice = "Black";
      Serial.println("Chossen colour is black");
      sendToPi(colourChoice, "C");
      break;
    }
  }
 }
}

String getCoordinates(int buttonAlreadyPressed){
  //Serial.println("Getting co-ordinates...");
  int temp;
  String coordinates;
  String column = "x";
  String row = "x";
  
  while (column == "x"){
    //Serial.println("Waiting for user to input column via button press...");
//    if (buttonAlreadyPressed != 0){
//      temp = buttonAlreadyPressed;
//    } else {
    temp = random(8) + 1;
    //}
    
    switch (temp){
      case 1:
      column = "a";
      break;
      case 2:
      column = "b";
      break;
      case 3:
      column = "c";
      break;
      case 4:
      column = "d";
      break;
      case 5:
      column = "e";
      break;
      case 6:
      column = "f";
      break;
      case 7:
      column = "g";
      break;
      case 8:
      column = "h";
      break;
      default:
      break;
    }
  }
  
  //delay(buttonDebounceTime);
  
  while (row == "x"){
    //Serial.println("Waiting for user to input row via button press...");
    temp = random(8) + 1;
    switch (temp){
      case 1:
      row = "1";
      break;
      case 2:
      row = "2";
      break;
      case 3:
      row = "3";
      break;
      case 4:
      row = "4";
      break;
      case 5:
      row = "5";
      break;
      case 6:
      row = "6";
      break;
      case 7:
      row = "7";
      break;
      case 8:
      row = "8";
      break;
      default:
      break;
    }
  }
  coordinates = column + row;
  //Serial.println("Co-ordinates are " + coordinates);
  delay(buttonDebounceTime);
  return coordinates;
}

void hint(){
 static unsigned long last_interrupt_time = 0;
 unsigned long interrupt_time = millis();
 // If interrupts come faster than 200ms, assume it's a bounce and ignore
 if (interrupt_time - last_interrupt_time > 200) 
 {
  if (digitalRead(A1) == LOW){
    Serial.println("Starting a new game....");
    sendToPi("","n");
    int var1 = 0;
    while (var1 < 64){
            //var1 = loadingStatus(var1);
            delay(25);
          }
    delay(1000);
    return;
  } else if (digitalRead(12) == LOW){
    sendToPi("shutdown", "x");
  } else if (pisSuggestedBestMove.length() != 1){
    Serial.println("Suggested best move= " + pisSuggestedBestMove);
    
  } else {
    Serial.println("No hint provided by pi yet.");
  }
 }
 last_interrupt_time = interrupt_time;
}
