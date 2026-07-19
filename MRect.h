#ifndef MRECT_H
#define MRECT_H

#include "ponto2d.h"

struct MRect
{
    double x, y, largura, altura;

    MRect(double x_ = 0, double y_ = 0, double w_ = 0, double h_ = 0) : x(x_), y(y_), largura(w_), altura(h_) {}

    double left() const
    {
        return x;
    }
    double right() const
    {
        return x + largura;
    }
    double top() const
    {
        return y;
    }
    double bottom() const
    {
        return y + altura;
    }

    bool contem(const Ponto2D& p) const
    {
        return p.x >= left() && p.x <= right() && p.y >= top() && p.y <= bottom();
    }
};

#endif // MRECT_H
