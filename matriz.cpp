#include <cmath>
#include "matriz.h"

Matriz::Matriz()
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            m[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
}

Matriz Matriz::operator*(const Matriz& other) const
{
    Matriz resultado;

    for(int i = 0; i < 4; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            resultado.m[i][j] = 0;
            for(int k = 0; k < 4; ++k)
            {
                resultado.m[i][j] += m[i][k] * other.m[k][j];
            }
        }
    }
    return resultado;
}

Vetor3D Matriz::transformar(const Vetor3D& point) const
{
    double x, y, z;

    x = point.x * m[0][0] + point.y * m[0][1] + point.z * m[0][2] + m[0][3];
    y = point.x * m[1][0] + point.y * m[1][1] + point.z * m[1][2] + m[1][3];
    z = point.x * m[2][0] + point.y * m[2][1] + point.z * m[2][2] + m[2][3];

    return Vetor3D(x, y, z);
}

void Matriz::translacao(double dx, double dy, double dz)
{
    m[0][3] = dx;
    m[1][3] = dy;
    m[2][3] = dz;
}

void Matriz::escala(double sx, double sy, double sz, Vetor3D pivo)
{
    Matriz t, s, ti;

    t.m[0][3] = -pivo.x;
    t.m[1][3] = -pivo.y;
    t.m[2][3] = -pivo.z;

    s.m[0][0] = sx;
    s.m[1][1] = sy;
    s.m[2][2] = sz;

    ti.m[0][3] = pivo.x;
    ti.m[1][3] = pivo.y;
    ti.m[2][3] = pivo.z;

    *this = ti * s * t;
}

void Matriz::rotacao(double angulo, const Vetor3D& pivo, char eixo)
{
    Matriz t, r, ti;
    double rad = angulo * M_PI / 180.0;
    double c = cos(rad);
    double s = sin(rad);

    t.m[0][3] = -pivo.x;
    t.m[1][3] = -pivo.y;
    t.m[2][3] = -pivo.z;

    ti.m[0][3] = pivo.x;
    ti.m[1][3] = pivo.y;
    ti.m[2][3] = pivo.z;

    switch(eixo)
    {
    case 'x': case 'X':
        r.m[1][1] = c;  r.m[1][2] = -s;
        r.m[2][1] = s;  r.m[2][2] = c;
        break;
    case 'y': case 'Y':
        r.m[0][0] = c;  r.m[0][2] = s;
        r.m[2][0] = -s; r.m[2][2] = c;
        break;
    case 'z': case 'Z': default:
        r.m[0][0] = c;  r.m[0][1] = -s;
        r.m[1][0] = s;  r.m[1][1] = c;
        break;
    }

    *this = ti * r * t;
}
