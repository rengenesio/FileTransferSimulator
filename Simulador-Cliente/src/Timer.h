#ifndef TIMER_H_
#define TIMER_H_

#include <time.h>

/* Classe que implementa um temporizador */
class Timer {
public:
	Timer();
	virtual ~Timer();

	void inicia(double duracao);
	void para();
	bool esgotou();
	double restante();

private:
	// Marca em qual clock da CPU começou a contar
	clock_t inicio;
	// Duração em segundos do temporizador
	double duracao;
	bool ativo;
};

#endif /* TIMER_H_ */
