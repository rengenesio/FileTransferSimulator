#ifndef BYTEVETOR_H_
#define BYTEVETOR_H_


#include <stdlib.h>
#include <string.h>


class ByteVetor {
public:
	ByteVetor(unsigned int tamanho, unsigned long int valor = 0);
	ByteVetor(unsigned int tamanho, unsigned char string[], int a);
	virtual ~ByteVetor();

	void setTamanho(unsigned int tamanho);
	unsigned int getTamanho();

	void fromInt(unsigned long int n);
	unsigned long int toInt();
	void fromChar(unsigned int tamanho, unsigned char * string);
	unsigned char * toChar();
	char * toString();

	void zera();

private:
	unsigned int tamanho;
	unsigned char * byte;
};

#endif /* BYTEVETOR_H_ */
