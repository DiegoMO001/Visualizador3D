#include "vetor3d.h"

Vetor3D::Vetor3D(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

double Vetor3D::comprimento() const
{
    return std::sqrt(x * x + y * y + z * z);
}

void Vetor3D::normalizar()
{
    double len = comprimento();
    if (len > 1e-9)
    {
        x /= len;
        y /= len;
        z /= len;
    }
}

Vetor3D Vetor3D::normalizado() const
{
    Vetor3D temp = *this;
    temp.normalizar();
    return temp;
}

double Vetor3D::produtoEscalar(const Vetor3D& v1, const Vetor3D& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vetor3D Vetor3D::produtoVetorial(const Vetor3D& v1, const Vetor3D& v2)
{
    return Vetor3D(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
        );
}

Vetor3D Vetor3D::operator+(const Vetor3D& outro) const
{
    return Vetor3D(x + outro.x, y + outro.y, z + outro.z);
}

Vetor3D Vetor3D::operator-(const Vetor3D& outro) const
{
    return Vetor3D(x - outro.x, y - outro.y, z - outro.z);
}

Vetor3D Vetor3D::operator*(double escalar) const
{
    return Vetor3D(x * escalar, y * escalar, z * escalar);
}

Vetor3D Vetor3D::operator/(double escalar) const
{
    return Vetor3D(x / escalar, y / escalar, z / escalar);
}

void Vetor3D::operator+=(const Vetor3D& outro)
{
    x += outro.x;
    y += outro.y;
    z += outro.z;
}
