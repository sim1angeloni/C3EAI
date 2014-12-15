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

#ifndef IPTEXTBOX_H
#define IPTEXTBOX_H

#include <QtGui/QSpinBox>
#include "util/generic/ipUtils.h"

class QHBoxLayout;
class CIPSpinBoxWidget;

class CIPTextBoxWidget : public QWidget, public CIPUtils
{
	Q_OBJECT

	public:
        CIPTextBoxWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
        virtual ~CIPTextBoxWidget();

		bool setIP(const QString & ip);
		QString getIP() const;

	private Q_SLOTS:
        void keyPress(QKeyEvent *pEvent);
	    void fieldChanged();

	protected:
        void keyPressEvent(QKeyEvent *event) { Q_UNUSED(event) }
        void keyReleaseEvent(QKeyEvent *event) { Q_UNUSED(event) }
        void wheelEvent(QWheelEvent *event) { Q_UNUSED(event) }

    private:
        void jumpNext(bool highlight);
        void jumpPrevious(bool highlight);
        CIPSpinBoxWidget * getFocusedSpinBox() const;

    private:
        Q_DISABLE_COPY(CIPTextBoxWidget)

	private:
        QHBoxLayout * m_pLayout;
        CIPSpinBoxWidget * m_pPartA;
        CIPSpinBoxWidget * m_pPartB;
        CIPSpinBoxWidget * m_pPartC;
        CIPSpinBoxWidget * m_pPartD;
};

/* ============================================ */

class CIPSpinBoxWidget : public QSpinBox
{
	Q_OBJECT

	public:
        CIPSpinBoxWidget(QWidget *parent = 0);
        virtual ~CIPSpinBoxWidget() {}

	Q_SIGNALS:
		void key_pressed(QKeyEvent *event);
		void key_released(QKeyEvent *event);

	protected:
        void keyPressEvent(QKeyEvent *pEvent);
        void keyReleaseEvent(QKeyEvent *pEvent);
        void wheelEvent(QWheelEvent *event) { Q_UNUSED(event) }

    private:
        Q_DISABLE_COPY(CIPSpinBoxWidget)

	private:
        int m_cursor_position;
};

#endif /* IPTEXTBOX_H */
