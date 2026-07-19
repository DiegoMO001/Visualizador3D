#ifndef CANVAS_H
#define CANVAS_H

#include <QFrame>
#include <QPainter>
#include <QWheelEvent>
#include <QMouseEvent>
#include "ponto2d.h"
#include "displayfile.h"
#include "mrect.h"

class Canvas : public QFrame
{
    Q_OBJECT

public:
    bool modoInsercao = false;
    bool ClippingAtivo() const;

    void setDisplayFile(DisplayFile* df);
    void setClippingAtivo(bool ativo);
    void aplicarClipping();
    void removerClipping();
    void resizeEvent(QResizeEvent *event) override;

    explicit Canvas(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    bool clippingAtivo = false;
    bool isPanning = false;
    double focalLength;
    double nearPlane;

    DisplayFile* displayFile;

    MRect window;
    MRect viewportInternal;

    Ponto2D lastMousePos;
    Ponto2D normalizarCoordenadas(const Ponto2D& pontoTela) const;
    Vetor3D cameraPosition;
};

#endif // CANVAS_H
