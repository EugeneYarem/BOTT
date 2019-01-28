#include "constants.h"


// Army
const int MAX_ARMY_SIZE = 6;
const int TROOP_TYPES_COUNT = 4;

// Army - units price
const int ARCHER_PRICE = 1000;
const int MAGE_PRICE = 2000;
const int RIDER_PRICE = 3000;
const int SOLDIER_PRICE = 1000;

// Army - upgrade coefficients
const double HP_DOCTORS_UPGRADE_COEFFICIENT = 2;
const double MAGE_ATTACK_UPGRADE_NEW_PRICE_COEFFICIENT = 1.3;
const double MAGE_HP_UPGRADE_NEW_PRICE_COEFFICIENT = 1.3;
const double QUARANTINE_UPGRADE_NEW_PRICE_COEFFICIENT = 1.6;
const int ARCHER_ATTACK_UPGRADE_COEFFICIENT = 100;
const int ARCHER_HP_UPGRADE_COEFFICIENT = 400;
const int HP_QUARANTINE_UPGRADE_COEFFICIENT = 50;
const int MAGE_ATTACK_UPGRADE_COEFFICIENT = 30;
const int MAGE_HP_UPGRADE_COEFFICIENT = 300;
const int RIDER_ATTACK_UPGRADE_COEFFICIENT = 70;
const int RIDER_HP_UPGRADE_COEFFICIENT = 600;
const int SOLDIER_ATTACK_UPGRADE_COEFFICIENT = 30;
const int SOLDIER_ATTACK_UPGRADE2_COEFFICIENT = 20;
const int SOLDIER_ATTACK_UPGRADE3_COEFFICIENT = 40;
const int SOLDIER_HP_UPGRADE_COEFFICIENT = 250;
const int SOLDIER_HP_UPGRADE2_COEFFICIENT = 300;

// Battlefield
const double ATTACK_TOWN_COEFFICIENT = 0.01;
const int ARMY_CONTROL_TIMER_INTERVAL = 200;
const int BATTLE_TIMER_INTERVAL = 2000;
const int DISTANCE_BETWEEN_ARMIES_FOR_START_BATTLE = 150;
const int DISTANCE_BETWEEN_TROOPS_FOR_START_BATTLE = 100;
const int DISTANCE_TO_TOWN1_TO_ATTACK = 310;
const int DISTANCE_TO_TOWN2_TO_ATTACK = 2090;

// Control keys (P1 - player 1, P2 - player 2) for views
const Qt::Key CREATE_ARCHER_P1 = Qt::Key_2;
const Qt::Key CREATE_ARCHER_P2 = Qt::Key_8;
const Qt::Key CREATE_RIDER_P1 = Qt::Key_3;
const Qt::Key CREATE_RIDER_P2 = Qt::Key_9;
const Qt::Key CREATE_SOLDIER_P1 = Qt::Key_1;
const Qt::Key CREATE_SOLDIER_P2 = Qt::Key_7;
const Qt::Key CREATE_WIZARD_P1 = Qt::Key_4;
const Qt::Key CREATE_WIZARD_P2 = Qt::Key_0;
const Qt::Key EXIT_FROM_MENU_P1 = Qt::Key_E;
const Qt::Key EXIT_FROM_MENU_P2 = Qt::Key_Backspace;
const Qt::Key MENU_P1 = Qt::Key_Q;
const Qt::Key MENU_P2 = Qt::Key_M;
const Qt::Key MENU_DOWN_P1 = Qt::Key_S;
const Qt::Key MENU_DOWN_P2 = Qt::Key_Down;
const Qt::Key MENU_SELECT_P1 = Qt::Key_F;
const Qt::Key MENU_SELECT_P2 = Qt::Key_Return;
const Qt::Key MENU_UP_P1 = Qt::Key_W;
const Qt::Key MENU_UP_P2 = Qt::Key_Up;

// General (for view.cpp and widget.cpp)
const int IN_MENU_INTERVAL = 20000; // интервал в мс, в течение которого игрок может находиться в игровом меню
const int MENU_PAUSE_INTERVAL = 15000; // интервал в мс, в течение которого игрок не может зайти в игровое меню после выхода из него
const int MIN_WINDOW_HEIGHT = 720;
const int MIN_WINDOW_WIDTH = 1280;
const int MUSIC_VOLUME = 50;
const int LAST_SCENE_X_FOR_BOTTOM_VIEW = MIN_WINDOW_WIDTH - 25;
const int SCENE_HEIGHT = MIN_WINDOW_HEIGHT / 2 - 17;
const std::vector<int> ERRORS_READING_SETTINGS = {0, 1, 2, 3, 4, 5, 6, 7, 8}; // вектор ошибок при считывании настроек

// Keeper (paths to files with gama data)
const QProcessEnvironment env(QProcessEnvironment::systemEnvironment());
const QString DATA_DIR_PATH = QDir::toNativeSeparators(env.value("USERPROFILE") + "/AppData/Local/Battle Of The Towns");
const QString DATABASE_FILE_PATH = QDir::toNativeSeparators(env.value("USERPROFILE") + "/AppData/Local/Battle Of The Towns/statistics.sqlite");
const QString LAST_GAME_FILE_PATH = QDir::toNativeSeparators(env.value("USERPROFILE") + "/AppData/Local/Battle Of The Towns/lastGame.json");
const QString SETTING_FILE_PATH = QDir::toNativeSeparators(env.value("USERPROFILE") + "/AppData/Local/Battle Of The Towns/settings.json");

// Price of upgrades
const int ARMOR_PRICE = 8000;
const int ARQUEBUS_PRICE = 5000;
const int DOCTORS_PRICE = 5000;
const int HAUBERK_PRICE = 5000;
const int MAGE_ATTACK_PRICE = 2000;
const int MAGE_HP_PRICE = 2000;
const int MINE_LEVEL_UP_PRICE = 6000;
const int QUARANTINE_PRICE = 500;
const int WEAPON_PRICE = 12000;

// Town
const double INCOME_UPGRADE_NEW_PRICE_COEFFICIENT = 1.7;
const int INCOME_INTERVAL = 4500;
const int INCOME_UPGRADE = 500;
const int TOWN_HEALTH = 100;
const int TOWN_INCOME = 1000;
const int TOWN_MONEY = 10000;

// Troops animation
const int ARCHER_ATTACK_ANIMATION_SLIDES_COUNT = 7;
const int ARCHER_RUN_ANIMATION_SLIDES_COUNT = 7;
const int MAGE_ATTACK_ANIMATION_SLIDES_COUNT = 6;
const int MAGE_RUN_ANIMATION_SLIDES_COUNT = 8;
const int RIDER_ATTACK_ANIMATION_SLIDES_COUNT = 4;
const int RIDER_RUN_ANIMATION_SLIDES_COUNT = 8;
const int SOLDIER_ATTACK_ANIMATION_SLIDES_COUNT = 7;
const int SOLDIER_RUN_ANIMATION_SLIDES_COUNT = 8;
const int TROOPS_HP_TEXT_HEIGHT_UNDER_TROOP = 30;
const int TROOPS_STEP_WIDTH = 5;

// Troops configuration
const int ARCHER_ATTACK = 70;
const int ARCHER_HP = 1000;
const int ARCHER_INTERVAL = 100;
const int ARCHER_REMAINING_TIME = 100;
const int MAGE_ATTACK = 50;
const int MAGE_HP = 500;
const int MAGE_INTERVAL = 100;
const int MAGE_REMAINING_TIME = 100;
const int RIDER_ATTACK = 180;
const int RIDER_HP = 1500;
const int RIDER_INTERVAL = 100;
const int RIDER_REMAINING_TIME = 100;
const int SOLDIER_ATTACK = 100;
const int SOLDIER_HP = 1000;
const int SOLDIER_INTERVAL = 100;
const int SOLDIER_REMAINING_TIME = 100;

// Troops start position
const int LEFT_START_POSITION_X = 300;
const int RIGHT_START_POSITION_X = 2110;
const int START_POSITION_Y = 220;
