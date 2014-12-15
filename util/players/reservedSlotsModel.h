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

#ifndef RESERVEDSLOTSMODEL_H
#define RESERVEDSLOTSMODEL_H

#include <QtCore/QAbstractTableModel>

class CReservedSlotsModel : public QAbstractTableModel
{
	public:
		struct SSlot
		{
            QString m_name;
            int m_profileid;

			SSlot()
			{
				m_name.clear();
				m_profileid = -1;
			}

			SSlot(QString _name, int _profileid)
			{
				m_name = _name;
				m_profileid = _profileid;
			}

			bool operator== ( const SSlot & other ) const
			{
				if (other.m_name == m_name) 			return true;

				return false;
			}

			bool isNull() const
			{
				if (*this == SSlot()) return true;

				return false;
			}

			const QVariant operator[] ( int i ) const
			{
				switch (i)
				{
					case 0: return QVariant(m_name);
					case 1: return QVariant(QString::number(m_profileid));
				}

				return QVariant();
			}
		};

    public:
        CReservedSlotsModel(QObject * parent = 0);
        virtual ~CReservedSlotsModel();

		Qt::ItemFlags flags ( const QModelIndex & index ) const;
		QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
		QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
		int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
		int columnCount ( const QModelIndex &parent = QModelIndex() ) const;

		SSlot getSlot(const QModelIndex & index);
		bool addSlot(SSlot player);
		bool addSlot(QString name, int profileid);
		bool resetSlots();

    private:
        Q_DISABLE_COPY(CReservedSlotsModel)

	private:
        QList<SSlot> m_slots;
};

#endif /* RESERVEDSLOTSMODEL_H */
