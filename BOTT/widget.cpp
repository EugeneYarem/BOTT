#include "widget.h"
#include "ui_widget.h"
#include "view.h"
#include "Military/battlefield.h"
#include "Military/army.h"
#include "message.h"
#include "dialog.h"
#include "town.h"
#include <QGraphicsScene>
#include <qDebug>
#include <QBrush>
#include <QKeyEvent>
#include <QKeySequence>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    gameDuration = 0;
    wastedMoneyP1 = 0;
    wastedMoneyP2 = 0;
    countOfUnitsP1 = 0;
    countOfUnitsP2 = 0;
    countOfModicationP1 = 0;
    countOfModicationP2 = 0;

    // Устанавливаем минимальный размер окна (1280 х 720)
    this->setMinimumSize(1280, 720);

    // Фиксируем высоту
    setFixedHeight(minimumHeight());

    setMaximumWidth(1280);

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

    installEventFilters();

    // Создаём и настраиваем поле боя
    btf = new Battlefield();
    btf->setScene(scene);
    btf->setArmies(view->getArmy(), view_2->getArmy());

    lastVisitedPage = 1;
    eventEvoke = false;

    // Эти две функции обязательно вызывать только после того, как добавлены другие элементы на сцену, чтобы меню всегда были на первом плане
    view->setConfiguration();
    view_2->setConfiguration();
    earnedMoneyP1 = view->getTown()->getMoney();
    earnedMoneyP2 = view_2->getTown()->getMoney();

    // Коннэкты для сбора статистики о доходах и затратах
    connect(view, SIGNAL(moneyWasted(int)), this, SLOT(wastedMoneyP1Plus(int)));
    connect(view_2, SIGNAL(moneyWasted(int)), this, SLOT(wastedMoneyP2Plus(int)));
    connect(view->getTown(), SIGNAL(moneyEarned(int)), this, SLOT(earnedMoneyP1Plus(int)));
    connect(view_2->getTown(), SIGNAL(moneyEarned(int)), this, SLOT(earnedMoneyP2Plus(int)));
    connect(view->getTown(), SIGNAL(moneyWasted(int)), this, SLOT(wastedMoneyP1Plus(int)));
    connect(view_2->getTown(), SIGNAL(moneyWasted(int)), this, SLOT(wastedMoneyP2Plus(int)));
    connect(view->getArmy(), SIGNAL(moneyWasted(int)), this, SLOT(wastedMoneyP1Plus(int)));
    connect(view_2->getArmy(), SIGNAL(moneyWasted(int)), this, SLOT(wastedMoneyP2Plus(int)));
    connect(view->getArmy(), SIGNAL(uniteCreated()), this, SLOT(countOfUnitsP1Plus()));
    connect(view_2->getArmy(), SIGNAL(uniteCreated()), this, SLOT(countOfUnitsP2Plus()));
    connect(view->getArmy(), SIGNAL(modificate()), this, SLOT(countOfModicationP1Plus()));
    connect(view_2->getArmy(), SIGNAL(modificate()), this, SLOT(countOfModicationP2Plus()));

    viewWithOpenMenu = NULL;

    // Коннэктим таймеры view's к слоту, чтобы по таймеру закрывалось меню игрока, у которого оно открыто
    connect(view, SIGNAL(menuVisibleStatusChanged(View*)), this, SLOT(updateViewWithOpenMenu(View*)));
    connect(view_2, SIGNAL(menuVisibleStatusChanged(View*)), this, SLOT(updateViewWithOpenMenu(View*)));

    createSettingsPage();

    ui->stackedWidget->setCurrentIndex(1);
}

Widget::~Widget()
{
    delete ui;
    delete btf;
}

void Widget::setGamerNameP1(QString name)
{
    gamerNameP1 = name;
}

void Widget::setGamerNameP2(QString name)
{
    gamerNameP2 = name;
}

void Widget::startNewGame()
{
    startAllTimers();
    ui->stackedWidget->setCurrentIndex(0);
    clearFocusOfMainMenu();
    setMaximumWidth(16777215);
}

void Widget::updateViewWithOpenMenu(View * sender)
{
    // Если view-отправитель сигнала таймера об истечении времени нахождения в меню - это нынешний view с открытым меню

    if(viewWithOpenMenu == sender)
        viewWithOpenMenu = NULL;
}

void Widget::earnedMoneyP1Plus(int income)
{
    earnedMoneyP1 += income;
}

void Widget::earnedMoneyP2Plus(int income)
{
    earnedMoneyP2 += income;
}

void Widget::wastedMoneyP1Plus(int wasted)
{
    wastedMoneyP1 += wasted;
}

void Widget::wastedMoneyP2Plus(int wasted)
{
    wastedMoneyP2 += wasted;
}

void Widget::countOfUnitsP1Plus()
{
    countOfUnitsP1++;
}

void Widget::countOfUnitsP2Plus()
{
    countOfUnitsP2++;
}

void Widget::countOfModicationP1Plus()
{
    countOfModicationP1++;
}

void Widget::countOfModicationP2Plus()
{
    countOfModicationP2++;
}

void Widget::createSettingsPage()
{
    ui->lineEditMenu->setText(QString(view->getControlKey("menu")));
    ui->lineEditMenu_2->setText(QString(view_2->getControlKey("menu")));

    ui->lineEditChoose->setText(QString(view->getControlKey("menu select")));
    ui->lineEditChoose_2->setText(QKeySequence(view_2->getControlKey("menu select")).toString()); // QString(view_2->getControlKey("menu select"))

    ui->lineEditExit->setText(QString(view->getControlKey("exit from menu")));
    ui->lineEditExit_2->setText(QKeySequence(view_2->getControlKey("exit from menu")).toString()); // QString(view_2->getControlKey("exit from menu"))

    ui->lineEditUp->setText(QString(view->getControlKey("menu up")));
    ui->lineEditUp_2->setText(QKeySequence(view_2->getControlKey("menu up")).toString()); // QString(view_2->getControlKey("menu up"))

    ui->lineEditDown->setText(QString(view->getControlKey("menu down")));
    ui->lineEditDown_2->setText(QKeySequence(view_2->getControlKey("menu down")).toString()); // QString(view_2->getControlKey("menu down"))

    ui->lineEditSoldier->setText(QString(view->getControlKey("create soldier")));
    ui->lineEditSoldier_2->setText(QString(view_2->getControlKey("create soldier")));

    ui->lineEditArcher->setText(QString(view->getControlKey("create archer")));
    ui->lineEditArcher_2->setText(QString(view_2->getControlKey("create archer")));

    ui->lineEditRider->setText(QString(view->getControlKey("create rider")));
    ui->lineEditRider_2->setText(QString(view_2->getControlKey("create rider")));

    ui->lineEditWizard->setText(QString(view->getControlKey("create wizard")));
    ui->lineEditWizard_2->setText(QString(view_2->getControlKey("create wizard")));
}

bool Widget::isSettingLineEdit(QObject *watched)
{
    if(watched == ui->lineEditMenu || watched == ui->lineEditMenu_2 ||
       watched == ui->lineEditChoose || watched == ui->lineEditChoose_2 ||
       watched == ui->lineEditExit || watched == ui->lineEditExit_2 ||
       watched == ui->lineEditUp || watched == ui->lineEditUp_2 ||
       watched == ui->lineEditDown || watched == ui->lineEditDown_2 ||
       watched == ui->lineEditSoldier || watched == ui->lineEditSoldier_2 ||
       watched == ui->lineEditArcher || watched == ui->lineEditArcher_2 ||
       watched == ui->lineEditRider || watched == ui->lineEditRider_2 ||
       watched == ui->lineEditWizard || watched == ui->lineEditWizard_2)
        return true;
    else return false;
}

void Widget::installEventFilters()
{
    view->installEventFilter(this);
    view_2->installEventFilter(this);
    ui->lineEditMenu->installEventFilter(this);
    ui->lineEditMenu_2->installEventFilter(this);
    ui->lineEditChoose->installEventFilter(this);
    ui->lineEditChoose_2->installEventFilter(this);
    ui->lineEditExit->installEventFilter(this);
    ui->lineEditExit_2->installEventFilter(this);
    ui->lineEditUp->installEventFilter(this);
    ui->lineEditUp_2->installEventFilter(this);
    ui->lineEditDown->installEventFilter(this);
    ui->lineEditDown_2->installEventFilter(this);
    ui->lineEditSoldier->installEventFilter(this);
    ui->lineEditSoldier_2->installEventFilter(this);
    ui->lineEditArcher->installEventFilter(this);
    ui->lineEditArcher_2->installEventFilter(this);
    ui->lineEditRider->installEventFilter(this);
    ui->lineEditRider_2->installEventFilter(this);
    ui->lineEditWizard->installEventFilter(this);
    ui->lineEditWizard_2->installEventFilter(this);

    ui->buttonContinue->installEventFilter(this);
    ui->buttonNew->installEventFilter(this);
    ui->buttonSettings->installEventFilter(this);
    ui->buttonExit->installEventFilter(this);
    ui->buttonEnd->installEventFilter(this);
    ui->buttonStatistics->installEventFilter(this);
}

bool Widget::isLineEditOfFirstPlayer(QObject * watched)
{
    if(watched == ui->lineEditMenu || watched == ui->lineEditChoose ||
       watched == ui->lineEditExit || watched == ui->lineEditUp ||
       watched == ui->lineEditDown || watched == ui->lineEditSoldier ||
       watched == ui->lineEditArcher || watched == ui->lineEditRider ||
       watched == ui->lineEditWizard)
        return true;
    else return false;
}

void Widget::clearFocusOfMainMenu()
{
    ui->buttonContinue->clearFocus();
    //this->eventFilter(ui->buttonContinue, new QEvent(QEvent::FocusOut));
    //ui->buttonContinue->eventFilter(this, new QEvent(QEvent::FocusOut));
    ui->buttonNew->clearFocus();
    ui->buttonSettings->clearFocus();
    ui->buttonStatistics->clearFocus();
    ui->buttonEnd->clearFocus();
    ui->buttonExit->clearFocus();
}

void Widget::stopAllTimers()
{
    if(!eventEvoke)
    {
        view->stopAllTimers();
        view_2->stopAllTimers();
        btf->stopAllTimers();

        eventEvoke = true;
    }
}

void Widget::startAllTimers()
{
    view->startAllTimers();
    view_2->startAllTimers();
    btf->startAllTimers();

    eventEvoke = false;
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->buttonContinue)
    {
        if(event->type() == QEvent::FocusIn)
            ui->buttonContinue->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/continueFocused.png);");
        if(event->type() == QEvent::FocusOut)
            ui->buttonContinue->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/continue.png);");
        if(event->type() == QEvent::HoverEnter)
            ui->buttonContinue->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/continueFocused.png);");
        if(event->type() == QEvent::HoverLeave)
            if(!ui->buttonContinue->hasFocus())
                ui->buttonContinue->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/continue.png);");
        return false;
    }

    if(watched == ui->buttonNew)
    {
        if(event->type() == QEvent::FocusIn)
            ui->buttonNew->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/newFocused.png);");
        if(event->type() == QEvent::FocusOut)
            ui->buttonNew->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/new.png);");
        if(event->type() == QEvent::HoverEnter)
            ui->buttonNew->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/newFocused.png);");
        if(event->type() == QEvent::HoverLeave)
            if(!ui->buttonNew->hasFocus())
                ui->buttonNew->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/new.png);");
        return false;
    }

    if(watched == ui->buttonSettings)
    {
        if(event->type() == QEvent::FocusIn)
            ui->buttonSettings->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/settingsFocused.png);");
        if(event->type() == QEvent::FocusOut)
            ui->buttonSettings->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/settings.png);");
        if(event->type() == QEvent::HoverEnter)
            ui->buttonSettings->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/settingsFocused.png);");
        if(event->type() == QEvent::HoverLeave)
            if(!ui->buttonSettings->hasFocus())
                ui->buttonSettings->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/settings.png);");
        return false;
    }

    if(watched == ui->buttonStatistics)
    {
        if(event->type() == QEvent::FocusIn)
            ui->buttonStatistics->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/statisticsFocused.png);");
        if(event->type() == QEvent::FocusOut)
            ui->buttonStatistics->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/statistics.png);");
        if(event->type() == QEvent::HoverEnter)
            ui->buttonStatistics->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/statisticsFocused.png);");
        if(event->type() == QEvent::HoverLeave)
            if(!ui->buttonStatistics->hasFocus())
                ui->buttonStatistics->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/statistics.png);");
        return false;
    }

    if(watched == ui->buttonEnd)
    {
        if(event->type() == QEvent::FocusIn)
            ui->buttonEnd->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/finishFocused.png);");
        if(event->type() == QEvent::FocusOut)
            ui->buttonEnd->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/finish.png);");
        if(event->type() == QEvent::HoverEnter)
            ui->buttonEnd->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/finishFocused.png);");
        if(event->type() == QEvent::HoverLeave)
            if(!ui->buttonEnd->hasFocus())
                ui->buttonEnd->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/finish.png);");
        return false;
    }

    if(watched == ui->buttonExit)
    {
        if(event->type() == QEvent::FocusIn)
            ui->buttonExit->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/exitFocused.png);");
        if(event->type() == QEvent::FocusOut)
            ui->buttonExit->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/exit.png);");
        if(event->type() == QEvent::HoverEnter)
            ui->buttonExit->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/exitFocused.png);");
        if(event->type() == QEvent::HoverLeave)
            if(!ui->buttonExit->hasFocus())
                ui->buttonExit->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/exit.png);");
        return false;
    }

    if(isSettingLineEdit(watched))
    {
        if (event->type() == QEvent::KeyPress)
        {
            /*
             * QEvent::nativeKey() умеет принять код английской клавиши даже при другой раскладке клавиатуры, но неправильно реагирует на некоторые кнопки
             * QEvent::key() правильно реагирует на все кнопки, но считывает код клавиши с буквами на том языке, в котором сейчас установлена раскладка клавиатуры
             * Для понимания дальнейших проверок стоит раскомментировать следующие три строки
            */
            //qDebug() << QKeySequence(key).toString();
            //qDebug() << QKeySequence(nativeKey).toString();
            //qDebug() << ((QKeyEvent *)event)->text();

            Qt::Key key = (Qt::Key)(((QKeyEvent *)event)->key());
            Qt::Key nativeKey = (Qt::Key)(((QKeyEvent *)event)->nativeVirtualKey());
            QString text = ((QKeyEvent *)event)->text();
            if(text.length() == 1)
                text = text.toLower();

            // Проверка на выбор Shift, Alt, Ctrl, Windows, Esc в роли клавиш управления
            /*
             * Если одна нажатая клавиша - одна из них, то мы кидаем false для того,
             * чтобы это событие полетело к watched. Watched их не обработает и кинет их в функцию Widget::event().
             * false возвращается для того, чтобы среагировать на нажатие Esc мог только Widget.
             * Для понимания ситуации поставь return true. Во время игры зайди в настройки, нажми Esc, затем снова Esc
            */
            if(QKeySequence(nativeKey).toString() == "\u0010" || QKeySequence(nativeKey).toString() == "\u0011" ||  QKeySequence(nativeKey).toString() == "\u0012" || QKeySequence(nativeKey).toString() == "[" || QKeySequence(nativeKey).toString() == "\u001B")
                return false;

            if(QKeySequence(nativeKey).toString() >= QKeySequence(key).toString() && QKeySequence(nativeKey).toString().length() >= QKeySequence(key).toString().length()
               && QKeySequence(key).toString() != text && QKeySequence(key).toString() != text.toUpper())
            {
                if(isLineEditOfFirstPlayer(watched))
                {
                    if(!view->checkControlKey(nativeKey) && !view_2->checkControlKey(nativeKey))
                        view->setControlKey(view->getValueByControlKey(((QLineEdit *)watched)->text()), nativeKey);
                    else
                    {
                        Message message(this);
                        message.setMessage("Эта кнопка уже используется.");
                        message.show();
                        message.exec();
                        return true;
                    }
                }
                else
                {
                    if(!view->checkControlKey(nativeKey) && !view_2->checkControlKey(nativeKey))
                        view_2->setControlKey(view_2->getValueByControlKey(((QLineEdit *)watched)->text()), nativeKey);
                    else
                    {
                        Message message(this);
                        message.setMessage("Эта кнопка уже используется.");
                        message.show();
                        message.exec();
                        return true;
                    }
                }

                ((QLineEdit *)watched)->setText(QKeySequence(nativeKey).toString());
                ((QLineEdit *)watched)->clearFocus();
                return true;
            }
            if(QKeySequence(nativeKey).toString().length() < QKeySequence(key).toString().length())
            {
                if(isLineEditOfFirstPlayer(watched))
                {
                    if(!view->checkControlKey(key) && !view_2->checkControlKey(key))
                        view->setControlKey(view->getValueByControlKey(((QLineEdit *)watched)->text()), key);
                    else
                    {
                        Message message(this);
                        message.setMessage("Эта кнопка уже используется.");
                        message.show();
                        message.exec();
                        return true;
                    }
                }
                else
                {
                    if(!view->checkControlKey(key) && !view_2->checkControlKey(key))
                        view_2->setControlKey(view_2->getValueByControlKey(((QLineEdit *)watched)->text()), key);
                    else
                    {
                        Message message(this);
                        message.setMessage("Эта кнопка уже используется.");
                        message.show();
                        message.exec();
                        return true;
                    }
                }

                ((QLineEdit *)watched)->setText(QKeySequence(key).toString());
                ((QLineEdit *)watched)->clearFocus();
                return true;
            }
            if(QKeySequence(key).toString() == text)
            {
                if(isLineEditOfFirstPlayer(watched))
                {
                    if(!view->checkControlKey(key) && !view_2->checkControlKey(key))
                        view->setControlKey(view->getValueByControlKey(((QLineEdit *)watched)->text()), key);
                    else
                    {
                        Message message(this);
                        message.setMessage("Эта кнопка уже используется.");
                        message.show();
                        message.exec();
                        return true;
                    }
                }
                else
                {
                    if(!view->checkControlKey(key) && !view_2->checkControlKey(key))
                        view_2->setControlKey(view_2->getValueByControlKey(((QLineEdit *)watched)->text()), key);
                    else
                    {
                        Message message(this);
                        message.setMessage("Эта кнопка уже используется.");
                        message.show();
                        message.exec();
                        return true;
                    }
                }

                ((QLineEdit *)watched)->setText(QKeySequence(key).toString());
                ((QLineEdit *)watched)->clearFocus();
                return true;
            }
            if(QKeySequence(key).toString() != text && QKeySequence(key).toString() == text.toUpper())
            {
                if(isLineEditOfFirstPlayer(watched))
                {
                    if(!view->checkControlKey(nativeKey) && !view_2->checkControlKey(nativeKey))
                        view->setControlKey(view->getValueByControlKey(((QLineEdit *)watched)->text()), nativeKey);
                    else
                    {
                        Message message(this);
                        message.setMessage("Эта кнопка уже используется.");
                        message.show();
                        message.exec();
                        return true;
                    }
                }
                else
                {
                    if(!view->checkControlKey(nativeKey) && !view_2->checkControlKey(nativeKey))
                        view_2->setControlKey(view_2->getValueByControlKey(((QLineEdit *)watched)->text()), nativeKey);
                    else
                    {
                        Message message(this);
                        message.setMessage("Эта кнопка уже используется.");
                        message.show();
                        message.exec();
                        return true;
                    }
                }

                ((QLineEdit *)watched)->setText(QKeySequence(nativeKey).toString());
                ((QLineEdit *)watched)->clearFocus();
                return true;
            }
            return true;
        }
    }
    if (watched == view || watched == view_2)
    {
        if (event->type() == QEvent::KeyPress)
        {
            if(((view->isControlKey(((QKeyEvent *)event)->nativeVirtualKey()) || view->isControlKey(((QKeyEvent *)event)->key())) && (view == viewWithOpenMenu || viewWithOpenMenu == NULL) && view->isCanMenuOpen())
              || view->isShortcut(((QKeyEvent *)event)->nativeVirtualKey()) || view->isShortcut(((QKeyEvent *)event)->key()))
            {
                if(!view->isShortcut(((QKeyEvent *)event)->nativeVirtualKey()) && !view->isShortcut(((QKeyEvent *)event)->key()) && viewWithOpenMenu == NULL)
                    viewWithOpenMenu = view;
                view->setFocus();
                view->event(event);
                return true;
            }
            if(((view_2->isControlKey(((QKeyEvent *)event)->nativeVirtualKey()) || view_2->isControlKey(((QKeyEvent *)event)->key())) && (view_2 == viewWithOpenMenu || viewWithOpenMenu == NULL) && view_2->isCanMenuOpen())
              || view_2->isShortcut(((QKeyEvent *)event)->nativeVirtualKey()) || view_2->isShortcut(((QKeyEvent *)event)->key()))
            {
                if(!view_2->isShortcut(((QKeyEvent *)event)->nativeVirtualKey()) && !view_2->isShortcut(((QKeyEvent *)event)->key()) && viewWithOpenMenu == NULL)
                    viewWithOpenMenu = view_2;
                view_2->setFocus();
                view_2->event(event);
                return true;
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

void Widget::on_buttonSettings_pressed()
{
    lastVisitedPage = 1;
    lastVisitedPage = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(2);
}

void Widget::on_buttonContinue_pressed()
{
    lastVisitedPage = 1;
    startAllTimers();
    ui->stackedWidget->setCurrentIndex(0);
    clearFocusOfMainMenu();
    setMaximumWidth(16777215);
}

bool Widget::event(QEvent *event)
{
    if(((QKeyEvent *)event)->key() == Qt::Key_Escape)
    {
        if(ui->stackedWidget->currentIndex() == 0)
            stopAllTimers();

        clearFocusOfMainMenu();
        int temp = lastVisitedPage;
        lastVisitedPage = ui->stackedWidget->currentIndex();
        ui->stackedWidget->setCurrentIndex(temp);

        if(temp == 0)
        {
            setMaximumWidth(16777215);
            startAllTimers();
        }
        else setMaximumWidth(1280);

        return true;
    }
    return QWidget::event(event);
}

void Widget::on_buttonNew_pressed()
{
    lastVisitedPage = 1;
    Dialog * dialog = new Dialog(this);
    dialog->show();
}

void Widget::on_buttonExit_pressed()
{
    close();
}
