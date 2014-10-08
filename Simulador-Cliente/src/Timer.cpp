#include "Timer.h"

Timer::Timer() {
	this->ativo = false;
}
Timer::~Timer() {
}

void Timer::inicia(double duracao) {
	inicio = clock(); // Marca o instante em que começa
	this->duracao = duracao; // Seta a duração
	ativo = true; // Ativa o timer
}

void Timer::para() {
	ativo = false; // Desativa o timer
}

bool Timer::esgotou() {
	if(ativo) // Verifica se o timer está ativo
		if((double) (clock() - inicio) / CLOCKS_PER_SEC > duracao) // Verifica o tempo em segundos
			return true;

	return false;
}

double Timer::restante() {
	if(ativo) // Verifica se o timer está ativo
		return duracao - ((double) (clock() - inicio) / CLOCKS_PER_SEC); // Retorna o tempo restante em segundos

	return -1;
}
