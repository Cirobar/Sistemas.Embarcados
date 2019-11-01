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
#define PWM  4   //GPIO 23
 
double dist = 0.0;
int stexec = 0;
 
void setup() {
       wiringPiSetup();
       pinMode(TRIG, OUTPUT);
       pinMode(ECHO, INPUT);
       pinMode(PWM, OUTPUT); //motor
 
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
		dist = iDistance/10.0;
		printf("Distance = %lf cm\n", dist);
	}
		//usleep(100000); // 50ms
        return 1;
} //tof sensor

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
		system("curl -s -X POST https://api.telegram.org/bot1067349259:AAHQQ2jD7224MYYtyJr8s0aeJONG3aTHqHg/sendMessage -d chat_id=705655937 -d text='A stream falhou em ser executada'");
		system("sudo rm camanalyser.txt"); //remove arquivo de análise do status da câmera
		exit(-1);
	
	}
	system("sudo rm camanalyser.txt"); //remove arquivo de análise do status da câmera
	
	stexec=1;
	strcpy(buffer_ent,"uv4l --driver raspicam --auto-video_nr --width 640 --height 480 --encoding jpeg");
	//puts(buffer_ent);
	system(buffer_ent);
	usleep(300000);
	strcpy(buffer_sai,"dd if=/dev/video0 of=uv4image.jpeg bs=11M count=1");
	//puts(buffer_sai);
	system(buffer_sai);
	usleep(300000);

	strcpy(buffer_ent,"Stream executando...para parar aperte ctrl_c\n");
	puts(buffer_ent);
	
	system("curl -s -X POST https://api.telegram.org/bot1067349259:AAHQQ2jD7224MYYtyJr8s0aeJONG3aTHqHg/sendMessage -d chat_id=705655937 -d text='A stream foi aberta'");
} //stream

void sg90 (int pin, int angulo, int N)
{
        int t1 = (100*angulo+4)/9+1500;
        int t2 = 20000-t1;
        int i;
        for(i=0; i<N; i++)
        {
                digitalWrite(pin, HIGH);
                usleep(t1);
                digitalWrite(pin, LOW);
                usleep(t2);

        }
} //motor sg90

void ctrl_c(int sig)
{
	system("sudo pkill uv4l"); // remove file video%d
	printf("Cancelando os processos\n");
	exit(-1);
}//interrupção ctrl_c

int main(int argc, char *argv[])
{
    
    //char ch;
    //int fd[2];
    //int fd1[2];
    //setup();
    pid_t pfilho;
    signal(SIGINT, ctrl_c);
    signal(SIGUSR1, stream);
    
    //pipe(fd);
    //pipe(fd1);
    
    pfilho = fork();
		
    if(pfilho == 0){
	while(1){
	//espera sinal pra executar a stream	
	} 
    }
    else{
	while(1){
	delay(300);  //0.5s
	tof();	
		if(dist > 3 && dist < 5){
			while(1){
			sg90(PWM, 50, 40);
			}
			while(stexec==0){ 
			usleep(500);	
			kill(pfilho,SIGUSR1);
			stexec = 1;	
			}
		
		}
	}
    }	
    return 0;
}
