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

#ifndef RBTABLEHEADERVIEW_H_
#define RBTABLEHEADERVIEW_H_

#include <QHeaderView>

enum eRbHeaderRole
{
    COLUMN_SPAN_ROLE = Qt::UserRole + 1,
	ROW_SPAN_ROLE,
	COLUMN_SIZE_ROLE,
	ROW_SIZE_ROLE,
};

class RbTableHeaderView: public QHeaderView
{
	Q_OBJECT

    int firstSectionPressed;

public:
    RbTableHeaderView(Qt::Orientation orientation, int rows, int columns, QWidget * parent = nullptr);
    virtual ~RbTableHeaderView() override;

    void setBackgroundColor(const QColor & );
    void setCellBackgroundColor(const QModelIndex & index, const QColor & );
    void setCellForegroundColor(const QModelIndex & index, const QColor & );
    void setColumnWidth(int col, int colWidth);
    void setForegroundColor(const QColor & );
	void setRowHeight(int row, int rowHeight);
	void setSpan(int row, int column, int rowSpanCount, int columnSpanCount);

protected:
	// override
    virtual QModelIndex indexAt(const QPoint & ) const override;
    virtual QSize sectionSizeFromContents(int logicalIndex) const override;
    void mouseMoveEvent(QMouseEvent * event) override;
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void paintSection(QPainter * painter, const QRect & rect, int logicalIndex) const override;

	// inherent features
    int columnSpanSize(int row, int from, int spanCount) const;
    int getSectionRange(QModelIndex & index, int * beginSection, int * endSection) const;
    int rowSpanSize(int column, int from, int spanCount) const;
    QModelIndex columnSpanIndex(const QModelIndex & currentIndex) const;
    QModelIndex rowSpanIndex(const QModelIndex & currentIndex) const;

protected slots:
    void onSectionResized(int logicalIdx, int oldSize, int newSize);

signals:
    void sectionEntered(int from, int to);
	void sectionPressed(int from, int to);

};

#endif // RBTABLEHEADERVIEW_H_
