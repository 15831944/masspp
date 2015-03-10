/**
 * @file MslowessManager.h
 * @brief interfaces of MslowessManager class
 *
 * @author A.Ozaki
 * @date 2013.08.23
 * 
 * Copyright(C) 2013-2013 Shimadzu Corporation All rights reserved.
 */
   

#include "stdafx.h"
#include "MslowessManager.h"

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


using namespace kome::labeling::mslowess;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define	ERROR_MESSAGE_APPL_INITIALIZE	( "BioinformaticsToolboxFunc library cannot be initialized.\nIf MATLAB Compiler Runtime Routine (MCR) ver.R2013a was not installed, please install it from flollwing web site:\nhttp://www.mathworks.com/products/compiler/mcr/index.html" )
#define	ERROR_MESSAGE_INITIALIZE		( "BioinformaticsToolboxFunc library cannot be initialized." )
#define	ERROR_MESSAGE_FUNCTION			( "Expection error occured." )
#define	FUNCTION_NAME					( "Mslowess" )

////////////////////////////////////////
#define SPECTRUM_PEAK_PICKING_FUNC_TYPE			"SPEC_PEAK_PICKING"
////////////////////////////////////////



// constructor
MslowessManager::MslowessManager( void )
{
	m_pProgress = (kome::window::DialogProgress *)NULL;
	return;
}

// destructor
MslowessManager::~MslowessManager( void )
{
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
MslowessManager& MslowessManager::getInstance( void )
{
	// create object (This is the only object.)
	static	MslowessManager		insMgr;

	return	insMgr;
}

// set order
void	MslowessManager::setOrder( const int nOrder )
{
	m_nOrder = nOrder;

	return;
}

// get order
int	MslowessManager::getOrder( void )
{
	return	m_nOrder;
}

// set Span
void	MslowessManager::setSpan( const double dSpan )
{
	m_dSpan = dSpan;

	return;
}

// get Span
double	MslowessManager::getSpan( void )
{
	return	m_dSpan;
}

// set kernel
void	MslowessManager::setKernel( const int nKernel )
{
	m_nKernel = nKernel;

	return;
}

// get kernel
int	MslowessManager::getKernel( void )
{
	return	m_nKernel;
}

// set robust iterations
void	MslowessManager::setRobustIterations( const double dRobustIterations )
{
	m_dRobustIterations = dRobustIterations;

	return;
}

// get borust iterations
double	MslowessManager::getRobustIterations( void )
{
	return	m_dRobustIterations;
}

// set S/N
void	MslowessManager::setSn( const double dSn )
{
	m_dSn = dSn;

	return;
}

// get S/N
double	MslowessManager::getSn( void )
{
	return	m_dSn;
}

// initialize MATLAB application
bool	MslowessManager::initializeApplication( void )
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

// filtering
kome::objects::Variant	MslowessManager::filtering
(
	kome::objects::Parameters* params
)
{
	// return value
	kome::objects::Variant ret;
	ret.prim.boolVal = false;
	ret.type = kome::objects::Variant::BOOL;

	// spectrum
	kome::objects::Spectrum	*pSpec = kome::plugin::PluginCallTool::getSpectrum( params );
	if  ( (kome::objects::Spectrum *)NULL == pSpec )
	{
		return	ret;
	}

	// peaks 
	kome::objects::Peaks	*pPeaks = kome::plugin::PluginCallTool::getPeaks( params );
	if  ( (kome::objects::Peaks *)NULL == pPeaks)
	{
		return	ret;
	}

	// setting parameter values
	kome::objects::SettingParameterValues	*pSettings = kome::plugin::PluginCallTool::getSettingValues( params );
	if  ( (kome::objects::SettingParameterValues *)NULL == pSettings )
	{
		return	ret;
	}

	// xy data
	kome::core::XYData	*pXyData = kome::plugin::PluginCallTool::getXYData( params );
	if  ( (kome::core::XYData *)NULL == pXyData )
	{
		return	ret;
	}

	// initialize application for MATLAB
	if  ( false == initializeApplication( ) )
	{
		LOG_ERROR( FMT( "initializeApplication function failure." ) );
		return	ret;
	}

	while	( true )
	{
		int	nIndex = 0;

#if 1
		const int	nRowTemp = pPeaks->getLength( );
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
		// mslowessが例外で落ちてしまいます
		//
		int	nRow = 0;
		int	nIndexTemp = 0;
		for ( nIndex = 0 ; nIndex < nRowTemp ; nIndex++ )
		{
			if  ( 0 < pPeaks->getY( nIndex ) )
			{
				*(pdDataArray + nRow) = pPeaks->getX( nIndex );
				nRow++;
			}
		}
		for ( nIndex = 0 ; nIndex < nRowTemp ; nIndex++ )
		{
			if  ( 0 < pPeaks->getY( nIndex ) )
			{
				*(pdDataArray + nIndexTemp + nRow) = pPeaks->getY( nIndex );
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

#else
		const int	nRow = pPeaks->getLength( );
		const int	nRowTemp = pPeaks->getLength( );
		const int	nColumn = 2;		// 行列（m/zの１次元配列、intensityの１次元配列からなる２列）
		const int	nDataSize = nRow * nColumn;

		double	*pdDataArray = (double *)NULL;

		pdDataArray = new double[nDataSize];
		if  ( (double *)NULL == pdDataArray )
		{
			break;
		}

		memset( (void *)pdDataArray, 0x00, sizeof( double ) * nDataSize );

		// dataを格納…
		for ( nIndex = 0 ; nIndex < nRow ; nIndex++ )
		{
			*(pdDataArray + nIndex) = pPeaks->getX( nIndex );
			*(pdDataArray + nIndex + nRow ) = pPeaks->getY( nIndex );
		}
#endif
		mwArray	insArrayData( nRow, nColumn, mxDOUBLE_CLASS, mxREAL );

		// dDataArray -> insArrayData
		insArrayData.SetData( pdDataArray, nDataSize );
	
		//
		delete[]	pdDataArray;

		mwArray	insOrder( m_nOrder );
		mwArray	insSpan( m_dSpan );
		mwArray	insKernel( m_nKernel );
		mwArray	insRobustIterations( m_dRobustIterations );

		const int	nArgOut = 1;		// size of return values

		mwArray	insLowess;				// return

		bool	bSuccess = true;

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

			return	ret;
		}

		// execute MATLAB function
		//
		if  ( (kome::window::DialogProgress *)NULL != m_pProgress )
		{
			m_pProgress->setPosition( 4 );
			m_pProgress->setStatus( "Processing..." );
		}
		try
		{
			MatlabMslowess( nArgOut, insLowess, insArrayData,
							insOrder, insSpan, insKernel,
							insRobustIterations );
		}
		catch (...)
		{
			LOG_ERROR( FMT( "MatlabMslowess function failure." ) );
			bSuccess = false;
		}

		// release MATLAB function libray
		//
		try
		{
			BioinformaticsToolboxFuncTerminate( );
		}
		catch (...)
		{
		}

		if  ( true == bSuccess )
		{
			mwArray	insSize = insLowess.GetDimensions( );

			int	nGetSize[1];

			insSize.GetData( nGetSize, _countof( nGetSize ) );

			int	nCountOfLowess = nGetSize[0];

			// check values
			if  ( nRow != nCountOfLowess )
			{
				// error
				LOG_ERROR( FMT( "unmatch row size (%d:%d).", nRow, nCountOfLowess ) );
				break;
			}
	
			double	*pdLowess = new double[nCountOfLowess];
			if  ( (double *)NULL == pdLowess )
			{
				LOG_ERROR( FMT( "can not allocate data array.(size:%dx%d)", sizeof( double ), nCountOfLowess ) );
				break;
			}

			memset( (void *)pdLowess, 0x00, sizeof( double ) * nCountOfLowess );

			// store
			insLowess.GetData( pdLowess, nCountOfLowess );

			// processing
			double	dSumDiff = 0.0;
			double	dSumSqDiff = 0.0;
			double	dDiff = 0.0;

			for ( nIndex = 0 ; nIndex < nRow ; nIndex++ )
			{
				dDiff = pPeaks->getY( nIndex ) - *(pdLowess + nIndex);
				dSumDiff += dDiff;
				dSumSqDiff += dDiff * dDiff;
			}

			double	dSigma = sqrt( ( dSumSqDiff / nRow )
									- ( dSumDiff / nRow ) * ( dSumDiff / nRow ) );	// @Date:2013/08/20	<Modify>	A.Ozaki

			double	dSNThreshold = m_dSn * dSigma;

			kome::objects::PeaksManager	&insPeaksMgr = kome::objects::PeaksManager::getInstance( );
	
			std::vector<kome::objects::PeakElement>	insAddPeak;
	
			for ( nIndex = 0 ; nIndex < (int)pPeaks->getLength( ) ; nIndex++ )
			{
			// WMDなど、ピーク面積を内部的にピーク強度として扱っているものもあるので、ピーク強度(getY(nIndex))でなくピーク頂点(newPeak->getApexY())でフィルターを掛ける。
			//	if  ( pPeaks->getY(nIndex) >  dSNThreshold )    @Date:2014/02/26	<Del>	Y.Fujita
			//	{
				kome::objects::PeakElement*	pNewPeak = NULL;

				pNewPeak = pPeaks->getPeak( nIndex );
				double temp1 = pNewPeak->getApexY();
				double temp2 = pPeaks->getY(nIndex);
				if ( pNewPeak->getApexY() >=  dSNThreshold )      // @Date:2014/02/20	<Add>	Y.Fujita
				{
					if  ( (kome::objects::PeakElement *)NULL != pNewPeak )
					{
						pNewPeak->setRight( pNewPeak->getRightX( ), pNewPeak->getRightY( ) );
						pNewPeak->setApex( pNewPeak->getApexX( ), pNewPeak->getApexY( ) );
						pNewPeak->setId( pNewPeak->getId( ) );
						insAddPeak.push_back( *pNewPeak );
					}
				}
			}

			insPeaksMgr.updatePeaks( *pSpec, pPeaks, insAddPeak );
			kome::plugin::PluginCallTool::onUpdateSpecPeaks( *pSpec );
			kome::plugin::PluginCallTool::onUpdateSpectrum( *pSpec );

			ret.prim.boolVal = true;

			delete[]	pdLowess;
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

	return ret;

}

// release MATLAB application
void	MslowessManager::releaseMatlab( void )
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
