#include "mywidget.h"

void myWidget::addHome(int number)
{
    QQueue<QColor>colors;
//    colors  << QColor(0, 255, 255)
//             << QColor(255, 99, 71)
//             << QColor(75, 0, 130)
//             << QColor(139, 0, 139)
//             << QColor(255, 0, 255);

    for ( int n = 0; n < number; ++n ) {
        int i = rand() % 140;
        int j = rand() % 90;
        while (!world.getPossability(location(j,i))) {
            i = rand() % 140;
            j = rand() % 90;
        }
        int r = rand() % 255;
        int g = rand() % 255;
        int b = rand() % 255;

        home *pHome = new home(location(j,i), &world);
        pHome->color = QColor(r,g,b);
        homes.push_back(pHome);

    }

}

myWidget::myWidget(QWidget *parent) : QWidget(parent)
{
    world.init();
    X = 0;
    Y = 0;
    this->resize(1450,900);

    QGraphicsScene * scene = new QGraphicsScene(QRectF(0, 0, 5000, 5000));
    view = new CustomView(scene);

    Bug::world = &world;
    addHome(5);
    setPopulation();

//    QVBoxLayout * central = new QVBoxLayout;
//    central->addWidget(view);
//    this->setLayout(central);

//    QGraphicsRectItem* pRectItem =
//                                scene->addRect(QRectF(1000, 1000, 50, 50),
//                                        QPen(Qt::black),
//                                        QBrush(Qt::green)
//                                        ) ;


    timer = new QTimer();
    timer->setInterval(80);
    timer->start();

    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
    connect( this, SIGNAL(antsOneAct()),&world, SLOT(tick()));
    //pBug->observableWorld.lookAround(&world, pBug->getLocation(), pBug->getViewRadius());
}

myWidget::~myWidget()
{
    for ( int i = 0; i < homes.size(); ++i ) {
        delete homes[i];
    }
}

void myWidget::paintEvent(QPaintEvent *) {
    QPainter * pPaint = new QPainter(this);
    int width = world.getWidth();
    int height = world.getHeight();
    QPen * pen = new QPen(Qt::NoPen);
    pen->setWidth(0);
    pPaint->setPen(*pen);
    for ( int i = 0; i < height; ++i ) {
        for  ( int j = 0; j < width; ++j ) {
            if ( world.getType(i,j) == ground ) {
                pPaint->setBrush(QBrush(QColor(235,147,60, 200)));
            } else if (  world.getType(i,j) == grass )  {
                pPaint->setBrush(QBrush(QColor(0, 255, 0)));
            } else if (  world.getType(i,j) == water ) {
                pPaint->setBrush(QBrush(QColor(0, 0, 255)));
            } else if ( world.getType(i,j) == unknown ) {
                pPaint->setBrush(QBrush(QColor(169, 169, 169, 200)));
            }
            pPaint->drawPolygon(world.getQPolygon(i, j));

        }
    }

    antAmount = 0;
    location paintLoc;
    for ( int i = 0; i < homes.size(); ++i ) {
        pPaint->setPen(QPen(Qt::NoPen));
        pPaint->setBrush(QBrush(Qt::red));
        paintLoc = homes[i]->getLocation();
        Y = paintLoc.getJ();
        X = paintLoc.getI();
        pPaint->drawEllipse(world.getPoint(Y,X), 6, 6);
        pPaint->setBrush(QBrush(Qt::black));
        for ( int j = 0; j < homes[i]->ants.size(); ++j){
            if (homes[i]->ants[j]->isAlive) {
                antAmount += 1;
                homes[i]->ants[j]->drawYourself(pPaint);
            }

        }

    }

    //lbl12->setText(QString::number(antAmount));

//pPaint->drawPoint(world.getPoint(Y,X));
    delete pPaint;
    delete pen;
}

void myWidget::addPopolation()
{
    Ant * pAnt;
    for (int i = 0; i < homes.size(); ++i ) {
        if ( homes[i]->amount > 1000 ) {
            int choise = rand() % 100 + 1;
            if ( choise < 50 ) {
                if ( choise > 5 ) {
                    pAnt = new Ant("ant_" + QString::number(homes[i]->ants.size()), homes[i]->place);
                } else {
                    pAnt = new Ant("antW_" + QString::number(homes[i]->ants.size()), homes[i]->place);
                }

                pAnt->myHome = homes[i];
                homes[i]->ants.append(pAnt);
                connect(this, SIGNAL(antsOneAct()), pAnt, SLOT(oneAction()));
            }

            homes[i]->amount -= 500;
        }
    }
}

void myWidget::keyPressEvent(QKeyEvent *event) {

    int key;
    switch (event->key()) {
    case 82:
        key = NW;
        break;
    case 84:
        key = NE;
        break;
    case 68:
        key = E;
        break;
    case 71:
        key = SE;
        break;
    case 67:
        key = SW;
        break;
    case 86:
        key = W;
        break;
    }
    //pBug->makeStep(key, &world);
    this->update();
}

void myWidget::rotate() {
    rotor += 10;
    this->update();
}

void myWidget::tick()
{
    emit antsOneAct();
    this->update();
    collectDeadAnts();
    addPopolation();
}

void myWidget::collectDeadAnts()
{

    for ( int i = 0; i < homes.size(); ++i ) {
        QList<Ant *>::iterator it = homes[i]->ants.begin();
        while (it != homes[i]->ants.end()) {
            if (!(*it)->isAlive) {
                homes[i]->ants.removeOne(*it);
            }
            if (it != homes[i]->ants.end()) {
                ++it;
            }
        }
    }

}

void myWidget::setPopulation()
{

    for ( int j = 0; j < homes.size(); ++j ) {
        int quantity = rand() % 10 + 5;
        for ( int i = 0; i < quantity; ++i ) {
            Ant * pAnt = new Ant("ant_" + QString::number(i), homes[j]->place);
            pAnt->myHome = homes[j];
            pAnt->color = homes[j]->color;
            homes[j]->ants.append(pAnt);
            connect(this, SIGNAL(antsOneAct()), pAnt, SLOT(oneAction()));
        }
        int wariorLimit = quantity / 5;
        for ( int i = 0; i < wariorLimit; ++i ) {
            Ant * pAnt = new Ant("antW_" + QString::number(i), homes[j]->place);
            pAnt->myHome = homes[j];
            pAnt->color = homes[j]->color;
            homes[j]->ants.append(pAnt);
            connect(this, SIGNAL(antsOneAct()), pAnt, SLOT(oneAction()));
        }
    }

}





