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

#ifndef ADVANCEDLINEEDIT_H
#define ADVANCEDLINEEDIT_H

#include <QtCore/QObject>
#include <QtGui/QLineEdit>

#define BIT(a) 1 << a

class CAdvancedLineEditWidget : public QLineEdit
{
	Q_OBJECT

	Q_PROPERTY(int minLength READ minLength WRITE setMinLength)
	Q_PROPERTY(AllowedCharacters allowedCharacters READ allowedCharacters WRITE setAllowedCharacters)
	Q_PROPERTY(bool disallowedCharactersLocked READ disallowedCharactersLocked WRITE setLockDisallowedCharacters)
	Q_PROPERTY(QRgb errorColor READ errorColor WRITE setErrorColor)
	Q_PROPERTY(bool errorHighlight READ errorHighlight WRITE setErrorHighlight)
	Q_PROPERTY(bool valid READ valid)

	public:
		enum EAllowedCharacters
		{
			eAC_ASCIIalphabetic		= BIT(0),
			eAC_ASCIIdigit			= BIT(1),
			eAC_Hex					= BIT(2),
			eAC_Binary				= BIT(3),
			eAC_Space				= BIT(4),
			eAC_Underscore			= BIT(5),
			eAC_Minus				= BIT(6),
			eAC_ASCII				= eAC_ASCIIalphabetic | eAC_ASCIIdigit,
			eAC_Any					= BIT(7)
		};
		Q_DECLARE_FLAGS(AllowedCharacters, EAllowedCharacters)

        CAdvancedLineEditWidget(QWidget* parent=0);
        CAdvancedLineEditWidget(const QString &, QWidget* parent=0);
        virtual ~CAdvancedLineEditWidget();

        int minLength () const { return m_minLength; }
		void setMinLength ( int );

	    void setMaxLength ( int );

        AllowedCharacters allowedCharacters () const { return m_allowedCharacters; }
		void setAllowedCharacters ( AllowedCharacters );

        bool disallowedCharactersLocked() const { return m_lockDisallowedChars; }
		void setLockDisallowedCharacters( bool );

        QRgb errorColor() const { return m_errorColor; }
		void setErrorColor( QRgb );

        bool errorHighlight() const { return m_highlightErrors; }
		void setErrorHighlight( bool );

        bool valid() const { return m_valid; }

	public Q_SLOTS:
		void setText(const QString &);

	Q_SIGNALS:
		void validityChanged(bool);
		void minLengthReached();
		void maxLengthReached();

	protected:
        void keyPressEvent(QKeyEvent *pEvent);

    private:
        void update();
        void refreshColor();
        void refreshColor(bool _valid);
        bool testCharacter(const QChar & character) const;
        void testBorders();

    private:
        Q_DISABLE_COPY(CAdvancedLineEditWidget)

	private:
        int m_minLength;
        AllowedCharacters m_allowedCharacters;
        bool m_lockDisallowedChars;
        bool m_highlightErrors;
        QRgb m_errorColor;
        bool m_valid;

        int m_currentLength;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CAdvancedLineEditWidget::AllowedCharacters)

#endif /* ADVANCEDLINEEDIT_H */
