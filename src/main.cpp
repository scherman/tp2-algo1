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
void crearYCargarDrone();
void crearYCargarSistema();
void compararSistemas();

int main()
{
    // crearSistema();
    // crearDrone();
    crearCampo();
    // compararDrones();
    // crearYCargarCampo();
    // crearYCargarDrone();
    // compararDrones();
    // crearYCargarSistema();
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
    Secuencia<Producto> productos1 = {Fertilizante, Plaguicida,PlaguicidaBajoConsumo, Fertilizante};
    Secuencia<Producto> productos2 = {Fertilizante, Fertilizante, Plaguicida, PlaguicidaBajoConsumo};
    Secuencia<Producto> productos3 = {Fertilizante, Fertilizante, Fertilizante, Plaguicida};
    Secuencia<Producto> productos4 = {Fertilizante, Fertilizante, Fertilizante};
    Drone drone1 (id, productos1);
    Drone drone2 (id, productos2);
    Drone drone3 (id, productos3);
    Drone drone4 (id, productos4);
    std::cout << "Drone1 == Drone1: " << (drone1 == drone1) << std::endl;
    std::cout << "Drone1 == Drone2: " << (drone1 == drone2) << std::endl;
    std::cout << "Drone1 == Drone3: " << (drone1 == drone3) << std::endl;
    std::cout << "Drone1 == Drone4: " << (drone1 == drone4) << std::endl;


    Posicion pos1 = {0,1};
    Posicion pos2 = {1,1};
    Secuencia<Posicion> secuencia1 = {pos1, pos2};
    std::cout << "Trayectoria == trayectoria: " << (secuencia1 == secuencia1) <<  std::endl;

}


void crearSistema() {
    Sistema sistema;
    sistema.mostrar(std::cout);
}

void compararSistemas(){
    Posicion posG, posC;
    posG.x = 3;
    posG.y = 2;
    posC.x = 4;
    posC.y = 1;
    Campo c = {posG, posC};

    ID id = 15;
    Secuencia<Producto> productos = {Fertilizante, Plaguicida, PlaguicidaBajoConsumo, Fertilizante};
    Drone drone1 (id, productos);
    ID id1 = 15;
    Secuencia<Producto> productos1 = {Fertilizante, Fertilizante, PlaguicidaBajoConsumo, Plaguicida};
    Drone drone2 (id1, productos1);
    Secuencia<Drone> drones1 = {drone1};
    Secuencia<Drone> drones2 = {drone2};
    Secuencia<Drone> drones3 = {drone2,drone1};

    Sistema sist1 (c, drones1);
    Sistema sist2 (c, drones2);
    Sistema sist3 (c, drones3);
    std::cout << "Sistema1 == Sistema1: " << (sist1 == sist1) << std::endl;
    std::cout << "Sistema1 == Sistema2: " << (sist1 == sist2) << std::endl;
    std::cout << "Sistema2 == Sistema3: " << (sist2 == sist3) << std::endl;
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

    std::cout << "Iguales: " << (campo == campo2) << std::endl;
}

void crearYCargarDrone() {
    Secuencia<Producto> productos1 = {Fertilizante, Plaguicida,PlaguicidaBajoConsumo, Fertilizante};
    Drone drone1 (15, productos1);
    drone1.moverA({0,1});
    drone1.moverA({1,1});
    drone1.mostrar(std::cout);
    std::ofstream file;
    file.open("drone.txt");
    drone1.guardar(file);
    file.close();
    Drone drone2;
    std::ifstream in("drone.txt");
    drone2.cargar(in);
    drone2.mostrar(std::cout);
    in.close();
}

void crearYCargarSistema() {
    Campo campo;
    Drone drone;
    drone.moverA({0,1});
    drone.moverA({1,1});
    Drone otroDrone(15, {Fertilizante});
    otroDrone.moverA({5,1});
    Secuencia<Drone> drones = {drone, otroDrone};
    Sistema sistema(campo, drones);
    sistema.mostrar(std::cout);
    std::ofstream file;
    file.open("sistema.txt");
    sistema.guardar(file);
    file.close();

    Sistema sistema2;
    std::ifstream in("sistema.txt");
    sistema2.cargar(in);
    in.close();

    file.open("result.txt");
    sistema.guardar(file);
    file << std::endl;
    file << std::endl;
    file << std::endl;
    sistema2.guardar(file);
    file.close();

}
