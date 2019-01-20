/* Copyright 2016 Lee Cho Kang.
* email: pzesseto@gmail.com
* This file is part of the RbTableHeaderView.
*
* The RbTableHeaderView is free software: you can redistribute it
* and/or modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation, either version 3 of the
* License, or (at your option) any later version.
*
* The RbTableHeaderView is distributed in the hope that it will be
* useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
* Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with the RbTableHeaderView. If not, see http://www.gnu.org/licenses/.
*/
/*
* RbTableHeaderView.h
* Created on: 2016. 6. 13.
* Taken from https://www.opendesktop.org/p/1153715/
*/

#ifndef RBTABLEHEADERITEM_H
#define RBTABLEHEADERITEM_H

#include <QHash>
#include <QVariant>

class RbTableHeaderItem
{
    // properties
    int column_prop;
    int row_prop;

    // inherent features
    RbTableHeaderItem * parent_item;
    QHash<int, QVariant> role_datas;
    QHash<QPair<int, int>, RbTableHeaderItem * > child_items;

public:
    RbTableHeaderItem(int arow, int acolumn, RbTableHeaderItem * parent = nullptr);
    RbTableHeaderItem(RbTableHeaderItem * parent = nullptr);
    ~RbTableHeaderItem();

    // interface
    const RbTableHeaderItem * child(int row, int col) const;
    inline int column() const { return column_prop; }
    inline int row() const { return row_prop; }
    RbTableHeaderItem * child(int row, int col);
    RbTableHeaderItem * insertChild(int row, int col);
    RbTableHeaderItem * parent() { return parent_item; }
    QVariant data(int role = Qt::UserRole + 1) const;
    void clear();
    void setData(const QVariant & data, int role);
    void setText(const QString & text);

};

#endif // RBTABLEHEADERITEM_H
