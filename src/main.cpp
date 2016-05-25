#include <iostream>
#include "campo.h"
#include "drone.h"

using namespace std;

void crearCampo();
void crearDrone();

int main()
{
    crearCampo();
    crearDrone();

    return 0;
}

void crearCampo(){
    // // Creo posiciones
     Posicion posG, posC;
     posG.x = 3;
     posG.y = 2;
     posC.x = 4;
     posC.y = 1;
    //
    // // Creo campo con granero y casa
    Campo campo1 (posG, posC);
    campo1.mostrar(std::cout);

    // Creo campo sin granero y Casa
    Campo campo2;
    campo2.mostrar(std::cout);
}

void crearDrone(){
    ID id = 15;
    Secuencia<Producto> productos = {Fertilizante, Plaguicida, PlaguicidaBajoConsumo, Fertilizante};
    Drone drone (id, productos);
    drone.mostrar(std::cout);
}
