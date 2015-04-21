/**
 * @file Spectrum.cpp
 * @brief implements of Spectrum class
 *
 * @author S.Tanaka
 * @date 2006.08.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Spectrum.h"

#include "DataGroupNode.h"
#include "PointsManager.h"
#include "PeaksManager.h"
#include "Sample.h"
#include "SampleSet.h"
#include "XYDataOperation.h"


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define DEFAULT_RESOLUTION   0.3


// static member
std::vector< std::string > Spectrum::m_commonProps;


// constructor
Spectrum::Spectrum( Sample* sample, const char* name ) {
	// initialize
	m_sample = sample;
	m_name = NVL( name, "" );

	m_rt.start = -1.0;
	m_rt.end = -1.0;

	m_bpm = -1.0;

	m_msStage = 1;
	m_parentSpec = NULL;
	m_chrom = false;

	m_charge = 0;

	m_group = NULL;
	m_scanNumber = -1;

	m_polarity = POLARITY_UNKNOWN;
	m_centroidMode = false;

	m_resolution = -1.0;

	m_orgSpec = NULL;
	m_op = false;

	// reserve properties
	isCommonProperty( NULL );

	for( unsigned int i = 0; i < m_commonProps.size(); i++ ) {
		m_props.setValue( m_commonProps[ i ].c_str(), "" );
	}
	m_props.setValue( "Name", m_name.c_str() );

	// issue spectrum id
	if( sample != NULL ){
		m_specId = sample->issueSpecId();
	}

	// visible flag
	m_visible = true;
	m_autoZeroPoints = true;

	// >>>>>> @Date:2013/07/16	<Add>	A.Ozaki
	m_strSpotId = FMT( "" );
	// <<<<<< @Date:2013/07/16	<Add>	A.Ozaki

}

// destructor
Spectrum::~Spectrum() {
	// manager
	PointsManager& ptMgr = PointsManager::getInstance();
	PeaksManager& pkMgr = PeaksManager::getInstance();

	// close
	DataManager::closeSpectrum( this, true );

	// delete
	deleteXYData();
	ptMgr.clearOperations( this );
	pkMgr.deletePeaks( this );
}

// get sample object
Sample* Spectrum::getSample() {
	return m_sample;
}

// set spectrum name
void Spectrum::setName( const char* name ) {
	m_name = std::string( NVL( name, "" ) ) ;
	if( m_title.empty() ) {
		setTitle( name );
	}

	m_props.setValue( "Name", m_name.c_str() );
}

// get spectrum name
const char* Spectrum::getName() {
	firstAccessProcess( );			// @Date:2013/07/16	<Add>	A.Ozaki
	return m_name.c_str();
}

// set retention time
void Spectrum::setRt( const double rt ) {
	// properties
	if( rt >= 0.0 ) {
		m_props.setDoubleValue( "RT", rt );
		m_props.setDoubleValue( "Start RT", rt );
		m_props.setDoubleValue( "End RT", rt );
	}
	else {
		m_props.setValue( "RT", "" );
		m_props.setValue( "Start RT", "" );
		m_props.setValue( "End RT", "" );
	}

	// RT
	setRt( rt, rt );
}

// set retention time
void Spectrum::setRt( const double startRt, const double endRt ) {
	if( startRt >= 0.0 && endRt >= 0.0 ) {
		m_props.setDoubleValue( "RT", ( startRt + endRt ) / 2.0 );
	}
	else {
		m_props.setValue( "RT", "" );
	}

	setStartRt( startRt );
	setEndRt( endRt );
}

// set start retention time
void Spectrum::setStartRt( const double rt ) {
	if( rt >= 0.0 ) {
		m_props.setDoubleValue( "Start RT", rt );
	}
	else {
		m_props.setValue( "Start RT", "" );
	}

	m_rt.start = rt;
}

// set end retention time
void Spectrum::setEndRt( const double rt ) {
	if( rt >= 0.0 ) {
		m_props.setDoubleValue( "End RT", rt );
	}
	else {
		m_props.setValue( "End RT", "" );
	}

	m_rt.end = rt;
}

// get retention time
double Spectrum::getRt() {
	firstAccessProcess( );			// @Date:2013/07/16	<Add>	A.Ozaki
	double rt = ( m_rt.start + m_rt.end ) / 2.0;
	return rt;
}

// get start retention time
double Spectrum::getStartRt() {
	firstAccessProcess( );			// @Date:2013/07/16	<Add>	A.Ozaki
	return m_rt.start;
}

// get end retention time
double Spectrum::getEndRt() {
	firstAccessProcess( );			// @Date:2013/07/16	<Add>	A.Ozaki
	return m_rt.end;
}

// set spectrum type
void Spectrum::setSpecType( const char* type ) {
	m_specType = trimstring( type );
	m_props.setValue( "Spectrum Type", m_specType.c_str() );
}

// get spectrum type
const char* Spectrum::getSpecType() {
	firstAccessProcess( );			// @Date:2013/07/16	<Add>	A.Ozaki
	return m_specType.c_str();
}

// set title
void Spectrum::setTitle( const char* title ) {
	// title
	m_title = trimstring( title );
	m_props.setValue( "Title", m_title.c_str() );
}

// get title
const char* Spectrum::getTitle() {
	firstAccessProcess( );			// @Date:2013/07/16	<Add>	A.Ozaki

	if( m_title.empty() ) {
		std::string name = m_name;
		std::string values;

		int scanNum = getScanNumber();
		if( scanNum >= 0 ) {
			if( !values.empty() ) {
				values.append( "," );
			}
			values.append( FMT( "scanNum=%d", scanNum ) );
		}

		double rt = getRt();
		if( rt >= 0.0 ) {
			if( !values.empty() ) {
				values.append( "," );
			}
			values.append( FMT( "rt=%.2f", rt ) );
		}

		double prec = getPrecursor();
		if( prec >= 0.0 ) {
			if( !values.empty() ) {
				values.append( "," );
			}
			values.append( FMT( "precursor=%.2f", prec ) );
		}

		if( values.empty() ) {
			m_title = name;
		}
		else if( name.empty() ) {
			m_title = values;
		}
		else {
			m_title = FMT( "%s: %s", name.c_str(), values.c_str() );
		}
	}

	return m_title.c_str();
}

// set icon name
void Spectrum::setIcon( const char* icon ) {
	m_icon = std::string( NVL( icon, "" ) );
}

// get icon name
const char* Spectrum::getIcon() {
	firstAccessProcess( );			// @Date:2013/07/16	<Add>	A.Ozaki
	return m_icon.c_str();
}

// get xy data from data manager
kome::core::XYData* Spectrum::getXYData() {
	PointsManager& ptMgr = PointsManager::getInstance();

	return ptMgr.getXYData( this );
}

// delete xy data of data manager
void Spectrum::deleteXYData() {
	PointsManager& ptMgr = PointsManager::getInstance();

	ptMgr.deleteXYData( this );
}

// get data points
void Spectrum::getXYData( kome::core::XYData* const xyData, const bool op ) {
	// get data points
	getXYData( xyData, -1.0, -1.0, op );

	// operation
	if( op ) {
		// get operations
		PointsManager& ptMgr = PointsManager::getInstance();
		for( unsigned int i = 0; i < ptMgr.getNumberOfOperations( this ); i++ ) {
			XYDataOperation* operation = ptMgr.getOperation( this, i );

			operation->update( *xyData, *xyData, *this );
		}
	}
}

// get data poins
void Spectrum::getXYData( kome::core::XYData* const xyData, const double minX, const double maxX, const bool zero ) {
	// check parameter
	if( xyData == NULL ) {
		return;
	}

	// gets data points
	kome::core::DataPoints dps;
	onGetXYData( &dps, minX, maxX );

	// correct data points
	if( minX < 0.0 && maxX < 0.0 ) {
		// get length
		unsigned int length = dps.getLength();

		// set x range
		double minXX = getMinX();
		double maxXX = getMaxX();

		// set total and max y
		if( !m_tic || !m_bpi || m_bpm < 0.0 ) {
			double tic = 0.0;
			double bpi = 0.0;
			double bpm = 0.0;

			for( unsigned int i = 0; i < length; i++ ) {
				double x = dps.getX( i );
				double y = dps.getY( i );

				tic += y;
				if( y > bpi ) {
					bpi = y;
					bpm = x;
				}
			}

			if( !m_tic ) {
				setTotalIntensity( tic );
			}
			if( !m_bpi ) {
				setMaxIntensity( bpi );
			}

			if( m_bpm < 0.0 ) {
				setBasePeakMass( bpm );
			}
		}
	}
	
	// get precursor intensity
	getPrecursorIntensity();

	// check the points
	if( dps.getLength() == 0 ) {
		return;
	}

	// zero points
	if( zero && m_autoZeroPoints ) {
		double minX = -1.0;
		if( m_minX ) {
			minX = m_minX.get();
		}

		double maxX = -1.0;
		if( m_maxX ) {
			maxX = m_maxX.get();
		}

		const double space = DEFAULT_RESOLUTION;

		// the first point
		double prevX = dps.getX( 0 );
		if( prevX > 0.0 && prevX > minX ) {
			prevX = prevX - space / 3.0;
			if( prevX < 0.0 ) {
				prevX = 0.0;
			}
			if( prevX < minX ) {
				prevX = minX;
			}

			xyData->addPoint( prevX, 0.0 );
		}

		// each points
		for( unsigned int i = 0; i < dps.getLength(); i++ ) {
			const double x = dps.getX( i );
			const double y = dps.getY( i );

		    const double d = x - prevX;

			if( d > space ) {
				const double startX = prevX + space / 3.0;
				const double endX = x - space / 3.0;

				xyData->addPoint( startX, 0.0 );

				const int num = (int)( ( endX - startX ) / space );

				for( int j = 0; j < num; j++ ) {
					double midX = startX + (double)j * ( endX - startX ) / (double)( num + 2 );
					if( midX > startX && midX < endX ) {
						xyData->addPoint( midX, 0.0 );
					}
				}

				xyData->addPoint( endX, 0.0 );
			}

			xyData->addPoint( x, y );

			prevX = x;
		}

		// last point
		if( maxX < 0.0 || prevX < maxX ) {
			prevX = prevX + space / 3.0;
			if( maxX >= 0.0 && prevX > maxX ) {
				prevX = maxX;
			}

			xyData->addPoint( prevX, 0.0 );
		}
	}
	else {
		for( unsigned int i = 0; i < dps.getLength(); i++ ) {
			xyData->addPoint( dps.getX( i ), dps.getY( i ) );
		}
	}
}

// set x range
void Spectrum::setXRange( const double minX, const double maxX ) {
	setMinX( minX );
	setMaxX( maxX );
}

// set min x
void Spectrum::setMinX( const double minX ) {
	if( minX >= 0.0 ) {
		m_props.setDoubleValue( "Low Mass", minX );
	}
	else  {
		m_props.setValue( "Low Mass", "" );
	}

	m_minX = minX;
}

// get min x
double Spectrum::getMinX() {
	firstAccessProcess( );			// @Date:2013/07/16	<Add>	A.Ozaki
	// check the member
	if( !m_minX ) {
		double minX = double();
		double maxX = double();

		onGetXRange( &minX, &maxX );

		setXRange( minX, maxX );
	}

	return m_minX.get();
}

// set max x
void Spectrum::setMaxX( const double maxX ) {
	if( maxX >= 0.0 ) {
		m_props.setDoubleValue( "High Mass", maxX );
	}
	else {
		m_props.setValue( "High Mass", "" );
	}

	m_maxX = maxX;
}

// get max x
double Spectrum::getMaxX() {
	firstAccessProcess( );			// @Date:2013/07/16	<Add>	A.Ozaki
	// check the member
	if( !m_maxX ) {
		double minX = double();
		double maxX = double();

		onGetXRange( &minX, &maxX );

		setXRange( minX, maxX );
	}

	return m_maxX.get();
}

// set total intensity
void Spectrum::setTotalIntensity( const double intensity ) {
	if( intensity > 0.0 ) {
		m_props.setDoubleValue( "TIC", intensity );
		m_tic = intensity;
	}
	else {
		m_props.setValue( "TIC", "" );
	}
}

// get total intensity
double Spectrum::getTotalIntensity( const double minX, const double maxX ) {
	firstAccessProcess( );			// @Date:2013/07/16	<Add>	A.Ozaki
	// check member
	if( minX < 0.0 && maxX < 0.0 ) {
		if( m_tic ) {
			return m_tic.get();
		}
	}

	// get total intensity
	double intensity = onGetTotalIntensity( minX, maxX );

	// set tic
	if( minX < 0.0 && maxX < 0.0 ) {
		setTotalIntensity( intensity );
	}

	return intensity;
}

// set max intensity
void Spectrum::setMaxIntensity( const double intensity ) {
	if( intensity > 0.0 ) {
		m_props.setDoubleValue( "BPI", intensity );
		m_bpi = intensity;
	}
	else {
		m_props.setValue( "BPI", "" );
	}
}

// get max intensity
double Spectrum::getMaxIntensity( const double minX, const double maxX ) {
	firstAccessProcess( );			// @Date:2013/07/16	<Add>	A.Ozaki
	// check member
	if( minX < 0.0 && maxX < 0.0 ) {
		if( m_bpi ) {
			return m_bpi.get();
		}
	}

	// get max intensity
	double intensity = onGetMaxIntensity( minX, maxX );

	// set bpc
	if( minX < 0.0 && maxX < 0.0 ) {
		setMaxIntensity( intensity );
	}

	return intensity;
}

// set base peak mass
void Spectrum::setBasePeakMass( const double mass ) {
	if( mass >= 0.0 ) {
		m_props.setDoubleValue( "BPM", mass );
	}
	else {
		m_props.setValue( "BPM", "" );
	}

	m_bpm = mass;
}

// get Base Peak Mass
double Spectrum::getBasePeakMass() {
	firstAccessProcess( );			// @Date:2013/07/16	<Add>	A.Ozaki
	return m_bpm;
}

// set ms stage
void Spectrum::setMsStage( const int stage ) {
	// properties
	if( stage > 0 ) {
		m_props.setIntValue( "Stage", stage );
	}
	else {
		m_props.setValue( "Stage", "" );
	}

	for( int i = 1; i < m_msStage; i++ ) {
		std::string key = FMT( "Precursor%d (Mass)", i );
		m_props.setValue( key.c_str(), "" );

		key = FMT( "Precursor%d (Intensity)", i );
		m_props.setValue( key.c_str(), "" );
	}

	// stage
	m_msStage = stage;

	// precursor
	int idx = -1;
	double prec = -1.0;
	double intensity = -1.0;

	for( unsigned int i = 0; i < m_precursor.size() && idx < 0; i++ ) {
		if( m_precursor[ i ].stage < 0 ) {
			idx = (int)i;
			prec = m_precursor[ i ].precursor;
			intensity = m_precursor[ i ].intensity;
		}
	}

	if( idx >= 0 ) {
		m_precursor.erase( m_precursor.begin() + idx );

		if( prec >= 0.0 ) {
			setPrecursor( stage - 1, prec );
		}

		if( intensity >= 0.0 ) {
			setPrecursorIntensity( stage - 1, intensity );
		}
	}
}

// get ms stage
int Spectrum::getMsStage() {
	firstAccessProcess( );			// @Date:2013/07/16	<Add>	A.Ozaki
	return m_msStage;
}

// set precursor
void Spectrum::setPrecursor( const int stage, const double precursor ) {
	// property
	if( stage >= 1 ) {
		std::string key = FMT( "Precursor%d (Mass)", stage );
		m_props.setDoubleValue( key.c_str(), precursor );

		key = FMT( "Precursor%d (Intensity)", stage );
		if( !m_props.hasKey( key.c_str() ) ) {
			m_props.setValue( key.c_str(), "" );
		}
	}

	if( m_msStage > 1 && stage == m_msStage - 1 ) {
		m_props.setDoubleValue( "Precursor", precursor );
	}

	// search
	for( unsigned int i = 0; i < m_precursor.size(); i++ ) {
		if( m_precursor[ i ].stage == stage ) {
			m_precursor[ i ].precursor = precursor;
			return;
		}
	}

	// add
	m_precursor.resize( m_precursor.size() + 1 );
	m_precursor.back().stage = stage;
	m_precursor.back().intensity = -1.0;
	m_precursor.back().precursor = precursor;
}

// set precursor
void Spectrum::setPrecursor( const double precursor ) {
	// stage
	if( m_msStage > 1 ) {
		setPrecursor( m_msStage - 1, precursor );
	}
	else {
		setPrecursor( -1, precursor );
	}
}

// get precursor
double Spectrum::getPrecursor( const int stage ) {
	firstAccessProcess( );			// @Date:2013/07/16	<Add>	A.Ozaki
	for( unsigned int i = 0; i < m_precursor.size(); i++ ) {
		if( m_precursor[ i ].stage == stage ) {
			return m_precursor[ i ].precursor;
		}
	}

	return -1.0;
}

// get precursor
double Spectrum::getPrecursor() {
	firstAccessProcess( );			// @Date:2013/07/16	<Add>	A.Ozaki
	int stage = ( m_msStage > 1 ? ( m_msStage - 1 ) : -1 );
	return getPrecursor( stage );
}

// set precursor intensity
void Spectrum::setPrecursorIntensity( const int stage, const double intensity ) {
	// check parameters
	if( intensity < 0.0 ) {
		return;
	}

	// property
	if( stage >= 1 ) {
		std::string key = FMT( "Precursor%d (Mass)", stage );
		if( !m_props.hasKey( key.c_str() ) ) {
			m_props.setValue( key.c_str(), "" );
		}

		key = FMT( "Precursor%d (Intensity)", stage );
		m_props.setDoubleValue( key.c_str(), intensity );
	}

	if( m_msStage > 1 && stage == m_msStage - 1 ) {
		m_props.setDoubleValue( "Precursor Intensity", intensity );
	}

	// search
	for( unsigned int i = 0; i < m_precursor.size(); i++ ) {
		if( m_precursor[ i ].stage == stage ) {
			m_precursor[ i ].intensity = intensity;
			return;
		}
	}

	// add
	m_precursor.resize( m_precursor.size() + 1 );
	m_precursor.back().stage = stage;
	m_precursor.back().precursor = -1.0;
	m_precursor.back().intensity = intensity;
}

// set precursor intensity
void Spectrum::setPrecursorIntensity( const double intensity ) {
	if( m_msStage > 1 ) {
		setPrecursorIntensity( m_msStage - 1, intensity );
	}
	else {
		setPrecursorIntensity( -1, intensity );
	}
}

// get precursor intensity
double Spectrum::getPrecursorIntensity( const int stage ) {
	firstAccessProcess( );			// @Date:2013/07/16	<Add>	A.Ozaki
	// get intensity
	double intensity = -1.0;
	for( unsigned int i = 0; i < m_precursor.size(); i++ ) {
		if( m_precursor[ i ].stage == stage ) {
			intensity = m_precursor[ i ].intensity;
		}
	}
	if( intensity >= 0.0 ) {
		return intensity;
	}

	// get from parent spectrum
	Spectrum* parent = getParentSpectrum();
	double precursor = getPrecursor( stage );
	if( precursor < 0.0 ) {
		return -1.0;
	}

	std::set< Spectrum* > parentSet;
	parentSet.insert( this );
	parentSet.insert( parent );
	while( parent != NULL && parent->getMsStage() != stage ) {
		parent = parent->getParentSpectrum();
		if( parentSet.find( parent ) == parentSet.end() ) {
			parentSet.insert( parent );
		}
		else {
			parent = NULL;
		}
	}

	if( parent != NULL ) {
		intensity = 0.0;
		kome::core::DataPoints dps;
		parent->getXYData( &dps, precursor - 0.5, precursor + 0.5, false );

		for( unsigned int i = 0; i < dps.getLength(); i++ ) {
			intensity = std::max( intensity, dps.getY( i ) );
		}   
		setPrecursorIntensity( stage, intensity );
	}

	return intensity;
}

// get precursor intensity
double Spectrum::getPrecursorIntensity() {
	firstAccessProcess( );			// @Date:2013/07/16	<Add>	A.Ozaki
	int stage = ( m_msStage > 1 ? ( m_msStage - 1 ) : -1 );
	return getPrecursorIntensity( stage );
}

// set precursor charge
void Spectrum::setPrecursorCharge( const int charge ) {
	m_charge = charge;

	if( charge > 0 ) {
		m_props.setIntValue( "Precursor Charge", charge );
	}
	else {
		m_props.setValue( "Precursor Charge", "" );
	}
}

// get precursor charge
int Spectrum::getPrecursorCharge() {
	return m_charge;
}

// set parent spectrum
void Spectrum::setParentSpectrum( Spectrum* const parent ) {
	// set parent spectrum
	m_parentSpec = parent;

	// precursor
	std::set< Spectrum* > parentSet;
	parentSet.insert( this );
	parentSet.insert( parent );
	Spectrum* p = parent;
	while( p != NULL ) {
		int stage = p->getMsStage() - 1;
		if( stage > 0 ) {
			double precursor = getPrecursor( stage );
			if( precursor < 0.0 ) {
				precursor = p->getPrecursor();
				setPrecursor( stage, precursor );
			}
		}
		p = p->getParentSpectrum();
		if( parentSet.find( p ) == parentSet.end() ) {
			parentSet.insert( p );
		}
		else { 
			p = NULL;
		}
	}
}

// get parent spectrum
Spectrum* Spectrum::getParentSpectrum() {
	return m_parentSpec;
}

// get child spectra
void Spectrum::getChildSpectra( std::vector< Spectrum* >& children ) {
	// sample
	Sample* smpl = getSample();
	if( smpl == NULL ) {
		return;
	}

	// get children
	if( m_children.size() == 0 ) {
		// spectra
		for( unsigned int i = 0; i < smpl->getNumberOfGroups(); i++ ) {
			DataGroupNode* group = smpl->getGroup( i );
			for( unsigned int j = 0; j < group->getNumberOfSpectra(); j++ ) {
				Spectrum* spec = group->getSpectrum( j );
				if( spec->getParentSpectrum() == this ) {
					m_children.push_back( spec );
				}
			}
		}
	}

	// store
	for( unsigned int i = 0; i < m_children.size(); i++ ) {
		children.push_back( m_children[ i ] );
	}
}

// set whether this spectrum has chromatogram
void Spectrum::setHasChromatogram( const bool chromatogram ) {
	m_chrom = chromatogram;
}

// judge whether this spectrum has chromatogram
bool Spectrum::hasChromatogram() {
	return m_chrom;
}

// set spectrum group
void Spectrum::setGroup( DataGroupNode* group ) {
	m_group = group;
}

// get spectrum group
DataGroupNode* Spectrum::getGroup() {
	return m_group;
}

// set scan number
void Spectrum::setScanNumber( const int scan ) {
	if( scan >= 0 ) {
		m_props.setIntValue( "Scan Number", scan );
	}
	else {
		m_props.setValue( "Scan Number", "" );
	}

	m_scanNumber = scan;
}

// get scan number
int Spectrum::getScanNumber() {
	return m_scanNumber;
}

// set polarity
void Spectrum::setPolarity( Polarity polarity ) {
	m_props.setValue(
		"Polarity",
		( polarity == POLARITY_POSITIVE ? "+" : (polarity == POLARITY_NEGATIVE ? "-" : "" ) )
	);

	m_polarity = polarity;
}

// get polarity
Spectrum::Polarity Spectrum::getPolarity() {
	return m_polarity;
}

// set centroid mode
void Spectrum::setCentroidMode( const bool centroidMode ) {
	m_props.setBoolValue( "Centroid Mode", centroidMode );
	m_centroidMode = centroidMode;
}

// get centroid mode
bool Spectrum::isCentroidMode() {
	return m_centroidMode;
}

// set resolution
void Spectrum::setResolution( const double resolution ) {
	m_props.setDoubleValue( "Resolution", resolution );
	m_resolution = resolution;
}

// get resolution
double Spectrum::getResolution() {
	return m_resolution;
}

// set collision energy
void Spectrum::setCollisionEnergy( const char* collisionEnergy ) {
	m_props.setValue( "Collision Energy", NVL( collisionEnergy, "" ) );
	m_collisionEnergy = NVL( collisionEnergy, "" );
}

// get collision energy
const char* Spectrum::getCollisionEnergy() {
	return m_collisionEnergy.c_str();
}

// get properties
kome::core::Properties& Spectrum::getProperties() {
	return m_props;
}

// set visible flag value
void Spectrum::setVisible( const bool visible ) {
	m_visible = visible;
}

// get visible flag value
bool Spectrum::isVisible() {
	return m_visible;
}

// set auto zero points flag
void Spectrum::setAutoZeroPoints( const bool autoZero ) {
	m_autoZeroPoints = autoZero;
}

// get auto zero points flag
bool Spectrum::isAutoZeroPoints() {
	return m_autoZeroPoints;
}

// get properties
kome::core::Properties& Spectrum::getUserProperties() {
	return m_userProps;
}

// set original spectrum
void Spectrum::setOrgSpectrum( Spectrum* spec ) {
	m_orgSpec = spec;
}

// get original spectrum
Spectrum* Spectrum::getOrgSpectrum() {
	if( m_orgSpec == NULL ) {
		m_orgSpec = this;
	}

	return m_orgSpec;
}

// get properties
void Spectrum::getProperties( kome::core::Properties& properties ) {
	firstAccessProcess( );			// @Date:2013/07/16	<Add>	A.Ozaki
	// get spectrum properties
	for( unsigned int i = 0; i < m_props.getNumberOfProperties(); i++ ) {
		properties.setValue( m_props.getKey( i ), m_props.getValue( i ) );
	}

	// get peak information
	PeaksManager& pkMgr = PeaksManager::getInstance();
	Peaks* peaks = pkMgr.getPeaks( this );
	if( peaks != NULL ) {
		int peakNum = peaks->getLength();
		double precursor = peaks->getPrecursor();
		std::string chargeStr;
		for( unsigned int i = 0; i < peaks->getNumberOfCharges(); i++ ) {
			int charge = peaks->getCharge( i );
			if( charge > 0 ) {
				if( !chargeStr.empty() ) {
					chargeStr.append( ", " );
				}
			}
			chargeStr.append( FMT( "%d", charge ) );
		}

		properties.setIntValue( "Peaks Count", peakNum );
		if( precursor < 0.0 ) {
			properties.setValue( "Calculated Precursor", "" );
		}
		else {
			properties.setDoubleValue( "Calculated Precursor", precursor );
		}
		properties.setValue( "Calculated Precursor Charge", chargeStr.c_str() );
	}

	// get group properties
	DataGroupNode* group = m_group;

	while( group != NULL ) {
		for( unsigned int i = 0; i < group->getProperties().getNumberOfProperties(); i++ ) {
			properties.setValue(
				group->getProperties().getKey( i ),
				group->getProperties().getValue( i )
			);
		}
		group = group->getParentGroup();
	}
}

// get user properties
void Spectrum::getUserProperties( kome::core::Properties& userProperties ) {
	firstAccessProcess( );			// @Date:2013/07/16	<Add>	A.Ozaki
	// get spectrum user properties
	for( unsigned int i = 0; i < m_userProps.getNumberOfProperties(); i++ ) {
		userProperties.setValue( m_userProps.getKey( i ), m_userProps.getValue( i ) );
	}

	// get group user properties
	DataGroupNode* group = m_group;

	while( group != NULL ) {
		for( unsigned int i = 0; i < group->getUserProperties().getNumberOfProperties(); i++ ) {
			userProperties.setValue(
				group->getUserProperties().getKey( i ),
				group->getUserProperties().getValue( i )
			);
		}
		group = group->getParentGroup();
	}
}

// check the property key
bool Spectrum::isCommonProperty( const char* key ) {
	// common properties
	const char* keys[] = {
		"Name",
		"RT",
		"Start RT",
		"End RT",
		"Low Mass",
		"High Mass",
		"TIC",
		"BPI",
		"BPM",
		"Stage",
		"Precursor",
		"Precursor Intensity",
		"Scan Number",
		"Polarity",
		"Centroid Mode",
		"Resolution",
		"Collision Energy",
		"Spectrum Type",
		"Title"
	};

	// create array
	if( m_commonProps.size() == 0 ) {
		const unsigned int num = sizeof( keys ) / sizeof( const char* );

		for( unsigned int i = 0; i < num; i++ ) {
			m_commonProps.push_back( keys[ i ] );
		}
	}

	// check the key
	if( key == NULL ) {
		return false;
	}

	for( unsigned int i = 0; i < m_commonProps.size(); i++ ) {
		if( m_commonProps[ i ].compare( key ) == 0 ) {
			return true;
		}
	}

	return false;
}

// set spectrum id
void Spectrum::setId( int id ){
	m_specId = id;
}

// get spectrum id
int Spectrum::getId(){
	return m_specId;
}

// >>>>>> @Date:2013/07/16	<Add>	A.Ozaki
//

// set spot id
void	Spectrum::setSpotId( const char *pcSpotId )
{
	m_strSpotId = std::string( NVL( pcSpotId, "" ) );
	m_props.setValue( "Spot ID", m_strSpotId.c_str( ) );	// @Date:2013/08/20	<Add>	A.Ozaki

	return;
}

// get spot id
const char	*Spectrum::getSpotId( void )
{
	firstAccessProcess( );
	return	m_strSpotId.c_str( );
}

// set the flag of request load data
void	Spectrum::setRequestLoadData( void )
{
	onSetRequestLoadData( );

	return;
}

// reset the flag of request load data
void	Spectrum::resetRequestLoadData( void )
{
	onResetRequestLoadData( );

	return;
}

// check the flag of request load data
bool	Spectrum::isRequestLoadData( void )
{
	return	onIsRequestLoadData( );
}

// set the flag of first access
void	Spectrum::setFirstAccess( void )
{
	onSetFirstAccess( );

	return;
}

// reset the flag of first access
void	Spectrum::resetFirstAccess( void )
{
	onResetFirstAccess( );

	return;
}

// check the flag of first access
bool	Spectrum::isFirstAccess( void  )
{
	return	onIsFirstAccess( );
}

// load data
bool	Spectrum::loadData( void )
{
	return	onLoadData( );
}

// set the flag of request load data (virtual)
void	Spectrum::onSetRequestLoadData( void )
{
	return;
}

// set the flag of request load data (virtual)
void	Spectrum::onResetRequestLoadData( void )
{
	return;
}

// check the flag of request load data (virtual)
bool	Spectrum::onIsRequestLoadData( void )
{
	return	false;
}

// set the flag of first access (virtual)
void	Spectrum::onSetFirstAccess( void )
{
	return;
}

// reset the flag of first access (virtual)
void	Spectrum::onResetFirstAccess( void )
{
	return;
}

// check the flag of first access (virtual)
bool	Spectrum::onIsFirstAccess( void  )
{
	return	false;
}

// load data (virtual)
bool	Spectrum::onLoadData( void )
{
	return	true;
}

// process of first access
bool	Spectrum::firstAccessProcess( void )
{
	bool	bRet = true;
	if  ( isFirstAccess( ) && isRequestLoadData( ) )
	{
		bRet = loadData( );
		resetFirstAccess( );
	}

	return	bRet;
}

//
// <<<<<< @Date:2013/07/11	<Add>	A.Ozaki




