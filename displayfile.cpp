#include "displayfile.h"

void DisplayFile::adicionarObjeto(Objeto &obj)
{
    objetos.append(obj);
}

void DisplayFile::limpar()
{
    objetos.clear();
}

const QList<Objeto>& DisplayFile::listarObjetos() const
{
    return objetos;
}

bool DisplayFile::removerObjeto(int index)
{
    if (index >= 0 && index < objetos.size())
    {
        objetos.removeAt(index);
        return true;
    }
    return false;
}

Objeto& DisplayFile::getObjeto(int index)
{
    if (index >= 0 && index < objetos.size())
    {
        return objetos[index];
    }
    throw std::out_of_range("Índice inválido");
}
