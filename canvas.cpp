#include <QWheelEvent>
#include "canvas.h"
#include "clipping.h"

Canvas::Canvas(QWidget *parent) : QFrame(parent), displayFile(nullptr), clippingAtivo(false)
{
    setStyleSheet("background-color: gray;");

    cameraPosition = Vetor3D(0.0, 0.0, -50.0);
    focalLength = 50.0;
    nearPlane = 1.0;

    window = MRect(-25, -25, 50, 50);
    viewportInternal = MRect(10, 10, width()-20, height()-20);
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    if(!displayFile) return;

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 1, Qt::DotLine));
    painter.drawRect(QRectF(viewportInternal.x, viewportInternal.y, viewportInternal.largura, viewportInternal.altura));

    for(const Objeto& obj : displayFile->listarObjetos())
    {
        painter.setPen(QPen(Qt::black, 2));

        if (obj.arestas.isEmpty()) continue;

        for(const auto& aresta : obj.arestas)
        {
            if(aresta.first >= obj.pontos.size() || aresta.second >= obj.pontos.size()) continue;

            Vetor3D p1_world = obj.transformacao.transformar(obj.pontos[aresta.first]);
            Vetor3D p2_world = obj.transformacao.transformar(obj.pontos[aresta.second]);
            Vetor3D p1_cam = p1_world - cameraPosition;
            Vetor3D p2_cam = p2_world - cameraPosition;

            bool p1_is_visible = p1_cam.z > nearPlane;
            bool p2_is_visible = p2_cam.z > nearPlane;

            if (!p1_is_visible && !p2_is_visible) continue;
            if (!p1_is_visible)
            {
                double t = (nearPlane - p1_cam.z) / (p2_cam.z - p1_cam.z);
                p1_cam = p1_cam + (p2_cam - p1_cam) * t;
            }
            if (!p2_is_visible)
            {
                double t = (nearPlane - p2_cam.z) / (p1_cam.z - p2_cam.z);
                p2_cam = p2_cam + (p1_cam - p2_cam) * t;
            }

            Ponto2D p1_proj( (p1_cam.x * focalLength) / p1_cam.z, (p1_cam.y * focalLength) / p1_cam.z );
            Ponto2D p2_proj( (p2_cam.x * focalLength) / p2_cam.z, (p2_cam.y * focalLength) / p2_cam.z );

            bool desenhar = true;
            if(clippingAtivo)
            {
                desenhar = Clipping::clipLinha(p1_proj, p2_proj, window);
            }

            if(desenhar)
            {
                double sx = viewportInternal.largura / window.largura;
                double sy = viewportInternal.altura / window.altura;

                Ponto2D p1_viewport((p1_proj.x - window.left()) * sx + viewportInternal.left(), viewportInternal.bottom() - (p1_proj.y - window.top()) * sy);
                Ponto2D p2_viewport((p2_proj.x - window.left()) * sx + viewportInternal.left(), viewportInternal.bottom() - (p2_proj.y - window.top()) * sy);

                // Converte no final
                painter.drawLine(QPointF(p1_viewport.x, p1_viewport.y), QPointF(p2_viewport.x, p2_viewport.y));
            }
        }
    }
}

void Canvas::wheelEvent(QWheelEvent *event)
{
    int delta = event->angleDelta().y();
    if (delta == 0) return;

    double factor = (delta > 0) ? 0.9 : 1.1;
    cameraPosition.z *= factor;
    if (cameraPosition.z > -nearPlane) cameraPosition.z = -nearPlane;
    update();
}

Ponto2D Canvas::normalizarCoordenadas(const Ponto2D& pontoTela) const
{
    double x_mundo = window.left() + (pontoTela.x - viewportInternal.left()) * window.largura / viewportInternal.largura;
    double y_mundo = window.top() + (viewportInternal.bottom() - pontoTela.y) * window.altura / viewportInternal.altura;

    return Ponto2D(x_mundo, y_mundo);
}

void Canvas::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);

    viewportInternal = MRect(10, 10, width()-20, height()-20);
    update();
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isPanning = true;

        lastMousePos = Ponto2D(event->position().x(), event->position().y());
    }
}


void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (isPanning)
    {
        Ponto2D currentMousePos(event->position().x(), event->position().y());
        Ponto2D delta = currentMousePos - lastMousePos;
        double moveSpeedX = -window.largura / viewportInternal.largura;
        double moveSpeedY = window.altura / viewportInternal.altura;

        cameraPosition.x += delta.x * moveSpeedX;
        cameraPosition.y += delta.y * moveSpeedY;

        lastMousePos = currentMousePos;
        update();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isPanning = false;
    }
}

void Canvas::setDisplayFile(DisplayFile* df)
{
    displayFile = df;
    update();
}

void Canvas::setClippingAtivo(bool ativo)
{
    clippingAtivo = ativo;
    update();
}

bool Canvas::ClippingAtivo() const
{
    return clippingAtivo;
}

void Canvas::aplicarClipping()
{
    clippingAtivo = true;
    update();
}

void Canvas::removerClipping()
{
    clippingAtivo = false;
    update();
}
