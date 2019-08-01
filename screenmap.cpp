#include "screenmap.h"


ScreenMap::ScreenMap(int height, int width):
    height(height),
    width(width)
{
    fieldMater.resize(height);
    seenTime.resize(height);
    for ( int i = 0; i < height; ++i ) {
        fieldMater[i].resize(width);
        seenTime[i].resize(width);
        for ( int j = 0; j < width; ++j ) {
            fieldMater[i][j].type = unknown;
            seenTime[i][j] = time(0);
            fieldMater[i][j].visible = 0;
        }
    }
}

void ScreenMap::mutalUpdate(ScreenMap * other)
{
    for ( int i = 0; i < height; ++i ) {
        for ( int j = 0; j < width; ++j ) {
            if (this->seenTime[i][j] > other->seenTime[i][j]) {
                other->fieldMater[i][j] = this->fieldMater[i][j];
            } else {
                this->fieldMater[i][j] = other->fieldMater[i][j];
            }
        }
    }
}

void ScreenMap::lookAround(map * world, location locate, int radius)
{
    int pCell = 0;
    QVector<location> cells;
    int x = locate.getX();
    int y = locate.getY();
    for ( int i = 0; i < height; ++i ) {
        for ( int j = 0; j < width; ++j ) {
             fieldMater[i][j].visible = 0;
        }
    }

    cells.push_back(locate);
    for ( int r = 0; r < radius; ++r ) {
        int limit = cells.size();
        for (int cell = pCell; cell < limit; ++cell) {
            for ( int direction = 0; direction < 6; ++direction) {
                x = cells[cell].getX();
                y = cells[cell].getY();
                oneStep(direction, y, x);

                location tmp(y, x);
                bool contains = false;

                for (int i = 0; i < cells.size(); ++i ) {
                    if ( cells[i] == tmp ) {
                        contains = true;
                        break;
                    }
                }

                if ( !contains ) {
                    cells.push_back(tmp);
                    fieldMater[y][x] = world->getCell(y, x);
                    fieldMater[y][x].visible = 1;
                    seenTime[y][x] = time(0);
                }
            }
        }
    }
}

cell ScreenMap::getCell(int y, int x)
{
    return fieldMater[y][x];
}
