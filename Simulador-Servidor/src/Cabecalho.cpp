/*
 * Cabecalho.cpp
 *
 *  Created on: 18/06/2013
 *      Author: renan
 */

#include "Cabecalho.h"

Cabecalho::Cabecalho() {
}
Cabecalho::Cabecalho(unsigned int tipo, unsigned int tamanho_dados, unsigned int seq) {
	this->tipo = new ByteVetor(TAM_TIPO, tipo);
	this->tamanho_dados = new ByteVetor(TAM_TAMANHO_DADOS, tamanho_dados);
	this->seq = new ByteVetor(TAM_SEQ, seq);
	tamanho = CABECALHO_APLICACAO;
}
Cabecalho::Cabecalho(unsigned char * cabecalho) {
	this->tipo = new ByteVetor(TAM_TIPO);
	this->tamanho_dados = new ByteVetor(TAM_TAMANHO_DADOS);
	this->seq = new ByteVetor(TAM_SEQ);
	tamanho = CABECALHO_APLICACAO;
	fromChar(cabecalho);
}
Cabecalho::~Cabecalho() {
}

void Cabecalho::setTamanho(unsigned int tamanho) {
	this->tamanho = tamanho;
}
unsigned int Cabecalho::getTamanho() {
	return tamanho;
}

void Cabecalho::setTipo(ByteVetor *tipo) {
	this->tipo = tipo;
}
ByteVetor * Cabecalho::getTipo() {
	return tipo;
}

void Cabecalho::setTamanhoDados(ByteVetor *tamanho_dados) {
	this->tamanho_dados = tamanho_dados;
}
ByteVetor * Cabecalho::getTamanhoDados() {
	return tamanho_dados;
}

void Cabecalho::setSeq(ByteVetor *seq) {
	this->seq = seq;
}
ByteVetor * Cabecalho::getSeq() {
	return seq;
}

void Cabecalho::fromChar(unsigned char * cabecalho) {
	tipo->fromChar(TAM_TIPO, cabecalho);
	tamanho_dados->fromChar(TAM_TAMANHO_DADOS, cabecalho + TAM_TIPO);
	this->seq->fromChar(TAM_SEQ, cabecalho + TAM_TIPO + TAM_TAMANHO_DADOS);
}

unsigned char * Cabecalho::toChar() {
	unsigned char * cabecalho;

	cabecalho = (unsigned char *) malloc(tamanho);
	memset(cabecalho, 0, tamanho);

	memcpy(cabecalho, tipo->toChar(), TAM_TIPO);
	memcpy(cabecalho + TAM_TIPO, tamanho_dados->toChar(), TAM_TAMANHO_DADOS);
	memcpy(cabecalho + TAM_TIPO + TAM_TAMANHO_DADOS, seq->toChar(), TAM_SEQ);

	return cabecalho;
}
