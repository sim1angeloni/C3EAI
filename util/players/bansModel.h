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

#ifndef BANSMODEL_H
#define BANSMODEL_H

#include <QtCore/QAbstractTableModel>

class CBansModel : public QAbstractTableModel
{
	public:
		struct SBan
		{
            QString m_name;
            int m_profileid;
            QString m_time_remaining;

			SBan()
			{
				m_name.clear();
				m_profileid = -1;
				m_time_remaining.clear();
			}

			SBan(QString _name, int _profileid, QString _m_time_remaining)
			{
				m_name = _name;
				m_profileid = _profileid;
				m_time_remaining = _m_time_remaining;
			}

			bool operator== ( const SBan & other ) const
			{
				if (other.m_profileid == m_profileid) return true;

				return false;
			}

			bool isNull() const
			{
				if (*this == SBan()) return true;

				return false;
			}

			const QVariant operator[] ( int i ) const
			{
				switch (i)
				{
					case 0: return QVariant(m_name);
					case 1: return QVariant(QString::number(m_profileid));
					case 2: return QVariant(m_time_remaining);
				}

				return QVariant();
			}
		};

    public:
        CBansModel(QObject * parent = 0);
        virtual ~CBansModel();

		Qt::ItemFlags flags ( const QModelIndex & index ) const;
		QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
		QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
		int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
		int columnCount ( const QModelIndex &parent = QModelIndex() ) const;

		SBan getBan(const QModelIndex & index);
		bool addBan(SBan player);
		bool addBan(QString name, int profileid, QString time_remaining);
		bool resetBans();

    private:
        Q_DISABLE_COPY(CBansModel)

	private:
        QList<SBan> m_bans;
};

#endif /* BANSMODEL_H */
