#include "sistema.h"
#include <algorithm>

bool vecinoConPlaga(Sistema s, Posicion p);

Sistema::Sistema()
{
}

Sistema::Sistema(const Campo & c, const Secuencia<Drone>& ds)
{
}

const Campo & Sistema::campo() const
{
	return this->_campo;
}

EstadoCultivo Sistema::estadoDelCultivo(const Posicion & p) const
{
	return this->_estado.parcelas[p.x][p.y];
}

const Secuencia<Drone>& Sistema::enjambreDrones() const
{
	return this->_enjambre;
}

void Sistema::crecer()
{
	Posicion p;
	EstadoCultivo c = RecienSembrado;
	EstadoCultivo a = EnCrecimiento;
	EstadoCultivo b = ListoParaCosechar;
	for (int i = campo().dimensiones().largo; i >= 0; --i){
		for (int j = 0; j < campo().dimensiones().ancho; ++j){
			p.y = j;
			p.x = i;
			if (campo().contenido(p) == Cultivo){
				if (estadoDelCultivo(p) == c){
					this->_estado.parcelas[p.x][p.y] = a;
				}
				else{
					if (estadoDelCultivo(p) == a){
					this->_estado.parcelas[p.x][p.y] = b; //no me deja usar estadoDelCultivo(p) == b;
					}
				}
			}
		}
	}
}

void Sistema::seVinoLaMaleza(const Secuencia<Posicion>& ps)
{
	//en la especificacion se REQUIERE que la lista ps solo contenga Parcelas de Cultivo...
	EstadoCultivo c = ConMaleza;
	for (int i = 0; i < ps.size(); ++i){
		Posicion p = ps[i];
		this->_estado.parcelas[p.x][p.y] = c;
	}
}

void Sistema::seExpandePlaga()
{
	Posicion p;
	EstadoCultivo a = ConPlaga;
	for (int i = campo().dimensiones().largo; i >= 0; --i){
		for (int j = 0; j < campo().dimensiones().ancho; ++j){
			p.y = j;
			p.x = i;
			if (vecinoConPlaga(*this,p)){
				this->_estado.parcelas[p.x][p.y] = a;
			}
		}
	}
}

bool vecinoConPlaga(Sistema s, Posicion p) //le pongo const a los parametros o no??
{
	bool tieneUnVecinoConPlaga = false;
	EstadoCultivo a = ConPlaga;
	Posicion pos;
	for (int i = s.campo().dimensiones().largo; i >= 0; --i){
		for (int j = 0; j < s.campo().dimensiones().ancho; ++j){
			pos.y = j;
			pos.x = i;
			bool esVecino = (((pos.y -1 == p.y)||(pos.y == p.y)||(pos.y+1 == p.y)) &&((pos.x == p.x)||(pos.x+1 == p.x)||(pos.x-1 == p.x)));
			if (esVecino && s.estadoDelCultivo(pos) == a){
				tieneUnVecinoConPlaga = true;
				break;
			}
		}
	}
	return tieneUnVecinoConPlaga;
}

void Sistema::despegar(const Drone & d)
{
}

bool Sistema::listoParaCosechar() const
{
	return false;
}

void Sistema::aterrizarYCargarBaterias(Carga b)
{
}

void Sistema::fertilizarPorFilas()
{
}

void Sistema::volarYSensar(const Drone & d)
{
}

void Sistema::mostrar(std::ostream & os) const
{
}

void Sistema::guardar(std::ostream & os) const
{
}

void Sistema::cargar(std::istream & is)
{
}

bool Sistema::operator==(const Sistema & otroSistema) const
{
	return false;
}

std::ostream & operator<<(std::ostream & os, const Sistema & s)
{
	return os;
}
