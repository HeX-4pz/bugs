#include "ant.h"


void Ant::eat()
{
    energy += world->pickUpPart(loc, 8);
    if ( energy > 0.8 * limitEnergy ) {
        isLookingForFood = false;
    }

}

void Ant::takeCargo()
{
    if ( cargoLimit - cargo > 13 ) {
        cargo += world->pickUpPart(loc, 13);
    } else {
        cargo += world->pickUpPart(loc, cargoLimit - cargo);
    }

}

void Ant::putCargo()
{
    myHome->setFood(cargo);
    cargo = 0;

}

QQueue<location> Ant::returnToHome()
{
    QQueue<location *> border;
    QMap<location *, location *> from;
    QVector<location> route;
    QQueue<location> out;
    location * current;
    location investigatedWay;
    location * adressInvestigatedWay;

    route.reserve(100);
    out.reserve(100);

    border.enqueue(&loc);
    from[&loc] = nullptr;

    while (!border.empty()) {
        current = border.dequeue();
        int direction = rand() - 6;
        int limit = direction + 6;
        for ( ; direction < limit; ++direction ) {
            int i = direction % 6;
            investigatedWay = *current;

            investigatedWay.step(i);
            adressInvestigatedWay = world->getLocationAdress(investigatedWay);
            if ( investigatedWay == myHome->getLocation() ) {
                from[adressInvestigatedWay] = current;
                border.clear();
                location * ptrPrev = adressInvestigatedWay;
                while (ptrPrev != nullptr) {
                    route.push_back(*ptrPrev);
                    ptrPrev = from[ptrPrev];
                }
                break;

            }

            if ( !from.contains(adressInvestigatedWay) && world->getPossability(investigatedWay)) {
                border.enqueue(adressInvestigatedWay);
                from[adressInvestigatedWay] = current;
            }
        }
    }

    for ( int i = route.size() - 2; i >=0; --i ) {
        out.enqueue(route[i]);
    }

    return out;
}

void Ant::touchWorld()
{
    int i = loc.getI();
    int j = loc.getJ();
    world->fieldAnts[j][i].insert(this);
}

void Ant::detouchWorld()
{
    int i = loc.getI();
    int j = loc.getJ();
    world->fieldAnts[j][i].remove(this);
}

Ant::Ant(QString name, location loc) :
    Bug(name, loc)
{
    cargo = 0;
    cargoLimit = 100;
    isAlive = true;
    limitEnergy = 100;
    energy = rand() % 50 + 51;
    energyCostsPerAction = 1;
    limitHP = 5;
    HP = limitHP;
}

void Ant::takeDamage(int dmg)
{
    HP -= dmg;
    if ( HP < 0 ) {
        HP = 0;
        isAlive = false;
    }
    qDebug() << name << "was attacked, dmg = " << dmg;
}


//void Ant::makeMove()
//{

//}

void Ant::buildRoute(location destination)
{
    location virtLocation(this->getLocation());
    while ( virtLocation != destination ) {
        double angle = virtLocation.angle(destination);
        int direction = 0;
        if (angle > -30.0 && angle <= 30.0) {
            direction = E;
        } else if (angle > 30.0 && angle <= 90.0) {
            direction = NE;
        } else if (angle > 90.0 && angle <= 150.0) {
            direction = NW;
        } else if (angle > 150.0 || angle <= -150.0) {
            direction = W;
        } else if (angle > -150.0 && angle <= -90.0) {
            direction = SE;
        } else if (angle > -90.0 && angle <= -30.0) {
            direction = SW;
        }

        virtLocation.step(direction);
        nextSteps.enqueue(virtLocation);
        //qDebug() << name << destination.getI() << destination.getJ() << virtLocation.getI() << virtLocation.getJ() << direction << angle;
    }

}

void Ant::makeStep(int direction)
{


}

void Ant::action()
{
    if (!isAlive) {
        return;
    }

    if ( energy == 0 ) {
        if ( cargo > 0 ) {
            if ( cargo > 5 ) {
                cargo -= 5;
                energy += 5;
            } else {
                energy += cargo;
                cargo = 0;
            }
        } else {
            isAlive = false;
            qDebug() << name << "There is no food! I'm dying!!";
        }

        return;
    }

    if (energy < limitEnergy * 0.3 && !isLookingForFood) {
        isLookingForFood = true;
        isReturning = false;
        nextSteps.clear();
    }

    if (isLookingForFood) {
        if (world->getType(loc) == food) {
            eat();
            return;
        } else if (nextSteps.empty()) {
            nextSteps = lookNearestFood();
            if (nextSteps.empty()) {
                isAlive = false;
                qDebug() << name << "There is no food! I'm dying!!";
                return;
            }

        }

    }

    if (nextSteps.empty() && cargo != cargoLimit) {
        nextSteps = lookNearestFood();
    }

    if ( cargo != cargoLimit && world->getType(loc) == food) {
        takeCargo();
        energy -= 1;
        return;
    }

    if ( cargo == cargoLimit && !isReturning) {
        nextSteps = returnToHome();
        isReturning = true;
    }

    if (isReturning && loc == myHome->getLocation()) {
        putCargo();
        isReturning = false;
    }

    if ( nextSteps.size() > 0) {
        if (!isReturning && world->getType(nextSteps[nextSteps.size()-1]) != food) {
            nextSteps.clear();
            nextSteps = lookNearestFood();
        }
        location tmp(nextSteps.dequeue());
        if (world->getPossability(tmp)) {
            detouchWorld();
            loc = tmp;
            touchWorld();
        } else {
            nextSteps.clear();

            int destI = rand() % 140;
            int destJ = rand() % 90;

            buildRoute(location(destJ, destI));
        }
    }


    energy -= 1;
    return;
}

void Ant::drawYourself(QPainter * pPaint)
{
    pPaint->setBrush(QBrush(Qt::black));
    pPaint->drawEllipse(world->getPoint(&loc), 3, 3);
    //pPaint->setBrush(QBrush(color));
    //pPaint->drawEllipse(world->getPoint(&loc), 2, 2);

}

QQueue<location> Ant::lookNearestFood()
{
    QQueue<location *> border;
    QMap<location *, location *> from;
    QVector<location> route;
    QQueue<location> out;
    location * current;
    location investigatedWay;
    location * adressInvestigatedWay;
    QVector<location *> targets;
    int targetLimit = 10;

    route.reserve(100);
    out.reserve(100);

    border.enqueue(&loc);
    from[&loc] = nullptr;

    while (!border.empty()) {
        current = border.dequeue();
        int direction = rand() - 6;
        int limit = direction + 6;
        for ( ; direction < limit; ++direction ) {
            int i = direction % 6;
            investigatedWay = *current;

            investigatedWay.step(i);
            adressInvestigatedWay = world->getLocationAdress(investigatedWay);

            if ( !from.contains(adressInvestigatedWay) ) {

                if ( world->getPossability(investigatedWay)) {
                    border.enqueue(adressInvestigatedWay);
                    from[adressInvestigatedWay] = current;
                }

                if ( world->getType(investigatedWay) == food) {
                    targets.push_back(adressInvestigatedWay);
                    if (targets.size() == targetLimit ) {
                        //from[adressInvestigatedWay] = current;

                        int choise = rand() % targetLimit;
                        border.clear();
                        location * ptrPrev = targets[choise];
                        while (ptrPrev != nullptr) {
                            route.push_back(*ptrPrev);
                            ptrPrev = from[ptrPrev];
                        }
                        break;
                    }
                }
            }
        }
    }

    for ( int i = route.size() - 2; i >=0; --i ) {
        out.enqueue(route[i]);
    }

    return out;
}

void Ant::oneAction()
{
    action();
}


