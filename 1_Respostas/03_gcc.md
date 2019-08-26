Para todas as questões, compile-as com o gcc e execute-as via terminal.

1. Crie um "Olá mundo!" em C.

```c
#include <stdio.h>

int main(int argc, char const *argv[])
{
	printf("Ola Mundo!\n");
	return 0;
}
```
2. Crie um código em C que pergunta ao usuário o seu nome, e imprime no terminal "Ola " e o nome do usuário. Por exemplo, considerando que o código criado recebeu o nome de 'ola_usuario_1':

```bash
$ ./ola_usuario_1
$ Digite o seu nome: Eu
$ Ola Eu
```

```c
#include <stdio.h>

int main(int argc, char const *argv[])
{
	char name[31];

	printf("Digite seu nome: ");
	fgets(name,31,stdin);
	printf("Ola %s\n", name);
	return 0;
}
```

3. Apresente os comportamentos do código anterior nos seguintes casos:

(a) Se o usuário insere mais de um nome.
```bash
$ ./ola_usuario_1
$ Digite o seu nome: Eu Mesmo
$ Ola Eu Mesmo
```

(b) Se o usuário insere mais de um nome entre aspas duplas. Por exemplo:
```bash
$ ./ola_usuario_1
$ Digite o seu nome: "Eu Mesmo"
$ Ola "Eu Mesmo"
```

(c) Se é usado um pipe. Por exemplo:
```bash
$ echo Eu | ./ola_usuario_1
$ Ola Eu
```

(d) Se é usado um pipe com mais de um nome. Por exemplo:
```bash
$ echo Eu Mesmo | ./ola_usuario_1
$ Ola Eu Mesmo
```

(e) Se é usado um pipe com mais de um nome entre aspas duplas. Por exemplo:
```bash
$ echo "Eu Mesmo" | ./ola_usuario_1
$ Ola "Eu Mesmo"
```

(f) Se é usado o redirecionamento de arquivo. Por exemplo:
```bash
$ echo Ola mundo cruel! > ola.txt
$ ./ola_usuario_1 < ola.txt
$ Ola Ola mundo cruel!
```

4. Crie um código em C que recebe o nome do usuário como um argumento de entrada (usando as variáveis argc e *argv[]), e imprime no terminal "Ola " e o nome do usuário. Por exemplo, considerando que o código criado recebeu o nome de 'ola_usuario_2':

```bash
$ ./ola_usuario_2 Eu
$ Ola Eu
```

```c
#include <stdio.h>

int main(int argc, char const *argv[])
{
	printf("Ola %s\n",argv[1]);
	return 0;
}
```

5. Apresente os comportamentos do código anterior nos seguintes casos:

(a) Se o usuário insere mais de um nome.
```bash
$ ./ola_usuario_2 Eu Mesmo
$ Ola Eu
```

(b) Se o usuário insere mais de um nome entre aspas duplas. Por exemplo:
```bash
$ ./ola_usuario_2 "Eu Mesmo"
$ Ola "Eu Mesmo"
```

(c) Se é usado um pipe. Por exemplo:
```bash
$ echo Eu | ./ola_usuario_2
$ Ola (null)
```

(d) Se é usado um pipe com mais de um nome. Por exemplo:
```bash
$ echo Eu Mesmo | ./ola_usuario_2
$ Ola (null)
```

(e) Se é usado um pipe com mais de um nome entre aspas duplas. Por exemplo:
```bash
$ echo "Eu Mesmo" | ./ola_usuario_2
$ Ola (null)
```

(f) Se é usado o redirecionamento de arquivo. Por exemplo:
```bash
$ echo Ola mundo cruel! > ola.txt
$ ./ola_usuario_2 < ola.txt
$ Ola (null)
```

6. Crie um código em C que faz o mesmo que o código da questão 4, e em seguida imprime no terminal quantos valores de entrada foram fornecidos pelo usuário. Por exemplo, considerando que o código criado recebeu o nome de 'ola_usuario_3':

```bash
$ ./ola_usuario_3 Eu
$ Ola Eu
$ Numero de entradas = 2
```

```c
#include <stdio.h>

int main(int argc, char const *argv[])
{
	printf("Ola %s\n",argv[1]);
	printf("Numero de entradas = %d\n", argc);
	return 0;
}
```


7. Crie um código em C que imprime todos os argumentos de entrada fornecidos pelo usuário. Por exemplo, considerando que o código criado recebeu o nome de 'ola_argumentos':

```bash
$ ./ola_argumentos Eu Mesmo e Minha Pessoa
$ Argumentos: Eu Mesmo e Minha Pessoa
```

```c
#include <stdio.h>

int main(int argc, char const *argv[])
{
	int i;
	printf("Argumentos: ");
	for(i = 0; i < argc-1; i++)
		printf("%s ",argv[i+1]);
	printf("\n");
	return 0;
}
```


8. Crie uma função que retorna a quantidade de caracteres em uma string, usando o seguinte protótipo:
`int Num_Caracs(char *string);` Salve-a em um arquivo separado chamado 'num_caracs.c'. Salve o protótipo em um arquivo chamado 'num_caracs.h'. Compile 'num_caracs.c' para gerar o objeto 'num_caracs.o'.


```c
int Num_Caracs(char *string)
{
	return strlen(string);
}
```

```bash
$ gcc -c num_caracs.c
```

9. Re-utilize o objeto criado na questão 8 para criar um código que imprime cada argumento de entrada e a quantidade de caracteres de cada um desses argumentos. Por exemplo, considerando que o código criado recebeu o nome de 'ola_num_caracs_1':

```bash
$ ./ola_num_caracs_1 Eu Mesmo
$ Argumento: ./ola_num_caracs_1 / Numero de caracteres: 18
$ Argumento: Eu / Numero de caracteres: 2
$ Argumento: Mesmo / Numero de caracteres: 5
```

```bash
$ gcc -c ola_num_caracs_1.c
$ gcc -o ola_num_caracs_1 ola_num_caracs_1.o num_caracs.o
```

10. Crie um Makefile para a questão anterior.

```bash
ola_num_caracs: ola_num_caracs_1.o num_caracs.o
	gcc $(CFLAGS) -o ola_num_caracs ola_num_caracs_1.o num_caracs.o
ola_num_caracs_1.o: num_caracs.h
	gcc $(CFLAGS) -c ola_num_caracs_1.c
num_caracs.o: num_caracs.c num_caracs.h
	gcc $(CFLAGS) -c num_caracs.c
clean:
	rm *.o
remove:
	rm ola_num_caracs
```

11. Re-utilize o objeto criado na questão 8 para criar um código que imprime o total de caracteres nos argumentos de entrada. Por exemplo, considerando que o código criado recebeu o nome de 'ola_num_caracs_2':

```bash
$ ./ola_num_caracs_2 Eu Mesmo
$ Total de caracteres de entrada: 25
```

```c
#include <stdio.h>

int main(int argc, char const *argv[])
{
	int i, lenTotal = 0;
	printf("Argumentos: ");
	for(i = 0; i < argc-1; i++)
		lenTotal += Num_Caracs(argv[i]);
	printf("Total de caracteres de entrada: %d \n",lenTotal);
	return 0;
}
```

12. Crie um Makefile para a questão anterior.

```bash
ola_num_caracs: ola_num_caracs_2.o num_caracs.o
	gcc $(CFLAGS) -o ola_num_caracs ola_num_caracs_2.o num_caracs.o
ola_num_caracs_2.o: num_caracs.h
	gcc $(CFLAGS) -c ola_num_caracs_2.c
num_caracs.o: num_caracs.c num_caracs.h
	gcc $(CFLAGS) -c num_caracs.c
clean:
	rm *.o
remove:
	rm ola_num_caracs
```


