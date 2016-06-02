#include "drone.h"
#include <cstdlib>
#include <iostream>
#include "campo.h"

Drone::Drone()
{
}

Drone::Drone(ID i, const std::vector<Producto>& ps)
{
	this->_id = i;
	this->_productos = ps;
	this->_bateria = 100;
	this->_enVuelo = false;
	this->_trayectoria = Secuencia<Posicion>();

	// srand(time(NULL));
	// Posicion pos0;
	// pos0.x = 0;
	// pos0.y = 1;
	// this->_trayectoria[0] = pos0;
	//
	// for (int i = 1; i < 5; i++) {
	// 	Posicion posicion;
	// 	switch (rand() % 2) {
	// 		case 0:
	// 			posicion.x = this->_trayectoria[i-1].x + 1;
	// 			posicion.y = this->_trayectoria[i-1].y;
	// 		break;
	// 		case 1:
	// 			posicion.y = this->_trayectoria[i-1].y + 1;
	// 			posicion.x = this->_trayectoria[i-1].x;
	// 		break;
	// 	}
	// 	this->_trayectoria[i] = posicion;
	// }
}

ID Drone::id() const
{
	return this->_id;
}

Carga Drone::bateria() const
{
	return this->_bateria;
}

bool Drone::enVuelo() const
{
	return this->_enVuelo;
}

const Secuencia<Posicion>& Drone::vueloRealizado() const
{
	return this->_trayectoria;
}

Posicion Drone::posicionActual() const
{
	return this->_trayectoria.back();
}

const Secuencia<Producto>& Drone::productosDisponibles() const
{
	return this->_productos;
}

bool Drone::vueloEscalerado() const
{
	bool escalerado = true;
	if (this->_trayectoria.size() >= 2) {
		for (int i = 0; i < (this->_trayectoria.size()-2); i++) {
			int deltaX = this->_trayectoria[i].x - this->_trayectoria [i+2].x;
			int deltaY = this->_trayectoria[i].y - this->_trayectoria [i+2].y;
			if ((deltaX != 1) && (deltaX != -1)) escalerado = false;
			if ((deltaX != 1) && (deltaX != -1)) escalerado = false;
		}
	}

	return enVuelo() && escalerado;
}

Secuencia<InfoVueloCruzado> Drone::vuelosCruzados(const Secuencia<Drone>& ds)
{
	return Secuencia<InfoVueloCruzado>();
}

void Drone::mostrar(std::ostream & os) const
{
	os << "Id del Drone: " << id() << " | Su bateria es: " << bateria();
	os << " | Trayectoria realizada: [ ";
	for (int i = 0; i < vueloRealizado().size(); ++i) {
		if (i == vueloRealizado().size() -1) {
			os << "[" << vueloRealizado()[i].x << "," << vueloRealizado()[i].y << "] ]";
			break;
		}
		os << "[" << vueloRealizado()[i].x << "," << vueloRealizado()[i].y << "] , ";
	}

	os << " | Lista de Productos:\n";
	for (int j = 0; j < productosDisponibles().size(); ++j) {
		os << productosDisponibles()[j];
		os << "\n";
	}
}

void Drone::guardar(std::ostream & os) const
{
	os << "{ D " << id() << " "<< bateria() << " " << "[";
	for (int i = 0; i < vueloRealizado().size(); ++i) {
		if (i == vueloRealizado().size() -1) {
			os << "[" << vueloRealizado()[i].x << "," << vueloRealizado()[i].y << "]]";
			break;
		}
		os << "[" << vueloRealizado()[i].x << "," << vueloRealizado()[i].y << "] , ";
	}
	os << " [";
	for (int j = 0; j < productosDisponibles().size(); ++j) {
		if (j == productosDisponibles().size() -1){
			os << productosDisponibles()[j] << "]}";
			break;
		}
		os << productosDisponibles()[j] << ", ";
	}
}

void Drone::cargar(std::istream & is)
{
}

void Drone::moverA(const Posicion pos)
{
	//o uso _cambiarPosicionActual????
	this->_trayectoria.push_back(pos);
}

void Drone::setBateria(const Carga c)
{
	this->_bateria = c;
}

void Drone::borrarVueloRealizado()
{//digo algo de enVuelo??
	this->_enVuelo = false;
	this->_trayectoria.clear();
}

void Drone::cambiarPosicionActual(const Posicion p)
{//requiere que no este en vuelo. No se entiende la especificacion...
	this->_posicionActual = p;
}

void Drone::sacarProducto(const Producto p)
{
	for (int i = 0; i < this->_productos.size(); ++i)
	{
		if (this->_productos[i] == p)
		{
			this->_productos.erase(ps.begin()+i);
			break;
		}
	}
}

bool Drone::operator==(const Drone & otroDrone) const
{
	bool mismosProductos = false;
	bool igualRecorrido = false;
	if((id() == otroDrone.id()) && (bateria() == otroDrone.bateria()) && (enVuelo() == otroDrone.enVuelo()) && (productosDisponibles().size() == otroDrone.productosDisponibles().size())){
		//miro que tenga los MISMOS productos.
		for (int j = 0; j < productosDisponibles().size(); ++j){
			if ((j > 0) && (mismosProductos == false)) break;
			for (int i = 0; i < otroDrone.productosDisponibles().size(); ++i){
				if (productosDisponibles()[j] == otroDrone.productosDisponibles()[i]){
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

std::ostream & operator<<(std::ostream & os, const Drone & d)
{
	d.mostrar(os);
	return os;
}
