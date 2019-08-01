#include "customview.h"

CustomView::CustomView(QWidget *parent) : QGraphicsView(parent)
{

}

CustomView::CustomView(QGraphicsScene *scene, QWidget *parent) : QGraphicsView(scene, parent)
{

}

void CustomView::enterEvent(QEvent *event)
{
    QGraphicsView::enterEvent(event);
    viewport()->setCursor(Qt::ArrowCursor);
}

void CustomView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    viewport()->setCursor(Qt::ArrowCursor);
}

void CustomView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    viewport()->setCursor(Qt::ArrowCursor);
}

void CustomView::wheelEvent(QWheelEvent *event)
{
    double scale = 0.9;
    qDebug() << scene()->width() << scene()->height() << width() / this->transform().m11()
             <<  height() / this->transform().m22();
    double limitMaxHorizontal = scene()->width();
    double limitMaxVertical = scene()->height();
    double limitMinHorizontal = limitMaxHorizontal / 20.0;
    double limitMinVertical = limitMaxVertical / 20.0;

    double currentWidth = width() / this->transform().m11();
    double currentHeight = height() / this->transform().m22();


    if ( event->delta() < 0 ) {
        if (currentWidth / scale >= limitMaxHorizontal) {
            scale = currentWidth / limitMaxHorizontal;
        }
        if ( currentHeight / scale >= limitMaxVertical) {
            scale = currentHeight / limitMaxHorizontal;
        }
        this->scale(scale,scale);
    } else {

        if (currentWidth * scale <= limitMinHorizontal) {
            scale = limitMinHorizontal / currentWidth;
        }
        if ( currentHeight * scale <= limitMinVertical) {
            scale = limitMinVertical / currentHeight;
        }
        this->scale(1.0 / scale, 1.0 / scale);
    }
}

