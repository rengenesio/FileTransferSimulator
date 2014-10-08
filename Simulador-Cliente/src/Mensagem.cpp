#include "Mensagem.h"
#include <stdio.h>

Mensagem::Mensagem() {
	this->cabecalho = new Cabecalho();
}
Mensagem::Mensagem(unsigned int tipo, unsigned int tamanho_dados, unsigned long int seq, unsigned char * mensagem) {
	this->cabecalho = new Cabecalho(tipo, tamanho_dados, seq);
	this->tamanho = cabecalho->getTamanho() + tamanho_dados;
	this->dados = new ByteVetor(tamanho_dados, mensagem, 0);
}
Mensagem::~Mensagem() {
}

unsigned int Mensagem::getTamanho() {
	return tamanho;
}

Cabecalho * Mensagem::getCabecalho() {
	return cabecalho;
}

ByteVetor * Mensagem::getDados() {
	return dados;
}

void Mensagem::fromChar(unsigned char * mensagem) {
	this->cabecalho = new Cabecalho(mensagem);
	this->dados = new ByteVetor(this->cabecalho->getTamanhoDados()->toInt(), mensagem + this->cabecalho->getTamanho(), 0);
	this->tamanho = cabecalho->getTamanho() + dados->getTamanho();
}

unsigned char * Mensagem::toChar() {
	unsigned char * mensagem;

	mensagem = (unsigned char *) malloc(tamanho);
	memset(mensagem, 0, tamanho);

	memcpy(mensagem, cabecalho->toChar(), cabecalho->getTamanho());
	memcpy(mensagem + cabecalho->getTamanho(), dados->toChar(), dados->getTamanho());

	return mensagem;
}
