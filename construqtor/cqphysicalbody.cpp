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

// box2d
#include "b2Body.h"

// local
#include "cqphysicalbody.h"

// ========================== constructor =================================
CqPhysicalBody::CqPhysicalBody( QGraphicsItem* parent, CqWorld* world )
	: QGraphicsItem( parent )
	, _pWorld( world )
{
	init();
}

CqPhysicalBody::CqPhysicalBody( CqWorld* world )
	: QGraphicsItem( NULL )
	, _pWorld( world )
{
	init();
}

// =========================== destructor ===================================
CqPhysicalBody::~CqPhysicalBody()
{
}

// =========================== init ===================================
void CqPhysicalBody::init()
{
	_pBody = NULL;
	_rotation = 0;
}

// =========================== set world ===================================
/// Sets pohysical world to use. b2d Body is (re) cretaed.
void CqPhysicalBody::setWorld ( CqWorld* pWorld )
{
	// TODO destroy burrent body if world already set
	if ( pWorld && pWorld != _pWorld )
	{
		_pWorld = pWorld;
		Q_ASSERT( _pBody ); // TODO this could be habndked - body rempoved from one world and addetd to another
	}
}

// =========================== set world ===================================
/// destroys current body, and creates new one. Calls createShape() along the way
void CqPhysicalBody::recreateBody()
{
	if ( _pWorld )
	{
		if ( _pBody )
		{
			// destroy current body
			destroyBody( _pWorld );
		}
		// NOTE: body will be created when needed, by simulation with assureBodyCreated()
	}
}

// =========================== create body ===================================
void CqPhysicalBody::createBody( CqWorld* pWorld )
{
	Q_ASSERT( pWorld );
	
	b2BodyDef bodyDef;
	
	// create body shape(s)
	QList< b2ShapeDef* > shapes = createShape();
	
	// add shapes to body
	foreach( b2ShapeDef* pShape, shapes )
	{
		bodyDef.AddShape( pShape );
	}
	
	// attach pointer to self to the box2d body
	// TODO is this needed?
	bodyDef.userData = this;
	
	// create body
	_pBody = pWorld->CreateBody(&bodyDef);
	
	// set body position
	QPointF position = pos();
	b2Vec2 b2pos;
	b2pos.x = position.x();
	b2pos.y = position.y();
	_pBody->SetCenterPosition( b2pos, 0.0 /* TODO: rotation */ );
	
	// get body center just after creation
	// TODO
	/*
	b2Vec2 center = _pBody->GetCenterPosition();
	QPointF centerPoint( center.x, center.y );
	*/
	
	// delete shapes
	foreach ( b2ShapeDef* pShape, shapes )
	{
		delete pShape;
	}
	
}

// =========================== simulaton step ===================================
/// Called by simulation after each simulation step.
/// Updates Grpahics ITem position and rotation to body's
void CqPhysicalBody::simulationStep()
{
	if ( _pBody )
	{
		b2Vec2		b2pos		= _pBody->GetCenterPosition();
		double		b2rotation	= _pBody->GetRotation();
		
		/* TODO no no 
		QTransform		transform;
		transform.translate( b2pos.x, b2pos.y );
		transform.rotateRadians( b2rotation );
		
		setTransform( transform );
		*/
		
		setPos( b2pos.x, b2pos.y );
		setRotationRadians( b2rotation );
		
	}
}

// =========================== assure body created ===================================
/// Called by simulation before it starts.
/// It couses body creation on bodies which does not have body yet
/// This method is called late, so body creation is deffered as much as possible
void CqPhysicalBody::assureBodyCreated()
{
	Q_ASSERT( _pWorld );
	
	if ( ! _pBody )
	{
		createBody(_pWorld);
	}
}

// =========================== destroy body ===================================
void CqPhysicalBody::destroyBody( CqWorld* pWorld )
{
	Q_ASSERT( pWorld );
	Q_ASSERT( _pBody );
	
	pWorld->DestroyBody( _pBody );
	_pBody = NULL;
}

// ======================= set rotation [radians] ==========================
void CqPhysicalBody::setRotationRadians( double radians )
{
	_rotation = radians;
	// TODO update here???
}

// EOF
