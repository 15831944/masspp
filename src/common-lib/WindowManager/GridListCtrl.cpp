/**
 * @file GridListCtrl.cpp
 * @brief implements of GridListCtrl class
 *
 * @author S.Tanaka
 * @date 2007.11.12
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "GridListCtrl.h"

#include "GridListDialog.h"
#include "WindowTool.h"

#include "ListTool.h"
#include <float.h>
#include <algorithm>
#include <boost/bind.hpp>
#include <wx/clipbrd.h>

#include "MainFrame.h"

using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define ID_SELECT_ALL	13345
#define ID_DESELECT_ALL	13346
#define ID_COPY			13347
#define ID_FILTER		13348
#define ID_SAVE			13349

#define ID_ACTIVE		13350
#define ID_DELETE		13351
#define ID_ADD			13352

#define ID_SENTINEL		13353

#define COLUMN_WIDTH	   80

#define FLG_EDITABLE      ( wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_VIRTUAL | wxLC_EDIT_LABELS )
#define FLG_UNEDITABLE    ( wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_VIRTUAL )
#define FLG_EDITABLE_S    ( wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_VIRTUAL | wxLC_EDIT_LABELS | wxLC_SINGLE_SEL )
#define FLG_UNEDITABLE_S  ( wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_VIRTUAL | wxLC_SINGLE_SEL )



BEGIN_EVENT_TABLE( GridListCtrl, ListCtrl )
	EVT_LIST_COL_CLICK( wxID_ANY, GridListCtrl::onColClick )
	EVT_LIST_END_LABEL_EDIT( wxID_ANY, GridListCtrl::onLabelEdit )
	EVT_LIST_ITEM_ACTIVATED( wxID_ANY, GridListCtrl::onActivateItem )
	EVT_CONTEXT_MENU( GridListCtrl::onContextMenu )

	EVT_MENU( ID_ACTIVE, GridListCtrl::onActive )	// Context Menu

	EVT_MENU( ID_SELECT_ALL, GridListCtrl::onSelectAll )
	EVT_MENU( ID_DESELECT_ALL, GridListCtrl::onDeselectAll )
	EVT_MENU( ID_COPY, GridListCtrl::onCopy )
	EVT_MENU( ID_FILTER, GridListCtrl::onFilter )
	EVT_MENU( ID_SAVE, GridListCtrl::onSave )

	EVT_MENU( ID_DELETE, GridListCtrl::onDelete )	// Context Menu

	EVT_UPDATE_UI_RANGE( ID_SELECT_ALL, ID_SENTINEL, GridListCtrl::onUpdateUI )

	EVT_KEY_UP( GridListCtrl::onKeyUp )
END_EVENT_TABLE()


// constructor
GridListCtrl::GridListCtrl(
		wxWindow* parent,
		const int id,
		const int width,
		const int height,
		const bool editable,
		const bool autoColumnWidth,
		const bool singleSelect
) : ListCtrl(
		parent,
		id,
		width,
		height,
	    ( singleSelect ? ( editable ? FLG_EDITABLE_S : FLG_UNEDITABLE_S ) : ( editable ? FLG_EDITABLE : FLG_UNEDITABLE ) ),
		autoColumnWidth
) {
	// initialize
	m_sort = -1;

	m_filterSearchType = GridSearchDialog::SEARCH_NEW;
	m_filterCol = -1;
	
	m_title = "";

	// set list
	GridListDialog* dlg = dynamic_cast< GridListDialog* >( parent );
	if( dlg != NULL ) {
		dlg->setGridListCtrl( this );
	}
	else{
		int debug=0;	
	}
	m_dlgParent = (void *)dlg;	// @date 2012/12/13 <Add> OKADA

	// Context Menu
	m_flgPopupMenu = 0;
	m_strActivateName = "";

	// SPEC 91673
	setPopupMenuFlag( POPUP_SELECT | POPUP_DESELECT );	// @date 2013/03/19 <Add> OKADA

}

// destructor
GridListCtrl::~GridListCtrl() {
}

// set header title
void GridListCtrl::setHeaderTitle( const int column, const char* title ) {
	// array size
	int size = (int)m_titles.size();
	if( column >= size ) {
		m_titles.resize( column + 1 );
	}

	// set title
	m_titles[ column ] = NVL( title, "" );
}

// get header title
const char* GridListCtrl::getHeaderTitle( const int column ) {
	if( column < 0 || column >= (int)m_titles.size() ) {
		return NULL;
	}

	return m_titles[ column ].c_str();
}

// set column data type
void GridListCtrl::setDataType( const int column, const ColumnDataType type ) {
	// array size
	int size = (int)m_columnTypes.size();
	if( column >= size ) {
		m_columnTypes.resize( column + 1 );
		for( int i = size; i <= column; i++ ) {
			m_columnTypes[ i ] = TYPE_STRING;
		}
	}

	// set type
	m_columnTypes[ column ] = type;
}

// get column data type
GridListCtrl::ColumnDataType GridListCtrl::getDataType( const int column ) {
	if( column < 0 || column >= (int)m_columnTypes.size() ) {
		return TYPE_STRING;
	}
	return m_columnTypes[ column ];
}

// clear data
void GridListCtrl::clearData( const bool update ) {
	m_data.clear();
	m_allData.clear();

	// There is a problem with the wxListCtrl in wxWidgets relating
	// to scrollbars when all items in the list are deleted.
	// The following function call solves the problem.
	EnsureVisible(0);

	if( update ) {
		updateList();
	}
}

// add data
void GridListCtrl::addData( long long data, const bool update ) {
	m_data.push_back( data );
	m_allData.push_back( data );

	if( update ) {
		updateList();
	}
}

// delete data
void GridListCtrl::deleteData( long long data, const bool update ) {
	// delete data
	if( !onDeleteData( data ) ) {
		return;
	}

	// delete from array
	for( int i = (int)m_data.size() - 1; i >= 0; i-- ) {
		if( m_data[ i ] == data ) {
			m_data.erase( m_data.begin() + i );
		}
	}
	for( int i = (int)m_allData.size() - 1; i >= 0; i-- ) {
		if( m_allData[ i ] == data ) {
			m_allData.erase( m_allData.begin() + i );
		}
	}

	// update 
	if( update ) {
		updateList();
	}
}

// swap data
void GridListCtrl::swapData( const int idx0, const int idx1, const bool update ) {
	// check index
	if( idx0 < 0 || idx0 >= (int)m_data.size()
			|| idx1 < 0 || idx1 >= (int)m_data.size() ) {
		return;
	}

	// index
	const int minIdx = std::min( idx0, idx1 );
	const int maxIdx = std::max( idx0, idx1 );

	int allMinIdx = -1;
	int allMaxIdx = -1;

	for( int i = minIdx; i < (int)m_allData.size() && allMinIdx < 0; i++ ) {
		if( m_data[ minIdx ] == m_allData[ i ] ) {
			allMinIdx = i;
		}
	}

	for( int i = std::max( allMinIdx, maxIdx ); i < (int)m_allData.size() && allMaxIdx < 0; i++ ) {
		if( m_data[ maxIdx ] == m_allData[ i ] ) {
			allMaxIdx = i;
		}
	}

	// swap
	long long tmp = m_data[ minIdx ];
	m_data[ minIdx ] = m_data[ maxIdx ];
	m_data[ maxIdx ] = tmp;

	if( allMinIdx >= 0 && allMaxIdx >= 0 ) {
		tmp = m_allData[ allMinIdx ];
		m_allData[ allMinIdx ] = m_allData[ allMaxIdx ];
		m_allData[ allMaxIdx ] = tmp;
	}

	// update
	if( update ) {
		updateList();
	}
}

// get row size
unsigned int GridListCtrl::getRowSize() {
	return m_data.size();
}

// get list data
long long GridListCtrl::getListData( const int row ) {
	if( row >= (int)m_data.size() ) {
		return 0;
	}
	return m_data[ row ];
}

// update list
void GridListCtrl::updateList() {
	m_sort = -1;
	SetItemCount( m_data.size() );

	if( m_autoColumnWidth ) {
		setColumnWidths();
	}
	
	Refresh();
}

// create list
void GridListCtrl::create() {
	// delete column
	while( GetColumnCount() > 0 ) {
		DeleteColumn( 0 );
	}

	// delete data
	clearData( false );

	// create
	onCreate();

	// update
	updateList();
}

// get the number of data
unsigned int GridListCtrl::getNumberOfData() {
	return m_data.size();
}

// get item data
long long GridListCtrl::getData( const unsigned int item ) {
	if( item >= m_data.size() ) {
		return 0;
	}
	return m_data[ item ];
}

// export data
bool GridListCtrl::exportData() {
	
	// dialog
	wxFileDialog dlg(
		this,
		wxT( "Save" ),
		wxEmptyString,
		wxT( "list.txt" ),
		wxT( "Text files (*.txt)|*.txt" ),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT	// SPEC No.83004: Overwrite the file without the confirmation @date 2011.11.22 <Mod> M.Izumi 
	);

	// open
	if( dlg.ShowModal() == wxID_OK ) {
		// save
		FILE* fp = fileopen( dlg.GetPath().c_str(), "w" );
		if( fp == NULL ) {
			std::string msg = FMT( "Cannot open the file for writing. Check the file / folder permissions. [%s]", dlg.GetPath().c_str() );
			WindowTool::showError( msg.c_str() );
			return false;
		}
		
		// header
		for( int j = 0; j < GetColumnCount(); j++ ) {
			// tab
			if( j > 0 ) {
				fprintf( fp, "\t" );
			}

			// column title
			const char* title = getHeaderTitle( j );
			fprintf( fp, "%s", NVL( title, "" ) );
		}

		// value
		for( int i = 0; i < GetItemCount(); i++ ) {
			// new line
			fprintf( fp, "\n" );

			// value
			for( int j = 0; j < GetColumnCount(); j++ ) {
				if( j > 0 ) {
					fprintf( fp, "\t" );
				}

				std::string s = trimstring( OnGetItemText( i, j ).c_str() );
				fprintf( fp, "%s", s.c_str() );
			}
		}

		// close
		fflush( fp );
		fclose( fp );
	}

	return true;
}

// execute filter
void GridListCtrl::executeFilter() {
	// dialog
	GridSearchDialog dlg( this );

	dlg.setSearchType( m_filterSearchType );
	dlg.setColumn( m_filterCol );
	dlg.setKeyword( m_filterKeyword.c_str() );
	dlg.setValueType( m_filterValueType );
	dlg.setValueRange( m_filterMinVal.c_str(), m_filterMaxVal.c_str() );

	if( dlg.ShowModal() == wxID_OK ) {

		// keep filter condition
		m_filterSearchType = dlg.getSearchType();
		m_filterCol = dlg.getColumn();
		m_filterKeyword = dlg.getKeyword();
		m_filterValueType = dlg.getValueType();
		m_filterMinVal = dlg.getMinValue();
		m_filterMaxVal = dlg.getMaxValue();

		// source
		std::vector< long long > source;
		if( m_filterSearchType == GridSearchDialog::SEARCH_NARROW ) {
			source = m_data;
		}
		else {
			source = m_allData;
		}

		// filter
		std::set< long long > result;
		if( m_filterSearchType == GridSearchDialog::SEARCH_APPEND ) {
			for( unsigned int i = 0; i < m_data.size(); i++ ) {
				result.insert( m_data[ i ] );
			}
		}

		for( unsigned int i = 0; i < source.size(); i++ ) {
			long long data = source[ i ];
			bool flg = false;

			for( int j = 0; j < GetColumnCount() && !flg; j++ ) {
				if( !flg && ( m_filterCol < 0 || m_filterCol == j ) ) {
					if( m_filterValueType == GridSearchDialog::VALUE_KEYWORD ) {	// keyword
						std::string v = getItemText( data, j );
						if( m_filterKeyword.empty()	|| v.find( m_filterKeyword ) != v.npos ) {
							flg = true;
						}
					}
					else {	// range
// >>>>>>	@Date:2013/12/10	<Add>	A.Ozaki
// 日付・時間文字列形式を追加しました
//
//						if( m_columnTypes[ j ] == TYPE_STRING ) {	// string
						if( m_columnTypes[ j ] == TYPE_STRING ||
							m_columnTypes[ j ] == TYPE_DATE_TIME_STRING ) {	// string
// <<<<<<	@Date:2013/12/10	<Add>	A.Ozaki
								std::string v = getItemText( data, j );
						
							if( ( m_filterMinVal.empty() || v.compare( m_filterMinVal ) >= 0 )
									&& ( m_filterMaxVal.empty() || v.compare( m_filterMaxVal ) <= 0 ) ) {
								flg = true;
							}
						}
						else {	// number
							// value
							double v = double();
							if( m_columnTypes[ j ] == TYPE_INT || m_columnTypes[ j ] == TYPE_UINT ) {	// integer
								v = (double)getInt( data, j );
							}
							else {	// double
								v = getDouble( data, j );
							}

							// check range
							double minVal = double();
							double maxVal = double();
							if( ( v >= 0.0 || ( m_columnTypes[ j ] != TYPE_UINT && m_columnTypes[ j ] != TYPE_UDOUBLE ) )
									&& ( !isdouble( m_filterMinVal.c_str(), &minVal ) || v >= minVal )
									&& ( !isdouble( m_filterMaxVal.c_str(), &maxVal ) || v <= maxVal ) ) {
								flg = true;
							}								
						}
					}
				}
			}

			if( flg ) {
				result.insert( data );
			}
		}

		// update
		m_data.clear();
		for( unsigned int i = 0; i < m_allData.size(); i++ ) {
			long long data = m_allData[ i ];
			if( result.find( data ) != result.end() ) {
				m_data.push_back( data );
			}
		}
	}

	// SPEC No.86120 The number of peaks is not updated.
	// main frame
	std::string s;
	int iFind1 = m_title.find( "(" );
	if( iFind1 != -1 ){
		s.append( FMT( "%s(", m_title.substr( 0, iFind1 ).c_str() ).c_str() );
		std::string tmp =  m_title.substr( iFind1 );
		
		s.append( FMT("%d %s", m_data.size(), tmp.substr( tmp.find( " " ) ).c_str() ).c_str() );
		
		setTitle( s.c_str() );
	}

	// update
	updateList();
}

// add column
void GridListCtrl::addColumn( const char* title, const ColumnDataType type, const int width ) {
	// column number
	int column = GetColumnCount();

	// insert column
	InsertColumn( column, wxT( NVL( title, "" ) ), wxLIST_FORMAT_LEFT, width );

	// set type
	setHeaderTitle( column, title );
	setDataType( column, type );
}

// select all
void GridListCtrl::selectAll() {
	for( int i = 0; i < GetItemCount(); i++ ) {
		SetItemState( i, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
	}
}

// deselect all
void GridListCtrl::deselectAll() {
	for( int i = 0; i < GetItemCount(); i++ ) {
		SetItemState( i, 0, wxLIST_STATE_SELECTED );
	}
}

// >>>>>>	@Date:2013/12/10	<Add>	A.Ozaki
//
// convert date/time format
std::string	GridListCtrl::convertDateTimeString( const char *pcFormatedDate )
{
	std::string	strDateTime;

	std::vector< std::string > elms;
	stringtoken( pcFormatedDate, " ", elms );

	if  ( 4 != elms.size( ) )
	{	// 形式が異なるので、そのまま使用
		strDateTime = pcFormatedDate;
	}
	else
	{
		std::string	strMonth;
		std::string	strDay;

		strDay = FMT( "%02d", toint( elms[0].c_str( ) ) );

		strMonth.clear( );
		strMonth = tolowercase( elms[1].c_str( ) );
		std::string	strMonthVal;
		if  ( "jan" == strMonth )
		{
			strMonthVal = "01";
		}
		else if  ( "feb" == strMonth )
		{
			strMonthVal = "02";
		}
		else if  ( "mar" == strMonth )
		{
			strMonthVal = "03";
		}
		else if  ( "apr" == strMonth )
		{
			strMonthVal = "04";
		}
		else if  ( "may" == strMonth )
		{
			strMonthVal = "05";
		}
		else if  ( "jun" == strMonth )
		{
			strMonthVal = "06";
		}
		else if  ( "jul" == strMonth )
		{
			strMonthVal = "07";
		}
		else if  ( "aug" == strMonth )
		{
			strMonthVal = "08";
		}
		else if  ( "sep" == strMonth )
		{
			strMonthVal = "09";
		}
		else if  ( "oct" == strMonth )
		{
			strMonthVal = "10";
		}
		else if  ( "nov" == strMonth )
		{
			strMonthVal = "11";
		}
		else if  ( "dec" == strMonth )
		{
			strMonthVal = "12";
		}

		strDateTime = FMT( "%s %s %s %s", elms[2].c_str( ),
										strMonthVal.c_str( ),
										strDay.c_str( ),
										elms[3].c_str( ) );
	}

	return	strDateTime;
}
//
// <<<<<<	@Date:2013/12/10	<Add>	A.Ozaki

// compare data to sort
bool GridListCtrl::lessData( long long data0, long long data1, const int column, const bool desc ) {
	// type
	ColumnDataType type = getDataType( column );

	// compare
	if( type == TYPE_INT || type == TYPE_UINT ) {   // integer
		// get integer values
		int n0 = getInt( data0, column );
		int n1 = getInt( data1, column );

		// check value
		if( type == TYPE_UINT ) {
			if( n0 < 0 && n1 < 0 ) {
				return false;
			}
			if( n0 < 0 ) {
				return desc;
			}
			if( n1 < 0 ) {
				return !desc;
			}
		}
		
		// compare
		if( n0 < n1 ) {
			return !desc;
		}
		if( n0 > n1 ) {
			return desc;
		}
	}
	if( type == TYPE_DOUBLE || type == TYPE_UDOUBLE ) {		// double
		// get double values
		double v0 = getDouble( data0, column );
		double v1 = getDouble( data1, column );

		// check value
		if( type == TYPE_UDOUBLE ) {
			if( v0 < 0.0 && v1 < 0.0 ) {
				return false;
			}
			if( v0 < 0.0 ) {
				return desc;
			}
			if( v1 < 0.0 ) {
				return !desc;
			}
		}

		// compare
		if( v0 < v1 ) {
			return !desc;
		}
		if( v0 > v1 ) {
			return desc;
		}
	}
	else {  // string
		// get string
// >>>>>>	@Date:2013/12/10	<Add>	A.Ozaki
// 日付・時間文字列のソート処理を追加
//
//		std::string s0 = getString( data0, column );
//		std::string s1 = getString( data1, column );
//
		std::string	s0;
		std::string	s1;

		if  ( TYPE_DATE_TIME_STRING == type )
		{
			// dd mmm yyyy hh:mm:ss形式からyyyy mm dd hh:mm:ss形式に変換します
			//
			s0 = convertDateTimeString( getString( data0, column ).c_str( ) );
			s1 = convertDateTimeString( getString( data1, column ).c_str( ) );
		}
		else
		{
			s0 = getString( data0, column );
			s1 = getString( data1, column );
		}
//
// <<<<<<	@Date:2013/12/10	<Add>	A.Ozaki

		// check string
		if( s0.empty() && s1.empty() ) {
			return false;
		}
		if( s0.empty() ) {
			return desc;
		}
		if( s1.empty() ) {
			return !desc;
		}

		// compare
		int cmp = s0.compare( s1 );
		if( desc ) {
			return ( cmp > 0 );
		}
		else {
			return ( cmp < 0 );
		}
	}

	return false;
}

// set data color
void GridListCtrl::setDataColor( const long long data, const COLOR color ) {
	// color
	wxColour c(
		kome::img::ColorTool::getRed( color ),
		kome::img::ColorTool::getGreen( color ),
		kome::img::ColorTool::getBlue( color )
	);

	m_colorMap[ data ].SetTextColour( c );
}

// set data color
void GridListCtrl::setBackgroundColor( const long long data, const COLOR color ) {
	// color
	wxColour c(
		kome::img::ColorTool::getRed( color ),
		kome::img::ColorTool::getGreen( color ),
		kome::img::ColorTool::getBlue( color )
	);

	m_colorMap[ data ].SetBackgroundColour( c );
}

// clear item color
void GridListCtrl::clearDataColor() {
	m_colorMap.clear();
}

//  on create
void GridListCtrl::onCreate() {
}

// get interger value
int GridListCtrl::getInt( const long long data, const int column ) const {
	return 0;
}

// get double value
double GridListCtrl::getDouble( const long long data, const int column ) const {
	return 0.0;
}

// get character string value
std::string GridListCtrl::getString( const long long data, const int column ) const {
	std::string s;
	return s;
}

// set item property value
void GridListCtrl::setItemValue( const long long data, const int column, const char* s ) {
}

// create menu
wxMenu* GridListCtrl::createMenu() {
	// menu
	wxMenu* menu = new wxMenu();
	
	bool flgMenuDisp=false;	// menuを1項目でもセットしたらtrue。SEPARATOR表示判別用。

	if( !getActivateName().empty() ){

		// @date 2012/11/30 <Add> OKADA
		bool bCheckable = false;	// check可能なボタン *ではない*
		wxMenuItem* item = new wxMenuItem(
			menu,
			ID_ACTIVE,
			wxT( getActivateName().c_str() ),
			wxT( "" ),
			bCheckable ? wxITEM_CHECK : wxITEM_NORMAL
		);
		item->SetCheckable( bCheckable );

		wxFont font = item->GetFont();
		font.SetWeight( wxBOLD );
		item->SetFont( font );

		menu->Append( item );

		flgMenuDisp = true;

	}

	int nPopupMenuFlag = getPopupMenuFlag();

	wxString wxsStr;

	// ID_SELECT_ALL
	if( nPopupMenuFlag & POPUP_SELECT ){
		wxsStr = getPopupMenuName(POPUP_SELECT).c_str();
		menu->Append( ID_SELECT_ALL, wxsStr );
		flgMenuDisp = true;
	}

	// ID_DESELECT_ALL
	if( nPopupMenuFlag & POPUP_DESELECT ){
		menu->Append( ID_DESELECT_ALL, wxT( getPopupMenuName(POPUP_DESELECT).c_str() ) );
		flgMenuDisp = true;
	}

	// ID_COPY
	if( nPopupMenuFlag & POPUP_COPY ){
		if( flgMenuDisp ){
			menu->AppendSeparator();	// SEPARATOR //////////////////////////////
			flgMenuDisp = false;
		}
		menu->Append( ID_COPY, wxT( getPopupMenuName(POPUP_COPY).c_str() ) );
		flgMenuDisp = true;
	}

	// ID_DELETE
	if( nPopupMenuFlag & POPUP_DELETE ){
		if( flgMenuDisp ){
			menu->AppendSeparator();// SEPARATOR //////////////////////////////
			flgMenuDisp = false;
		}
		menu->Append( ID_DELETE, wxT( getPopupMenuName(POPUP_DELETE).c_str() ) );
		flgMenuDisp = true;
	}

	// ID_FILTER
	if( nPopupMenuFlag & POPUP_FILTER ){
		if( flgMenuDisp ){
			menu->AppendSeparator();	// SEPARATOR //////////////////////////////
			flgMenuDisp = false;
		}
		menu->Append( ID_FILTER, wxT( getPopupMenuName(POPUP_FILTER).c_str() ) );
		flgMenuDisp = true;
	}

	// ID_SAVE
	if( nPopupMenuFlag & POPUP_SAVE ){
		if( flgMenuDisp ){
			menu->AppendSeparator();	// SEPARATOR //////////////////////////////
			flgMenuDisp = false;
		}
		menu->Append( ID_SAVE, wxT( getPopupMenuName(POPUP_SAVE).c_str() ) );
		flgMenuDisp = true;
	}

	return menu;
}

// on activate data
void GridListCtrl::onActivateData( const long long data ) {
}

// on delete data
bool GridListCtrl::onDeleteData( const long long data ) {
	return true;
}

// get item text
std::string GridListCtrl::getItemText( const long long data, const int column ) const {
	// string object
	std::string str;

	// column type
	ColumnDataType type = TYPE_STRING;
	if( column < (int)m_columnTypes.size() ) {
		type = m_columnTypes[ column ];
	}

	// character string
	if( type == TYPE_INT || type == TYPE_UINT ) {   // integer
		int n = getInt( data, column );
		if( n > INT_MIN ) {
			if( type == TYPE_INT || n >= 0 ) {
				str = FMT( "%d", n );
			}
		}
	}
	else if( type == TYPE_DOUBLE || type == TYPE_UDOUBLE ) {		// double
		double v = getDouble( data, column );
		if( v > - FLT_MAX ) {
			if( type == TYPE_DOUBLE || v >= 0.0 ) {
				str = FMT( "%f", v );
			}
		}
	}
	else {  // string
		str = getString( data, column );
	}

	return str;
}

// get item text
wxString GridListCtrl::OnGetItemText( long item, long column ) const {
	// data
	long long data = 0;
	if( item <  (int)m_data.size() ) {
		data = m_data[ item ];
	}

	// item text
	std::string str = getItemText( data, column );
	wxString ret = wxT( str.c_str() );
	return ret;
}

// get item attribute
wxListItemAttr* GridListCtrl::OnGetItemAttr( long item ) const {
	// data
	long long data = 0;
	if( item <  (int)m_data.size() ) {	// @date 2011/03/24 <Add> OKADA
		data = m_data[ item ];
	}

	// default
	wxListItemAttr* attr = wxListCtrl::OnGetItemAttr( item );

	// set color
	if( m_colorMap.find( data ) != m_colorMap.end() ) {
		attr = const_cast< wxListItemAttr* >( &( m_colorMap.at( data ) ) );
	}

	return attr;
}

// on column click
void GridListCtrl::onColClick( wxListEvent& evt ) {
	// column
	int col = evt.GetColumn();

	// check the array
	if( m_data.size() == 0 ) {
		return;
	}

	// descending flag
	bool desc = ( m_sort == col );

	// get selected data
	std::set< long long > selectedData;
	for( int i = 0; i < GetItemCount(); i++ ) {
		if( GetItemState( i, wxLIST_STATE_SELECTED ) ) {
			selectedData.insert( m_data[ i ] );
		}
	}

	// deselect
	deselectAll();

	// keep filtered data
	std::set< long long > dataSet;
	for( unsigned int i = 0; i < m_data.size(); i++ ) {
		dataSet.insert( m_data[ i ]  );
	}

	// sort
	std::sort(
		m_allData.begin(),
		m_allData.end(),		
		boost::bind(
			&GridListCtrl::lessData,
			this,
			_1,
			_2,
			col,
			desc
		)
	);

	m_data.clear();
	for( unsigned int i = 0; i < m_allData.size(); i++ ) {
		long long data = m_allData[ i ];
		if( dataSet.find( data ) != dataSet.end() ) {
			m_data.push_back( data );
		}
	}

	// set sort info
	if( m_sort == col ) {
		m_sort = -1;
	}
	else {
		m_sort = col;
	}

	// select
	for( int i = 0; i < GetItemCount(); i++ ) {
		long long data = m_data[ i ];
		if( selectedData.find( data ) != selectedData.end() ) {		// selected data
			SetItemState( i, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
		}
	}

	// refresh
	Refresh();
}

// on label edited
void GridListCtrl::onLabelEdit( wxListEvent& evt ) {
	// check canceled
	if( evt.IsEditCancelled() ) {
		return;
	}

	// item
	int item = evt.GetIndex();
	if( item < 0 || item >= (int)m_data.size() ) {
		return;
	}
	long long data = m_data[ item ];

	// column
	int col = evt.GetColumn();

	// set value
	setItemValue( data, col, evt.GetLabel().c_str() );

	// refresh
	Refresh();
}

// on activate item
void GridListCtrl::onActivateItem( wxListEvent& evt ) {
	// item
	int item = evt.GetIndex();
	if( item < 0 || item >= (int)m_data.size() ) {
		return;
	}
	long long data = m_data[ item ];

	// activate
	onActivateData( data );
}

// context menu
void GridListCtrl::onContextMenu( wxContextMenuEvent& evt ) {

	// @date 2011/08/10 <Add> OKADA ------->
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
	// @date 2011/08/10 <Add> OKADA <-------


	// create menu
	wxMenu* menu = createMenu();
	if( menu == NULL ) {
		return;
	}

	// popup menu
	PopupMenu( menu, pt.x, pt.y );
	delete menu;
}

// on update UI
void GridListCtrl::onUpdateUI( wxUpdateUIEvent& evt ) {
	evt.Enable( true );
}

// on key up
void GridListCtrl::onKeyUp( wxKeyEvent& evt ) {
	// control
	if( evt.ControlDown() ) {
		if( evt.GetKeyCode() == 'F' ) {	// Ctrl + F
			executeFilter();
		}
	}
}

// on active
void GridListCtrl::onActive( wxCommandEvent& evt ) {
	// text data
	wxString str;

	// create text
	for( int i = 0; i < GetItemCount(); i++ ) {		// row
		if( GetItemState( i, wxLIST_STATE_SELECTED ) ) {	// selected item
			// i が選択された行番号になります。一番上は0行目です

			// activate
			onActivateData( i );
		}
	}
}

// "Select All"
void GridListCtrl::onSelectAll( wxCommandEvent& evt ) {
	selectAll();
}

// "Deselect All"
void GridListCtrl::onDeselectAll( wxCommandEvent& evt ) {
	deselectAll();
}

// copy
void GridListCtrl::onCopy( wxCommandEvent& evt ) {
	// text data
	wxString str;

	// create text
	for( int i = 0; i < GetItemCount(); i++ ) {		// row
		if( GetItemState( i, wxLIST_STATE_SELECTED ) ) {	// selected item
			// new line
			if( !str.empty() ) {
				str.Append( NEW_LINE );
			}

			// line
			for( int j = 0; j < GetColumnCount(); j++ ) {
				// tab
				if( j > 0 ) {
					str.append( "\t" );
				}

				// string
				str.append( OnGetItemText( i, j ) );
			}
		}
	}

	// text	
	wxTextDataObject* text = new wxTextDataObject( str );

	// open clipboard
	if( !wxTheClipboard->Open() ) {
		return;
	}

	// set clipboard
	wxTheClipboard->SetData( text );

	// close
	wxTheClipboard->Close();
}

// filter
void GridListCtrl::onFilter( wxCommandEvent& evt ) {
	executeFilter();
}

// save
void GridListCtrl::onSave( wxCommandEvent& evt ) {
	exportData();
}

// set title
void GridListCtrl::setTitle( const char* title ){
	m_title = title;

	MainFrame* mainFrame = (MainFrame*)kome::window::WindowTool::getMainWindow();
	if( mainFrame != NULL && !m_title.empty() ){
		
		mainFrame->setPaneTitle( this , m_title.c_str() );
		mainFrame->Refresh();
	
	}	
}

// ----------------
// Context Menu
// ----------------

// set popup menu flag
void GridListCtrl::setPopupMenuFlag( int flgPopupMenu ){
	m_flgPopupMenu = flgPopupMenu;
};

// get popup menu flag
int GridListCtrl::getPopupMenuFlag( void ){
	return m_flgPopupMenu;
};

// get popup menu name
std::string GridListCtrl::getPopupMenuName( PopupMenuFlg flgPopupMenu ){

	std::string strPopupMenuName;

	switch( flgPopupMenu ){
	case POPUP_DELETE:
		strPopupMenuName = "&Delete";
		break;
	case POPUP_COPY:
		strPopupMenuName = "&Copy";
		break;
	case POPUP_SAVE:
		strPopupMenuName = "&Save All...";
		break;
	case POPUP_SELECT:
		strPopupMenuName = "Select &All";
		break;
	case POPUP_DESELECT:
		strPopupMenuName = "&Deselect All";
		break;
	case POPUP_FILTER:
		strPopupMenuName = "&Filter...\t(Ctrl+F)";
		break;
	case POPUP_ADD:
		strPopupMenuName = "&Add";
		break;
	case POPUP_UP:
		strPopupMenuName = "&Up";
		break;
	case POPUP_DOWN:
		strPopupMenuName = "&Down";
		break;
	case POPUP_EDIT:
		strPopupMenuName = "&Edit";
		break;
	default:
		break;
	};

	return strPopupMenuName;
};

// set activate name
void GridListCtrl::setActivateName( std::string strActivateName ){
	m_strActivateName = strActivateName;
};

// get activate name
std::string GridListCtrl::getActivateName( void ){
	return m_strActivateName;
};

// on delete
void GridListCtrl::onDelete( wxCommandEvent& evt ) {

	// selected items
	std::vector< int > items;
	ListTool::getSelectedItems( this, items );
	ListTool::unselectAll( this );

	int iItemToSelect = 999999;

	for( int i=items.size()-1; i >= 0; i-- ){
		// Set iItemToSelect to the index of the first item selected.
		if ( items[ i ] < iItemToSelect)
		{
			iItemToSelect = items[ i ];
		}
		deleteData( iItemToSelect, false );
	}

	updateList();	// @date 2012/12/13 <Add> OKADA
	
	// After a deletion, select the item at the same position, or the last item.
	if (iItemToSelect > GetItemCount() - 1)
	{
		iItemToSelect = GetItemCount() - 1;
	}
	if (iItemToSelect >= 0)
	{
		ListTool::selectItem( this, iItemToSelect );
	}
	Refresh();		// @date 2012/12/13 <Add> OKADA

	return;
}

// on activate data
void GridListCtrl::onActivateDataOther( const long long data ) {	// ボタン押下などにより特定行のダブルクリックと同様の処理を追加する為に使います
}
