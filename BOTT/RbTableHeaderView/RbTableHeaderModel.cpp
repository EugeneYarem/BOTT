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
#include "RbTableHeaderView.h"


RbTableHeaderModel::~RbTableHeaderModel()
{
    root_item->clear();
    delete root_item;
}

RbTableHeaderModel::RbTableHeaderModel(int rows, int cols, QObject * parent) :
    QAbstractTableModel(parent), column_count_prop(cols), row_count_prop(rows), root_item(new RbTableHeaderItem())
{
}

QModelIndex RbTableHeaderModel::index(int row, int column, const QModelIndex & parent) const
{
    if (!hasIndex(row, column, parent)) return QModelIndex();

    RbTableHeaderItem * parentItem;
    if (!parent.isValid()) parentItem = root_item; // parent item is always the root_item on table model
    else parentItem = static_cast<RbTableHeaderItem *>(parent.internalPointer()); // no effect

    RbTableHeaderItem * childItem = parentItem->child(row, column);
    if (!childItem) childItem = parentItem->insertChild(row, column);
    return createIndex(row, column, childItem);

    return QModelIndex();
}

QVariant RbTableHeaderModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= row_count_prop || index.row() < 0 || index.column() >= column_count_prop || index.column() < 0)
        return QVariant();

    RbTableHeaderItem * item = static_cast<RbTableHeaderItem *>(index.internalPointer());

    return item->data(role);
}

bool RbTableHeaderModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (index.isValid())
    {
         RbTableHeaderItem * item = static_cast<RbTableHeaderItem *>(index.internalPointer());
         if (role == COLUMN_SPAN_ROLE)
         {
             int col = index.column();
             int span = value.toInt();
             if (span > 0) // span size should be more than 1, else nothing to do
             {
                 if (col + span - 1 >= column_count_prop) // span size should be less than whole columns,
                     span = column_count_prop - col;
                 item->setData(span, COLUMN_SPAN_ROLE);
             }
         }
         else if (role == ROW_SPAN_ROLE)
         {
             int row = index.row();
             int span = value.toInt();
             if (span > 0) // span size should be more than 1, else nothing to do
             {
                 if (row + span - 1 >= row_count_prop)
                     span = row_count_prop - row;
                 item->setData(span, ROW_SPAN_ROLE);
             }
         }
         else item->setData(value, role);

        return true;
    }
    return false;
}

Qt::ItemFlags RbTableHeaderModel::flags(const QModelIndex & index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
