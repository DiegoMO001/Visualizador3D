#ifndef DISPLAYFILE_H
#define DISPLAYFILE_H

#include <QString>
#include <QList>
#include <QPointF>
#include "vetor3d.h"
#include "matriz.h"

enum TipoObjeto
{
    PONTO,
    RETA,
    QUADRADO,
    TRIANGULO,
    POLIGONO,
    OBJETO_3D
};

struct Objeto
{
    QString nome;
    TipoObjeto tipo;

    QList<Vetor3D> pontos;
    QList<QPair<int, int>> arestas;

    Vetor3D centroLocal, soma;

    Matriz transformacao;

    void aplicarTransformacao(const Matriz& novaTransformacao)
    {
        transformacao = novaTransformacao * transformacao;
    }

    void calcularCentroLocal()
    {
        if (pontos.isEmpty())
        {
            centroLocal = Vetor3D(0,0,0);
            return;
        }

        for(const auto& p : pontos)
        {
            soma += p;
        }
        centroLocal = soma / pontos.size();
    }
};

class DisplayFile
{
public:
    void adicionarObjeto(Objeto& obj);
    void limpar();
    bool removerObjeto(int index);

    const QList<Objeto>& listarObjetos() const;

    Objeto& getObjeto(int index);

private:
    QList<Objeto> objetos;
};

#endif // DISPLAYFILE_H
