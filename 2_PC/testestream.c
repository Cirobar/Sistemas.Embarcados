#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    char ch;
    int fd[2];
		int fd1[2];1000 Mb/s
    char buffer_ent[100];
    char buffer_sai[100];
    pid_t pid;
    pipe(fd);
		pipe(fd1);
    pid = fork();
		int j;

    if(pid == 0){
      strcpy(buffer_ent,"uv4l --driver raspicam --auto-video_nr --width 640 --height 480 --encoding jpeg");
      write(fd[1],buffer_ent,100);
      read(fd1[0],buffer_sai,100);
			printf("%s\n",buffer_sai);
      system(buffer_sai);
      strcpy(buffer_ent,"Stream executando...para matar o processo pressione 0\n");
      write(fd[1],buffer_ent,100);
			read(fd1[0],buffer_sai,100);
			printf("%s",buffer_sai);
    }
    else{
      read(fd[0],buffer_sai,100);
			printf("%s\n",buffer_sai);
      system(buffer_sai);
      strcpy(buffer_ent,"dd if=/dev/video0 of=uv4image.jpeg bs=11M count=1");
			printf("Digite um numero:");
			scanf("%c",&ch);
			buffer_ent[16] = ch;
      write(fd1[1],buffer_ent,100);
      read(fd[0],buffer_sai,100);
      printf("%s",buffer_sai);
			printf("Para parar stream digite /sudo pkill uv4l\n");
			//scanf("%d",&j);
			//if(j = 0){
			//	strcpy(buffer_ent,"Operation is now over");
			//	write(fd1[1],buffer_ent,100);
			//}
    }
    return 0;
}
