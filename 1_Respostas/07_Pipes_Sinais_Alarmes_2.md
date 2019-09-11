1. Crie um programa em C que cria um processo-filho e um pipe de comunicação. Faça com que o processo-pai envie os valores 1, 2, 3, 4, 5, 6, 7, 8, 9 e 10 para o processo-filho, com intervalos de 1 segundo entre cada envio. Depois de o processo-pai enviar o número 10, ele aguarda 1 segundo e termina a execução. O processo-filho escreve na tela cada valor recebido, e quando ele receber o valor 10, ele termina a execução.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
	pid_t childPid;
	int fd[2];
	pipe(fd);

	childPid = fork();
	if (childPid == 0)
	{
		//Child Process Code
		int iRead;
		while(1)
		{
			read(fd[0], &iRead, sizeof(int));
			printf("Processo %d leu %d\n", getpid(),iRead);
			if (iRead == 10)
			{
				sleep(1);
				printf("Prcesso %d disse FIM!\n",getpid());
				exit(0);
			}
		}
	}
	else
	{
		//Parent Process Code
		for (int i = 0; i < 11; ++i)
		{
			printf("Processo %d escreve %d\n", getpid(),i);
			write(fd[1], &i, sizeof(int));
			sleep(1);
		}
	}
	return 0;
}
```

2. Crie um programa em C que cria um processo-filho e um pipe de comunicação. Utilize o pipe para executar a seguinte conversa entre processos:

```
FILHO: Pai, qual é a verdadeira essência da sabedoria?
PAI: Não façais nada violento, praticai somente aquilo que é justo e equilibrado.
FILHO: Mas até uma criança de três anos sabe disso!
PAI: Sim, mas é uma coisa difícil de ser praticada até mesmo por um velho como eu...
```

Neste exercício, quem recebe a mensagem via pipe é quem as escreve no terminal.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
	pid_t childPid;
	int fd[2];
	pipe(fd);

	childPid = fork();
	if (childPid == 0)
	{
		//Child Process Code
		int k = 0;
		char buffChild[90];
		char *dialogChild[]={"Morty: Pai, qual é a verdadeira essência da sabedoria?",
							 "Morty: Mas até uma criança de três anos sabe disso!"};
		for (int i = 0; i < 2; ++i)
		{
			if(k < 2)
				write(fd[1], strcpy(buffChild,dialogChild[k]), sizeof(char)*90);
			sleep(1);
			read(fd[0], &buffChild, sizeof(char)*90);
			printf("%d- %s\n", getpid(), buffChild);
			k++;
		}
		exit(0);
	}
	else
	{
		//Parent Process Code
		int j = 0;
		char buffParent[90];
		char *dialogParent[]={"Jerry: Não façais nada violento, praticai somente aquilo que é justo e equilibrado.",
							  "Jerry: Sim, mas é uma coisa difícil de ser praticada até mesmo por um velho como eu..."};				
		for (int i = 0; i < 2; ++i)
		{
			read(fd[0], &buffParent, sizeof(char)*90);
			printf("%d- %s\n", getpid(), buffParent);
			if(j < 2)
				write(fd[1], strcpy(buffParent,dialogParent[j]), sizeof(char)*90);
			sleep(1);
			j++;
		}
		wait(NULL);						  
	}
	return 0;
}
```

3. Crie um programa em C que cria dois processos-filhos e um pipe de comunicação. Utilize o pipe para executar a seguinte conversa entre processos:

```
FILHO1: Quando o vento passa, é a bandeira que se move.
FILHO2: Não, é o vento que se move.
PAI: Os dois se enganam. É a mente dos senhores que se move.
```

Neste exercício, quem recebe a mensagem via pipe é quem as escreve no terminal.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
	pid_t childPid[2];
	int fd[2];
	pipe(fd);

	childPid[0] = fork();
	if (childPid[0] == 0)
	{
		//Child1 Process Code
		char buffChild1[65];
		char msgChild1[] = "FILHO1: Quando o vento passa, é a bandeira que se move.";
		write(fd[1], strcpy(buffChild1, msgChild1), 65);
		exit(0);
	}
	else
	{
		int fd2[2];
		pipe(fd2);
		childPid[1] = fork();
		if (childPid[1] == 0)
		{
			//Child2 Process Code
			char buffChild2[65];
			char msgChild2[] = "FILHO2: Não, é o vento que se move.";
			write(fd2[1], strcpy(buffChild2, msgChild2), 65);
			exit(0);
		}
		else
		{
			//Parent Process Code
			char buffParent[65];
			read(fd[0], &buffParent, 65);
			printf("%s\n", buffParent);
			read(fd2[0], &buffParent, 65);
			printf("%s\n", buffParent);
			printf("PAI: Os dois se enganam. É a mente dos senhores que se move.");
			wait(NULL);
		}
	}
	return 0;
}
```

4. Crie um programa em C que cria um processo-filho e um pipe de comunicação. O processo-filho deverá pedir o nome do usuário, envia-lo para o pai via pipe, e o pai deverá escrever o nome do usuário no terminal.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
	pid_t childPid;
	int fd[2];

	pipe(fd);
	childPid = fork();
	if (childPid == 0)
	{
		//Child Process Code
		char username[50];
		printf("PID %d - Digite seu nome de usuario: ", getpid());
		fgets(username, 50, stdin);
		write(fd[1], username, 50);
	}
	else
	{
		//Parent Process Code
		char buffParent[50];
		read(fd[0], buffParent, 50);
		printf("PID %d - Nome de usuario e: %s\n", getpid(), buffParent);
		wait(NULL);
	}
	return 0;
}
```

5. Utilize o sinal de alarme para chamar a cada segundo o comando `ps` ordenando todos os processos de acordo com o uso da CPU. Ou seja, seu código atualizará a lista de processos a cada segundo. Além disso, o código deverá tratar o sinal do CTRL-C, escrevendo "Processo terminado!" na tela antes de terminar a execução do processo.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

void tratamento_alarme(int sig)
{
	alarm(1);
	system("ps --sort=-pcpu");
}

void acaba_prog(int sig)
{
	printf("Processo Terminado\n");
	exit(0);
}

int main()
{
	signal(SIGALRM, tratamento_alarme);
	signal(SIGINT, acaba_prog);
	alarm(1);
	printf("Aperte CTRL+C para acabar:\n");
	while(1);
	return 0;
}
```