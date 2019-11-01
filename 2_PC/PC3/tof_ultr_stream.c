#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <errno.h>

#include <tof.h>

#define TRIG 5   //GPIO 24
#define ECHO 6   //GPIO 25

double dist0 = 0.0;
double dist1 = 0.0;
int stexec = 0;

void setup() {
       wiringPiSetup();
       pinMode(TRIG, OUTPUT);
       pinMode(ECHO, INPUT);

}

int tof(){
	int i;
	int iDistance;
	int model, revision;

	// Rpi3 conecta no canal 1
	// arg tofInit(int iChan, int iAddr, int bLongRange)
	i = tofInit(1, 0x29, 1); // modo long range (até 2m)
	if (i != 1)
	{
		return -1; //quit caso haja algum problema
 	}
	//printf("VL53L0X em funcionamento\n");
	i = tofGetModel(&model, &revision);
	//printf("Model ID - %d\n", model);
	//printf("Revision ID - %d\n", revision);

	iDistance = tofReadDistance();
	if (iDistance < 4096) {// distancia valida
		dist0 = iDistance/10.0;
		printf("Dist0 = %lf cm\n", dist0);
	}
		//usleep(100000); // 50ms
        return 1;
} //tof sensor

int ultr1() {
        //Manda pulso trig
        digitalWrite(TRIG, HIGH);
        delay(0.01);      //delayMicroseconds(20);usleep (20)
        digitalWrite(TRIG, LOW);


        while(digitalRead(ECHO) == 0); //Espera echo ficar em nível alto
        double startTime = micros(); 	//marcação de tempo
        while(digitalRead(ECHO) == 1); //espera echo acabar
        double endTime = micros(); //tempo do fim do pulso - tempo inicial
        double travelTime = endTime - startTime;

        //Conversão em cm
        dist1 = (travelTime/58.0); //

        //printf("Distance: %.5lf us\n", travelTime);
        printf("Dist1: %.3lf cm\n", dist1);
        return 2;
} //sensor ultrassom 1

void stream(){
	int fp, i=0;
	char ch;
	char det[35];
	char buffer_ent[100];
	char buffer_sai[100];


	system("vcgencmd get_camera >> camanalyser.txt");

	fp = open("camanalyser.txt", O_RDONLY);

	while(read(fp,&ch,1) > 0){
  		  det[i] = ch;
		     i++;
	}
	close(fp);
	if (det[21] == '0') { //posição da string que diz se a camera está conectada
		printf("A camera não está conectada\n Falha em abrir stream\n");
		system("curl -s -X POST https://api.telegram.org/bot106.../sendMessage -d chat_id=705... -d text='A stream falhou em ser executada'");
		system("sudo rm camanalyser.txt"); //remove arquivo de análise do status da câmera
		exit(-1);

	}
        system("sudo rm camanalyser.txt"); //remove arquivo de análise do status da câmera


	strcpy(buffer_ent,"uv4l --driver raspicam --auto-video_nr --width 640 --height 480 --encoding jpeg");
	//puts(buffer_ent);
	system(buffer_ent);
	usleep(500000);
	strcpy(buffer_sai,"dd if=/dev/video0 of=uv4image.jpeg bs=11M count=1");
	//puts(buffer_sai);
	system(buffer_sai);
	usleep(500000);

	strcpy(buffer_ent,"Stream executando...para parar aperte ctrl_c\n");
	puts(buffer_ent);

	system("curl -s -X POST https://api.telegram.org/bot106../sendMessage -d chat_id=705... -d text='A stream foi aberta'");
	// envia a mensagem "Stream foi aberta" no telegram
} //stream

void ctrl_c(int sig)
{
	system("sudo pkill uv4l"); // remove file video%d
	printf("Cancelando os processos\n");
	//sudo pkill uv4l
	exit(-1);
}

int main(int argc, char *argv[])
{

    //char ch;
     int fd[2];
    //int fd1[2];
    setup();  	//definindo as portas wiringpi
    pid_t pfilho;
    signal(SIGINT, ctrl_c);
    signal(SIGUSR1, stream);

    //pipe(fd);
    //pipe(fd1);

    pfilho = fork();

    if(pfilho == 0){
	printf("Processo filho\n");
	while(1){
	//write(fd[0],&stexec,1);
	}
    }
    else{
	while(1){
	delay(300);  //0.5s
	tof();
	ultr1();
		if(dist0 > 3 && dist0 < 5){
			while(stexec==0){
			usleep(300);
			kill(pfilho,SIGUSR1);
			//read(fd[0],&stexec,1);
			stexec = 1;
			system("curl -s -X POST https://api.telegram.org/bot106.../sendMessage -d chat_id=705... -d text='A stream foi aberta pelo tof'");
			}
		}
		if(dist1 > 3 && dist1 < 5){
			while(stexec==0){
			usleep(300);
			kill(pfilho,SIGUSR1);
			//read(fd[0],&stexec,1);
			stexec = 1;
			system("curl -s -X POST https://api.telegram.org/bot106.../sendMessage -d chat_id=705... -d text='A stream foi aberta pelo ultr1'");
			}
		}   // confere distancia do ultrassom e abre a stream caso necessário (não abre se já estiver aberta) aberta).
	}
    }
    return 0;
}
