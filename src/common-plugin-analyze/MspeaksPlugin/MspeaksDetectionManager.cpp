/**
 * @file MspeaksDetection.h
 * @brief interfaces of MspeaksDetection class
 *
 * @author S.Tanaka
 * @date 2012.03.16
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "MspeaksDetectionManager.h"

#include <math.h>
#include <float.h>
#include <boost/bind.hpp>

// for use Matlab libraries
//
#include	"BioinformaticsToolboxFunc.h"
#pragma comment( lib, "BioinformaticsToolboxFunc" )
#pragma comment( lib, "mclmcrrt" )
//
//


using namespace kome::peaks::mspeaks;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define	ERROR_MESSAGE_APPL_INITIALIZE	( "BioinformaticsToolboxFunc library cannot be initialized.\nIf MATLAB Compiler Runtime Routine (MCR) ver.R2013a was not installed, please install it from flollwing web site:\nhttp://www.mathworks.com/products/compiler/mcr/index.html" )
#define	ERROR_MESSAGE_INITIALIZE		( "BioinformaticsToolboxFunc library cannot be initialized." )
#define	ERROR_MESSAGE_FUNCTION			( "Expection error occured." )
#define	FUNCTION_NAME					( "Mspeaks" )


// constructor
MspeaksDetectionManager::MspeaksDetectionManager( void )
{

	// initialize application for MATLAB
	if  ( false == initializeApplication( ) )
	{
		LOG_ERROR( FMT( "initializeApplication function failure." ) );
		return;
	}

	// initialize MATLAB function libray
	//
	if  ( (kome::window::DialogProgress *)NULL != m_pProgress )
	{
		m_pProgress->setPosition( 3 );
		m_pProgress->setStatus( "MATLAB function initializing..." );
	}
	if  ( false == BioinformaticsToolboxFuncInitialize( ) )
	{
		// initialize false
		LOG_ERROR( FMT( "BioinformaticsToolboxFuncInitialize function failure." ) );

		wxMessageBox( wxT( ERROR_MESSAGE_INITIALIZE ), wxT( FUNCTION_NAME ), wxOK );

	//		break;
	}


		m_pProgress->fill();
		delete m_pProgress;


	m_pProgress = (kome::window::DialogProgress *)NULL;
	return;
}

// destructor
MspeaksDetectionManager::~MspeaksDetectionManager( void )
{

    // release MATLAB function libray
	try
	{
		BioinformaticsToolboxFuncTerminate( );
	}
	catch (...)
	{
		// 何もしない
	}

	
	// release application
	//
	kome::core::MsppManager	&insMsppMgr = kome::core::MsppManager::getInstance( );
	if  ( true == insMsppMgr.isInitializedMATLAB( ) )
	{
		// initialized MATLAB application
		try
		{
			mclTerminateApplication( );

			// reset flag
			insMsppMgr.resetInitializedMATLAB( );
		}
		catch (...)
		{
		}
	}

	return;
}

// get instance
MspeaksDetectionManager& MspeaksDetectionManager::getInstance( void )
{
	// create object (This is the only object.)
	static	MspeaksDetectionManager		insMgr;

	return	insMgr;
}

// set base
void	MspeaksDetectionManager::setBase( const int nBase )
{
	m_nBase = nBase;

	return;
}

// get base
int	MspeaksDetectionManager::getBase( void )
{
	return	m_nBase;
}

// set levels
void	MspeaksDetectionManager::setLevels( const int nLevels )
{
	m_nLevels = nLevels;

	return;
}

// get Levels
int	MspeaksDetectionManager::getLevels( void )
{
	return	m_nLevels;
}

// set noise
void	MspeaksDetectionManager::setNoise( const int nNoise )
{
	m_nNoise = nNoise;

	return;
}

// get noise
int	MspeaksDetectionManager::getNoise( void )
{
	return	m_nNoise;
}

// set multipliner
void	MspeaksDetectionManager::setMultipliner( const double dMultipliner )
{
	m_dMultipliner = dMultipliner;

	return;
}

// get multipliner
double	MspeaksDetectionManager::getMultipliner( void )
{
	return	m_dMultipliner;
}

// set denoising
void	MspeaksDetectionManager::setDenoising( const int nDenoising )
{
	m_nDenoising = nDenoising;

	return;
}

// get denoising
int	MspeaksDetectionManager::getDenoising( void )
{
	return	m_nDenoising;
}

// set peak location
void	MspeaksDetectionManager::setPeakLocation( const double dPeakLocation )
{
	m_dPeakLocation = dPeakLocation;

	return;
}

// get peak location
double	MspeaksDetectionManager::getPeakLocation( void )
{
	return	m_dPeakLocation;
}

// set FWHMFilter
void	MspeaksDetectionManager::setFWHMFilter( const double dFWHMFilter )
{
	m_dFWHMFilter = dFWHMFilter;

	return;
}

// get FWHMFilter
double	MspeaksDetectionManager::getFWHMFilter( void )
{
	return	m_dFWHMFilter;
}

// set over segmentation
void	MspeaksDetectionManager::setOverSegmentation( const double dOverSegmentation )
{
	m_dOverSegmentation = dOverSegmentation;

	return;
}

// get over segmentation
double	MspeaksDetectionManager::getOverSegmentation( void )
{
	return	m_dOverSegmentation;
}

// initialize MATLAB application
bool	MspeaksDetectionManager::initializeApplication( void )
{
	kome::core::MsppManager	&insMsppMgr = kome::core::MsppManager::getInstance( );

	// check initialize
	bool	bRet = insMsppMgr.isInitializedMATLAB( );
	if  ( false == bRet )
	{
		// uninitialize

		// create progress dialog
		if  ( (kome::window::DialogProgress *)NULL != m_pProgress )
		{
			delete	m_pProgress;
		}
		if  ( (wxWindow *)NULL != kome::window::WindowTool::getMainWindow() )
		{
			m_pProgress = new kome::window::DialogProgress( kome::window::WindowTool::getMainWindow(), FUNCTION_NAME );
		}
		if  ( (kome::window::DialogProgress *)NULL != m_pProgress )
		{
			m_pProgress->setStatus( "MATLAB application initialzing..." ); 
			m_pProgress->setRange( 1, 4 );
			m_pProgress->setPosition( 2 );
		}

		// initialize application
		//
		if  ( true == mclInitializeApplication( NULL, 0 ) )
		{
			// success
			insMsppMgr.setInitializedMATLAB( );
			bRet = true;
		}
		else
		{
			// failure
			LOG_ERROR( FMT( "mclInitializeApplication function failure." ) );

			wxMessageBox( wxT( ERROR_MESSAGE_APPL_INITIALIZE ), wxT( FUNCTION_NAME ), wxOK );

			if  ( (kome::core::Progress *)NULL != m_pProgress )
			{
				m_pProgress->fill( );
				delete	m_pProgress;
				m_pProgress = (kome::window::DialogProgress *)NULL;
			}
		}

		if  ( (kome::window::DialogProgress *)NULL != m_pProgress )
		{
			m_pProgress->setPosition( 3 );
		}
	}

	return	bRet;
}

// detect peaks
void	MspeaksDetectionManager::detection
(
	kome::core::XYData		&refXyData,
	kome::objects::Peaks	&refPeaks
)
{

	while ( 1 )
	{
		int	nIndex = 0;
		const int	nRowTemp = refXyData.getLength( );
		const int	nColumn = 2;		// 行列（m/zの１次元配列、intensityの１次元配列からなる２列）
		const int	nDataSizeTemp = nRowTemp * nColumn;

		double	*pdDataArray = (double *)NULL;

		pdDataArray = new double[nDataSizeTemp];
		if  ( (double *)NULL == pdDataArray )
		{
			LOG_ERROR( FMT( "can not allocate data array.(size:%dx%d)", sizeof( double ), nDataSizeTemp ) );
			break;
		}

		memset( (void *)pdDataArray, 0x00, sizeof( double ) * nDataSizeTemp );
	
		// m/z、intensityを設定します
		// ただし、intensityの値がゼロのものは、省いて設定する必要があります
		// mspeaksが例外で落ちてしまいます
		//
		int	nRow = 0;
		int	nIndexTemp = 0;
		for ( nIndex = 0 ; nIndex < nRowTemp ; nIndex++ )
		{
			if  ( 0 < refXyData.getY( nIndex ) )
			{
				*(pdDataArray + nRow) = refXyData.getX( nIndex );
				nRow++;
			}
		}
		for ( nIndex = 0 ; nIndex < nRowTemp ; nIndex++ )
		{
			if  ( 0 < refXyData.getY( nIndex ) )
			{
				*(pdDataArray + nIndexTemp + nRow) = refXyData.getY( nIndex );
				nIndexTemp++;
			}
		}
		const int	nDataSize = nRow * nColumn;

		// データ数の確認
		if  ( 0 >= nDataSize )
		{
			LOG_ERROR( FMT( "illegal data size (%d)", nDataSize ) );
			break;
		}

		mwArray	insArrayData( nRow, nColumn, mxDOUBLE_CLASS, mxREAL );

		// dDataArray -> insArrayData
		insArrayData.SetData( pdDataArray, nDataSize );
	
		//
		delete[]	pdDataArray;

		mwArray	insBase( m_nBase );
		mwArray	insLevels( m_nLevels );
		mwArray	insNoiseEstimator( m_nNoise );
		mwArray	insMultipliner( m_dMultipliner );
		mwArray	insDenoising( m_nDenoising );
		mwArray	insPeakLocation( m_dPeakLocation );
		mwArray	insFWHMFilter( m_dFWHMFilter );
		mwArray	insOverSegmentation( m_dOverSegmentation );

		const int	nArgOut = 1;		// size of return values

		mwArray	insPeakList;			// return

		bool	bSuccess = true;

		// execute MATLAB function
		//
		if  ( (kome::window::DialogProgress *)NULL != m_pProgress )
		{
			m_pProgress->setPosition( 4 );
			m_pProgress->setStatus( "Processing..." );
		}
		try
		{
			MatlabMspeaks( nArgOut, insPeakList, insArrayData,
							insBase, insLevels, insNoiseEstimator, insMultipliner,
							insDenoising, insPeakLocation, insFWHMFilter,
							insOverSegmentation );
		}
		catch (...)
		{
			LOG_ERROR( FMT( "MatlabMspeaks function failure." ) );
			bSuccess = false;
		}

		// post processing
		//
		if  ( true == bSuccess )
		{
			mwArray	insSize = insPeakList.GetDimensions( );

			int	nGetSize[2];

			insSize.GetData( nGetSize, _countof( nGetSize ) );

			int	nPeakListRow = nGetSize[0];
			int	nPeakListColumn = nGetSize[1];

			// check values
			if  ( ( 0 > nPeakListRow || nRow < nPeakListRow ) ||
				( 0 > nPeakListColumn || nColumn < nPeakListColumn ) )
			{
				// error
				LOG_ERROR( FMT( "unmatch row size (%d:%d) or column size (%d:%d)", nRow, nPeakListRow, nColumn, nPeakListColumn ) );
				break;
			}
			int	nGetDataSize = nPeakListRow * nPeakListColumn;
	
			double	*pdPeakList = new double[nGetDataSize];
			if  ( (double *)NULL == pdPeakList )
			{
				LOG_ERROR( FMT( "can not allocate data array.(size:%dx%d)", sizeof( double ), nGetDataSize ) );
				break;
			}

			memset( (void *)pdPeakList, 0x00, sizeof( double ) * nGetDataSize );

			// store dPeakList
			insPeakList.GetData( pdPeakList, nGetDataSize );

			for ( nIndex = 0 ; nIndex < nPeakListRow ; nIndex++ )
			{
				refPeaks.addPoint( *(pdPeakList + nIndex), *(pdPeakList + nIndex + nPeakListRow ) );
			}

			delete[]	pdPeakList;
		}

		break;
	}

	// clean up progress
	//
	if  ( (kome::window::DialogProgress *)NULL != m_pProgress )
	{
		m_pProgress->fill( );

		delete	m_pProgress;
		m_pProgress = (kome::window::DialogProgress *)NULL;
	}

	return;
}


// release MATLAB application
void	MspeaksDetectionManager::releaseMatlab( void )
{
	kome::core::MsppManager	&insMsppMgr = kome::core::MsppManager::getInstance( );

	// check initialize
	bool	bRet = insMsppMgr.isInitializedMATLAB( );
	if  ( true == bRet )
	{
		// already initialized
		try
		{
			// release application
			//
			mclTerminateApplication( );
		}
		catch (...)
		{
			// failure
			LOG_ERROR( FMT( "mclTerminateApplication function failure." ) );
		}

		// reset initialized flag
		//
		insMsppMgr.resetInitializedMATLAB( );
	}

	return;
}

//
