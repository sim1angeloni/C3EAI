/****************************************************************************************
** Copyright (C) 2011, 2015 Simone Angeloni
** This file is part of C3 External Admin Interface.
**
** C3 External Admin Interface is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** C3 External Admin Interface is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with C3 External Admin Interface. If not, see <http://www.gnu.org/licenses/>
**
****************************************************************************************/

#include "stdafx.h"
#include "reservedSlotsModel.h"

static const int VIEW_COLUMNS = 2;

CReservedSlotsModel::CReservedSlotsModel(QObject * parent):
	QAbstractTableModel(parent)
{
	FUNCTION_LOG
	m_slots.clear();
}

CReservedSlotsModel::~CReservedSlotsModel()
{
	FUNCTION_LOG
}

Qt::ItemFlags CReservedSlotsModel::flags ( const QModelIndex & index ) const
{
    Q_UNUSED(index)
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant CReservedSlotsModel::data ( const QModelIndex & index, int role ) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (m_slots.isEmpty())
		return QVariant();

	const int column = index.column();
	const int row = index.row();

	if (row > m_slots.size() - 1)
		return QVariant();

    if (column > VIEW_COLUMNS - 1)
		return QVariant();

	return m_slots[row][column];
}

QVariant CReservedSlotsModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation != Qt::Horizontal)
		return QVariant();

	switch (section)
	{
		case 0: return QVariant(TR_PLAYER_NICK);
		case 1: return QVariant(TR_PLAYER_PROFILE);
	}

	return QVariant();
}

int CReservedSlotsModel::rowCount ( const QModelIndex & parent ) const
{
    Q_UNUSED(parent)
	return m_slots.size();
}

int CReservedSlotsModel::columnCount ( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return VIEW_COLUMNS;
}

bool CReservedSlotsModel::addSlot(SSlot player)
{
	FUNCTION_LOG

	if (m_slots.contains(player))
		return false;

	m_slots.append(player);

    reset();
	return true;
}

CReservedSlotsModel::SSlot CReservedSlotsModel::getSlot(const QModelIndex & index)
{
	FUNCTION_LOG

	const int row = index.row();

	if (row < 0)
		return SSlot();

	if (m_slots.isEmpty())
		return SSlot();

	if (row > m_slots.size() - 1)
		return SSlot();

	return m_slots[row];
}

bool CReservedSlotsModel::addSlot(QString name, int profileid)
{
	FUNCTION_LOG
	return addSlot(SSlot(name, profileid));
}

bool CReservedSlotsModel::resetSlots()
{
	FUNCTION_LOG

	m_slots.clear();

	reset();
	return true;
}
