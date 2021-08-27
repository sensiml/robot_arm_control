#   Program to automate data collection thru open gateway:
#   Loops thru decided order of gestures and captures each gesture switch
#   Starts, stops recording and downloads captured data (.csv file + video)

import serial
import requests
import time

PORT = "COM4"           #serial port connection to arduino uno board
BAUD_RATE = 115200      #baurate matching serial monitor on IDE & BraccioController.ino


def send_command(ser, command):
    ser.write(str.encode(command))


if __name__ == "__main__":

    with serial.Serial(PORT, BAUD_RATE, timeout=1) as ser:

        r = requests.post(
            "http://127.0.0.1:5555/record", data={"event_type": "record-stop"}      #stops recording for initialization
        )
        send_command(ser, "start")
        time.sleep(3)

        steps_to_run = ["stand", "start", "reach", "start" , "stand"]

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
                time.sleep(2)
                send_command(ser, step)
                print("command {step} sent".format(step=step))
                time.sleep(5)
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
