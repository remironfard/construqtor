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
#include "b2World.h"

// local
#include "cqsimulation.h"
#include "cqnail.h"
#include "cqphysicalbox.h" 

// constants
static const int SIMULATION_INTERVAL	= 100;	// [ms]
static const double B2D_SPS				= 30.0;	// Box2D simulation steps per second

// ===================== constructor =================
CqSimulation::CqSimulation(QObject* parent): QObject(parent)
{
	// init
	InitWorld();
	connect( &_simulationTimer, SIGNAL(timeout()), SLOT(simulationTimerTimeout() ) );
}

// ======================== destructoir ==================
CqSimulation::~CqSimulation()
{
	// none
}

// ======================== start ==================
void CqSimulation::start()
{
	assurePhysicalObjectsCreated();
	
	// tell everyone simulation will start
	QList< QGraphicsItem* > items = _scene.items();
	
	foreach( QGraphicsItem* pItem, items )
	{
		CqItem* pBody = dynamic_cast<CqItem*>( pItem );
		
		if ( pBody )
		{
			pBody->simulationStarted();
		}
	}
	_simulationTimer.start( SIMULATION_INTERVAL );
	
	emit simulationStarted();
}

// ======================== stop ==================
void CqSimulation::stop()
{
	_simulationTimer.stop();
		
	// tell everyone simulation has stopped
	QList< QGraphicsItem* > items = _scene.items();
	
	foreach( QGraphicsItem* pItem, items )
	{
		CqItem* pBody = dynamic_cast<CqItem*>( pItem );
		
		if ( pBody )
		{
			pBody->simulationStopped();
		}
	}

	emit simulationPaused();
}

// ======================== isRunning ==================
bool CqSimulation::isRunning() const
{
	return _simulationTimer.isActive();
}

// =========================== timer timeout =============
void CqSimulation::simulationTimerTimeout()
{
	// physical world simulation step
	_pPhysicalWorld->Step( 1.0/B2D_SPS, B2D_SPS * SIMULATION_INTERVAL / 1000.0 );
	
	// update all shapes
	QList< QGraphicsItem* > items = _scene.items();
	
	foreach( QGraphicsItem* pItem, items )
	{
		CqItem* pBody = dynamic_cast<CqItem*>( pItem );
		
		if ( pBody && pBody->physicalParent() == NULL ) // only top-level items
		{
			pBody->simulationStep();
		}
	}
}

// =========================== timer timeout =============
void CqSimulation::InitWorld()
{
	// TODO define outside
	
	// world size spec
	b2AABB worldAABB;
	worldAABB.minVertex.Set(-100.0, -100.0);
	worldAABB.maxVertex.Set(100.0, 100.0);
	
	// gravity
	b2Vec2 gravity(0.0, -10.0);
	
	// create world
	_pPhysicalWorld = new CqWorld( worldAABB, gravity, true /* do sleep*/, this );
	
	_scene.setSceneRect( -50, -50, 100, 100 );
	
	// ground object
	// TODO replace with loadable ground
	CqPhysicalBox* pGround = new CqPhysicalBox( NULL, _pPhysicalWorld );
	pGround->setSize( QSizeF( 100.0, 50.0 ) );
	pGround->setPos( 0.0, -25.0 );
	pGround->setMaterial( CqMaterial( 0, 0.9, 0.2 ) );
	pGround->setBrush( Qt::darkGreen );
	addItem( pGround );
	
	stop();
}

// =========================== assure objects created =============
void CqSimulation::assurePhysicalObjectsCreated()
{
	// TODO suboptimal - maintain separate list of bodyless-objects
	QList< QGraphicsItem* > items = _scene.items();
	
	// first - create bodies
	foreach( QGraphicsItem* pItem, items )
	{
		//CqPhysicalBody* pBody = qgraphicsitem_cast<CqPhysicalBody*>( pItem );
		CqPhysicalBody* pBody = dynamic_cast<CqPhysicalBody*>( pItem );
		
		if ( pBody )
		{
			pBody->assureBodyCreated();
		}
	}
	
	// second - create joints
	foreach( QGraphicsItem* pItem, items )
	{
		//CqJoint* pJoint = qgraphicsitem_cast<CqJoint*>( pItem );
		CqJoint* pJoint = dynamic_cast<CqJoint*>( pItem );
		
		if ( pJoint )
		{
			pJoint->assureJointCreated();
		}
	}
}

// =============================== add item =========================
void CqSimulation::addItem( CqItem* pItem )
{
	Q_ASSERT( pItem );
	
	_scene.addItem( pItem );
	pItem->setSimulation( this );
	pItem->setWorld( _pPhysicalWorld );
}

// ============================= can be selected ? ==================
bool CqSimulation::canBeSelected( const CqItem* pItem ) const
{
	Q_ASSERT( pItem );
	
	// TODO 'selectable' are here
	return true;
}

// ============================== can be moved ? =====================
bool CqSimulation::canBeMoved( const CqItem* pItem ) const
{
	// TODO 'movable' area here
	return ! isRunning();
}

// ============================== can be moved here ? =====================
bool CqSimulation::canBeMovedHere( const CqItem* pItem, const QPointF& pos ) const
{
	// TODO editable region here
	return true;
}

// ================================ can be rotated =======================
bool CqSimulation::canBeRotated( const CqItem* pItem ) const
{
	// TODO 'movable' area here
	return ! isRunning();
}
// ============================ can add here ? ======================
// Can item be added here now?
bool CqSimulation::canAddHere( const CqItem* pItem, const QPointF& pos ) const
{
	// TODO 'edit' area here
	return ! isRunning();
}

// EOF
