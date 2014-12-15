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

#ifndef CUSTOMSPINBOX_H
#define CUSTOMSPINBOX_H

#include <QtGui/QDoubleSpinBox>

class CCustomSpinboxWidget : public QDoubleSpinBox
{
	Q_OBJECT

	public:
        CCustomSpinboxWidget(QWidget *parent = 0);
        virtual ~CCustomSpinboxWidget();

        bool eventFilter(QObject *pObject, QEvent *pEvent);

	Q_SIGNALS:
		void sigValueChanged(double);
		void sigValueChanged(const QString);

	private Q_SLOTS:
		void editing_finished();

    private:
        Q_DISABLE_COPY(CCustomSpinboxWidget)

	private:
        double m_previous_value;
};

#endif /* CUSTOMSPINBOX_H */
