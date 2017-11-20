#include "widget.h"
#include "ui_widget.h"
#include "rect.h"
#include "view.h"
#include "Military/battlefield.h"
#include <QGraphicsScene>
#include <qDebug>
#include <QBrush>
#include <QKeyEvent>

Rect * rect1, * rect2;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    // Устанавливаем минимальный размер окна (1280 х 720)
    this->setMinimumSize(1280, 720);

    // Фиксируем высоту
    setFixedHeight(minimumHeight());

    // Создаём и настраиваем сцену
    QGraphicsScene * scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 2510, 343); // Размер сцены: 2510 x 343
    scene->setBackgroundBrush(QBrush(QImage(":/images/images/background.png")));

    // Создаём и настраиваем view's
    view = new View(false, this); // view = new View(false)
    view_2 = new View(true, this); // view_2 = new View(true)
    ui->verticalLayout->addWidget(view);
    ui->verticalLayout->addWidget(view_2);
    view->setScene(scene);
    view_2->setScene(scene);
    view->setSceneRect(0, 0, 1255, 343);
    view_2->setSceneRect(1255, 0, 1255, 343);
    view->installEventFilter(this);
    view_2->installEventFilter(this);

    btf = new Battlefield();
    btf->setScene(scene);

    // Временные объекты сцены
    rect1 = new Rect();
    rect2 = new Rect();
    rect1->setPos(scene->width() - 101, scene->height() - 120);
    rect2->setPos(0, scene->height() - 120);
    scene->addItem(rect1);
    scene->addItem(rect2);

    // Эти две функции обязательно вызывать только после того, как добавлены другие элементы на сцену, чтобы меню всегда были на первом плане
    view->setConfiguration();
    view_2->setConfiguration();

    viewWithOpenMenu = NULL;

    // Коннэктим таймеры view's к слоту, чтобы по таймеру закрывалось меню игрока, у которого оно открыто
    connect(view, SIGNAL(menuVisibleStatusChanged(View*)), this, SLOT(updateViewWithOpenMenu(View*)));
    connect(view_2, SIGNAL(menuVisibleStatusChanged(View*)), this, SLOT(updateViewWithOpenMenu(View*)));
}

Widget::~Widget()
{
    delete ui;
    delete btf;
}

void Widget::updateViewWithOpenMenu(View * sender)
{
    // Если view-отправитель сигнала таймера об истечении времени нахождения в меню - это нынешний view с открытым меню

    if(viewWithOpenMenu == sender)
        viewWithOpenMenu = NULL;
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    //qDebug() << "\teventFilter";
    if (watched == view || watched == view_2)
    {
        //qDebug() << "\t\teventFilter";
        //qDebug() << "\t\t\t" << event->type();
        if (event->type() == QEvent::KeyPress)
        {
            //qDebug() << "\t\t\teventFilter";
            if((view->isControlKey(((QKeyEvent *)event)->nativeVirtualKey()) || view->isControlKey(((QKeyEvent *)event)->key())) && (view == viewWithOpenMenu || viewWithOpenMenu == NULL) && view->isCanMenuOpen())
            {
                //qDebug() << "\t\t\t\teventFilter view";
                if(viewWithOpenMenu == NULL)
                    viewWithOpenMenu = view;
                view->setFocus();
                view->event(event);
                return true;
            }
            if((view_2->isControlKey(((QKeyEvent *)event)->nativeVirtualKey()) || view_2->isControlKey(((QKeyEvent *)event)->key())) && (view_2 == viewWithOpenMenu || viewWithOpenMenu == NULL) && view_2->isCanMenuOpen())
            {
                //qDebug() << "\t\t\t\teventFilter view_2";
                if(viewWithOpenMenu == NULL)
                    viewWithOpenMenu = view_2;
                view_2->setFocus();
                view_2->event(event);
                return true;
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}
