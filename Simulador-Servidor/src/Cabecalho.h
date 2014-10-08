/*
 * Cabecalho.h
 *
 *  Created on: 18/06/2013
 *      Author: renan
 */

#ifndef CABECALHO_H_
#define CABECALHO_H_


#include "ByteVetor.h"
#include "../../Defines.h"


class Cabecalho {
public:
	Cabecalho();
	Cabecalho(unsigned int tipo, unsigned int tamanho_dados, unsigned int seq);
	Cabecalho(unsigned char * cabecalho);
	virtual ~Cabecalho();

	void setTamanho(unsigned int tamanho);
	unsigned int getTamanho();
	void setTipo(ByteVetor *tipo);
	ByteVetor * getTipo();
	void setTamanhoDados(ByteVetor *tamanho);
	ByteVetor * getTamanhoDados();
	void setSeq(ByteVetor *seq);
	ByteVetor * getSeq();

	void fromChar(unsigned char * cabecalho);
	unsigned char * toChar();

private:
	unsigned int tamanho;
	ByteVetor *tipo;
	ByteVetor *tamanho_dados;
	ByteVetor *seq;
};

#endif /* CABECALHO_H_ */
