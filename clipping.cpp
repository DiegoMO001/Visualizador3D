#include <cmath>
#include "clipping.h"

Clipping::RegionCode Clipping::calculaRegionCode(double x, double y, const MRect &rect)
{
    RegionCode code = DENTRO;

    if (x < rect.left())
        code = static_cast<RegionCode>(code | ESQUERDA);
    else if (x > rect.right())
        code = static_cast<RegionCode>(code | DIREITA);
    if (y < rect.top()) // Topo > Base em cartesiano
        code = static_cast<RegionCode>(code | TOPO);
    else if (y > rect.bottom())
        code = static_cast<RegionCode>(code | BASE);

    return code;
}

bool Clipping::clipLinha(Ponto2D &p1, Ponto2D &p2, const MRect &rect)
{
    RegionCode code1 = calculaRegionCode(p1.x, p1.y, rect);
    RegionCode code2 = calculaRegionCode(p2.x, p2.y, rect);

    bool aceita = false;

    while (true)
    {
        if (!(code1 | code2)) // Ambos dentro
        {
            aceita = true;
            break;
        }
        else if (code1 & code2) // Ambos fora, mesmo lado
        {
            break;
        }
        else
        {
            double x = 0, y = 0;
            double dx = p2.x - p1.x;
            double dy = p2.y - p1.y;

            RegionCode codeFora = code1 ? code1 : code2;

            if (codeFora & TOPO)
            {
                x = p1.x + (rect.top() - p1.y) * dx / dy;
                y = rect.top();
            }
            else if (codeFora & BASE)
            {
                x = p1.x + (rect.bottom() - p1.y) * dx / dy;
                y = rect.bottom();
            }
            else if (codeFora & DIREITA)
            {
                y = p1.y + (rect.right() - p1.x) * dy / dx;
                x = rect.right();
            }
            else if (codeFora & ESQUERDA)
            {
                y = p1.y + (rect.left() - p1.x) * dy / dx;
                x = rect.left();
            }

            if (codeFora == code1)
            {
                p1.x = x;
                p1.y = y;
                code1 = calculaRegionCode(p1.x, p1.y, rect);
            }
            else
            {
                p2.x = x;
                p2.y = y;
                code2 = calculaRegionCode(p2.x, p2.y, rect);
            }
        }
    }
    return aceita;
}
