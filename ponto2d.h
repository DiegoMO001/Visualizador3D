#ifndef PONTO2D_H
#define PONTO2D_H

struct Ponto2D
{
    double x, y;

    Ponto2D(double x_ = 0.0, double y_ = 0.0) : x(x_), y(y_) {}

    Ponto2D operator-(const Ponto2D& outro) const
    {
        return Ponto2D(x - outro.x, y - outro.y);
    }
};

#endif // PONTO2D_H
