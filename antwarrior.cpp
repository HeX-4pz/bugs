#include "antwarrior.h"



AntWarrior::AntWarrior(QString name, location loc):
    Ant(name, loc)
{
    isAlive = true;
    limitEnergy = 200;
    energy = rand() % 100 + 101;
    energyCostsPerAction = 1;
    damage = 10;
    limitHP = 50;
    HP = limitHP;
}

void AntWarrior::action()
{
    if (!isAlive) {
        return;
    }

    nextSteps = returnToHome();
    if ( nextSteps.size() == energy - 1 ) {
        isReturning = true;
    }

    if ( !isReturning ) {
        nextSteps = lookForEnemy();
        if ( nextSteps.size() == 1 ) {
            Ant * enemy = identifyNearbyEnemy();
            if ( enemy != this ) {
                this->attack(enemy);
                energy -= 1;
                return;
            }
        }
    }

    if ( isReturning && loc == myHome->getLocation() ) {
        eat();
        return;
    }

    if ( nextSteps.size() > 0) {
        location tmp(nextSteps.dequeue());
        if (world->getPossability(tmp)) {
            detouchWorld();
            loc = tmp;
            touchWorld();
        } else {
            nextSteps.clear();
        }
    }


    energy -= 1;
    return;

}

void AntWarrior::takeDamage(int dmg)
{
    HP -= dmg;
    if ( HP < 0 ) {
        HP = 0;
        isAlive = false;
    }
    qDebug() << name << "was attacked, dmg = " << dmg;

}

void AntWarrior::drawYourself(QPainter *pPaint)
{
    pPaint->setBrush(QBrush(Qt::red));
    pPaint->drawEllipse(world->getPoint(&loc), 4, 4);
    pPaint->setBrush(QBrush(color));
    pPaint->drawEllipse(world->getPoint(&loc), 2, 2);

}

void AntWarrior::oneAction()
{
    action();

}

QQueue<location> AntWarrior::lookForEnemy()
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
        for ( int i = 0; i < 6; ++i ) {
            investigatedWay = *current;

            investigatedWay.step(i);
            adressInvestigatedWay = world->getLocationAdress(investigatedWay);


            if ( !from.contains(adressInvestigatedWay) && world->getPossability(investigatedWay)) {
                border.enqueue(adressInvestigatedWay);
                from[adressInvestigatedWay] = current;

                QList <Ant *> otherAnts =  world->getTouches(&investigatedWay).values();
                Ant * oneAnt;
                foreach (oneAnt, otherAnts){
                    if ( oneAnt->myHome != this->myHome ) {
                        border.clear();
                        location * ptrPrev = adressInvestigatedWay;
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

Ant *AntWarrior::identifyNearbyEnemy()
{
    location virt;
    QSet< Ant * > ants;
    Ant * ant;
    for ( int i = 0; i < 6; ++i ) {
        virt = loc;
        virt.step(i);

        ants = world->getTouches(&virt);

        if ( ants.size() > 0 ) {
            foreach(ant, ants) {
                if ( ant->myHome != this->myHome ) {
                    return ant;
                }
            }
        }
    }
    return this;
}



void AntWarrior::attack(Ant * enemy)
{
    int attackPower = rand() % damage + damage / 2;
    enemy->takeDamage(attackPower);
    qDebug() << name << " attack ather ant, put dmg = " << attackPower;
}

void AntWarrior::eat()
{
    int takedFood = myHome->getPartFood(20);
    energy += takedFood;
    if ( energy >= limitEnergy ) {
        energy = limitEnergy;
        isReturning = false;
        HP = limitHP;
    }
}

