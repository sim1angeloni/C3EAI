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

#include <QtCore/QSettings>
#include "stdafx.h"
#include "profilesDB.h"

static const QString SEPARATOR(";");
static const int FIELDS = 5;
static const uint MAX_INDEX = 0xffffffff;

/* PROFILES KEYS */
static const QString PROFILE_N_KEY("PROFILE_N_");
static const QString PROFILE_INDEXES_KEY("PROFILE_INDEXES");
static const QString PROFILE_INDEXES_DEFVAL(" ");

CProfilesDatabase::CProfilesDatabase():
	m_pFile(NULL)
{
	FUNCTION_LOG

    m_pFile = new QSettings();

    if (!m_pFile->isWritable())
		LOGFatalErrorANDExit("Not allowed to use settings keys");

	tryCreateMasterKey();

	if (!loadProfiles())
		LOGError("Unable to reload profiles correctly");
}

CProfilesDatabase::~CProfilesDatabase()
{
	FUNCTION_LOG

    if (m_pFile) m_pFile->sync();
	SAFE_DELETE(m_pFile);
}

void CProfilesDatabase::tryCreateMasterKey()
{
	FUNCTION_LOG

    if (!m_pFile->contains(PROFILE_INDEXES_KEY))
    {
        LOGWarning(QString("'%1' missing in profile file, now setted to '%2'").arg(PROFILE_INDEXES_KEY).arg(PROFILE_INDEXES_DEFVAL));
        m_pFile->setValue(PROFILE_INDEXES_KEY, PROFILE_INDEXES_DEFVAL);
    }
}

bool CProfilesDatabase::loadProfiles()
{
	FUNCTION_LOG

	m_profiles.clear();

	QList<uint> indexes;
	if (!loadIndexes(indexes))
		return false;

	for (int i = 0; i < indexes.size(); ++i)
	{
		if (!loadProfile(indexes.at(i)))
		{
			indexes.removeAll(indexes.at(i));
		}
	}

	QString indexes_output;
	if (!makeIndexesLine(indexes, indexes_output))
	{
		LOGError("Unable to build indexes output line");
		return false;
	}

    m_pFile->setValue(PROFILE_INDEXES_KEY,	indexes_output);
	return true;
}

bool CProfilesDatabase::loadProfile(uint index)
{
    const QString profile_key = QString("%1%2").arg(PROFILE_N_KEY).arg(QString::number(index));

	if (!m_pFile->contains(profile_key))
	{
		LOGError(QString("Profile '%1' does not exist").arg(profile_key));
		return false;
	}

	QString profile_input = m_pFile->value(profile_key, QString()).toString();
	if (profile_input.isNull() || profile_input.isEmpty())
	{
		LOGError(QString("Profile '%1' is null or empty. Now removed").arg(profile_key));
		m_pFile->remove(profile_key);
		return false;
	}

	QList<QString> profile_list = profile_input.split(SEPARATOR, QString::SkipEmptyParts, Qt::CaseSensitive);
	if (profile_list.isEmpty() || profile_list.size() != FIELDS)
	{
		LOGError(QString("Profile '%1' is empty or incorrect. Now removed").arg(profile_key));
		m_pFile->remove(profile_key);
		return false;
	}

	SProfile current_profile;

	current_profile.m_id = index;
	current_profile.m_name = profile_list.at(0);
	current_profile.m_host = profile_list.at(1);
	current_profile.m_password = profile_list.at(2);

	bool m_game_port_isInt = false;
	current_profile.m_game_port = profile_list.at(3).toULong(&m_game_port_isInt);

	bool m_http_port_isInt = false;
	current_profile.m_http_port = profile_list.at(4).toUShort(&m_http_port_isInt);

	if (current_profile.m_name.isEmpty())
	{
		LOGError(QString("Profile '%1' is incorrect (%2=empty). Now removed").arg(profile_key).arg("name"));
		m_pFile->remove(profile_key);
		return false;
	}

	if (current_profile.m_host.isEmpty())
	{
		LOGError(QString("Profile '%1' is incorrect (%2=empty). Now removed").arg(profile_key).arg("host"));
		m_pFile->remove(profile_key);
		return false;
	}

	if (current_profile.m_password.isEmpty())
	{
		LOGError(QString("Profile '%1' is incorrect (%2=empty). Now removed").arg(profile_key).arg("password"));
		m_pFile->remove(profile_key);
		return false;
	}

	if (current_profile.m_game_port == 0 || !m_game_port_isInt)
	{
		LOGError(QString("Profile '%1' is incorrect (%2=0). Now removed").arg(profile_key).arg("game_port"));
		m_pFile->remove(profile_key);
		return false;
	}

	if (current_profile.m_http_port == 0 || !m_http_port_isInt)
	{
		LOGError(QString("Profile '%1' is incorrect (%2=0). Now removed").arg(profile_key).arg("http_port"));
		m_pFile->remove(profile_key);
		return false;
	}

	if (m_profiles.contains(current_profile))
	{
		LOGWarning(QString("Profile duplicated '%1'").arg(current_profile.m_name))
	}
	else
	{
		m_profiles.append(current_profile);
	}

	return true;
}

bool CProfilesDatabase::profileNameIsFree(QString name)
{
	FUNCTION_LOG

	name = name.simplified();

	for (int i = 0; i < m_profiles.size(); ++i)
	{
		if (QString::compare(m_profiles.at(i).m_name, name) == 0)
		{
			return false;
		}
	}

	return true;
}

bool CProfilesDatabase::addProfile(QString name, QString host, QString password, quint16 game_port, quint16 http_port)
{
	FUNCTION_LOG
	return addProfile(SProfile(name, host, password, game_port, http_port));
}

bool CProfilesDatabase::addProfile(SProfile profile)
{
	FUNCTION_LOG

	profile.m_name = profile.m_name.simplified();
	profile.m_host = profile.m_host.simplified();
	profile.m_password = profile.m_password.simplified();

	if (!profileNameIsFree(profile.m_name))
		return false;

	uint new_index;
	if (!getFreeIndex(new_index))
		return false;

    const QString profile_key = QString("%1%2").arg(PROFILE_N_KEY).arg(QString::number(new_index));

	if (m_pFile->contains(profile_key))
	{
		LOGWarning("Index was already in use. Old content is now deleted");
		m_pFile->remove(profile_key);
	}

	if (!addIndex(new_index))
		return false;

	QString profile_line;
	if (!makeProfileLine(profile, profile_line))
	{
		LOGError("Unable to build profile output line");
		return false;
	}

	LOGInfo(QString("Saved profile '%1' with index '%2'").arg(profile.m_name.simplified()).arg(QString::number(new_index)));
	m_pFile->setValue(profile_key, profile_line);

	refresh();
	return true;
}

bool CProfilesDatabase::removeProfile(SProfile profile)
{
	FUNCTION_LOG
	return removeProfile(profile.m_name);
}

bool CProfilesDatabase::removeProfile(QString name)
{
	FUNCTION_LOG

	name = name.simplified();

	uint remove_index;
	bool remove_index_found = false;
	for (int i = 0; i < m_profiles.size(); ++i)
	{
		if (QString::compare(m_profiles.at(i).m_name, name) == 0)
		{
			remove_index = m_profiles.at(i).m_id;
			remove_index_found = true;
			break;
		}
	}

	if (!remove_index_found)
	{
		LOGError(QString("There is not a profile with name '%1'").arg(name));
		return false;
	}

    const QString profile_key = QString("%1%2").arg(PROFILE_N_KEY).arg(QString::number(remove_index));
	if (!m_pFile->contains(profile_key))
	{
		LOGError(QString("There is not a profile for key '%1'").arg(profile_key));
		return false;
	}

	if (!removeIndex(remove_index))
	{
		return false;
	}

	LOGInfo(QString("Removed profile '%1' with index '%2'").arg(name.simplified()).arg(QString::number(remove_index)));
	m_pFile->remove(profile_key);

	refresh();
	return true;
}

bool CProfilesDatabase::updateProfile(QString name_old, SProfile profile)
{
	FUNCTION_LOG

	name_old = name_old.simplified();
	profile.m_name = profile.m_name.simplified();
	profile.m_host = profile.m_host.simplified();
	profile.m_password = profile.m_password.simplified();

	uint update_index;
	bool update_index_found = false;
	for (int i = 0; i < m_profiles.size(); ++i)
	{
		if (QString::compare(m_profiles.at(i).m_name, name_old) == 0)
		{
			update_index = m_profiles.at(i).m_id;
			update_index_found = true;
			break;
		}
	}

	if (!update_index_found)
	{
		LOGError(QString("There is not a profile with name '%1'").arg(profile.m_name));
		return false;
	}

    const QString profile_key = QString("%1%2").arg(PROFILE_N_KEY).arg(QString::number(update_index));
	if (!m_pFile->contains(profile_key))
	{
		LOGError(QString("There is not a profile for key '%1'").arg(profile_key));
		return false;
	}

	QString profile_line;
	if (!makeProfileLine(profile, profile_line))
	{
		LOGError("Unable to build profile output line");
		return false;
	}

	LOGInfo(QString("Update profile '%1' with index '%2'").arg(profile.m_name.simplified()).arg(QString::number(update_index)));
	m_pFile->setValue(profile_key, profile_line);

	refresh();
	return true;
}

bool CProfilesDatabase::updateProfile(QString name_old, QString name, QString host, QString password, quint16 game_port, quint16 http_port)
{
	FUNCTION_LOG
	return updateProfile(name_old, SProfile(name, host, password, game_port, http_port));
}

bool CProfilesDatabase::makeProfileLine(SProfile profile, QString & output)
{
	FUNCTION_LOG

	output.clear();

	if (profile.m_name.isEmpty())
	{
		LOGError(QString("Profile incorrect (%1=empty).").arg("name"));
		return false;
	}

	if (profile.m_host.isEmpty())
	{
		LOGError(QString("Profile incorrect (%1=empty).").arg("host"));
		return false;
	}

	if (profile.m_password.isEmpty())
	{
		LOGError(QString("Profile incorrect (%1=empty).").arg("password"));
		return false;
	}

	if (profile.m_game_port == 0)
	{
		LOGError(QString("Profile incorrect (%1=0).").arg("game_port"));
		return false;
	}

	if (profile.m_http_port == 0)
	{
		LOGError(QString("Profile incorrect (%1=0).").arg("http_port"));
		return false;
	}

	output += profile.m_name;
	output += SEPARATOR;
	output += profile.m_host;
	output += SEPARATOR;
	output += profile.m_password;
	output += SEPARATOR;
	output += QString::number(profile.m_game_port);
	output += SEPARATOR;
	output += QString::number(profile.m_http_port);

	return true;
}

void CProfilesDatabase::refresh()
{
	FUNCTION_LOG

	if (!loadProfiles())
		LOGError("Unable to reload profiles correctly");
}

bool CProfilesDatabase::loadIndexes(QList<uint> & output)
{
	FUNCTION_LOG

	output.clear();

    QString indexes_input = m_pFile->value(PROFILE_INDEXES_KEY, QString()).toString();
	if (indexes_input.isNull())
	{
		LOGError("Unable to load profile indexes. Now restored.");
        m_pFile->setValue(PROFILE_INDEXES_KEY,	PROFILE_INDEXES_DEFVAL);
		return false;
	}

    if (QString::compare(indexes_input, PROFILE_INDEXES_DEFVAL) == 0)
	{
		LOGInfo("There are no indexes");
		return true;
	}

	indexes_input = indexes_input.simplified();

	QList<QString> indexes_list = indexes_input.split(SEPARATOR, QString::SkipEmptyParts, Qt::CaseSensitive);
	if (indexes_list.isEmpty())
	{
		LOGError("Unable to split profile indexes. Now restored.");
        m_pFile->setValue(PROFILE_INDEXES_KEY, PROFILE_INDEXES_DEFVAL);
		return false;
	}

	for (int i = 0; i < indexes_list.size(); ++i)
	{
		QString current_idx_str = indexes_list.at(i);

		bool isInt = false;
		uint current_idx = current_idx_str.toUInt(&isInt);
		if (!isInt)
		{
			LOGWarning(QString("Index '%1' is not a number.").arg(current_idx_str));
			continue;
		}

		output.append(current_idx);
	}

	return true;
}

bool CProfilesDatabase::addIndex(uint index)
{
	FUNCTION_LOG

	QList<uint> indexes;
	if (!loadIndexes(indexes))
		return false;

	if (indexes.contains(index))
	{
		LOGError(QString("Index '%1' already exists").arg(index));
		return false;
	}

	indexes.append(index);

	QString indexes_output;
	if (!makeIndexesLine(indexes, indexes_output))
	{
		LOGError("Unable to build indexes output line");
		return false;
	}

    m_pFile->setValue(PROFILE_INDEXES_KEY,	indexes_output);
	return true;
}

bool CProfilesDatabase::removeIndex(uint index)
{
	FUNCTION_LOG

	QList<uint> indexes;
	if (!loadIndexes(indexes))
		return false;

	if (!indexes.contains(index))
	{
		LOGError(QString("Index '%1' is not in list").arg(index));
		return false;
	}

	indexes.removeAll(index);

	QString indexes_output;
	if (!makeIndexesLine(indexes, indexes_output))
	{
		LOGError("Unable to build indexes output line");
		return false;
	}

    m_pFile->setValue(PROFILE_INDEXES_KEY,	indexes_output);
	return true;
}

bool CProfilesDatabase::makeIndexesLine(QList<uint> &indexes, QString & output)
{
	FUNCTION_LOG

	output.clear();

	if (indexes.isEmpty())
	{
        output = PROFILE_INDEXES_DEFVAL;
		return true;
	}

	for (int i = 0; i < indexes.size(); ++i)
	{
		output += QString("%1;").arg(QString::number(indexes.at(i)));
	}

	output = output.left(output.size() - 1);
	return true;
}

bool CProfilesDatabase::getFreeIndex(uint & index)
{
	FUNCTION_LOG

	index = 0;

	bool new_index_found = false;
	while (index <= MAX_INDEX)
	{
		bool value_already_used = false;

		for (int i = 0; i < m_profiles.size(); ++i)
		{
			if (m_profiles.at(i).m_id == index)
			{
				value_already_used = true;
				break;
			}
		}

		if (!value_already_used)
		{
			new_index_found = true;
			break;
		}

		index++;
	}

	if (!new_index_found)
	{
		LOGError("Unable to find a new index where to save the current profile");
		return false;
	}

	return true;
}
