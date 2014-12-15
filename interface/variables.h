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

#ifndef VARIABLES_H
#define VARIABLES_H

#include <QtCore/QObject>
#include <QtGui/QWidget>
#include "ui_widget_variables.h"

class CVariablesWidget : public QWidget
{
	Q_OBJECT

	public:
        CVariablesWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
        virtual ~CVariablesWidget();

		QList<QString> get_list();

	public Q_SLOTS:
		void set_variable(QString);

	Q_SIGNALS:
		void variable_changed(QString, QString);

	private Q_SLOTS:
		void Value_Changed(const QString value);

    private:
        Q_DISABLE_COPY(CVariablesWidget)

	private:
        Ui::widget_variables m_ui;
};

#endif /* VARIABLES_H */
