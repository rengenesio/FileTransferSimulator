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

	unsigned int getTamanho();
	Cabecalho * getCabecalho();
	ByteVetor * getDados();

	void fromChar(unsigned char * mensagem);
	unsigned char * toChar();

private:
	unsigned int tamanho;
	Cabecalho *cabecalho;
	ByteVetor *dados;
};

#endif /* Mensagem_H_ */
