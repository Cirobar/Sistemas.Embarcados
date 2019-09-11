1. Crie um código em C para gerar três processos-filho usando o `fork()`.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
	pid_t childPid[3];
			
	childPid[0] = fork();
	if (childPid[0] == 0)
	{
		printf("Processo filho 1 com PID %d\n", (int) getpid());
		
	}
	else
	{
		childPid[1] = fork();
		if (childPid[1] == 0)
		{
			printf("Processo filho 2 com PID %d\n", (int) getpid());
			
		}
		else
		{
			childPid[2] = fork();
			if (childPid[2] == 0)
				printf("Processo filho 3 com PID %d\n", (int) getpid());
				
		}
	}
	return 0;
}
```

2. Crie um código em C que recebe o nome de diversos comandos pelos argumentos de entrada (`argc` e `*argv[]`), e executa cada um sequencialmente usando `system()`. Por exemplo, considerando que o código criado recebeu o nome de 'serial_system', e que ele foi executado na pasta '/Sistemas_Embarcados/Code/06_Processos', que contem diversos arquivos:

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
		int i = 1;
		int ret;
		while(i < argc){
		 ret = system(argv[i]);
		 sleep(2);
		 i++;
		}

		return 0;
}
```

```bash
$ ./serial_system pwd echo ls echo cal
$ ~/Sistemas_Embarcados/Code/06_Processos
$
$ Ex1.c    Ex2b.c   Ex4.c   Ex6.c
$ Ex2a.c   Ex3.c    Ex5.c   serial_system
$
$     Março 2017
$ Do Se Te Qu Qu Se Sá
$           1  2  3  4
$  5  6  7  8  9 10 11
$ 12 13 14 15 16 17 18
$ 19 20 21 22 23 24 25
$ 26 27 28 29 30 31
```

3. Crie um código em C que recebe o nome de diversos comandos pelos argumentos de entrada (`argc` e `*argv[]`), e executa cada um usando `fork()` e `exec()`.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
	pid_t childPid[argc];

	for (int i = 0; i < argc-1; ++i)
	{
		childPid[i] = fork();
		if (childPid[i] == 0)
		{
			char *args[2] = {(char*) argv[i+1],NULL};
			execvp(args[0],args); // Permite que o nome ou a procura do programa esteja
			//no caminho atual e que a lista de argumentos do novo programa seja nula
			printf("ERRO\n");
		}
		else
			sleep(2);
			printf("*************************\n");
			printf("* Este é o processo PAI *\n");
			printf("*************************\n");
			printf("\n");
	}
	return 0;
}

```

4. Crie um código em C que gera três processos-filho usando o `fork()`, e que cada processo-filho chama a seguinte função uma vez:

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

int v_global = 0; // Variavel global para este exemplo
void Incrementa_Variavel_Global(pid_t id_atual);

int main(int argc, char const *argv[])
{
	pid_t childPid[3];
			
	childPid[0] = fork();
	if (childPid[0] == 0)
	{
		Incrementa_Variavel_Global(getpid());
	}
	else
	{
		childPid[1] = fork();
		if (childPid[1] == 0)
		{
			Incrementa_Variavel_Global(getpid());	
		}
		else
		{
			childPid[2] = fork();
			if (childPid[2] == 0)
				Incrementa_Variavel_Global(getpid());
				
		}
	}
	return 0;
}

```

```C
int v_global = 0; // Variavel global para este exemplo
void Incrementa_Variavel_Global(pid_t id_atual)
{
	v_global++;
	printf("ID do processo que executou esta funcao = %d\n", id_atual);
	printf("v_global = %d\n", v_global);
}
```

(Repare que a função `Incrementa_Variavel_Global()` recebe como entrada o ID do processo que a chamou.) Responda: a variável global `v_global` foi compartilhada por todos os processos-filho, ou cada processo enxergou um valor diferente para esta variável?

> Cada processo enxergou um valor diferente para a variável `v_global`.

5. Repita a questão anterior, mas desta vez faça com que o processo-pai também chame a função `Incrementa_Variavel_Global()`. Responda: a variável global `v_global` foi compartilhada por todos os processos-filho e o processo-pai, ou cada processo enxergou um valor diferente para esta variável?

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

int v_global = 0; // Variavel global para este exemplo
void Incrementa_Variavel_Global(pid_t id_atual);

int main(int argc, char const *argv[])
{
	Incrementa_Variavel_Global(getpid());
	pid_t childPid[3];
	childPid[0] = fork();
	if (childPid[0] == 0)
	{
		Incrementa_Variavel_Global(getpid());
	}
	else
	{
		Incrementa_Variavel_Global(getpid());
		childPid[1] = fork();
		if (childPid[1] == 0)
		{
			Incrementa_Variavel_Global(getpid());	
		}
		else
		{
			Incrementa_Variavel_Global(getpid());
			childPid[2] = fork();
			if (childPid[2] == 0)
				Incrementa_Variavel_Global(getpid());
		}
	}
	return 0;
}

void Incrementa_Variavel_Global(pid_t id_atual)
{
	v_global++;
	printf("ID do processo que executou esta funcao = %d\n", id_atual);
	printf("v_global = %d\n", v_global);
}
```

> Cada processo foi capaz de operar com o valor herdado do processo que foi bifurcado.

