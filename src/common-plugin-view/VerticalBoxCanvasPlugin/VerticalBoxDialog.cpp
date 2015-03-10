/**
 * @file VerticalBoxDialog.cpp
 * @brief implements of VerticalBoxCanvas class
 *
 * @author A.Ozaki(SSD)
 * @date 2013.05.23
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#include	"stdafx.h"
#include	"VerticalBoxDialog.h"
#include	"VerticalBoxSettingsViewManager.h"
#include	"VerticalBoxSpectrumCanvas.h"
#include	"VerticalBoxChromatogramCanvas.h"

#include <wx/colordlg.h>

using namespace	kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace	kome::window;

#define	BORDER_SIZE						( 3 )
#define	GRID_GAP						( 3 )

#define	ID_LIST							( 1 )
#define	ID_UPDATE_BUTTON				( 2 )
#define	ID_REMOVE_BUTTON				( 3 )
#define	ID_BMP_DELETE_BUTTON			( 4 )
#define	ID_BMP_UP_BUTTON				( 5 )
#define	ID_BMP_DOWN_BUTTON				( 6 )

#define	LISTBOX_HEIGHT					( 200 )
#define	SPACER_SIZE						( 10 )

// for context menu
#define	ID_POPUP_MENU_DELETE			( 60003 )
#define	STR_POPUP_MENU_DELETE			"Delete"


BEGIN_EVENT_TABLE( VerticalBoxDialog, kome::window::StandardPanel )
	EVT_LISTBOX			( ID_LIST, VerticalBoxDialog::onSelect )
	EVT_BUTTON			( ID_BMP_DELETE_BUTTON, VerticalBoxDialog::onRemoveButton )
	EVT_BUTTON			( ID_BMP_UP_BUTTON, VerticalBoxDialog::onUpButton )
	EVT_BUTTON			( ID_BMP_DOWN_BUTTON, VerticalBoxDialog::onDownButton )
	EVT_MENU			( ID_POPUP_MENU_DELETE, VerticalBoxDialog::onDelete )
	EVT_UPDATE_UI		( ID_POPUP_MENU_DELETE, VerticalBoxDialog::onUpdateUIPopupMenu )
	EVT_CONTEXT_MENU	( VerticalBoxDialog::onContextMenu )
END_EVENT_TABLE()


// constructor
VerticalBoxDialog::VerticalBoxDialog( wxWindow* parent )
		: StandardPanel( parent )
{
	// クラスない変数の初期化
	//
	m_pCanvas = (CanvasBase *)NULL;
	m_pList = (wxListBox *)NULL;
	m_bChromFlg = false;

	m_pBitmapButtonDelete = (wxBitmapButton *)NULL;
	m_pBitmapButtonUp = (wxBitmapButton *)NULL;
	m_pBitmapButtonDown = (wxBitmapButton *)NULL;

	createControls( );

	return;
}

// destructor
VerticalBoxDialog::~VerticalBoxDialog( )
{
	return;
}

// set canvas
void	VerticalBoxDialog::setCanvas( kome::window::CanvasBase* pCanvas )
{
	m_pCanvas = pCanvas;
	if  ( (CanvasBase *)NULL != pCanvas )
	{
		m_bChromFlg = ( dynamic_cast< VerticalBoxChromatogramCanvas* >( pCanvas ) != NULL );
	}
	else
	{
		// NULLの場合は、初期値に設定
		m_bChromFlg = false;
	}
	update( );

	return;
}

// update dialog
void	VerticalBoxDialog::update( void )
{
	// check listbox control
	if  ( (wxListBox *)NULL == m_pList )
	{
		// not created list control
		return;
	}

	m_pList->Freeze( );

	// clear list
	while ( m_pList->GetCount( ) > 0 )
	{
		m_pList->Delete( 0 );
	}
	m_pList->DeselectAll( );

	// update
	unsigned int	i;

	if  ( true == m_bChromFlg )
	{	// chromatograms
		// canvas
		VerticalBoxChromatogramCanvas*	pChromCanvas
			= dynamic_cast< VerticalBoxChromatogramCanvas* >( m_pCanvas );

		if  ( (VerticalBoxChromatogramCanvas *)NULL != pChromCanvas )
		{
			//
			// getNumberOfChromatogramsでcanvasないで保有しているobject数が戻ってきます
			// canvasないのobjectを設定順にしか並んでいないため、
			// そのまま表示するとリスト順とは合わなくなってしまいます
			// そのため、canvasの表示位置に該当するobject情報を取得するようにして、
			// リスト順と合わせます
			//
			for ( i = 0 ; i < pChromCanvas->getNumberOfChromatograms( ) ; i++ )
			{
				// 表示位置に該当するobjectの格納位置を調べます
				int	nIndex = pChromCanvas->GetVerticalBoxChromatogramNum( i );

				// chromatogram
				kome::objects::Chromatogram*	pChrom = pChromCanvas->getChromatogram( nIndex );

				if  ( (kome::objects::Chromatogram *)NULL != pChrom )
				{
					std::string name = pChrom->getTitle( );
					kome::objects::DataGroupNode*	pGroup = pChrom->getGroup( );
					while ( (kome::objects::DataGroupNode *)NULL != pGroup )
					{
						name.append( FMT( " - %s", pGroup->getName( ) ) );
						pGroup = pGroup->getParentGroup( );
					}
					bool	bSet = true;
					if  ( bSet == true )
					{
						m_pList->Append( name.c_str(), pChrom );
					}
				}
			}
			// 選択状態の設定
			if  ( 0 < pChromCanvas->getNumberOfChromatograms( ) )
			{
				int		nIndex = pChromCanvas->getIndex( );
				if  ( 0 <= nIndex &&
					( 0 < m_pList->GetCount( ) && nIndex < (int)m_pList->GetCount( ) ) )
				{
					// 処理できる範囲内の場合のみ、選択設定します
					m_pList->SetSelection( nIndex );
				}
			}
		}
	}
	else
	{	// spectra
		// canvas
		VerticalBoxSpectrumCanvas*	pSpecCanvas
			= dynamic_cast< VerticalBoxSpectrumCanvas* >( m_pCanvas );
		
		if  ( (VerticalBoxSpectrumCanvas*)NULL != pSpecCanvas )
		{
			//
			// getNumberOfSpectraでcanvasないで保有しているobject数が戻ってきます
			// canvasないのobjectを設定順にしか並んでいないため、
			// そのまま表示するとリスト順とは合わなくなってしまいます
			// そのため、canvasの表示位置に該当するobject情報を取得するようにして、
			// リスト順と合わせます
			//
			for ( i = 0 ; i < pSpecCanvas->getNumberOfSpectra( ) ; i++ )
			{
				// 表示位置に該当するobjectの格納位置を調べます
				int	nIndex = pSpecCanvas->GetVerticalBoxSpectrumNum( i );

				// spectrum
				kome::objects::Spectrum*	pSpec = pSpecCanvas->getSpectrum( nIndex );

				if  ( (kome::objects::Spectrum *)NULL != pSpec )
				{
					std::string	name = pSpec->getTitle( );
					kome::objects::DataGroupNode*	pGroup = pSpec->getGroup( );
					while ( pGroup != NULL )
					{
						name.append( FMT( " - %s", pGroup->getName( ) ) );
						pGroup = pGroup->getParentGroup( );
					}
				
					m_pList->Append( name.c_str(), pSpec );

				}
			}

			// 選択状態の設定
			if  ( 0 < pSpecCanvas->getNumberOfSpectra( ) )
			{
				int		nIndex = pSpecCanvas->getIndex( );
				if  ( 0 <= nIndex &&
					( 0 < m_pList->GetCount( ) && nIndex < (int)m_pList->GetCount( ) ) )
				{
					// 処理できる範囲内の場合のみ、選択設定します
					m_pList->SetSelection( nIndex );
				}
			}
		}
	}

	m_pList->Thaw( );
	m_pList->Update( );

	setButtonStatus( );

	return;
}

// select spectra or chromatograms
void	VerticalBoxDialog::select( void )
{
	unsigned int	i;

	// check values
	if  ( (wxListBox *)NULL == m_pList )
	{
		return;
	}

	// abstract selected items
	std::vector< int >	selectedItems;
	for  ( i = 0 ; i < m_pList->GetCount( ) ; i++ )
	{
		if  ( m_pList->IsSelected( i ) )
		{
			selectedItems.push_back( (int)i );
		}
	}

	if  ( true == m_bChromFlg )
	{	// chromatogram
		// canvas
		VerticalBoxChromatogramCanvas*	pChromCanvas
			= dynamic_cast< VerticalBoxChromatogramCanvas* >( m_pCanvas );

		if  ( (VerticalBoxChromatogramCanvas *)NULL != pChromCanvas )
		{
			// unselect all
			if  ( 0 < selectedItems.size( ) &&
				(VerticalBoxChromatogramCanvas *)NULL != pChromCanvas )
			{
				pChromCanvas->clearSelectedChroms( );
			}

			// chromatograms
			for ( i = 0 ; i < selectedItems.size( ) ; i++ )
			{
				// 選択されている行番号から、canvasに登録されている位置を
				// 調べます
				//
				int	item = pChromCanvas->GetVerticalBoxChromatogramNum( selectedItems[ i ] );
				kome::objects::Chromatogram*	pChrom = pChromCanvas->getChromatogram( item );

				// add selected
				if  ( (kome::objects::Chromatogram *)NULL != pChrom )
				{
					if  ( (kome::objects::Chromatogram *)NULL != pChrom->getOrgChromatogram( ) )
					{
						pChromCanvas->addSelectedChrom(  pChrom->getOrgChromatogram( ) );
						pChromCanvas->setIndex( selectedItems[ i ] );
						pChromCanvas->setSelectedChrom(  pChrom->getOrgChromatogram( ) );	// 選択枠を表示するため
					}
				}
			}
		}
	}
	else
	{	// spectra
		// canvas
		VerticalBoxSpectrumCanvas*	pSpectraCanvas
			= dynamic_cast< VerticalBoxSpectrumCanvas* >( m_pCanvas );

		if  ( (VerticalBoxSpectrumCanvas *)NULL != pSpectraCanvas )
		{
			// unselect all
			if  ( 0 < selectedItems.size( ) &&
				(VerticalBoxSpectrumCanvas *)NULL != pSpectraCanvas )
			{
				pSpectraCanvas->clearSelectedSpectrum( );
			}

			// spectra
			for ( i = 0 ; i < selectedItems.size( ) ; i++ )
			{
				// 選択されている行番号から、canvasに登録されている位置を
				// 調べます
				//
				int	item = pSpectraCanvas->GetVerticalBoxSpectrumNum( selectedItems[ i ] );

				kome::objects::Spectrum*	pSpec = pSpectraCanvas->getSpectrum( item );

				// add selected
				if  ( (kome::objects::Spectrum *)NULL != pSpec )
				{
					if  ( (kome::objects::Spectrum *)NULL != pSpec->getOrgSpectrum( ) )
					{
						pSpectraCanvas->addSelectedSpectrum( pSpec->getOrgSpectrum( ) );
						pSpectraCanvas->setIndex( selectedItems[ i ] );
						pSpectraCanvas->setSelectedSpec( pSpec->getOrgSpectrum( ) );	// 選択枠を表示するため
					}
				}
			}
		}
	}

	setButtonStatus( );

	// refresh
	kome::window::WindowTool::refresh( );

	return;
}

// create edit sizer
wxSizer*	VerticalBoxDialog::createEditSizer( void )
{
	wxBoxSizer*	pSamplesSizer = new wxBoxSizer( wxVERTICAL );

	// Add buttons
	wxBoxSizer*	pButtonSizer = new wxBoxSizer( wxHORIZONTAL );

	IconManager&	iconMgr = IconManager::getInstance( );

	if  ( (wxBoxSizer *)NULL == pSamplesSizer || (wxBoxSizer *)NULL == pButtonSizer )
	{
		return	(wxSizer *)NULL;
	}

	// delete button
	m_pBitmapButtonDelete = new wxBitmapButton( this, ID_BMP_DELETE_BUTTON, *iconMgr.getIcon( "vertical_box_del" ) );
	if  ( (wxBitmapButton *)NULL != m_pBitmapButtonDelete )
	{
		m_pBitmapButtonDelete->SetToolTip( "Remove trace" );
		m_pBitmapButtonDelete->SetBitmapDisabled( *iconMgr.getIcon( "vertical_box_dis_del" ) );
		m_pBitmapButtonDelete->Enable( false );
		pButtonSizer->Add( m_pBitmapButtonDelete, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );
	}

	// up button
	m_pBitmapButtonUp = new wxBitmapButton( this, ID_BMP_UP_BUTTON, *iconMgr.getIcon( "vertical_box_up" ) );
	if  ( (wxBitmapButton *)NULL != m_pBitmapButtonUp )
	{
		m_pBitmapButtonUp->SetToolTip( "Move trace up" );
		m_pBitmapButtonUp->SetBitmapDisabled( *iconMgr.getIcon( "vertical_box_dis_up" ) );
		m_pBitmapButtonUp->Enable( false );
		pButtonSizer->Add( m_pBitmapButtonUp, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );
	}

	// down button
	m_pBitmapButtonDown = new wxBitmapButton( this, ID_BMP_DOWN_BUTTON, *iconMgr.getIcon( "vertical_box_down" ) );
	if  ( (wxBitmapButton *)NULL != m_pBitmapButtonDown )
	{
		m_pBitmapButtonDown->SetToolTip( "Move trace down" );
		m_pBitmapButtonDown->SetBitmapDisabled( *iconMgr.getIcon( "vertical_box_dis_down" ) );
		m_pBitmapButtonDown->Enable( false ); 
		pButtonSizer->Add( m_pBitmapButtonDown, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );
	}

	pSamplesSizer->Add( pButtonSizer, 0, (wxALL & ~wxLEFT) | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	return	pSamplesSizer;
}


// creates main sizer
wxSizer*	VerticalBoxDialog::createMainSizer( void )
{
	// main sizer
	wxFlexGridSizer*	pSizer = new wxFlexGridSizer( 2, 1, GRID_GAP, GRID_GAP );

	if  ( (wxFlexGridSizer *)NULL == pSizer )
	{
		return	(wxSizer *)NULL;
	}

	// Sizerないのオブジェクトのサイズ変更を許可
	pSizer->AddGrowableRow( 0 );
	pSizer->AddGrowableCol( 0 );

	// list box
	m_pList = new wxListBox( this, ID_LIST, wxDefaultPosition, wxDefaultSize );
	if  ( (wxListBox *)NULL == m_pList )
	{
		delete	pSizer;
		return	(wxSizer *)NULL;
	}
	pSizer->Add( m_pList, 0, wxALL | wxGROW, BORDER_SIZE );

	// edit sizer
	wxSizer*	pEditSizer = createEditSizer( );
	if  ( (wxSizer *)NULL == pEditSizer )
	{
		delete	pSizer;
		return	(wxSizer *)NULL;
	}
	pSizer->Add( pEditSizer, 1, (wxALL & ~wxLEFT) | wxGROW | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	update( );

	return	pSizer;
}

// on select
void	VerticalBoxDialog::onSelect( wxCommandEvent& evt )
{
	select( );

	return;
}

// on remove
void	VerticalBoxDialog::onRemoveButton( wxCommandEvent& evt )
{
	onRemoveButton( );

	return;
}

// on remove
void	VerticalBoxDialog::onRemoveButton( void )
{
	unsigned int	i = 0;

	// extruct selected items
	std::vector< int >	selectedItems;
	for ( i = 0 ; i < m_pList->GetCount( ) ; i++ )
	{
		if  ( m_pList->IsSelected( i ) )
		{
			selectedItems.push_back( (int)i );
		}
	}

	// remove
	for  ( int j = (int)selectedItems.size( ) - 1 ; j >= 0 ; j-- )
	{
		int	item = selectedItems[ j ];

		if  ( true == m_bChromFlg )
		{	// chromatogram
			kome::objects::Chromatogram*	pChrom = (kome::objects::Chromatogram*)m_pList->GetClientData( item );
			VerticalBoxChromatogramCanvas*	pChromCanvas = dynamic_cast< VerticalBoxChromatogramCanvas* >( m_pCanvas );
			if  ( (VerticalBoxChromatogramCanvas *)NULL != pChromCanvas )
			{
				pChromCanvas->setIndex( -1 );
				pChromCanvas->setIndexDisp( -1 );
				pChromCanvas->removeChromatogram( pChrom );
			}
		}
		else
		{	// spectrum
			kome::objects::Spectrum*	pSpec = (kome::objects::Spectrum*)m_pList->GetClientData( item );
			VerticalBoxSpectrumCanvas*	pSpectraCanvas = dynamic_cast< VerticalBoxSpectrumCanvas* >( m_pCanvas );
			if  ( (VerticalBoxSpectrumCanvas *)NULL != pSpectraCanvas )
			{
				pSpectraCanvas->setIndex( -1 );
				pSpectraCanvas->setIndexDisp( -1 );
				pSpectraCanvas->removeSpectrum( pSpec );
			}
		}
	}

	// deselect
	m_pList->DeselectAll( );

	select( );

	return;
}

// the up(BMP) button is clicked
void	VerticalBoxDialog::onUpButton( wxCommandEvent& evt )
{
	if  ( (wxListBox *)NULL == m_pList )
	{
		return;
	}

	int	itemnum = m_pList->GetSelection( );

	if  ( 0 != itemnum )
	{
		void	*clientdata = (void *)NULL;
		wxString	strTemp = m_pList->GetString( itemnum );
		clientdata = m_pList->GetClientData( itemnum );
		m_pList->Freeze( );
		m_pList->InsertItems( 1, &strTemp, itemnum - 1 );
		m_pList->Delete( itemnum + 1 );
		m_pList->SetClientData( itemnum - 1, clientdata );
		m_pList->SetSelection( itemnum -1 );
		m_pList->Thaw( );

		// reconstruction of specCanvas
		if  ( true == m_bChromFlg )
		{	// chromatogram
			// in function update() m_list is deleted and reconstructed base on chromCanvas
			VerticalBoxChromatogramCanvas*	pChromCanvas
				= dynamic_cast< VerticalBoxChromatogramCanvas* >( m_pCanvas );
			if  ( (VerticalBoxChromatogramCanvas *)NULL != pChromCanvas )
			{
				pChromCanvas->exchange( itemnum, itemnum - 1 );
			}
		}
		else
		{	// spectrum
			// in function update() m_list is deleted and reconstructed base on specCanvas
			VerticalBoxSpectrumCanvas*	pSpecCanvas
				= dynamic_cast< VerticalBoxSpectrumCanvas* >( m_pCanvas );
			if  ( (VerticalBoxSpectrumCanvas *)NULL != pSpecCanvas )
			{
				pSpecCanvas->exchange( itemnum, itemnum - 1 );
			}
		}
	}

	select( );

	return;
}

// the down(BMP) button is clicked
void	VerticalBoxDialog::onDownButton( wxCommandEvent& evt )
{
	// check the argument
	if  ( (wxListBox *)NULL == m_pList )
	{
		return;
	}

	int	itemnum = m_pList->GetSelection( );

	if  ( itemnum != ( m_pList->GetCount( ) - 1 ) )
	{
		// item in listbox maintenance
		//
		void	*clientdata = (void *)NULL;
		wxString	strTemp = m_pList->GetString( itemnum );
		clientdata = m_pList->GetClientData( itemnum );
		m_pList->Freeze( );
		m_pList->InsertItems( 1, &strTemp, itemnum + 2 );
		m_pList->Delete( itemnum );
		m_pList->SetClientData( itemnum + 1, clientdata );
		m_pList->SetSelection( itemnum + 1 );
		m_pList->Thaw( );

		// reconstruction of specCanvas
		if  ( true == m_bChromFlg )
		{	// chromatogram
			// in function update() m_list is deleted and reconstructed base on chromCanvas
			VerticalBoxChromatogramCanvas*	pChromCanvas
				= dynamic_cast< VerticalBoxChromatogramCanvas* >( m_pCanvas );
			if  ( (VerticalBoxChromatogramCanvas *)NULL != pChromCanvas )
			{
				pChromCanvas->exchange( itemnum, itemnum + 1 );
			}
		}
		else
		{	// spectrum
			// in function update() m_list is deleted and reconstructed base on specCanvas
			VerticalBoxSpectrumCanvas*	pSpecCanvas
				= dynamic_cast< VerticalBoxSpectrumCanvas* >( m_pCanvas );
			if  ( (VerticalBoxSpectrumCanvas *)NULL != pSpecCanvas )
			{
				// swap
				pSpecCanvas->exchange( itemnum, itemnum + 1 );
			}
		}
	}

	select( );

	return;
}

// キャンバスで選択されたスペクトラムをリストに反映
void	VerticalBoxDialog::SelectSpecOfCanvas( kome::objects::Spectrum* pSpec )
{
	if  ( (wxListBox *)NULL != m_pList )
	{
		m_pList->Freeze( );

		// unselect all
		m_pList->DeselectAll( );
	
		if  ( (kome::objects::Spectrum *)NULL != pSpec )
		{
			for ( unsigned int i = 0 ; i < m_pList->GetCount( ) ; i++ )
			{
				kome::operation::TransformedSpectrum*	pListSpec =
					(kome::operation::TransformedSpectrum *)m_pList->GetClientData( i );

				if  ( (kome::operation::TransformedSpectrum *)NULL != pListSpec )
				{
					if  ( pSpec->getOrgSpectrum( ) == pListSpec->getOrgSpectrum( ) )
					{
						m_pList->SetSelection( i );
					}
				}
			}
		}
		m_pList->Thaw( );
	}

	select( );

	return;
}

//	キャンバスで選択されたクロマトグラムをリストに反映
void	VerticalBoxDialog::SelectChromOfCanvas( kome::objects::Chromatogram* pChrom )
{
	if  ( (wxListBox *)NULL != m_pList )
	{
		m_pList->Freeze( );

		// unselect all
		m_pList->DeselectAll( );
	
		if  ( (kome::objects::Chromatogram *)NULL != pChrom )
		{
			for ( unsigned int i = 0 ; i < m_pList->GetCount( ) ; i++ )
			{
				kome::operation::TransformedChromatogram*	pListChrom =
					(kome::operation::TransformedChromatogram*)m_pList->GetClientData( i );

				if  ( pChrom->getOrgChromatogram( ) == pListChrom->getOrgChromatogram( ) )
				{
					m_pList->SetSelection( i );
				}
			}
		}
		m_pList->Thaw( );
	}

	select( );

	return;
}

// set button status
void	VerticalBoxDialog::setButtonStatus( void )
{
	if  ( (wxListBox *)NULL == m_pList )
	{
		return;
	}

	int		i;
	int		nSelect = -1;
	int		nListCount = (int)m_pList->GetCount( );
	bool	bFlg = false;

	for ( i = 0 ; i < nListCount ; i++ )
	{
		if  ( m_pList->IsSelected( i ) )
		{
			bFlg = true;
			nSelect = i;
			break;
		}
	}

	if  ( true == bFlg )
	{
		if  ( false == m_pBitmapButtonDelete->IsEnabled( ) )
		{
			m_pBitmapButtonDelete->Enable( true );
		}

		if  ( 0 == nSelect )
		{
			if  ( true == m_pBitmapButtonUp->IsEnabled( ) )
			{
				m_pBitmapButtonUp->Enable( false );
			}
		}
		else if  ( false == m_pBitmapButtonUp->IsEnabled( ) )
		{
			m_pBitmapButtonUp->Enable( true );
		}
		if  ( ( nListCount - 1 ) == nSelect )
		{
			if  ( true == m_pBitmapButtonDown->IsEnabled( ) )
			{
				m_pBitmapButtonDown->Enable( false );
			}
		}
		else if ( false == m_pBitmapButtonDown->IsEnabled( ) )
		{
			m_pBitmapButtonDown->Enable( true );
		}
	}
	else if  ( false == bFlg )
	{
		if  ( true == m_pBitmapButtonDelete->IsEnabled( ) )
		{
			m_pBitmapButtonDelete->Enable( false );
		}
		if  ( true == m_pBitmapButtonUp->IsEnabled( ) )
		{
			m_pBitmapButtonUp->Enable( false );
		}
		if  ( true == m_pBitmapButtonDown->IsEnabled( ) )
		{
			m_pBitmapButtonDown->Enable( false );
		}
	}

	return;
}


// Delete ( list )
void	VerticalBoxDialog::onDelete( wxCommandEvent& evt )
{
	// 項目が選択されているか?
	bool	bFlgSelected = false;

	// selected items
	std::vector< int >	selectedItems;
	for ( unsigned int i = 0 ; i < m_pList->GetCount( ) ; i++ )
	{
		if  ( m_pList->IsSelected( i ) )
		{
			bFlgSelected = true;	// 選択されている
		}
	}

	// onRemoveButtonと同様の処理
	if  ( true == bFlgSelected )
	{
		onRemoveButton( evt );
	}

	return;
}

// UI enable/disable check
void	VerticalBoxDialog::onUpdateUIPopupMenu( wxUpdateUIEvent& evt )
{
	int	nEventId = evt.GetId( );

	// ID check
	if  ( nEventId != ID_POPUP_MENU_DELETE )
	{
		return;
	}

	// 項目が選択されているか?
	bool	bFlgSelected = false;

	// selected items
	std::vector< int >	selectedItems;
	for ( unsigned int i = 0 ; i < m_pList->GetCount( ) ; i++ )
	{
		if  ( m_pList->IsSelected( i ) )
		{
			bFlgSelected = true;	// 選択されている
		}
	}

	// 選択されていればenable、されていなければdisable
	evt.Enable( bFlgSelected );

	return;
}

// create context Menu
void	VerticalBoxDialog::onContextMenu( wxContextMenuEvent& evt )
{
	wxPoint	ptScreen = evt.GetPosition();
	wxPoint	pt;
	pt = ScreenToClient( ptScreen );

	wxMenu menu;
	menu.Append( ID_POPUP_MENU_DELETE, wxT( STR_POPUP_MENU_DELETE ) );

	PopupMenu( &menu, pt.x, pt.y );

	return;
}

// close Chromatgram object
void	VerticalBoxDialog::closeChromatgram( kome::objects::Chromatogram* pChrom )
{
	if  ( false == m_bChromFlg || (wxListBox *)NULL == m_pList )
	{
		// ここでは、処理しません
		// 選択中のキャンバスがSpectrumのため、何もしません
		return;
	}

	m_pList->Freeze( );

	for ( unsigned int unIndex = 0 ; unIndex < m_pList->GetCount( ) ; unIndex++ )
	{
		kome::objects::Chromatogram*	pChromList = (kome::objects::Chromatogram*)m_pList->GetClientData( unIndex );
		if  ( pChrom == pChromList )
		{
			// リスト中に持っているchromatgramが削除されます
			m_pList->DeselectAll( );
			m_pList->SetSelection( unIndex );

			VerticalBoxChromatogramCanvas*	pChromCanvas = dynamic_cast< VerticalBoxChromatogramCanvas* >( m_pCanvas );
			if  ( (VerticalBoxChromatogramCanvas *)NULL != pChromCanvas )
			{
				pChromCanvas->setIndex( -1 );
				pChromCanvas->setIndexDisp( -1 );
			}
			m_pList->Delete( unIndex );

			break;
		}
	}
	m_pList->Thaw( );

	setButtonStatus( );

	return;
}

// close Spectrum object
void	VerticalBoxDialog::closeSpectrum( kome::objects::Spectrum* pSpec )
{
	if  ( true == m_bChromFlg )
	{
		// ここでは、処理しません
		// 選択中のキャンバスがChromatgramのため、なにもしません
		return;
	}

	m_pList->Freeze( );

	for ( unsigned int unIndex = 0 ; unIndex < m_pList->GetCount( ) ; unIndex++ )
	{
		kome::objects::Spectrum*	pSpecList = (kome::objects::Spectrum*)m_pList->GetClientData( unIndex );
		if  ( pSpec == pSpecList )
		{
			// リスト中に持っているchromatgramが削除されます
			m_pList->DeselectAll( );
			m_pList->SetSelection( unIndex );

			VerticalBoxSpectrumCanvas*	pSpecCanvas = dynamic_cast< VerticalBoxSpectrumCanvas* >( m_pCanvas );
			if  ( (VerticalBoxSpectrumCanvas *)NULL != pSpecCanvas )
			{
				pSpecCanvas->setIndex( -1 );
				pSpecCanvas->setIndexDisp( -1 );
			}
			m_pList->Delete( unIndex );

			break;
		}
	}
	m_pList->Thaw( );

	setButtonStatus( );

	return;
}

//
// end of source file
