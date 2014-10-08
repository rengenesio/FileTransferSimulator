#ifndef CLIENTE_H_
#define CLIENTE_H_

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <queue>
#include <signal.h>
using namespace std;

#include "ModuloAtraso.h"
#include "Mensagem.h"
#include "../../Defines.h"

typedef struct PARAMETROS_THREAD_ {
	unsigned int * estado;
	unsigned long int * base, * nextseqnum, * expectedseqnum;

	int * socket;
	struct sockaddr_in * socketaddr;
	queue<Mensagem> * buffer_entrada, * buffer_saida;
	pthread_mutex_t * mutex_buffer_entrada, * mutex_buffer_saida;

	ModuloAtraso * modulo_atraso;
} PARAMETROS_THREAD;

class Cliente {

public:
	Cliente();
	virtual ~Cliente();

	void inicializaVariaveis();
	void principal();
	static void * threadEnvia(void * parametros_out);
	static void * threadRecebe(void * parametros_in);

private:
	int iniciaSocketEntrada(unsigned int porta);
	int iniciaSocketSaida(char endereco[], unsigned int porta);

	unsigned int estado;
	unsigned long int base, nextseqnum, expectedseqnum;

	int socketin, socketout;
	struct sockaddr_in socketaddrin, socketaddrout;

	pthread_t thread_envia, thread_recebe;
	PARAMETROS_THREAD parametros_out, parametros_in;

	queue<Mensagem> * buffer_entrada, * buffer_saida;
	pthread_mutex_t mutex_buffer_saida;

	ModuloAtraso modulo_atraso;
};


#endif /* CLIENTE_H_ */
