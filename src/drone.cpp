#include "drone.h"
#include <cstdlib>
#include <iostream>
#include "campo.h"
#include <algorithm>
#include "aux.h"

Drone::Drone()
{
	this->_id = rand() % 200;
	this->_productos = {Fertilizante, Fertilizante, PlaguicidaBajoConsumo};
	this->_bateria = 100;
	this->_enVuelo = false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
Drone::Drone(ID i, const std::vector<Producto>& ps)
{
	this->_id = i;
	this->_productos = ps;
	this->_bateria = 100;
	this->_enVuelo = false;
	// this->_trayectoria = tr;

	 /*srand(time(NULL));
	 Posicion pos0;
	 pos0.x = 0;
	 pos0.y = 1;
	 this->_trayectoria[0] = pos0;

	 for (int i = 1; i < 5; i++) {
	 	Posicion posicion;
	 	switch (rand() % 2) {
	 		case 0:
	 			posicion.x = this->_trayectoria[i-1].x + 1;
	 			posicion.y = this->_trayectoria[i-1].y;
	 		break;
	 		case 1:
	 			posicion.y = this->_trayectoria[i-1].y + 1;
	 			posicion.x = this->_trayectoria[i-1].x;
	 		break;
	 	}
	 	this->_trayectoria[i] = posicion;
	}*/
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

ID Drone::id() const
{
	return this->_id;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

Carga Drone::bateria() const
{
	return this->_bateria;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

bool Drone::enVuelo() const
{
	return this->_enVuelo;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

const Secuencia<Posicion>& Drone::vueloRealizado() const
{
	return this->_trayectoria;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

Posicion Drone::posicionActual() const
{
	return this->_trayectoria.back();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

const Secuencia<Producto>& Drone::productosDisponibles() const
{
	return this->_productos;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

bool Drone::vueloEscalerado() const
{
	bool escalerado = true;
	if (vueloRealizado().size() >= 2) {
		int i = 0;
		while (i < vueloRealizado().size() - 2) {
			int deltaX = vueloRealizado()[i].x - vueloRealizado() [i+2].x;
			int deltaY = vueloRealizado()[i].y - vueloRealizado() [i+2].y;
			if ((deltaX != 1) && (deltaX != -1)) escalerado = false;
			if ((deltaX != 1) && (deltaX != -1)) escalerado = false;
			i++;
		}
	}
	return enVuelo() && escalerado;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

Secuencia<InfoVueloCruzado> Drone::vuelosCruzados(const Secuencia<Drone>& ds)
{
	return Secuencia<InfoVueloCruzado>();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void Drone::mostrar(std::ostream & os) const
{
	os << "ID: " << id() << " | Bateria: " << bateria();
	os << " | Trayectoria: [";
	if (vueloRealizado().size() > 0) os << vueloRealizado()[0];
	for (int i = 1; i < vueloRealizado().size(); ++i) {
		os << "," << vueloRealizado()[i];
	}
	os << "] | Productos: [";
	if (productosDisponibles().size() > 0) os << productosDisponibles()[0];
	for (int j = 1; j < productosDisponibles().size(); ++j) {
		os << "," << productosDisponibles()[j];
	}
	os << "] \n";
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void Drone::guardar(std::ostream & os) const
{
	os << "{ D " << id() << " "<< bateria() << " " << "[";

	if (vueloRealizado().size() > 0) os << vueloRealizado()[0];
	for (int i = 1; i < vueloRealizado().size(); ++i) {
		os << "," << vueloRealizado()[i];
	}
	os << "] [";

	if (productosDisponibles().size() > 0) os << productosDisponibles()[0];
	for (int j = 1; j < productosDisponibles().size(); ++j) {
		os << "," << productosDisponibles()[j];
	}
	os << "] " << (enVuelo()? "true" : "false") << " " << posicionActual()  << "}"; // ROMPE SI NO HAY POSICIONACTUAL
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

// Recibe un string de forma "[x,y]" (o sin los corchetes) y devuelve una Posicion {x,y}
Posicion split(const std::string &text){
	// MEJORA: Si viene con espacios se rompe
  int start = text.find('[') != std::string::npos ? text.find('[') : 0;
  int end = text.find(']') != std::string::npos ? text.find(']') : text.size();
  int sep = text.find(',');
	std::string x = text.substr(start + 1, sep - 1);
	std::string y = text.substr(sep + 1, end - 1);
	return {std::stoi(x), std::stoi(y)};
}

void Drone::cargar(std::istream & is)
{
		std::string contenido, id, bateria;
		std::getline(is, contenido, ' ');
		std::getline(is, contenido, ' ');
		std::getline(is, id, ' ');
		std::getline(is, bateria, ' ');
		std::getline(is, contenido, '[');
		Secuencia<Posicion> trayectoria;
		while(contenido[0] != ']'){
			std::string posicion;
			std::getline(is, posicion, ']');
			trayectoria.push_back(split(posicion));
			std::getline(is, contenido);
		}

		this->_trayectoria = trayectoria;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void Drone::moverA(const Posicion pos)
{
	//o uso _cambiarPosicionActual????
	this->_enVuelo = true;
	this->_trayectoria.push_back(pos);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void Drone::setBateria(const Carga c)
{
	this->_bateria = c;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void Drone::borrarVueloRealizado()
{//digo algo de enVuelo??
	this->_enVuelo = false;
	this->_trayectoria.clear();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void Drone::cambiarPosicionActual(const Posicion p)
{//requiere que no este en vuelo. No se entiende la especificacion...
	this->_posicionActual = p;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void Drone::sacarProducto(const Producto p)
{
	for (int i = 0; i < this->_productos.size(); ++i)
	{
		if (this->_productos[i] == p)
		{
			this->_productos.erase(this->_productos.begin()+i);
			break;
		}
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

bool Drone::operator==(const Drone & otroDrone) const
{
	bool mismosProductos = false;
	bool igualRecorrido = false;
	if((id() == otroDrone.id()) && (bateria() == otroDrone.bateria()) && (enVuelo() == otroDrone.enVuelo()) && (productosDisponibles().size() == otroDrone.productosDisponibles().size())){
		//miro que tenga los MISMOS productos.
		for (int j = 0; j < productosDisponibles().size(); ++j){
			if((mismosProductos = false) && (j>0)) break;
			for (int i = 0; i < otroDrone.productosDisponibles().size(); ++i){
				if ((productosDisponibles()[j] == otroDrone.productosDisponibles()[i]) && (cuentaDrones(productosDisponibles()[j], productosDisponibles()) == cuentaDrones(otroDrone.productosDisponibles()[i], otroDrone.productosDisponibles()))){
					mismosProductos = true;
					break;
				}
				else {
					mismosProductos = false;
				}
			}
		}
		//miro que tenga exactamente el mismo recorrido
		for (int j = 0; j < vueloRealizado().size(); ++j){
			if ((j > 0) && (igualRecorrido == false)) break;
			for (int i = 0; i < otroDrone.vueloRealizado().size(); ++i){
				if ((vueloRealizado()[j].x == otroDrone.vueloRealizado()[i].x) && (vueloRealizado()[j].y == otroDrone.vueloRealizado()[i].y)){
					igualRecorrido = true;
					break;
				}
			}
		}
		return (mismosProductos && igualRecorrido);
	}
}
//AUXILIARES
int cuentaDrones(const Producto p, const Secuencia<Producto> ps)
{
		int count = 0;
		for (int i = 0; i < ps.size();++i){
				if(ps[i] == p){
					count++;
				}
		}
		return count;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

std::ostream & operator<<(std::ostream & os, const Drone & d)
{
	d.mostrar(os);
	return os;
}
