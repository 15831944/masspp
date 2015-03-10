/**
 * @file OperationManagerDialog.cpp
 * @brief implements of OperationManagerDialog class
 *
 * @author S.Tanaka
 * @date 2007.03.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 
 * 補足説明
   Operationの処理を1つのダイアログにまとめた @date 2011.07.25 <Add> M.Izumi
 */

#include "stdafx.h"
#include "OperationManagerDialog.h"
#include "OperationManager.h"

#include <set>


using namespace kome::operation;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE			 3
#define GRID_GAP			 3

#define ID_LIST_BOX			 1
#define ID_DELETE_BUTTON	 2
#define ID_DELETEALL_BUTTON  3 


#define ID_SELECT_ALL	13345	// @date 2012/12/06 <Add> OKADA
#define ID_DESELECT_ALL	13346	// @date 2012/12/06 <Add> OKADA


BEGIN_EVENT_TABLE( OperationManagerDialog, kome::window::StandardDialog )
	EVT_BUTTON( ID_DELETE_BUTTON, OperationManagerDialog::onDelete )
	EVT_BUTTON( ID_DELETEALL_BUTTON, OperationManagerDialog::onDeleteAll )
    EVT_UPDATE_UI( ID_DELETE_BUTTON, OperationManagerDialog::onDeleteUpdate )
	EVT_UPDATE_UI( ID_DELETEALL_BUTTON, OperationManagerDialog::onDeleteAllUpdate )
	EVT_LISTBOX( ID_LIST_BOX, OperationManagerDialog::onSelect )
	EVT_CONTEXT_MENU( OperationManagerDialog::onContextMenu )			// @date 2012/12/06 <Add> OKADA
	EVT_MENU( ID_SELECT_ALL, OperationManagerDialog::onSelectAll )		// @date 2012/12/06 <Add> OKADA
	EVT_MENU( ID_DESELECT_ALL, OperationManagerDialog::onDeselectAll )	// @date 2012/12/06 <Add> OKADA
	EVT_UPDATE_UI(ID_SELECT_ALL, onUpdateUIPopupMenu) 
	EVT_UPDATE_UI(ID_DESELECT_ALL, onUpdateUIPopupMenu) 
END_EVENT_TABLE()


// constructor
OperationManagerDialog::OperationManagerDialog( wxWindow* parent )
		: kome::window::StandardDialog( parent, "Processing History" ) {
	// initialize
	m_listBox = NULL;
	m_type = TYPE_SPEC;
	m_data.spec = NULL;
}

// destructor
OperationManagerDialog::~OperationManagerDialog() {
}

// set spectrum
void OperationManagerDialog::setSpectrum( kome::objects::Spectrum* spec ) {
	m_type = TYPE_SPEC;
	m_data.spec = spec;
}

// set chromatogram
void OperationManagerDialog::setChromatogram( kome::objects::Chromatogram* chrom ) {
	m_type = TYPE_CHROM;
	m_data.chrom = chrom;
}

// get the number of operations to be deleted
unsigned int OperationManagerDialog::getNumberOfDeleteOperations() {
	return m_removeOperations.size();
}

// get operation to be deleted
kome::objects::XYDataOperation* OperationManagerDialog::getDeleteOperation( const unsigned int index ) {
	if( index >= m_removeOperations.size() ) {
		return NULL;
	}
	return m_removeOperations[ index ];
}

// get operations
void OperationManagerDialog::getOperations() {
	// clear
	m_operations.clear();

	// manager
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();

	// get operations
	if( m_type == TYPE_SPEC ) {
		kome::objects::Spectrum* spec = m_data.spec;
		for( unsigned int i = 0; i < ptMgr.getNumberOfOperations( spec ); i++ ) {
			kome::objects::XYDataOperation* op = ptMgr.getOperation( spec, i );
			m_operations.push_back( op );
		}
	}
	else {
		kome::objects::Chromatogram* chrom = m_data.chrom;
		for( unsigned int i = 0; i < ptMgr.getNumberOfOperations( chrom ); i++ ) {
			kome::objects::XYDataOperation* op = ptMgr.getOperation( chrom, i );
			m_operations.push_back( op );
		}
	}
}

// create main sizer
wxSizer* OperationManagerDialog::createMainSizer() {
	// top sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 2, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableRow( 0 );
	sizer->AddGrowableCol( 0 );

	// create string array
	getOperations();
	wxString* ops = NULL;
	if( m_operations.size() > 0 ) {
		ops = new wxString[ m_operations.size() ];
		for( unsigned int i = 0; i < m_operations.size(); i++ ) {
			ops[ i ] = wxT( m_operations[ i ]->getName() );
		}
	}
	
	// add list box
	m_listBox = new wxListBox(
		this,
		ID_LIST_BOX,
		wxDefaultPosition,
		wxSize( -1, 75 ),
		m_operations.size(),
		ops,
		wxLB_EXTENDED | wxLB_HSCROLL | wxLB_NEEDED_SB
	);
	sizer->Add( m_listBox, 1, wxGROW | wxALL, BORDER_SIZE );
		
	// delete
	if( ops != NULL ) {
		delete[] ops;
	}

	return sizer;
}

wxSizer* OperationManagerDialog::createButtonSizer(){
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );
	wxSizer* buttonSizer = new wxBoxSizer( wxHORIZONTAL );

	// @date 2011.08.25 <Mod> M.Izumi
	// SPEC 82173 : No error dialog is shown 
	m_delButton = new wxButton( this, ID_DELETE_BUTTON, wxT( "Delete" ), wxDefaultPosition, wxDefaultSize );
	m_delButton->Enable( false );
	// add delete button
	buttonSizer->Add(
		m_delButton,
		1,
		wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);
	// add delete all button // @date 2011.07.25 <Add> M.Izumi
	buttonSizer->Add(
		new wxButton( this, ID_DELETEALL_BUTTON, wxT( "Delete All" ), wxDefaultPosition, wxDefaultSize ),
		1,
		wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);
	sizer->Add( buttonSizer, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, BORDER_SIZE );
	
	buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	// ok button
	buttonSizer->Add( new wxButton( this, wxID_OK, wxT( "OK" ), wxDefaultPosition, wxDefaultSize ),
		1,
		wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);
	// cancel button
	buttonSizer->Add( new wxButton( this, wxID_CANCEL, wxT( "Cancel" ), wxDefaultPosition, wxDefaultSize ),
		1,
		wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);
	sizer->AddSpacer(10);
	sizer->Add( buttonSizer, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, BORDER_SIZE );
	return sizer;
}

// on OK // @date 2011.07.25 <Add> M.Izumi
void OperationManagerDialog::onOK( wxCommandEvent& evt ) {
	// end dialog
	EndDialog( wxID_OK );
}

// on cancel // @date 2011.07.25 <Add> M.Izumi
void OperationManagerDialog::onCancel( wxCommandEvent& evt ) {
	// end dialog
	EndDialog( wxID_CANCEL );
}

// on delete // @date 2011.07.25 <Add> M.Izumi
void OperationManagerDialog::onDelete( wxCommandEvent& evt ) {
	// get selection
	std::vector< int > selection;
	std::set< kome::objects::XYDataOperation* > rmOperationSet;

	for( unsigned int i = 0; i < m_listBox->GetCount(); i++ ) {
		if( m_listBox->IsSelected( i ) ) {
			rmOperationSet.insert( m_operations[ i ]  );
			m_removeOperations.push_back( m_operations[ i ] );

			selection.push_back( i );
		}
	}
	setRemoveFunction( selection, rmOperationSet );	
}

// on delete all // @date 2011.07.25 <Add> M.Izumi
void OperationManagerDialog::onDeleteAll( wxCommandEvent& evt ) {
	// @date 2012/04/26 <Add> OKADA ------->
	//　SPEC 85548
	bool bRet = false;	// [OK]=true [Cancel]=false
	bRet = kome::window::WindowTool::showYesNo( 
				"Are you sure you want to delete all items in the Processing History?", "Delete all", "Don't delete" );
	if( bRet ){
		;	// Yes
	}else{
		return;		// No
	}
	// @date 2012/04/26 <Add> OKADA <-------

	// @date 2011.08.25 <Mod> M.Izumi
	// get selection
	std::vector< int > selection;
	std::set< kome::objects::XYDataOperation* > rmOperationSet;

	// all select 
	for( unsigned int i = 0; i < m_listBox->GetCount(); i++ ) {	
		rmOperationSet.insert( m_operations[ i ]  );
		m_removeOperations.push_back( m_operations[ i ] );

		selection.push_back( i );
	}

	setRemoveFunction( selection, rmOperationSet );	
}

// on update delete
void OperationManagerDialog::onDeleteUpdate( wxUpdateUIEvent& evt ) {
	// get the selection
	bool selected = false;
	for( unsigned int i = 0; i < m_listBox->GetCount() && !selected; i++ ) {
		if( m_listBox->IsSelected( i ) ) {
			selected = true;
		}
	}

	// enable
	evt.Enable( selected );
}

// on update delete all
void OperationManagerDialog::onDeleteAllUpdate( wxUpdateUIEvent& evt ) {
	evt.Enable( m_listBox->GetCount() > 0 );
}

// set remove list	@date 2011.08.25 <Add> M.Izumi
void OperationManagerDialog::setRemoveFunction( 
	std::vector< int > selection, 
	std::set< kome::objects::XYDataOperation* > rmOperationSet
){
	// copy to array
	std::vector< kome::objects::XYDataOperation* > operations;
	for( unsigned int i = 0; i < m_operations.size(); i++ ) {
		operations.push_back( m_operations[ i ] );
	}

	// update array
	m_operations.clear();
	for( unsigned int i = 0; i < operations.size(); i++ ) {
		kome::objects::XYDataOperation* operation = operations[ i ];
		if( rmOperationSet.find( operation ) == rmOperationSet.end() ) {
			m_operations.push_back( operation );
		}
	}

	// delete from list
	for( int i = (int)selection.size() - 1; i >= 0; i-- ) {
		m_listBox->Delete( selection[ i ] );
	}
	m_delButton->Enable( false );
}

// on select	@date 2011.08.25 <Add> M.Izumi
void OperationManagerDialog::onSelect( wxCommandEvent& evt ){
	if( !m_delButton->IsEnabled() ){
		m_delButton->Enable( true );
	}
}

// @date 2012/12/06 <Add> OKADA
// context menu
void OperationManagerDialog::onContextMenu( wxContextMenuEvent& evt ) {

	// get position
	wxPoint ptScreen = evt.GetPosition();
	wxPoint pt;
	
	// If the event originated from a keyboard event ...
	if(	ptScreen == wxDefaultPosition ){
		// keyboard event
		pt.x = 0;	// top left of cliant window
		pt.y = 0;
	}else{
		// mouse event
		pt = ScreenToClient( ptScreen );
	}

	// create menu
	wxMenu* menu = createMenu();
	if( menu == NULL ) {
		return;
	}

	// popup menu
	PopupMenu( menu, pt.x, pt.y );
	delete menu;
}

wxMenu* OperationManagerDialog::createMenu(){
	// menu
	wxMenu* menu = new wxMenu();
	menu->Append( ID_SELECT_ALL, wxT( "Select &All" ) );
	menu->Append( ID_DESELECT_ALL, wxT( "&Deselect All" ) );

	return menu;
}

// "Select All"
void OperationManagerDialog::onSelectAll( wxCommandEvent& evt ) {
	selectAll();
}

// "Deselect All"
void OperationManagerDialog::onDeselectAll( wxCommandEvent& evt ) {
	deselectAll();
}

// select all
void OperationManagerDialog::selectAll() {
	for( unsigned int i = 0; i < m_listBox->GetCount(); i++ ) {	// @date 2013/06/10 <Mod> OKADA
		m_listBox->Select( i );
	}
}

// deselect all
void OperationManagerDialog::deselectAll() {
	m_listBox->DeselectAll();
}

// UI enable/disable check
void OperationManagerDialog::onUpdateUIPopupMenu( wxUpdateUIEvent& evt ){
	int nEventId = evt.GetId();

	// ID check
	if(	( nEventId == ID_SELECT_ALL ) || ( nEventId == ID_DESELECT_ALL )	){
		// OK
	}else{
		// error
		return;
	}

	// selectするものはあるか？
	if( m_listBox->GetCount() <= 0 ){
		// 表示項目が無い
		evt.Enable( false );	// selectするものが無いので、disable
	}else{
		// 表示項目あり
		evt.Enable( true );
	}

}
