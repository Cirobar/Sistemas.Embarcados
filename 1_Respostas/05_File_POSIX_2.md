Para todas as questões, utilize as funções da norma POSIX (`open()`, `close()`, `write()`, `read()` e `lseek()`). Compile os códigos com o gcc e execute-os via terminal.

1. Crie um código em C para escrever "Ola mundo!" em um arquivo chamado 'ola_mundo.txt'.

```c
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
	int fd,rec;
	fd = open("ola_mundo.txt", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR); // r/w | create file, chmod to r/w 
	if (fd < 0)
	{
		printf("Erro ao abrir arquivo!\n");
		return 0;
	}
	char data[] = "Ola Mundo!\n";
	if (strlen(data) == write(fd, data, sizeof(strlen(data))))
	{
		printf("Erro ao gravar no arquivo!\n");
	}
	close(fd);
	return 0;
}
```

2. Crie um código em C que pergunta ao usuário seu nome e sua idade, e escreve este conteúdo em um arquivo com o seu nome e extensão '.txt'. Por exemplo, considerando que o código criado recebeu o nome de 'ola_usuario_1':

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
	char name[31], namecpy[36], idade[4], str1[]="Nome: ", str2[]="\nIdade: ";
	int fd;

	printf("Digite seu nome: \n");
	fgets(name,31,stdin);
	printf("Digite sua idade: \n");
	fgets(idade,4,stdin);
	name[strcspn(name,"\n")] = 0;
	strcpy(namecpy,name);
	fd = open(strcat(namecpy,".txt"), O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
	if (fd < 0)
	{
		printf("Erro ao abrir arquivo!\n");
		return 0;
	}
	write(fd, str1, sizeof(str1));
	write(fd, name, sizeof(name));
	write(fd, str2, sizeof(str2));
	write(fd, idade, sizeof(idade));
	close(fd);
	return 0;
}
```

```bash
$ ./ola_usuario_1
$ Digite o seu nome: Eu
$ Digite a sua idade: 30
$ cat Eu.txt
$ Nome: Eu
$ Idade: 30 anos
```

3. Crie um código em C que recebe o nome do usuário e e sua idade como argumentos de entrada (usando as variáveis `argc` e `*argv[]`), e escreve este conteúdo em um arquivo com o seu nome e extensão '.txt'. Por exemplo, considerando que o código criado recebeu o nome de 'ola_usuario_2':

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
	char *fileName, *str1;
	int fd, writeData;

	if (argc != 3)
	{
		printf("Insira Nome e Idade como argumentos\n");
		return 0;
	}
	fileName = malloc(sizeof(strlen(argv[1]+5)));
	strcpy(fileName,argv[1]);
	strcat(fileName,".txt");
	fd = open(fileName, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
	if (fd < 0)
	{
		printf("Erro ao abrir arquivo!\n");
		return 0;
	}
	
	str1 = malloc(sizeof(strlen(argv[1])+strlen(argv[2])+15));
	strcat(str1,"Nome: ");
	strcat(str1,argv[1]);
	strcat(str1,"\nIdade: ");
	strcat(str1,argv[2]);
	strcat(str1,"\n");
	if(strlen(str1) != write(fd, str1,strlen(str1)))
	{
		printf("Erro ao gravar no arquivo\n");
		return 0;
	}
	close(fd);
	free(fileName);
	return 0;
}
```

```bash
$ ./ola_usuario_2 Eu 30
$ cat Eu.txt
$ Nome: Eu
$ Idade: 30 anos
```

4. Crie uma função que retorna o tamanho de um arquivo, usando o seguinte protótipo: `int tam_arq_texto(char *nome_arquivo);` Salve esta função em um arquivo separado chamado 'bib_arqs.c'. Salve o protótipo em um arquivo chamado 'bib_arqs.h'. Compile 'bib_arqs.c' para gerar o objeto 'bib_arqs.o'.

```bash
$ gcc -c bib_arqs.c
```

```c
int tam_arq_texto(char *nome_arquivo)
{
	int fd;
	int fileLength = 0;
	char ch;
	fd = open(nome_arquivo, O_RDONLY);
	if(fd < 0)
	{
		printf("Erro ao abrir arquivo!\n");
		return 0;
	}
	while(read(fd,&ch,1) > 0)
		fileLength++;
	close(fd);
	return fileLength;
}
```

5. Crie uma função que lê o conteúdo de um arquivo-texto e o guarda em uma string, usando o seguinte protótipo: `char* le_arq_texto(char *nome_arquivo);` Repare que o conteúdo do arquivo é armazenado em um vetor interno à função, e o endereço do vetor é retornado ao final. (Se você alocar este vetor dinamicamente, lembre-se de liberar a memória dele quando acabar o seu uso.) Salve esta função no mesmo arquivo da questão 4, chamado 'bib_arqs.c'. Salve o protótipo no arquivo 'bib_arqs.h'. Compile 'bib_arqs.c' novamente para gerar o objeto 'bib_arqs.o'.

```c
char* le_arq_texto(char *nome_arquivo)
{
	int fd, fileLength = tam_arq_texto(nome_arquivo);
	char * data;

	fd = open(nome_arquivo, O_RDONLY);
	if(fd < 0)
	{
		printf("Erro ao abrir arquivo!\n");
		return 0;
	}
	data = malloc(sizeof(char) * fileLength);
	if (read(fd, data, fileLength) < 0)
	{
		printf("Erro ao ler arquivo!\n");
		return 0;
	}
	free(data);
	return data;
}
```

6. Crie um código em C que copia a funcionalidade básica do comando `cat`: escrever o conteúdo de um arquivo-texto no terminal. Reaproveite as funções já criadas nas questões anteriores. Por exemplo, considerando que o código criado recebeu o nome de 'cat_falsificado':

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bib_arqs.h"

int main(int argc, char const *argv[])
{

	printf("%s\n", le_arq_texto(argv[1]));
	return 0;
}
```

```bash
$ echo Ola mundo cruel! Ola universo ingrato! > ola.txt
$ ./cat_falsificado ola.txt
$ Ola mundo cruel! Ola universo ingrato!
```

7. Crie um código em C que conta a ocorrência de uma palavra-chave em um arquivo-texto, e escreve o resultado no terminal. Reaproveite as funções já criadas nas questões anteriores. Por exemplo, considerando que o código criado recebeu o nome de 'busca_e_conta':

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bib_arqs.h"

int main(int argc, char const *argv[])
{
	char *str = le_arq_texto((char *) argv[2]);
	char *srch = (char *) argv[1];
	int len = strlen(srch);
	int count = 0;
	char *where = str;

	if (len)
    while ((where = strstr(where, srch))) {
        where += len;
        count++;
    }
    printf("'%s' ocorre %d vezes no arquivo %s\n", argv[1],count,argv[2]);
	return 0;
}
```

```bash
$ echo Ola mundo cruel! Ola universo ingrato! > ola.txt
$ ./busca_e_conta Ola ola.txt
$ 'Ola' ocorre 2 vezes no arquivo 'ola.txt'.
```


