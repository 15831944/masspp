/**
 * @file MsbackadjBaselineDetectionManager.cpp
 * @brief implements of MsbackadjBaselineDetectionManager class
 *
 * @author A.Ozaki
 * @date 2013.08.01
 * 
 * Copyright(C) 2013-2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "MsbackadjBaselineDetectionManager.h"

#include <math.h>
#include <float.h>


using namespace kome::baseline::msbackadj;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// for use Matlab libraries
//
#include	"BioinformaticsToolboxFunc.h"
#pragma comment( lib, "BioinformaticsToolboxFunc" )
#pragma comment( lib, "mclmcrrt" )
//
//

// for Error Message on Dialog
#define	ERROR_MESSAGE_APPL_INITIALIZE	( "BioinformaticsToolboxFunc library cannot be initialized.\nIf MATLAB Compiler Runtime Routine (MCR) ver.R2013a was not installed, please install it from flollwing web site:\nhttp://www.mathworks.com/products/compiler/mcr/index.html" )
#define	ERROR_MESSAGE_INITIALIZE		( "BioinformaticsToolboxFunc library cannot be initialized." )
#define	FUNCTION_NAME					( "Msbackadj" )


// constructor
MsbackadjBaselineDetectionManager::MsbackadjBaselineDetectionManager( void )
{
	// initialize application
	//
	m_pProgress = (kome::window::DialogProgress *)NULL;

	// set default value
	m_dWindowSize = 200.0;
	m_dStepSize = 50.0;
	m_nRegressionMethod = 0;
	m_nEstimationMethod = 0;
	m_nSmoothingMethod = 0;
	m_dQuantile = 0.1;

	return;
}

// destructor
MsbackadjBaselineDetectionManager::~MsbackadjBaselineDetectionManager( void )
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

// set window size
void	MsbackadjBaselineDetectionManager::setWindowSize( const double dSize )
{
	m_dWindowSize = MAX( 0.0, dSize );
}

// get window size
double	MsbackadjBaselineDetectionManager::getWindowSize( void )
{
	return	m_dWindowSize;
}

// set step size
void	MsbackadjBaselineDetectionManager::setStepSize( const double dSize )
{
	m_dStepSize = MAX( 0.0, dSize );
}

// get step size
double	MsbackadjBaselineDetectionManager::getStepSize( )
{
	return	m_dStepSize;
}

// set regression method
void	MsbackadjBaselineDetectionManager::setRegressionMethod( const int nMethodValue )
{
	m_nRegressionMethod = MAX( 0, nMethodValue );
}

// get regression method
int	MsbackadjBaselineDetectionManager::getRegressionMethod( void )
{
	return	m_nRegressionMethod;
}

// set estimation method
void	MsbackadjBaselineDetectionManager::setEstimationMethod( const int nMethodValue )
{
	m_nEstimationMethod = MAX( 0, nMethodValue );
}

// get estimation method
int	MsbackadjBaselineDetectionManager::getEstimationMethod( void )
{
	return	m_nEstimationMethod;
}

// set smoothing method
void	MsbackadjBaselineDetectionManager::setSmoothingMethod( const int nMethodValue )
{
	m_nSmoothingMethod = MAX( 0, nMethodValue );
}

// get smoothing method
int	MsbackadjBaselineDetectionManager::getSmoothingMethod( void )
{
	return	m_nSmoothingMethod;
}

// set quantile
void	MsbackadjBaselineDetectionManager::setQuantile( const double dQuantile )
{
	m_dQuantile = MAX( 0.0, dQuantile );
}

// get quantile
double	MsbackadjBaselineDetectionManager::getQuantile( void )
{
	return	m_dQuantile;
}

// initialize MATLAB application
bool	MsbackadjBaselineDetectionManager::initializeApplication( void )
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
		if  ( (wxWindow *)NULL != kome::window::WindowTool::getMainWindow( ) )
		{
//			m_pProgress = new kome::window::DialogProgress( kome::window::WindowTool::getMainWindow( ), FUNCTION_NAME );
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
// get baseline
void	MsbackadjBaselineDetectionManager::getMsbackadj
(
	kome::core::XYData		&src,
	kome::core::XYData		&baseline
)
{
	// initialize application for MATLAB
	if  ( false == initializeApplication( ) )
	{
		LOG_ERROR( FMT( "initializeApplication function failure." ) );
		return;
	}

	while ( 1 )
	{
		int	nIndex = 0;

		const int	nRowTemp = src.getLength( );
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
		//
		int	nRow = 0;
		int	nIndexTemp = 0;
		
		// 強度の値が0のところを省く
		for ( nIndex = 0 ; nIndex < nRowTemp ; nIndex++ )
		{
			if  ( 0 < src.getY( nIndex ) )
			{
				*(pdDataArray + nRow) = src.getX( nIndex );
				nRow++;
			}
		}
		for ( nIndex = 0 ; nIndex < nRowTemp ; nIndex++ )
		{
			if  ( 0 < src.getY( nIndex ) )
			{
				*(pdDataArray + nIndexTemp + nRow) = src.getY( nIndex );
				nIndexTemp++;
			}
		}
		const int	nDataSize = nRow * nColumn;

		//  nRowTemp...raw dataのデータポイント数
		//  nRow、nIndexTemp....強度が0のデータポイントを除いたデータポイント数
		
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

		mwArray	insWindowSize( m_dWindowSize );
		mwArray	insStepSize( m_dStepSize );
		mwArray	insRegressionMethod( m_nRegressionMethod );
		mwArray	insEstimationMethod( m_nEstimationMethod );
		mwArray	insSmoothingMethod( m_nSmoothingMethod );
		mwArray	insQuantile( m_dQuantile);

		const int	nArgOut = 1;		// size of return values

		mwArray	insBackadj;			// return

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

			break;
		}
		    
		// execute MATLAB function
		if  ( (kome::window::DialogProgress *)NULL != m_pProgress )
		{
			m_pProgress->setPosition( 4 );
			m_pProgress->setStatus( "Processing..." );
		}
		try   
		{
			MatlabMsbackadj( nArgOut, insBackadj, insArrayData,
							insWindowSize, insStepSize, insRegressionMethod, insEstimationMethod,
							insSmoothingMethod, insQuantile );

			std::string	strTemp;
		}
		catch (...)
		{
			LOG_ERROR( FMT( "MatlabMsbackadj function failure." ) );
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

		// prot process
		//
		if  ( true == bSuccess )
		{
			mwArray	insSize = insBackadj.GetDimensions( );

			int	nGetSize[1];

			insSize.GetData( nGetSize, _countof( nGetSize ) );

			int	nCountOfBackadj= nGetSize[0];
			
			// 入力したデータポイント数と、出力されてきたデータポイント数が異なればエラー
			// check values
			if  ( nRow != nCountOfBackadj )
			{
				// error
				LOG_ERROR( FMT( "unmatch row size (%d:%d)", nRow, nDataSize ) );
				break;
			}
				
			double	*pdBackadj = new double[nCountOfBackadj];
			if  ( (double *)NULL == pdBackadj )
			{
				LOG_ERROR( FMT( "can not allocate data array.(size:%dx%d)", sizeof( double ), nCountOfBackadj ) );
				break;
			}

			// メモリの確保
			memset( (void *)pdBackadj, 0x00, sizeof( double ) * nCountOfBackadj );
		
			// store
			insBackadj.GetData( pdBackadj, nCountOfBackadj );   // pdBackadjにベースラインサブトラクト後のピーク強度が入る。

			baseline.reserve( nRowTemp );

			// ベースラインの値を baseline に格納
			int nRow_index = 0;
			for ( nIndex = 0 ; nIndex < nRowTemp ; nIndex++ )
			{
				if  ( 0 < src.getY( nIndex ) )  // 元の配列において、強度が0でなければ
				{
					baseline.addPoint( src.getX( nIndex ), src.getY( nIndex )- pdBackadj[nRow_index]);
					nRow_index++;
				}
				else // 元の配列において、強度が0以下ならば
				{
					baseline.addPoint( src.getX( nIndex ), 0);
				
				}
			}

			// 強度が0以外の点数を確認します
			if  ( nRow != nRow_index )
			{
				// error
				LOG_ERROR( FMT( "unmatch row size without zero value (%d:%d)", nRow, nRow_index ) );
				delete[]	pdBackadj;		// @Date:2014/02/21	<Add>	A.Ozaki
				break;
			}

            /*   baseline除去後のデータをbaselineに渡していたので、削除 <del> 2014-2-7 Fujita
			baseline.reserve( nRow );

			nRow
			for ( nIndex = 0 ; nIndex < nRow ; nIndex++ )
			{
				if  ( 0 < src.getY( nIndex ){  // 元の配列において、強度が0でなければ
					baseline.addPoint( src.getX( nIndex ), *( pdBackadj + nIndex ) );
				} 
			}
	        */

			delete[]	pdBackadj;
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

// get noise threshold
double	MsbackadjBaselineDetectionManager::getNoiseThreshold( kome::core::XYData& src )
{
	// noise threshold
	return	0.0;
}

// get instance
MsbackadjBaselineDetectionManager	&MsbackadjBaselineDetectionManager::getInstance( void )
{
	// create object
	static MsbackadjBaselineDetectionManager	mgr;

	return	mgr;
}

// release MATLAB application
void	MsbackadjBaselineDetectionManager::releaseMatlab( void )
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
