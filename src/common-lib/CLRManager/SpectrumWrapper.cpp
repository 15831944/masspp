/**
 * @file SpectrumWrapper.cpp
 * @brief implements of SpectrumWrapper class
 *
 * @author S.Tanaka
 * @date 2006.08.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SpectrumWrapper.h"

#include "SampleWrapper.h"
#include "XYDataWrapper.h"
#include "PropertiesWrapper.h"
#include "DataGroupNodeWrapper.h"
#include "ClrObjectTool.h"
#include "ClrSample.h"
#include "ClrSpectrum.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SpectrumWrapper::SpectrumWrapper( kome::objects::Spectrum& spec ) : m_spec( spec ) {
}

// destructor
SpectrumWrapper::~SpectrumWrapper() {
}

// get Spectrum object
kome::objects::Spectrum& SpectrumWrapper::getSpectrum() {
	return m_spec;
}

// set ID
void SpectrumWrapper::setId( int id ) {
	m_spec.setId( id );
}

// get ID
int SpectrumWrapper::getId() {
	return m_spec.getId();
}

// get sample
SampleWrapper^ SpectrumWrapper::getSample() {
	// get sample
	kome::objects::Sample* sample = m_spec.getSample();
	if( sample == NULL ) {
		return nullptr;
	}

	// cast
	ClrSample* clrSample = dynamic_cast< ClrSample* >( sample );
	if( clrSample != NULL ) {
		return clrSample->getBaseSample();
	}

	return gcnew SampleWrapper( *sample );
}

// set name
void SpectrumWrapper::setName( System::String^ name ) {
	m_spec.setName( ClrObjectTool::convertString( name ).c_str() );
}

// get name 
System::String^ SpectrumWrapper::getName() {
	return gcnew System::String( m_spec.getName() );
}

// set retention time
void SpectrumWrapper::setRt( double rt ) {
	m_spec.setRt( rt );
}

// set retention time
void SpectrumWrapper::setRt( double startRt, double endRt ) {
	m_spec.setRt( startRt, endRt );
}

// set start retention time
void SpectrumWrapper::setStartRt( double rt ) {
	m_spec.setStartRt( rt );
}

// set end retention time
void SpectrumWrapper::setEndRt( double rt ) {
	m_spec.setEndRt( rt );
}

// get retention time
double SpectrumWrapper::getRt() {
	return m_spec.getRt();
}

// get start retention time
double SpectrumWrapper::getStartRt() {
	return m_spec.getStartRt();
}

// get end retention time
double SpectrumWrapper::getEndRt() {
	return m_spec.getEndRt();
}

// set spectrum type
void SpectrumWrapper::setSpecType( System::String^ type ) {
	m_spec.setSpecType( ClrObjectTool::convertString( type ).c_str() );
}

// get spectrum type
System::String^ SpectrumWrapper::getSpecType() {
	return ClrObjectTool::convertString( m_spec.getSpecType(), NULL );
}

// set title
void SpectrumWrapper::setTitle( System::String^ title ) {
	m_spec.setTitle( ClrObjectTool::convertString( title ).c_str() );
}

// get title
System::String^ SpectrumWrapper::getTitle() {
	return gcnew System::String( m_spec.getTitle() );
}

// set icon
void SpectrumWrapper::setIcon( System::String^ icon ) {
	m_spec.setIcon( ClrObjectTool::convertString( icon ).c_str() );
}

// get icon
System::String^ SpectrumWrapper::getIcon() {
	return gcnew System::String( m_spec.getIcon() );
}

// get xy data from data manager
XYDataWrapper^ SpectrumWrapper::getXYData() {
	kome::core::XYData* xyData = m_spec.getXYData();
	if( xyData == NULL ) {
		return nullptr;
	}

	return gcnew XYDataWrapper( *xyData );
}

// delete xy data of data manager
void SpectrumWrapper::deleteXYData() {
	m_spec.deleteXYData();
}

// get data points
void SpectrumWrapper::getXYData( XYDataWrapper^ xyData, bool op ) {
	m_spec.getXYData( &xyData->getXYData(), op );
}

void SpectrumWrapper::getXYData( XYDataWrapper^ xyData, double minX, double maxX, bool zero ) {
	m_spec.getXYData( &xyData->getXYData(), minX, maxX, zero );
}

// void set x range
void SpectrumWrapper::setXRange( double minX, double maxX ) {
	m_spec.setXRange( minX, maxX );
}

// set min x
void SpectrumWrapper::setMinX( double minX ) {
	m_spec.setMinX( minX );
}

// get min x
double SpectrumWrapper::getMinX() {
	return m_spec.getMinX();
}

// set max x
void SpectrumWrapper::setMaxX( double maxX ) {
	m_spec.setMaxX( maxX );
}

// get max x
double SpectrumWrapper::getMaxX() {
	return m_spec.getMaxX();
}

// set total intenstiy
void SpectrumWrapper::setTotalIntensity( double intensity ) {
	m_spec.setTotalIntensity( intensity );
}

// get total intensity
double SpectrumWrapper::getTotalIntensity( double minX, double maxX ) {
	return m_spec.getTotalIntensity( minX, maxX );
}

// set max intensity
void SpectrumWrapper::setMaxIntensity( double intensity ) {
	m_spec.setMaxIntensity( intensity );
}

// get max intensity
double SpectrumWrapper::getMaxIntensity( double minX, double maxX ) {
	return m_spec.getMaxIntensity( minX, maxX );
}

// set base peak mass
void SpectrumWrapper::setBasePeakMass( double mass ) {
	m_spec.setBasePeakMass( mass );
}

// get base peak mass
double SpectrumWrapper::getBasePeakMass() {
	return m_spec.getBasePeakMass();
}

// set ms stage
void SpectrumWrapper::setMsStage( int stage ) {
	m_spec.setMsStage( stage );
}

// get ms stage
int SpectrumWrapper::getMsStage() {
	return m_spec.getMsStage();
}

// set precursor
void SpectrumWrapper::setPrecursor( int stage, double precursor ) {
	m_spec.setPrecursor( stage, precursor );
}

// set precursor
void SpectrumWrapper::setPrecursor( double precursor ) {
	m_spec.setPrecursor( precursor );
}

// get precursor
double SpectrumWrapper::getPrecursor( int stage ) {
	return m_spec.getPrecursor( stage );
}

// get precursor
double SpectrumWrapper::getPrecursor() {
	return m_spec.getPrecursor();
}

// set precursor intensity
void SpectrumWrapper::setPrecursorIntensity( int stage, double intensity ) {
	m_spec.setPrecursorIntensity( stage, intensity );
}

// set precursor intensity
void SpectrumWrapper::setPrecursorIntensity( double intensity ) {
	m_spec.setPrecursorIntensity( intensity );
}

// get precursor intensity
double SpectrumWrapper::getPrecursorIntensity( int stage ) {
	return m_spec.getPrecursorIntensity( stage );
}

// get precursor intensity
double SpectrumWrapper::getPrecursorIntensity() {
	return m_spec.getPrecursorIntensity();
}

// set precursor charge
void SpectrumWrapper::setPrecursorCharge( int charge ) {
	m_spec.setPrecursorCharge( charge );
}

// get precursor charge
int SpectrumWrapper::getPrecursorCharge() {
	return m_spec.getPrecursorCharge();
}

// set parent spectrum
void SpectrumWrapper::setParentSpectrum( SpectrumWrapper^ parent ) {
	if( parent == nullptr ) {
		m_spec.setParentSpectrum( NULL );
	}
	else {
		m_spec.setParentSpectrum( &parent->getSpectrum() );
	}
}

// get parent spectrum
SpectrumWrapper^ SpectrumWrapper::getParentSpectrum() {
	// get parent spectrum
	kome::objects::Spectrum* parent = m_spec.getParentSpectrum();
	if( parent == NULL ) {
		return nullptr;
	}

	// cast
	SpectrumWrapper^ ps = ClrObjectTool::createSpectrumWrapper( parent );
	return ps;
}

// get child spectra
cli::array< SpectrumWrapper^ >^ SpectrumWrapper::getChildSpectra() {
	// children
	std::vector< kome::objects::Spectrum* > children;
	m_spec.getChildSpectra( children );
	if( children.size() == 0 ) {
		return nullptr;
	}

	// array
	cli::array< SpectrumWrapper^ >^ arr = gcnew cli::array< SpectrumWrapper^ >( children.size() );
	for( unsigned int i = 0; i < children.size(); i++ ) {
		SpectrumWrapper^ child = ClrObjectTool::createSpectrumWrapper( children[ i ] );
		arr[ i ] = child;
	}

	return arr;
}

// set whether this spectrum has chromatogram
void SpectrumWrapper::setHasChromatogram( bool chromatogram ) {
	m_spec.setHasChromatogram( chromatogram );
}

// judge whether this spectrum has chromatogram
bool SpectrumWrapper::hasChromatogram() {
	return m_spec.hasChromatogram();
}

// set spectrum group
void SpectrumWrapper::setGroup( DataGroupNodeWrapper^ group ) {
	m_spec.setGroup( &group->getDataGroupNode() );
}

// get spectrum group
DataGroupNodeWrapper^ SpectrumWrapper::getGroup() {
	kome::objects::DataGroupNode* group = m_spec.getGroup();
	if( group == NULL ) {
		return nullptr;
	}

	return gcnew DataGroupNodeWrapper( *group );
}

// set scan number
void SpectrumWrapper::setScanNumber( int scan ) {
	m_spec.setScanNumber( scan );
}

// get scan number
int SpectrumWrapper::getScanNumber() {
	return m_spec.getScanNumber();
}

// set polarity
void SpectrumWrapper::setPolarity( Polarity polarity ) {
	if( polarity == Polarity::POSITIVE ) {
		m_spec.setPolarity( kome::objects::Spectrum::POLARITY_POSITIVE );
	}
	else if( polarity == Polarity::NEGATIVE ) {
		m_spec.setPolarity( kome::objects::Spectrum::POLARITY_NEGATIVE );
	}
	else {
		m_spec.setPolarity( kome::objects::Spectrum::POLARITY_UNKNOWN );
	}
}

// get polarity
SpectrumWrapper::Polarity SpectrumWrapper::getPolarity() {
	if( m_spec.getPolarity() == kome::objects::Spectrum::POLARITY_POSITIVE ) {
		return Polarity::POSITIVE;
	}
	else if( m_spec.getPolarity() == kome::objects::Spectrum::POLARITY_POSITIVE ) {
		return Polarity::NEGATIVE;
	}
	return Polarity::UNKNOWN;
}

// set centroid mode
void SpectrumWrapper::setCentroidMode( bool centroidMode ) {
	m_spec.setCentroidMode( centroidMode );
}

// get centroid mode
bool SpectrumWrapper::isCentroidMode() {
	return m_spec.isCentroidMode();
}

// set resolution
void SpectrumWrapper::setResolution( double resolution ) {
	m_spec.setResolution( resolution );
}

// get resolution
double SpectrumWrapper::getResolution() {
	return m_spec.getResolution();
}

// set collision energy
void SpectrumWrapper::setCollisionEnergy( System::String^ collisionEnergy ) {
	m_spec.setCollisionEnergy( ClrObjectTool::convertString( collisionEnergy ).c_str() );
}

// get collision energy
System::String^ SpectrumWrapper::getCollisionEnergy() {
	return ClrObjectTool::convertString( m_spec.getCollisionEnergy(), NULL );
}

// set visible
void SpectrumWrapper::setVisible( bool visible ) {
	m_spec.setVisible( visible );
}

// get visible
bool SpectrumWrapper::isVisible() {
	return m_spec.isVisible();
}

// get properties
PropertiesWrapper^ SpectrumWrapper::getProperties() {
	return gcnew PropertiesWrapper( m_spec.getProperties() );
}

// set auto zero points
void SpectrumWrapper::setAutoZeroPoints( bool autoZero ) {
	m_spec.setAutoZeroPoints( autoZero );
}

// get auto zero points
bool SpectrumWrapper::isAutoZeroPoints() {
	return m_spec.isAutoZeroPoints();
}

// set original spectrum
void SpectrumWrapper::setOrgSpectrum( SpectrumWrapper^ spec ) {
	kome::objects::Spectrum* s = ClrObjectTool::getSpectrum( spec );
	m_spec.setOrgSpectrum( s );
}

// get original spectrum
SpectrumWrapper^ SpectrumWrapper::getOrgSpectrum() {
	kome::objects::Spectrum* s = m_spec.getOrgSpectrum();
	SpectrumWrapper^ spec = ClrObjectTool::createSpectrumWrapper( s );

	return spec;
}

// get properties
void SpectrumWrapper::getProperties( PropertiesWrapper^ properties ) {
	if( properties != nullptr ) {
		m_spec.getProperties( properties->getProperties() );
	}
}

// check whether the specified key is common or not
bool SpectrumWrapper::isCommonProperty( System::String^ key ) {
	return kome::objects::Spectrum::isCommonProperty( ClrObjectTool::convertString( key ).c_str() );
}

// >>>>>>	@Date:2013/07/18	<Add>	A.Ozaki
//

// set spot id
void	SpectrumWrapper::setSpotId( System::String^ spotId )
{
	m_spec.setSpotId( ClrObjectTool::convertString( spotId ).c_str( ) );
}

// get spot id
System::String^	SpectrumWrapper::getSpotId( void )
{
	return gcnew System::String( m_spec.getSpotId( ) );
}

// set the flag of request load data
void	SpectrumWrapper::setRequestLoadData( void )
{
	m_spec.setRequestLoadData( );

	return;
}

// reset the flag of request load data
void	SpectrumWrapper::resetRequestLoadData( void )
{
	m_spec.resetRequestLoadData( );

	return;
}

// check the flag of request load data
bool	SpectrumWrapper::isRequestLoadData( void )
{
	return	m_spec.isRequestLoadData( );
}

// set the flag of first access
void	SpectrumWrapper::setFirstAccess( void )
{
	m_spec.setFirstAccess( );

	return;
}

// reset the flag of first access
void	SpectrumWrapper::resetFirstAccess( void )
{
	m_spec.resetFirstAccess( );

	return;
}

// check the flag of first access
bool	SpectrumWrapper::isFirstAccess( void  )
{
	return	m_spec.isFirstAccess( );
}

// load data
bool	SpectrumWrapper::loadData( void )
{
	return	m_spec.loadData( );
}



//
// <<<<<<	@Date:2013/07/18	<Add>	A.Ozaki
