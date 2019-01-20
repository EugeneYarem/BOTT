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

#include "RbTableHeaderItem.h"
#include "RbTableHeaderModel.h"


RbTableHeaderItem::RbTableHeaderItem(RbTableHeaderItem * parent) :
   column_prop(0), row_prop(0), parent_item(parent)
{
}

RbTableHeaderItem::RbTableHeaderItem(int arow, int acolumn, RbTableHeaderItem * parent) :
    column_prop(acolumn), row_prop(arow), parent_item(parent)
{
}

RbTableHeaderItem::~RbTableHeaderItem()
{
}

RbTableHeaderItem * RbTableHeaderItem::insertChild(int row, int col)
{
    RbTableHeaderItem * newChild = new RbTableHeaderItem(row, col, this);
    child_items.insert(QPair<int, int>(row, col), newChild);
    return newChild;
}

const RbTableHeaderItem * RbTableHeaderItem::child(int row, int col) const
{
    QHash<QPair<int, int>, RbTableHeaderItem *>::const_iterator itr = child_items.find(QPair<int, int>(row, col));
    if (itr != child_items.end()) return itr.value();
    return nullptr;
}

RbTableHeaderItem * RbTableHeaderItem::child(int row, int col)
{
    QHash<QPair<int, int>,RbTableHeaderItem *>::iterator itr = child_items.find(QPair<int, int>(row, col));
    if (itr != child_items.end()) return itr.value();
    return nullptr;
}

void RbTableHeaderItem::setText(const QString & text)
{
    role_datas.insert(Qt::DisplayRole, text);
}

QVariant RbTableHeaderItem::data(int role) const
{
    QHash<int, QVariant>::const_iterator itr = role_datas.find(role);
    if (itr != role_datas.end()) return itr.value();
    return QVariant();
}

void RbTableHeaderItem::setData(const QVariant & data, int role)
{
    role_datas.insert(role, data);
}

void RbTableHeaderItem::clear()
{
    QList<RbTableHeaderItem *> items = child_items.values();
    foreach (RbTableHeaderItem * item, child_items)
    {
        if (item) delete item;
    }
     child_items.clear();
}
