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

#ifndef RBTABLEHEADERMODEL_H
#define RBTABLEHEADERMODEL_H

#include <QAbstractTableModel>

class RbTableHeaderItem;

class RbTableHeaderModel: public QAbstractTableModel
{
    Q_OBJECT

    // properties
    int column_count_prop;
    int row_count_prop;

    // inherent features
    RbTableHeaderItem * root_item;

public:
    RbTableHeaderModel(int rows, int cols, QObject * parent = nullptr);
    virtual ~RbTableHeaderModel() override;

    // override
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    virtual int columnCount(const QModelIndex & parent = QModelIndex()) const override { Q_UNUSED(parent); return column_count_prop; }
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const override { Q_UNUSED(parent); return row_count_prop; }
    virtual QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex & index, int role) const override;
    virtual Qt::ItemFlags flags(const QModelIndex & index) const override;

};

#endif // RBTABLEHEADERMODEL_H
