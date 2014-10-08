#include "Servidor.h"

Servidor::Servidor() {
	fprintf(stderr, "Inicializando o servidor...\n");

	inicializaVariaveis();

	unsigned int porta = PORTA_PADRAO;
	fprintf(stderr, "Inicializando socket de entrada na porta %d... ", porta);
	if(!iniciaSocketEntrada(porta)) {
		fprintf(stderr, "erro... abortando programa!\n");
		return;
	}
	fprintf(stderr, "sucesso!\n");

	fprintf(stderr, "Servidor inicializado com sucesso!\n");

	inicia();
}

Servidor::~Servidor() {
}

void Servidor::inicializaVariaveis() {
	memset(&socketaddrout, 0, sizeof(socketaddrout));

	//buffer_entrada = new queue<Mensagem>();
	//buffer_saida = new queue<Mensagem>();
	buffer_saida = new vector<Mensagem>();

	estado = 0;
	base = 1;
	nextseqnum = 1;
	expectedseqnum = 1;

	pthread_mutex_init(&mutex_buffer_saida, NULL);
	pthread_mutex_init(&mutex_timer, NULL);

	parametros_out.estado = &estado;
	parametros_out.base = &base;
	parametros_out.nextseqnum = &nextseqnum;
	parametros_out.expectedseqnum = &expectedseqnum;
	parametros_out.timer = &timer;
	parametros_out.socket = &socketout;
	parametros_out.socketaddr = &socketaddrout;
	//parametros_out.buffer_entrada = buffer_entrada;
	parametros_out.buffer_saida = buffer_saida;
	parametros_out.mutex_buffer_saida = &mutex_buffer_saida;
	parametros_out.mutex_timer = &mutex_timer;

	parametros_in.estado = &estado;
	parametros_in.base = &base;
	parametros_in.nextseqnum = &nextseqnum;
	parametros_in.expectedseqnum = &expectedseqnum;
	parametros_in.timer = &timer;
	parametros_in.socket = &socketin;
	parametros_in.socketaddr = &socketaddrin;
	parametros_in.buffer_saida = buffer_saida;
	//parametros_in.buffer_entrada = buffer_entrada;
	parametros_in.mutex_buffer_saida = &mutex_buffer_saida;
	parametros_in.mutex_timer = &mutex_timer;
}

void Servidor::inicia(void) {
	Mensagem m;

	char * endereco;
	unsigned int porta_saida;

	socklen_t len = sizeof(socketaddrout);
	unsigned char dados[DADOS_APLICACAO];

	char * nome_arquivo;
	FILE *pa;
	unsigned char * dados_arquivo;
	unsigned int dados_lidos;
	unsigned long int parte = 1;

	while(1) {
		switch(estado) {
			case 0:
				endereco = NULL;
				porta_saida = 0;
				nome_arquivo = NULL;
				pa = NULL;
				dados_arquivo = NULL;
				estado = 1;
				break;

			case 1: // Servidor está aguardando conexão
				fprintf(stderr, "\nServidor aguardando conexão... ");
				recvfrom(socketin, dados, DADOS_APLICACAO, MSG_WAITALL, (struct sockaddr *) &socketaddrin, &len);
				m.fromChar(dados);

				if(m.getCabecalho()->getTipo()->toInt() == SOLICITA_CONEXAO) { // Servidor recebeu solicitação do cliente
					fprintf(stderr, "%s solicitou conexão!\n", inet_ntoa(socketaddrin.sin_addr));

					endereco = inet_ntoa(socketaddrin.sin_addr);
					porta_saida = m.getDados()->toInt();
					fprintf(stderr, "Tentando iniciar conexão com %s:%d... ", endereco, porta_saida);
					if(iniciaSocketSaida(inet_ntoa(socketaddrout.sin_addr), porta_saida)) {
						fprintf(stderr, "sucesso!\n");
						m = Mensagem(CONEXAO_ACEITA, 0, 0); // Servidor indica ao cliente que aceitou a conexão
						sendto(socketout, m.toChar(), m.getTamanho(), MSG_WAITALL, (struct sockaddr *) &socketaddrout, sizeof(struct sockaddr_in));
						recvfrom(socketin, dados, DADOS_APLICACAO, MSG_WAITALL, (struct sockaddr *) &socketaddrin, &len);
						m.fromChar(dados);

						if(m.getCabecalho()->getTipo()->toInt() == ACK) // Servidor recebeu ACK do cliente
							estado = 2;
						else {
							fprintf(stderr, "erro!\n");
							estado = 0;
						}

					}
					else {
						fprintf(stderr, "erro!\n");
						estado = 0;
					}
				}
				else {
					fprintf(stderr, "\nProtocolo não cumprido!\n");
					estado = 0;
				}
				break;

			case 2: // Servidor esperando pelo nome do arquivo
				fprintf(stderr, "Aguardando nome do arquivo... ");
				recvfrom(socketin, dados, DADOS_APLICACAO, MSG_WAITALL, (struct sockaddr *) &socketaddrin, &len); // Instrução de recebimento bloqueante (retorna quantos bytes recebeu)
				m.fromChar(dados);

				if(m.getCabecalho()->getTipo()->toInt() == SOLICITA_ARQUIVO) {
					nome_arquivo = m.getDados()->toString();
					fprintf(stderr, "arquivo \"%s\"\n", nome_arquivo);

					fprintf(stderr, "Tentando abrir arquivo... ");
					pa = fopen(nome_arquivo, "rb");
					if(pa != NULL) {
						fprintf(stderr, "sucesso!\n");
						m = Mensagem(ARQUIVO_ACEITO, 0, 0);
						sendto(socketout, m.toChar(), m.getTamanho(), MSG_WAITALL, (struct sockaddr *) &socketaddrout, sizeof(struct sockaddr_in));
						estado = 3;
					}
					else {
						fprintf(stderr, "erro!\n");
						estado = 0;
					}
				}
				else {
					fprintf(stderr, "\nProtocolo não cumprido!\n");
					estado = 0;
				}
				break;

			case 3: // Servidor carregando arquivo na memória
				fprintf(stderr, "Carregando arquivo na memória...\n");

				m = Mensagem(0, 0, 0);
				buffer_saida->insert(buffer_saida->end(), 1, m);

				dados_arquivo = (unsigned char *) malloc(DADOS_APLICACAO);
				while(!feof(pa)) {
					dados_lidos = fread(dados, 1, DADOS_APLICACAO, pa);
					m = Mensagem(TRANSFERE_ARQUIVO, dados_lidos, parte, dados_arquivo);
					//pthread_mutex_lock(&mutex_buffer_saida);
					buffer_saida->insert(buffer_saida->end(), 1, m);
					//pthread_mutex_unlock(&mutex_buffer_saida);
					parte++;
				}
				m = Mensagem(FINAL_ARQUIVO, 0, parte);
				buffer_saida->insert(buffer_saida->end(), 1, m);

				estado = 4;
				break;

			case 4: // Servidor dispara threads que vão transferir o arquivo
				fprintf(stderr, "Enviando arquivo...\n");
				pthread_create(&thread_envia, NULL, &Servidor::threadEnvia, &parametros_out);
				pthread_create(&thread_recebe, NULL, &Servidor::threadRecebe, &parametros_in);

				estado = 5;
				break;

			case 5: // Servidor transferindo o arquivo
				sleep(1);
				break;

			case 6: // Servidor encerra
				//pthread_kill(thread_envia, 0);
				//pthread_kill(thread_recebe, 0);
				fprintf(stderr, "Acabei!\n");
				exit(0);
				break;
		}
	}
}

int Servidor::iniciaSocketSaida(char endereco[], unsigned int porta) {
	socketaddrout.sin_family = AF_INET;
	socketaddrout.sin_port = htons(porta);
	inet_aton(endereco, &socketaddrout.sin_addr);

	if((socketout = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		return 0;

	return 1;
}

int Servidor::iniciaSocketEntrada(unsigned int porta) {
	memset(&socketaddrin, 0, sizeof(socketaddrin));
	socketaddrin.sin_family = AF_INET;
	socketaddrin.sin_port = htons(porta);
	socketaddrin.sin_addr.s_addr = INADDR_ANY;

	if((socketin = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		return 0;

	if(bind(socketin, (struct sockaddr *) &socketaddrin, sizeof(socketaddrin)) == -1)
		return 0;

	return 1;
}

void * Servidor::threadEnvia(void * parametros_out) {
	PARAMETROS_THREAD * pt = (PARAMETROS_THREAD *) parametros_out;

	unsigned long int * base = pt->base;
	unsigned long int * nextseqnum = pt->nextseqnum;
	Timer * timer = pt->timer;

	int * socketout = pt->socket;
	struct sockaddr_in * socketaddrout = pt->socketaddr;
	vector<Mensagem> * buffer_saida = pt->buffer_saida;
	pthread_mutex_t * mutex_buffer_saida = pt->mutex_buffer_saida;
	pthread_mutex_t * mutex_timer = pt->mutex_timer;

	srand(time(NULL));
	ModuloAtraso modulo_atraso(500);

	Mensagem m;

	while(1) {
		pthread_mutex_lock(mutex_buffer_saida);
		unsigned long int tamanho_buffer = buffer_saida->size();
		pthread_mutex_unlock(mutex_buffer_saida);

		if(tamanho_buffer > *nextseqnum) {
			if(*nextseqnum < *base + N) {
				pthread_mutex_lock(mutex_buffer_saida);
				m = buffer_saida->at(*nextseqnum);
				pthread_mutex_unlock(mutex_buffer_saida);
				modulo_atraso.adiciona(m);

				if(*base == *nextseqnum) {
					pthread_mutex_lock(mutex_timer);
					timer->inicia(TIMEOUT);
					pthread_mutex_unlock(mutex_timer);
				}
				(*nextseqnum)++;
			}
		}

		pthread_mutex_lock(mutex_timer);
		bool timer_estogou = timer->esgotou();
		pthread_mutex_unlock(mutex_timer);
		if(timer_estogou) {
			//fprintf(stderr, "\nTimeout!");
			for(int i = *base ; i < (*nextseqnum) ; i++) {
				m = buffer_saida->at(i);
				modulo_atraso.adiciona(m);
			}
			pthread_mutex_lock(mutex_timer);
			timer->inicia(TIMEOUT);
			pthread_mutex_unlock(mutex_timer);
		}

		if(modulo_atraso.esgotouTimer()) {
			Mensagem * enviar = modulo_atraso.leMensagem(1);
			sendto(*socketout, enviar->toChar(), enviar->getTamanho(), MSG_WAITALL, (struct sockaddr *) socketaddrout, sizeof(struct sockaddr_in));
			//fprintf(stderr, "\n##### Enviada (tamanho %d) tipo: %ld  seq: %ld", enviar->getTamanho(), enviar->getCabecalho()->getTipo()->toInt(), enviar->getCabecalho()->getSeq()->toInt());
			modulo_atraso.remove();
		}
	}

	return NULL;
}

void * Servidor::threadRecebe(void * parametros_in) {
	PARAMETROS_THREAD * pt = (PARAMETROS_THREAD *) parametros_in;

	unsigned int * estado = pt->estado;
	unsigned long int * base = pt->base;
	unsigned long int * nextseqnum = pt->nextseqnum;
	Timer * timer = pt->timer;

	int * socketin = pt->socket;
	struct sockaddr_in * socketaddrin = pt->socketaddr;
	socklen_t len = sizeof(socketaddrout);
	vector<Mensagem> * buffer_saida = pt->buffer_saida;
	pthread_mutex_t * mutex_timer = pt->mutex_timer;

	unsigned char dados[DADOS_APLICACAO];
	Mensagem recebida, ack;

	while(1) {
		recvfrom(*socketin, dados, DADOS_APLICACAO, MSG_WAITALL, (struct sockaddr *) socketaddrin, &len); // Instrução de recebimento bloqueante (retorna quantos bytes recebeu)
		recebida.fromChar(dados);

		//fprintf(stderr, "\n##### Recebida (tamanho %d) tipo: %ld   seq: %ld", recebida.getTamanho(), recebida.getCabecalho()->getTipo()->toInt(), recebida.getCabecalho()->getSeq()->toInt());
		*base = recebida.getCabecalho()->getSeq()->toInt() + 1;
		if(*base == *nextseqnum) {
			pthread_mutex_lock(mutex_timer);
			timer->para();
			pthread_mutex_unlock(mutex_timer);
		}
		else {
			pthread_mutex_lock(mutex_timer);
			timer->inicia(TIMEOUT);
			pthread_mutex_unlock(mutex_timer);
		}
		if(recebida.getCabecalho()->getSeq()->toInt() == buffer_saida->size()) {
			*estado = 6;
		}
	}

	return NULL;
}

