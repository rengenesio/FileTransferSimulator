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

	unsigned int getTamanho();
	ByteVetor * getTipo();
	ByteVetor * getTamanhoDados();
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
