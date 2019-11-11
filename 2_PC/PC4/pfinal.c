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
#include <pthread.h>

#include <tof.h>

#define TRIG 5   //GPIO 24
#define ECHO 6   //GPIO 25
#define TRIG2 21   //GPIO 5
#define ECHO2 22   //GPIO 6
#define MOTOR 1	   //GPIO 18

int stexec = 0;
double dist0 = 0.0;
double dist1 = 0.0;
double dist2 =0.0;

void setup() {
       wiringPiSetup();
       pinMode(TRIG, OUTPUT);
       pinMode(ECHO, INPUT);
       pinMode(TRIG2, OUTPUT);
       pinMode(ECHO2, INPUT);
       pinMode(MOTOR, OUTPUT);
}

void sg90up ()
{
        int t1 = (100*10+4)/9+1500;
        int t2 = 20000-t1;
        int i;
        for(i=0; i<40; i++)
        {
                digitalWrite(MOTOR, HIGH);
                usleep(t1);
                digitalWrite(MOTOR, LOW);
                usleep(t2);

        }
} //Servo motor para cima

void sg90down (){
        int t1 = (100*50+40)/9+1500;
        int t2 = 20000-t1;
        int i;
        for(i=0; i<40; i++)
        {
                digitalWrite(MOTOR, HIGH);
                usleep(t1);
                digitalWrite(MOTOR, LOW);
                usleep(t2);

	}
} // Servo motor para baixo

void* tof(void *arg){
	int i;
	int iDistance;
	int model, revision;

	// Rpi3 conecta no canal 1
	// arg tofInit(int iChan, int iAddr, int bLongRange)
	i = tofInit(1, 0x29, 1); // modo long range (até 2m)
	//if (i != 1)
	//{
	//	return -1; //quit caso haja algum problema
 	//}
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
        return NULL; //return 1
} //tof sensor

void* ultr1(void* arg) {
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
        return NULL;
} //thread sensor ultrassom 1

void* ultr2(void* arg) {
        //Manda pulso trig
        digitalWrite(TRIG2, HIGH);
        delay(0.01);      //delayMicroseconds(20);usleep (20)
        digitalWrite(TRIG2, LOW);

        while(digitalRead(ECHO2) == 0); //Espera echo ficar em nível alto
        double startTime = micros(); 	//marcação de tempo
        while(digitalRead(ECHO2) == 1); //espera echo acabar
        double endTime = micros(); //tempo do fim do pulso - tempo inicial
        double travelTime = endTime - startTime;

        //Conversão em cm
        dist2 = (travelTime/58.0); //

        //printf("Distance: %.5lf us\n", travelTime);
        printf("Dist2: %.3lf cm\n", dist2);
        return NULL;
} //thread sensor ultrassom 2

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
		system("curl -s -X POST https://api.telegram.org/bot106/sendMessage -d chat_id=705-d text='A stream falhou em ser executada'");
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

} //stream

int readtxt(){
	int fd;
	int ch;
	fd = open("stream.txt", O_RDONLY);
	if(fd < 0)
	{
		printf("Erro ao abrir arquivo!\n");
		return -1;
	}
	read(fd,&ch,1); //lendo o arquivo e armazenando
	close(fd);

	if(ch == '1'){
		stexec = 1;
	}
	else{
		stexec = 0;
	}
	return 0;
}

int writetxt(){
	char ch;
	if(stexec==0)
	{
		ch = '0';
	}
	else
	{
		ch = '1';
	}

	int fout = open("stream.txt", O_WRONLY | O_TRUNC);
	if(fout < 0)
	{
		printf("Erro ao abrir arquivo!\n");
		return -1;
	}
	write(fout,&ch,1); //lendo o arquivo e armazenando
	close(fout);
	return 0;
}

void ctrl_c(int sig)
{
     system("sudo pkill uv4l"); // remove file video%d
     //system("sudo rm stream.txt"); // remove file video%d
     printf("Cancelando os processos\n");
     //sudo pkill uv4l
     //system("rm stream.txt");
     exit(-1);
}

int main(int argc, char *argv[])
{

    setup();  	//definindo as portas wiringpi
    pid_t pfilho,pfilhobot;
    pthread_t thr_0, thr_1, thr_2;
    signal(SIGINT, ctrl_c);
    signal(SIGUSR1, stream);


    pfilho = fork();

    if(pfilho == 0)
    {
	     printf("Processo filho\n");
	     while(1){
	     }
    }

    else
    {
	pfilhobot = fork();
	if(pfilhobot == 0)
	{
		system("./telegram_bot");  //executável em c++ do bot
	}

	else
	{
		while(1)
		{
			usleep(50000);
			pthread_create(&thr_0, NULL, &tof, NULL);
			pthread_create(&thr_1, NULL, &ultr1, NULL);
			pthread_create(&thr_2, NULL, &ultr2, NULL);
			pthread_join(thr_0, NULL);
			pthread_join(thr_1, NULL);
			pthread_join(thr_2, NULL);

			if(dist0 > 1 && dist0 < 5)
			{
				usleep(50000);
				readtxt();
				usleep(50000);
				printf("stexec %d\n",stexec);
				while(stexec==0)
				{
					kill(pfilho,SIGUSR1);
					system("curl -s -X POST https://api.telegram.org/bot106/sendMessage -d chat_id=705 -d text='A stream foi aberta pelo tof'");
					stexec = 1;
					usleep(50000);
					writetxt();
				}
				sg90up();
				usleep(1000000);
			 }
			if(dist1 > 1 && dist1 < 5)
			{
				usleep(50000);
				readtxt();
				usleep(50000);
				printf("stexec %d\n",stexec);
				while(stexec==0)
				{
					kill(pfilho,SIGUSR1);
					system("curl -s -X POST https://api.telegram.org/bot106/sendMessage -d chat_id=705 -d text='A stream foi aberta pelo ultrassom'");
					stexec = 1;
					writetxt();
				}
			 }// confere distancia do ultrassom e abre a stream caso necessário (não abre se já estiver aberta) aberta).
			 if(dist2 > 1 && dist2 < 5)
			{
				usleep(50000);
				readtxt();
				usleep(50000);
				printf("stexec %d\n",stexec);
				while(stexec==0)
				{
					kill(pfilho,SIGUSR1);
					system("curl -s -X POST https://api.telegram.org/bot106/sendMessage -d chat_id=705 -d text='A stream foi aberta pelo tof'");
					stexec = 1;
					writetxt();
				}
			 }
		}
	}
     }
    return 0;
}
