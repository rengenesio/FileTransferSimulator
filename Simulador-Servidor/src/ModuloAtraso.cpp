#include "ModuloAtraso.h"
#include <stdio.h>


ModuloAtraso::ModuloAtraso() {
}
ModuloAtraso::ModuloAtraso(unsigned int tamanho) {
	this->tamanho = tamanho;
	mensagem = (Mensagem *) malloc(tamanho * sizeof(Mensagem));
	envia = (bool *) malloc(tamanho * sizeof(bool));
	timer = (Timer *) malloc(tamanho * sizeof(Timer));

	srand(time(NULL));

	FILE *pa = fopen("../../config_modulo.ini", "r");

	if(!fscanf(pa, "p = %lf\n", &p))
		return;
	if(!fscanf(pa, "rtt = %lf\n", &rtt))
		return;
	if(!fscanf(pa, "e[x] = %lf", &e_x))
		return;
	fclose(pa);

	fprintf(stderr, "p = %lf\nrtt = %lf\n e[x] = %lf\n", p, rtt, e_x);

	elementos = 0;
}
ModuloAtraso::~ModuloAtraso() {
}

void ModuloAtraso::adiciona(Mensagem m) {
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
		//fprintf(stderr, "\nPacote %ld será atrasado %lf segundos (tem %ld antes e %ld depois)", m.getCabecalho()->getSeq()->toInt(), atraso, i-1, elementos-i);

	}
	//else {
		//fprintf(stderr, "\nPacote %ld foi perdido!", m.getCabecalho()->getSeq()->toInt());
	//}
}

Mensagem * ModuloAtraso::leMensagem(unsigned long int posicao) {
	if(vazio() || posicao > elementos || posicao == 0)
		return NULL;

	return &(mensagem[posicao]);
}

bool * ModuloAtraso::leEnvia(unsigned long int posicao) {
	if(vazio() || posicao > elementos || posicao == 0)
		return NULL;

	return &(envia[posicao]);
}

Timer * ModuloAtraso::leTimer(unsigned long int posicao) {
	if(vazio() || posicao > elementos || posicao == 0)
		return NULL;

	return &(timer[posicao]);
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

void ModuloAtraso::paraTimer(unsigned long int posicao) {
	timer[posicao].para();
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

void ModuloAtraso::alteraEnvia(unsigned long int posicao) {
	if((double(rand() % 1000) / 1000) >= p) {
		envia[posicao] = true;
	}
	else {
		fprintf(stderr, "\nPacote %ld foi perdido!", mensagem[posicao].getCabecalho()->getSeq()->toInt());
		envia[posicao] = false;
	}
}

void ModuloAtraso::alteraAtraso(unsigned long int posicao) {
	if(envia[posicao]) {
		double atraso = rtt/2 + randExponencial(e_x);
		fprintf(stderr, "\nPacote %ld será atrasado %lf segundos", mensagem[posicao].getCabecalho()->getSeq()->toInt(), atraso);
		timer[posicao].inicia(atraso);
	}
}

