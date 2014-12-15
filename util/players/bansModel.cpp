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
#include "bansModel.h"

static const int VIEW_COLUMNS = 3;

CBansModel::CBansModel(QObject * parent):
	QAbstractTableModel(parent)
{
	FUNCTION_LOG
	m_bans.clear();
}

CBansModel::~CBansModel()
{
	FUNCTION_LOG
}

Qt::ItemFlags CBansModel::flags ( const QModelIndex & index ) const
{
    Q_UNUSED(index)
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant CBansModel::data ( const QModelIndex & index, int role ) const
{
	//C3EAI_LOG

	if (role != Qt::DisplayRole)
		return QVariant();

	if (m_bans.isEmpty())
		return QVariant();

	const int column = index.column();
	const int row = index.row();

	if (row > m_bans.size() - 1)
		return QVariant();

    if (column > VIEW_COLUMNS - 1)
		return QVariant();

	return m_bans[row][column];
}

QVariant CBansModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
	//C3EAI_LOG

	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation != Qt::Horizontal)
		return QVariant();

	switch (section)
	{
		case 0: return QVariant(TR_PLAYER_NICK);
		case 1: return QVariant(TR_PLAYER_PROFILE);
		case 2: return QVariant(TR_PLAYER_TIMEREMAINING);
	}

	return QVariant();
}

int CBansModel::rowCount ( const QModelIndex & parent ) const
{
    Q_UNUSED(parent)
	return m_bans.size();
}

int CBansModel::columnCount ( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return VIEW_COLUMNS;
}

bool CBansModel::addBan(SBan player)
{
	FUNCTION_LOG

	if (m_bans.contains(player))
		return false;

	m_bans.append(player);

	reset();
	return true;
}

CBansModel::SBan CBansModel::getBan(const QModelIndex & index)
{
	FUNCTION_LOG

	const int row = index.row();

	if (row < 0)
		return SBan();

	if (m_bans.isEmpty())
		return SBan();

	if (row > m_bans.size() - 1)
		return SBan();

	return m_bans[row];
}

bool CBansModel::addBan(QString name, int profileid, QString time_remaining)
{
	FUNCTION_LOG
	return addBan(SBan(name, profileid, time_remaining));
}

bool CBansModel::resetBans()
{
	FUNCTION_LOG

	m_bans.clear();

	reset();
	return true;
}
