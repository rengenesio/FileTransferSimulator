/*
 * Buffer.h
 *
 *  Created on: 20/06/2013
 *      Author: renan
 */

#ifndef MODULOATRASO_H_
#define MODULOATRASO_H_


#include <stdlib.h>
#include <math.h>
#include "Mensagem.h"
#include "Timer.h"


class ModuloAtraso {
public:
	ModuloAtraso();
	ModuloAtraso(unsigned int tamanho);
	virtual ~ModuloAtraso();

	void adiciona(Mensagem m);
	Mensagem * leMensagem(unsigned long int posicao);
	bool * leEnvia(unsigned long int posicao);
	Timer * leTimer(unsigned long int posicao);
	bool esgotouTimer();
	void remove();
	void paraTimer(unsigned long int posicao);

	double randExponencial(double media);
	void alteraEnvia(unsigned long int posicao);
	void alteraAtraso(unsigned long int posicao);

	int vazio();

private:
	unsigned long int tamanho, elementos;

	Mensagem * mensagem;
	bool * envia;
	Timer * timer;

	double p;
	double rtt;
	double e_x;
};

#endif /* MODULOATRASO_H_ */
