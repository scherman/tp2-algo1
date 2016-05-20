#include "campo.h"
#include <iostream>
#include <fstream>

Campo::Campo()
{
	_dimension = (3,1);
	_grilla = (Cultivo, Casa, Granero); //esta bien definida la GRILLA??
}

Campo::Campo(const Posicion &posG, const Posicion &posC) 
{
	contenido(posG) = Granero;
	contenido(posC) = Casa;
	for(int x = 0; x < ancho.dimensiones();x++)
	{
		for(int y = 0;y < largo.dimensiones(); y++)
		{
			if((x,y) != posG && (x,y) != posC) contenido(x,y) = Cultivo; //esta bien este if en UNA sola linea???
		}
	}
}
/*tengo que decir que el resto de las posiciones
 son cultivo, usando que x < ancho.dimensiones...
*/


Dimension Campo::dimensiones() const
{
	return _dimension;
}

Parcela Campo::contenido(const Posicion & p) const
{
	return p._grilla; // es correcto???
}

void Campo::mostrar(std::ostream & os) const
{
	os << "CropFieldRobotics: Dimensiones : " << dimensiones() << "| Grilla: " << _grilla << endl;
}
//como hago para que me mande dimensiones() entre []
//_grilla ya devuelve la matriz entre []??


void Campo::guardar(std::ostream & os) const
{
	os << "{ C  " << dimensiones() << "  " << _grilla << " }";
}

void Campo::cargar(std::istream & is)
{
}

bool Campo::operator==(const Campo & otroCampo) const
{
	bool mismoContenido = true;
	
	for(int x = 0; x < ancho.dimensiones();x++)
	{
		if(mismoContenido = false) break; //esta bien hecho este if en UNA sola linea?
		for(int y = 0;y < largo.dimensiones(); y++)
		{
			if(contenido(x,y) != otroCampo.contenido(x,y))
			{
				mismoContenido = false;
				break; 
			}
		}
	}
	return (dimensiones() == otroCampo.dimensiones() && mismoContenido);
}

std::ostream & operator<<(std::ostream & os, const Campo & c)
{
	c.mostrar(os)
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
			os << "HerbicidaLargoAlcance"
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
