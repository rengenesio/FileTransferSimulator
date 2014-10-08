#ifndef MODULOATRASO_H_
#define MODULOATRASO_H_

#include <stdlib.h>
#include <math.h>
#include "Mensagem.h"
#include "Timer.h"

class ModuloAtraso {
public:
	ModuloAtraso();
	ModuloAtraso(unsigned long int tamanho);
	virtual ~ModuloAtraso();

	void adicionaOrdenado(Mensagem m);
	Mensagem * leMensagem(unsigned long int posicao);
	bool esgotouTimer();
	void remove();

	double randExponencial(double media);

	int vazio();

private:
	unsigned long int tamanho, elementos;

	Mensagem * mensagem;
	bool * envia;
	Timer * timer;

	double p, rtt, e_x;
};

#endif /* MODULOATRASO_H_ */
