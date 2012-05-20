#ifndef DRAWER_H
#define DRAWER_H

#include <QMenu>
#include <QGraphicsScene>

#include "state.h"
#include "diagramitem.h"

typedef QPair<int, int> Edge;
typedef QList<Edge> Path;
typedef QPair<Edge, Edge> Boundary;
typedef QPair<int, int> Location;

class Drawer
{
public:
	virtual QGraphicsScene * draw() = 0;
};

class PlanarDrawer : public Drawer
{
public:
	PlanarDrawer(StateList * states, QMenu * menu);
	~PlanarDrawer();

	QGraphicsScene * draw();

private:
	QMenu * menu;
	StateList * states;

	int ** removeCycles(int ** matrix, int length);
	QList<Edge> edgesTrack(int ** matrix, int length, int index, bool used[]);
	QList<Path> findBaseFaces(int ** matrix, int length, int index, bool used[], QList<int> &track);
	QList<int> incomingRang(int ** matrix, int length);
	int * topologicalSort(int ** matrix, int length);
	bool findEdgeLocation(const Path &path, Edge edge, const Boundary &boundary);
	QList<Location> location(const QList<Edge> &edges, const QList<Path> &faces);
};

#endif // DRAWER_H
