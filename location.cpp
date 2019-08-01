#include "location.h"


location::location(int j, int i)
{
    this->i = i;
    this->j = j;


    this->X = i + j % 2 / 2.0;
    this->Y = j * cos(M_PI / 6);

}

int location::getI()
{
    return i;
}

int location::getJ()
{
    return j;
}

double location::range(location destination)
{
    return hypot(this->X - destination.getX(), this->Y - destination.getY());
}

double location::angle(location destination)
{
    return qRadiansToDegrees(qAtan2(this->Y - destination.getY(), this->X - destination.getX()));
}

void location::step(int direction)
{

    switch (direction) {
    case NW:
        if ( j % 2 == 0 ) {
            i -= 1;
        }
        j -= 1;
        break;
    case NE:
        if ( j % 2 != 0 ) {
            i += 1;
        }
        j -= 1;
        break;
    case E:
        i -= 1;
        break;
    case SE:
        if ( j % 2 != 0 ) {
            i += 1;
        }
        j += 1;
        break;
    case SW:
        if ( j % 2 == 0 ) {
            i -= 1;
        }
        j += 1;
        break;
    case W:
        i += 1;
        break;
    }
    if ( i < 0 ) {
        i = 0;
    } else if ( i == 140 ) {
        i = 140 - 1;
    }

    if ( j < 0 ) {
        j = 0;
    } else if ( j == 90 ) {
        j = 90 - 1;
    }

    this->X = i + j % 2 / 2.0;
    this->Y = j * cos(M_PI / 6);
}


double location::getX()
{
    return X;
}

double location::getY()
{
    return Y;
}

void location::operator=(location inp)
{
    this->X = inp.getX();
    this->Y = inp.getY();
    this->i = inp.getI();
    this->j = inp.getJ();
}

bool location::operator==(location inp) const
{
    if ( this->i == inp.getI() && this->j == inp.getJ() ) {
        return true;
    }
    return false;
}

bool location::operator!=(location inp) const
{
    if ( this->i == inp.getI() && this->j == inp.getJ() ) {
        return false;
    }
    return true;
}
