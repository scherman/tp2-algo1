#include "tipos.h"

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


const bool operator==(const Posicion &p1, const Posicion &p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

const bool operator==(const InfoVueloCruzado &a, const InfoVueloCruzado &b) {
    return a.posicion == b.posicion && a.cantidadCruces == b.cantidadCruces;
}
