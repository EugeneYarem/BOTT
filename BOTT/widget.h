#ifndef WIDGET_H
#define WIDGET_H

#include "enums.h"
#include <QEvent>
#include <QWidget>

namespace Ui {
    class Widget;
}

class Battlefield;
class Keeper;
class Message;
class View;
class QLabel;
class QLineEdit;
class QMediaPlayer;
class QPushButton;
class QSqlQuery;

class Widget : public QWidget
{
    Q_OBJECT

    Battlefield * btf;
    bool eventEvoke; // Переменная нужна для того, чтобы определить было ли обработано событие нажатие клавиши Esc одним из view.
    bool isGameOver; // Переменная нужна, чтобы при каждой победе показать только один белый фон с победной записью
    bool isSaved; // Указывает, что статистика была сохранена
    bool isStartDialogOpen; // Нужна, чтобы диалоговое окно старта новой игры не показывалось несколько раз
    bool settingsChanged; // Переменная, указывающая на то, что настройки управления были изменены. При возвращении в гавное меню в случае, если она true, то настройки сохранятся в файл
    bool wasNotAnyGame; // Указывает, что в текущей сессии игры не было еще начато ни одной игры
    int lastVisitedPage;
    int volume;
    Keeper * keeper;
    QLabel * gameOverLabel;
    QMediaPlayer * musicPlayer;
    Ui::Widget * ui;
    View * view; // Верхний view
    View * view_2; // Нижний view
    View * viewWithOpenMenu; // view, у которого активно меню

public:
    explicit Widget(QWidget * parent = nullptr);
    ~Widget() override;

    // QObject interface
    bool event(QEvent * event) override;
    bool eventFilter(QObject * watched, QEvent * event) override;

    void restoreLastGame();

    // QWidget interface
protected:
    void closeEvent(QCloseEvent * event) override;

private:
    bool checkKeyAndSet(QLineEdit * watched, Qt::Key key);
    bool checkViewAndSetEvent(View * view, QEvent * event);
    bool isLineEditOfFirstPlayer(const QObject * watched) const;
    bool isSettingLineEdit(const QObject * watched) const;
    void clearFocusOfMainMenu();
    void createConnectsForDispMess() const; // создание коннектов для вывода нужных сообщений от разных объектов
    void createMusicPlayerConnects();
    void createSettingButtonsConnects();
    void createSettingsPage() const;
    void createStatisticsConnects() const;
    void createStatisticsPage() const;
    void createStatisticsTable(const int & rowCount) const;
    void createViewsConnects();
    void fillInStatisticsTable(QSqlQuery & records) const;
    void installEventFilters();
    void save();
    void setRequiredBGIToMainMenuItem(const QEvent::Type & event, QPushButton * button, const MenuBG & bgType = MenuBG::MainBG) const; // BGI - background image
    void showMessageAboutUnsavedSettings();
    void showStartDialog();
    void startAllTimers();
    void startNewGame();
    void stopAllTimers();

public slots:
    void showMessage(QString text, QObject * sender = nullptr, int eventType = -1);

private slots:
    void gameOver();
    void on_buttonContinue_pressed();
    void on_buttonExit_pressed();
    void on_buttonForPlayers_pressed();
    void on_buttonNew_pressed();
    void on_buttonSettings_pressed();
    void on_buttonStatistics_pressed();
    void on_pushButtonExitFromFP_pressed();
    void on_pushButtonExitFromSettings_pressed();
    void selectVerticalHeaderItems() const;
    void verticalHeaderSectionPressed(int beginSection, int endSection) const;

};

#endif // WIDGET_H
