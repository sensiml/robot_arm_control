/*
  BraccioController.ino

  This program uses a command line interface. (CommandLine.h)
  Instruction words command the robot on certain motions it has to perform.
  After downloading code, open serial monitor and enter the commands. 
  "Done 1" is sent out once the instruction has been completed (can be adjusted to something else)
  Gives user more control on what it wants the arm to do.

  Created on 11 Aug 2021
  by Anoushka Gupta

*/

#include "CommandLine.h"    //contains all callable functions with instruction words.
#include <Braccio.h>
#include <Servo.h>

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;

  void
  setup() {
    //Initialization functions and set up the initial position for Braccio
    //All the servo motors will be positioned in the "safety" position:
    //Base (M1):0 degrees => since M1 doesn't work correctly on my arm, I'm negating it
    //Shoulder (M2): 45 degrees
    //Elbow (M3): 180 degrees
    //Wrist vertical (M4):180 degrees
    //Wrist rotation (M5): 90 degrees
    //gripper (M6): 10 degrees => gripper in open state
    Braccio.begin();
    Serial.begin(115200);
  }

  void
  loop() {
    bool received = getCommandLineFromSerialPort(CommandLine);      //global CommandLine is defined in CommandLine.h
    if (received) DoMyCommand(CommandLine);
  }
