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
#ifndef CQCOMPOUNDITEM_H
#define CQCOMPOUNDITEM_H

// Local
#include "cqitem.h"

/**
	Compound item - does not paint itself. Adopts shape and bounding box of it's children.
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqCompoundItem : public CqItem
{
	Q_OBJECT
public:
	
	CqCompoundItem( QGraphicsItem *pParent = 0 );
	virtual ~CqCompoundItem();

	void addChild( CqItem* pChild );					///< adds child
	void removeChild( CqItem* pChild );					///< removes child
	
	virtual void setWorld ( CqWorld* pWorld );			///< Sets world
	virtual void setSelected( bool selected );			///< Selects
	virtual void setSimulation( CqSimulation* pSimulation );
					
	/// Return list of CqItem children
	QList< CqItem* > physicalChildren(){ return _children; }
	
	// QGrpahicsItem
	virtual void paint( QPainter*, const QStyleOptionGraphicsItem*, QWidget* );
	virtual QRectF boundingRect() const;
	virtual QPainterPath shape() const;
	
	virtual void updatePhysicalPos();
	
	// followed child
	void setFollowedChild( CqItem* child ){ _followedChild = child; }
	CqItem* followedChild() { return _followedChild; }
	const CqItem* followedChild() const { return _followedChild; }
	
	// signals from simulation
	virtual void updatePosToPhysical();		///< Updates position and rotation to physical
	
	// storing / reading
	virtual void store( CqElement& element ) const;		///< stores item state 
	virtual void load( const CqElement& element );		///< restores item state 

private:

	// methods
	
	void init();			
	
	// data
	
	QList<CqItem*> 	_children;						///< Child items
	CqItem*			_followedChild;					///< Followed child. ITem with follow this child
};

#endif // CQCOMPOUNDITEM_H

// EOF


