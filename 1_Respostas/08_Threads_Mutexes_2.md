1. Crie um programa em C que cria uma thread, e faça com que o programa principal envie os valores 1, 2, 3, 4, 5, 6, 7, 8, 9 e 10 para a thread, com intervalos de 1 segundo entre cada envio. Depois de o programa principal enviar o número 10, ele aguarda 1 segundo e termina a execução. A thread escreve na tela cada valor recebido, e quando ela receber o valor 10, ela termina a execução.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/stat.h>

void* print_int(void* v)
{
	int *i = (int *) v;
	printf("%d\n", *i);
	return NULL;
}

int main(int argc, char const *argv[])
{
	pthread_t thr_1;
	int count = 0;
	while(count < 11)
	{
		pthread_create(&thr_1, NULL, &print_int, &count);
		pthread_join(thr_1, NULL);
		count++;
		sleep(1);
	}
	sleep(1);
	printf("FIM\n");
	return 0;
}
```

2. Crie um programa em C que preenche o vetor `long int v[50000]` completamente com valores aleatórios (use a função `random()`), e que procura o valor máximo do vetor por dois métodos:

(a) Pela busca completa no vetor `v[]`;

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/stat.h>

long int v[50000];

void* maxvalue(void* v)
{
	int *vetmax = (int *) v;
	long int max;
	int i;
	max = vetmax[0];
	for(i = 0; i < 50000; i++){
		if(vetmax[i]>max){
			max = vetmax[i];
		}
	}
	printf("Valor máximo = %ld\n",max);
	return NULL;
}

int main(int argc, char const *argv[])
{
	pthread_t thr_1;
	int count = 0;
	for(count = 0; count < 50000; count++){
			v[count] = random();
	}
	pthread_create(&thr_1, NULL, &maxvalue, &v);
	pthread_join(thr_1, NULL);
	sleep(1);
	printf("FIM\n");
	return 0;
}
```
(b) Separando o vetor em 4 partes, e usando 4 threads para cada uma encontrar o máximo de cada parte. Ao final das threads, o programa principal compara o resultado das quatro threads para definir o máximo do vetor.

```c
#define SIZE 50000
#define PART_SIZE 12500

void* calcula_max(void* v)
{
	long int *vp = (long int *) v;
	long int max = 0;
	for (int i = 0; i < PART_SIZE; ++i)
	{
		if(vp[i] > max)
			max = vp[i];
	}
	vp[0] = max; //Ret. vmax pela posicao inicial de cada vetor partido!
	return NULL;
}

int main(int argc, char const *argv[])
{
	long int v[SIZE];
	long int max = 0, max_thread = 0;
	pthread_t thr[4];
	for(long int i = 0; i < SIZE; i++)
		v[i] = random();
	for (int i = 0; i < SIZE; ++i)
		if(v[i] > max)
			max = v[i];
	for (int i = 0; i < 4; ++i)
		pthread_create(&thr[i], NULL, &calcula_max, &v[i*PART_SIZE]);
	for (int i = 0; i < 4; ++i)
		pthread_join(thr[i], NULL);
	for (int i = 0; i < 4; ++i)
		if(v[i*PART_SIZE] > max_thread)
			max_thread = v[i*PART_SIZE];
	max - max_thread == 0 ? printf("DEU BOM!\n") : printf("DEU RUIM\n");

	return 0;
}
```

Ao final do programa principal, compare os resultados obtidos pelos dois métodos.

> Os resultados obtidos foram iguais!

3. Repita o exercício anterior, mas calcule a média do vetor ao invés do valor máximo.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/stat.h>

float v[50000];
float media = 0;
void* somaf(void* v)
{
  float *vp = (float *) v;
  int count = 0;
  float soma = 0;
	for(count = 0; count < 50000; count++){
		   soma = vp[count] + soma;
		}
  media = soma/50000.0;
  printf("%f\n",media);
	return NULL;
}
int main(int argc, char const *argv[])
{
	pthread_t thr;
	int count = 0;
  float media;
	for(count = 0; count < 50000; count++){
			v[count] = random();
	}
  pthread_create(&thr, NULL, &somaf, &v);
  pthread_join(thr, NULL);
	return 0;
}
```
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/stat.h>

#define SIZE 50000
#define PART_SIZE 12500

void* calcula_soma(void* v)
{
	long int *vp = (long int *) v;
	long int med = 0;
	for (int i = 0; i < PART_SIZE; ++i)
		med += vp[i];
	vp[0] = med; //Ret. vmax pela posicao inicial de cada vetor partido!
	return NULL;
}

int main(int argc, char const *argv[])
{
	long int v[SIZE];
	long int med = 0, med_thread = 0;
	pthread_t thr[4];
	for(long int i = 0; i < SIZE; i++)
	{
		v[i] = random();
		v[i] = v[i]/SIZE; // Divide-se todos os elementos por SIZE
	}
	for (int i = 0; i < SIZE; ++i)
		med += v[i];	  // a media sera a soma
	for (int i = 0; i < 4; ++i)
		pthread_create(&thr[i], NULL, &calcula_soma, &v[i*PART_SIZE]);
	for (int i = 0; i < 4; ++i)
		pthread_join(thr[i], NULL);
	for (int i = 0; i < 4; ++i)
		med_thread += v[i*PART_SIZE];
	med == med_thread ? printf("DEU BOM\n") : printf("DEU RUIM\n");
	return 0;
}
```

> Os resultados obtidos são iguais aos resultados esperados usando os 2 métodos.

4. Repita o exercício anterior, mas calcule a variância do vetor ao invés da média.
