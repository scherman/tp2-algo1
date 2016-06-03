#include "sistema.h"
#include <algorithm>

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

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

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

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

const Campo & Sistema::campo() const
{
	return this->_campo;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

EstadoCultivo Sistema::estadoDelCultivo(const Posicion & p) const
{
		return this->_estado.parcelas[p.x][p.y];
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

const Secuencia<Drone>& Sistema::enjambreDrones() const
{
	return this->_enjambre;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

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
					this->_estado.parcelas[p.x][p.y] = b;
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void Sistema::seVinoLaMaleza(const Secuencia<Posicion>& ps)
{
	//en la especificacion se REQUIERE que la lista ps solo contenga Parcelas de Cultivo...
	EstadoCultivo c = ConMaleza;
	for (unsigned int i = 0; i < ps.size(); ++i){
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
bool vecinoConPlaga(const Sistema &s, const Posicion &p) //le pongo const a los parametros o no??
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

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void Sistema::despegar(const Drone & d) //hay un requiere que dice que d debe pertenecer a Enjambre y que existe al menos una parcela libre.
{
	srand(time(NULL));
	int x = rand() % (posiblePosicionLibre(*this).size());
	Posicion p = posiblePosicionLibre(*this)[x];//estaria bueno que elija una posicion aleatoriamente...
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
		if ((pos.x == p.x) && (pos.y == p.y)){ //modificar luego de que tengamos el OPERADOR pos == pos1;
			libre = false;
			break;
		}
	}
	return libre;
}
//AUXILIARES
Posicion posDelGranero(const Campo &c)
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

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

bool Sistema::listoParaCosechar() const //de este ejercicio me encargo yo asi hago el teorema del INVARIANTE y todo eso.
{
	bool res = ((cantCultivosCosechables(*this))/(parcelasCultivo(campo()).size())) >= 0.9;
	return res;
}
//AUXILIAR
Secuencia<Posicion> parcelasCultivo(const Campo &c)
{
	Secuencia<Posicion> ts = {};
	int i = 0;
	int j = 0;
	while ((j < c.dimensiones().largo) && (i < c.dimensiones().ancho)){
		Posicion p = {i,j};
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
int cantCultivosCosechables(const Sistema &s)
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

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void Sistema::aterrizarYCargarBaterias(Carga b)
//requiere 0<=b<=100
{
	Carga c = 100;
	Campo campo = this->_campo;
	Posicion posG = posDelGranero(campo);
	for (int i = 0; i < enjambreDrones().size(); ++i)
	{
		if (enjambreDrones()[i].bateria() < b)
		{
			_enjambre[i].setBateria(c);
			_enjambre[i].borrarVueloRealizado();
			_enjambre[i].cambiarPosicionActual(posG);
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void Sistema::fertilizarPorFilas()
//requiere que aLoSumoUnDroneVolandoPorFila
{
	//modifico los drones del sistema...
	for (int i = 0; i < enjambreDrones().size(); ++i)
	{
		int viaje = recorridoMaximo(*this, enjambreDrones()[i]);
		if (enjambreDrones()[i].enVuelo() == true)
		{
			int c = enjambreDrones()[i].bateria() - viaje;
			Posicion p = {enjambreDrones()[i].posicionActual().x - viaje, enjambreDrones()[i].posicionActual().y};
			this->_enjambre[i].moverA(p);
			this->_enjambre[i].cambiarPosicionActual(p);
			this->_enjambre[i].setBateria(c);

			//modifico el VueloRealizado...
			
			for (int j = 1; j < viaje; ++j)
			{
				Posicion pos =  {enjambreDrones()[i].posicionActual().x - j, enjambreDrones()[i].posicionActual().y};
				this->_enjambre[i].moverA(pos);
			}
			//modifico ProductosDisponibles...
			int h = 0;
			int k = 0;
			Producto pr = Fertilizante;
			while ((h < parcelasFertilizables(*this, i, viaje)) && (k < enjambreDrones()[i].productosDisponibles().size()))
			{
				if (enjambreDrones()[i].productosDisponibles()[k] == Fertilizante)
				{
					this->_enjambre[i].sacarProducto(pr);
					h++;
					k--;
				}
				k++;
			}
		}

		for (int h = 0; h < campo().dimensiones().largo; ++h)
		{
			for (int j = 0; j < campo().dimensiones().ancho; ++j)
			{
				Posicion pos = {j,h};
				if (enjambreDrones()[i].posicionActual() == pos)
				{
					for (int m = 0; m < viaje; m++)
					{
						Posicion ps = {j-m, h};
						if ((estadoDelCultivo(ps) == EnCrecimiento) || (estadoDelCultivo(ps) == RecienSembrado))
						{
							this->_estado.parcelas[ps.x][ps.y] = ListoParaCosechar;
						}
					}
					//aca podria poner un break; ya que por el requiere no debe haber mas de 1 drone por fila...
				}
			}
		}
	}

}


//AUXILIARES
int recorridoMaximo(const Sistema &s, const Drone &d)
{
	int viaje = 0;
	int h = d.posicionActual().x;
	int k = d.posicionActual().y;
	int carga = d.bateria();
	int totalFertilizantes = cantFertilizantes(d);
	for (int i = h; i >=0; --i)
	{
		Posicion p = {i, k};
		if ((s.campo().contenido(p) != Cultivo) || (totalFertilizantes == 0) || (carga == 0))
		{
			viaje = h - i;
			break;
		}
		else
		{
			carga = carga - 1;
			if ((s.estadoDelCultivo(p) == EnCrecimiento) || (s.estadoDelCultivo(p) == RecienSembrado))
			{
				totalFertilizantes = totalFertilizantes -1;
			}
		}
	}
	return viaje;
}
//AUXILIAR
int cantFertilizantes(const Drone &d)
{
	int totalFertilizantes = 0;
	for (unsigned int i = 0; i < d.productosDisponibles().size(); ++i)
	{
		if (d.productosDisponibles()[i] == Fertilizante)
		{
			totalFertilizantes++;
		}
	}
	return totalFertilizantes;
}

int parcelasFertilizables(const Sistema &s, const int &i, const int &viaje)
{
	int cantFertilizables = 0;
	int h = s.enjambreDrones()[i].posicionActual().x;
	int k = s.enjambreDrones()[i].posicionActual().y;
	for (int j = h; j >= 0; j--)
	{
		Posicion p = {j, k};
		if ((s.estadoDelCultivo(p) == EnCrecimiento) || (s.estadoDelCultivo(p) == RecienSembrado))
		{
			cantFertilizables++;
		}
	}
	return cantFertilizables;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

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
	os << "{ S ";
	//puedo llamar directamente a guardar del CAMPO?? campo().guardar(os)...
	campo().guardar(os);
	//puedo llamar directamente a guardar del DRONE?? enjambreDrones()[i].guardar(os)...
	os << " [";
	for (unsigned int i = 0; i < enjambreDrones().size();++i)
	{
		if (i != enjambreDrones().size() - 1){
			enjambreDrones()[i].guardar(os);
			os << " , ";

		}
		else {
			enjambreDrones()[i].guardar(os);
			os << " ]";
		}
	}
	//guardo lo de SISTEMA
	os << " [";
	for (int i = campo().dimensiones().largo -1; i >= 0; --i)
	{
		os << "[" << estadoDelCultivo({i,0});
		for (int j = 1; j < campo().dimensiones().ancho -1; ++j)
		{
				os << ", " << estadoDelCultivo({i,j});
		}
		os << "],";
	}
	os << " ]}";
}

void Sistema::cargar(std::istream & is)
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//AUXILIARES
bool cuentaDron(const Drone d, const Secuencia<Drone> ds)
{
	bool count = 0;
	for(int i = 0; i < ds.size(); ++i){
		if (ds[i] == d) ++count;
	}
	return count;
}

//AUXILIARES
bool mismosDrones(const Secuencia<Drone> ps, const Secuencia<Drone> ds)
{
	bool mismos = false;
	if (ps.size() == ds.size()){
		for (int i = 0; i < ps.size(); ++i){
			if ((i > 0) && (mismos == false)) break;
			for (int j = 0; j < ds.size(); ++j){
				if((ps[i] == ds[j]) && (cuentaDron(ps[i], ps) == cuentaDron(ds[j], ds))){
					mismos = true;
				}
			}
		}
	}
	return mismos;
}
bool Sistema::operator==(const Sistema & otroSistema) const
{
	bool mismoCampo = true;
	if(!(campo() == otroSistema.campo())) return false;

	bool mismosAviones = true;
	if (!(mismosDrones(enjambreDrones(), otroSistema.enjambreDrones()))) return false;

	bool mismosEstados = true;
	for(int i = 0; i < parcelasCultivo(campo()).size(); ++i){
		if(estadoDelCultivo(parcelasCultivo(campo())[i]) != estadoDelCultivo(parcelasCultivo(otroSistema.campo())[i])) return false;
	}
	return (mismoCampo && mismosAviones && mismosEstados);
}



//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

std::ostream & operator<<(std::ostream & os, const Sistema & s)
{
	// TODO: insert return statement here
	return os;
}
