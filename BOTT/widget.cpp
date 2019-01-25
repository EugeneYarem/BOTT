#include "constants.h"
#include "dialog.h"
#include "enums.h"
#include "keeper.h"
#include "message.h"
#include "Military/army.h"
#include "Military/battlefield.h"
#include "RbTableHeaderView/RbTableHeaderView.h"
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
#include <QVector>


Widget::Widget(QWidget * parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    keeper = new Keeper(this);

    // Устанавливаем минимальный размер окна (1280 х 720)
    this->setMinimumSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);

    // Фиксируем высоту и ширину
    setFixedHeight(minimumHeight());
    setMaximumWidth(MIN_WINDOW_WIDTH);

    // Создаём и настраиваем сцену
    QGraphicsScene * scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 2510, SCENE_HEIGHT); // Размер сцены: 2510 x 343
    scene->setBackgroundBrush(QBrush(QImage(":/images/images/background.png")));

    // Создаём и настраиваем view's
    view = new View(ViewPosition::TopView, this);
    view_2 = new View(ViewPosition::BottomView, this);
    ui->verticalLayout->addWidget(view);
    ui->verticalLayout->addWidget(view_2);
    view->setScene(scene);
    view_2->setScene(scene);
    view->setSceneRect(0, 0, LAST_SCENE_X_FOR_BOTTOM_VIEW, SCENE_HEIGHT);
    view_2->setSceneRect(LAST_SCENE_X_FOR_BOTTOM_VIEW, 0, LAST_SCENE_X_FOR_BOTTOM_VIEW, SCENE_HEIGHT);

    installEventFilters();

    // Создаём и настраиваем поле боя
    btf = new Battlefield();
    btf->setScene(scene);
    btf->setArmies(view->getArmy(), view_2->getArmy());

    // Создаем и настраиваем аудиопроигрыватель
    volume = MUSIC_VOLUME;
    musicPlayer = new QMediaPlayer();
    musicPlayer->setMedia(QUrl("qrc:/sounds/musicBackground.mp3"));
    musicPlayer->setVolume(volume);
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
    viewWithOpenMenu = nullptr;

    // Загрузка клавиш управления
    view->configureControlKeys( keeper->loadSettings( view->getControlKeys(), 0 ) );
    view_2->configureControlKeys( keeper->loadSettings( view_2->getControlKeys(), 1 ) );
    if(view->getControlKeys() == view_2->getControlKeys())
    {
        view->configureControlKeys(new QVector<int>(QVector<int>::fromStdVector(ERRORS_READING_SETTINGS)));
        view_2->configureControlKeys(new QVector<int>(QVector<int>::fromStdVector(ERRORS_READING_SETTINGS)));
    }

    // Сохранение в статистику начальных сумм
    keeper->earnedMoneyP1Plus( view->getTown()->getMoney() );
    keeper->earnedMoneyP2Plus( view_2->getTown()->getMoney() );

    // Создание нужных коннектов
    createStatisticsConnects();
    createMusicPlayerConnects();
    createViewsConnects();
    createConnectsForDispMess();
    createSettingButtonsConnects();

    // Заполнение данными меню настроек и статистики
    createSettingsPage();
    createStatisticsPage();

    // загрузка громкости музыки
    volume = keeper->loadMusicVolume();
    if(volume != -1)
        ui->spinBox->setValue(volume);
    else volume = MUSIC_VOLUME;

    ui->stackedWidget->setCurrentIndex(1); // Отображение главного меню

    // Проверка, есть ли незавершенная игра
    if(keeper->isLastGameExists())
    {
        view->clearStart();
        view_2->clearStart();
        btf->clearStart();
        if(keeper->loadLastGame(view->getArmy(), view_2->getArmy(), view->getTown(), view_2->getTown(), view, view_2))
        {
            view->getArmy()->addRestoredTroopsOnScene(scene);
            view_2->getArmy()->addRestoredTroopsOnScene(scene);
            isFirstGame = false;
            isGameOver = false;
            isSaved = false;
        }
        else
        {
            showMessage("Последняя игра не может быть восстановлена, т. к. файл с её состоянием был поврежден.");           
            ui->buttonContinue->hide(); // Скрываем кнопку "Продолжить игру"
        }
        keeper->removeLastGameFile();
    }
    else ui->buttonContinue->hide(); // Скрываем кнопку "Продолжить игру"

    // Инициализацию этой переменной проводить в конце, т.к. при изменении громкости она станет равной true
    settingsChanged = false;
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

bool Widget::checkKeyAndSet(QLineEdit * watched, Qt::Key key)
{
    if(!view->isControlKey(key, SettingMap::Temp) && !view->isCKContainKeyWithoutCrossingWithTS(key)
       && !view_2->isControlKey(key, SettingMap::Temp) && !view_2->isCKContainKeyWithoutCrossingWithTS(key))
    {
        if(isLineEditOfFirstPlayer(watched))
        {
            QString tempInMainMap = view->getValueByControlKey(dynamic_cast<QLineEdit *>(watched)->text());
            QString tempInTempMap = view->getValueByControlKey(dynamic_cast<QLineEdit *>(watched)->text(), SettingMap::Temp);
            view->setTempControlKey( tempInMainMap.isEmpty() ? tempInTempMap : tempInMainMap , key);
        }
        else
        {
            QString tempInMainMap = view_2->getValueByControlKey(dynamic_cast<QLineEdit *>(watched)->text());
            QString tempInTempMap = view_2->getValueByControlKey(dynamic_cast<QLineEdit *>(watched)->text(), SettingMap::Temp);
            view_2->setTempControlKey( tempInMainMap.isEmpty() ? tempInTempMap : tempInMainMap , key);
        }
        settingsChanged = true;
        dynamic_cast<QLineEdit *>(watched)->setText(QKeySequence(key).toString());
        dynamic_cast<QLineEdit *>(watched)->clearFocus();
        ui->pushButtonCancel->setEnabled(true);
    }
    else showMessage("Эта кнопка уже используется.");

    return true;
}

bool Widget::checkViewAndSetEvent(View * view, QEvent * event)
{
    if(((view->isControlKey(dynamic_cast<QKeyEvent *>(event)->nativeVirtualKey()) || view->isControlKey(dynamic_cast<QKeyEvent *>(event)->key())) && (view == viewWithOpenMenu || viewWithOpenMenu == nullptr) && view->isCanMenuOpen())
      || view->isShortcut(dynamic_cast<QKeyEvent *>(event)->nativeVirtualKey()) || view->isShortcut(dynamic_cast<QKeyEvent *>(event)->key()))
    {
        if(!view->isShortcut(dynamic_cast<QKeyEvent *>(event)->nativeVirtualKey()) && !view->isShortcut(dynamic_cast<QKeyEvent *>(event)->key()) && viewWithOpenMenu == nullptr)
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
        keeper->saveSettings(view->getControlKeys(), view_2->getControlKeys(), volume);
    if(!isFirstGame)
        keeper->saveStatistics();
    isSaved = true;
}

void Widget::createSettingsPage() const
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
    ui->spinBox->setValue(volume);
}

void Widget::createStatisticsPage() const
{
    createStatisticsTable( keeper->getCountOfGamesRecords(true) );
    fillInStatisticsTable( keeper->getGamesRecords() );
}

void Widget::createStatisticsTable(const int & res) const
{
    ui->tableWidget->setRowCount(res * 2);

    RbTableHeaderView * vHead = new RbTableHeaderView(Qt::Vertical, res * 2, 1);
    QAbstractItemModel * vModel = vHead->model();

    connect(vHead, &RbTableHeaderView::sectionPressed, this, &Widget::verticalHeaderSectionPressed);
    connect(vHead, &RbTableHeaderView::sectionEntered, this, &Widget::verticalHeaderSectionPressed);
    connect(ui->tableWidget, &QTableWidget::currentCellChanged, this, &Widget::selectVerticalHeaderItems);
    connect(ui->tableWidget, &QTableWidget::clicked, this, &Widget::selectVerticalHeaderItems);

    vHead->setFont(QFont("Century Gothic", 12));
    vHead->setColumnWidth(0, 78);
    vHead->setDefaultAlignment(Qt::AlignCenter);
    ui->tableWidget->setVerticalHeader(vHead);

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
        vHead->setSpan(i, 0, 2, 0);
        vHead->setCellBackgroundColor(vModel->index(i, 0), Qt::white);
        vHead->setSectionResizeMode(i, QHeaderView::Fixed);
        vHead->setSectionResizeMode(i + 1, QHeaderView::Fixed);
        vModel->setData(vModel->index(i, 0), QString::number(res - num), Qt::DisplayRole);
        vModel->setData(vModel->index(i + 1, 0), QString::number(res - num), Qt::DisplayRole);
        i++;
    }
}

void Widget::fillInStatisticsTable(QSqlQuery & query) const
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
            ui->tableWidget->setSpan(i, 0, 2, 1);

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
                    if(player1.value(1).toString() == player2.value(1).toString())
                        ui->tableWidget->setSpan(i, 2, 2, 1);
                }
            }
            i++;
        }
    }
}

bool Widget::isSettingLineEdit(const QObject * watched) const
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

void Widget::setRequiredBGIToMainMenuItem(const QEvent::Type & event, QPushButton * button, const MenuBG & bgType) const
{
    QString str = bgType == MenuBG::SpecialBG ? "2" : "";
    if(event == QEvent::FocusIn || event == QEvent::HoverEnter)
        button->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/menuItemsFocusedBG" + str + ".png);");
    if(event == QEvent::FocusOut)
        button->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/menuItemsBG" + str + ".png);");
    if(event == QEvent::HoverLeave)
        if(!button->hasFocus())
            button->setStyleSheet("background-image: url(:/images/images/Main_Menu_Of_BOTT/menuItemsBG" + str + ".png);");
}

void Widget::showMessageAboutUnsavedSettings()
{
    if(settingsChanged)
    {
        Message * mess = new Message(this);
        mess->setMessage("Настройки были изменены. Вы желаете их сохранить?");
        connect(mess, &Message::okButtonPress, [this] () {
                        volume = ui->spinBox->value();
                        view->replaceCKByTS();
                        view_2->replaceCKByTS();
                        keeper->saveSettings(view->getControlKeys(), view_2->getControlKeys(), volume);
        });
        connect(mess, &Message::end, [this] (bool isButtonOkPressed) {
                        if(!isButtonOkPressed)
                        {
                            createSettingsPage();
                            view->clearTempSettings();
                            view_2->clearTempSettings();
                        }
        });
        mess->show();
        mess->exec();
        settingsChanged = false;
    }
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

bool Widget::isLineEditOfFirstPlayer(const QObject * watched) const
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

void Widget::createConnectsForDispMess() const
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

void Widget::createStatisticsConnects() const
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
    connect(ui->spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), ui->horizontalSlider, &QSlider::setValue);
    connect(ui->horizontalSlider, &QSlider::valueChanged, ui->spinBox, &QSpinBox::setValue);
    connect(ui->spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this] (int value) {
                    musicPlayer->setVolume(value);
                    settingsChanged = true;
                    ui->pushButtonCancel->setEnabled(true);
    });
    connect(ui->horizontalSlider, &QSlider::valueChanged, [this] (int value) {
                    musicPlayer->setVolume(value);
                    settingsChanged = true;
                    ui->pushButtonCancel->setEnabled(true);
    });
    connect(musicPlayer, &QMediaPlayer::stateChanged, [this] (QMediaPlayer::State state) { if(state == QMediaPlayer::StoppedState) musicPlayer->play(); });
}

void Widget::createSettingButtonsConnects()
{
    connect(ui->pushButtonStandart, &QPushButton::pressed, [this] () {
                    view->configureControlKeys(new QVector<int>(QVector<int>::fromStdVector(ERRORS_READING_SETTINGS)));
                    view_2->configureControlKeys(new QVector<int>(QVector<int>::fromStdVector(ERRORS_READING_SETTINGS)));
                    view->clearTempSettings();
                    view_2->clearTempSettings();
                    volume = MUSIC_VOLUME;
                    keeper->removeSettingsFile();
                    this->createSettingsPage();
                    ui->pushButtonCancel->setEnabled(false);
                    settingsChanged = false;
    });
    connect(ui->pushButtonSave, &QPushButton::pressed, [this] () {
                    volume = ui->spinBox->value();
                    view->replaceCKByTS();
                    view_2->replaceCKByTS();
                    keeper->saveSettings(view->getControlKeys(), view_2->getControlKeys(), volume);
                    settingsChanged = false;
                    ui->pushButtonCancel->setEnabled(false);
    });
    connect(ui->pushButtonCancel, &QPushButton::pressed, [this] () {
                    view->clearTempSettings();
                    view_2->clearTempSettings();
                    this->createSettingsPage();
                    settingsChanged = false;
                    ui->pushButtonCancel->setEnabled(false);
    });
}

void Widget::createViewsConnects()
{
    // Коннэктим таймеры view's к слоту, чтобы по таймеру закрывалось меню игрока, у которого оно открыто
    connect(view, &View::menuVisibleStatusChanged, [this] (const View * sender) {
            // Если view-отправитель сигнала таймера об истечении времени нахождения в меню - это нынешний view с открытым меню
            if(viewWithOpenMenu == sender)
                viewWithOpenMenu = nullptr;
    });
    connect(view_2, &View::menuVisibleStatusChanged, [this] (const View * sender) {
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
        setMaximumWidth(MIN_WINDOW_WIDTH);
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
        ui->tableWidget->clearSpans();
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

bool Widget::eventFilter(QObject * watched, QEvent * event)
{
    if(watched == ui->buttonContinue || watched == ui->buttonNew || watched == ui->buttonSettings ||
       watched == ui->buttonStatistics || watched == ui->buttonForPlayers)
    {
        setRequiredBGIToMainMenuItem(event->type(), dynamic_cast<QPushButton *>(watched));
        return false;
    }
    if(watched == ui->buttonExit)
    {
        setRequiredBGIToMainMenuItem(event->type(), ui->buttonExit, MenuBG::SpecialBG);
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
                return checkKeyAndSet(dynamic_cast<QLineEdit *>(watched), nativeKey);

            if(QKeySequence(nativeKey).toString().length() < QKeySequence(key).toString().length())
                return checkKeyAndSet(dynamic_cast<QLineEdit *>(watched), key);

            if(QKeySequence(key).toString() == text)
                return checkKeyAndSet(dynamic_cast<QLineEdit *>(watched), key);

            if(QKeySequence(key).toString() != text && QKeySequence(key).toString() == text.toUpper())
                return checkKeyAndSet(dynamic_cast<QLineEdit *>(watched), nativeKey);

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

void Widget::closeEvent(QCloseEvent * )
{
    if(isFirstGame)
    {
        showMessageAboutUnsavedSettings();
        close();
        return;
    }
    if(!isSaved)
        keeper->saveLastGame(view->getArmy()->getArmy(), view_2->getArmy()->getArmy(),
                             view->getArmy()->getPrototypes(), view_2->getArmy()->getPrototypes(),
                             view->getTown(), view_2->getTown(), *view->getPriceUpgradeMap(), *view_2->getPriceUpgradeMap());
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
        setMaximumWidth(MIN_WINDOW_WIDTH);
        return;
    }
    setMaximumWidth(16777215);
}

bool Widget::event(QEvent * event)
{
    if(event->type() == QEvent::KeyRelease && (dynamic_cast<QKeyEvent *>(event))->key() == Qt::Key_Escape)
    {
        if(ui->stackedWidget->currentIndex() == 0)
            stopAllTimers();
        if(ui->stackedWidget->currentIndex() == 2)
            showMessageAboutUnsavedSettings();

        clearFocusOfMainMenu();
        int temp = lastVisitedPage;
        bool visible = ui->buttonContinue->isVisible();
        lastVisitedPage = ui->stackedWidget->currentIndex();
        ui->stackedWidget->setCurrentIndex(temp);

        if(temp == 0 && visible)
        {
            if(isGameOver)
            {
                setMaximumWidth(MIN_WINDOW_WIDTH);
                return true;
            }
            setMaximumWidth(16777215);
            startAllTimers();
        }
        else
        {
            setMaximumWidth(MIN_WINDOW_WIDTH);
            if(temp == 0 && ui->buttonContinue->isHidden())
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


void Widget::on_pushButtonExitFromFP_pressed()
{
    clearFocusOfMainMenu();
    lastVisitedPage = 4;
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::on_pushButtonExitFromSettings_pressed()
{
    showMessageAboutUnsavedSettings();
    clearFocusOfMainMenu();
    lastVisitedPage = 2;
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::selectVerticalHeaderItems() const
{
    if(QApplication::keyboardModifiers() != Qt::ControlModifier)
    {
        dynamic_cast<RbTableHeaderView *>(ui->tableWidget->verticalHeader())->setBackgroundColor(Qt::white);
        dynamic_cast<RbTableHeaderView *>(ui->tableWidget->verticalHeader())->setForegroundColor(Qt::black);
    }

    QModelIndex index;

    foreach(auto i, ui->tableWidget->selectedItems())
    {
        if(i->row() % 2 == 0)
            index = ui->tableWidget->verticalHeader()->model()->index(i->row(), 0);
        else index = ui->tableWidget->verticalHeader()->model()->index(i->row() - 1, 0);

        dynamic_cast<RbTableHeaderView *>(ui->tableWidget->verticalHeader())->setCellBackgroundColor(index, Qt::gray);
        dynamic_cast<RbTableHeaderView *>(ui->tableWidget->verticalHeader())->setCellForegroundColor(index, Qt::white);
    }

    ui->tableWidget->verticalHeader()->repaint();
}

void Widget::verticalHeaderSectionPressed(int beginSection, int endSection) const
{
    int tempEndSection = endSection;
    static int lastSection = endSection;

    if(beginSection < endSection && lastSection > endSection && QApplication::keyboardModifiers() != Qt::ControlModifier)
        ui->tableWidget->setRangeSelected(QTableWidgetSelectionRange(endSection - 1, 0, lastSection, 6), false);
    if(beginSection > endSection && lastSection < endSection && QApplication::keyboardModifiers() != Qt::ControlModifier)
        ui->tableWidget->setRangeSelected(QTableWidgetSelectionRange(lastSection - 1, 0, endSection, 6), false);

    QAbstractItemView::SelectionMode oldSelectionMode = ui->tableWidget->selectionMode();
    ui->tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    if(beginSection > endSection)
    {
        beginSection++;
        if(endSection % 2 == 1)
            endSection--;
        int temp = beginSection;
        beginSection = endSection;
        endSection = temp;
    }

    if(endSection - beginSection <= 1 && lastSection != endSection && QApplication::keyboardModifiers() != Qt::ControlModifier)
        ui->tableWidget->clearSelection();

    for (int i = beginSection; i < endSection + 1; ++i)
        ui->tableWidget->selectRow(i);

    ui->tableWidget->setSelectionMode(oldSelectionMode);
    lastSection = tempEndSection;
}
