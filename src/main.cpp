#include <iostream>
#include "campo.h"
#include "drone.h"

using namespace std;

void crearCampo();
void crearDrone();
void compararDrones();

int main()
{
    crearCampo();
    crearDrone();
    compararDrones();


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

void compararDrones(){
    ID id = 15;
    Secuencia<Producto> productos1 = {Fertilizante, Plaguicida, PlaguicidaBajoConsumo, Fertilizante};
    Secuencia<Producto> productos2 = {Plaguicida, Fertilizante, PlaguicidaBajoConsumo, Fertilizante};
    Secuencia<Producto> productos3 = {Fertilizante};
    Drone drone1 (id, productos1);
    Drone drone2 (id, productos2);
    Drone drone3 (id, productos3);
    std::cout << "Drone1 == Drone1: " << (drone1 == drone1) << std::endl;
    std::cout << "Drone1 == Drone2: " << (drone1 == drone2) << std::endl;
    std::cout << "Drone1 == Drone3: " << (drone1 == drone3) << std::endl;
}
