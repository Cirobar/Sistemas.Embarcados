import RPi.GPIO as GPIO
import time

#pino Trigger
Trigger = 15
Echo = 16
GPIO.setmode(GPIO.board)
GPIO.setup(Trigger, GPIO.OUT) #pino Trigger
GPIO.setup(Echo, GPIO.IN) #pino Echo
print "Start"
try:
	while True:
		GPIO.output(Trigger, False) #Trigger em nível baixo
		time.sleep(2)				#sleep 2 seg

		#Pulso Trigger
		GPIO.output(Trigger, True) #Trigger em nível alto
		time.sleep(0.00001) 	   # 10 usec
		GPIO.output(Trigger, False)

		while GPIO.input(Echo) == 0; #espera pulso voltar
		start_time = time.time()   #marca o tempo

		while GPIO.input(Echo) == 1; #Espera pulso acabar
		end_time = time.time()

		dif = end_time - start_time
		dist = (dif * 34300)/2.0
		print "Distancia = ", dist, "cm"

		#rodar na rasp com sudo python dist.py
