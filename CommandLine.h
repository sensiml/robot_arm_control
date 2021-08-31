#include <string.h>
#include <stdlib.h>
#include <Braccio.h>
#include <Servo.h>

#define print2(x,y) (Serial.print(x), Serial.println(y))

#define CR '\r'
#define LF '\n'
#define BS '\b'
#define NULLCHAR '\0'
#define SPACE ' '

#define COMMAND_BUFFER_LENGTH        25                        //length of serial buffer for incoming commands
char   CommandLine[COMMAND_BUFFER_LENGTH + 1];                 //Read commands into this buffer from Serial.  +1 in length for a termination char

const char *delimiters            = ", \n";                    //commands can be separated by return, space or comma

/*************************************************************************************************************
     your Command Names Here
*/
const char *addCommandToken       = "add";                     //add
const char *subtractCommandToken  = "sub";                     //sub operators to check if command line is running or not

const char *standCommandToken     = "stand";                   //stand position
const char *startCommandToken     = "start";                   //start position
const char *reachCommandToken     = "reach";                   //reach for object

const char *stand_startCommandToken     = "stand_start";                 //start position
const char *start_reachCommandToken     = "start_reach";                 //reach for object
const char *reach_startCommandToken     = "reach_start";                 //start position
const char *start_standCommandToken     = "start_stand";                 //stand position

const char *grabCommandToken      = "grab";                  //grab object
const char *liftCommandToken      = "lift";                  //lift object
const char *holdCommandToken      = "hold";                  //hold up object
const char *downCommandToken      = "down";                  //place down object
const char *releaseCommandToken   = "release";               //release object


/*************************************************************************************************************
    getCommandLineFromSerialPort()
      Return the string of the next command. Commands are delimited by return"
      Handle BackSpace character
      Make all chars lowercase
*************************************************************************************************************/

bool
getCommandLineFromSerialPort(char * commandLine)
{
  static uint8_t charsRead = 0;                      //note: COMAND_BUFFER_LENGTH must be less than 255 chars long
  //read asynchronously until full command input
  while (Serial.available()) {
    char c = Serial.read();
    switch (c) {
      case CR:      //likely have full command in buffer now, commands are terminated by CR and/or LS
      case LF:
        commandLine[charsRead] = NULLCHAR;       //null terminate our command char array
        if (charsRead > 0)  {
          charsRead = 0;                           //charsRead is static, so have to reset
          Serial.println(commandLine);
          return true;
        }
        break;
      case BS:                                    // handle backspace in input: put a space in last char
        if (charsRead > 0) {                        //and adjust commandLine and charsRead
          commandLine[--charsRead] = NULLCHAR;
          Serial << byte(BS) << byte(SPACE) << byte(BS); 
        }
        break;
      default:
        // c = tolower(c);
        if (charsRead < COMMAND_BUFFER_LENGTH) {
          commandLine[charsRead++] = c;
        }
        commandLine[charsRead] = NULLCHAR;     //just in case        
        break;
    }
  }
  return false;
}


/* ****************************
   readNumber: return a 16bit (for Arduino Uno) signed integer from the command line
   readWord: get a text word from the command line

*/
int
readNumber () {
  char * numTextPtr = strtok(NULL, delimiters);       
  return atoi(numTextPtr);                              
}

char * readWord() {
  char * word = strtok(NULL, delimiters);               
  return word;
}

void
nullCommand(char * ptrToCommandName) {
  print2("Command not found: ", ptrToCommandName);      //see above for macro print2
}

/****************************************************
   Add your commands here
*/

int addCommand() {                                      
  int firstOperand = readNumber();
  int secondOperand = readNumber();
  return firstOperand + secondOperand;
}

int subtractCommand() {                                
  int firstOperand = readNumber();
  int secondOperand = readNumber();
  return firstOperand - secondOperand;
}

int standCommand() {                                      
  // the arm is aligned upwards  and the gripper is closed
                     //(step delay, M1, M2, M3, M4, M5, M6);
  Braccio.ServoMovement(20,         90, 90, 90, 90, 90,  73);  //straight up position

  //Wait 2 second, check if it successfully attained straight up position
  delay(2000);
  return 1;
}

int start_standCommand() {                                      
  // the arm is aligned upwards  and the gripper is closed
                     //(step delay, M1, M2, M3, M4, M5, M6);
  Braccio.ServoMovement(20,         90, 90, 90, 90, 90,  73);  //straight up position

  //Wait 2 second, check if it successfully attained straight up position
  delay(2000);
  return 1;
}

int startCommand() {                               
  //Starting position, positioned it so instead of picking up the object from the top, it aligns to lift from the side
                      //(step delay  M1 , M2 , M3 , M4 , M5 , M6);
  Braccio.ServoMovement(20,           0, 45, 180, 180,  90,  10);
  
  //Wait 2 second, check if it successfully attained start position
  delay(2000);
  return 1;
}

int stand_startCommand() {                               
  //Starting position, positioned it so instead of picking up the object from the top, it aligns to lift from the side
                      //(step delay  M1 , M2 , M3 , M4 , M5 , M6);
  Braccio.ServoMovement(20,           0, 45, 180, 180,  90,  10);
  
  //Wait 2 second, check if it successfully attained start position
  delay(2000);
  return 1;
}

int reach_startCommand() {                               
  //Starting position, positioned it so instead of picking up the object from the top, it aligns to lift from the side
                      //(step delay  M1 , M2 , M3 , M4 , M5 , M6);
  Braccio.ServoMovement(20,           0, 45, 180, 180,  90,  10);
  
  //Wait 2 second, check if it successfully attained start position
  delay(2000);
  return 1;
}

int reachCommand() {
  //The braccio moves to the sponge. Both M2 and M4 servo will move
  Braccio.ServoMovement(20,           0,  150, 180, 60,  90,  10);
  //Braccio.ServoMovement(20,           0,  90, 180, 180,  90,   10);
  return 1;
}

int start_reachCommand() {
  //The braccio moves to the sponge. Both M2 and M4 servo will move
  Braccio.ServoMovement(20,           0,  150, 180, 60,  90,  10);
  //Braccio.ServoMovement(20,           0,  90, 180, 180,  90,   10);
  return 1;
}

int grabCommand() {
  //Close the gripper to take the sponge. Only the M6 servo will move to grab the object from the side
  Braccio.ServoMovement(10,           0,  150, 180, 60,  90,  73 );
  return 1;
}

int liftCommand() {
  //Brings the sponge upwards. Only M2 will move
  Braccio.ServoMovement(20,         0,   90, 180,  60,  90, 73);
  return 1;
}

int holdCommand() {
  //Hold up and show object. Wait 2 seconds to hold it up
  delay(2000);
  return 1;
}

int downCommand() {
  //Place object back down
  Braccio.ServoMovement(20,         0,  150, 180, 60,  90, 73);
  return 1;
}

int releaseCommand() {
  //Open the gripper
  Braccio.ServoMovement(20,         0,   150, 180,  60,  90, 10 );
  return 1;
}

/****************************************************
   DoMyCommand
*/
bool
DoMyCommand(char * commandLine) {
  //  print2("\nCommand: ", commandLine);
  int result;

  char * ptrToCommandName = strtok(commandLine, delimiters);
  //  print2("commandName= ", ptrToCommandName)
  
  if (strcmp(ptrToCommandName, standCommandToken) == 0) {                   
    result = standCommand();
    
  }

  else if (strcmp(ptrToCommandName, startCommandToken) == 0) {                   
    result = startCommand();
    print2(">    Done ", result);
  }

  else if (strcmp(ptrToCommandName, reachCommandToken) == 0) {                   
    result = reachCommand();
    print2(">    Done ", result);
  }

  else if (strcmp(ptrToCommandName, stand_startCommandToken) == 0) {                   
    result = stand_startCommand();
    print2(">    Done ", result);
  }

  else if (strcmp(ptrToCommandName, start_reachCommandToken) == 0) {                   
    result = start_reachCommand();
    print2(">    Done ", result);
  }

  else if (strcmp(ptrToCommandName, reach_startCommandToken) == 0) {                   
    result = reach_startCommand();
    print2(">    Done ", result);
  }

  else if (strcmp(ptrToCommandName, start_standCommandToken) == 0) {                   
    result = start_standCommand();
    print2(">    Done ", result);
  }

  else if (strcmp(ptrToCommandName, grabCommandToken) == 0) {                   
    result = grabCommand();
    print2(">    Done ", result);
  }

  else if (strcmp(ptrToCommandName, liftCommandToken) == 0) {                   
    result = liftCommand();
    print2(">    Done ", result);
  }

  else if (strcmp(ptrToCommandName, holdCommandToken) == 0) {                   
    result = holdCommand();
    print2(">    Done ", result);
  }

  else if (strcmp(ptrToCommandName, downCommandToken) == 0) {                   
    result = downCommand();
    print2(">    Done ", result);
  }

  else if (strcmp(ptrToCommandName, releaseCommandToken) == 0) {                   
    result = releaseCommand();
    print2(">    Done ", result);
  }

  else if (strcmp(ptrToCommandName, addCommandToken) == 0) {                   
    result = addCommand();
    print2(">    The sum is = ", result);

  } else {
    if (strcmp(ptrToCommandName, subtractCommandToken) == 0) {           
      result = subtractCommand();                                       
      print2(">    The difference is = ", result);

    } else {
      result = standCommand();
    }
  }
}
