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
#include <qdrawutil.h>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QStandardItem>


RbTableHeaderView::~RbTableHeaderView()
{
}

void RbTableHeaderView::setBackgroundColor(const QColor & color)
{
    RbTableHeaderModel * tblModel = qobject_cast<RbTableHeaderModel *>(this->model());
    const int LEVEL_CNT = tblModel->rowCount();
    for (int i = 0; i < LEVEL_CNT; ++i)
    {
        QModelIndex cellIndex = tblModel->index(i, 0);
        tblModel->setData(cellIndex, color, Qt::BackgroundRole);
    }
}

RbTableHeaderView::RbTableHeaderView(Qt::Orientation orientation, int rows, int columns, QWidget * parent) :
    QHeaderView(orientation,parent)
{
    QSize baseSectionSize;
    if (orientation == Qt::Horizontal)
    {
        baseSectionSize.setWidth(defaultSectionSize());
        baseSectionSize.setHeight(20);
    }
    else
    {
        baseSectionSize.setWidth(50);
        baseSectionSize.setHeight(defaultSectionSize());
    }

    // create header model
    RbTableHeaderModel * headerModel = new RbTableHeaderModel(rows, columns);

    // set default size of item
    for (int row = 0; row < rows; ++row)
        for (int col = 0; col < columns; ++col)
            headerModel->setData(headerModel->index(row, col), baseSectionSize, Qt::SizeHintRole);

    setModel(headerModel);

    connect(this, SIGNAL(sectionResized(int, int, int)), this, SLOT(onSectionResized(int, int, int)));
}

void RbTableHeaderView::setRowHeight(int row, int rowHeight)
{
    RbTableHeaderModel * md = qobject_cast<RbTableHeaderModel *>(model());
	const int cols = md->columnCount();
    for (int col = 0; col < cols; ++col)
	{
        QSize sz = md->index(row, col).data(Qt::SizeHintRole).toSize();
		sz.setHeight(rowHeight);
        md->setData(md->index(row, col), sz, Qt::SizeHintRole);
	}
	if (orientation() == Qt::Vertical)
        resizeSection(row, rowHeight);
}

void RbTableHeaderView::setColumnWidth(int col, int colWidth)
{
    RbTableHeaderModel * md = qobject_cast<RbTableHeaderModel *>(model());
	const int rows = md->rowCount();
    for (int row = 0; row < rows; ++row)
	{
        QSize sz = md->index(row, col).data(Qt::SizeHintRole).toSize();
		sz.setWidth(colWidth);
        md->setData(md->index(row, col), sz, Qt::SizeHintRole);
	}
	if (orientation() == Qt::Horizontal)
        resizeSection(col, colWidth);
}

void RbTableHeaderView::setForegroundColor(const QColor & color)
{
    RbTableHeaderModel * tblModel = qobject_cast<RbTableHeaderModel *>(this->model());
    const int LEVEL_CNT = tblModel->rowCount();
    for (int i = 0; i < LEVEL_CNT; ++i)
    {
        QModelIndex cellIndex = tblModel->index(i, 0);
        tblModel->setData(cellIndex, color, Qt::ForegroundRole);
    }
}

void RbTableHeaderView::setSpan(int row, int column, int rowSpanCount, int columnSpanCount)
{
    RbTableHeaderModel * md = qobject_cast<RbTableHeaderModel *>(model());
    QModelIndex idx = md->index(row, column);
	if (rowSpanCount > 0)
        md->setData(idx, rowSpanCount, ROW_SPAN_ROLE);
	if (columnSpanCount)
        md->setData(idx, columnSpanCount, COLUMN_SPAN_ROLE);
}

void RbTableHeaderView::setCellBackgroundColor(const QModelIndex & index, const QColor & color)
{
    RbTableHeaderModel * md = qobject_cast<RbTableHeaderModel *>(model());
    md->setData(index, color, Qt::BackgroundRole);
}

void RbTableHeaderView::setCellForegroundColor(const QModelIndex & index, const QColor & color)
{
    RbTableHeaderModel * md = qobject_cast<RbTableHeaderModel *>(model());
    md->setData(index, color, Qt::ForegroundRole);
}


void RbTableHeaderView::mousePressEvent(QMouseEvent * event)
{
	QHeaderView::mousePressEvent(event);

	QPoint pos = event->pos();
	QModelIndex index = indexAt(pos);
	const int OTN = orientation();
	if (index.isValid())
	{
		int beginSection = -1;
        int endSection = -1;
		int numbers = 0;
        numbers = getSectionRange(index, &beginSection, &endSection);
		if (numbers > 0)
		{
            emit sectionPressed(beginSection, endSection);
            if(event->button() == Qt::LeftButton)
                firstSectionPressed = beginSection;
			return;
		}
		else
		{
            const RbTableHeaderModel * tblModel = qobject_cast<RbTableHeaderModel *>(this->model());
            const int LEVEL_CNT = (OTN == Qt::Horizontal) ? tblModel->rowCount() : tblModel->columnCount();
            int logicalIdx = (OTN == Qt::Horizontal) ? index.column() : index.row();
            int curLevel = (OTN == Qt::Horizontal) ? index.row() : index.column();
            for (int i = 0; i < LEVEL_CNT; ++i)
			{
                QModelIndex cellIndex = (OTN == Qt::Horizontal) ? tblModel->index(i, logicalIdx) : tblModel->index(logicalIdx, i);
                numbers = getSectionRange(cellIndex, &beginSection, &endSection);
				if (numbers > 0)
				{
					if (beginSection <= logicalIdx && logicalIdx <= endSection)
					{
                        int beginLevel = (OTN == Qt::Horizontal) ? cellIndex.row() : cellIndex.column();
                        QVariant levelSpanCnt = cellIndex.data((OTN == Qt::Horizontal) ? ROW_SPAN_ROLE : COLUMN_SPAN_ROLE);
						if (!levelSpanCnt.isValid())
							continue;
                        int endLevel = beginLevel + levelSpanCnt.toInt() - 1;
						if (beginLevel <= curLevel && curLevel <= endLevel)
						{
                            emit sectionPressed(beginSection, endSection);
                            break;
						}
					}
				}
			}
		}
	}
}

QModelIndex RbTableHeaderView::indexAt(const QPoint & pos) const
{
    const RbTableHeaderModel * tblModel = qobject_cast<RbTableHeaderModel *>(this->model());
	const int OTN = orientation();
	const int ROWS = tblModel->rowCount();
	const int COLS = tblModel->columnCount();
	int logicalIdx = logicalIndexAt(pos);

	if (OTN == Qt::Horizontal)
	{
        int dY = 0;
        for (int row = 0; row < ROWS; ++row)
		{
            QModelIndex cellIndex = tblModel->index(row, logicalIdx);
			dY += cellIndex.data(Qt::SizeHintRole).toSize().height();
			if (pos.y() <= dY) return cellIndex;
		}
	}
	else
	{
        int dX = 0;
        for (int col = 0; col < COLS; ++col)
		{
			QModelIndex cellIndex = tblModel->index(logicalIdx,col);
			dX += cellIndex.data(Qt::SizeHintRole).toSize().width();
			if (pos.x() <= dX) return cellIndex;
		}
	}

	return QModelIndex();
}

void RbTableHeaderView::paintSection(QPainter * painter, const QRect & rect, int logicalIdx) const
{
    const RbTableHeaderModel * tblModel = qobject_cast<RbTableHeaderModel *>(this->model());
	const int OTN = orientation();
    const int LEVEL_CNT = (OTN == Qt::Horizontal) ? tblModel->rowCount() : tblModel->columnCount();
    for (int i = 0; i < LEVEL_CNT; ++i)
	{
        QModelIndex cellIndex = (OTN == Qt::Horizontal) ? tblModel->index(i, logicalIdx) : tblModel->index(logicalIdx, i);
		QSize cellSize=cellIndex.data(Qt::SizeHintRole).toSize();
		QRect sectionRect(rect);

		// set position of the cell
		if (OTN == Qt::Horizontal)
            sectionRect.setTop(rowSpanSize(logicalIdx, 0, i)); // distance from 0 to i-1 rows
		else
            sectionRect.setLeft(columnSpanSize(logicalIdx, 0, i));

		sectionRect.setSize(cellSize);

		// check up span column or row
		QModelIndex colSpanIdx = columnSpanIndex(cellIndex);
		QModelIndex rowSpanIdx = rowSpanIndex(cellIndex);
		if (colSpanIdx.isValid())
		{
			int colSpanFrom = colSpanIdx.column();
            int colSpanCnt = colSpanIdx.data(COLUMN_SPAN_ROLE).toInt();
            int colSpanTo = colSpanFrom + colSpanCnt - 1;
            int colSpan = columnSpanSize(cellIndex.row(), colSpanFrom, colSpanCnt);
			if (OTN == Qt::Horizontal)
				sectionRect.setLeft(sectionViewportPosition(colSpanFrom));
			else
			{
                sectionRect.setLeft(columnSpanSize(logicalIdx, 0, colSpanFrom));
				i = colSpanTo;
			}

			sectionRect.setWidth(colSpan);

			// check up  if the column span index has row span
			QVariant subRowSpanData = colSpanIdx.data(ROW_SPAN_ROLE);
			if (subRowSpanData.isValid())
			{
				int subRowSpanFrom = colSpanIdx.row();
                int subRowSpanCnt = subRowSpanData.toInt();
                int subRowSpanTo = subRowSpanFrom + subRowSpanCnt - 1;
                int subRowSpan = rowSpanSize(colSpanFrom, subRowSpanFrom, subRowSpanCnt);
				if (OTN == Qt::Vertical)
					sectionRect.setTop(sectionViewportPosition(subRowSpanFrom));
				else
				{
                    sectionRect.setTop(rowSpanSize(colSpanFrom, 0, subRowSpanFrom));
					i = subRowSpanTo;
				}
				sectionRect.setHeight(subRowSpan);
			}
            cellIndex = colSpanIdx;
		}
		if (rowSpanIdx.isValid())
		{
			int rowSpanFrom = rowSpanIdx.row();
            int rowSpanCnt = rowSpanIdx.data(ROW_SPAN_ROLE).toInt();
            int rowSpanTo = rowSpanFrom + rowSpanCnt - 1;
            int rowSpan = rowSpanSize(cellIndex.column(), rowSpanFrom, rowSpanCnt);
			if (OTN == Qt::Vertical)
				sectionRect.setTop(sectionViewportPosition(rowSpanFrom));
			else
			{
                sectionRect.setTop(rowSpanSize(logicalIdx, 0, rowSpanFrom));
				i = rowSpanTo;
			}
			sectionRect.setHeight(rowSpan);

			// check up if the row span index has column span
			QVariant subColSpanData = rowSpanIdx.data(COLUMN_SPAN_ROLE);
			if (subColSpanData.isValid())
			{
				int subColSpanFrom = rowSpanIdx.column();
                int subColSpanCnt = subColSpanData.toInt();
                int subColSpanTo = subColSpanFrom + subColSpanCnt - 1;
                int subColSpan = columnSpanSize(rowSpanFrom, subColSpanFrom, subColSpanCnt);
				if (OTN == Qt::Horizontal)
					sectionRect.setLeft(sectionViewportPosition(subColSpanFrom));
				else
				{
                    sectionRect.setLeft(columnSpanSize(rowSpanFrom, 0, subColSpanFrom));
					i = subColSpanTo;
				}
				sectionRect.setWidth(subColSpan);
			}
            cellIndex = rowSpanIdx;
		}

		// draw section with style
		QStyleOptionHeader sectionStyle;
		initStyleOption(&sectionStyle);
		sectionStyle.textAlignment = Qt::AlignCenter;
		sectionStyle.iconAlignment = Qt::AlignVCenter;
		sectionStyle.section = logicalIdx;
		sectionStyle.text = cellIndex.data(Qt::DisplayRole).toString();
		sectionStyle.rect = sectionRect;

		// file background or foreground color of the cell
		QVariant bg = cellIndex.data(Qt::BackgroundRole);
		QVariant fg = cellIndex.data(Qt::ForegroundRole);
        QVariant font = cellIndex.data(Qt::FontRole);
        if (bg.canConvert<QBrush>()/*qVariantCanConvert<QBrush>(bg)*/)
		{
			sectionStyle.palette.setBrush(QPalette::Button, qvariant_cast<QBrush>(bg));
			sectionStyle.palette.setBrush(QPalette::Window, qvariant_cast<QBrush>(bg));
		}
        if (fg.canConvert<QBrush>()/*qVariantCanConvert<QBrush>(fg)*/)
		{
			sectionStyle.palette.setBrush(QPalette::ButtonText, qvariant_cast<QBrush>(fg));
		}
        if (font.canConvert<QFont>())
        {
            /*qDebug() << "fgdfg";
            sectionStyle.palette.setBrush(QPalette::ButtonText, QBrush(Qt::cyan));
            sectionStyle.text = "fgdfg";
            model()->*/
        }

		painter->save();
        qDrawShadePanel(painter, sectionStyle.rect, sectionStyle.palette, false, 1, &sectionStyle.palette.brush(QPalette::Button));
		style()->drawControl(QStyle::CE_HeaderLabel, &sectionStyle, painter);
		painter->restore();
	}
}

QSize RbTableHeaderView::sectionSizeFromContents(int logicalIndex) const
{
    const RbTableHeaderModel * tblModel = qobject_cast<const RbTableHeaderModel *>(this->model());
	const int OTN = orientation();
    const int LEVEL_CNT = (OTN == Qt::Horizontal) ? tblModel->rowCount() : tblModel->columnCount();

	QSize siz = QHeaderView::sectionSizeFromContents(logicalIndex);
    for (int i = 0; i < LEVEL_CNT; ++i)
	{
        QModelIndex cellIndex = (OTN == Qt::Horizontal) ? tblModel->index(i,logicalIndex) : tblModel->index(logicalIndex, i);
		QModelIndex colSpanIdx = columnSpanIndex(cellIndex);
		QModelIndex rowSpanIdx = rowSpanIndex(cellIndex);
		siz=cellIndex.data(Qt::SizeHintRole).toSize();

		if (colSpanIdx.isValid())
		{
			int colSpanFrom = colSpanIdx.column();
            int colSpanCnt = colSpanIdx.data(COLUMN_SPAN_ROLE).toInt();
            int colSpanTo = colSpanFrom + colSpanCnt - 1;
            siz.setWidth(columnSpanSize(colSpanIdx.row(), colSpanFrom, colSpanCnt));
			if (OTN == Qt::Vertical) i = colSpanTo;
		}
		if (rowSpanIdx.isValid())
		{
			int rowSpanFrom = rowSpanIdx.row();
			int rowSpanCnt  = rowSpanIdx.data(ROW_SPAN_ROLE).toInt();
            int rowSpanTo   = rowSpanFrom + rowSpanCnt - 1;
            siz.setHeight(rowSpanSize(rowSpanIdx.column(), rowSpanFrom, rowSpanCnt));
			if (OTN == Qt::Horizontal) i = rowSpanTo;
		}
	}
    return siz;
}

void RbTableHeaderView::mouseMoveEvent(QMouseEvent * event)
{
    QHeaderView::mousePressEvent(event);
    if(event->buttons() == Qt::LeftButton)
    {
        QPoint pos = event->pos();
        QModelIndex index = indexAt(pos);
        if (index.isValid())
        {
            int beginSection = -1;
            int endSection = -1;
            int numbers = 0;
            numbers = getSectionRange(index, &beginSection, &endSection);
            if (numbers > 0)
            {
                emit sectionEntered(firstSectionPressed, endSection);
                return;
            }
        }
    }
}

QModelIndex RbTableHeaderView::columnSpanIndex(const QModelIndex & currentIdx) const
{
    const RbTableHeaderModel * tblModel = qobject_cast<RbTableHeaderModel *>(model());
	const int curRow = currentIdx.row();
	const int curCol = currentIdx.column();
	int i = curCol;
	while (i >= 0)
	{
       QModelIndex spanIndex = tblModel->index(curRow, i);
        QVariant span = spanIndex.data(COLUMN_SPAN_ROLE);
        if (span.isValid() && spanIndex.column() + span.toInt() - 1 >= curCol)
			return spanIndex;
		i--;
	}
	return QModelIndex();
}

QModelIndex RbTableHeaderView::rowSpanIndex(const QModelIndex & currentIdx) const
{
    const RbTableHeaderModel * tblModel = qobject_cast<RbTableHeaderModel *>(model());
	const int curRow = currentIdx.row();
	const int curCol = currentIdx.column();
	int i = curRow;
	while (i >= 0)
	{
        QModelIndex spanIndex = tblModel->index(i, curCol);
        QVariant span = spanIndex.data(ROW_SPAN_ROLE);
        if (span.isValid() && spanIndex.row() + span.toInt() - 1 >= curRow)
			return spanIndex;
		i--;
	}
	return QModelIndex();
}

int RbTableHeaderView::columnSpanSize(int row, int from, int spanCount) const
{
    const RbTableHeaderModel * tblModel = qobject_cast<RbTableHeaderModel *>(model());
	int span = 0;
    for (int i = from; i < from + spanCount; ++i)
	{
        QSize cellSize = tblModel->index(row, i).data(Qt::SizeHintRole).toSize();
		span += cellSize.width();
	}
	return span;
}


int RbTableHeaderView::rowSpanSize(int column, int from, int spanCount) const
{
    const RbTableHeaderModel * tblModel = qobject_cast<RbTableHeaderModel *>(model());
	int span = 0;
    for (int i = from; i < from + spanCount; ++i)
	{
        QSize cellSize = tblModel->index(i, column).data(Qt::SizeHintRole).toSize();
		span += cellSize.height();
	}
	return span;
}

/**
 * @return section numbers
 */
int RbTableHeaderView::getSectionRange(QModelIndex & index, int * beginSection, int * endSection) const
{
	// check up section range from the index
	QModelIndex colSpanIdx = columnSpanIndex(index);
	QModelIndex rowSpanIdx = rowSpanIndex(index);

	if (colSpanIdx.isValid())
	{
		int colSpanFrom = colSpanIdx.column();
        int colSpanCnt = colSpanIdx.data(COLUMN_SPAN_ROLE).toInt();
        int colSpanTo = colSpanFrom + colSpanCnt - 1;
		if (orientation() == Qt::Horizontal)
		{
			*beginSection = colSpanFrom;
            *endSection = colSpanTo;
			index = colSpanIdx;
			return colSpanCnt;
		}
		else
		{
			// check up  if the column span index has row span
			QVariant subRowSpanData = colSpanIdx.data(ROW_SPAN_ROLE);
			if (subRowSpanData.isValid())
			{
				int subRowSpanFrom = colSpanIdx.row();
                int subRowSpanCnt = subRowSpanData.toInt();
                int subRowSpanTo = subRowSpanFrom + subRowSpanCnt - 1;
				*beginSection = subRowSpanFrom;
                *endSection = subRowSpanTo;
				index = colSpanIdx;
				return subRowSpanCnt;
			}
		}
	}

	if (rowSpanIdx.isValid())
	{
		int rowSpanFrom = rowSpanIdx.row();
        int rowSpanCnt = rowSpanIdx.data(ROW_SPAN_ROLE).toInt();
        int rowSpanTo = rowSpanFrom + rowSpanCnt - 1;
		if (orientation() == Qt::Vertical)
		{
			*beginSection = rowSpanFrom;
            *endSection = rowSpanTo;
			index = rowSpanIdx;
			return rowSpanCnt;
		}
		else
		{
			// check up if the row span index has column span
			QVariant subColSpanData = rowSpanIdx.data(COLUMN_SPAN_ROLE);
			if (subColSpanData.isValid())
			{
				int subColSpanFrom = rowSpanIdx.column();
                int subColSpanCnt = subColSpanData.toInt();
                int subColSpanTo = subColSpanFrom + subColSpanCnt - 1;
				*beginSection = subColSpanFrom;
                *endSection = subColSpanTo;
				index = rowSpanIdx;
				return subColSpanCnt;
			}
		}
	}

	return 0;
}


void RbTableHeaderView::onSectionResized(int logicalIndex, int oldSize, int newSize)
{
    Q_UNUSED(oldSize);

    RbTableHeaderModel * tblModel = qobject_cast<RbTableHeaderModel *>(this->model());
	const int OTN = orientation();
    const int LEVEL_CNT = (OTN == Qt::Horizontal) ? tblModel->rowCount() : tblModel->columnCount();
	int pos = sectionViewportPosition(logicalIndex);
    int xx = (OTN == Qt::Horizontal) ? pos : 0;
    int yy = (OTN == Qt::Horizontal) ? 0 : pos;
    QRect sectionRect(xx, yy, 0, 0);
    for (int i = 0; i < LEVEL_CNT; ++i)
	{
        QModelIndex cellIndex = (OTN == Qt::Horizontal) ? tblModel->index(i, logicalIndex) : tblModel->index(logicalIndex, i);
        QSize cellSize = cellIndex.data(Qt::SizeHintRole).toSize();
		// set position of cell
		if (OTN == Qt::Horizontal)
		{
            sectionRect.setTop(rowSpanSize(logicalIndex, 0, i));
			cellSize.setWidth(newSize);
		}
		else
		{
            sectionRect.setLeft(columnSpanSize(logicalIndex, 0, i));
			cellSize.setHeight(newSize);
		}
        tblModel->setData(cellIndex, cellSize, Qt::SizeHintRole);

		QModelIndex colSpanIdx = columnSpanIndex(cellIndex);
		QModelIndex rowSpanIdx = rowSpanIndex(cellIndex);

		if (colSpanIdx.isValid())
		{
			int colSpanFrom = colSpanIdx.column();
			if (OTN == Qt::Horizontal)
				sectionRect.setLeft(sectionViewportPosition(colSpanFrom));
			else
			{
                sectionRect.setLeft(columnSpanSize(logicalIndex, 0, colSpanFrom));
			}

		}
		if (rowSpanIdx.isValid())
		{
			int rowSpanFrom = rowSpanIdx.row();
			if (OTN == Qt::Vertical)
				sectionRect.setTop(sectionViewportPosition(rowSpanFrom));
			else
                sectionRect.setTop(rowSpanSize(logicalIndex, 0, rowSpanFrom));
		}
		QRect rToUpdate(sectionRect);
		rToUpdate.setWidth(viewport()->width()-sectionRect.left());
		rToUpdate.setHeight(viewport()->height()-sectionRect.top());
		viewport()->update(rToUpdate.normalized());
	}
}
