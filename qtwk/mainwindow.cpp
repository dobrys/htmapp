/*------------------------------------------------------------------
// Copyright (c) 1997 - 2011
// Robert Umbehant
// htmapp@wheresjames.com
// http://www.wheresjames.com
//
// Redistribution and use in source and binary forms, with or
// without modification, are permitted for commercial and
// non-commercial purposes, provided that the following
// conditions are met:
//
// * Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// * The names of the developers or contributors may not be used to
//   endorse or promote products derived from this software without
//   specific prior written permission.
//
//   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
//   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
//   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
//   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
//   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
//   OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
//   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//----------------------------------------------------------------*/

#include "qtwk.h"
#include "network.h"
#include "mainwindow.h"

CMainWindow::CMainWindow()
{
	m_url = "http://embedded/index.htm";

#if defined( CII_PROJECT_NAME )
	m_name = CII_PROJECT_NAME;
#endif

#if defined( CII_PROJECT_DESC )
	m_desc = CII_PROJECT_DESC;
#endif

	m_width = 0;
	m_height = 0;
}

void CMainWindow::Init()
{
	// Create web view
	m_pView = new QWebView( this );
	if ( m_pView.isNull() )
		throw;

	// Create custom network manager
	m_pNet = new CNetworkMgr( this, m_pView->page()->networkAccessManager() );
	if ( m_pNet.isNull() )
		throw;

	// Set our custom network manager
	m_pView->page()->setNetworkAccessManager( m_pNet );

	// Set window title
	if ( m_desc.length() )
		setWindowTitle( m_desc.c_str() );
	else if ( m_name.length() )
		setWindowTitle( m_name.c_str() );

	// Check for fixed size window
	if ( 0 < m_width && 0 < m_height )
		setFixedSize( m_width, m_height );

	// No scrollbars
#if defined( CII_NOSCROLL )
	m_pView->page()->mainFrame()->setScrollBarPolicy( Qt::Vertical,Qt::ScrollBarAlwaysOff );
	m_pView->page()->mainFrame()->setScrollBarPolicy( Qt::Horizontal,Qt::ScrollBarAlwaysOff );
#endif

	// No context menu
#if defined( CII_NOCONTEXT )
	m_pView->setContextMenuPolicy( Qt::PreventContextMenu );
#endif
	
	// Enable cross scripting
#if defined( CII_ALLOW_CROSS_SCRIPTING )
	m_pView->settings()->setAttribute( QWebSettings::XSSAuditingEnabled, 0 );
	m_pView->settings()->setAttribute( QWebSettings::LocalContentCanAccessRemoteUrls, 1 );
#endif

	// Make the keyboard easier to use
//	m_pView->settings()->setAttribute( QWebSettings::SpatialNavigationEnabled, 1 );

	// Start the web view
	setCentralWidget( m_pView );

	// Load the home page
	m_pView->load( QUrl( m_url.c_str() ) );
}

