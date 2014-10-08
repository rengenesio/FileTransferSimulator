/*
 * Datagrama.h
 *
 *  Created on: 18/06/2013
 *      Author: renan
 */

#ifndef MENSAGEM_H_
#define MENSAGEM_H_


#include <string.h>
#include "Cabecalho.h"
#include "../../Defines.h"


class Mensagem {
public:
	Mensagem();
	Mensagem(unsigned int tipo, unsigned int tamanho_dados, unsigned long int seq, unsigned char * mensagem = NULL);
	virtual ~Mensagem();

	void setTamanho(unsigned int tamanho);
	unsigned int getTamanho();
	void setCabecalho(Cabecalho * cabecalho);
	Cabecalho * getCabecalho();
	void setDados(ByteVetor * dados);
	ByteVetor * getDados();

	void fromChar(unsigned char * mensagem);
	unsigned char * toChar();

private:
	unsigned int tamanho;
	Cabecalho *cabecalho;
	ByteVetor *dados;
};

#endif /* Mensagem_H_ */
