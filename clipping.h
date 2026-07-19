#ifndef CLIPPING_H
#define CLIPPING_H

#include "ponto2d.h"
#include "mrect.h"

class Clipping
{
public:
    enum RegionCode
    {
        DENTRO = 0,       // 0000
        ESQUERDA = 1,    // 0001
        DIREITA = 2,    // 0010
        BASE= 4,       // 0100
        TOPO = 8      // 1000
    };

    static RegionCode calculaRegionCode(double x, double y, const MRect &rect);
    static bool clipLinha(Ponto2D &p1, Ponto2D &p2, const MRect &rect);
};

#endif // CLIPPING_H
