#include "drone.h"
#include <cstdlib>
#include <iostream>
#include "campo.h"
#include <algorithm>
#include "auxiliares.h"


Drone::Drone()
{
	this->_id = rand() % 200;
	this->_productos = {Fertilizante, Fertilizante, PlaguicidaBajoConsumo};
	this->_bateria = 100;
	this->_enVuelo = false;
	this->_trayectoria = Secuencia<Posicion>();
	this->_posicionActual = {0,0};
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
Drone::Drone(ID i, const std::vector<Producto>& ps)
{
	this->_id = i;
	this->_productos = ps;
	this->_bateria = 100;
	this->_enVuelo = false;
	this->_trayectoria = Secuencia<Posicion>();
	this->_posicionActual = {0,0};
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
	return this->_posicionActual;
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
{//requiere que todos los drones tengan la misma longitud de vueloRealizado
	if(ds.size()==0) return {};
	//el if es por que si me dan una secuencia vacia se rompe al pedir abajo, ds[0]..-
	Secuencia<InfoVueloCruzado> infoVuelos = {};
	Posicion p = {};
	Secuencia<Posicion> todasLasPos = todasLasPosSinRepetir(ds);
	int cantDrones = 0;
	int k = 0;
	while(k < ds[0].vueloRealizado().size()){
		for(int i = 0; i < todasLasPos.size(); i++){
			//miro todasLasPos de los drones que me pasen y me voy fijando cuantos drones en el momento "k" estuvieron en esa posicion.
			//Siempre habra al menos uno, si hay mas de uno, es un cruce.
			for(int j = 0; j < ds.size(); ++j){
				if(ds[j].vueloRealizado()[k] == todasLasPos[i]){
					cantDrones++;
				}
			}
			if(cantDrones > 1){
				//InfoVueloCruzado es un struct definido en tipos.h
				InfoVueloCruzado info;
				info.posicion =  todasLasPos[i];
				info.cantidadCruces = cantDrones;
				infoVuelos.push_back(info);
			}
			cantDrones = 0;
		}
	}
	return infoVuelos;
}

//AUXILIARES
Secuencia<Posicion> todasLasPosSinRepetir(const Secuencia<Drone> &ds)
{
	Secuencia<Posicion> todas = {};
	for(int i = 0; i < ds.size(); ++i){
		for(int j = 0; j < ds[0].vueloRealizado().size(); ++j){
			if(noPertenece(ds[i].vueloRealizado()[j], todas)){
				Posicion pos = ds[i].vueloRealizado()[j];
				todas.push_back(pos);
			}
		}
	}
	return todas;
}
//AUXILIARES
bool noPertenece(Posicion p, Secuencia<Posicion> ps){
	bool noPert = true;
	for(int i = 0; i < ps.size(); ++i){
		if(ps[i] == p) return false;
	}
	return noPert;
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
	os << "] " << (enVuelo()? "true" : "false") << " " << posicionActual()  << "}";
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

// Recibe un string de forma "[x,y]" (o sin los corchetes) y devuelve una Posicion {x,y}
Posicion splitPosicion(const std::string &posicion){
	// MEJORA: Si viene con espacios se rompe
	std::string parseado = posicion;
	parseado = posicion.find('[') != std::string::npos ? posicion.substr(posicion.find('[') + 1, -1) : parseado;
	parseado = posicion.find(']') != std::string::npos ? posicion.substr(0, posicion.find(']') - 1) : parseado;
  int sep = parseado.find(',');
	std::string x = parseado.substr(0, sep);
	std::string y = parseado.substr(sep + 1, -1);
	return {std::stoi(x), std::stoi(y)};
}

const Producto aProducto (const std::string &producto) {
	if (producto.find("Fertilizante") != std::string::npos) return Fertilizante;
	if (producto.find("PlaguicidaBajoConsumo") != std::string::npos) return PlaguicidaBajoConsumo;
	if (producto.find("Plaguicida") != std::string::npos) return Plaguicida;
	if (producto.find("HerbicidaLargoAlcance") != std::string::npos) return HerbicidaLargoAlcance;
	if (producto.find("Herbicida") != std::string::npos) return Herbicida;
}

// Recibe un string de forma "Producto,Producto,Producto" (o sin los corchetes) y devuelve Secuencia<Producto>
Secuencia<Producto> splitProductos(const std::string &productos){
	// MEJORA: Si viene con espacios se rompe
	std::string parseado = productos;
	int sep = parseado.find(',');
	Secuencia<Producto> resultado;
	while (sep != std::string::npos) {
		Producto producto = aProducto(parseado.substr(0, sep));
		resultado.push_back(producto);
		parseado = parseado.substr(sep + 1, -1);
		sep = parseado.find(',');
	}
	resultado.push_back(aProducto(parseado.substr(0,-1)));
	return resultado;
}

void Drone::cargar(std::istream & is)
{
		std::string contenido, id, bateria, aux, productos, enVuelo, posicionActual;
		std::getline(is, contenido, ' ');
		std::getline(is, contenido, ' ');
		std::getline(is, id, ' ');
		this->_id = std::stoi(id);

		std::getline(is, bateria, ' ');
		this->_bateria = std::stoi(bateria);

		std::getline(is, contenido, '[');
		std::getline(is, aux, ']');
		if (aux.size() == 0)  {
			std::getline(is, aux, '[');
			std::getline(is, aux, ']');
		} else {
			while(aux.size() > 0 && contenido[0] != ']'){
				this->_trayectoria.push_back(splitPosicion(aux));
				std::getline(is, contenido, '[');
				std::getline(is, aux, ']');
			}
		}

		this->_productos = splitProductos(aux);

		std::getline(is, enVuelo, '[');
		this->_enVuelo = enVuelo.find("true") != std::string::npos ? true : false;

		std::getline(is, posicionActual, ']');
		this->_posicionActual = splitPosicion(posicionActual);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void Drone::moverA(const Posicion pos)
{
	this->cambiarPosicionActual(pos);
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
	this->_trayectoria = {};
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void Drone::cambiarPosicionActual(const Posicion p)
{//requiere que no este en vuelo. En la especificacion dice que vueloRealizado no cambia...
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

//AUXILIARES
int cantidadProducto (const Secuencia<Producto> & productos, const Producto & producto) {
	int cont = 0;
  for (int i = 0; i < productos.size(); i++) {
  	if (productos[i] == producto) cont++;
  }
	return cont;
}

//AUXILIARES
bool mismosProductos(const Secuencia<Producto> & productos1, const Secuencia<Producto> & productos2) {
	if (productos1.size() != productos2.size()) return false;
	for (int i = 0; i < productos1.size(); i++) {
		Producto producto = productos1[i];
		if (cantidadProducto(productos1, producto) != cantidadProducto(productos2, producto)) return false;
	}
	return true;
}

bool Drone::operator==(const Drone & otroDrone) const
{
	if (id() != otroDrone.id()) return false;
	if (bateria() != otroDrone.bateria()) return false;
	if (enVuelo() != otroDrone.enVuelo()) return false;
  if (vueloRealizado() != otroDrone.vueloRealizado()) return false;
	if (!mismosProductos(productosDisponibles(), otroDrone.productosDisponibles())) return false;
	if (!(posicionActual() == otroDrone.posicionActual())) return false;
	return true;
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
