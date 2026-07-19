#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QStandardItemModel>
#include <QInputDialog>
#include <QMessageBox>
#include "clipping.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    canvas = new Canvas(this);
    canvas->setGeometry(ui->frame->geometry());
    canvas->setDisplayFile(&displayFile);

    ui->frame->hide();
    listModel = new QStandardItemModel(this);
    ui->listView->setModel(listModel);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}

Vetor3D MainWindow::calcularCentro(const Objeto& obj)
{
    Vetor3D centro;
    if (obj.pontos.isEmpty()) return centro;

    for (const Vetor3D& p : obj.pontos)
    {
        centro += p;
    }

    return centro / obj.pontos.size();
}

void MainWindow::carregarObjeto(const QString& filePath)
{
    Objeto novoObjeto;
    novoObjeto.nome = QFileInfo(filePath).baseName();
    novoObjeto.tipo = OBJETO_3D;

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Erro", "Não foi possível abrir o arquivo OBJ.");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        QStringList parts = line.split(' ', Qt::SkipEmptyParts);
        if (parts.isEmpty()) continue;

        if (parts[0] == "v")
        {
            if (parts.size() >= 4)
            {
                novoObjeto.pontos.append(Vetor3D(parts[1].toDouble(), parts[2].toDouble(), parts[3].toDouble()));
            }
        }
        else if (parts[0] == "f")
        {
            QList<int> face_indices;
            for (int i = 1; i < parts.size(); ++i)
            {
                face_indices.append(parts[i].split('/').first().toInt() - 1);
            }
            for (int i = 0; i < face_indices.size(); ++i)
            {
                novoObjeto.arestas.append({face_indices[i], face_indices[(i + 1) % face_indices.size()]});
            }
        }
    }
    file.close();

    novoObjeto.calcularCentroLocal();
    displayFile.adicionarObjeto(novoObjeto);
    atualizarListaObjetos();
    canvas->update();
}

void MainWindow::atualizarListaObjetos()
{
    QModelIndexList selected = ui->listView->selectionModel()->selectedIndexes();
    int selectedRow = selected.isEmpty() ? -1 : selected.first().row();

    listModel->clear();
    listModel->setHorizontalHeaderLabels({"Objetos"});

    for (const Objeto& obj : displayFile.listarObjetos())
    {
        QString tipo;

        switch(obj.tipo)
        {
            case PONTO: tipo = "Ponto"; break;
            case RETA: tipo = "Reta"; break;
            case TRIANGULO: tipo = "Triângulo"; break;
            case QUADRADO: tipo = "Quadrado"; break;
            case POLIGONO: tipo = "Polígono"; break;
        }

        QStandardItem *item = new QStandardItem(QString("%1 (%2)").arg(obj.nome, tipo));
        listModel->appendRow(item);
    }

    if(selectedRow >= 0 && selectedRow < listModel->rowCount())
    {
        ui->listView->setCurrentIndex(listModel->index(selectedRow, 0));
    }
}

void MainWindow::rotacionarObjeto()
{
    bool ok;
    double angulo;
    Matriz rotacao;
    QModelIndex currentIndex;
    QString eixo;
    QStringList eixos;
    Vetor3D centroOriginal, centroMundo;

    currentIndex = ui->listView->currentIndex();
    if(!currentIndex.isValid()) return;

    eixos = {"X", "Y", "Z"};
    eixo = QInputDialog::getItem(this, "Rotacionar", "Eixo de Rotação:", eixos, 0, false, &ok);
    if(!ok) return;

    angulo = QInputDialog::getDouble(this, "Rotacionar", "Ângulo (graus):", 0, -360, 360, 1, &ok);
    if(!ok) return;

    Objeto& obj = displayFile.getObjeto(currentIndex.row());

    for (const Vetor3D& p : obj.pontos)
    {
        centroOriginal += p;
    }

    Vetor3D pivo = obj.transformacao.transformar(obj.centroLocal);

    rotacao.rotacao(angulo, pivo, eixo.toLatin1()[0]);
    obj.aplicarTransformacao(rotacao);
    canvas->update();
}

void MainWindow::translacionarObjeto()
{
    bool ok;
    double dx, dy, dz;
    Matriz translacao;
    QModelIndex currentIndex;

    currentIndex = ui->listView->currentIndex();
    if(!currentIndex.isValid()) return;

    dx = QInputDialog::getDouble(this, "Translacionar", "Deslocamento X:", 0, -1000, 1000, 2, &ok);
    if(!ok) return;
    dy = QInputDialog::getDouble(this, "Translacionar", "Deslocamento Y:", 0, -1000, 1000, 2, &ok);
    if(!ok) return;
    dz = QInputDialog::getDouble(this, "Translacionar", "Deslocamento Z:", 0, -1000, 1000, 2, &ok);
    if(!ok) return;

    Objeto& obj = displayFile.getObjeto(currentIndex.row());

    translacao.translacao(dx, dy, dz);
    obj.aplicarTransformacao(translacao);
    canvas->update();
}

void MainWindow::escalarObjeto()
{
    bool ok;
    Matriz escala;

    QModelIndex currentIndex = ui->listView->currentIndex();
    if(!currentIndex.isValid()) return;

    double sx = QInputDialog::getDouble(this, "Escalar", "Fator de escala X:", 1, 0.1, 10, 2, &ok);
    if(!ok) return;
    double sy = QInputDialog::getDouble(this, "Escalar", "Fator de escala Y:", 1, 0.1, 10, 2, &ok);
    if(!ok) return;
    double sz = QInputDialog::getDouble(this, "Escalar", "Fator de escala Z:", 1, 0.1, 10, 2, &ok);
    if(!ok) return;

    Objeto& obj = displayFile.getObjeto(currentIndex.row());
    Vetor3D pivo = obj.transformacao.transformar(obj.centroLocal);

    escala.escala(sx, sy, sz, pivo);
    obj.aplicarTransformacao(escala);
    canvas->update();
}

void MainWindow::on_aplicarClippingBTN_clicked()
{
    if(canvas->ClippingAtivo())
    {
        canvas->removerClipping();
        ui->aplicarClippingBTN->setText("Aplicar Clipping");
    }
    else
    {
        canvas->aplicarClipping();
        ui->aplicarClippingBTN->setText("Remover Clipping");
    }
}

void MainWindow::on_carregarObjetoBTN_clicked()
{
    QStringList filePaths = QFileDialog::getOpenFileNames(
        this,
        "Selecione um ou mais objetos para carregar",
        "",
        "Ficheiros de Objeto (*.obj)");

    if (filePaths.isEmpty())
    {
        return;
    }

    for (const QString& filePath : filePaths)
    {
        carregarObjeto(filePath);
    }

    atualizarListaObjetos();
    canvas->update();
}

void MainWindow::on_resetObjetosBTN_clicked()
{
    displayFile.limpar();
    atualizarListaObjetos();
    canvas->update();

    contadorPontos = 1;
    contadorLinhas = 1;
    contadorQuadrados = 1;
    contadorTriangulos = 1;
    contadorPoligonos = 1;
}

void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    QMenu contextMenu(this);
    QAction *removerAction = contextMenu.addAction("Remover objeto");
    QAction *rotacionarAction = contextMenu.addAction("Rotacionar");
    QAction *translacionarAction = contextMenu.addAction("Translacionar");
    QAction *escalarAction = contextMenu.addAction("Escalar");

    connect(removerAction, &QAction::triggered, this, [this]()
            {
                QModelIndex currentIndex = ui->listView->currentIndex();
                int row = currentIndex.row();

                if(!currentIndex.isValid()) return;

                if(displayFile.removerObjeto(row))
                {
                    atualizarListaObjetos();
                    canvas->update();
                }
            });

    connect(rotacionarAction, &QAction::triggered, this, &MainWindow::rotacionarObjeto);
    connect(translacionarAction, &QAction::triggered, this, &MainWindow::translacionarObjeto);
    connect(escalarAction, &QAction::triggered, this, &MainWindow::escalarObjeto);

    contextMenu.exec
        (ui->listView->viewport()->mapToGlobal
         (ui->listView->visualRect(index).center()));
}

// Funções vazias abaixo

void MainWindow::on_listView_indexesMoved(const QModelIndexList &indexes)
{

}

void MainWindow::on_frame_customContextMenuRequested(const QPoint &pos)
{

}

void MainWindow::on_listView_activated(const QModelIndex &index)
{

}
