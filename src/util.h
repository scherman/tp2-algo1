#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <vector>
#include "tipos.h"

class Util {
public:
  // La idea es poner todos los mismos aca para que sea generico en un futuro y no haya que cambiarlo en todos lados
  static bool mismos(const std::vector<Producto> & t1,const std::vector<Producto> & t2);
private:
};

#endif
