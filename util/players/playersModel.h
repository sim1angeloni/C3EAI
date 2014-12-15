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

#ifndef PLAYERSMODEL_H
#define PLAYERSMODEL_H

#include <QtCore/QAbstractTableModel>

class CPlayersModel : public QAbstractTableModel
{
	public:
		struct SPlayer
		{
            QString m_name;
            int m_id;
            QString	m_ip;
            int m_ping;
            int m_state;
            int m_profile;

			SPlayer()
			{
				m_name.clear();
				m_id = -1;
				m_ip.clear();
				m_ping = -1;
				m_state = -1;
				m_profile = -1;
			}

			SPlayer(QString _name, int _id, QString _ip, int _ping, int _state, int _profile)
			{
				m_name = _name;
				m_id = _id;
				m_ip = _ip;
				m_ping = _ping;
				m_state = _state;
				m_profile = _profile;
			}

			bool operator== ( const SPlayer & other ) const
			{
				if (other.m_name == m_name) return true;

				return false;
			}

			bool isNull() const
			{
				if (*this == SPlayer()) return true;

				return false;
			}

			const QVariant operator[] ( int i ) const
			{
				switch (i)
				{
					case 0: return QVariant(m_name);
					case 1: return QVariant(m_ip);
					case 2: return QVariant(QString::number(m_ping));
					case 3: return QVariant(QString::number(m_state));
					case 4: return QVariant(QString::number(m_id));
					case 5: return QVariant(QString::number(m_profile));
				}

				return QVariant();
			}
		};

    public:
        CPlayersModel(QObject * parent = 0);
        virtual ~CPlayersModel();

		Qt::ItemFlags flags ( const QModelIndex & index ) const;
		QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
		QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
		int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
		int columnCount ( const QModelIndex &parent = QModelIndex() ) const;

		SPlayer getPlayer(const QModelIndex & index);
		bool addPlayer(SPlayer player);
		bool addPlayer(QString name, int id, QString ip, int ping, int state, int profileid);
		bool resetPlayers();

    private:
        Q_DISABLE_COPY(CPlayersModel)

	private:
        QList<SPlayer> m_players;
};

#endif /* PLAYERSMODEL_H */
