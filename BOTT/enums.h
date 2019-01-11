#ifndef ENUMS_H
#define ENUMS_H

enum ConflictSide { Left, Right }; //сторона конфликта
enum DirectionInMenu { Down, Up }; // указывает направление фокуса в игровом меню (то есть какой пункт в игр. меню выбрал юзер - тот, что выше или ниже)
enum Status { Attack, Run, Stand }; //статус юнитов
enum ViewPosition { BottomView, TopView }; // указывает вертикальную позицию view - верхний или нижний

#endif // ENUMS_H
