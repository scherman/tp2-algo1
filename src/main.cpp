#include <iostream>
#include "campo.h"
#include "drone.h"
#include "sistema.h"
#include <fstream>

using namespace std;

void crearCampo();
void crearDrone();
void compararDrones();
void crearSistema();
void crearYCargarCampo();

int main()
{
    crearYCargarCampo();
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
    // std::cout << "mismosProductos(Drone1, Drone2)" << (Util::mismos(productos1, productos2)) << std::endl;
    // std::cout << "mismosProductos(Drone1, Drone2)" << (Util::mismos(productos1, productos2)) << std::endl;
    // std::cout << "mismosProductos(Drone1, Drone3)" << (Util::mismos(productos1, productos3)) << std::endl;

    Posicion pos1;
    pos1.x = 0;
    pos1.y = 1;
    Posicion pos2;
    pos2.x = 1;
    pos2.y = 1;
    Secuencia<Posicion> secuencia1 = {pos1, pos2};
    std::cout << "Trayectoria == trayectoria: " << (secuencia1 == secuencia1) <<  std::endl;

}


void crearSistema() {
    Sistema sistema;
    sistema.mostrar(std::cout);
}

void crearYCargarCampo() {
    Campo campo;
    campo.mostrar(std::cout);
    std::ofstream file;
    file.open("campo.txt");
    campo.guardar(file);
    file.close();

    Campo campo2;
    std::ifstream in("campo.txt");
    campo2.cargar(in);
    campo2.mostrar(std::cout);
    in.close();



}
