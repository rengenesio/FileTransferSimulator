#ifndef TIMER_H_
#define TIMER_H_

#include <time.h>

class Timer {
public:
	Timer();
	virtual ~Timer();

	void setInicio(clock_t inicio);
	clock_t getInicio();
	void setDuracao(double duracao);
	double getDuracao();
	void setAtivo(bool ativo);
	bool getAtivo();

	void inicia(double duracao);
	void para();
	bool esgotou();
	double restante();

private:
	clock_t inicio;
	double duracao;
	bool ativo;

};

#endif /* TIMER_H_ */
