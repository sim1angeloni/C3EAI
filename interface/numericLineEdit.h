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

#ifndef NUMERICLINEEDIT_H
#define NUMERICLINEEDIT_H

#include <QtCore/QObject>
#include "advancedLineEdit.h"
#include "util/generic/numericValue.h"

class CNumericLineEditWidget : public CAdvancedLineEditWidget
{
	Q_OBJECT

	public:
        CNumericLineEditWidget(QWidget* parent=0);
        CNumericLineEditWidget(int, QWidget* parent=0);
        virtual ~CNumericLineEditWidget();

		void setAllowedCharacters ( AllowedCharacters ) {}
		void setLockDisallowedCharacters( bool ) {}

		int maximum () const;
		void setMaximum ( int );

		int minimum () const;
		void setMinimum ( int );

		void setRange ( int, int );

		int singleStep () const;
		void setSingleStep ( int );

		int value () const;

	public Q_SLOTS:
		void setText(const QString &);
		void setValue(int);

	Q_SIGNALS:
		void valueChanged(int);
		void valueChanged(const QString &);

	protected:
        void wheelEvent(QWheelEvent * pEvent);
        void keyPressEvent(QKeyEvent * pEvent);

    private:
        void refreshTextFromValue();
        void refreshValueFromText();
        void init();

    private:
        Q_DISABLE_COPY(CNumericLineEditWidget)

	private:
        CNumericValue m_value;
};

#endif /* NUMERICLINEEDIT_H */
