#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <vector>
#include "tipos.h"

class Util {
public:
  static bool mismos(const std::vector<Producto> & t1,const std::vector<Producto> & t2);
  // template<typename T, typename A>
  // static bool mismos(const std::vector<T,A> & t1,const std::vector<T,A> & t2);
private:
};

#endif
