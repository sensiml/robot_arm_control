#   Program to automate data collection thru open gateway:
#   Loops thru decided order of gestures and captures each gesture switch
#   Starts, stops recording and downloads captured data (.csv file + video)

import serial
import requests
import time

PORT = "COM4"           #serial port connection to arduino uno board
BAUD_RATE = 115200      #baurate matching serial monitor on IDE & BraccioController.ino


def send_command(ser, command):
    ser.write(str.encode(command+"\n"))


if __name__ == "__main__":

    with serial.Serial(PORT, BAUD_RATE, timeout=2) as ser:
        time.sleep(15)

        r = requests.post(
            "http://127.0.0.1:5555/record", data={"event_type": "record-stop"}      #stops recording for initialization
        )

        send_command(ser, "stand")
        time.sleep(5)

        steps_to_run = ["stand_start", "start_reach", "reach_start", "start_stand"]

        num_iterations = 2

        for iteration in range(num_iterations):
            for step in steps_to_run:
                r = requests.post(
                    "http://127.0.0.1:5555/record",
                    data={
                        "event_type": "record-start",               #starts recording
                        "filename": "{step}_{iteration}".format(
                            step=step, iteration=iteration
                        ),
                    },
                )
                print(r.json())
                time.sleep(5)
                send_command(ser, step)
                print("command {step} sent".format(step=step))
                time.sleep(10)
                r = requests.post(
                    "http://127.0.0.1:5555/record",
                    data={
                        "event_type": "record-stop",                #stops recording
                        "filename": "{step}_{iteration}".format(
                            step=step, iteration=iteration
                        ),
                    },
                )
                print(r.json())
