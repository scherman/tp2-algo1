#define ALCANZABLE_EN 5 // Deberia ser 100 pero lo hice parametrizable para que sea mas facil ver la representacion por consola

#include <algorithm> // Para funcion max
#include "campo.h"

const char nombreAbreviado (const Parcela &parcela);

Campo::Campo()
{
	// Creo aleatoriamente un granero y una casa (me aseguro de que sean diferentes)
	srand(time(NULL));
	Posicion posG, posC;
	posG.x = rand() % (ALCANZABLE_EN + 1); // Random en [0..ALCANZABLE_EN] = [0..ALCANZABLE_EN+1)
	posG.y = rand() % (ALCANZABLE_EN + 1);
	do {
		posC.x = rand() % (ALCANZABLE_EN + 1);
		posC.y = rand() % (ALCANZABLE_EN + 1);
	} while(posC.x == posG.x && posC.y == posG.y);

	// REFACTORIZAR: Aca se deberia llamar al otro constructor enviando posG y posC pero todavia no se como hacerlo. Todo lo que sigue esta repetido

	// Dimensiono campo.
	// * Idea: Si a=ANCHO_MIN, t=TOPE_ANCHO y z=ALCANZABLE_EN
	// ** Ej1) Si a=z=5: rangoAncho<-[0..5] => dim.ancho = 6
	// ** Ej2) Si a=3, z=5: rangoAncho<-[0..3/4/5] => dim.ancho = 4/5/6
  // * En general, rangoAncho<-[0..t] => dim.ancho = t+1, donde t es un random en [a..z]
	const int ANCHO_MIN = std::max(posG.x, posC.x);
	const int LARGO_MIN = std::max(posG.y, posC.y);
	const int TOPE_ANCHO = rand() % (ALCANZABLE_EN + 1 - ANCHO_MIN) + ANCHO_MIN; // Random en [ANCHO_MIN..ALCANZABLE_EN] = [ANCHO_MIN..ALCANZABLE_EN+1)
	const int TOPE_LARGO = rand() % (ALCANZABLE_EN + 1 - LARGO_MIN) + LARGO_MIN;
	this->_dimension.ancho = TOPE_ANCHO + 1;
	this->_dimension.largo = TOPE_LARGO + 1;

	// Creo un campo, que viene con parcelas de cultivo por default, y ubico Casa y Granero en las parcelas correspondientes
	this->_grilla = Grilla<Parcela> (this->_dimension);
	this->_grilla.parcelas[posG.x][posG.y] = Granero;
	this->_grilla.parcelas[posC.x][posC.y] = Casa;
}

Campo::Campo(const Posicion &posG, const Posicion &posC)
{
	// Dimensiono campo.
	// * Idea: Si a=ANCHO_MIN, t=TOPE_ANCHO y z=ALCANZABLE_EN
	// ** Ej1) Si a=z=5: rangoAncho<-[0..5] => dim.ancho = 6
	// ** Ej2) Si a=3, z=5: rangoAncho<-[0..3/4/5] => dim.ancho = 4/5/6
  // * En general, rangoAncho<-[0..t] => dim.ancho = t+1, donde t es un random en [a..z]
	srand(time(NULL));
	const int ANCHO_MIN = std::max(posG.x, posC.x);
	const int LARGO_MIN = std::max(posG.y, posC.y);
	const int TOPE_ANCHO = rand() % (ALCANZABLE_EN + 1 - ANCHO_MIN) + ANCHO_MIN; // Random en [ANCHO_MIN..ALCANZABLE_EN] = [ANCHO_MIN..ALCANZABLE_EN+1)
	const int TOPE_LARGO = rand() % (ALCANZABLE_EN + 1 - LARGO_MIN) + LARGO_MIN;
	this->_dimension.ancho = TOPE_ANCHO + 1;
	this->_dimension.largo = TOPE_LARGO + 1;

	// Creo un campo, que viene con parcelas de cultivo por default, y ubico Casa y Granero en las parcelas correspondientes
	this->_grilla = Grilla<Parcela> (this->_dimension);
	this->_grilla.parcelas[posG.x][posG.y] = Granero;
	this->_grilla.parcelas[posC.x][posC.y] = Casa;
}

Dimension Campo::dimensiones() const
{
	return this->_dimension;
}

Parcela Campo::contenido(const Posicion & p) const
{
	return this->_grilla.parcelas[p.x][p.y];
}

void Campo::mostrar(std::ostream & os) const
{
	os << "Representacion de campo " << this->_dimension.ancho << "x" << this->_dimension.largo << ". Notacion: C(Casa), G(Granero), -(Cultivo)\n";
	for (int j = this->_dimension.largo - 1; j >= 0; j--) {
		for (int i = 0; i < this->_dimension.ancho ; i++) {
			os << nombreAbreviado(this->_grilla.parcelas[i][j]) << "  ";
		}
		os << "\n";
	}
}

const char nombreAbreviado (const Parcela &parcela) {
	switch (parcela) {
		case Cultivo: return '-';
		case Casa: return 'C';
		case Granero: return 'G';
	}
}


void Campo::guardar(std::ostream & os) const
{
	Posicion p;
	os << "{ C  " << "[" << this->_dimension.ancho << "," << this->_dimension.largo << "] [";
	for(int j = this->dimensiones().largo -1; j >= -1; --j){
		if(j == -1){
			os << "] }";
			break;
		}
		os << "[";
		for (int i = 0; i <this->dimensiones().ancho -1; ++i){
			p.y = j;
			p.x = i;
			if(i != this->dimensiones().ancho -1){
				os << this->contenido(p) << ",";
			}
			else{
				if (j == 0){
					os << this->contenido(p) << "]";
				}
				else {
					os << this->contenido(p) << "] , ";
				}
			}
		}
	}
}




void Campo::cargar(std::istream & is)
{
}

bool Campo::operator==(const Campo & otroCampo) const
{
	Posicion p;
	bool mismoContenido = true;
	if((dimensiones().ancho == otroCampo.dimensiones().ancho) && (dimensiones().largo == otroCampo.dimensiones().largo)){
		for(int j = dimensiones().largo -1; j >= 0; --j){
			for(int i = 0; i < dimensiones().ancho -1; ++i){
				p.y = j;
				p.x = i;
				if(contenido(p) != otroCampo.contenido(p)){
					mismoContenido = false;
					break;
				}
			}
		}
	}
	return mismoContenido;
}

std::ostream & operator<<(std::ostream & os, const Campo & c)
{
	c.mostrar(os);
	return os;
}

std::ostream & operator<<(std::ostream & os, const Parcela & p)
{
	switch (p)
	{
	case Cultivo:
		os << "Cultivo";
		break;
	case Casa:
		os << "Casa";
		break;
	case Granero:
		os << "Granero";
		break;
	}
	return os;
}

std::ostream & operator<<(std::ostream & os, const Producto & p)
{
	switch (p)
	{
		case Fertilizante:
			os << "Fertilizante";
			break;
		case Plaguicida:
			os << "Plaguicida";
			break;
		case PlaguicidaBajoConsumo:
			os << "PlaguicidaBajoConsumo";
			break;
		case Herbicida:
			os << "Herbicida";
			break;
		case HerbicidaLargoAlcance:
			os << "HerbicidaLargoAlcance";
			break;
	}
	return os;
}

std::ostream & operator<<(std::ostream & os, const EstadoCultivo & e)
//{RecienSembrado, EnCrecimiento, ListoParaCosechar, ConMaleza, ConPlaga, NoSensado}
{
	switch (e)
	{
		case RecienSembrado:
			os << "RecienSembrado";
			break;
		case EnCrecimiento:
			os << "EnCrecimiento";
			break;
		case ListoParaCosechar:
			os << "ListoParaCosechar";
			break;
		case ConMaleza:
			os << "ConMaleza";
			break;
		case ConPlaga:
			os << "ConPlaga";
			break;
		case NoSensado:
			os << "NoSensado";
			break;
	}
	return os;
}
