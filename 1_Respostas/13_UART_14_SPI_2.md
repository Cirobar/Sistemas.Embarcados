1. Considere um MSP430 sendo usado para leituras analógicas. O Raspberry Pi está conectado a ele via UART. O MSP430 foi programado para converter e enviar dados de 10 bits a cada 10 ms. Escreva o código para o Raspberry Pi receber estes dados, e cada 1 segundo apresentar no terminal a média das últimas 100 amostras.

```C
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define TTY "/dev/ttyS0"
#define N 100

int uart0_fd;
void ctrl_c(int sig)
{
	puts(" Fechando " TTY "...");
	close(uart0_fd);
	exit(-1);
}

int main(int argc, char const *argv[])
{
	struct termios options;
	int counter = 0;
	long int sum = 0;
	char val;

	signal(SIGINT, ctrl_c);
	uart0_fd = open(TTY, O_RDWR); // | O_NOCTTY); // | O_NDELAY);
	if(uart0_fd==-1)
	{
		puts("Erro abrindo a UART.");
		return -1;
	}
	tcgetattr(uart0_fd, &options);
	options.c_cflag = CS8 | CREAD | CLOCAL;
	options.c_iflag = 0;
	options.c_oflag = 0;
	options.c_lflag = 0;
	options.c_cc[VTIME] = 0;
	options.c_cc[VMIN]  = 1;
	cfsetospeed(&options, B9600);
	cfsetispeed(&options, B9600);
	tcflush(uart0_fd, TCIOFLUSH);
	tcsetattr(uart0_fd, TCSANOW, &options);
	// puts("UART configurada:");
	// system("stty -F " TTY);

	while(1)
	{
		
		if(read(uart0_fd, &val, 1) == 1)
		{
			sum += (long int) val;
			if(read(uart0_fd, &val, 1) == 1)
			{
				sum += (long int) (val<<8);
				counter ++;
			}
		}
		
		if (counter == N)
		{
			printf("Média das %d amostras:  %ld\n", N, sum/N);
			counter = 0;
			sum = 0;
		}
	}
	return 0;
}
```

2. Considere um MSP430 sendo usado para leituras analógicas. O Raspberry Pi está conectado a ele via SPI, e é o mestre. O MSP430 foi programado para funcionar da seguinte forma:

- O MSP430 recebe o byte `0x55` e envia o byte `0xAA`, o que indica o começo de conversão. 
- 100us depois, o MSP430 recebe os bytes `0x01` e `0x02`, e envia o byte menos significativo e o mais significativo da conversão de 10 bits, nesta ordem.
 
Escreva o código para o Raspberry Pi executar este protocolo, de forma a obter conversões a cada 10 ms. A cada 1 segundo ele deve apresentar no terminal a média das últimas 100 amostras.

```C
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define N 100
#define DLY 10e2

int spi_fd;

// void tratamento_alarme(int sig)
// {
// 	alarm(1);
	
// }

void ctrl_c(int sig)
{
	close(spi_fd);
	exit(-1);
}

int main(int argc, char const *argv[])
{
	int counter = 0;
	long int sum = 0;
	unsigned char send = 0x55;

	// signal(SIGALRM, tratamento_alarme);
	signal(SIGINT, ctrl_c);

	wiringPiSetup();
	spi_fd = wiringPiSPISetup(0, 500000);

	while(1)
	{
		usleep(DLY);
		do
		{
			wiringPiSPIDataRW(0, &send, 1);

		}while(send != 0xAA);
		usleep(DLY);
		send = 0x01;
		wiringPiSPIDataRW(0, &send, 1);
		sum += (long int) send;
		send = 0x02;
		wiringPiSPIDataRW(0, &send, 1);
		sum += ((long int) send) << 8;

		if (counter == N)
		{
			printf("Média das %d amostras:  %ld\n", N, sum/N);
			counter = 0;
			sum = 0;
		}
	}
	return 0;
}
```
