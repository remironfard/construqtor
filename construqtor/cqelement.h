/***************************************************************************
 *   Copyright (C) 2007 by Maciek Gajewski                                 *
 *   maciej.gajewski0@gmail.com                                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef CQELEMENT_H
#define CQELEMENT_H

// Qt
#include <QDomElement>
#include <QObject>
#include <QPointF>
#include <QSizeF>
#include <QPolygonF>
#include <QRectF>

// local
class CqItem;

/**
	Data element, stored withing CqDocument. Element holds it's child elements,
	idetinfied by string tag. Some of them can be immediately converted to comont type,
	thus the 'readXXXX' methods.
	This implementation is wrapper around Qt's DOM classes.
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqElement : public QObject
{
	Q_OBJECT
public:

	static const char* TAG_ITEM;		///< Default tag used to store items

	CqElement ( QObject *parent = 0 );
	CqElement ( const CqElement& source );
	CqElement ( const QDomElement& element, QObject *parent = 0 );
	virtual ~CqElement();

	CqElement createElement(); ///< Creates element which can be used as \b this's sub-elebent

	bool hasElement( const QString& tag ) const; ///< Checks if has element with given tag

	// storing data
	void appendString( const QString& tag, const QString& value );
	void appendDouble( const QString& tag, double value );
	void appendInt( const QString& tag, int value );
	void appendPointF( const QString& tag, const QPointF& value );
	void appendSizeF( const QString& tag, const QSizeF& value );
	void appendRectF( const QString& tag, const QRectF& value );
	void appendPolygonF( const QString& tag, const QPolygonF& value );
	
	void appendItemPointer( const QString& tag, const CqItem* pointer );
	void appendItem( const CqItem* pItem, const QString& tag = TAG_ITEM );
	
	void appendElement( const QString& tag, const CqElement& element );
	
	// reading data
	QString		readString( const QString& tag ) const;
	double		readDouble( const QString& tag ) const;
	int			readInt( const QString& tag ) const;
	QPointF		readPointF( const QString& tag ) const;
	QSizeF		readSizeF( const QString& tag ) const;
	QRectF		readRectF( const QString& tag ) const;
	QPolygonF	readPolygonF( const QString& tag ) const;
	CqItem*		readItemPointer( const QString& tag ) const;
	CqItem*		readItem( const QString& tag = TAG_ITEM ) const;
	
	CqElement	readElement( const QString& tag ) const;
	
	// underlying DOM access
	const QDomElement& domElement() const { return _element; }
	QDomElement& domElement() { return _element; }
	
private:
	
	QDomElement	_element;
	
};

#endif // CQELEMENT_H

// EOF


