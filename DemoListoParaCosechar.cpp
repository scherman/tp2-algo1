//ESPECICACIONES
aux cantCultivosCosechables (s: Sistema) : Z =
|[1|pos ← parcelasCultivo(campo(s)), estadoDelCultivo(pos, s) == ListoParaCosechar]| ;

aux parcelasCultivo (c: Campo) : [(Z, Z)] =
[(i, j) | i ← [0..prm(dimensiones(c))), j ← [0..sgd(dimensiones(c))), contenido((i, j), c) == Cultivo] ;

//----------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------//

bool Sistema::listoParaCosechar() const
{
	bool res = ((cantCultivosCosechables(*this))/(parcelasCultivo(campo()).size())) >= 0.9;
    //Estado E1;
    //Vale: res == ((cantCultivosCosechables(this))/|(parcelasCultivo(c)|)) >= 0.9;
    **En las anteriores demostraciones (cantCultivosCosechables y parcelasCultivo),
    demostre que ambas funciones cumplen con las funciones del mismo nombre en la ESPECIFICACION**
	return res;
    //Estado E2;
    //Vale: result == res;
    //Implica: result == (((cantCultivosCosechables(this))/|(parcelasCultivo(c)|)) >= 0.9);
}
