#ifndef VETOR3D_H
#define VETOR3D_H

#include <cmath>

struct Vetor3D
{
    double x, y, z;

    Vetor3D(double x_ = 0.0, double y_ = 0.0, double z_ = 0.0);

    double comprimento() const;
    void normalizar();
    Vetor3D normalizado() const;

    static double produtoEscalar(const Vetor3D& v1, const Vetor3D& v2);
    static Vetor3D produtoVetorial(const Vetor3D& v1, const Vetor3D& v2);

    Vetor3D operator + (const Vetor3D& outro) const;
    Vetor3D operator - (const Vetor3D& outro) const;
    Vetor3D operator * (double escalar) const;
    Vetor3D operator / (double escalar) const;

    void operator += (const Vetor3D& outro);
};

#endif // VETOR3D_H
