#include "home.h"

void home::setFood(int amount)
{
    this->amount += amount;
}

int home::getPartFood(int amount)
{
    int result = 0;

    if (amount < this->amount ) {
        this->amount -= amount;
        return amount;
    }

    result = this->amount;
    this->amount = 0;

    return result;
}

home::home(location place, map * world)
{
    this->place = place;
    this->world = world;
}

location home::getLocation()
{
    return place;
}

void home::oneAction()
{
    emit antsOneAct();

}
