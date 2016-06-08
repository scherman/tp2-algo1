#include "sistema.h"
#include <algorithm>
#include <sstream>

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
	int x = rand() % (posiblePosicionLibre(*this, posDelGranero(campo()))).size();
	Posicion p = posiblePosicionLibre(*this, posDelGranero(campo()))[x];
	for (unsigned int i = 0; i < enjambreDrones().size(); ++i){
		if(enjambreDrones()[i] == d){
			_enjambre[i].moverA(p);
	 	}
	 }
}
//AUXILIARES
Secuencia<Posicion> posiblePosicionLibre(const Sistema &s, const Posicion &pos)
{
	Secuencia<Posicion> ps;
	Posicion p;
	for (unsigned int i = s.campo().dimensiones().largo; i >= 0; --i){
		for (unsigned int j = 0; j < s.campo().dimensiones().ancho; ++j){
			p.y = j;
			p.x = i;
			bool esVecino = (((p.y -1 == pos.y)||(p.y == pos.y)||(p.y+1 == pos.y)) &&((p.x == pos.x)||(p.x+1 == pos.x)||(p.x-1 == pos.x)));
			if (esVecino && estaLibre(s, p)){
				ps.push_back(p);
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
	int i = 0;
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
			int j = 1;
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
//requiere perteneceDrone(d, enjambreDrones(this)) ;
//requiere bateria(d) > 0 ;
//requiere hayParcelaLibre
{
	int sub;
	srand(time(NULL));
	int x = rand() % (posiblePosicionLibre(*this, d.posicionActual()).size());
	Posicion p = posiblePosicionLibre(*this, d.posicionActual())[x];
	for (unsigned int i = 0; i < enjambreDrones().size(); ++i){
		if(enjambreDrones()[i] == d){
			sub = i;
			_enjambre[i].moverA(p);
			Carga c = enjambreDrones()[i].bateria() - 1;
			_enjambre[i].setBateria(c);
			break;
		}
	}
	Posicion pos = enjambreDrones()[sub].posicionActual();
//miro si no estaSensado
	if(estadoDelCultivo(pos) == NoSensado){
		this->_estado.parcelas[pos.x][pos.y] = estaSensado(*this, pos);
	}


//veo que pasa si estadoDelCultivo es ConMaleza.....
	Secuencia<Producto> herbicidas = {Herbicida, HerbicidaLargoAlcance};
	//aca deberia preguntar que pasa si es HerbicidaLargoAlcance, deberia modificar las parcelas adyacentes que tambien tengan maleza....
	if((estadoDelCultivo(pos) == ConMaleza) && (enjambreDrones()[sub].bateria() >= 5)){
		//veo para cada caso que Herbicida tengo disponible.
		if((perteneceA(Herbicida, enjambreDrones()[sub].productosDisponibles())) && (perteneceA(HerbicidaLargoAlcance, enjambreDrones()[sub].productosDisponibles()))){
			int l = rand() % 2;
			Carga d = enjambreDrones()[sub].bateria() - 5;
			_enjambre[sub].setBateria(d);
			this->_estado.parcelas[pos.x][pos.y] = RecienSembrado;
			if(l==0){
				this->_enjambre[sub].sacarProducto(herbicidas[0]);
			}
			else{//aca deberia preguntar que pasa si es HerbicidaLargoAlcance, deberia modificar las parcelas adyacentes que tambien tengan maleza....
				this->_enjambre[sub].sacarProducto(herbicidas[1]);
			}
		}

		if(perteneceA(Herbicida, enjambreDrones()[sub].productosDisponibles()) && (!perteneceA(HerbicidaLargoAlcance, enjambreDrones()[sub].productosDisponibles()))){
			Carga d = enjambreDrones()[sub].bateria() - 5;
			_enjambre[sub].setBateria(d);
			this->_estado.parcelas[pos.x][pos.y] = RecienSembrado;
			this->_enjambre[sub].sacarProducto(herbicidas[0]);
		}
		if(perteneceA(HerbicidaLargoAlcance, enjambreDrones()[sub].productosDisponibles()) && (!perteneceA(Herbicida, enjambreDrones()[sub].productosDisponibles()))){
			Carga d = enjambreDrones()[sub].bateria() - 5;
			_enjambre[sub].setBateria(d);
			this->_estado.parcelas[pos.x][pos.y] = RecienSembrado;
			this->_enjambre[sub].sacarProducto(herbicidas[1]);
		}
	}

//veo que pasa si estadoDelCultivo es ConPlaga...
	Secuencia<Producto> plaguicidas = {Plaguicida, PlaguicidaBajoConsumo};
	if((estadoDelCultivo(pos) == ConPlaga) && (enjambreDrones()[sub].bateria() >= 5)){
		if((perteneceA(Plaguicida, enjambreDrones()[sub].productosDisponibles())) && (perteneceA(PlaguicidaBajoConsumo, enjambreDrones()[sub].productosDisponibles()))){
			if(enjambreDrones()[sub].bateria() >= 10){
				int k = rand() % 2;
				if(k == 0){
					Carga cr = enjambreDrones()[sub].bateria() - 10;
					_enjambre[sub].setBateria(cr);
					this->_estado.parcelas[pos.x][pos.y] = RecienSembrado;
					this->_enjambre[sub].sacarProducto(plaguicidas[0]);
				}
				else{
					Carga carga = enjambreDrones()[sub].bateria() - 5;
					_enjambre[sub].setBateria(carga);
					this->_estado.parcelas[pos.x][pos.y] = RecienSembrado;
					this->_enjambre[sub].sacarProducto(plaguicidas[1]);
				}
			}
			else{
				Carga carga = enjambreDrones()[sub].bateria() - 5;
				_enjambre[sub].setBateria(carga);
				this->_estado.parcelas[pos.x][pos.y] = RecienSembrado;
				this->_enjambre[sub].sacarProducto(plaguicidas[1]);
			}
		}
		if((perteneceA(Plaguicida, enjambreDrones()[sub].productosDisponibles())) && (enjambreDrones()[sub].bateria() >= 10) && (!perteneceA(PlaguicidaBajoConsumo, enjambreDrones()[sub].productosDisponibles()))){
			Carga cr = enjambreDrones()[sub].bateria() - 10;
			_enjambre[sub].setBateria(cr);
			this->_estado.parcelas[pos.x][pos.y] = RecienSembrado;
			this->_enjambre[sub].sacarProducto(plaguicidas[0]);
		}
		if((!perteneceA(Plaguicida, enjambreDrones()[sub].productosDisponibles())) && (perteneceA(PlaguicidaBajoConsumo, enjambreDrones()[sub].productosDisponibles()))){
			Carga carga = enjambreDrones()[sub].bateria() - 5;
			_enjambre[sub].setBateria(carga);
			this->_estado.parcelas[pos.x][pos.y] = RecienSembrado;
			this->_enjambre[sub].sacarProducto(plaguicidas[1]);
		}
	}
}

//AUXILIARES
EstadoCultivo estaSensado(const Sistema &s, const Posicion &p)
{
	Secuencia<EstadoCultivo> estados = {RecienSembrado, EnCrecimiento, ListoParaCosechar, ConMaleza, ConPlaga};
	srand(time(NULL));
	int x = rand() % (estados.size());
	return estados[x];
}
//AUXILIARES
bool perteneceA(const Producto &p, const Secuencia<Producto> &ds)
{
	bool pert = false;
	for(unsigned int i = 0; i < ds.size(); i++){
		if(ds[i] == p) return true;
	}
	return pert;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void Sistema::mostrar(std::ostream & os) const
{
	os << "Representacion de sistema con campo de " << campo().dimensiones().ancho << "x" << campo().dimensiones().largo << ". Notacion: [Parcela/EstadoCultivo]/|drones| \n";
	os << "Parcelas: C(Casa), G(Granero), [estado](si es cultivo) \n";
	os << "Estados: EC(EnCrecimiento), LC(ListoParaCosechar), CM(ConMaleza), CP(ConPlaga), NS(NoSensado) \n";

	for (int j = campo().dimensiones().largo - 1; j >= 0; j--) {
		for (int i = 0; i < campo().dimensiones().ancho ; i++) {
			int cantDrones = 0;
			Posicion posActual = {i,j};
			for (int d = 0; d < enjambreDrones().size(); d++) {
				if (enjambreDrones()[d].posicionActual() == posActual) cantDrones++;
			}
			if (campo().contenido({i,j}) == Cultivo) {
				 os << estadoAbreviado(estadoDelCultivo({i,j}));
				if (cantDrones > 0) {
					os << "/" << cantDrones << "  ";
				}  else {
					os << "    ";
				}
			} else {
				os << nombreAbreviado(campo().contenido({i,j}));
				if (cantDrones > 0) {
					os << "/" << cantDrones << "  ";
				}  else {
					os << "     ";
				}
			}
		}
		os << "\n";
	}
}

//AUXILIARES
const std::string estadoAbreviado (const EstadoCultivo &estado) {
	switch (estado) {
		case RecienSembrado: return "RS";
		case EnCrecimiento: return "EC";
		case ListoParaCosechar: return "LC";
		case ConMaleza: return "CM";
		case ConPlaga: return "CP";
		case NoSensado: return "NS";
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void Sistema::guardar(std::ostream & os) const
{
	os << "{ S ";
	campo().guardar(os);
 	os << " [";
	if (enjambreDrones().size() > 0) enjambreDrones()[0].guardar(os);
	for (int i = 1; i < enjambreDrones().size();++i){
		os << ",";
 		enjambreDrones()[i].guardar(os);
	}
	os << "] [";
	for (int j = campo().dimensiones().largo - 1; j >= 0; j--) {
		os << "[" << estadoDelCultivo({0,j});
		for (int i = 1; i < campo().dimensiones().ancho; i++) {
			os << "," << estadoDelCultivo({i,j});
		}
		os << "]" << ((j==0)? "" : ",") ;
	}
	os << "]}";
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void Sistema::cargar(std::istream & is)
{
	std::string contenido;
	std::getline(is, contenido, ' ');
	std::getline(is, contenido, ' ');
	std::getline(is, contenido, '}');
	Campo campo;
	std::istringstream campoSerializadoStream(contenido);
	campo.cargar(campoSerializadoStream);
	this->_campo = campo;

	Secuencia<Drone> drones;
	auto pos = is.tellg();    // Leer posicion actual
	while (std::getline(is,contenido,'{') && !is.eof()) {
	    std::getline(is,contenido,'}');
			Drone drone;
			std::istringstream droneSerializadoStream(contenido);
			drone.cargar(droneSerializadoStream);
			drones.push_back(drone);
	    pos = is.tellg();     // Actualizar posicoin antes de iterar de nuevo
	}
	is.seekg(pos); // Cambiar posicion actual
	this->_enjambre = drones;

	std::getline(is, contenido, '[');
	for (int j = campo.dimensiones().largo - 1; j >= 0; j--) {
		std::getline(is, contenido, '[');
		std::string fila;
		std::getline(is, fila, ']');
		std::vector<EstadoCultivo> columnas = splitEstados(fila, ',');
		for (int i = 0; i < campo.dimensiones().ancho; i++) {
					this->_estado.parcelas[i][j] = columnas[i];
		}
	}
}

//AUXILIARES
std::vector<EstadoCultivo> splitEstados(const std::string &text, char sep) {
  std::vector<EstadoCultivo> tokens;
  std::size_t start = 0, end = 0;
  while ((end = text.find(sep, start)) != std::string::npos) {
		std::string value = text.substr(start, end - start);
		tokens.push_back(aEstadoCultivo(value));
    start = end + 1;
  }
  tokens.push_back(aEstadoCultivo(text.substr(start)));
  return tokens;
}
//AUXILIARES
const EstadoCultivo aEstadoCultivo (const std::string &text) {
	if (text.find("RecienSembrado") != std::string::npos) return RecienSembrado;
	if (text.find("EnCrecimiento") != std::string::npos) return EnCrecimiento;
	if (text.find("ListoParaCosechar") != std::string::npos) return ListoParaCosechar;
	if (text.find("ConMaleza") != std::string::npos) return ConMaleza;
	if (text.find("ConPlaga") != std::string::npos) return ConPlaga;
	if (text.find("NoSensado") != std::string::npos) return NoSensado;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

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

//AUXILIARES
bool cuentaDron(const Drone d, const Secuencia<Drone> ds)
{
	bool count = 0;
	for(int i = 0; i < ds.size(); ++i){
		if (ds[i] == d) count;
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

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------//

std::ostream & operator<<(std::ostream & os, const Sistema & s)
{
	s.mostrar(os);
	return os;
}
