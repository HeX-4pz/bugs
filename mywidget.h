#pragma once

#include <QWidget>
#include <QPainter>
#include <QtMath>
#include <QtWidgets>
#include <QDebug>
#include <random>
#include <ctime>
#include "map.h"
#include "antwarrior.h"
#include "customview.h"



class myWidget : public QWidget
{
    Q_OBJECT

    double rotor = 0;

    int X = 0;
    int Y = 0;

    QVector<Bug *> population;
    CustomView * view;
    int memory = ground;
    map world;
    QVector<home *> homes;
    void addHome(int number);

    QTimer * timer;
    QLabel * lbl11;
    QLabel * lbl12;
    int antAmount;

    void init();
public:
    explicit myWidget(QWidget *parent = nullptr);
    ~myWidget();
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);
    void addPopolation();
    void setPopulation();
    void collectDeadAnts();

signals:
    void antsOneAct();

public slots:
    void rotate();
    void tick();
};

