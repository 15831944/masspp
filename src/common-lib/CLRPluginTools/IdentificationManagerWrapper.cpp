/**
 * @file IdentificationManagerWrapper.cpp
 * @brief implements of IdentificationManagerWrapper class
 *
 * @author S.Tanaka
 * @date 2012.11.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#include "stdafx.h"
#include "IdentificationManagerWrapper.h"

#include "SearchResultWrapper.h"
#include "SearchEngineWrapper.h"
#include "EnzymeWrapper.h"
#include "HitWrapper.h"
#include "ModificationWrapper.h"
#include "ClrSearchEngine.h"
#include "AlignmentWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
IdentificationManagerWrapper::IdentificationManagerWrapper( kome::ident::IdentificationManager& identMgr )
		: m_identMgr( identMgr ) {
}

// destrucotr
IdentificationManagerWrapper::~IdentificationManagerWrapper() {
}

// get identification manger object
kome::ident::IdentificationManager& IdentificationManagerWrapper::getIdentificationManager() {
	return m_identMgr;
}

// clear focused peaks
void IdentificationManagerWrapper::clearFocusedPeaks() {
	m_identMgr.clearFocusedPeaks();
}

// add focused peaks
void IdentificationManagerWrapper::addFocusedPeak( int peakId ) {
	m_identMgr.addFocusedPeak( peakId );
}

// set focused peak
void IdentificationManagerWrapper::setFocusedPeak( int peakId ) {
	m_identMgr.setFocusedPeak( peakId );
}

// activate peak
void IdentificationManagerWrapper::activatePeak( int peakId ) {
	m_identMgr.activatePeak( peakId );
}

// remove focused peak
void IdentificationManagerWrapper::removeFocusedPeak( int peakId ) {
	m_identMgr.removeFocusedPeak( peakId );
}

// get focused peaks
cli::array< int >^ IdentificationManagerWrapper::getFocusedPeaks() {
	// get focused peaks
	std::vector< int > peaks;
	m_identMgr.getFocusedPeaks( peaks );

	// create array
	return makeIntArrayFromVector( peaks );
}

// is focused peak
bool IdentificationManagerWrapper::isFocusedPeak( int peakId ) {
	return m_identMgr.isFocusedPeak( peakId );
}

// write pepXML file
bool IdentificationManagerWrapper::writePepXML( System::String^ path, SearchResultWrapper^ srchResult, bool forTpp, ProgressWrapper^ progress ) {
	return m_identMgr.writePepXML( 
		ClrObjectTool::convertString( path ).c_str(),
		srchResult->getSearchResult(),
		forTpp,
		ClrObjectTool::getProgress( progress )
	);
}

// write tab separated values file
bool IdentificationManagerWrapper::writeTsv( System::String^ path, SearchResultWrapper^ srchResult, ProgressWrapper^ progress ) {
		
	return m_identMgr.writeTsv( 
		ClrObjectTool::convertString( path ).c_str(),
		srchResult->getSearchResult(),
		ClrObjectTool::getProgress( progress )
	);
}

// get the nubmer of local databases
int IdentificationManagerWrapper::getNumberOfLocalDbs() {

	return m_identMgr.getNumberOfLocalDbs();
}

// get local database
System::String^ IdentificationManagerWrapper::getLocalDb( int index ) {

	return ClrObjectTool::convertString( m_identMgr.getLocalDb( index ).c_str(), NULL );
}

// get the number of local database files
int IdentificationManagerWrapper::getNumberOfLocalDbFiles( int index ) {

	return m_identMgr.getNumberOfLocalDbFiles( index );
}

// get local database file name
System::String^ IdentificationManagerWrapper::getLocalDbFileName( int dbIndex, int fileIndex ) {

	return ClrObjectTool::convertString( m_identMgr.getLocalDbFileName( dbIndex, fileIndex ).c_str(), NULL );
}

//  get local database file type
int IdentificationManagerWrapper::getLocalDbFileType( int dbIndex, int fileIndex ) {

	return m_identMgr.getLocalDbFileType( dbIndex, fileIndex );
}

// get local database file path
System::String^ IdentificationManagerWrapper::getLocalDbFilePath( int dbIndex, int fileIndex, DbFileType fileType ) {
	kome::ident::DbFileType type;
	if( fileType == DbFileType::OTHER ){
		type = kome::ident::TYPE_OTHER;

	}else if( fileType == DbFileType::FASTA ){
		type = kome::ident::TYPE_FASTA;

	}else if( fileType == DbFileType::FASTA_PRO ){
		type = kome::ident::TYPE_FASTA_PRO;

	}else if( fileType == DbFileType::BLAST ){
		type = kome::ident::TYPE_BLAST;

	}else{
		type = kome::ident::TYPE_OTHER;
	}

	return ClrObjectTool::convertString( m_identMgr.getLocalDbFilePath( dbIndex, fileIndex, type ).c_str(), NULL );
}

// add public database
int IdentificationManagerWrapper::addPublicDb( System::String^ dbName, System::String^ url ) {

	return m_identMgr.addPublicDb( 
		ClrObjectTool::convertString( dbName ).c_str(),
		ClrObjectTool::convertString( url ).c_str()
	);
}

// set public database URL
void IdentificationManagerWrapper::setPublicUrl( int dbId, System::String^ format, System::String^ url ) {

	m_identMgr.setPublicUrl( 
		dbId, 
		ClrObjectTool::convertString( format ).c_str(),
		ClrObjectTool::convertString( url ).c_str()
	);

}

// register search result
unsigned int IdentificationManagerWrapper::registerResult( SearchResultWrapper^ srchResult, ProgressWrapper^ prog ) {

	// progress
	kome::core::Progress* prgs = ClrObjectTool::getProgress( prog );
	if( prgs == NULL ) {
		prgs = &( kome::core::Progress::getIgnoringProgress() );
	}

	return m_identMgr.registerResult( 
		srchResult->getSearchResult(), 
		*prgs
	);
}

// register search result
unsigned int IdentificationManagerWrapper::registerResult( int searchId, SearchResultWrapper^ srchResult, ProgressWrapper^ prog ) {

	// progress
	kome::core::Progress* prgs = ClrObjectTool::getProgress( prog );
	if( prgs == NULL ) {
		prgs = &( kome::core::Progress::getIgnoringProgress() );
	}

	return m_identMgr.registerResult( 
		searchId,
		srchResult->getSearchResult(), 
		*prgs
	);
}

// load enzymes
void IdentificationManagerWrapper::loadEnzymes() {
	m_identMgr.loadEnzymes();
}

// get number of enzymes
int IdentificationManagerWrapper::getNumberOfEnzymes() {
	return m_identMgr.getNumberOfEnzymes();
}

// get enzyme
EnzymeWrapper^ IdentificationManagerWrapper::getEnzyme( int index ) {
	kome::ident::Enzyme* enz = m_identMgr.getEnzyme( index );
	if( enz == NULL ) {
		return nullptr;
	}

	return gcnew EnzymeWrapper( *enz );
}

// get enzyme from ID
EnzymeWrapper^ IdentificationManagerWrapper::getEnzymeFromId( int id ) {
	kome::ident::Enzyme* enz = m_identMgr.getEnzymeFromId( id );

	if( enz == NULL ) {
		return nullptr;
	}

	return gcnew EnzymeWrapper( *enz );
}

// set all enzymes hidden
void IdentificationManagerWrapper::setAllEnzymeHidden() {
	m_identMgr.setAllEnzymeHidden();
}

// update enzyme
int IdentificationManagerWrapper::updateEnzyme( EnzymeWrapper^ enzyme ) {
	// enzyme
	kome::ident::Enzyme* e = NULL;
	if( enzyme != nullptr ) {
		e = &( enzyme->getEnzyme() );
	}

	// update
	return m_identMgr.updateEnzyme( e );
}

// load modifications
void IdentificationManagerWrapper::loadModifications() {
	m_identMgr.loadModifications();
}

// get number of modifications
int IdentificationManagerWrapper::getNumberOfModifications() {
	return m_identMgr.getNumberOfModifications();
}

// get modification
ModificationWrapper^ IdentificationManagerWrapper::getModification( int index ) {
	kome::ident::Modification* m = m_identMgr.getModification( index );

	if( m == NULL ) {
		return nullptr;
	}

	return gcnew ModificationWrapper( *m );
}

// get modification from ID
ModificationWrapper^ IdentificationManagerWrapper::getModificationFromId( int id ) {
	kome::ident::Modification* m = m_identMgr.getModificationFromId( id );

	if( m == NULL ) {
		return nullptr;
	}

	return gcnew ModificationWrapper( *m );
}

// set all modifications hidden
void IdentificationManagerWrapper::setAllModificationHidden() {
	m_identMgr.setAllModificationHidden();
}

// update modification
int IdentificationManagerWrapper::updateModification( ModificationWrapper^ modification ) {
	// modification
	kome::ident::Modification* m = NULL;
	if( modification != nullptr ) {
		m = &( modification->getModification() );
	}

	// update
	return m_identMgr.updateModification( m );
}

// add search modification
int IdentificationManagerWrapper::addSearchModification( int search, int modification, bool isVariable ) {
	return m_identMgr.addSearchModification( search, modification, isVariable );
}

// get search modifications
cli::array< int >^ IdentificationManagerWrapper::getSearchModifications( int searchId ) {
	// search modifications
	std::vector< int > mods;
	m_identMgr.getSearchModifications( mods, searchId );

	// array
	return makeIntArrayFromVector( mods );
}

// get modification from search modification
int IdentificationManagerWrapper::getModificationFromSearchModification( int searchModId ) {
	return m_identMgr.getModificationFromSearchModification( searchModId );
}

// get search from search modification
int IdentificationManagerWrapper::getSearchFromSearchModification( int searchModId ) {
	return m_identMgr.getSearchFromSearchModification( searchModId );
}

// is variable modification
bool IdentificationManagerWrapper::isVariableModification( int searchModId ) {
	return m_identMgr.isVariableModification( searchModId );
}

// add hit modification
int IdentificationManagerWrapper::addHitModification( int hit, int searchMod, int pos ) {
	return m_identMgr.addHitModification( hit, searchMod, pos );
}

// get hit modifications
cli::array< int >^ IdentificationManagerWrapper::getHitModifications( int hitId ) {
	// modifications
	std::vector< int > mods;
	m_identMgr.getHitModifications( mods, hitId );

	// array
	return makeIntArrayFromVector( mods );
}

// get search modification from hit modification
int IdentificationManagerWrapper::getSearchModificationFromHitModification( int hitMod ) {
	return m_identMgr.getSearchModificationFromHitModification( hitMod );
}

// get hit from hit modification
int IdentificationManagerWrapper::getHitFromHitModification( int hitMod ) {
	return m_identMgr.getHitFromHitModification( hitMod );
}

// get hit modification position
int IdentificationManagerWrapper::getHitModPosition( int hitMod ) {
	return m_identMgr.getHitModPosition( hitMod );
}

// parse spectrum title
bool IdentificationManagerWrapper::ParseSpectrumTitle(System::String^ spectrumTitle, HitWrapper::SpecInfo^ spectrumInfo, int% charge) {
	
	kome::ident::SpecInfo* specInfo = new kome::ident::SpecInfo();
	HitWrapper::copySpectrum( spectrumInfo, *specInfo );
	int c = 0;
		
	bool bflg =  m_identMgr.ParseSpectrumTitle( 
		              ClrObjectTool::convertString( spectrumTitle ).c_str(),
		              *specInfo,
		              &c
	             );
	 
	charge = c;

	return bflg;
}

// get database home
System::String^ IdentificationManagerWrapper::getDbHome() {

	return ClrObjectTool::convertString( m_identMgr.getDbHome(), NULL );
}

// get warning code (insufficient title)
unsigned int IdentificationManagerWrapper::InsufficientTitleTagsWarningCode() {

	return m_identMgr.InsufficientTitleTagsWarningCode();
}

// get warning code (no valid title tags)
unsigned int IdentificationManagerWrapper::NoValidTitleTagsWarningCode() {

	return m_identMgr.NoValidTitleTagsWarningCode();
}

// get warning code (no valid hit spectra)
unsigned int IdentificationManagerWrapper::NoValidHitSpectraWarningCode() {

	return m_identMgr.NoValidHitSpectraWarningCode();
}

// get warning code (search result addition)
unsigned int IdentificationManagerWrapper::SearchResultAdditionWarningCode() {

	return m_identMgr.SearchResultAdditionWarningCode();
}

// add search engine
void IdentificationManagerWrapper::addSearchEngine( SearchEngineWrapper^ pSrchEngine ) {
	m_identMgr.addSearchEngine( ( pSrchEngine == nullptr ? NULL : &( pSrchEngine->getSearchEngine() ) ) );
}

// get number of search engines
int IdentificationManagerWrapper::getNumberOfSearchEngines() {

	return m_identMgr.getNumberOfSearchEngines();
}

// get search engine
SearchEngineWrapper^ IdentificationManagerWrapper::getSearchEngine( int index ) {

	kome::ident::SearchEngine* search = m_identMgr.getSearchEngine( index );
	if( search != NULL ){
		return gcnew SearchEngineWrapper( *search );
	}

	return nullptr;
}

// remove search engine
bool IdentificationManagerWrapper::removeSearchEngine( SearchEngineWrapper^ pSrchEngine ) {

	return m_identMgr.removeSearchEngine( ( pSrchEngine == nullptr ? NULL : &( pSrchEngine->getSearchEngine() ) ) );
}

// commit database
void IdentificationManagerWrapper::commit() {

	m_identMgr.commit();
}

// rollback database
void IdentificationManagerWrapper::rollback() {

	m_identMgr.rollback();
}

// add matrix
int IdentificationManagerWrapper::addMatrix(
		System::String^ name,
		System::String^ comment,
		bool rtFlg,
		bool mzFlg,
		bool chargeFlg
) {

	return m_identMgr.addMatrix( 
		ClrObjectTool::convertString( name ).c_str(),
		ClrObjectTool::convertString( comment ).c_str(),
		rtFlg,
		mzFlg,
		chargeFlg 
	);
}

// delete matrix
void IdentificationManagerWrapper::deleteMatrix( int id ) {

	m_identMgr.deleteMatrix( id );
}

// get selected matrix
int IdentificationManagerWrapper::getSelectedMatrix() {
	return m_identMgr.getSelectedMatrix();
}

// select matrix
void IdentificationManagerWrapper::selectMatrix( int id ) {
	m_identMgr.selectMatrix( id );
}

// update matrix flags
void IdentificationManagerWrapper::updateMatrixFlags( int id, bool rtFlg, bool mzFlg, bool chargeFlg ) {

	m_identMgr.updateMatrixFlags( id, rtFlg, mzFlg, chargeFlg );

}

// update matrix
void IdentificationManagerWrapper::updateMatrix( int id, System::String^ name, System::String^ comment ) {

	m_identMgr.updateMatrix( 
		id, 
		ClrObjectTool::convertString( name ).c_str(),
		ClrObjectTool::convertString( comment ).c_str() );
}

// get matrices
cli::array< int >^ IdentificationManagerWrapper::getMatrices() {

	std::vector< int > matrices;
	m_identMgr.getMatrices( matrices );

	return makeIntArrayFromVector( matrices );
}

// get matrix name
System::String^ IdentificationManagerWrapper::getMatrixName( int id ) {

	return ClrObjectTool::convertString( m_identMgr.getMatrixName( id ).c_str(), NULL );
}

// get matrix comment
System::String^ IdentificationManagerWrapper::getMatrixComment( int id ) {

	return ClrObjectTool::convertString( m_identMgr.getMatrixComment( id ).c_str(), NULL );
}

// get matrix flags
void IdentificationManagerWrapper::getMatrixFlags( int id, bool% rtFlg, bool% mzFlg, bool% chargeFlg ) {

	bool brtFlg = false;
	bool bmzFlg = false;
	bool bchargeFlg = false;

	m_identMgr.getMatrixFlags( 
		id, 
		&brtFlg,
		&bmzFlg,
		&bchargeFlg
	);

	rtFlg = brtFlg;
	mzFlg = bmzFlg;
	chargeFlg = bchargeFlg;
}

// add group
int IdentificationManagerWrapper::addGroup( System::String^ name, int matrix, System::Drawing::Color color ) {

	COLOR c = kome::img::ColorTool::getColor(
		color.R,
		color.G,
		color.B
	);
	return m_identMgr.addGroup(
		ClrObjectTool::convertString( name ).c_str(),
		matrix,
		c
	);
}

// delete group
void IdentificationManagerWrapper::deleteGroup( int id ) {

	m_identMgr.deleteGroup( id );

}

// update group
void IdentificationManagerWrapper::updateGroup( int id, System::String^ name, System::Drawing::Color color ) {

	COLOR c = kome::img::ColorTool::getColor(
		color.R,
		color.G,
		color.B
	);

	m_identMgr.updateGroup( 
		id,
		ClrObjectTool::convertString( name ).c_str(),
		c
	);

}

// set group order
void IdentificationManagerWrapper::setGroupOrder( int id, int order ) {

	m_identMgr.setGroupOrder( id, order );
}

// get groups
cli::array< int >^ IdentificationManagerWrapper::getGroups( int matrixId ) {
	// groups
	std::vector< int > groups;
	m_identMgr.getGroups( groups, matrixId );

	// array
	return makeIntArrayFromVector( groups );
}

// get matrix from group
int IdentificationManagerWrapper::getGroupMatrix( int id ) {

	return m_identMgr.getGroupMatrix( id );
}

// get group name
System::String^ IdentificationManagerWrapper::getGroupName( int id ) {
	return ClrObjectTool::convertString( m_identMgr.getGroupName( id ).c_str(), NULL );
}

// get group color
System::Drawing::Color IdentificationManagerWrapper::getGroupColor( int id ) {
	// color
	COLOR c = m_identMgr.getGroupColor( id );
	System::Drawing::Color ret = System::Drawing::Color::FromArgb(
		0xff,
		kome::img::ColorTool::getRed( c ),
		kome::img::ColorTool::getGreen( c ),
		kome::img::ColorTool::getBlue( c )
	);

	return ret;
}

// set standard sample
void IdentificationManagerWrapper::setStandardSample( int matrixId, int  matrixSampleId ) {

	m_identMgr.setStandardSample( matrixId, matrixSampleId );
}

// get standard sample
int IdentificationManagerWrapper::getStandardSample( int matrixId ) {

	return m_identMgr.getStandardSample( matrixId );
}

// clear normalization
void IdentificationManagerWrapper::clearNormalization( int matrix ) {

	m_identMgr.clearNormalization( matrix );
}

// set normalization filter
void IdentificationManagerWrapper::setNormalizationFilter(
		int matrix,
		double minInt,
		double maxInt,
		bool relFlg,
		double minRt,
		double maxRt,
		double minMz,
		double maxMz,
		System::String^ method,
		System::String^ params
) {

	m_identMgr.setNormalizationFilter( 
		matrix,
		minInt,
		maxInt,
		relFlg,
		minRt,
		maxRt,
		minMz,
		maxMz,
		ClrObjectTool::convertString( method ).c_str(),
		ClrObjectTool::convertString( params ).c_str()
	);

}

// get normalization filter
IdentificationManagerWrapper::NormalizationFilterInfo^ IdentificationManagerWrapper::getNormalizationFilter( int matrix ) {

	double minInt = 0.0;
	double maxInt = 0.0;
	bool relFlg;
	double minRt = 0.0;
	double maxRt = 0.0;
	double minMz = 0.0;
	double maxMz = 0.0;
	std::string method;
	std::string params;

	m_identMgr.getNormalizationFilter( matrix, &minInt, &maxInt, &relFlg, &minRt, &maxRt, &minMz, &maxMz, method, params );
	
	IdentificationManagerWrapper::NormalizationFilterInfo^ filterInfo = gcnew IdentificationManagerWrapper::NormalizationFilterInfo();
	filterInfo->minInt = minInt;
	filterInfo->maxInt = maxInt;
	filterInfo->relFlg = relFlg;
	filterInfo->minRt;
	filterInfo->maxRt;
	filterInfo->minMz;
	filterInfo->maxMz;
	filterInfo->method = ClrObjectTool::convertString( method.c_str(), NULL );
	filterInfo->params = ClrObjectTool::convertString( params.c_str(), NULL );

	return filterInfo;
}

// add normalization method
void IdentificationManagerWrapper::addNormalizationMethod(
		int matrix,
		System::String^ method,
		System::String^ params
) {

	m_identMgr.addNormalizationMethod(
		matrix,
		ClrObjectTool::convertString( method ).c_str(),
		ClrObjectTool::convertString( params ).c_str()
	);
}

// get normalization methods
cli::array< IdentificationManagerWrapper::NormalizationMethodInfo^ >^ IdentificationManagerWrapper::getNormalizationMethods( int matrix ) {

	std::vector< kome::ident::NormalizationMethodInfo > info;
	m_identMgr.getNormalizationMethods( matrix, info );

	if( info.size() == 0 ){
		return nullptr;
	}

	// array
	cli::array< IdentificationManagerWrapper::NormalizationMethodInfo^ >^ m = gcnew cli::array< IdentificationManagerWrapper::NormalizationMethodInfo^ >( info.size() );
	for( unsigned int i=0; i < info.size(); i++ ){
		IdentificationManagerWrapper::NormalizationMethodInfo^ methods = gcnew IdentificationManagerWrapper::NormalizationMethodInfo();
		methods->method = ClrObjectTool::convertString( info[i].method.c_str(), NULL );
		methods->params = ClrObjectTool::convertString( info[i].params.c_str(), NULL );

		m[i] = methods;
	}

	return m;
}

// set alignment
void IdentificationManagerWrapper::setAlignment( int matrix, System::String^ method, System::String^ params ) {
	m_identMgr.setAlignment(
		matrix,
		ClrObjectTool::convertString( method ).c_str(),
		ClrObjectTool::convertString( params ).c_str()
	);
}

// get alignment
AlignmentWrapper^ IdentificationManagerWrapper::getAlignment( int matrix ) {
	kome::operation::Alignment* align = m_identMgr.getAlignment( matrix );
	if( align == NULL ) {
		return nullptr;
	}

	return gcnew AlignmentWrapper( *align );
}

// add matrix column
int IdentificationManagerWrapper::addMatrixColumn( System::String^ name, ColumnDataType type, int matrixId ) {

	kome::ident::ColumnDataType columType;
	if( type == ColumnDataType::INT ){
		columType = kome::ident::TYPE_INT;

	}else if( type == ColumnDataType::UINT ){
		columType = kome::ident::TYPE_UINT;

	}else if( type == ColumnDataType::DOUBLE ){
		columType = kome::ident::TYPE_DOUBLE;

	}else if( type == ColumnDataType::UDOUBLE ){
		columType = kome::ident::TYPE_UDOUBLE;

	}else if( type == ColumnDataType::STRING ){
		columType = kome::ident::TYPE_STRING;
	}

	return m_identMgr.addMatrixColumn( 
		ClrObjectTool::convertString( name ).c_str(),
		columType,
		matrixId );
}



// delete matrix column
void IdentificationManagerWrapper::deleteMatrixColumn( int id ) {

	m_identMgr.deleteMatrixColumn( id );
}

// update matrix column
void IdentificationManagerWrapper::updateMatrixColumn( int id, System::String^ name, ColumnDataType type ) {

	kome::ident::ColumnDataType columType;
	if( type == ColumnDataType::INT ){
		columType = kome::ident::TYPE_INT;

	}else if( type == ColumnDataType::UINT ){
		columType = kome::ident::TYPE_UINT;

	}else if( type == ColumnDataType::DOUBLE ){
		columType = kome::ident::TYPE_DOUBLE;

	}else if( type == ColumnDataType::UDOUBLE ){
		columType = kome::ident::TYPE_UDOUBLE;

	}else if( type == ColumnDataType::STRING ){
		columType = kome::ident::TYPE_STRING;
	}

	m_identMgr.updateMatrixColumn( id, ClrObjectTool::convertString( name ).c_str(), columType );
}

// get matrix columns
cli::array< int >^ IdentificationManagerWrapper::getMatrixColumns( int matrixId ) {

	std::vector< int > columns;
	m_identMgr.getMatrixColumns( columns, matrixId );

	return makeIntArrayFromVector( columns );
}

// get matrix colunn name
System::String^ IdentificationManagerWrapper::getMatrixColumnName( int id ) {

	return ClrObjectTool::convertString( m_identMgr.getMatrixColumnName( id ).c_str(), NULL );
}

// get matrix column type
IdentificationManagerWrapper::ColumnDataType IdentificationManagerWrapper::getMatrixColumnType( int id ) {

	kome::ident::ColumnDataType type = m_identMgr.getMatrixColumnType( id );
	
	ColumnDataType columType;
	if( type == kome::ident::TYPE_INT ){
		columType = ColumnDataType::INT;

	}else if( type == kome::ident::TYPE_UINT ){
		columType = ColumnDataType::UINT;

	}else if( type == kome::ident::TYPE_DOUBLE ){
		columType = ColumnDataType::DOUBLE;

	}else if( type == kome::ident::TYPE_UDOUBLE ){
		columType = ColumnDataType::UDOUBLE;

	}else if( type == kome::ident::TYPE_STRING ){
		columType = ColumnDataType::STRING;
	}

	return columType;
}

// add matrix peak
int IdentificationManagerWrapper::addMatrixPeak( double rt, double mz, int charge, int matrixId ) {

	return m_identMgr.addMatrixPeak( rt, mz, charge, matrixId );
}

// delete matrix peak
void IdentificationManagerWrapper::deleteMatrixPeak( int id ) {

	m_identMgr.deleteMatrixPeak( id );
}

// update matrix peak
void IdentificationManagerWrapper::updateMatrixPeak( int id, double rt, double mz, int charge ) {

	m_identMgr.updateMatrixPeak( id, rt, mz, charge );

}

// get matrix peaks
cli::array< int >^ IdentificationManagerWrapper::getMatrixPeaks( int matrixId ) {

	std::vector< int > peaks;
	m_identMgr.getMatrixPeaks( peaks, matrixId );

	return makeIntArrayFromVector( peaks );
}

// get matrix peak RT
double IdentificationManagerWrapper::getMatrixPeakRt( int id ) {

	return m_identMgr.getMatrixPeakRt( id );
}

// get matrix peak m/z
double IdentificationManagerWrapper::getMatrixPeakMz( int id ) {

	return m_identMgr.getMatrixPeakMz( id );
}

// get matrix peak charge
int IdentificationManagerWrapper::getMatrixPeakCharge( int id ) {

	return m_identMgr.getMatrixPeakCharge( id );
}

// add sample set
int IdentificationManagerWrapper::addSampleSet( System::String^ path ) {
	
	return m_identMgr.addSampleSet( ClrObjectTool::convertString( path ).c_str() );

}

// get sample set
int IdentificationManagerWrapper::getSampleSet( System::String^ path ) {
	return m_identMgr.getSampleSet( ClrObjectTool::convertString( path ).c_str() );
}

// add sample
int IdentificationManagerWrapper::addSample( SampleWrapper^ sample ) {

	kome::objects::Sample* s = ClrObjectTool::getSample( sample );
	return m_identMgr.addSample( s );
}

// add sample
int IdentificationManagerWrapper::addSample( System::String^ path, int sampleIndex, System::String^ name ) {
	return m_identMgr.addSample(
		ClrObjectTool::convertString( path ).c_str(),
		sampleIndex,
		ClrObjectTool::convertString( name ).c_str() );
}

// get sample
int IdentificationManagerWrapper::getSample( SampleWrapper^ sample ) {
	kome::objects::Sample* s = ClrObjectTool::getSample( sample );
	return m_identMgr.getSample( s );
}

// get sample
int IdentificationManagerWrapper::getSample( System::String^ path, int sampleIndex ) {
	return m_identMgr.getSample(
		ClrObjectTool::convertString( path ).c_str(),
		sampleIndex
	);
}

// delete sample
void IdentificationManagerWrapper::deleteSample( int id ) {

	m_identMgr.deleteSample( id );
}

// get sample name
System::String^ IdentificationManagerWrapper::getSampleName( int sampleId ) {

	return ClrObjectTool::convertString( m_identMgr.getSampleName( sampleId ).c_str(), NULL );
}

// get sample file path
System::String^ IdentificationManagerWrapper::getSampleFilePath( int sampleId ) {

	return ClrObjectTool::convertString( m_identMgr.getSampleFilePath( sampleId ).c_str(), NULL );
}

// get sample index
int IdentificationManagerWrapper::getSampleIndex( int sampleId ) {

	return m_identMgr.getSampleIndex( sampleId );
}

// open sample
SampleWrapper^ IdentificationManagerWrapper::openSample( int id, SampleReadInfo^ readInfo ) {
	if( readInfo == nullptr ){
		return nullptr; 
	}
		
	kome::ident::SampleReadInfo* rinfo = new kome::ident::SampleReadInfo();
	copySampleInfo( readInfo, *rinfo );
	
	kome::objects::Sample* s = m_identMgr.openSample( id, *rinfo );
	if( s != NULL ){
		return	ClrObjectTool::createSampleWrapper( s );
	}

	return nullptr; 
}

// release sample
void IdentificationManagerWrapper::releaseSample( SampleWrapper^ sample, SampleReadInfo^ readInfo ) {
	if( readInfo == nullptr ){
		return; 
	}

	kome::objects::Sample* s = ClrObjectTool::getSample( sample );
	kome::ident::SampleReadInfo* rinfo = new kome::ident::SampleReadInfo();
	copySampleInfo( readInfo, *rinfo );

	if( s != NULL ){
		m_identMgr.releaseSample( s, *rinfo );
	}
}

// add matrix sample
int IdentificationManagerWrapper::addMatrixSample(
		int group,
		int sample,
		System::String^ name,
		System::Drawing::Color color
) {
	COLOR c = kome::img::ColorTool::getColor(
		color.R,
		color.G,
		color.B
	);

	return m_identMgr.addMatrixSample(
		group,
		sample,
		ClrObjectTool::convertString( name ).c_str(),
		c 
	);

}

// delete matrix sample
void IdentificationManagerWrapper::deleteMatrixSample( int id ) {

	m_identMgr.deleteMatrixSample( id );
}

// update matrix sample
void IdentificationManagerWrapper::updateMatrixSample( int id, System::String^ name, System::Drawing::Color color ) {
	COLOR c = kome::img::ColorTool::getColor(
		color.R,
		color.G,
		color.B
	);

	m_identMgr.updateMatrixSample( 
		id,
		ClrObjectTool::convertString( name ).c_str(),
		c
	);

}
// get matrix samples
cli::array< int >^ IdentificationManagerWrapper::getMatrixSamplesFromGroup( int group ) {
	// samples
	std::vector< int > samples;
	m_identMgr.getMatrixSamplesFromGroup( samples, group );

	// array
	return makeIntArrayFromVector( samples );
}

// get matrix samples
cli::array< int >^ IdentificationManagerWrapper::getMatrixSamplesFromSample( int sample ) {

	// samples
	std::vector< int > matrixSamples;
	m_identMgr.getMatrixSamplesFromSample( matrixSamples, sample );

	return makeIntArrayFromVector( matrixSamples );
}

// get sample ID
int IdentificationManagerWrapper::getMatrixSampleSample( int id ) {

	return m_identMgr.getMatrixSampleSample( id );
}

// get group ID from matrix sample
int IdentificationManagerWrapper::getMatrixSampleGroup( int id ) {

	return m_identMgr.getMatrixSampleGroup( id );
}

// get matrix sample name
System::String^ IdentificationManagerWrapper::getMatrixSampleName( int id ) {
	return ClrObjectTool::convertString( m_identMgr.getMatrixSampleName( id ).c_str(), NULL );
}

// get matrix sample color
System::Drawing::Color IdentificationManagerWrapper::getMatrixSampleColor( int id ) {
	// color
	COLOR c = m_identMgr.getMatrixSampleColor( id );
	System::Drawing::Color ret = System::Drawing::Color::FromArgb(
		0xff,
		kome::img::ColorTool::getRed( c ),
		kome::img::ColorTool::getGreen( c ),
		kome::img::ColorTool::getBlue( c )
	);

	return ret;
}

// set appended value
void IdentificationManagerWrapper::setAppendedValue( int peak, int col, System::String^ val ) {

	m_identMgr.setAppendedValue( 
		peak,
		col,
		ClrObjectTool::convertString( val ).c_str()
	);
}

// get appended value
System::String^ IdentificationManagerWrapper::getAppendedValue( int peak, int col ) {

	return ClrObjectTool::convertString( m_identMgr.getAppendedValue( peak, col ).c_str(), NULL );
}

// delete appended values
void IdentificationManagerWrapper::deleteAppendedValueFromPeak( int peak ) {

	m_identMgr.deleteAppendedValueFromPeak( peak );
}

// delete appended values
void IdentificationManagerWrapper::deleteAppendedValueFromMatrixColumn( int col ) {

	m_identMgr.deleteAppendedValueFromMatrixColumn( col );
}

// set profile
int IdentificationManagerWrapper::setProfile( XYDataWrapper^ data, bool isChrom ) {
	kome::core::XYData* xyd = ClrObjectTool::getXYData( data );

	return m_identMgr.setProfile( *xyd, isChrom );
}

// delete profile
void IdentificationManagerWrapper::deleteProfile( int id ) {

	m_identMgr.deleteProfile( id );
}

// get profile
void IdentificationManagerWrapper::getProfile( int id, XYDataWrapper^ data ) {
	kome::core::XYData* xyd = ClrObjectTool::getXYData( data );
	m_identMgr.getProfile( id, *xyd );
}

// is chromatogram profile
bool IdentificationManagerWrapper::isChromatogramProfile( int id ) {

	return m_identMgr.isChromatogramProfile( id );
}

// set peak value
void IdentificationManagerWrapper::setPeakValue(
		int matrixPeak,
		int matrixSample,
		double intensity,
		PeakElementWrapper^ peak,
		int profile
 ) {

	kome::objects::PeakElement* p = NULL;
	if( peak != nullptr ){
		p = ClrObjectTool::getPeakElement( peak );
	}
	
	m_identMgr.setPeakValue( matrixPeak, matrixSample, intensity, p, profile );
} 

// set normalized intensity
void IdentificationManagerWrapper::setNormalizedIntensity(
		int matrixPeak,
		int matrixSample,
		double normalized
) {

	m_identMgr.setNormalizedIntensity( matrixPeak, matrixSample, normalized );
}

// delete peak values
void IdentificationManagerWrapper::deletePeakValuesFromMatrixPeak( int matrixPeak ) {

	m_identMgr.deletePeakValuesFromMatrixPeak( matrixPeak );
}

// delete peak values
void IdentificationManagerWrapper::deletePeakValuesFromMatrixSample( int matrixSample ) {

	m_identMgr.deletePeakValuesFromMatrixSample( matrixSample );
}

// get peak value
double IdentificationManagerWrapper::getPeakValue( int matrixPeak, int matrixSample ) {
	return m_identMgr.getPeakValue( matrixPeak, matrixSample );
}

// get normalized peak value
double IdentificationManagerWrapper::getNormalizedPeakValue( int matrixPeak, int matrixSample ) {
	return m_identMgr.getNormalizedPeakValue( matrixPeak, matrixSample );
}

// has blank value
bool IdentificationManagerWrapper::hasBlankValue( int matrixId ) {

	return m_identMgr.hasBlankValue( matrixId );
}

// is empty sample
bool IdentificationManagerWrapper::isEmptySample( int matrixSample ) {

	return m_identMgr.isEmptySample( matrixSample );
}

// get peak information
void IdentificationManagerWrapper::getPeakInfo(
	int matrixPeak,
	int matrixSample,
	PeakElementWrapper^ peak	
) {
	kome::objects::PeakElement* p = ClrObjectTool::getPeakElement( peak );

	m_identMgr.getPeakInfo( matrixPeak, matrixSample, p );
}

// get profile
int IdentificationManagerWrapper::getProfile( int matrixPeak, int matrixSample ) {

	return m_identMgr.getProfile( matrixPeak, matrixSample );
}


// get spectrum information
HitWrapper::SpecInfo^ IdentificationManagerWrapper::getSpecInfo( SpectrumWrapper^ spec ) {
	// get spectrum
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );

	// spectrum
	kome::ident::SpecInfo sInfo;
	m_identMgr.getSpecInfo( *s, sInfo );

	// copy
	HitWrapper::SpecInfo^ specInfo = HitWrapper::copySpectrum( &sInfo );

	return specInfo;
}

// add spectrum
int IdentificationManagerWrapper::addSpectrum( SpectrumWrapper^ spec ) {
	// get spectrum
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );

	return m_identMgr.addSpectrum( s );
}

// add spectrum
int IdentificationManagerWrapper::addSpectrum( HitWrapper::SpecInfo^ spec ) {

	// spectrum
	kome::ident::SpecInfo* sInfo = new kome::ident::SpecInfo();
	HitWrapper::copySpectrum( spec, *sInfo );

	return m_identMgr.addSpectrum( *sInfo );
}

// delete spectrum
void IdentificationManagerWrapper::deleteSpectrum( int id ) {

	m_identMgr.deleteSpectrum( id );
}

// get spectra
cli::array< int >^ IdentificationManagerWrapper::getSpectra( int sampleId ) {

	std::vector< int > spectra;
	m_identMgr.getSpectra( spectra, sampleId );

	// create array
	return makeIntArrayFromVector( spectra );
}

// get spectrum
HitWrapper::SpecInfo^ IdentificationManagerWrapper::getSpectrum( int id ) {
	// spectrum
	kome::ident::SpecInfo sInfo;
	m_identMgr.getSpectrum( id, sInfo );

	HitWrapper::SpecInfo^ spec = HitWrapper::copySpectrum( &sInfo );

	return spec;
}

// add database
int IdentificationManagerWrapper::addDatabase( System::String^ name, System::String^ url ) {

	return m_identMgr.addDatabase(
		ClrObjectTool::convertString( name ).c_str(),
		ClrObjectTool::convertString( url ).c_str()
	);
}

// get database ID
int IdentificationManagerWrapper::getDatabaseId( System::String^ name ) {

	return m_identMgr.getDatabaseId( ClrObjectTool::convertString( name ).c_str() );
}

// delete database
void IdentificationManagerWrapper::deleteDatabase( int id ) {

	m_identMgr.deleteDatabase( id );
}

// get databases
cli::array< int >^ IdentificationManagerWrapper::getDatabases() {

	std::vector< int >db;
	m_identMgr.getDatabases( db );

	// create array
	return makeIntArrayFromVector( db );
}

// get database name
System::String^ IdentificationManagerWrapper::getDatabaseName( int id ) {

	return ClrObjectTool::convertString( m_identMgr.getDatabaseName( id ).c_str(), NULL );
}

// get database URL
System::String^ IdentificationManagerWrapper::getDatabaseUrl( int id ) {

	return ClrObjectTool::convertString( m_identMgr.getDatabaseUrl( id ).c_str(), NULL );
}

// set format URL
void IdentificationManagerWrapper::setFormatUrl( int dbId, System::String^ format, System::String^ url ) {

	m_identMgr.setFormatUrl(
		dbId,
		ClrObjectTool::convertString( format ).c_str(),
		ClrObjectTool::convertString( url ).c_str()
	);
}

// delete format
void IdentificationManagerWrapper::deleteFormat( System::String^ format ) {

	m_identMgr.deleteFormat( ClrObjectTool::convertString( format ).c_str() );
}

// delte format URL
void IdentificationManagerWrapper::deleteFormatUrl( int dbId, System::String^ format ) {

	m_identMgr.deleteFormatUrl( 
		dbId,
		ClrObjectTool::convertString( format ).c_str()
	);
}

// delete format URL
void IdentificationManagerWrapper::deleteFormatUrl( int dbId ) {

	m_identMgr.deleteFormatUrl( dbId );
}

// get format URL
System::String^ IdentificationManagerWrapper::getFormatUrl( int dbId, System::String^ format ) {
	
	return ClrObjectTool::convertString( 
		m_identMgr.getFormatUrl( dbId, ClrObjectTool::convertString( format ).c_str() ).c_str(),
		NULL
	);
}

// get format list
cli::array< System::String^ >^ IdentificationManagerWrapper::getFormatList( int dbId ) {

	std::vector< std::string > strformat;
	m_identMgr.getFormatList( strformat, dbId );

	if( strformat.size() != 0 ){
		// array
		cli::array< System::String^ >^ s = gcnew cli::array< System::String^ >( strformat.size() );
		for( unsigned int i=0; i < strformat.size(); i++ ){
			s[ i ] = ClrObjectTool::convertString( strformat[ i ].c_str(), NULL );
		}

		return s;
	}

	return nullptr;
}

// add substance
int IdentificationManagerWrapper::addSubstance( SubstanceInfo^ substance ) {
	kome::ident::SubstanceInfo* info = new kome::ident::SubstanceInfo();
	copySubstance( substance, *info );
	
	return m_identMgr.addSubstance( *info );
}

// get substance ID
int IdentificationManagerWrapper::getSubstanceId(
		int dbId,
		System::String^ accession,
		System::String^ name,
		int parentId
) {

	return m_identMgr.getSubstanceId( 
		dbId,
		ClrObjectTool::convertString( accession ).c_str(),
		ClrObjectTool::convertString( name ).c_str(),
		parentId
	);
}

// delete substance
void IdentificationManagerWrapper::deleteSubstance( int id ) {

	m_identMgr.deleteSubstance( id );
}

// get substances
cli::array< int >^ IdentificationManagerWrapper::getSubstances( int dbId ) {

	std::vector< int > substances;
	m_identMgr.getSubstances( substances, dbId );

	// create array
	return makeIntArrayFromVector( substances );
}

// get substances
cli::array< int >^ IdentificationManagerWrapper::getSubstances() {

	std::vector< int > subtrance;
	m_identMgr.getSubstances( subtrance );

	// create array
	return makeIntArrayFromVector( subtrance );
}

// get substance
IdentificationManagerWrapper::SubstanceInfo^ IdentificationManagerWrapper::getSubstance( int id ) {
	kome::ident::SubstanceInfo info;
	m_identMgr.getSubstance( id, info );

	return copySubstance( &info );
}

// add search engine
int IdentificationManagerWrapper::addSearchEngine( System::String^ name, System::String^ version ) {

	return m_identMgr.addSearchEngine( 
		ClrObjectTool::convertString( name ).c_str(),
		ClrObjectTool::convertString( version ).c_str()
	);
}

// delete search engine
void IdentificationManagerWrapper::deleteSearchEngine( int id ) {
	m_identMgr.deleteSearchEngine( id );
}

// get search engines
cli::array< int >^ IdentificationManagerWrapper::getSearchEngines() {

	std::vector< int > engines;
	m_identMgr.getSearchEngines( engines );

	// create array
	return makeIntArrayFromVector( engines );
}

// get search engine name
System::String^ IdentificationManagerWrapper::getSearchEngineName( int id ) {

	return ClrObjectTool::convertString( m_identMgr.getSearchEngineName( id ).c_str(), NULL );
}

// get search engine version
System::String^ IdentificationManagerWrapper::getSearchEngineVersion( int id ) {
	return ClrObjectTool::convertString( m_identMgr.getSearchEngineVersion( id ).c_str(), NULL );
}

// add peak detection
int IdentificationManagerWrapper::addPeakDetection( System::String^ method ) {
	return m_identMgr.addPeakDetection( ClrObjectTool::convertString( method ).c_str() );
}

// set peak parameter
void IdentificationManagerWrapper::setPeakParameter( int detect, System::String^ name, System::String^ value ) {
	m_identMgr.setPeakParameter(
		detect,
		ClrObjectTool::convertString( name ).c_str(),
		ClrObjectTool::convertString( value ).c_str()
	);
}

// delete peak detection
void IdentificationManagerWrapper::deletePeakDetection( int detect ) {
	m_identMgr.deletePeakDetection( detect );
}

// get peak detection method
System::String^ IdentificationManagerWrapper::getPeakDetectionMethod( int detect ) {
	return ClrObjectTool::convertString(
		m_identMgr.getPeakDetectionMethod( detect ).c_str(),
		NULL
	);
}

// get peak detection parameters
PropertiesWrapper^ IdentificationManagerWrapper::getPeakDetectionParameters( int detect ) {
	// properties
	PropertiesWrapper^ props = gcnew PropertiesWrapper();

	m_identMgr.getPeakDetectionParameters(
		detect,
		*( ClrObjectTool::getProperties( props ) )
	);

	return props;
}

// delete unlinked peak detection
void IdentificationManagerWrapper::deleteUnlinkedPeakDetection() {
	m_identMgr.deleteUnlinkedPeakDetection();
}

// set peaks
int IdentificationManagerWrapper::setPeaks( XYDataWrapper^ peaks, int peakDetection ) {
	kome::core::XYData* xyData = ClrObjectTool::getXYData( peaks );
	return m_identMgr.setPeaks( xyData, peakDetection );
}

// get peaks
int IdentificationManagerWrapper::getPeaks( int detect, XYDataWrapper^ peaks ) {
	kome::core::XYData* xyData = ClrObjectTool::getXYData( peaks );
	return m_identMgr.getPeaks( detect, xyData );
}

// delete peaks
void IdentificationManagerWrapper::deletePeaks( int peaks ) {
	m_identMgr.deletePeaks( peaks );
}

// delete unlinked peaks
void IdentificationManagerWrapper::deleteUnlinkedPeaks() {
	m_identMgr.deleteUnlinkedPeaks();
}

// add search
int IdentificationManagerWrapper::addSearch( SearchEngineWrapper::SearchInfo^ search ) {

	kome::ident::SearchInfo* info = new kome::ident::SearchInfo();
	SearchEngineWrapper::copySearchInfo( search, *info );

	return m_identMgr.addSearch( *info );
}

// delete search
void IdentificationManagerWrapper::deleteSearch( int id ) {

	m_identMgr.deleteSearch( id );
}

// update search
void IdentificationManagerWrapper::updateSearch( int id, SearchEngineWrapper::SearchInfo^ search ) {

	kome::ident::SearchInfo* info = new kome::ident::SearchInfo();
	SearchEngineWrapper::copySearchInfo( search, *info );

	m_identMgr.updateSearch( id, *info );
}

// set search status
void IdentificationManagerWrapper::setSearchStatus( int id, SearchEngineWrapper::SearchStatus status ) {
	kome::ident::SearchEngine::SearchStatus s = kome::ident::SearchEngine::STATUS_UNKNOWN;
	if( status == SearchEngineWrapper::SearchStatus::WAITING ) {
		s = kome::ident::SearchEngine::STATUS_WAITING;
	}
	else if( status == SearchEngineWrapper::SearchStatus::RUNNING ) {
		s = kome::ident::SearchEngine::STATUS_RUNNING;
	}
	else if( status == SearchEngineWrapper::SearchStatus::COMPLETED ) {
		s = kome::ident::SearchEngine::STATUS_COMPLETED;
	}
	else if( status == SearchEngineWrapper::SearchStatus::ERROR ) {
		s = kome::ident::SearchEngine::STATUS_ERROR;
	}

	m_identMgr.setSearchStatus( id, s );
}

// get search status
SearchEngineWrapper::SearchStatus IdentificationManagerWrapper::getSearchStatus( int id ) {
	kome::ident::SearchEngine::SearchStatus s = m_identMgr.getSearchStatus( id );

	SearchEngineWrapper::SearchStatus ret = SearchEngineWrapper::SearchStatus::UNKNOWN;
	if( s == kome::ident::SearchEngine::STATUS_WAITING ) {
		ret = SearchEngineWrapper::SearchStatus::WAITING;
	}
	else if( s == kome::ident::SearchEngine::STATUS_RUNNING ) {
		ret = SearchEngineWrapper::SearchStatus::RUNNING;
	}
	else if( s == kome::ident::SearchEngine::STATUS_COMPLETED ) {
		ret = SearchEngineWrapper::SearchStatus::COMPLETED;
	}
	else if( s == kome::ident::SearchEngine::STATUS_ERROR ) {
		ret = SearchEngineWrapper::SearchStatus::ERROR;
	}

	return ret;
}

// set result location
void IdentificationManagerWrapper::setResultLocation( int id, System::String^ location ) {
	m_identMgr.setResultLocation( id, ClrObjectTool::convertString( location ).c_str() );
}

// get result location
System::String^ IdentificationManagerWrapper::getResultLocation( int id ) {
	return ClrObjectTool::convertString( m_identMgr.getResultLocation( id ).c_str(), NULL );
}

// get searches
cli::array< int >^ IdentificationManagerWrapper::getSearchesFromEngine( int engineId ) {

	std::vector< int > searches;
	m_identMgr.getSearchesFromEngine( searches, engineId );

	// create array
	return makeIntArrayFromVector( searches );
}

// get searches
cli::array< int >^ IdentificationManagerWrapper::getSearchesFromDb( int dbId ) {

	std::vector< int > db;
	m_identMgr.getSearchesFromDb( db, dbId );

	// create array
	return makeIntArrayFromVector( db );
}

// get searches
cli::array< int >^ IdentificationManagerWrapper::getSearchesFromMatrix( int matrixId ) {

	std::vector< int > matrixs;
	m_identMgr.getSearchesFromMatrix( matrixs, matrixId );

	// create array
	return makeIntArrayFromVector( matrixs );
}

// get searches
cli::array< int >^ IdentificationManagerWrapper::getSearchesFromSample( SampleWrapper^ sample ) {
	// sample
	kome::objects::Sample* s = ClrObjectTool::getSample( sample );

	// searches
	std::vector< int > searches;
	m_identMgr.getSearchesFromSample( searches, s );

	// create array
	return makeIntArrayFromVector( searches );
}

// get searches
cli::array< int >^ IdentificationManagerWrapper::getSearches() {
	
	std::vector< int > searches;
	m_identMgr.getSearches( searches );

	// create array
	return makeIntArrayFromVector( searches );
}

// get search
SearchEngineWrapper::SearchInfo^ IdentificationManagerWrapper::getSearch( int id ) {
	kome::ident::SearchInfo info;
	m_identMgr.getSearch( id, info );

	return SearchEngineWrapper::copySearchInfo( &info );
}

// get search
void IdentificationManagerWrapper::getSearch( int id, SearchResultWrapper^ result ) {
	if( result == nullptr ) {
		return;
	}

	m_identMgr.getSearch( id, result->getSearchResult() );
}

// get search
int IdentificationManagerWrapper::getSearch( System::String^ engine, System::String^ version, System::String^ searchId ) {
	return m_identMgr.getSearch( 
		ClrObjectTool::convertString( engine ).c_str(),
		ClrObjectTool::convertString( version ).c_str(),
		ClrObjectTool::convertString( searchId ).c_str()
	);
}

// get search ID
System::String^ IdentificationManagerWrapper::getSearchId( int id ) {
	std::string searchId = m_identMgr.getSearchId( id );

	return ClrObjectTool::convertString( searchId.c_str(), NULL );
}

// set search spectrum
int IdentificationManagerWrapper::setSearchSpectrum( int specId, int searchId, int peaks, double precursor, cli::array< int >^ charges ) {
	// create array
	std::vector< int > arrayCharges;
	int size = 0;
	if( charges != nullptr && charges->Length > 0 ){
		size = charges->Length;
		arrayCharges.resize( size );

		for( int i=0; i < size; i++ ){
			arrayCharges[ i ] = charges[ i ];
		}
	}
	
	// set search specrum
	return m_identMgr.setSearchSpectrum(
			specId,
			searchId,
			peaks,
			precursor,
			arrayCharges
		);
}

// delete search spectrum
void IdentificationManagerWrapper::deleteSearchSpectrum( int id ) {

	m_identMgr.deleteSearchSpectrum( id );
}

// get search spectra
cli::array< int >^ IdentificationManagerWrapper::getSearchSpectraFromSearch( int searchId ) {

	std::vector< int > searches;
	m_identMgr.getSearchSpectraFromSearch( searches, searchId );

	// create array
	return makeIntArrayFromVector( searches );
}

// get search spectra
cli::array< int >^ IdentificationManagerWrapper::getSearchSpectraFromSpectrum( int specId ) {

	std::vector< int > spectra;
	m_identMgr.getSearchSpectraFromSpectrum( spectra, specId );

	// create array
	return makeIntArrayFromVector( spectra );
}

// get search spectrum
int IdentificationManagerWrapper::getSearchSpectrum( int searchId, int specId ) {

	return m_identMgr.getSearchSpectrum( searchId, specId );
}

// get search spectra
cli::array< int >^ IdentificationManagerWrapper::getSearchSpectra() {

	std::vector< int > spectra;
	m_identMgr.getSearchSpectra( spectra );

	// create array
	return makeIntArrayFromVector( spectra );
}

// get search spectrum
IdentificationManagerWrapper::SearchSpectrumInfo^ IdentificationManagerWrapper::getSearchSpectrum( int id ) {
	int s0 = -1;
	int s1 = -1;
	int p0 = -1;
	double p1 = -1.0;
	m_identMgr.getSearchSpectrum( id, &s0, &s1, &p0, &p1 );

	SearchSpectrumInfo^ ssi = gcnew SearchSpectrumInfo();
	ssi->specId = s0;
	ssi->searchId = s1;
	ssi->peaks = p0;
	ssi->precursor = p1;

	return ssi;
}

// get charges
cli::array< int >^ IdentificationManagerWrapper::getCharges( int searchSpecId ) {
	std::vector< int > charges;
	m_identMgr.getCharges( searchSpecId, charges );

	return makeIntArrayFromVector( charges );
}

// get matrix peak spectrum
int IdentificationManagerWrapper::setMatrixPeakSpectrum( int peakId, int specId ) {

	return m_identMgr.setMatrixPeakSpectrum( peakId, specId );
}

// delte matrix peak spectrum
void IdentificationManagerWrapper::deleteMatrixPeakSpectrum( int id ) {

	m_identMgr.deleteMatrixPeakSpectrum( id );
}

// get matrix peak spectra
cli::array< int >^ IdentificationManagerWrapper::getMatrixPeakSpectraFromSpectrum( int searchSpecId ) {

	std::vector< int > spectra;
	m_identMgr.getMatrixPeakSpectraFromSpectrum( spectra, searchSpecId );

	// create array
	return makeIntArrayFromVector( spectra );
}

// get matrix peak spectra
cli::array< int >^ IdentificationManagerWrapper::getMatrixPeakSpectraFromPeak( int peakId ) {

	std::vector< int >peakSpectra;
	m_identMgr.getMatrixPeakSpectraFromPeak( peakSpectra, peakId );

	// create array
	return makeIntArrayFromVector( peakSpectra );
}

// get matrix peak spectra
cli::array< int >^ IdentificationManagerWrapper::getMatrixPeakSpectraFromSearchAndPeak( int searchId, int peakId ) {

	std::vector< int >peakSpectra;
	m_identMgr.getMatrixPeakSpectraFromSearchAndPeak( peakSpectra, searchId, peakId );

	// create array
	return makeIntArrayFromVector( peakSpectra );
}

// get matrix peak spectra
cli::array< int >^ IdentificationManagerWrapper::getMatrixPeakSpectra() {

	std::vector< int >peakSpectra;
	m_identMgr.getMatrixPeakSpectra( peakSpectra );

	// create array
	return makeIntArrayFromVector( peakSpectra );
}

// get matrix peak spectrum peak
int IdentificationManagerWrapper::getMatrixPeakSpectrumPeak( int id ) {

	return m_identMgr.getMatrixPeakSpectrumPeak( id );
}

// get matrix peak spectrum spectrum
int IdentificationManagerWrapper::getMatrixPeakSpectrumSpectrum( int id ) {

	return m_identMgr.getMatrixPeakSpectrumSpectrum( id );
}

// add hit
int IdentificationManagerWrapper::addHit( HitInfo^ hit ) {

	kome::ident::HitInfo* info = new kome::ident::HitInfo();
	copyHitInfo( hit , *info );
	
	return m_identMgr.addHit( *info );
}

// delete hit
void IdentificationManagerWrapper::deleteHit( int id ) {

	m_identMgr.deleteHit( id );
}

// get hits
cli::array< int >^ IdentificationManagerWrapper::getHitsFromSubstance( int substanceId ) {

	std::vector< int > substances;
	m_identMgr.getHitsFromSubstance( substances, substanceId );

	// create array
	return makeIntArrayFromVector( substances );
}

// get hits
cli::array< int >^ IdentificationManagerWrapper::getHitsFromSearch( int searchId ) {
	std::vector< int > searchs;
	m_identMgr.getHitsFromSearch( searchs, searchId );

	// create array
	return makeIntArrayFromVector( searchs );
}

// get hits
cli::array< int >^ IdentificationManagerWrapper::getHitsFromSpectrum( int searchSpecId ) {

	std::vector< int > searchs;
	m_identMgr.getHitsFromSpectrum( searchs, searchSpecId );

	// create array
	return makeIntArrayFromVector( searchs );
}

// get hits
cli::array< int >^ IdentificationManagerWrapper::getHits() {

	std::vector< int > hits;
	m_identMgr.getHits( hits );

	// create array
	return makeIntArrayFromVector( hits );
}

// get hit
IdentificationManagerWrapper::HitInfo^ IdentificationManagerWrapper::getHit( int id ) {
	HitInfo^ hit = gcnew HitInfo();

	kome::ident::HitInfo info;
	m_identMgr.getHit( id, info );

	// copy
	hit->substance = info.substance;
	hit->charge = info.charge;
	hit->spectra = makeIntArrayFromVector( info.spectra );
	hit->props = nullptr;
	hit->altSubstances = gcnew cli::array< AltSubstance^ >( info.altSubstances.size() );
	for( unsigned int i = 0; i < info.altSubstances.size(); i++ ) {
		AltSubstance^ as = gcnew AltSubstance();
		( hit->altSubstances )[ i ] = as;

		as->substance = info.altSubstances.at( i ).id;
		as->before = ClrObjectTool::convertString( info.altSubstances.at( i ).before.c_str(), NULL );
		as->after = ClrObjectTool::convertString( info.altSubstances.at( i ).after.c_str(), NULL );
		as->start = info.altSubstances.at( i ).start;
		as->end = info.altSubstances.at( i ).end;
	}

	if( info.props.size() > 0 ) {
		hit->props = gcnew cli::array< SubstanceValue^ >( info.props.size() );
	}

	for( unsigned int i = 0; i < info.props.size(); i++ ) {
		SubstanceValue^ val = gcnew SubstanceValue();
		( hit->props )[ i ] = val;
		val->name = ClrObjectTool::convertString( info.props.at( i ).name.c_str(), NULL );
		val->value = ClrObjectTool::convertString( info.props.at( i ).value.c_str(), NULL );
		val->type = HitWrapper::ValueType::PROPERTY;
		if( info.props.at( i ).type == kome::ident::TYPE_SCORE ) {
			val->type = HitWrapper::ValueType::SCORE;
		}
		else if( info.props.at( i ).type == kome::ident::TYPE_EXPECTATION_VALUE ) {
			val->type = HitWrapper::ValueType::EXPECTATION_VALUE;
		}
	}

	return hit;
}

// get accession number
void IdentificationManagerWrapper::setAccessionNumber( System::String^ prefix, int num ) {

	m_identMgr.setAccessionNumber( ClrObjectTool::convertString( prefix ).c_str(), num );
}

// get accession number
int IdentificationManagerWrapper::getAccessionNumber( System::String^ prefix ) {

	return m_identMgr.getAccessionNumber( ClrObjectTool::convertString( prefix ).c_str() );
}

// issue accession number
int IdentificationManagerWrapper::issueAccessionNumber( System::String^ prefix ) {

	return m_identMgr.issueAccessionNumber( ClrObjectTool::convertString( prefix ).c_str() );
}

// get database version
int IdentificationManagerWrapper::getDbVersion() {

	return m_identMgr.getDbVersion();
}

// get current database version
int IdentificationManagerWrapper::getCurrentDbVersion() {

	return m_identMgr.getCurrentDbVersion();
}

// convert database schema
bool IdentificationManagerWrapper::convertDbSchema( ProgressWrapper^ progress ) {

	return m_identMgr.convertDbSchema( *( ClrObjectTool::getProgress( progress ) ) );
}

// get peak list
void IdentificationManagerWrapper::getPeakList( System::String^ peakList, XYDataWrapper^ xyData ) {
	kome::core::XYData* xyd = ClrObjectTool::getXYData( xyData );

	m_identMgr.getPeakList( ClrObjectTool::convertString( peakList ).c_str(), *xyd );
}

// make int array
cli::array< int >^ IdentificationManagerWrapper::makeIntArrayFromVector( std::vector< int >& arr ) {
	// check the parameter
	if( arr.size() == 0 ) {
		return nullptr;
	}

	// array
	cli::array< int >^ a = gcnew cli::array< int >( arr.size() );
	for( unsigned int i = 0; i < arr.size(); i++ ) {
		a[ i ] = arr[ i ];
	}

	return a;
}

// copy sample read Info
void IdentificationManagerWrapper::copySampleInfo( SampleReadInfo^ readInfo, kome::ident::SampleReadInfo& dst ){
	if( readInfo == nullptr ){
		return;
	}

	// copy
	dst.id = readInfo->id;
	dst.openSampleSet = readInfo->openSampleSet;
	dst.openSample = readInfo->openSample;
}

// copy sample read info
IdentificationManagerWrapper::SampleReadInfo^ IdentificationManagerWrapper::copySampleInfo( kome::ident::SampleReadInfo* readInfo ) {
	if( readInfo == NULL ) {
		return nullptr;
	}

	SampleReadInfo^ sri = gcnew SampleReadInfo();
	sri->id = readInfo->id;
	sri->openSampleSet = readInfo->openSampleSet;
	sri->openSample = readInfo->openSample;

	return sri;
}

// copy substance Info
void IdentificationManagerWrapper::copySubstance( SubstanceInfo^ substance, kome::ident::SubstanceInfo& dst ){
	if( substance == nullptr ){
		return;
	}
	kome::core::Properties* p = NULL;
	if( substance->props != nullptr ){
		p = ClrObjectTool::getProperties( substance->props );
	}
	
	// type
	kome::ident::SubstanceType t = kome::ident::TYPE_UNKNOWN;
	if( substance->type == SubstanceWrapper::SubstanceType::PROTEIN ) {
		t = kome::ident::TYPE_PROTEIN;
	}
	else if( substance->type == SubstanceWrapper::SubstanceType::PEPTIDE ) {
		t = kome::ident::TYPE_PEPTIDE;
	}
	else if( substance->type == SubstanceWrapper::SubstanceType::METABOLITE ) {
		t = kome::ident::TYPE_METABOLITE;
	}

	// copy
	dst.accession = ClrObjectTool::convertString( substance->accession ).c_str();
	dst.name = ClrObjectTool::convertString( substance->name ).c_str();
	dst.dbId = substance->dbId;
	dst.type = t;
	dst.props = *p;
	dst.parentId = substance->parentId;
}

// copy substance info
IdentificationManagerWrapper::SubstanceInfo^ IdentificationManagerWrapper::copySubstance( kome::ident::SubstanceInfo* substance ) {
	// check the parameter
	if( substance == NULL ) {
		return nullptr;
	}

	// substance info
	SubstanceInfo^ s = gcnew SubstanceInfo();

	// type
	SubstanceWrapper::SubstanceType t = SubstanceWrapper::SubstanceType::UNKNOWN;
	if( substance->type == kome::ident::TYPE_PROTEIN ) {
		t = SubstanceWrapper::SubstanceType::PROTEIN;
	}
	else if( substance->type == kome::ident::TYPE_PEPTIDE ) {
		t = SubstanceWrapper::SubstanceType::PEPTIDE;
	}
	else if( substance->type == kome::ident::TYPE_METABOLITE ) {
		t = SubstanceWrapper::SubstanceType::METABOLITE;
	}

	// copy
	s->accession = ClrObjectTool::convertString( substance->accession.c_str(), NULL );
	s->name = ClrObjectTool::convertString( substance->name.c_str(), NULL );
	s->dbId = substance->dbId;
	s->type = t;
	s->props = ClrObjectTool::createPropertiesWrapper( &( substance->props ) );
	s->parentId = substance->parentId;

	return s;
}

void IdentificationManagerWrapper::copyHitInfo( HitInfo^ hit, kome::ident::HitInfo& dst ){
	// create array
	std::vector< int > arrySpectra;
	int size = 0;
	if( hit->spectra != nullptr && hit->spectra->Length > 0 ){
		size = hit->spectra->Length;
		arrySpectra.resize( size );

		for( int i=0; i < size; i++ ){
			arrySpectra[ i ] = hit->spectra[ i ];
		}
	}

	std::vector< kome::ident::SubstanceValue > arrySubVal;
	size = 0;
	if( hit->props != nullptr && hit->props->Length > 0 ){
		size = hit->props->Length;
		arrySubVal.resize( size );

		for( int i=0; i < size; i++ ){
			SubstanceValue^ sub = hit->props[ i ];
			kome::ident::SubstanceValue val;
			if( sub != nullptr ){
				val.name = ClrObjectTool::convertString( sub->name ).c_str();
				val.value = ClrObjectTool::convertString( sub->value ).c_str();
				val.type = kome::ident::TYPE_PROPERTY;			
				if( sub->type == HitWrapper::ValueType::SCORE ){
					val.type = kome::ident::TYPE_SCORE;
				}else if(  sub->type ==HitWrapper::ValueType::EXPECTATION_VALUE ){
					val.type = kome::ident::TYPE_EXPECTATION_VALUE;
				}

				arrySubVal[ i ] = val;
			}
		}
	}

	dst.substance = hit->substance;
	dst.charge = hit->charge;
	dst.spectra = arrySpectra;
	dst.props = arrySubVal;
	if( hit->altSubstances != nullptr ) {
		for( int i = 0; i < hit->altSubstances->Length; i++ ) {
			kome::ident::AltSubstance as;
			as.id = ( hit->altSubstances )[ i ]->substance;
			as.before = ClrObjectTool::convertString( ( hit->altSubstances )[ i ]->before );
			as.after = ClrObjectTool::convertString( ( hit->altSubstances )[ i ]->after );
			as.start = ( hit->altSubstances )[ i ]->start;
			as.end = ( hit->altSubstances )[ i ]->end;
			dst.altSubstances.push_back( as );
		}
	}
}

// copy hit info
IdentificationManagerWrapper::HitInfo^ IdentificationManagerWrapper::copyHitInfo( kome::ident::HitInfo* hit ) {
	// check the parameter
	if( hit == NULL ) {
		return nullptr;
	}

	// hit info
	HitInfo^ h = gcnew HitInfo();

	h->substance = hit->substance;
	h->charge = hit->charge;
	h->spectra = makeIntArrayFromVector( hit->spectra );

	h->altSubstances = gcnew cli::array< AltSubstance^ >( hit->altSubstances.size() );
	for( unsigned int i = 0; i < hit->altSubstances.size(); i++ ) {
		AltSubstance^ as = gcnew AltSubstance();
		( h->altSubstances )[ i ] = as;

		as->substance = hit->altSubstances.at( i ).id;
		as->before = ClrObjectTool::convertString( hit->altSubstances.at( i ).before.c_str(), NULL );
		as->after = ClrObjectTool::convertString( hit->altSubstances.at( i ).after.c_str(), NULL );
		as->start = hit->altSubstances.at( i ).start;
		as->end = hit->altSubstances.at( i ).end;
	}

	if( hit->props.size() > 0 ) {
		h->props = gcnew cli::array< SubstanceValue^ >( hit->props.size() );

		for( unsigned int i = 0; i < hit->props.size(); i++ ) {
			kome::ident::SubstanceValue val = hit->props.at( i );
			SubstanceValue^ v = gcnew SubstanceValue();
			( h->props )[ i ] = v;
			v->name = ClrObjectTool::convertString( val.name.c_str(), NULL );
			v->value = ClrObjectTool::convertString( val.value.c_str(), NULL );
			v->type = HitWrapper::ValueType::PROPERTY;
			if( val.type = kome::ident::TYPE_SCORE ) {
				v->type = HitWrapper::ValueType::SCORE;
			}
			else if( val.type == kome::ident::TYPE_EXPECTATION_VALUE ) {
				v->type = HitWrapper::ValueType::EXPECTATION_VALUE;
			}
		}
	}

	return h;
}

// get identification manager object
IdentificationManagerWrapper^ IdentificationManagerWrapper::getInstance() { 
	// create object
	if( m_mgr == nullptr ) {
		m_mgr = gcnew IdentificationManagerWrapper( kome::ident::IdentificationManager::getInstance() );
	}

	return m_mgr;
}
