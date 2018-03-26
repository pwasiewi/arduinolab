#!/usr/bin/python

# setup:  sr04 4-pin ultrasonic device, trigger pin4, echo pin6, supply = 5V
# (3.3V not adequate for SR04)

import time
import virtGPIO as GPIO

class Sonar:
  def __init__(self, trigger, echo):
      self.trigPin = trigger
      self.echoPin = echo
      GPIO.output(trigger, GPIO.LOW)  # attempt to pre-set it low before enabling output
      GPIO.setup(trigger, GPIO.OUT)
      GPIO.output(trigger, GPIO.LOW)
      GPIO.setup(echo, GPIO.IN)


  def ping(self):
      GPIO.pulseOut(self.trigPin, GPIO.HIGH, 10)
      GPIO.Serial.timeout = 1.2
      a = GPIO.pulseIn(self.echoPin, GPIO.HIGH, 600, 22000)
      GPIO.Serial.timeout = GPIO.STDTIMEOUT
      if a<18750:    # 250 cm
        return a//70   # result in cm
      else:
        return 0
      # In air, sound travels about 1 cm / 29 uSecs. Wikipedia. Return trip = 58 uSec / cm of range.
      # That's the theory! ie result = a/58.
      # However, my empirical calibration gives formula a/70.   YMMV - correct the scaling for yourself!



sonar1 = Sonar(4,6)   # trigger    echo

while True:
    print ("sonar: %d" % sonar1.ping())
    time.sleep(1)
