#include "graphicview.h"
#include "translate.h"
#include "petrinet.h"
#include "rpn.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

View::View(QWidget * parent) : QWidget(parent) {}

View::~View()
{
	clear();
}

void View::clear()
{
	qDeleteAll(transitions);
	transitions.clear();
	qDeleteAll(items);
	items.clear();
}

DiagramView::DiagramView(QWidget * parent) : View(parent)
{
	initInterface();
	initConnections();
}

DiagramView::~DiagramView() {}

void DiagramView::initInterface()
{
	view = new QGraphicsView(this);
	view->setRenderHint(QPainter::Antialiasing, true);
	view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	view->setDragMode(QGraphicsView::ScrollHandDrag);

	QHBoxLayout * hLayout = new QHBoxLayout;
	QVBoxLayout * vLayout = new QVBoxLayout;
	btnConvertToColouredNet = new QPushButton(this);
	// Fixme: libpng warning: Ignoring attempt to set cHRM RGB triangle with zero area
	btnConvertToColouredNet->setIcon(QIcon(":Play"));
	btnConvertToColouredNet->setToolTip(conv("Преобразование в раскрашенную сеть Петри"));
	btnConvertToColouredNet->setIconSize(QSize(48, 48));
	btnConvertToColouredNet->setFixedSize(QSize(48, 48));
	btnConvertToSimpleNet = new QPushButton(this);
	btnConvertToSimpleNet->setIcon(QIcon(":Play"));
	btnConvertToSimpleNet->setToolTip(conv("Преобразование в сеть Петри"));
	btnConvertToSimpleNet->setIconSize(QSize(48, 48));
	btnConvertToSimpleNet->setFixedSize(QSize(48, 48));
	vLayout->addStretch();
	vLayout->addWidget(btnConvertToSimpleNet);
	vLayout->addWidget(btnConvertToColouredNet);
	vLayout->addStretch();

	hLayout->addWidget(view);
	hLayout->addLayout(vLayout);
	setLayout(hLayout);
}

void DiagramView::initConnections()
{
	connect(btnConvertToSimpleNet, SIGNAL(clicked()), SIGNAL(convertToSimpleNet()));
	connect(btnConvertToColouredNet, SIGNAL(clicked()), SIGNAL(convertToColouredNet()));
}

void DiagramView::drawDiagram(PlanarDrawer * drawer)
{
	scene = drawer->draw(items, transitions);
	view->setScene(scene);
	scene->update();
}

NetworkView::NetworkView(QWidget * parent) : View(parent)
{
	execution = false;

	initInterface();
	initConnections();
}

NetworkView::~NetworkView() {}

void NetworkView::initInterface()
{
	timer = new QTimer;
	timer->setInterval(1000);

	view = new QGraphicsView(this);
	view->setRenderHint(QPainter::Antialiasing, true);
	view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	view->setDragMode(QGraphicsView::ScrollHandDrag);

	QVBoxLayout * vLayout = new QVBoxLayout;
	QHBoxLayout * hLayout = new QHBoxLayout;
	btnPlay = new QPushButton(this);
	btnPlay->setIcon(QIcon(":Play"));
	btnPlay->setToolTip(conv("Моделирование работы сети"));
	btnPlay->setIconSize(QSize(48, 48));
	btnPlay->setFixedSize(48, 48);

	btnStop = new QPushButton(this);
	btnStop->setIcon(QIcon(":Stop"));
	btnStop->setToolTip(conv("Сброс результатов моделирования"));
	btnStop->setIconSize(QSize(48, 48));
	btnStop->setFixedSize(48, 48);

	btnRefresh = new QPushButton(this);
	btnRefresh->setIcon(QIcon(":Refresh"));
	btnRefresh->setToolTip(conv("Задание новой разметки сети"));
	btnRefresh->setIconSize(QSize(48, 48));
	btnRefresh->setFixedSize(48, 48);
	hLayout->addStretch();
	hLayout->addWidget(btnPlay);
	hLayout->addWidget(btnStop);
	hLayout->addStretch();
	hLayout->addWidget(btnRefresh);


	vLayout->addWidget(view);
	vLayout->addLayout(hLayout);
	setLayout(vLayout);
}

void NetworkView::initConnections()
{
	connect(btnPlay, SIGNAL(clicked()), SLOT(play()));
	connect(btnStop, SIGNAL(clicked()), SLOT(stop()));
	connect(btnStop, SIGNAL(clicked()), SIGNAL(reinitMarking()));
	connect(btnRefresh, SIGNAL(clicked()), SIGNAL(reinitValues()));
	connect(timer, SIGNAL(timeout()), SLOT(timeout()));
}

void NetworkView::play()
{
	if (!execution)
	{
		btnPlay->setIcon(QIcon(":Pause"));
		btnPlay->setToolTip(conv("Остановка моделирования"));
		timer->start();
	}
	else
	{
		btnPlay->setIcon(QIcon(":Play"));
		btnPlay->setToolTip(conv("Моделирование работы сети"));
		timer->stop();
	}
	execution = !execution;
}

void NetworkView::stop()
{
	btnPlay->setIcon(QIcon(":Play"));
	btnPlay->setToolTip(conv("Моделирование работы сети"));
	execution = false;

	scene->update();
	timer->stop();
}


void NetworkView::timeout()
{
	QList<NetPlace *> activePlaces;
	QList<State *> places = netStates->find(place_node);
	QMultiMap<NetPlace *, NetTransition *> activeTransitions;

	foreach (State * state, places)
	{
		NetPlace * place = dynamic_cast<NetPlace *>(state);
		if (place->marking() > 0)
			activePlaces.append(place);
	}

	foreach (NetPlace * place, activePlaces)
	{
		foreach (Transition * tr, place->outgoing())
		{
			bool flag = true;
			NetTransition * transition = dynamic_cast<NetTransition *>(tr->target());
			for (int i = 0; i < transition->incoming().count() && flag; ++i)
			{
				NetPlace * place = dynamic_cast<NetPlace *>(transition->incoming()[i]->source());

				if (place->marking() != 0)
				{
					QString expression = place->substituteValues(transition->incoming()[i]->guard());
					if (!expression.isEmpty())
					{
						flag = RPN::calculate(expression);
					}
				}
				else
					flag = false;
			}

			if (flag && activeTransitions.keys(transition).isEmpty())
				activeTransitions.insert(place, transition);
		}
	}

	foreach (NetPlace * place, activeTransitions.uniqueKeys())
	{
		QList<NetTransition *> list = activeTransitions.values(place);
		int number = rand() % list.count();
		NetTransition * transition = list[number];

		// original place
		Marking * marking;
		foreach (Transition * tr, transition->incoming())
		{
			NetPlace * pl = dynamic_cast<NetPlace *>(tr->source());
			marking = pl->takeMarking();
		}

		if (!transition->expression().isEmpty())
		{
			double result = RPN::calculate(marking->substituteValues(transition->expression()));
			qDebug() << transition->expression() << result;
			marking->addVariableValue(transition->result(), result);
		}

		// next place
		foreach (Transition * tr, transition->outgoing())
		{
			NetPlace * pl = dynamic_cast<NetPlace *>(tr->target());
			pl->addMarking(marking);
		}
	}

	if (activeTransitions.isEmpty())
	{
		stop();

		State * final = states->find(final_state).first();
		State * state = netStates->find(QString("%1_place").arg(final->id())).first();
		NetPlace * place = dynamic_cast<NetPlace *>(state);
		if (place->marking() == 0)
		{
			QString placeList;
			foreach (State * state, netStates->find(place_node))
			{
				NetPlace * place = dynamic_cast<NetPlace *>(state);
				if (place->marking() > 0)
					placeList += place->id() + ", ";
			}

			QMessageBox::information(this, conv("Результат моделирования"),
									 conv("Сеть не завершила работу,\nв позициях (%1) находятся фишки").arg(placeList.left(placeList.length() - 2)));
		}
		else
		{
			QMessageBox::information(this, conv("Результат моделирования"), conv("В процессе моделирования сеть пришла в конечное состояние"));
		}
	}

	scene->update();
}

void NetworkView::drawDiagram(PlanarDrawer * drawer)
{
	scene = drawer->draw(items, transitions);
	view->setScene(scene);
	scene->update();
}

void NetworkView::setNetwork(StateList * states, StateList * netStates)
{
	this->states = states;
	this->netStates = netStates;
}
