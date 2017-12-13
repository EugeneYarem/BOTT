#include "widget.h"
#include "ui_widget.h"
#include "view.h"
#include "Military/battlefield.h"
#include "Military/army.h"
#include "message.h"
#include "dialog.h"
#include "town.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QBrush>
#include <QKeyEvent>
#include <QKeySequence>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QFont>
#include <QCloseEvent>
#include <QSqlQuery>
#include <QSqlResult>
#include <QSqlError>

int countOfRecordsDB;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    gameDuration = QTime::currentTime();
    wastedMoneyP1 = 0;
    wastedMoneyP2 = 0;
    countOfUnitsP1 = 0;
    countOfUnitsP2 = 0;
    countOfModificationP1 = 0;
    countOfModificationP2 = 0;

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

    musicPlayer = new QMediaPlayer();
    musicPlayer->setMedia(QUrl("qrc:/sounds/musicBackground.mp3"));
    musicPlayer->setVolume(50);
    musicPlayer->play();

    db = QSqlDatabase::addDatabase("QSQLITE");

    lastVisitedPage = 1;
    eventEvoke = false;
    settingsChanged = false;
    isFirstGame = true;
    isGameOver = false;
    isSaved = false;
    isExit = false;
    isStartDialogOpen = false;

    // Эти две функции обязательно вызывать только после того, как добавлены другие элементы на сцену, чтобы меню всегда были на первом плане
    view->setConfiguration();
    view_2->setConfiguration();
    winner = NULL;
    earnedMoneyP1 = view->getTown()->getMoney();
    earnedMoneyP2 = view_2->getTown()->getMoney();

    // Коннэкты для сбора статистики о доходах и затратах
    connect(view->getArmy(), SIGNAL(moneyWasted(int)), this, SLOT(wastedMoneyP1Plus(int)));
    connect(view_2->getArmy(), SIGNAL(moneyWasted(int)), this, SLOT(wastedMoneyP2Plus(int)));
    connect(view->getArmy(), SIGNAL(uniteCreated()), this, SLOT(countOfUnitsP1Plus()));
    connect(view_2->getArmy(), SIGNAL(uniteCreated()), this, SLOT(countOfUnitsP2Plus()));
    connect(view->getArmy(), SIGNAL(modificate()), this, SLOT(countOfModificationP1Plus()));
    connect(view_2->getArmy(), SIGNAL(modificate()), this, SLOT(countOfModificationP2Plus()));
    connect(view->getTown(), SIGNAL(loose()), this, SLOT(winP2()));
    connect(view_2->getTown(), SIGNAL(loose()), this, SLOT(winP1()));
    connect(view->getTown(), SIGNAL(moneyEarned(int)), this, SLOT(earnedMoneyP1Plus(int)));
    connect(view_2->getTown(), SIGNAL(moneyEarned(int)), this, SLOT(earnedMoneyP2Plus(int)));
    connect(view->getTown(), SIGNAL(moneyWasted(int)), this, SLOT(wastedMoneyP1Plus(int)));
    connect(view_2->getTown(), SIGNAL(moneyWasted(int)), this, SLOT(wastedMoneyP2Plus(int)));
    connect(view->getTown(), SIGNAL(modificate()), this, SLOT(countOfModificationP1Plus()));
    connect(view_2->getTown(), SIGNAL(modificate()), this, SLOT(countOfModificationP2Plus()));

    // Коннэкты для работы с аудиопроигрывателем
    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(volumeChange(int)));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(volumeChange(int)));
    connect(musicPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(restartMusic(QMediaPlayer::State)));

    viewWithOpenMenu = NULL;
    gameOverLabel = NULL;

    // Коннэктим таймеры view's к слоту, чтобы по таймеру закрывалось меню игрока, у которого оно открыто
    connect(view, SIGNAL(menuVisibleStatusChanged(View*)), this, SLOT(updateViewWithOpenMenu(View*)));
    connect(view_2, SIGNAL(menuVisibleStatusChanged(View*)), this, SLOT(updateViewWithOpenMenu(View*)));

    connectDB();

    readSettings();
    createSettingsPage();
    createStatisticsPage();

    ui->stackedWidget->setCurrentIndex(1);
    ui->buttonContinue->hide();
}

Widget::~Widget()
{
    delete ui;
    delete btf;
    delete musicPlayer;
    if(gameOverLabel != NULL)
        delete gameOverLabel;
    db.close();
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
    isFirstGame = false;
    isSaved = false;
    isExit = false;
    isGameOver = false;

    winner = NULL;
    gameDuration = QTime::currentTime();
    wastedMoneyP1 = 0;
    wastedMoneyP2 = 0;
    countOfUnitsP1 = 0;
    countOfUnitsP2 = 0;
    countOfModificationP1 = 0;
    countOfModificationP2 = 0;
    viewWithOpenMenu = NULL;

    startAllTimers();
    ui->stackedWidget->setCurrentIndex(0);
    ui->buttonContinue->show();
    setMaximumWidth(16777215);
    lastVisitedPage = 1;
    clearFocusOfMainMenu();
    view->ClearStart();
    view_2->ClearStart();
    btf->ClearStart();
    readSettings();
}

void Widget::save()
{
    if(settingsChanged)
        writeSettings();

    if(!isFirstGame)
        writeStatistics();

    isSaved = true;
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

void Widget::countOfModificationP1Plus()
{
    countOfModificationP1++;
}

void Widget::countOfModificationP2Plus()
{
    countOfModificationP2++;
}

void Widget::restartMusic(QMediaPlayer::State state)
{
    if(state == QMediaPlayer::StoppedState)
        musicPlayer->play();
}

void Widget::volumeChange(int volume)
{
    musicPlayer->setVolume(volume);
}

void Widget::connectDB()
{
    db.setDatabaseName(qApp->applicationDirPath() + "/statistics.sqlite");

    try
    {
        if(!db.open())
            throw 1;
    }
    catch(int key)
    {
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT * FROM games");

    try
    {
        if(!query.exec())
            throw 1;
    }
    catch(int key)
    {
        query.exec("CREATE TABLE games (id INTEGER PRIMARY KEY UNIQUE, time INT, gamer1_id INT, gamer2_id INT)");
        query.exec("CREATE TABLE gamers (id INTEGER PRIMARY KEY UNIQUE, name STRING, result STRING, earnedMoney INT, wastedMoney INT, countOfUnits INT, countOdMods INT)");
    }

}

void Widget::createSettingsPage()
{
    ui->lineEditMenu->setText(QKeySequence(view->getControlKey("menu")).toString());
    ui->lineEditMenu_2->setText(QKeySequence(view_2->getControlKey("menu")).toString());
    ui->lineEditChoose->setText(QKeySequence(view->getControlKey("menu select")).toString());
    ui->lineEditChoose_2->setText(QKeySequence(view_2->getControlKey("menu select")).toString());
    ui->lineEditExit->setText(QKeySequence(view->getControlKey("exit from menu")).toString());
    ui->lineEditExit_2->setText(QKeySequence(view_2->getControlKey("exit from menu")).toString());
    ui->lineEditUp->setText(QKeySequence(view->getControlKey("menu up")).toString());
    ui->lineEditUp_2->setText(QKeySequence(view_2->getControlKey("menu up")).toString());
    ui->lineEditDown->setText(QKeySequence(view->getControlKey("menu down")).toString());
    ui->lineEditDown_2->setText(QKeySequence(view_2->getControlKey("menu down")).toString());
    ui->lineEditSoldier->setText(QKeySequence(view->getControlKey("create soldier")).toString());
    ui->lineEditSoldier_2->setText(QKeySequence(view_2->getControlKey("create soldier")).toString());
    ui->lineEditArcher->setText(QKeySequence(view->getControlKey("create archer")).toString());
    ui->lineEditArcher_2->setText(QKeySequence(view_2->getControlKey("create archer")).toString());
    ui->lineEditRider->setText(QKeySequence(view->getControlKey("create rider")).toString());
    ui->lineEditRider_2->setText(QKeySequence(view_2->getControlKey("create rider")).toString());
    ui->lineEditWizard->setText(QKeySequence(view->getControlKey("create wizard")).toString());
    ui->lineEditWizard_2->setText(QKeySequence(view_2->getControlKey("create wizard")).toString());

    ui->label->setStyleSheet("QLabel{background: rgba(255, 255, 255, 0);}");
    ui->labelSettings->setStyleSheet("QLabel{background: rgba(255, 255, 255, 220); padding-left: 5px; padding-right: 5px}");
    ui->labelSLogo->setStyleSheet("QLabel{background: rgba(255, 255, 255, 0);}");
    ui->labelSPlayer->setStyleSheet("QLabel{background: rgba(255, 255, 255, 220); padding-left: 5px; padding-right: 5px}");
    ui->labelSPlayer_2->setStyleSheet("QLabel{background: rgba(255, 255, 255, 220); padding-left: 5px; padding-right: 5px}");
    ui->labelSettingsSound->setStyleSheet("QLabel{background: rgba(255, 255, 255, 220); padding-left: 5px; padding-right: 5px;}");
    ui->labelSettingsSound->setFixedWidth(234);
    ui->spinBox->setStyleSheet("QSpinBox{background: rgba(255, 255, 255, 220); color: rgb(66, 66, 66);}");
    ui->horizontalSlider->setStyleSheet("QSlider{background: rgba(255, 255, 255, 220); color: rgb(66, 66, 66);}");
    ui->labelSettingsMenu->setStyleSheet("QLabel{background: rgba(255, 255, 255, 220); padding-left: 5px; padding-right: 5px}");
    ui->labelSettingsChoose->setStyleSheet("QLabel{background: rgba(255, 255, 255, 220); padding-left: 5px; padding-right: 5px}");
    ui->labelSettingsExit->setStyleSheet("QLabel{background: rgba(255, 255, 255, 220); padding-left: 5px; padding-right: 5px}");
    ui->labelSettingsUp->setStyleSheet("QLabel{background: rgba(255, 255, 255, 220); padding-left: 5px; padding-right: 5px}");
    ui->labelSettingsDown->setStyleSheet("QLabel{background: rgba(255, 255, 255, 220); padding-left: 5px; padding-right: 5px}");
    ui->labelSettingsSoldier->setStyleSheet("QLabel{background: rgba(255, 255, 255, 220); padding-left: 5px; padding-right: 5px}");
    ui->labelSettingsArcher->setStyleSheet("QLabel{background: rgba(255, 255, 255, 220); padding-left: 5px; padding-right: 5px}");
    ui->labelSettingsRider->setStyleSheet("QLabel{background: rgba(255, 255, 255, 220); padding-left: 5px; padding-right: 5px}");
    ui->labelSettingsWizard->setStyleSheet("QLabel{background: rgba(255, 255, 255, 220); padding-left: 5px; padding-right: 5px}");
    ui->lineEditMenu->setStyleSheet("QLineEdit{background: rgba(255, 255, 255, 220); color: rgb(66, 66, 66);}");
    ui->lineEditMenu_2->setStyleSheet("QLineEdit{background: rgba(255, 255, 255, 220); color: rgb(66, 66, 66);}");
    ui->lineEditChoose->setStyleSheet("QLineEdit{background: rgba(255, 255, 255, 220); color: rgb(66, 66, 66);}");
    ui->lineEditChoose_2->setStyleSheet("QLineEdit{background: rgba(255, 255, 255, 220); color: rgb(66, 66, 66);}");

    ui->lineEditExit->setStyleSheet("QLineEdit{background: rgba(255, 255, 255, 220); color: rgb(66, 66, 66);}");
    ui->lineEditExit_2->setStyleSheet("QLineEdit{background: rgba(255, 255, 255, 220); color: rgb(66, 66, 66);}");
    ui->lineEditUp->setStyleSheet("QLineEdit{background: rgba(255, 255, 255, 220); color: rgb(66, 66, 66);}");
    ui->lineEditUp_2->setStyleSheet("QLineEdit{background: rgba(255, 255, 255, 220); color: rgb(66, 66, 66);}");
    ui->lineEditDown->setStyleSheet("QLineEdit{background: rgba(255, 255, 255, 220); color: rgb(66, 66, 66);}");
    ui->lineEditDown_2->setStyleSheet("QLineEdit{background: rgba(255, 255, 255, 220); color: rgb(66, 66, 66);}");
    ui->lineEditSoldier->setStyleSheet("QLineEdit{background: rgba(255, 255, 255, 220); color: rgb(66, 66, 66);}");
    ui->lineEditSoldier_2->setStyleSheet("QLineEdit{background: rgba(255, 255, 255, 220); color: rgb(66, 66, 66);}");
    ui->lineEditArcher->setStyleSheet("QLineEdit{background: rgba(255, 255, 255, 220); color: rgb(66, 66, 66);}");
    ui->lineEditArcher_2->setStyleSheet("QLineEdit{background: rgba(255, 255, 255, 220); color: rgb(66, 66, 66);}");
    ui->lineEditRider->setStyleSheet("QLineEdit{background: rgba(255, 255, 255, 220); color: rgb(66, 66, 66);}");
    ui->lineEditRider_2->setStyleSheet("QLineEdit{background: rgba(255, 255, 255, 220); color: rgb(66, 66, 66);}");
    ui->lineEditWizard->setStyleSheet("QLineEdit{background: rgba(255, 255, 255, 220); color: rgb(66, 66, 66);}");
    ui->lineEditWizard_2->setStyleSheet("QLineEdit{background: rgba(255, 255, 255, 220); color: rgb(66, 66, 66);}");

    // Стиль для страницы "Для игроков"
    ui->labelForPlayers->setStyleSheet("QLabel{background: rgba(255, 255, 255, 220); padding-left: 5px; padding-right: 5px}");
    ui->labelScheme->setStyleSheet("QLabel{background: rgba(255, 255, 255, 0);}");

    connect(ui->spinBox, SIGNAL(valueChanged(int)), ui->horizontalSlider, SLOT(setValue(int)));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), ui->spinBox, SLOT(setValue(int)));
}

void Widget::createStatisticsPage()
{
    QSqlQuery query;

    try
    {
        if(!query.exec("SELECT time, gamer1_id, gamer2_id FROM games"))
            throw 1;
    }
    catch(int key)
    {
        Message mess(this);
        mess.setNewGameStatus();
        mess.setMessage("Не удалось получить статистику. Потеряно соединение с БД.");
        mess.exec();
        countOfRecordsDB = 0;
        return;
    }

    int res = 0;
    while (query.next())
        res++;

    countOfRecordsDB = res;

    ui->tableWidget->setRowCount(res * 2);
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->setStyleSheet("QTableWidget{background: rgba(101, 5, 4, 120); color: white}");
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Длительность игры" << "Имя" << "Результат" << "Заработано денег" << "Потрачено денег" << "Создано солдат" << "Куплено улучшений");
    ui->tableWidget->verticalHeader()->setMaximumWidth(78);
    ui->tableWidget->verticalHeader()->setMinimumWidth(78);
    ui->tableWidget->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

    ui->tableWidget->setColumnWidth(0, 170);
    ui->tableWidget->setColumnWidth(1, 190);
    ui->tableWidget->setColumnWidth(2, 170);
    ui->tableWidget->setColumnWidth(3, 170);
    if(res >= 12)
    {
        ui->tableWidget->setColumnWidth(4, 161);
        ui->tableWidget->setColumnWidth(5, 151);
    }
    else
    {
        ui->tableWidget->setColumnWidth(4, 170);
        ui->tableWidget->setColumnWidth(5, 160);
    }
    ui->tableWidget->setColumnWidth(6, 170);

    for(int i = 0; i < 7; i++)
    {
        ui->tableWidget->horizontalHeaderItem(i)->setFont(QFont("Century Gothic", 12));
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Fixed);
    }

    for(int i = 0, num = 0; i < res * 2; i++, num++)
    {
        ui->tableWidget->setVerticalHeaderItem(i, new QTableWidgetItem(QString::number(res - num)));
        ui->tableWidget->setVerticalHeaderItem(i + 1, new QTableWidgetItem(QString::number(res - num)));
        ui->tableWidget->verticalHeaderItem(i)->setFont(QFont("Century Gothic", 12));
        ui->tableWidget->verticalHeaderItem(i + 1)->setFont(QFont("Century Gothic", 12));
        ui->tableWidget->verticalHeader()->setSectionResizeMode(i, QHeaderView::Fixed);
        ui->tableWidget->verticalHeader()->setSectionResizeMode(i + 1, QHeaderView::Fixed);
        i++;
    }

    for(int i = 0, num = 0; i < ui->tableWidget->rowCount(); i++, num++)
    {
        query.previous();

        //------ Перевод милисекунд в часы, минуты и секунды
        int msec = query.value(0).toInt();
        int hours = msec / 60 / 60 / 1000;
        msec %= (60 * 60 * 1000);
        int minutes = msec / 60 / 1000;
        msec %= (60 * 1000);
        int seconds = msec / 1000;
        QString hoursStr, minutesStr, secondsStr;
        if(hours < 10)
            hoursStr = "0" + QString::number(hours);
        else hoursStr = QString::number(hours);

        if(minutes < 10)
            minutesStr = "0" + QString::number(minutes);
        else minutesStr = QString::number(minutes);

        if(seconds < 10)
            secondsStr = "0" + QString::number(seconds);
        else secondsStr = QString::number(seconds);
        //------

        QSqlQuery player1;
        player1.prepare("SELECT name, result, earnedMoney, wastedMoney, countOfUnits, countOdMods FROM gamers WHERE id = :id");
        player1.bindValue(":id", query.value(1).toInt());
        QSqlQuery player2;
        player2.prepare("SELECT name, result, earnedMoney, wastedMoney, countOfUnits, countOdMods FROM gamers WHERE id = :id");
        player2.bindValue(":id", query.value(2).toInt());

        try
        {
            if(!player1.exec() || !player2.exec())
                throw 1;
        }
        catch(int key)
        {
            Message mess(this);
            mess.setNewGameStatus();
            mess.setMessage("Не удалось получить статистику. Потеряно соединение с БД.");
            mess.exec();
            return;
        }

        player1.next();
        player2.next();

        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(hoursStr + ":" + minutesStr + ":" + secondsStr));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(player1.value(0).toString()));
        ui->tableWidget->setItem(i + 1, 1, new QTableWidgetItem(player2.value(0).toString()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(player1.value(1).toString()));
        ui->tableWidget->setItem(i + 1, 2, new QTableWidgetItem(player2.value(1).toString()));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(player1.value(2).toInt())));
        ui->tableWidget->setItem(i + 1, 3, new QTableWidgetItem(QString::number(player2.value(2).toInt())));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(player1.value(3).toInt())));
        ui->tableWidget->setItem(i + 1, 4, new QTableWidgetItem(QString::number(player2.value(3).toInt())));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(player1.value(4).toInt())));
        ui->tableWidget->setItem(i + 1, 5, new QTableWidgetItem(QString::number(player2.value(4).toInt())));
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(player1.value(5).toInt())));
        ui->tableWidget->setItem(i + 1, 6, new QTableWidgetItem(QString::number(player2.value(5).toInt())));
        i++;
    }
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
    ui->buttonForPlayers->installEventFilter(this);
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
    ui->buttonNew->clearFocus();
    ui->buttonSettings->clearFocus();
    ui->buttonStatistics->clearFocus();
    ui->buttonForPlayers->clearFocus();
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

void Widget::writeStatistics()
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO games (time, gamer1_id, gamer2_id) VALUES (:time, :gamer1_id, :gamer2_id)");
    query.bindValue(":time", gameDuration.msecsTo(QTime::currentTime()));
    if(countOfRecordsDB == 0)
    {
        query.bindValue(":gamer1_id", 1);
        query.bindValue(":gamer2_id", 2);
    }
    else
    {
        query.bindValue(":gamer1_id", countOfRecordsDB * 2 + 1);
        query.bindValue(":gamer2_id", countOfRecordsDB * 2 + 2);
    }

    try
    {
        if(!query.exec())
            throw 1;
    }
    catch(int key)
    {
        Message mess(this);
        mess.setNewGameStatus();
        mess.setMessage("Не удалось сохранить результат игры. Потеряно соединение с БД.");
        mess.exec();
        return;
    }

    QSqlQuery player1(db);
    player1.prepare("INSERT INTO gamers (name, result, earnedMoney, wastedMoney, countOfUnits, countOdMods) VALUES (:name, :result, :earnedMoney, :wastedMoney, :countOfUnits, :countOdMods)");
    QSqlQuery player2(db);
    player2.prepare("INSERT INTO gamers (name, result, earnedMoney, wastedMoney, countOfUnits, countOdMods) VALUES (:name, :result, :earnedMoney, :wastedMoney, :countOfUnits, :countOdMods)");

    if(winner != NULL)
    {
        if(*winner == gamerNameP1)
        {
            player1.bindValue(":result", QString("Выиграл"));
            player2.bindValue(":result", QString("Проиграл"));
        }
        else
        {
            player2.bindValue(":result", QString("Выиграл"));
            player1.bindValue(":result", QString("Проиграл"));
        }
    }
    else
    {
        player1.bindValue(":result", QString("Ничья"));
        player2.bindValue(":result", QString("Ничья"));
    }

    player1.bindValue(":name", gamerNameP1);
    player1.bindValue(":earnedMoney", earnedMoneyP1);
    player1.bindValue(":wastedMoney", wastedMoneyP1);
    player1.bindValue(":countOfUnits", countOfUnitsP1);
    player1.bindValue(":countOdMods", countOfModificationP1);
    player2.bindValue(":name", gamerNameP2);
    player2.bindValue(":earnedMoney", earnedMoneyP2);
    player2.bindValue(":wastedMoney", wastedMoneyP2);
    player2.bindValue(":countOfUnits", countOfUnitsP2);
    player2.bindValue(":countOdMods", countOfModificationP2);

    try
    {
        if(!player1.exec() || !player2.exec())
            throw 1;
    }
    catch(int key)
    {
        Message mess(this);
        mess.setNewGameStatus();
        mess.setMessage("Не удалось сохранить результат игры. Потеряно соединение с БД.");
        mess.exec();
        return;
    }
}

void Widget::writeSettings()
{
    QJsonObject player1, player2;
    player1.insert("menu", view->getControlKey("menu"));
    player1.insert("menu up", view->getControlKey("menu up"));
    player1.insert("menu down", view->getControlKey("menu down"));
    player1.insert("menu select", view->getControlKey("menu select"));
    player1.insert("exit from menu", view->getControlKey("exit from menu"));
    player1.insert("create soldier", view->getControlKey("create soldier"));
    player1.insert("create archer", view->getControlKey("create archer"));
    player1.insert("create rider", view->getControlKey("create rider"));
    player1.insert("create wizard", view->getControlKey("create wizard"));

    player2.insert("menu", view_2->getControlKey("menu"));
    player2.insert("menu up", view_2->getControlKey("menu up"));
    player2.insert("menu down", view_2->getControlKey("menu down"));
    player2.insert("menu select", view_2->getControlKey("menu select"));
    player2.insert("exit from menu", view_2->getControlKey("exit from menu"));
    player2.insert("create soldier", view_2->getControlKey("create soldier"));
    player2.insert("create archer", view_2->getControlKey("create archer"));
    player2.insert("create rider", view_2->getControlKey("create rider"));
    player2.insert("create wizard", view_2->getControlKey("create wizard"));

    QJsonArray array;
    array.insert(0, player1);
    array.insert(1, player2);

    QJsonDocument jsonDoc;
    jsonDoc.setArray(array);

    QFile file;
    file.setFileName("settings.json");
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    file.write(jsonDoc.toJson());
    file.close();
}

void Widget::gameOver()
{
    viewWithOpenMenu = NULL;
    stopAllTimers();
    view->hide();
    view_2->hide();

    if(!isGameOver)
    {
        setMaximumWidth(1280);
        gameOverLabel = new QLabel(this);
        ui->verticalLayout->addWidget(gameOverLabel);
        gameOverLabel->move(0, 0);
        gameOverLabel->setFixedSize(this->width() - 22, this->height() - 22);
        gameOverLabel->setStyleSheet("QLabel{background: rgba(255, 255, 255, 220); color: red;}");
        gameOverLabel->setFont(QFont("Century Gothic", 22));
        gameOverLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);        
        gameOverLabel->setText("Сражение окончено!\nПобедил в сражении " + *winner + ".");
        gameOverLabel->show();
        isGameOver = true;        
        save();
        createStatisticsPage();
        isExit = true;
    }
}

void Widget::showStartDialog()
{
    if(!isFirstGame)
    {
        if(!isSaved)
            save();
        createStatisticsPage();
    }

    isExit = true;
    Dialog * dialog = new Dialog(this);
    dialog->show();
    dialog->exec();
}

void Widget::setExit()
{
    isExit = true;
}

void Widget::setIsStartDialogOpen(bool status)
{
    isStartDialogOpen = status;
}

void Widget::readSettings()
{
    QFile file;
    file.setFileName("settings.json");

    try
    {
        if(!file.open(QIODevice::ReadOnly))
            throw 1;
    }
    catch(int key)
    {
        return;
    }


    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if(jsonDoc.isEmpty() || jsonDoc.isNull())
        return;

    view->setControlKey("menu", (Qt::Key)jsonDoc.array().at(0).toObject().value("menu").toInt());
    view->setControlKey("menu up", (Qt::Key)jsonDoc.array().at(0).toObject().value("menu up").toInt());
    view->setControlKey("menu down", (Qt::Key)jsonDoc.array().at(0).toObject().value("menu down").toInt());
    view->setControlKey("menu select", (Qt::Key)jsonDoc.array().at(0).toObject().value("menu select").toInt());
    view->setControlKey("exit from menu", (Qt::Key)jsonDoc.array().at(0).toObject().value("exit from menu").toInt());
    view->setControlKey("create soldier", (Qt::Key)jsonDoc.array().at(0).toObject().value("create soldier").toInt());
    view->setControlKey("create archer", (Qt::Key)jsonDoc.array().at(0).toObject().value("create archer").toInt());
    view->setControlKey("create rider", (Qt::Key)jsonDoc.array().at(0).toObject().value("create rider").toInt());
    view->setControlKey("create wizard", (Qt::Key)jsonDoc.array().at(0).toObject().value("create wizard").toInt());

    view_2->setControlKey("menu", (Qt::Key)jsonDoc.array().at(1).toObject().value("menu").toInt());
    view_2->setControlKey("menu up", (Qt::Key)jsonDoc.array().at(1).toObject().value("menu up").toInt());
    view_2->setControlKey("menu down", (Qt::Key)jsonDoc.array().at(1).toObject().value("menu down").toInt());
    view_2->setControlKey("menu select", (Qt::Key)jsonDoc.array().at(1).toObject().value("menu select").toInt());
    view_2->setControlKey("exit from menu", (Qt::Key)jsonDoc.array().at(1).toObject().value("exit from menu").toInt());
    view_2->setControlKey("create soldier", (Qt::Key)jsonDoc.array().at(1).toObject().value("create soldier").toInt());
    view_2->setControlKey("create archer", (Qt::Key)jsonDoc.array().at(1).toObject().value("create archer").toInt());
    view_2->setControlKey("create rider", (Qt::Key)jsonDoc.array().at(1).toObject().value("create rider").toInt());
    view_2->setControlKey("create wizard", (Qt::Key)jsonDoc.array().at(1).toObject().value("create wizard").toInt());
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

    if(watched == ui->buttonForPlayers)
    {
        if(event->type() == QEvent::FocusIn)
            ui->buttonForPlayers->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/forPlayerFocused.png);");
        if(event->type() == QEvent::FocusOut)
            ui->buttonForPlayers->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/forPlayer.png);");
        if(event->type() == QEvent::HoverEnter)
            ui->buttonForPlayers->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/forPlayerFocused.png);");
        if(event->type() == QEvent::HoverLeave)
            if(!ui->buttonForPlayers->hasFocus())
                ui->buttonForPlayers->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/forPlayer.png);");
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
                    {
                        view->setControlKey(view->getValueByControlKey(((QLineEdit *)watched)->text()), nativeKey);
                        settingsChanged = true;
                    }
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
                    {
                        view_2->setControlKey(view_2->getValueByControlKey(((QLineEdit *)watched)->text()), nativeKey);
                        settingsChanged = true;
                    }
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
                    {
                        view->setControlKey(view->getValueByControlKey(((QLineEdit *)watched)->text()), key);
                        settingsChanged = true;
                    }
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
                    {
                        view_2->setControlKey(view_2->getValueByControlKey(((QLineEdit *)watched)->text()), key);
                        settingsChanged = true;
                    }
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
                    {
                        view->setControlKey(view->getValueByControlKey(((QLineEdit *)watched)->text()), key);
                        settingsChanged = true;
                    }
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
                    {
                        view_2->setControlKey(view_2->getValueByControlKey(((QLineEdit *)watched)->text()), key);
                        settingsChanged = true;
                    }
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
                    {
                        view->setControlKey(view->getValueByControlKey(((QLineEdit *)watched)->text()), nativeKey);
                        settingsChanged = true;
                    }
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
                    {
                        view_2->setControlKey(view_2->getValueByControlKey(((QLineEdit *)watched)->text()), nativeKey);
                        settingsChanged = true;
                    }
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

void Widget::closeEvent(QCloseEvent *event)
{
    if(isFirstGame)
    {
        if(settingsChanged)
            writeSettings();
        close();
        return;
    }

    if(!isSaved)
    {
        Message mess(this);
        mess.setExitStatus();
        mess.setMessage("Текущее сражение будет закончено ничьей. Вы согласны?");
        mess.exec();
    }

    if(!isExit)
        event->ignore();
}

void Widget::on_buttonSettings_pressed()
{
    lastVisitedPage = 1;
    ui->stackedWidget->setCurrentIndex(2);
    clearFocusOfMainMenu();
}

void Widget::on_buttonContinue_pressed()
{
    lastVisitedPage = 1;
    startAllTimers();
    ui->stackedWidget->setCurrentIndex(0);
    clearFocusOfMainMenu();

    if(isGameOver)
    {
        setMaximumWidth(1280);
        return;
    }

    setMaximumWidth(16777215);
}

bool Widget::event(QEvent *event)
{
    if(((QKeyEvent *)event)->key() == Qt::Key_Escape)
    {
        if(ui->stackedWidget->currentIndex() == 0)
            stopAllTimers();

        if(ui->stackedWidget->currentIndex() == 2 && settingsChanged)
        {
            writeSettings();
            settingsChanged = false;
        }

        clearFocusOfMainMenu();
        int temp = lastVisitedPage;
        lastVisitedPage = ui->stackedWidget->currentIndex();
        ui->stackedWidget->setCurrentIndex(temp);

        if(temp == 0)
        {
            if(isGameOver)
            {
                setMaximumWidth(1280);
                return true;
            }

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
    if(isFirstGame)
    {
        clearFocusOfMainMenu();
        if(!isStartDialogOpen)
            showStartDialog();

        return;
    }

    if(!isSaved)
    {
        clearFocusOfMainMenu();
        Message mess(this);
        mess.setNewGameStatus();
        mess.setMessage("Текущее сражение будет закончено ничьей. Вы согласны?");
        mess.exec();
    }
    else
    {
        if(isGameOver)
        {
            view->show();
            view_2->show();
            delete gameOverLabel;
            gameOverLabel = NULL;
        }

        clearFocusOfMainMenu();

        if(!isStartDialogOpen)
            showStartDialog();

        return;
    }
}

void Widget::on_buttonExit_pressed()
{
    clearFocusOfMainMenu();
    close();
}

void Widget::on_buttonStatistics_pressed()
{
    lastVisitedPage = 1;
    ui->stackedWidget->setCurrentIndex(3);
    clearFocusOfMainMenu();
    ui->tableWidget->clearFocus();
}

void Widget::winP1()
{
    winner = &gamerNameP1;
    gameOver();
}

void Widget::winP2()
{
    winner = &gamerNameP2;
    gameOver();
}

void Widget::on_buttonForPlayers_pressed()
{
    lastVisitedPage = 1;
    ui->stackedWidget->setCurrentIndex(4);
    clearFocusOfMainMenu();
}
