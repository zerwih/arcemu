/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "RealmsTabWidget.h"
#include "RealmsLogonWidget.h"

RealmsTabWidget::RealmsTabWidget( QWidget *parent ) :
QWidget( parent )
{
	setupUi( this );
	stackedWidget->addWidget( new RealmsLogonWidget() );
	connect( listWidget, SIGNAL( currentRowChanged( int ) ), this, SLOT( onListRowChanged( int ) ) );
}

RealmsTabWidget::~RealmsTabWidget()
{
}

void RealmsTabWidget::onListRowChanged( int row )
{
	if( row >= stackedWidget->count() )
		return;

	stackedWidget->setCurrentIndex( row );
}


