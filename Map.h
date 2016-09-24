// Map.h ... interface to Map data type

#ifndef MAP_H
#define MAP_H

#include "Places.h"

typedef struct edge{
    LocationID  start;
    LocationID  end;
    TransportID type;
} Edge;

// graph representation is hidden
typedef struct MapRep *Map;

// operations on graphs

int *getConnections(LocationID from, int player, int round,
                   int road, int rail, int sea);
Map  newMap();
void disposeMap(Map g);
void showMap(Map g);
int  numV(Map g);
int  numE(Map g, TransportID t);

#endif
