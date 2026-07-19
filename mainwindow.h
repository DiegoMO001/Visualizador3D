#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileInfo>
#include <QMainWindow>
#include <QFileDialog>
#include "displayfile.h"
#include "canvas.h"
#include "qstandarditemmodel.h"
#include "vetor3d.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Canvas* canvas;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_listView_clicked(const QModelIndex &index);
    void on_listView_activated(const QModelIndex &index);
    void on_listView_indexesMoved(const QModelIndexList &indexes);

    void on_frame_customContextMenuRequested(const QPoint &pos);

    void atualizarListaObjetos();

    void on_aplicarClippingBTN_clicked();

    void on_carregarObjetoBTN_clicked();

    void on_resetObjetosBTN_clicked();

private:
    void rotacionarObjeto();
    void translacionarObjeto();
    void escalarObjeto();
    void carregarObjeto(const QString& filePath);

    DisplayFile displayFile;
    Ui::MainWindow *ui;
    QStandardItemModel *listModel;

    QList<QPointF> pontosTemporarios;

    bool modoInsercao = false;
    int contadorPontos = 1; int contadorLinhas = 1; int contadorQuadrados = 1; int contadorTriangulos = 1; int contadorPoligonos = 1;

    Vetor3D calcularCentro(const Objeto& obj);

protected:

};
#endif // MAINWINDOW_H
