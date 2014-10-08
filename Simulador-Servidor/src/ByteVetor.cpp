#include "ByteVetor.h"

#include<stdio.h>

ByteVetor::ByteVetor(unsigned int tamanho, unsigned long int valor) {
	this->tamanho = tamanho;
	byte = (unsigned char *) malloc(tamanho * sizeof(unsigned char));

	if(valor)
		fromInt(valor);
	else
		zera();
}
ByteVetor::ByteVetor(unsigned int tamanho, unsigned char * string, int a) {
	this->tamanho = tamanho;
	byte = (unsigned char *) malloc(tamanho * sizeof(unsigned char));

	fromChar(tamanho, string);
}

ByteVetor::~ByteVetor() {
}

void ByteVetor::setTamanho(unsigned int tamanho) {
	this->tamanho = tamanho;
}
unsigned int ByteVetor::getTamanho() {
	return tamanho;
}

void ByteVetor::fromInt(unsigned long int n) {
	zera();
	for(int i = (int) tamanho-1 ; i >= 0 ; i--) {
		memcpy(byte+i, &n, 1);
		n >>= 8;
	}
}

unsigned long int ByteVetor::toInt() {
	unsigned int n = 0;

	for(int i = 0 ; i < (int) tamanho-1 ; i++) {
		memcpy(&n, byte+i, 1);
		n <<= 8;
	}
	memcpy(&n, byte+tamanho-1, 1);

	return n;
}

unsigned char * ByteVetor::toChar() {
	unsigned char * string;

	string = (unsigned char *) malloc(tamanho);
	memset(string, 0, tamanho);

	memcpy(string, byte, tamanho);

	return string;
}

void ByteVetor::fromChar(unsigned int tamanho, unsigned char * string) {
	zera();
	memcpy(byte, string, tamanho);
}

char * ByteVetor::toString() {
	char * string;

	string = (char *) malloc(tamanho + 1);
	memset(string, 0, tamanho);

	memcpy(string, byte, tamanho);
	string[tamanho] = '\0';

	return string;
}

void ByteVetor::zera() {
	for(int i = 0 ; i < (int) tamanho ; i++)
		byte[i] = 0;
}
