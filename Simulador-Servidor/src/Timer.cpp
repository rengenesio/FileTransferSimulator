#include "Timer.h"

Timer::Timer() {
	this->ativo = false;
}
Timer::~Timer() {
}

void Timer::setInicio(clock_t inicio) {
	this->inicio = inicio;
}
clock_t Timer::getInicio() {
	return inicio;
}

void Timer::setDuracao(double duracao) {
	this->duracao= duracao;
}
double Timer::getDuracao() {
	return duracao;
}

void Timer::setAtivo(bool ativo) {
	this->ativo = ativo;
}
bool Timer::getAtivo() {
	return ativo;
}

void Timer::inicia(double duracao) {
	inicio = clock();
	this->duracao = duracao;
	ativo = true;
}

void Timer::para() {
	ativo = false;
}

bool Timer::esgotou() {
	if(ativo)
		if((double) (clock() - inicio) / CLOCKS_PER_SEC > duracao)
			return 1;

	return 0;
}

double Timer::restante() {
	if(ativo)
		return duracao - ((double) (clock() - inicio) / CLOCKS_PER_SEC);

	return -1;
}
