#include "map.h"

map::map()
{

}

void map::init()
{
    fieldGeometry.resize(sizeY);
    fieldMater.resize(sizeY);
    fieldAnts.resize(sizeY);
    for ( int i = 0; i < sizeY; ++i ) {
        fieldGeometry[i].resize(sizeX);
        fieldMater[i].resize(sizeX);
        fieldAnts[i].resize(sizeX);
    }
    calcInitCoord();
    for ( int j = 0; j < sizeY; ++j ) {
        for ( int i = 0; i < sizeX; ++i ) {
            fieldMater[j][i].place = new location(j,i);
        }
    }

    for ( int j = 0; j < sizeY; ++j ) {
        for ( int i = 0; i < sizeX; ++i ) {
            QSet<cell *> neigborCells;
            location * current = fieldMater[j][i].place;
            for ( int k = 0; k < 6; ++k ) {
                location neigbor(*current);
                neigbor.step(k);
                cell * neigborCell = getCellAdress(neigbor);
                if (neigbor != *current && !neigborCells.contains(neigborCell)) {
                    neigborCells.insert(neigborCell);
                }
            }
            getCellAdress(*current)->neighbors = neigborCells.toList();
        }
    }
    fillWorldMater();


}

int map::pickUpPart(location position, int part)
{
    int result = 0;
    int x = position.getI();
    int y = position.getJ();

    if (fieldMater[y][x].amount > part) {
        fieldMater[y][x].amount -= part;
        result = part;
    } else {
       result = fieldMater[y][x].amount;
       fieldMater[y][x].amount = 0;
       fieldMater[y][x].type = ground;
    }

    return result;


}

void map::calcInitCoord()
{
    for ( int i = 0; i < sizeY; ++i ) {
        for  ( int j = 0; j < sizeX; ++j ) {
            qreal centX = 30.0 + j * cellSize + i % 2 * cellSize / 2.0;
            qreal centY = 30.0 + i * cellSize * cos(M_PI / 6);
            QPolygonF polygon;
            int n = 6;

            for (int i = 0; i < n; ++i) {
               qreal fAngle = qDegreesToRadians((360.0 * i) / n + 30.0);
               qreal x = centX + cos(fAngle) * cellSize / 1.70;
               qreal y = centY + sin(fAngle) * cellSize / 1.70;

               polygon << QPointF(x,y);
            }
            fieldGeometry[i][j] = polygon;
        }
    }
}

void map::fillWorldMater()
{
    for ( int i = 0; i < sizeY; ++i ) {
        for  ( int j = 0; j < sizeX; ++j ) {
            fieldMater[i][j].type = ground;
            fieldMater[i][j].limit = 0;
            fieldMater[i][j].amount = 0;
            fieldMater[i][j].passability = 1;
            fieldMater[i][j].visible = 1;

        }
    }

    waterGenerate();
    grassGenerate(1000, 20);
}

void map::grassGenerate(int limit, int place)
{
    srand(clock());
    for ( int i = 0; i < place; ++i ) {
        int amountInPlace = ((rand() % 100 - 50) + 100) * limit;
        int xPlace = rand() % sizeX;
        int yPlace = rand() % sizeY;

        while ( amountInPlace > 0 ) {
            int stepDirection = rand() % 6;
            oneStep(stepDirection, yPlace, xPlace);
            if ( fieldMater[yPlace][xPlace].type == ground ) {
                fieldMater[yPlace][xPlace].type = grass;
                fieldMater[yPlace][xPlace].limit = 500;
                if ( amountInPlace > fieldMater[yPlace][xPlace].limit ) {
                    amountInPlace -= fieldMater[yPlace][xPlace].limit;
                    fieldMater[yPlace][xPlace].amount = fieldMater[yPlace][xPlace].limit;
                } else {
                    fieldMater[yPlace][xPlace].amount = amountInPlace;
                    amountInPlace = 0;
                }
            }
        }


    }

}

void map::waterGenerate()
{
    int limit = 20;
    int place = 3;

    srand(clock());
    for ( int i = 0; i < place; ++i ) {
        int amountInPlace = (rand() % 50) * limit;
        int xPlace = rand() % sizeX;
        int yPlace = rand() % sizeY;

        while ( amountInPlace > 0 ) {
            int stepDirection = rand() % 6;

            oneStep(stepDirection, yPlace, xPlace);
            if ( fieldMater[yPlace][xPlace].type == ground ) {
                fieldMater[yPlace][xPlace].type = water;
                fieldMater[yPlace][xPlace].passability = 0;
                amountInPlace -= 1;
            }
        }
    }
    fillWaterEmptys();
}

void map::fillWaterEmptys()
{
    int replaces = 1;

    while ( replaces ) {
        replaces = 0;
        for ( int i = 0; i < sizeY; ++i ) {
            for ( int j = 0; j < sizeX; ++j ) {
                if (fieldMater[i][j].type != water) {
                    int waterNeighbor = 0;

                    for ( int k = 0; k < 6; k++ ) {
                        int nX = j;
                        int nY = i;

                        oneStep(k,nY, nX);
                        if (fieldMater[nY][nX].type == water) {
                            waterNeighbor += 1;
                        }
                    }
                    if(waterNeighbor >= 4) {
                        fieldMater[i][j].type = water;
                        fieldMater[i][j].passability = 0;
                        replaces += 1;
                    }
                }
            }
        }
    }
}

void map::grassGrowth()
{
    QVector<QVector<int>> amountGrows(sizeY, QVector<int>(sizeX));

    for ( int j = 0; j < sizeY; ++j ) {
        for ( int i = 0; i < sizeX; ++i ) {
            QList<cell *> listNeigbor = getCellAdress(location(j,i))->neighbors;
            int amountNeigbors = listNeigbor.size();
            int neigborWithGrass = 0;
            int neigborWithWater = 0;
            for ( int k = 0; k < amountNeigbors; ++k ) {
                if (listNeigbor[k]->type == grass) {
                    neigborWithGrass += 1;
                }
                if (listNeigbor[k]->type == water) {
                    neigborWithWater += 1;
                }
            }
            if ( neigborWithWater > 0 ) {
                amountGrows[j][i] = 30;
            } else {
                amountGrows[j][i] = neigborWithGrass * 10 - 30;
            }
        }
    }
    for ( int j = 0; j < sizeY; ++j ) {
        for ( int i = 0; i < sizeX; ++i ) {
            if (fieldMater[j][i].type == ground && amountGrows[j][i] > 0 ) {
                fieldMater[j][i].type = grass;
                fieldMater[j][i].limit = 500;
                fieldMater[j][i].amount = amountGrows[j][i];
            } else if (fieldMater[j][i].type == grass) {
                fieldMater[j][i].amount += amountGrows[j][i];
                if ( fieldMater[j][i].amount < 0 ) {
                    fieldMater[j][i].type = ground;
                    fieldMater[j][i].limit = 0;
                    fieldMater[j][i].amount = 0;
                } else if ( fieldMater[j][i].amount > fieldMater[j][i].limit ) {
                    fieldMater[j][i].amount = fieldMater[j][i].limit;
                }
            }
        }
    }
}

int map::getWidth()
{
    return sizeX;
}

int map::getHeight()
{
    return sizeY;
}

QPointF map::getPoint(int y, int x)
{
    qreal X = 30 + x * cellSize + y % 2 * cellSize / 2.0;
    qreal Y = 30 + y * cellSize * cos(M_PI / 6);

    return QPointF(X, Y);
}

QPointF map::getPoint(location * loc)
{
    int y = loc->getJ();
    int x = loc->getI();
    qreal X = 30 + x * cellSize + y % 2 * cellSize / 2.0;
    qreal Y = 30 + y * cellSize * cos(M_PI / 6);

    return QPointF(X, Y);
}

void oneStep(int direction, int & Y, int & X)
{
    switch (direction) {
    case NW:
        Y -= 1;
        if ( Y % 2 == 1 ) {
            X -= 1;
        }
        break;
    case NE:
        Y -= 1;
        if ( Y % 2 == 0 ) {
            X += 1;
        }
        break;
    case E:
        X -= 1;
        break;
    case SE:
        X += 1;
        break;
    case SW:
        Y += 1;
        if ( Y % 2 == 1 ) {
            X -= 1;
        }
        break;
    case W:
        Y += 1;
        if ( Y % 2 == 0 ) {
            X += 1;
        }
        break;
    }
    if ( X < 0 ) {
        X = 0;
    } else if ( X == 140 ) {
        X = 140 - 1;
    }

    if ( Y < 0 ) {
        Y = 0;
    } else if ( Y == 90 ) {
        Y = 90 - 1;
    }
}

QPolygonF map::getQPolygon(int y, int x)
{
    return fieldGeometry[y][x];
}


QSet<Ant *> map::getTouches(location * loc)
{
    return fieldAnts[loc->getJ()][loc->getI()];

}



void map::tick()
{
    tacktsPass += 1;
    if (tacktsPass == 50) {
        tacktsPass = 0;
        grassGrowth();
        grassGenerate(20, 5);
    }
}



int map::getType(int y, int x)
{
    return fieldMater[y][x].type;
}

int map::getType(location destination)
{
    return fieldMater[destination.getJ()][destination.getI()].type;
}

int map::getPossability(location destination)
{
    return fieldMater[destination.getJ()][destination.getI()].passability;
}

cell *map::getCellAdress(location destination)
{
    return &fieldMater[destination.getJ()][destination.getI()];
}

location *map::getLocationAdress(location destination)
{
    return fieldMater[destination.getJ()][destination.getI()].place;
}


cell map::getCell(int y, int x)
{
    return fieldMater[y][x];
}
