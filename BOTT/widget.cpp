#include "dialog.h"
#include "enums.h"
#include "keeper.h"
#include "message.h"
#include "Military/army.h"
#include "Military/battlefield.h"
#include "town.h"
#include "typeinfo.h"
#include "ui_widget.h"
#include "view.h"
#include "widget.h"
#include <QBrush>
#include <QCloseEvent>
#include <QFont>
#include <QKeyEvent>
#include <QKeySequence>
#include <QLabel>
#include <QMediaPlayer>
#include <QSqlQuery>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    keeper = new Keeper(this);

    // Устанавливаем минимальный размер окна (1280 х 720)
    this->setMinimumSize(1280, 720);

    // Фиксируем высоту и ширину
    setFixedHeight(minimumHeight());
    setMaximumWidth(1280);

    // Создаём и настраиваем сцену
    QGraphicsScene * scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 2510, 343); // Размер сцены: 2510 x 343
    scene->setBackgroundBrush(QBrush(QImage(":/images/images/background.png")));

    // Создаём и настраиваем view's
    view = new View(ViewPosition::TopView, this); // view = new View(false)
    view_2 = new View(ViewPosition::BottomView, this); // view_2 = new View(true)
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

    // Создаем и настраиваем аудиопроигрыватель
    musicPlayer = new QMediaPlayer();
    musicPlayer->setMedia(QUrl("qrc:/sounds/musicBackground.mp3"));
    musicPlayer->setVolume(50);
    musicPlayer->play();

    // Инициализация нужных переменных
    eventEvoke = false;
    gameOverLabel = nullptr;
    isExit = false;
    isFirstGame = true;
    isGameOver = false;
    isSaved = false;
    isStartDialogOpen = false;
    lastVisitedPage = 1;
    settingsChanged = false;
    viewWithOpenMenu = nullptr;

    // Загрузка клавиш управления
    view->configureControlKeys( keeper->loadSettings( view->getControlKeys(), 0 ) );
    view_2->configureControlKeys( keeper->loadSettings( view_2->getControlKeys(), 1 ) );

    // Сохранение в статистику начальных сумм
    keeper->earnedMoneyP1Plus( view->getTown()->getMoney() );
    keeper->earnedMoneyP2Plus( view_2->getTown()->getMoney() );

    // Создание нужных коннектов
    createStatisticsConnects();
    createMusicPlayerConnects();
    createViewsConnects();
    createConnectsForDispMess();

    // Заполнение данными меню настроек и статистики
    createSettingsPage();
    createStatisticsPage();

    ui->stackedWidget->setCurrentIndex(1); // Отображение главного меню
    ui->buttonContinue->hide(); // Скрываем кнопку "Продолжить игру"

    // загрузка громкости музыки
    int volume = keeper->loadMusicVolume();
    if(volume != -1)
        ui->spinBox->setValue(volume);
}

Widget::~Widget()
{
    delete btf;
    delete musicPlayer;
    delete ui;
    if(gameOverLabel != nullptr)
        delete gameOverLabel;
}

void Widget::startNewGame()
{
    isExit = false;
    isFirstGame = false;
    isGameOver = false;
    isSaved = false;

    keeper->refreshStatistics();
    viewWithOpenMenu = nullptr;

    startAllTimers();
    ui->stackedWidget->setCurrentIndex(0);
    ui->buttonContinue->show();
    setMaximumWidth(16777215);
    lastVisitedPage = 1;
    clearFocusOfMainMenu();
    view->clearStart();
    view_2->clearStart();
    btf->clearStart();

    // Сохранение в статистику начальных сумм
    keeper->earnedMoneyP1Plus( view->getTown()->getMoney() );
    keeper->earnedMoneyP2Plus( view_2->getTown()->getMoney() );
}

bool Widget::checkKeyAndSet(QLineEdit *watched, Qt::Key key)
{
    if(isLineEditOfFirstPlayer(watched))
    {
        if(!view->checkControlKey(key) && !view_2->checkControlKey(key))
        {
            view->setControlKey(view->getValueByControlKey((dynamic_cast<QLineEdit *>(watched))->text()), key);
            settingsChanged = true;
        }
        else
        {
            showMessage("Эта кнопка уже используется.");
            return true;
        }
    }
    else
    {
        if(!view->checkControlKey(key) && !view_2->checkControlKey(key))
        {
            view_2->setControlKey(view_2->getValueByControlKey((dynamic_cast<QLineEdit *>(watched))->text()), key);
            settingsChanged = true;
        }
        else
        {
            showMessage("Эта кнопка уже используется.");
            return true;
        }
    }

    (dynamic_cast<QLineEdit *>(watched))->setText(QKeySequence(key).toString());
    (dynamic_cast<QLineEdit *>(watched))->clearFocus();
    return true;
}

bool Widget::checkViewAndSetEvent(View *view, QEvent *event)
{
    if(((view->isControlKey((dynamic_cast<QKeyEvent *>(event))->nativeVirtualKey()) || view->isControlKey((dynamic_cast<QKeyEvent *>(event))->key())) && (view == viewWithOpenMenu || viewWithOpenMenu == nullptr) && view->isCanMenuOpen())
      || view->isShortcut((dynamic_cast<QKeyEvent *>(event))->nativeVirtualKey()) || view->isShortcut((dynamic_cast<QKeyEvent *>(event))->key()))
    {
        if(!view->isShortcut((dynamic_cast<QKeyEvent *>(event))->nativeVirtualKey()) && !view->isShortcut((dynamic_cast<QKeyEvent *>(event))->key()) && viewWithOpenMenu == nullptr)
            viewWithOpenMenu = view;
        view->setFocus();
        view->event(event);
        return true;
    }
    return false;
}

void Widget::save()
{
    if(settingsChanged)
        keeper->saveSettings(view->getControlKeys(), view_2->getControlKeys(), ui->spinBox->value());

    if(!isFirstGame)
        keeper->saveStatistics();

    isSaved = true;
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

    connect(ui->spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), ui->horizontalSlider, &QSlider::setValue);
    connect(ui->horizontalSlider, &QSlider::valueChanged, ui->spinBox, &QSpinBox::setValue);
}

void Widget::createStatisticsPage()
{
    createStatisticsTable( keeper->getCountOfGamesRecords() );
    fillInStatisticsTable( keeper->getGamesRecords() );
}

void Widget::createStatisticsTable(int res)
{
    ui->tableWidget->setRowCount(res * 2);
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
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Fixed);

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
}

void Widget::fillInStatisticsTable(QSqlQuery & query)
{
    for(int i = 0, num = 0; i < ui->tableWidget->rowCount(); i++, num++)
    {
        if(query.next())
        {
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

            ui->tableWidget->setItem(i, 0, new QTableWidgetItem(hoursStr + ":" + minutesStr + ":" + secondsStr));

            QSqlQuery player1 = keeper->getRecordAboutGamer(query.value(1).toInt());
            QSqlQuery player2;
            if(player1.next())
            {
                ui->tableWidget->setItem(i, 1, new QTableWidgetItem(player1.value(0).toString()));
                ui->tableWidget->setItem(i, 2, new QTableWidgetItem(player1.value(1).toString()));
                ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(player1.value(2).toInt())));
                ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(player1.value(3).toInt())));
                ui->tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(player1.value(4).toInt())));
                ui->tableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(player1.value(5).toInt())));

                player2 = keeper->getRecordAboutGamer(query.value(2).toInt());
                if(player2.next())
                {
                    ui->tableWidget->setItem(i + 1, 1, new QTableWidgetItem(player2.value(0).toString()));
                    ui->tableWidget->setItem(i + 1, 2, new QTableWidgetItem(player2.value(1).toString()));
                    ui->tableWidget->setItem(i + 1, 3, new QTableWidgetItem(QString::number(player2.value(2).toInt())));
                    ui->tableWidget->setItem(i + 1, 4, new QTableWidgetItem(QString::number(player2.value(3).toInt())));
                    ui->tableWidget->setItem(i + 1, 5, new QTableWidgetItem(QString::number(player2.value(4).toInt())));
                    ui->tableWidget->setItem(i + 1, 6, new QTableWidgetItem(QString::number(player2.value(5).toInt())));
                }
            }
            i++;
        }
    }
}

bool Widget::isSettingLineEdit(QObject * watched)
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

void Widget::setRequiredBGIToMainMenuItem(QEvent::Type event, QPushButton *button, QString image)
{
    if(event == QEvent::FocusIn)
        button->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/" + image + "Focused.png);");
    if(event == QEvent::FocusOut)
        button->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/" + image + ".png);");
    if(event == QEvent::HoverEnter)
        button->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/" + image + "Focused.png);");
    if(event == QEvent::HoverLeave)
        if(!button->hasFocus())
            button->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/" + image + ".png);");
}

void Widget::showMessage(QString text, QObject * sender, int eventType)
{
    Message * message = new Message(this);
    message->setMessage(text);
    if(sender != nullptr && eventType != -1)
        connect(message, &Message::okButtonPress, [sender, eventType] () { QApplication::postEvent(sender, new QEvent(static_cast<QEvent::Type>(eventType))); } );
    message->show();
    message->exec();
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

void Widget::createConnectsForDispMess()
{
    connect(keeper, SIGNAL(requiredShowMes(QString)), this, SLOT(showMessage(QString)));
    connect(view->getTown(), SIGNAL(requiredShowMes(QString)), this, SLOT(showMessage(QString)));
    connect(view_2->getTown(), SIGNAL(requiredShowMes(QString)), this, SLOT(showMessage(QString)));
    connect(view->getArmy(), SIGNAL(requiredShowMes(QString)), this, SLOT(showMessage(QString)));
    connect(view_2->getArmy(), SIGNAL(requiredShowMes(QString)), this, SLOT(showMessage(QString)));
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

void Widget::createStatisticsConnects()
{
    // Коннэкты для сбора статистики о доходах и затратах
    connect(view->getArmy(), &Army::moneyWasted, keeper, &Keeper::wastedMoneyP1Plus);
    connect(view_2->getArmy(), &Army::moneyWasted, keeper, &Keeper::wastedMoneyP2Plus);
    connect(view->getArmy(), &Army::uniteCreated, keeper, &Keeper::countOfUnitsP1Plus);
    connect(view_2->getArmy(), &Army::uniteCreated, keeper, &Keeper::countOfUnitsP2Plus);
    connect(view->getArmy(), &Army::modificate, keeper, &Keeper::countOfModificationP1Plus);
    connect(view_2->getArmy(), &Army::modificate, keeper, &Keeper::countOfModificationP2Plus);
    connect(view->getTown(), &Town::loose, keeper, &Keeper::winP2);
    connect(view_2->getTown(), &Town::loose, keeper, &Keeper::winP1);
    connect(view->getTown(), &Town::loose, this, &Widget::gameOver);
    connect(view_2->getTown(), &Town::loose, this, &Widget::gameOver);
    connect(view->getTown(), &Town::moneyEarned, keeper, &Keeper::earnedMoneyP1Plus);
    connect(view_2->getTown(), &Town::moneyEarned, keeper, &Keeper::earnedMoneyP2Plus);
    connect(view->getTown(), &Town::moneyWasted, keeper, &Keeper::wastedMoneyP1Plus);
    connect(view_2->getTown(), &Town::moneyWasted, keeper, &Keeper::wastedMoneyP2Plus);
    connect(view->getTown(), &Town::modificate, keeper, &Keeper::countOfModificationP1Plus);
    connect(view_2->getTown(), &Town::modificate, keeper, &Keeper::countOfModificationP2Plus);
}

void Widget::createMusicPlayerConnects()
{
    // Коннэкты для работы с аудиопроигрывателем
    connect(ui->spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this] (int value) {
                    musicPlayer->setVolume(value);
                    settingsChanged = true;
    });
    connect(ui->horizontalSlider, &QSlider::valueChanged, [this] (int value) {
                    musicPlayer->setVolume(value);
                    settingsChanged = true;
    });
    connect(musicPlayer, &QMediaPlayer::stateChanged, [this] (QMediaPlayer::State state) { if(state == QMediaPlayer::StoppedState) musicPlayer->play(); });
}

void Widget::createViewsConnects()
{
    // Коннэктим таймеры view's к слоту, чтобы по таймеру закрывалось меню игрока, у которого оно открыто
    connect(view, &View::menuVisibleStatusChanged, [this] (View * sender) {
            // Если view-отправитель сигнала таймера об истечении времени нахождения в меню - это нынешний view с открытым меню
            if(viewWithOpenMenu == sender)
                viewWithOpenMenu = nullptr;
    });
    connect(view_2, &View::menuVisibleStatusChanged, [this] (View * sender) {
            // Если view-отправитель сигнала таймера об истечении времени нахождения в меню - это нынешний view с открытым меню
            if(viewWithOpenMenu == sender)
                viewWithOpenMenu = nullptr;
    });
}

void Widget::gameOver()
{
    viewWithOpenMenu = nullptr;
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
        gameOverLabel->setText("Сражение окончено!\nПобедил в сражении " + keeper->getWinner() + ".");
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
        ui->buttonContinue->hide();
    }

    isExit = true;
    Dialog * dialog = new Dialog(this);
    isStartDialogOpen = true;
    connect(dialog, &Dialog::dialogIsOpen, [this] (bool status) { isStartDialogOpen = status; });
    connect(dialog, &Dialog::startNewGame, [this] (QString gamer1, QString gamer2) {
                    this->keeper->setGamersNames(gamer1, gamer2);
                    this->startNewGame();
    });
    dialog->show();
    dialog->exec();
}

void Widget::setExit()
{
    isExit = true;
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->buttonContinue)
    {
        setRequiredBGIToMainMenuItem(event->type(), ui->buttonContinue, "continue");
        return false;
    }

    if(watched == ui->buttonNew)
    {
        setRequiredBGIToMainMenuItem(event->type(), ui->buttonNew, "new");
        return false;
    }

    if(watched == ui->buttonSettings)
    {
        setRequiredBGIToMainMenuItem(event->type(), ui->buttonSettings, "settings");
        return false;
    }

    if(watched == ui->buttonStatistics)
    {
        setRequiredBGIToMainMenuItem(event->type(), ui->buttonStatistics, "statistics");
        return false;
    }

    if(watched == ui->buttonForPlayers)
    {
        setRequiredBGIToMainMenuItem(event->type(), ui->buttonForPlayers, "forPlayer");
        return false;
    }

    if(watched == ui->buttonExit)
    {
        setRequiredBGIToMainMenuItem(event->type(), ui->buttonExit, "exit");
        return false;
    }

    if(isSettingLineEdit(watched))
    {
        if (event->type() == QEvent::KeyPress)
        {
            Qt::Key key = static_cast<Qt::Key>(((dynamic_cast<QKeyEvent *>(event))->key()));
            Qt::Key nativeKey = static_cast<Qt::Key>(((dynamic_cast<QKeyEvent *>(event))->nativeVirtualKey()));
            QString text = (dynamic_cast<QKeyEvent *>(event))->text();

            /*
             * QEvent::nativeKey() умеет принять код английской клавиши даже при другой раскладке клавиатуры, но неправильно реагирует на некоторые кнопки
             * QEvent::key() правильно реагирует на все кнопки, но считывает код клавиши с буквами на том языке, в котором сейчас установлена раскладка клавиатуры
             * Для понимания дальнейших проверок стоит раскомментировать следующие три строки
            */
            //qDebug() << QKeySequence(key).toString();
            //qDebug() << QKeySequence(nativeKey).toString();
            //qDebug() << ((QKeyEvent *)event)->text();

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
                return checkKeyAndSet(dynamic_cast<QLineEdit *>(watched), nativeKey);
            }

            if(QKeySequence(nativeKey).toString().length() < QKeySequence(key).toString().length())
            {
                return checkKeyAndSet(dynamic_cast<QLineEdit *>(watched), key);
            }

            if(QKeySequence(key).toString() == text)
            {
                return checkKeyAndSet(dynamic_cast<QLineEdit *>(watched), key);
            }

            if(QKeySequence(key).toString() != text && QKeySequence(key).toString() == text.toUpper())
            {
                return checkKeyAndSet(dynamic_cast<QLineEdit *>(watched), nativeKey);
            }
            return true;
        }
    }

    if (watched == view || watched == view_2)
    {
        if (event->type() == QEvent::KeyPress)
        {
            if(checkViewAndSetEvent(view, event))
                return true;
            else if(checkViewAndSetEvent(view_2, event))
                return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void Widget::closeEvent(QCloseEvent *event)
{
    if(isFirstGame)
    {
        if(settingsChanged)
            keeper->saveSettings(view->getControlKeys(), view_2->getControlKeys(), ui->spinBox->value());
        close();
        return;
    }

    if(!isSaved)
    {
        Message * mess = new Message(this);
        mess->setMessage("Текущее сражение будет закончено ничьей. Вы согласны?");
        connect(mess, &Message::okButtonPress, [this] () {
                        this->save();
                        this->setExit();
        });
        mess->show();
        mess->exec();
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
    if(event->type() == QEvent::KeyRelease && (dynamic_cast<QKeyEvent *>(event))->key() == Qt::Key_Escape)
    {
        if(ui->stackedWidget->currentIndex() == 0)
            stopAllTimers();

        if(ui->stackedWidget->currentIndex() == 2 && settingsChanged)
        {
            keeper->saveSettings(view->getControlKeys(), view_2->getControlKeys(), ui->spinBox->value());
            settingsChanged = false;
        }

        clearFocusOfMainMenu();
        int temp = lastVisitedPage;
        lastVisitedPage = ui->stackedWidget->currentIndex();
        ui->stackedWidget->setCurrentIndex(temp);

        if(temp == 0 && ui->buttonContinue->isVisible())
        {
            if(isGameOver)
            {
                setMaximumWidth(1280);
                return true;
            }

            setMaximumWidth(16777215);
            startAllTimers();
        }
        else
        {
            setMaximumWidth(1280);
            if(temp == 0 && !ui->buttonContinue->isVisible())
                ui->stackedWidget->setCurrentIndex(lastVisitedPage);
        }

        isStartDialogOpen = false;

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
        Message * mess = new Message(this);
        mess->setMessage("Текущее сражение будет закончено ничьей. Вы согласны?");
        connect(mess, &Message::okButtonPress, [this] () { this->showStartDialog(); });
        mess->show();
        mess->exec();
    }
    else
    {
        if(isGameOver)
        {
            view->show();
            view_2->show();
            delete gameOverLabel;
            gameOverLabel = nullptr;
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

void Widget::on_buttonForPlayers_pressed()
{
    lastVisitedPage = 1;
    ui->stackedWidget->setCurrentIndex(4);
    clearFocusOfMainMenu();
}
