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
#include <QPainter>

// box2d
#include "b2Body.h"

// local
#include "cqphysicalbody.h"
#include "cqitemtypes.h"
#include "cqjoint.h"
#include "cqsimulation.h"

// ========================== constructor =================================
CqPhysicalBody::CqPhysicalBody( QGraphicsItem* parent, CqWorld* world )
	: CqItem( parent )
{
	init();
	setWorld( world );
}

// ========================== constructor =================================
CqPhysicalBody::CqPhysicalBody( CqWorld* world )
	: CqItem( NULL )
{
	init();
	setWorld( world );
}

// =========================== destructor ===================================
CqPhysicalBody::~CqPhysicalBody()
{
}

// =========================== init ===================================
void CqPhysicalBody::init()
{
	_pBody = NULL;
	
	// make rotatable
	setEditorFlags( editorFlags() | Rotatable );
}

// =========================== set world ===================================
/// destroys current body, and creates new one. Calls createShape() along the way
void CqPhysicalBody::recreateBody()
{
	if ( world() )
	{
		if ( _pBody )
		{
			// destroy current body
			destroyBody( world() );
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
	
	// damping TODO: experimental
	bodyDef.linearDamping	= 0.002;
	bodyDef.angularDamping	= 0.002;
	
	// create body
	_pBody = pWorld->CreateBody(&bodyDef);
	
	// get center pos
	b2Vec2 cog = _pBody->GetCenterPosition();
	_cog = QPointF( cog.x, cog.y );
	qDebug("center: %f, %f", cog.x, cog.y ); // TODO remove
	
	// set body position
	updatePhysicalPos();
	
	// delete shapes
	foreach ( b2ShapeDef* pShape, shapes )
	{
		delete pShape;
	}
	
}

// =========================== update to physical ===================================
void CqPhysicalBody::updatePosToPhysical()
{
	if ( _pBody )
	{
		b2Vec2		b2pos		= _pBody->GetCenterPosition();
		double		b2rotation	= _pBody->GetRotation();
		
		setWorldPos( QPointF( b2pos.x, b2pos.y ) - _cog ); // correct pos by COG
		setWorldRotation( b2rotation );
	}
}

// =========================== assure body created ===================================
/// Called by simulation before it starts.
/// It couses body creation on bodies which does not have body yet
/// This method is called late, so body creation is deffered as much as possible
void CqPhysicalBody::assureBodyCreated()
{
	Q_ASSERT( world() );
	
	if ( ! _pBody )
	{
		createBody(world());
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

// =========================== type  ===================
int CqPhysicalBody::type() const
{
	return CQ_BODY;
}

// =================================== breaks all atached joints ===
void CqPhysicalBody::breakAllJoints()
{
	// get copy of joint list, couse they will be removed from 'official' list
	// during the process
	QList< CqJoint* > joints = _joints;
	
	foreach( CqJoint* pJoint, joints )
	{
		pJoint->breakJoint();
	}
}

// ============================== update physical pos ===============
void CqPhysicalBody::updatePhysicalPos()
{
	CqItem::updatePhysicalPos();
	if ( _pBody )
	{
		QPointF pp	= worldPos() + _cog; // correct physical pos by COG shift
		double r	= worldRotation();
		
		_pBody->SetCenterPosition( b2Vec2( pp.x(), pp.y() ), r );
		update();
	}
}

// =============================== add joint ======================
void CqPhysicalBody::addJoint( CqJoint* pJoint )
{
	// duplicate check
	if ( _joints.contains( pJoint ) )
	{
		qWarning("joint added once again to same body");
	}
	else
	{
		_joints.append( pJoint );
	}
}

// =============================== remove joint ======================
void CqPhysicalBody::removeJoint( CqJoint* pJoint )
{
	// existence check
	if ( _joints.contains( pJoint ) )
	{
		_joints.removeAll( pJoint );
	}
	else
	{
		qWarning("joint remove from body, to each it doesn;t belogns");
	}
}

// ========================= debug: draw collision ==================
void CqPhysicalBody::debugDrawCollision( QPainter* pPainter )
{
	// no body - no bodpainting :)
	if ( ! _pBody )
	{
		return;
	}
	
	Q_ASSERT( pPainter );
	
	pPainter->save();
	pPainter->setPen( QPen( Qt::red, 0 ) );
	pPainter->setBrush( QBrush() );
	
	for ( b2Shape* s = _pBody->GetShapeList(); s; s = s->GetNext() )
	{
		// paint polygon
		if ( s->GetType() == e_polyShape )
		{
			b2PolyShape* pPolyShape = (b2PolyShape*) s;
			QPolygonF shape( pPolyShape->m_vertexCount );
			//b2Mat22 rotation = pPolyShape->GetRotationMatrix();
			
			for ( int i =0; i < pPolyShape->m_vertexCount; i++ )
			{
				b2Vec2& vertex = pPolyShape->m_vertices[i];
				shape[i] =QPointF( pPolyShape->m_vertices[i].x, vertex.y );
			}
			
			// translate to body-local centroid
			shape.translate
				( pPolyShape->m_localCentroid.x + _cog.x()
				, pPolyShape->m_localCentroid.y + _cog.y()
				);
			
			pPainter->drawPolygon( shape );
		}
		// TODO other shape types
	}
	
	pPainter->restore();
}

// ========================= set rotation radians ===========
/// Sets transformation, taking COG into account
void CqPhysicalBody::setRotationRadians( double radians )
{
	if ( fabs( _rotation - radians ) > 0.01 ) // some fuse to avoid frequrnt updates
	{
		prepareGeometryChange();
		_rotation = radians;
		QTransform t;
		t.rotateRadians( _rotation );
		setTransform( t );
		// TODO cog here
	}
}

// EOF
