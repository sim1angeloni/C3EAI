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

#ifndef SETTINGSFILE_H
#define SETTINGSFILE_H
#pragma once

#include <QtCore/QFile>
#include <QtCore/QSettings>

class CSettingsFile
{
	public:
        CSettingsFile();
        virtual ~CSettingsFile();

		/**
		* Allows the user to recover a string value from the input setting.
		* @param key Input key used to recover the value.
		* @return The value recovered from the settings file. If there is no value an empty value for the current type is returned.
		*/
		QString getValueString(const QString key);

		/**
		* Allows the user to recover an integer value from the input setting.
		* @param key Input key used to recover the value.
		* @return The value recovered from the settings file. If there is no value an empty value for the current type is returned.
		*/
		int getValueInteger(const QString key);

		/**
		* Allows the user to set a new value in the settings file.
		* If the value does not exist, it will be added.
		* @param key Input key.
		* @param value Value to write in the settings file. Only QString compatible values are accepted.
		*/
		void setValue(QString & key, QVariant & value);

    private:
        Q_DISABLE_COPY(CSettingsFile)

    private:
        void tryCreateFile();
        void addValue(const QString & key, const QString & value);

	private:
        QSettings * m_settings;
};

#endif /* SETTINGSFILE_H */
