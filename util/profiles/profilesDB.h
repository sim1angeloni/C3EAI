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

#ifndef PROFILESDB_H
#define PROFILESDB_H

#include <QtCore/QString>
#include <QtCore/QList>

class QSettings;

class CProfilesDatabase
{
	public:
		struct SProfile
		{
            uint m_id;
            QString m_name;
            QString m_host;
            QString m_password;
            quint16 m_game_port;
            quint16 m_http_port;

			SProfile()
			{
				m_id = 0;
				m_name.clear();
				m_host.clear();
				m_password.clear();
				m_game_port = -1;
				m_http_port = -1;
			}

			SProfile(QString _name, QString	_host, QString _password, quint16 _game_port, quint16 _http_port)
			{
				m_id = 0;
				m_name = _name;
				m_host = _host;
				m_password = _password;
				m_game_port = _game_port;
				m_http_port = _http_port;
			}

			bool operator== ( const SProfile & other ) const
			{
				if (other.m_name == m_name) return true;

				return false;
			}

			bool isNull() const
			{
				if (*this == SProfile()) return true;

				return false;
			}

			const QVariant operator[] ( int i ) const
			{
				switch (i)
				{
					case 0: return QVariant(m_name);
				}

				return QVariant();
			}
		};

    public:
        CProfilesDatabase();
        virtual ~CProfilesDatabase();

		bool profileNameIsFree(QString name);
		bool addProfile(QString name, QString host, QString password, quint16 game_port, quint16 http_port);
		bool addProfile(SProfile profile);
		bool removeProfile(SProfile profile);
		bool removeProfile(QString name);
		bool updateProfile(QString name_old, SProfile profile);
		bool updateProfile(QString name_old, QString name, QString host, QString password, quint16 game_port, quint16 http_port);

		const QList<SProfile> & getProfiles() const	{ return m_profiles; }

    private:
        Q_DISABLE_COPY(CProfilesDatabase)

    private:
        void refresh();

        void tryCreateMasterKey();

        bool loadProfiles();
        bool loadProfile(uint index);
        bool makeProfileLine(SProfile profile, QString & output);

        bool loadIndexes(QList<uint> & output);
        bool addIndex(uint index);
        bool removeIndex(uint index);
        bool makeIndexesLine(QList<uint> &indexes, QString & output);
        bool getFreeIndex(uint & index);

	private:
        QSettings * m_pFile;
        QList<SProfile> m_profiles;
};

#endif /* PROFILESDB_H */
