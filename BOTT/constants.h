#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <vector>
#include <QDir>
#include <QProcessEnvironment>
#include <Qt>

// Army
extern const int MAX_ARMY_SIZE;
extern const int TROOP_TYPES_COUNT;

// Army - units price
extern const int ARCHER_PRICE;
extern const int MAGE_PRICE;
extern const int RIDER_PRICE;
extern const int SOLDIER_PRICE;

// Army - upgrade coefficients
extern const double HP_DOCTORS_UPGRADE_COEFFICIENT;
extern const double MAGE_ATTACK_UPGRADE_NEW_PRICE_COEFFICIENT;
extern const double MAGE_HP_UPGRADE_NEW_PRICE_COEFFICIENT;
extern const double QUARANTINE_UPGRADE_NEW_PRICE_COEFFICIENT;
extern const int ARCHER_ATTACK_UPGRADE_COEFFICIENT;
extern const int ARCHER_HP_UPGRADE_COEFFICIENT;
extern const int HP_QUARANTINE_UPGRADE_COEFFICIENT;
extern const int MAGE_ATTACK_UPGRADE_COEFFICIENT;
extern const int MAGE_HP_UPGRADE_COEFFICIENT;
extern const int RIDER_ATTACK_UPGRADE_COEFFICIENT;
extern const int RIDER_HP_UPGRADE_COEFFICIENT;
extern const int SOLDIER_ATTACK_UPGRADE_COEFFICIENT;
extern const int SOLDIER_ATTACK_UPGRADE2_COEFFICIENT;
extern const int SOLDIER_ATTACK_UPGRADE3_COEFFICIENT;
extern const int SOLDIER_HP_UPGRADE_COEFFICIENT;
extern const int SOLDIER_HP_UPGRADE2_COEFFICIENT;

// Battlefield
extern const double ATTACK_TOWN_COEFFICIENT;
extern const int ARMY_CONTROL_TIMER_INTERVAL;
extern const int BATTLE_TIMER_INTERVAL;
extern const int DISTANCE_BETWEEN_ARMIES_FOR_START_BATTLE;
extern const int DISTANCE_BETWEEN_TROOPS_FOR_START_BATTLE;
extern const int DISTANCE_TO_TOWN1_TO_ATTACK;
extern const int DISTANCE_TO_TOWN2_TO_ATTACK;

// Control keys (P1 - player 1, P2 - player 2) for views
extern const Qt::Key CREATE_ARCHER_P1;
extern const Qt::Key CREATE_ARCHER_P2;
extern const Qt::Key CREATE_RIDER_P1;
extern const Qt::Key CREATE_RIDER_P2;
extern const Qt::Key CREATE_SOLDIER_P1;
extern const Qt::Key CREATE_SOLDIER_P2;
extern const Qt::Key CREATE_WIZARD_P1;
extern const Qt::Key CREATE_WIZARD_P2;
extern const Qt::Key EXIT_FROM_MENU_P1;
extern const Qt::Key EXIT_FROM_MENU_P2;
extern const Qt::Key MENU_P1;
extern const Qt::Key MENU_P2;
extern const Qt::Key MENU_DOWN_P1;
extern const Qt::Key MENU_DOWN_P2;
extern const Qt::Key MENU_SELECT_P1;
extern const Qt::Key MENU_SELECT_P2;
extern const Qt::Key MENU_UP_P1;
extern const Qt::Key MENU_UP_P2;

// General (for view.cpp and widget.cpp)
extern const int IN_MENU_INTERVAL; // интервал в мс, в течение которого игрок может находиться в игровом меню
extern const int MENU_PAUSE_INTERVAL; // интервал в мс, в течение которого игрок не может зайти в игровое меню после выхода из него
extern const int MIN_WINDOW_HEIGHT;
extern const int MIN_WINDOW_WIDTH;
extern const int MUSIC_VOLUME;
extern const int LAST_SCENE_X_FOR_BOTTOM_VIEW;
extern const int SCENE_HEIGHT;
extern const std::vector<int> ERRORS_READING_SETTINGS; // вектор ошибок при считывании настроек

// Keeper (paths to files with gama data)
extern const QProcessEnvironment env;
extern const QString DATA_DIR_PATH;
extern const QString DATABASE_FILE_PATH;
extern const QString LAST_GAME_FILE_PATH;
extern const QString SETTING_FILE_PATH;

// Price of upgrades
extern const int ARMOR_PRICE;
extern const int ARQUEBUS_PRICE;
extern const int DOCTORS_PRICE;
extern const int HAUBERK_PRICE;
extern const int MAGE_ATTACK_PRICE;
extern const int MAGE_HP_PRICE;
extern const int MINE_LEVEL_UP_PRICE;
extern const int QUARANTINE_PRICE;
extern const int WEAPON_PRICE;

// Town
extern const double INCOME_UPGRADE_NEW_PRICE_COEFFICIENT;
extern const int INCOME_INTERVAL;
extern const int INCOME_UPGRADE;
extern const int TOWN_HEALTH;
extern const int TOWN_INCOME;
extern const int TOWN_MONEY;

// Troops animation
extern const int ARCHER_ATTACK_ANIMATION_SLIDES_COUNT;
extern const int ARCHER_RUN_ANIMATION_SLIDES_COUNT;
extern const int MAGE_ATTACK_ANIMATION_SLIDES_COUNT;
extern const int MAGE_RUN_ANIMATION_SLIDES_COUNT;
extern const int RIDER_ATTACK_ANIMATION_SLIDES_COUNT;
extern const int RIDER_RUN_ANIMATION_SLIDES_COUNT;
extern const int SOLDIER_ATTACK_ANIMATION_SLIDES_COUNT;
extern const int SOLDIER_RUN_ANIMATION_SLIDES_COUNT;
extern const int TROOPS_HP_TEXT_HEIGHT_UNDER_TROOP;
extern const int TROOPS_STEP_WIDTH;

// Troops configuration
extern const int ARCHER_ATTACK;
extern const int ARCHER_HP;
extern const int ARCHER_INTERVAL;
extern const int ARCHER_REMAINING_TIME;
extern const int MAGE_ATTACK;
extern const int MAGE_HP;
extern const int MAGE_INTERVAL;
extern const int MAGE_REMAINING_TIME;
extern const int RIDER_ATTACK;
extern const int RIDER_HP;
extern const int RIDER_INTERVAL;
extern const int RIDER_REMAINING_TIME;
extern const int SOLDIER_ATTACK;
extern const int SOLDIER_HP;
extern const int SOLDIER_INTERVAL;
extern const int SOLDIER_REMAINING_TIME;

// Troops start position
extern const int LEFT_START_POSITION_X;
extern const int RIGHT_START_POSITION_X;
extern const int START_POSITION_Y;

// Some additional functions
inline int getMinPriceUpgrade()
{
    static int min = -1;
    if(min == -1)
    {
        std::vector<int> vec;
        vec.push_back(ARMOR_PRICE);
        vec.push_back(ARQUEBUS_PRICE);
        vec.push_back(DOCTORS_PRICE);
        vec.push_back(HAUBERK_PRICE);
        vec.push_back(MAGE_ATTACK_PRICE);
        vec.push_back(MAGE_HP_PRICE);
        vec.push_back(MINE_LEVEL_UP_PRICE);
        vec.push_back(QUARANTINE_PRICE);
        vec.push_back(WEAPON_PRICE);

        std::sort(vec.begin(), vec.end());
        min = vec[0];
    }

    return min;
}

#endif // CONSTANTS_H
