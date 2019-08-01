#ifndef CUSTOMVIEW_H
#define CUSTOMVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QDebug>

class CustomView : public QGraphicsView
{
    Q_OBJECT


public:
    CustomView(QWidget *parent = nullptr);
    CustomView(QGraphicsScene *scene, QWidget *parent = nullptr);

public slots:
    void enterEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent * event);
};

#endif // CUSTOMVIEW_H
