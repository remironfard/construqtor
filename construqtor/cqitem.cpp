/***************************************************************************
 *   Copyright (C) 2007 by Maciek Gajewski   *
 *   maciej.gajewski0@gmail.com   *
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

// Qt
#include <QGraphicsSceneMouseEvent>

// local
#include "cqsimulation.h"
#include "cqitem.h"

// ========================== constructor ======================
CqItem::CqItem( QGraphicsItem* parent )
	: QGraphicsItem(parent)
{
	init();
}

// ========================== destructor ======================
CqItem::~CqItem()
{
	// none
}

// ==================================== init ==================
void CqItem::init()
{
	setFlag( QGraphicsItem::ItemIsSelectable, true ); // TODO test - maybe better would be to turn it off and use own selection
	_pSimulation = NULL;
	setAcceptedMouseButtons( Qt::NoButton );
}

// =========================== simulation step ===================
void CqItem::simulationStep()
{
	// nothing here
}

// =========================== simulationstarted ===================
void CqItem::simulationStarted()
{
	// nothing here
}

// =========================== simulation stopped ===================
void CqItem::simulationStopped()
{
	// nothing here
}

// ============================== mouse dbl click ==================
void CqItem::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * pEvent )
{
	pEvent->ignore();
}

// ============================== mouse move ==================
void CqItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * pEvent  )
{
	pEvent->ignore();
}

// ============================== mouse press ==================
void CqItem::mousePressEvent ( QGraphicsSceneMouseEvent * pEvent )
{
	pEvent->ignore();
}
// ============================== mouse release ==================
void CqItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent *  )
{
}

// ======================== can item be moved here? =============
bool CqItem::canBeMovedHere( const QPointF& scenePos )
{
	return _pSimulation->canBeMovedHere( this, scenePos );
}

// ============================== set physical pos ===============
/// Proper way to move the item. It alse updates all neccesary data
void CqItem::setPhysicalPos( const QPointF& pos )
{
	setPos( pos );
}

// ============================== set physical rotation ===============
/// Proper way to rtate item. Also rotates physical counterpart
void CqItem::setPhysicalRotation( double radians )
{
	setRotationRadians( radians );
}

// EOF




