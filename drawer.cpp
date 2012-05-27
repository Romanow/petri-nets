#include "drawer.h"

#include <QDebug>
#include <QStack>

PlanarDrawer::PlanarDrawer(StateList * states, QMenu * menu) :
	states(states), menu(menu) {}

PlanarDrawer::~PlanarDrawer() {}

int ** initMatrix(int length)
{
	int ** matrix = new int * [length];
	for (int i = 0; i < length; ++i)
		matrix[i] = new int [length]();

	return matrix;
}

int ** undirectedMatrix(int ** matrix, int length)
{
	int ** undirected = initMatrix(length);
	for (int i = 0; i < length; ++i)
		for (int j = 0; j < length; ++j)
			if (matrix[i][j] > 0)
				undirected[i][j] = undirected[j][i] = matrix[i][j];

	return undirected;
}

QGraphicsScene * PlanarDrawer::draw(QList<DiagramItem *> &items,
									QList<DiagramTransitionItem *> &transitions)
{
	int length = states->count();
	int ** matrix = initMatrix(length);
	for (int i = 0; i < states->count(); ++i)
		foreach (Transition * transition, states->at(i)->outgoing())
		{
			int j = states->indexOf(transition->target());
			matrix[i][j] = 1;
		}

	QList<int> track;
	track.append(0);
	bool used[length];
	for (int i = 0; i < length; ++i)
		used[i] = false;

	// Поиск внутрених граней
	int ** undirected = undirectedMatrix(matrix, length);
	QList<Path> faces = findBaseFaces(undirected, length, 0, used, track);
	for (int i = 0; i < length; ++i)
		delete undirected[i];
	delete undirected;

	// Список ребер исходного графа G
	QList<Edge> edges;
	for (int i = 0; i < length; ++i)
		for (int j = 0; j < length; ++j)
			if (matrix[i][j] > 0)
				edges.append(qMakePair(i, j));

	// Построение списка левых и правых граней для каждой дуги исходного графа G
	QList<Location> edgeLocation = location(edges, faces);

	// Построение графа G*
	int edgeMatrixLength = faces.count() + 2;
	int ** edgeMatrix = initMatrix(edgeMatrixLength);
	foreach (Edge pair, edgeLocation)
		edgeMatrix[pair.first][pair.second] = 1;

	// Удаляем циклы в графе
	int ** m = removeCycles(matrix, length);
	// Топологические уровни исходной матрицы G
	int * level = topologicalSort(m, length);
	// Топологические уровни присоединенной матрицы G*
	int * edgeLevel = topologicalSort(edgeMatrix, edgeMatrixLength);
	for (int i = 0; i < length; ++i)
		delete m[i];
	delete m;

	// Построение горизонтальных и вертикальных реберных отрезков
	int maxEdgeLevel = edgeLevel[0];
	for (int i = 1; i < edgeMatrixLength; ++i)
		if (maxEdgeLevel < edgeLevel[i])
			maxEdgeLevel = edgeLevel[i];

	QList<QLine> vertical, horizontal;
	for (int i = 0; i < length; ++i)
	{
		int x1 = maxEdgeLevel;
		int x2 = 0;
		int y = level[i];
		for (int j = 0; j < length; ++j)
			if (matrix[j][i] > 0)
			{
				int index = edges.indexOf(qMakePair(j, i));
				if (x1 > edgeLevel[edgeLocation[index].first])
					x1 = edgeLevel[edgeLocation[index].first];
				if (x2 < edgeLevel[edgeLocation[index].second])
					x2 = edgeLevel[edgeLocation[index].second];
			}

		if (x1 > x2)
		{
			int temp = x1; x1 = x2; x2 = temp;
		}
		horizontal.append(QLine(x1, y, x2, y));
	}

	int maxLevel = level[0];
	for (int i = 1; i < length; ++i)
		if (maxLevel < level[i])
			maxLevel = level[i];

	for (int i = 0; i < edges.count(); ++i)
	{
		int x = edgeLevel[edgeLocation[i].first];
		int y1 = level[edges[i].first];
		int y2 = level[edges[i].second];
		vertical.append(QLine(x, y1, x, y2));
	}

	QGraphicsScene * scene = new QGraphicsScene(QRectF(0, 0, 120 * maxEdgeLevel, 60 * maxLevel));
//	DiagramGridItem * item = new DiagramGridItem(vertical, horizontal);
//	scene->addItem(item);

	for (int i = 0; i < length; ++i)
	{
		DiagramItem * item = states->at(i)->diagramItem();
		int x = 120 * horizontal[i].x1() + 60 * horizontal[i].dx();
		int y = 60 * horizontal[i].y1();

		item->setPos(x, y);
		if (item->state()->type() == place_node && menu != 0)
		{
			DiagramNetPlaceItem * place = dynamic_cast<DiagramNetPlaceItem *>(item);
			place->setMenu(menu);
		}
		items.append(item);
	}

	for (int i = 0; i < edges.count(); ++i)
	{
		int u = edges[i].first;
		int v = edges[i].second;
		DiagramItem * source = items[u];
		DiagramItem * target = items[v];

		DiagramTransitionItem * transition = new DiagramTransitionItem(source, target);
		transitions.append(transition);
		scene->addItem(transition);
	}

	foreach (DiagramItem * item, items)
		scene->addItem(item);

	for (int i = 0; i < edgeMatrixLength; ++i)
		delete edgeMatrix[i];
	delete edgeMatrix;
	for (int i = 0; i < length; ++i)
		delete matrix[i];
	delete matrix;
	delete edgeLevel;
	delete level;

	return scene;
}

int ** PlanarDrawer::removeCycles(int ** matrix, int length)
{
	int ** m = initMatrix(length);
	bool used[length];
	for (int i = 0; i < length; ++i)
		used[i] = false;

	QList<Edge> track = edgesTrack(matrix, length, 0, used);
	foreach (Edge edge, track)
		m[edge.first][edge.second] = 1;

	return m;
}

QList<Edge> PlanarDrawer::edgesTrack(int ** matrix, int length, int index, bool used[])
{
	QList<Edge> track;

	used[index] = true;
	for (int i = 0; i < length; ++i)
		if (matrix[index][i] > 0 && !used[i])
		{
			track.append(qMakePair(index, i));
			track += edgesTrack(matrix, length, i, used);
		}
	used[index] = false;

	return track;
}

QList<Path> PlanarDrawer::findBaseFaces(int ** matrix, int length, int index, bool used[], QList<int> &track)
{
	used[index] = true;
	QList<Path> faces;

	for (int i = 0; i < length; ++i)
		if (matrix[index][i] > 0)
		{
			if (!used[i])
			{
				track.append(i);

				QList<Path> path = findBaseFaces(matrix, length, i, used, track);
				if (!path.isEmpty())
					faces.append(path);

				if (!track.isEmpty() && track.last() == i)
					track.removeLast();
			}
			else if (track.count() > 2 &&				// Список содержит больше двух вершин (цикл могут образовывать три и более вершин)
					 track.contains(i) &&				// Эта вершина уже была пройдена
					 track.at(track.count() - 2) != i)	// Чтобы не было обратных переходов
			{
				Path path;
				int value, prev = i;
				do
				{
					value = track.takeLast();
					Edge edge = value > prev ? qMakePair(prev, value) : qMakePair(value, prev);
					path.append(edge);

					used[value] = false;
					prev = value;
				}
				while (!track.isEmpty() && value != i);

				faces.append(path);
				// Удаляем первое ребро цикла
				QPair<int, int> edge = path.last();
				matrix[edge.first][edge.second] = 0;
				matrix[edge.second][edge.first] = 0;

//				qDebug() << "Cycle" << path << "\nremoved edge" << edge.first << edge.second;

				// Запускаем обход из текущей вершины
				index = i; i = - 1;
				used[index] = true;
				track.append(index);
			}
		}

	return faces;
}

QList<int> PlanarDrawer::incomingRang(int ** matrix, int length)
{
	QList<int> result;
	result.reserve(length);
	for (int i = 0; i < length; ++i)
	{
		int sum = 0;
		for (int j = 0; j < length; ++j)
			sum += matrix[j][i];
		result.append(sum);
	}

	return result;
}

int * PlanarDrawer::topologicalSort(int ** matrix, int length)
{
	int * level = new int[length];
	QList<int> rang = incomingRang(matrix, length);
	QList<int> used;
	used.reserve(length);

	int currentLevel = 0;
	while (used.length() != length)
	{
		QList<int> tempRang = rang;
		for (int i = 0; i < length; ++i)
		{
			if (rang[i] == 0 && !used.contains(i))
			{
				for (int j = 0; j < length; ++j)
					if (matrix[i][j] > 0)
						tempRang[j] -= 1;

				level[i] = currentLevel;
				used.append(i);
			}
		}

		rang = tempRang;
		currentLevel += 1;
	}

	return level;
}

bool PlanarDrawer::findEdgeLocation(const Path &path, Edge edge, const Boundary &boundary)
{
	while (edge.first != boundary.first.first)
	{
		foreach (Edge e, path)
			if (e.second == edge.first)
			{
				edge = e; break;
			}
	}

	return edge != boundary.first;
}

QList<Location> PlanarDrawer::location(const QList<Edge> &edges, const QList<Path> &faces)
{
	QList<Boundary> boundaries;
	boundaries.reserve(faces.count());
	foreach (Path path, faces)
	{
		// Поиск истока. С учетом того, что вершины в парах отсортированы по возрастанию,
		// истоком будет вершина с наименьшим номеров, стоком - с наибольшим.
		int source = path.first().first;
		foreach (Edge edge, path)
			if (edge.first < source)
				source = edge.first;

		// Ищем правую и левую дуги из истока
		// TODO: левая и правая грани определяются исходя из арифметического номера вершин в переходе.
		// На первом шаге для каждой грани ставится левая и правая дуга.
		// На втором шаге из каждой дуги пытаемся построить путь к источнику. Если последняя дуга в пути
		// соотвестввует левой дуге истока, то для исходной дуги эта грань правая, иначе наоборот.
		Edge left, right;
		bool found = false;
		foreach (Edge edge, path)
		{
			if (edge.first == source)
				if (found)
				{
					if (left.second > edge.second)
					{
						right = left; left = edge;
					}
					else
						right = edge;
				}
				else
				{
					left = edge;
					found = true;
				}
		}

		boundaries.append(qMakePair(left, right));
	}

	// Для каждого ребра ставим в соотвествие левую и правую грань.
	// Всего граней faces.count + 2:
	// 0 - левая внешняя грань
	// 1..faces.count - внутрение грани
	// faces.count + 1 - правая внешняя грань
	QList<Location> edgeLocation;
	edgeLocation.reserve(edges.count());
	for (int i = 0; i < edges.count(); ++i)
	{
		int leftFace, rightFace;
		bool left = false, right = false;
		for (int  j = 0; j < faces.count(); ++j)
		{
			// Если грань содержит дугу
			if (faces[j].indexOf(edges[i]) != - 1)
			{
				if (findEdgeLocation(faces[j], edges[i], boundaries[j]))
				{
					if (!left)
					{
						left = true;
						leftFace = j + 1;
					}
				}
				else
				{
					if (!right)
					{
						right = true;
						rightFace = j + 1;
					}
				}
			}
		}

		// Если не все грани определены
		if (!left)
			leftFace = 0;
		if (!right)
			rightFace = faces.count() + 1;

		edgeLocation.append(qMakePair(leftFace, rightFace));
	}

	return edgeLocation;
}
