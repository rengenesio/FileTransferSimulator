#include "ModuloAtraso.h"
#include <stdio.h>

ModuloAtraso::ModuloAtraso() {
}
ModuloAtraso::ModuloAtraso(unsigned long int tamanho) {
	this->tamanho = tamanho;

	mensagem = (Mensagem *) malloc(tamanho * sizeof(Mensagem));
	envia = (bool *) malloc(tamanho * sizeof(bool));
	timer = (Timer *) malloc(tamanho * sizeof(Timer));

	FILE *pa = fopen("../../config_modulo.ini", "r");

	if(!fscanf(pa, "p = %lf\n", &p))
		return;
	if(!fscanf(pa, "rtt = %lf\n", &rtt))
		return;
	if(!fscanf(pa, "e[x] = %lf", &e_x))
		return;
	fclose(pa);

	fprintf(stderr, "p = %lf\nrtt = %lf\n e[x] = %lf\n", p, rtt, e_x);

	srand(time(NULL));
	elementos = 0;
}
ModuloAtraso::~ModuloAtraso() {
}

void ModuloAtraso::adicionaOrdenado(Mensagem m) {
	if((double(rand() % 1000) / 1000) >= p) {
		unsigned long int i = 1;
		double atraso = rtt/2 + randExponencial(e_x);
		for(i = elementos+1 ; i > 1 && atraso < timer[i-1].restante() ; i--) {
			mensagem[i] = mensagem[i-1];
			timer[i] = timer[i-1];
		}
		mensagem[i] = m;
		timer[i].inicia(atraso);

		elementos++;
	}
}

Mensagem * ModuloAtraso::leMensagem(unsigned long int posicao) {
	if(vazio() || posicao > elementos || posicao == 0)
		return NULL;

	return &(mensagem[posicao]);
}

bool ModuloAtraso::esgotouTimer() {
	if(elementos)
		if(timer[1].esgotou())
			return true;

	return false;
}

void ModuloAtraso::remove() {
	if(elementos - 1 > 0) {
		for(unsigned long int i = 1 ; i < elementos ; i++) {
			mensagem[i] = mensagem[i+1];
			envia[i] = envia[i+1];
			timer[i] = timer[i+1];
		}
	}
	elementos--;
}

int ModuloAtraso::vazio() {
	if(elementos == 0)
		return 1;

	return 0;
}

double ModuloAtraso::randExponencial(double media) {
	double R;
    R = (double)rand()/(double)(RAND_MAX);

    return  -media * log(R);
}
