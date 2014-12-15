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
#include "profilesModel.h"

static const int VIEW_COLUMNS = 1;

CProfilesModel::CProfilesModel(CProfilesDatabase & db, QObject * parent):
	QAbstractTableModel(parent),
	m_db(db)
{
	FUNCTION_LOG
}

CProfilesModel::~CProfilesModel()
{
	FUNCTION_LOG
}

Qt::ItemFlags CProfilesModel::flags ( const QModelIndex & index ) const
{
    Q_UNUSED(index)
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant CProfilesModel::data ( const QModelIndex & index, int role ) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (m_db.getProfiles().isEmpty())
		return QVariant();

	const int column = index.column();
	const int row = index.row();

	if (row > m_db.getProfiles().size() - 1)
		return QVariant();

    if (column > VIEW_COLUMNS - 1)
		return QVariant();

	return m_db.getProfiles()[row][column];
}

QVariant CProfilesModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation != Qt::Horizontal)
		return QVariant();

	switch (section)
        case 0: return QVariant(TR_FIELD_PROFILE_NAME);

	return QVariant();
}

int CProfilesModel::rowCount ( const QModelIndex & parent ) const
{
    Q_UNUSED(parent)
	return m_db.getProfiles().size();
}

int CProfilesModel::columnCount ( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return VIEW_COLUMNS;
}

CProfilesDatabase::SProfile CProfilesModel::getProfile(const QModelIndex & index)
{
	FUNCTION_LOG

	const int row = index.row();

	if (row < 0)
		return CProfilesDatabase::SProfile();

	if (m_db.getProfiles().isEmpty())
		return CProfilesDatabase::SProfile();

	if (row > m_db.getProfiles().size() - 1)
		return CProfilesDatabase::SProfile();

	return m_db.getProfiles()[row];
}

void CProfilesModel::reload()
{
    this->reset();
}
