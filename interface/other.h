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

#ifndef OTHER_H
#define OTHER_H

#include <QtCore/QObject>
#include <QtGui/QWidget>
#include "ui_widget_other.h"
#include "util/generic/gamemode.h"

class COtherWidget : public QWidget
{
	Q_OBJECT

    public:
        struct SModifier
        {
            QString m_human_readable;
            QString m_name;
            QString m_description;
            GameModes m_gamemodes;

            SModifier(QString _human_readable, QString _name, QString _description, GameModes _gamemodes)
            {
                m_human_readable = _human_readable;
                m_name = _name;
                m_description = _description;
                m_gamemodes = _gamemodes;
            }
        };

	public:
        COtherWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
        virtual ~COtherWidget();

	Q_SIGNALS:
		void command_used(QString);

	private Q_SLOTS:
		void Generic_Command_Clicked();
		void Change_PlayList();
		void Selected_GameRule(const QString & mode);

    private:
        void BuildGameRulesList();
        void BuildModifiersList();

        void PopulateGameRules();
        void PopulateModifiers(const EGameMode mode, const bool reset = false);

    private:
        Q_DISABLE_COPY(COtherWidget)

	private:
        Ui::widget_other m_ui;

        QList<SGameMode> m_modes;
        QList<SModifier> m_modifiers;
};

#endif /* OTHER_H */
