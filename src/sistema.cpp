#include "sistema.h"
#include <algorithm>

//estos ahi que agregarlos en la parte privada despues! Y pasar las funciones a un aux.cpp?
bool vecinoConPlaga(const Sistema &s, const Posicion &p);
Secuencia<Posicion> posiblePosicionLibre(const Sistema &s);
Posicion posDelGranero(const Campo &c);
bool estaLibre(const Sistema &s, const Posicion &p);
Secuencia<Posicion> parcelasCultivo(const Campo &c);
int cantCultivosCosechables(const Sistema &s);

Sistema::Sistema()
{
	this->_campo = Campo();

	srand(time(NULL));
	int cantDrones = rand() % this->_campo.dimensiones().largo; // Calculo drones a crear
	this->_enjambre = Secuencia<Drone> (cantDrones);
	for (int i = 0; i < cantDrones; i++) { // Creo drone por drone y lo agrego al enjambre
		Secuencia<Producto> productos = {Fertilizante, Fertilizante, PlaguicidaBajoConsumo, Herbicida, HerbicidaLargoAlcance};
		Drone drone(i, productos);
		this->_enjambre[i] = drone;
	}

	// Inicializo estados del cultivo
	this->_estado = Grilla<EstadoCultivo>(this->_campo.dimensiones());
	for (int i = 0; i < this->_campo.dimensiones().ancho; i++) {
		for (int j = 0; j < this->_campo.dimensiones().largo; j++) {
			Posicion posicionActual;
			posicionActual.x = i;
			posicionActual.y = j;
			if (this->_campo.contenido(posicionActual) == Cultivo) {
				_estado.parcelas[i][j] = NoSensado;
			}
		}
	}
}

Sistema::Sistema(const Campo & c, const Secuencia<Drone>& ds)
{
	this->_campo = c;
	this->_enjambre = ds;
	this->_estado = Grilla<EstadoCultivo>(c.dimensiones());
	for (int i = 0; i < c.dimensiones().ancho; i++) {
		for (int j = 0; j < c.dimensiones().largo; j++) {
			Posicion posicionActual;
			posicionActual.x = i;
			posicionActual.y = j;
			if (c.contenido(posicionActual) == Cultivo) {
				_estado.parcelas[i][j] = NoSensado;
			}
		}
	}

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
//AUXILIARES
bool vecinoConPlaga(Sistema s, Posicion p) //le pongo const a los parametros o no??
{
	bool tieneUnVecinoConPlaga = false;
	EstadoCultivo a = ConPlaga;
	Posicion pos;
	for (int i = s.campo().dimensiones().largo; i >= 0; --i){
		for (int j = 0; j < s.campo().dimensiones().ancho; ++j){
			pos.y = j;
			pos.x = i;
			bool esVecino = (((pos.y -1 == p.y)||(pos.y == p.y)||(pos.y+1 == p.y)) &&((pos.x == p.x)||(pos.x+1 == p.x)||(pos.x-1 == p.x)));//se podria mejorar, implementado la funcion distancia!
			if (esVecino && s.estadoDelCultivo(pos) == a){
				tieneUnVecinoConPlaga = true;
				break;
			}
		}
	}
	return tieneUnVecinoConPlaga;
}



void Sistema::despegar(const Drone & d) //hay un requiere que dice que d debe pertenecer a Enjambre y que existe al menos una parcela libre.
{
	 Posicion p = posiblePosicionLibre(*this)[0];//estaria bueno que elija una posicion aleatoriamente...
	 for (unsigned int i = 0; i < enjambreDrones().size(); ++i){
		if(enjambreDrones()[i] == d){
			_enjambre[i].moverA(p);		//falta implementar moverA
	 	}
	 }
}
//AUXILIARES
Secuencia<Posicion> posiblePosicionLibre(const Sistema &s)
{
	Secuencia<Posicion> ps;
	Posicion pos;
	Posicion p = posDelGranero(s.campo());
	for (unsigned int i = s.campo().dimensiones().largo; i >= 0; --i){
		for (unsigned int j = 0; j < s.campo().dimensiones().ancho; ++j){
			pos.y = j;
			pos.x = i;
			bool esVecino = (((pos.y -1 == p.y)||(pos.y == p.y)||(pos.y+1 == p.y)) &&((pos.x == p.x)||(pos.x+1 == p.x)||(pos.x-1 == p.x)));//se podria mejorar, implementado la funcion distancia!
			if (esVecino && estaLibre(s, pos)){
				ps.push_back(pos);
			}
		}
	}
	return ps;
}
//AUXILIARES
bool estaLibre(const Sistema &s, const Posicion &p) //se fija si una posicion 'p' esta libre de drones.
{
	bool libre = true;
	for (unsigned int i = 0; i < s.enjambreDrones().size(); ++i){
		Posicion pos = s.enjambreDrones()[i].posicionActual();
		if ((pos.x == p.x) && (pos.y == p.y)){
			libre = false;
			break;
		}
	}
	return libre;
}
//AUXILIARES
Posicion posDelGranero(const Campo &c);
{
	Posicion p;
	for (unsigned int i = c.dimensiones().largo; i >= 0; --i){
		for (unsigned int j = 0; j < c.dimensiones().ancho; ++j){
			Posicion pos;
			pos.y = j;
			pos.x = i;
			if (c.contenido(pos) == Granero){
				p = pos;
				break;
			}
		}
	}
	return p;
}

bool Sistema::listoParaCosechar() const //de este ejercicio me encargo yo asi hago el teorema del INVARIANTE y todo eso.
{
	bool res = ((cantCultivosCosechables(*this))/(parcelasCultivo(campo()).size())) >= 0.9;
	return res;
}
//AUXILIAR
Secuencia<Posicion> parcelasCultivo(Campo c)
{
	Secuencia<Posicion> ts;
	unsigned int i = 0;
	unsigned int j = 0;
	while (j < c.dimensiones().largo && i < c.dimensiones().ancho){
		Posicion p;
		p.y = j;
		p.x = i;
		if (c.contenido(p) == Cultivo){
			ts.push_back(p);
		}
		if (j == 0){
			j = 0;
			i++;
		}
		else {
			j++;
		}
	}
	return ts;
}

//AUXILIAR
int cantCultivosCosechables(const Sistema &s);
{
	int res = 0;
	unsigned int i = 0;
	while (i < parcelasCultivo(s.campo()).size()){
		Posicion p = parcelasCultivo(s.campo())[i];
		if (s.estadoDelCultivo(p) == ListoParaCosechar){ //error expected primary-expression before "=="...
			res++;
			}
		i++;
	}
	return res;
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
	this->_campo.mostrar(os);

	// os << "Representacion estados del cultivo. Notacion: -(No es cultivo), NS(NoSensado)"
	// for (int i = 0; i < this->_campo.dimensiones().ancho; i++) {
	// 	for (int j = 0; j < this->_campo.dimensiones().largo; j++) {
	// 		Posicion posicionActual;
	// 		posicionActual.x = i;
	// 		posicionActual.y = j;
	// 		if (this->_campo.contenido(posicionActual) == Cultivo) {
	// 			_estado.parcelas[i][j] = NoSensado;
	// 		}
	// 	}
	// }

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
	// TODO: insert return statement here
	return os;
}
