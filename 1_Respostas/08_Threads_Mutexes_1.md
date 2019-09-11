1. Quais são as vantagens e desvantagens em utilizar:

(a) fork?

> É possível gerar processos-filhos e cada um executa sua tarefa. Cada filho mantém seu espaço de memória protegido. Comunicamos com pipe, sinais, etc. No entanto, a troca de contexto é custosa.

(b) threads?

> Uma aplicação pode rodar várias threads. Entre as threads há o compartilhamento de memória o que facilita a comunicação. Portanto há o risco de corrupção de dados. Finalmente, a troca de contexto é pouco custosa.

2. Quantas threads serão criadas após as linhas de código a seguir? Quantas coexistirão? Por quê?

(a)

```C
void* funcao_thread_1(void *arg);
void* funcao_thread_2(void *arg);

int main (int argc, char** argv)
{
	pthread_t t1, t2;
	pthread_create(&t1, NULL, funcao_thread_1, NULL);
	pthread_create(&t2, NULL, funcao_thread_2, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	return 0;
}
```

> Serão criadas 2 threads e ambas coexistirão.

(b)
```C
void* funcao_thread_1(void *arg);
void* funcao_thread_2(void *arg);

int main (int argc, char** argv)
{
	pthread_t t1, t2;
	pthread_create(&t1, NULL, funcao_thread_1, NULL);
	pthread_join(t1, NULL);
	pthread_create(&t2, NULL, funcao_thread_2, NULL);
	pthread_join(t2, NULL);
	return 0;
}
```

> Serão criadas 2 threads e serão executadas uma após a outra.

3. Apresente as características e utilidades das seguintes funções:

(a) `pthread_setcancelstate()`

> Com o seguinte protótipo `int pthread_setcancelstate(int state, int *oldstate);` a função permite setar a cancelabilidade de uma thread com o `state`: `PTHREAD_CANCEL_ENABLE` ou `PTHREAD_CANCEL_DISABLE`.

(b) `pthread_setcanceltype()`

> Com o seguinte protótipo `int pthread_setcanceltype(int type, int *oldtype);` a função permite setar o tipo de cancelabilidade de uma thread de maneira assincrona `PTHREAD_CANCEL_ASYNCHRONOUS` (cancelavel a qualquer momento) ou `PTHREAD_CANCEL_DEFERRED
 ` que é o tipo de cancelamento padrão para novas threads de maneira que somente é cancelada antes de sua próxima chamada.

(DICA: elas são relacionadas à função pthread_cancel().)