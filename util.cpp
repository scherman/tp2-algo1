#include "util.h"

const int cantidad(const Producto producto, const std::vector<Producto> & productos);

bool Util::mismos(const std::vector<Producto> & t1,const std::vector<Producto> & t2){
  if (t1.size() != t2.size()) return false;
  for (int i = 0; i < t1.size(); i++) {
    if (cantidad(t1[i], t1) != cantidad(t1[i], t2)) {
        return false;
    }
  }
  return true;
}

const int cantidad(const Producto producto, const std::vector<Producto> & productos){
  int cantidad = 0;
  for (int i = 0; i < productos.size(); i++) {
    if (productos[i] == producto) cantidad++;
  }
  return cantidad;
}
