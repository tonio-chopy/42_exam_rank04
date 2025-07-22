# ft_sandbox

# Import Libraries

```c
#include <stdbool.h> //bool, true, false
#include <unistd.h> //fork, alarm
#include <sys/wait.h> //wait, waitpid
#include <stdlib.h> //exit
#include <signal.h> //signal, sigaction, kill
#include <sys/types.h> //t_pid
#include <string.h> //strsignal
#include <errno.h> //EINTR
#include <stdio.h> //printf
```
# Set the handler function to sigaction

```c
void	alarm_handler(int sig)
{
	(void) sig;
	return ;
}
```

----------LEVES ESTUDOS PRA ENTENDER ESSA POMBA-----------------------------
## EINTR
é uma macro de erro que vem da lib #include <errno.h>, ela indica que uma chamada de sistema foi interrompida por um _sinal_ antes mesmo de terminar totalemtne


`waitpid` -> aguarda mudanças de estado do processo filho, ela retorna quando
- return
- exit
- sinal (SIGINT, SIGSEGV)
- stopado (WUNTRACED)
- continua (WCONTINUED)

quando o waitpid retorna _-1_ não é por causa de como o processo terminou, mas sim porque o próprio waitpid foi interrompido por um _sinal do processo filho_,

o waitpid utilizado no exercicio é o padrão então ele para o processo principal o processo pai

Se o processo filho retorna um sinal que faça que com o processo pai saia do waitpid antes do esperado automáticamente o waipid retorna o *valor -1* e o errno é setado para *EINTR*

Obs: não necessáriamente o processo precisa terminar com o sinal mas o usuário pode imputar um _Ctrl + C_(sigint) no meio de um sleep o que faz o waipid prematura

Algo comum de se fazer para _impedir_ que o usário pare algo é o seguinte código

```c
do
{
	res = waitpid(pid, &status, 0);
} while(res == -1 && errno == EINTR);
```
o loop só vai acabar quando o processo filho sair com sucesso ou retornar outro erro que não seja ser estopado

## SA_RESTART
é uma flag que faz um comportamento parecio com o ignorar sinais de parada feita pelo usuário como no exemplo de cima, porem o mesmo funciona mais low level e o programa nem percebe que foi solicitado ser parado

```c
int main(void)
{
	struct sigaction	sa;
	int					status;
	t_pid				pid;

	sa.sa_handler = handler_function;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaciotn(SIGUSR1, &sa, NULL);

	pid = fork();
	if (pid == 0)
	{
		sleep(10);
		exit(0);
	}

	waitpid (pid, &status, 0);
	// Se SA_RESTART estiver setado, este waitpid será reiniciado automaticamente
}
```

o _default_ sem nenhuma flag é o valor _0_

```c
struct sigaction sa;
sa.sa_mask = 0;
```

## sigemptyset
Alguns sinais podem ser executado durante a execução do handler de alguns sigaction, para setarmos quais devem ser ignorados ou se nenhum devem ser ignorado utilizamos _sigemptyset_ que limpa ou seta essa _mascara de sinais_ pertecente a aquele sigaction


_Quais sinais devem ser temporáriamente bloqueados enquanto o handler estiver executando_


## SIGUSR1 && SIGUSR2
São dois sinais reservados no sistema POSIX que não tem um utilidade em especifico eles podem ser definidos com N comportamentos a depender do nosso código

--------------------------------------------------------------

## Definir o sigaction

```c
void sighandler(int sig)
{
	(void)sig;
}

int main(void)
{
	struct sigaction sa;

	sa.sa_handler = sighandler;
	sa.flags = 0;
	sigemptyset(sa.sa_mask);
	sigaction(SIGALRM, &sa, NULL);
}
```

## Criar um subprocess e executar a função

```c
int main(void)
{
	int		pid;
	//.
	//.
	//. Previous code
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		f();// ptr de função que vem no escopo da ft_sandbox
		exit(0);
	}
}
```

## Alarm e loop
Momento onde devemos utilizar o valor setado no escopo da função para setar o alarm e entrar para dentro do loop do waitpid

```c
int main(void)
{
	//.
	//.
	//. previous code
	alarm(timeout);
	if (waitpid(pid, &status, 0) == -1)
	{
		//caso onde o processo filho mandou um sig para sair do waitpid
		if (errno == EINT)
		{
			kill (pid, SIGKILL); //matamos o subprocesso
			waitpid(pid, NULL, 0); //aguardo ele terminar
			if (verbose)
				printf ("Bad function: timed out after %d seconds\n", timeout);
			return (0); //retornarmos good pq foi parada intencionamento a função
		}
		return (-1); //caso onde não foi parada intencionalmente
	}
}
```

### Verificamos se terminou por saida
Agora devemos fazer a verificação se o subprocess que executou a função terminou com uma saida convencional como _return_ ou _exit_, e se sim devemos verificar se o valor de saida do processo é _0_ que significa okay;

```c
int main(void)
{
	int		pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		f();
		exit(0);
	}
	if (waitpid(pid, &status, 0) == -1)
	{
		//... faz as outras fita lá
	}

	if (WIFEXITED(status)) //saiu com return ou com exit
	{
		if (WEXITSTATIS(status) == 0) //sinal com um sinal positivo
		{
			if (verbose)
				printf ("Nice function!\n");
			return (1); // saida nice da parada
		}
	}
}

```
### Verficamos se terminou por sinal
Caso o caso de cima não entre então o subprocesso só pode ter saido com um sinal, ou seja a função não funcionou como deveria.

```c
int main(void)
{
	int		pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		f();
		exit(0);
	}

	if (WIFISIGNALED(status)) //processo saiu com um sinal
	{
		int		sig = WTERMSIG(status); //pegamos o indice de str dessa pomba
		if (verbose)
			printf ("Bad function: %s\n", strsignal(sig));
		return (0);
	}
	return (-1); //return que se tudo bugar é que deu erro
}
```
# Import Libraries
# Set the handler function to sigaction
## Definir o sigaction
## Criar um subprocess e executar a função
## Alarm e loop
### Verificamos se terminou por saida
### Verficamos se terminou por sinal

