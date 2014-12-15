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

#ifndef PROFILESMODEL_H
#define PROFILESMODEL_H

#include <QtCore/QAbstractTableModel>
#include "profilesDB.h"

class CProfilesModel : public QAbstractTableModel
{
	public:
        CProfilesModel(CProfilesDatabase & db, QObject * parent = 0);
        virtual ~CProfilesModel();

		Qt::ItemFlags flags ( const QModelIndex & index ) const;
		QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
		QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
		int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
		int columnCount ( const QModelIndex &parent = QModelIndex() ) const;

		CProfilesDatabase::SProfile getProfile(const QModelIndex & index);

		void reload();

    private:
        Q_DISABLE_COPY(CProfilesModel)

	private:
        CProfilesDatabase & m_db;
};

#endif /* PROFILESMODEL_H */
