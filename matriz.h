#ifndef MATRIZ_H
#define MATRIZ_H

#include <cmath>
#include "vetor3d.h"

class Matriz
{
public:
    Matriz();
        Matriz operator*(const Matriz& outro) const;

        Vetor3D transformar(const Vetor3D& point) const;

        void translacao(double dx, double dy, double dz);
        void escala(double sx, double sy, double sz, Vetor3D pivo);
        void rotacao(double angulo, const Vetor3D& pivo, char eixo);

private:
    double m[4][4];
};

#endif // MATRIZ_H
