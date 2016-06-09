#ifndef SISTEMA_H_INCLUDED
#define SISTEMA_H_INCLUDED

#include "tipos.h"
#include "campo.h"
#include "drone.h"

class Sistema {

public:
    Sistema();

    Sistema(const Campo &c, const Secuencia<Drone> &ds);

    const Campo &campo() const;

    EstadoCultivo estadoDelCultivo(const Posicion &p) const;

    const Secuencia<Drone> &enjambreDrones() const;

    void crecer();

    void seVinoLaMaleza(const Secuencia<Posicion> &ps);

    void seExpandePlaga();

    void despegar(const Drone &d);

    bool listoParaCosechar() const;

    void aterrizarYCargarBaterias(Carga b);

    void fertilizarPorFilas();

    void volarYSensar(const Drone &d);

    void mostrar(std::ostream &os) const;

    void guardar(std::ostream &os) const;

    void cargar(std::istream &is);

    bool operator==(const Sistema &otroSistema) const;

private:
    Campo _campo;
    Secuencia<Drone> _enjambre;
    Grilla<EstadoCultivo> _estado;

    //Para correr los tests (NO TOCAR)
    friend class test_sistema_lo_basico_crecer_funciona_ok_Test;
    friend class test_sistema_se_vino_la_maleza_lista_vacia_no_modifica_nada_Test;
    friend class test_sistema_se_vino_la_maleza_se_pone_con_maleza_las_posiciones_de_la_lista_Test;
    friend class test_sistema_se_expande_plaga_si_no_hay_plaga_no_cambia_Test;
    friend class test_sistema_se_expande_plaga_si_hay_plaga_se_expande_Test;
    friend class test_sistema_listo_para_cosechar_si_es_menos_del_90_por_ciento_retorna_false_Test;
    friend class test_sistema_listo_para_cosechar_si_es_exactamente_el_90_por_ciento_retorna_true_Test;
    friend class test_sistema_listo_para_cosechar_si_es_mas_del_90_por_ciento_retorna_true_Test;
    friend class test_sistema_volar_y_sensar_todo_libre_Test;

    // --> Defina sus funciones auxiliares debajo...

};
bool vecinoConPlaga(const Sistema &s, const Posicion &p);
Secuencia<Posicion> posiblePosicionLibre(const Sistema &s, const Posicion &pos);
Posicion posDelGranero(const Campo &c);
bool estaLibre(const Sistema &s, const Posicion &p);
Secuencia<Posicion> parcelasCultivo(const Campo &c);
int cantCultivosCosechables(const Sistema &s);
int recorridoMaximo(const Sistema &s, const Drone &d);
int cantFertilizantes(const Drone &d);
int parcelasFertilizables(const Sistema &s, const int &i, const int &viaje);
bool perteneceA(const Producto &p, const Secuencia<Producto> &ds);
EstadoCultivo estaSensado(const Sistema &s, const Posicion &p);
const std::string estadoAbreviado (const EstadoCultivo &estado);
std::vector<EstadoCultivo> splitEstados(const std::string &text, char sep);
const EstadoCultivo aEstadoCultivo (const std::string &text);
bool mismosDrones(const Secuencia<Drone> ps, const Secuencia<Drone> ds);
bool cuentaDron(const Drone d, const Secuencia<Drone> ds);
int distancia(const Posicion &p, const Posicion &pos);
int valAbs(const int &x);
// Definirlo usando mostrar, para poder usar << con este tipo.
std::ostream &operator<<(std::ostream &os, const Sistema &s);

#endif // SISTEMA_H_INCLUDED
