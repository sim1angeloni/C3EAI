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

#ifndef CHANGEMAP_H
#define CHANGEMAP_H

#include <QtCore/QObject>
#include <QtGui/QWidget>
#include "ui_widget_changemap.h"
#include "util/generic/gamemode.h"

class CChangeMapWidget : public QWidget
{
	Q_OBJECT

    public:
        struct SMap
        {
            QString m_human_readable;
            QString m_name;
            QString m_screenshot;
            QString m_description;
            GameModes m_gamemodes;

            SMap(QString _human_readable, QString _name, QString _screenshot, QString _description, GameModes _gamemodes)
            {
                m_human_readable = _human_readable;
                m_name = _name;
                m_screenshot = _screenshot;
                m_description = _description;
                m_gamemodes = _gamemodes;
            }
        };

	public:
        CChangeMapWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
        virtual ~CChangeMapWidget();

	Q_SIGNALS:
		void changed_map(QString, QString);

	private Q_SLOTS:
		void Change_Map();
		void Selected_Map(const QString & map);
		void Selected_GameRule(const QString & mode);

    private:
        void BuildGameRulesList();
        void BuildMapsList();

        void PopulateGameRules();
        void PopulateMaps(const EGameMode mode, const bool reset = false);

    private:
        Q_DISABLE_COPY(CChangeMapWidget)

	private:
        Ui::widget_changemap m_ui;

        QList<SMap> m_maps;
        QList<SGameMode> m_modes;
};

#endif /* CHANGEMAP_H */
