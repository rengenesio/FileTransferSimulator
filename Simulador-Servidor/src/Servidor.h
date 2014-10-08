#ifndef SERVIDOR_H_
#define SERVIDOR_H_


#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <queue>
#include <vector>
#include <signal.h>


#include "../../Defines.h"
#include "Mensagem.h"
#include "ModuloAtraso.h"
#include "Timer.h"


typedef struct PARAMETROS_THREAD_ {
	unsigned int * estado;
	unsigned long int * base, * nextseqnum, * expectedseqnum;
	Timer * timer;

	int * socket;
	struct sockaddr_in * socketaddr;
	vector<Mensagem> * buffer_entrada, * buffer_saida;
	pthread_mutex_t * mutex_buffer_saida;
	pthread_mutex_t * mutex_timer;
} PARAMETROS_THREAD;


class Servidor {
public:
	Servidor();
	virtual ~Servidor();
	void inicializaVariaveis();
	void inicia(void);
	static void * threadEnvia(void * parametros_out);
	static void * threadRecebe(void * parametros_in);

private:
	int iniciaSocketEntrada(unsigned int porta);
	int iniciaSocketSaida(char endereco[], unsigned int porta);

	unsigned int estado;
	unsigned long int base, nextseqnum, expectedseqnum;
	Timer timer;

	int socketin, socketout;
	struct sockaddr_in socketaddrin, socketaddrout;

	pthread_t thread_envia, thread_recebe;
	PARAMETROS_THREAD parametros_out, parametros_in;

	//queue<Mensagem> * buffer_entrada, * buffer_saida;
	vector<Mensagem> * buffer_saida;
	pthread_mutex_t mutex_buffer_saida;
	pthread_mutex_t mutex_timer;
};


#endif /* SERVIDOR_H_ */
