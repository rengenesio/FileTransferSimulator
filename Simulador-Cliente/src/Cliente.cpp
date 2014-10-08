#include "Cliente.h"

Cliente::Cliente() {
	puts("Inicializando o cliente...");

	inicializaVariaveis();

	fprintf(stderr, "Cliente inicializado com sucesso!\n");

	principal();
}

Cliente::~Cliente() {
}

void Cliente::inicializaVariaveis() {
	memset(&socketaddrout, 0, sizeof(socketaddrout));

	buffer_entrada = new queue<Mensagem>();
	buffer_saida = new queue<Mensagem>();

	estado = 0;
	base = 1;
	nextseqnum = 1;
	expectedseqnum = 1;

	modulo_atraso = ModuloAtraso(10000);

	pthread_mutex_init(&mutex_buffer_saida, NULL);

	parametros_out.estado = &estado;
	parametros_out.base = &base;
	parametros_out.nextseqnum = &nextseqnum;
	parametros_out.expectedseqnum = &expectedseqnum;
	parametros_out.socket = &socketout;
	parametros_out.socketaddr = &socketaddrout;
	parametros_out.buffer_entrada = buffer_entrada;
	parametros_out.buffer_saida = buffer_saida;
	parametros_out.mutex_buffer_saida = &mutex_buffer_saida;
	parametros_out.modulo_atraso = &modulo_atraso;

	parametros_in.estado = &estado;
	parametros_in.base = &base;
	parametros_in.nextseqnum = &nextseqnum;
	parametros_in.expectedseqnum = &expectedseqnum;
	parametros_in.socket = &socketin;
	parametros_in.socketaddr = &socketaddrin;
	parametros_in.buffer_saida = buffer_saida;
	parametros_in.buffer_entrada = buffer_entrada;
	parametros_in.mutex_buffer_saida = &mutex_buffer_saida;
	parametros_in.modulo_atraso = &modulo_atraso;
}

void Cliente::principal(void) {
	unsigned int porta_entrada = 0;
	srand(time(NULL));

	char endereco[15];
	unsigned char dados[DADOS_APLICACAO];
	unsigned char nome_arquivo[50];
	socklen_t len = sizeof(socketaddrout);
	unsigned int porta;
	Mensagem m;
	FILE *pa;

	clock_t inicio;
	double tempo;
	unsigned long int bytes = 0;

	while(1) {
		switch(estado) {
			case 0: //Pede IP e porta do servidor e tenta iniciar socket
				strcpy(endereco, ENDERECO);
				porta = PORTA_PADRAO;
				fprintf(stderr, "\nInicializando socket de saída a host %s:%d... ", endereco, porta);
				if(iniciaSocketSaida(endereco, porta)) {
					fprintf(stderr, "sucesso!\n");
					estado = 1;
				}
				else
					fprintf(stderr, "erro!\n");

				break;

			case 1: // Inicializa socket para o servidor se conectar
				porta_entrada = rand() % 65536;
				fprintf(stderr, "Tentando inicializar socket de entrada na porta %d... ", porta_entrada);
				if(iniciaSocketEntrada(porta_entrada)) {
					fprintf(stderr, "sucesso!\n");
					estado = 2;
				}
				else {
					fprintf(stderr, "erro! Reiniciando cliente!\n");
					estado = 1;
				}

				break;

			case 2: // Three-way handshake
				fprintf(stderr, "Tentando estabelecer conexão com o servidor... ");
	inicio = clock();
				m = Mensagem(SOLICITA_CONEXAO, 2, 0, ByteVetor(2, porta_entrada).toChar()); // Cliente solicita conexão
				sendto(socketout, m.toChar(), m.getTamanho(), MSG_WAITALL, (struct sockaddr *) &socketaddrout, sizeof(struct sockaddr_in));
				recvfrom(socketin, dados, DADOS_APLICACAO, MSG_WAITALL, (struct sockaddr *) &socketaddrin, &len); // Servidor responde
				m.fromChar(dados);
				if(m.getCabecalho()->getTipo()->toInt() == CONEXAO_ACEITA) {
					fprintf(stderr, "sucesso!\n");
					m = Mensagem(ACK, 0, 0); // Cliente envia um ack pra indicar que conexão foi estabelecida
					sendto(socketout, m.toChar(), m.getTamanho(), MSG_WAITALL, (struct sockaddr *) &socketaddrout, sizeof(struct sockaddr_in));
					estado = 3;
				}
				else {
					fprintf(stderr, "erro! Reiniciando cliente!\n");
					estado = 0;
				}
	fprintf(stderr, "Tempo para estabelecar a conexão: %lf", (double) (inicio - clock()) / CLOCKS_PER_SEC);
				break;

			case 3: // Solicita o arquivo do servidor
				nome_arquivo[0] = '5';
				nome_arquivo[1] = '0';
				nome_arquivo[2] = 'm';
				nome_arquivo[3] = 'b';

				fprintf(stderr, "Solicitando arquivo do servidor... ");
				m = Mensagem(SOLICITA_ARQUIVO, 4, 0, nome_arquivo); // Envia nome do arquivo ao servidor
				sendto(socketout, m.toChar(), m.getTamanho(), MSG_WAITALL, (struct sockaddr *) &socketaddrout, sizeof(struct sockaddr_in));
				recvfrom(socketin, dados, DADOS_APLICACAO, MSG_WAITALL, (struct sockaddr *) &socketaddrin, &len);
				m.fromChar(dados);
				if(m.getCabecalho()->getTipo()->toInt() == ARQUIVO_ACEITO) {
					fprintf(stderr, "sucesso!\n");
					estado = 4;
				}
				else {
					fprintf(stderr, "erro! Reiniciando cliente!\n");
					estado = 0;
				}
				break;

			case 4: // Inicia a transferência do arquivo
				fprintf(stderr, "Recebendo arquivo... ");
				inicio = clock();
				pthread_create(&thread_recebe, NULL, &Cliente::threadRecebe, &parametros_in);
				pthread_create(&thread_envia, NULL, &Cliente::threadEnvia, &parametros_out);
				estado = 5;
				break;

			case 5: // Thread principal não faz nada enquanto arquivo está sendo transferido
				sleep(1);
				break;

			case 6: // Junta o arquivo e encerra o cliente
				tempo = (double)(clock() - inicio) / CLOCKS_PER_SEC;
				fprintf(stderr, "\nArquivo transferido com sucesso!");
				fprintf(stderr, "\nTempo de transferência: %lf segundos", tempo);

				pa = fopen("abc", "wb");
				for(unsigned long int i = 0 ; i < expectedseqnum - 2 ; i++) {
					m = buffer_entrada->front();
					bytes += fwrite(m.getDados()->toChar(), 1, m.getCabecalho()->getTamanhoDados()->toInt(), pa);
					buffer_entrada->pop();
				}
				fclose(pa);
				fprintf(stderr, "\nTamanho do arquivo: %.2lf kbytes", (double)bytes/1000);
				fprintf(stderr, "\nVazão média: %lf kbytes/s\n", (double)(bytes)/(1000 * tempo));

				exit(0);
		}
	}
}

int Cliente::iniciaSocketSaida(char endereco[], unsigned int porta) {
	socketaddrout.sin_family = AF_INET;
	socketaddrout.sin_port = htons(porta);
	inet_aton(endereco, &socketaddrout.sin_addr);

	if((socketout = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		return 0;

	return 1;
}

int Cliente::iniciaSocketEntrada(unsigned int porta) {
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

void * Cliente::threadEnvia(void * parametros_out) {
	PARAMETROS_THREAD * pt = (PARAMETROS_THREAD *) parametros_out;

	int * socketout = pt->socket;
	struct sockaddr_in * socketaddrout = pt->socketaddr;
	ModuloAtraso * modulo_atraso = pt->modulo_atraso;

	Mensagem m;

	while(1) {
		if(modulo_atraso->esgotouTimer()) {
			Mensagem * enviar = modulo_atraso->leMensagem(1);
			sendto(*socketout, enviar->toChar(), enviar->getTamanho(), MSG_WAITALL, (struct sockaddr *) socketaddrout, sizeof(struct sockaddr_in));
			//fprintf(stderr, "\n##### Enviada (tamanho %d) tipo: %ld  seq: %ld", enviar->getTamanho(), enviar->getCabecalho()->getTipo()->toInt(), enviar->getCabecalho()->getSeq()->toInt());
			modulo_atraso->remove();
		}
	}

	return NULL;
}

void * Cliente::threadRecebe(void * parametros_in) {
	PARAMETROS_THREAD * pt = (PARAMETROS_THREAD *) parametros_in;

	unsigned int * estado = pt->estado;
	unsigned long int * expectedseqnum = pt->expectedseqnum;

	int * socketin = pt->socket;
	struct sockaddr_in * socketaddrin = pt->socketaddr;
	queue<Mensagem> * buffer_entrada = pt->buffer_entrada;

	unsigned char dados[DADOS_APLICACAO];
	socklen_t len = sizeof(socketaddrout);
	ModuloAtraso * modulo_atraso = pt->modulo_atraso;
	Mensagem recebida, ack;

	ack = Mensagem(ACK, 0, 0);

	while(1) {
		recvfrom(*socketin, dados, DADOS_APLICACAO, MSG_WAITALL, (struct sockaddr *) socketaddrin, &len); // Instrução de recebimento bloqueante (retorna quantos bytes recebeu)
		recebida.fromChar(dados);

		//fprintf(stderr, "\n##### Recebida (tamanho %d) tipo: %ld  seq: %ld (to esperando %lu)", recebida.getTamanho(), recebida.getCabecalho()->getTipo()->toInt(), recebida.getCabecalho()->getSeq()->toInt(), *expectedseqnum);

		if(recebida.getCabecalho()->getSeq()->toInt() == *expectedseqnum) {
			buffer_entrada->push(recebida);

			ack = Mensagem(ACK, 0, *expectedseqnum);
			(*expectedseqnum)++;
		}

		modulo_atraso->adicionaOrdenado(ack);

		if(recebida.getCabecalho()->getTipo()->toInt() == FINAL_ARQUIVO) {
			while(!modulo_atraso->vazio());
			*estado = 6;
		}

	}

	return NULL;
}
