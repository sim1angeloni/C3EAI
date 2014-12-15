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
#include "playersModel.h"

static const int VIEW_COLUMNS = 6;

CPlayersModel::CPlayersModel(QObject * parent):
	QAbstractTableModel(parent)
{
	FUNCTION_LOG
	m_players.clear();
}

CPlayersModel::~CPlayersModel()
{
	FUNCTION_LOG
}

Qt::ItemFlags CPlayersModel::flags ( const QModelIndex & index ) const
{
    Q_UNUSED(index)
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant CPlayersModel::data ( const QModelIndex & index, int role ) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (m_players.isEmpty())
		return QVariant();

	const int column = index.column();
	const int row = index.row();

	if (row > m_players.size() - 1)
		return QVariant();

    if (column > VIEW_COLUMNS - 1)
		return QVariant();

	return m_players[row][column];
}

QVariant CPlayersModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation != Qt::Horizontal)
		return QVariant();

	switch (section)
	{
		case 0: return QVariant(TR_PLAYER_NICK);
		case 1: return QVariant(TR_PLAYER_IP_PORT);
		case 2: return QVariant(TR_PLAYER_PING);
		case 3: return QVariant(TR_PLAYER_STATUS);
		case 4: return QVariant(TR_PLAYER_ID);
		case 5: return QVariant(TR_PLAYER_PROFILE);
	}

	return QVariant();
}

int CPlayersModel::rowCount ( const QModelIndex & parent ) const
{
    Q_UNUSED(parent)
	return m_players.size();
}

int CPlayersModel::columnCount ( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return VIEW_COLUMNS;
}

bool CPlayersModel::addPlayer(SPlayer player)
{
	FUNCTION_LOG

	if (m_players.contains(player))
		return false;

	m_players.append(player);

	reset();
	return true;
}

CPlayersModel::SPlayer CPlayersModel::getPlayer(const QModelIndex & index)
{
	FUNCTION_LOG

	const int row = index.row();

	if (row < 0)
		return SPlayer();

	if (m_players.isEmpty())
		return SPlayer();

	if (row > m_players.size() - 1)
		return SPlayer();

	return m_players[row];
}

bool CPlayersModel::addPlayer(QString name, int id, QString ip, int ping, int state, int profileid)
{
	FUNCTION_LOG
	return addPlayer(SPlayer(name, id, ip, ping, state, profileid));
}

bool CPlayersModel::resetPlayers()
{
	FUNCTION_LOG

	m_players.clear();

	reset();
	return true;
}
