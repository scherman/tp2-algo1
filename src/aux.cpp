#include "aux.h"

//Implementen aqui sus funciones auxiliares globales definidas en aux.h...

std::ostream & operator<<(std::ostream & os, const Posicion & p){
	os << "[" << p.x << "," << p.y << "]";
	return os;
}
