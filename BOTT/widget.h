#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTime>
#include <QMediaPlayer>

namespace Ui {
    class Widget;
}

class View;
class Battlefield;
class QLabel;

class Widget : public QWidget
{
    Q_OBJECT

    Ui::Widget *ui;
    View * view; // Верхний view
    View * view_2; // Нижний view
    View * viewWithOpenMenu; // view, у которого активно меню
    Battlefield * btf;
    QMediaPlayer * musicPlayer;
    int lastVisitedPage;
    bool eventEvoke; // Переменная нужна для того, чтобы определить было ли обработано событие нажатие клавиши Esc одним из view.
    bool settingsChanged; // Переменная, указывающая на то, что настройки управления были изменены. При возвращении в гавное меню в случае, если она true, то настройки сохранятся в файл
    bool isFirstGame; // Указывает, что это первая игра в текущей сессии
    bool isGameOver; // Переменная нужна, чтобы при каждой победе показать только один белый фон с победной записью
    bool isSaved; // Указывает, что статистика была сохранена
    bool isExit; // Если false, то перед выходом будет показано сообщение, том, что игра не окончена, если true - просто закроется

    // Переменные, которые хранят данные для сохранения статистики
    // P1 - player 1, P2 - player 2
    QString * winner;
    QString gamerNameP1;
    QString gamerNameP2;
    QTime gameDuration;
    int earnedMoneyP1;
    int earnedMoneyP2;
    int wastedMoneyP1;
    int wastedMoneyP2;
    int countOfUnitsP1;
    int countOfUnitsP2;
    int countOfModificationP1;
    int countOfModificationP2;

    void createSettingsPage();
    void createStatisticsPage();
    bool isSettingLineEdit(QObject *);
    void installEventFilters();
    bool isLineEditOfFirstPlayer(QObject * );
    void clearFocusOfMainMenu();
    void stopAllTimers();
    void startAllTimers();
    void writeStatistics();
    void writeSettings();
    void gameOver();

    QLabel * gameOverLabel;

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void setGamerNameP1(QString );
    void setGamerNameP2(QString );
    void startNewGame();
    void save();
    void readSettings();
    void showStartDialog();
    void setExit();

private slots:
    void on_buttonSettings_pressed();
    void on_buttonContinue_pressed();
    void on_buttonNew_pressed();
    void on_buttonExit_pressed();
    void on_buttonStatistics_pressed();
    void on_buttonForPlayers_pressed();

    // Слоты для сбора статистики
    void winP1();
    void winP2();
    void earnedMoneyP1Plus(int);
    void earnedMoneyP2Plus(int);
    void wastedMoneyP1Plus(int);
    void wastedMoneyP2Plus(int);
    void countOfUnitsP1Plus();
    void countOfUnitsP2Plus();
    void countOfModificationP1Plus();
    void countOfModificationP2Plus();

    // Слоты управления аудиопроигрывателем
    void restartMusic(QMediaPlayer::State);
    void volumeChange(int);

public slots:
    void updateViewWithOpenMenu(View * );

    // QObject interface
public:
    bool event(QEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // WIDGET_H
