/**
 * @file MatrixViewFunctions.h
 * @brief interfaces of MatrixView plug-in functions
 *
 * @author S.Tanaka
 * @date 2009.10.29
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_MATRIX_VIEW_FUNCTIONS_H__
#define __KOME_MATRIX_VIEW_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant togglePeakMatrixOpened( kome::objects::Parameters* )
 * @brief toggles visibility of the peaks matrix dialog
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant togglePeakMatrixOpened( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant isPeakMatrixOpened( kome::objects::Parameters* )
 * @brief judges whether the peaks matrix dialog is opened
 * @return If true, the peaks matrix dialog is opened.
 */
kome::objects::Variant isPeakMatrixOpened( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant updateMatrixDialog( kome::objects::Parameters* )
 * @brief updates matrix dialog
 * @return This value has no meanings.
 */
kome::objects::Variant updateMatrixDialog( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant onExitGUI( kome::objects::Parameters* )
 * @brief exits GUI
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant onExitGUI( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant openMatrixDialog( kome::objects::Parameters* )
 * @brief opens the matrix view
 * @return This value has no meanings
 */
kome::objects::Variant openMatrixDialog( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant closeMatrixDialog( kome::objects::Parameters* )
 * @brief closes the matrix view
 * @return This value has no meanings
 */
kome::objects::Variant closeMatrixDialog( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant canExitQuantitationDialog( kome::objects::Parameters* params )
 * @brief check whether can exit the Quantitation dialog
 * @param params parameters object (This object has no information.)
 * @return This function returns true or false. (boolean)
 */
kome::objects::Variant canExitQuantitationDialog( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant selectPeakMatrix( kome::objects::Parameters* params )
 * @brief selects peak matrix
 * @param params parameters object
 * @return If true, selecting matrix is succeeded. (boolean)
 */
kome::objects::Variant selectPeakMatrix( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant initializeMatrix( kome::objects::Parameters* )
 * @brief initializes peak matrix
 * @return If true, it succeeded to initialize the matrix
 */
kome::objects::Variant initializeMatrix( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant initializeMatrixRow( kome::objects::Parameters* )
 * @brief initializes peak matrix row
 * @return If true, it succeeded to initialize the matrix row.
 */
kome::objects::Variant initializeMatrixRow( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant initializeMatrixCol( kome::objects::Parameters* )
 * @brief initializes peak matrix col
 * @return If true, it succeeded to initializxe the matrix column.
 */
kome::objects::Variant initializeMatrixCol( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant addMatrixSampleWithGroup( kome::objects::Parameters* params )
 * @brief adds matrix sample with group
 * @param params parameters object
 * @return If true, it succeeded to add matrix sample. (boolean)
 */
kome::objects::Variant addMatrixSampleWithGroup( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant readSampleListFile( kome::objects::Parameters* params )
 * @brief reads sample list file
 * @param params parameters object
 * @return If true, it succeeded to read the matrix sample list file. (boolean)
 */
kome::objects::Variant readSampleListFile( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setStandardFilePath( kome::objects::Parameters* params )
 * @brief sets standard file path
 * @param params parameters object
 * @return If true, it succeeded to set standard file path
 */
kome::objects::Variant setStandardFilePath( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant createMatrixRow( kome::objects::Parameters* params )
 * @brief creates matrix row
 * @param params parameters object
 * @return If true, it succeeded to create matrix row. (boolean)
 */
kome::objects::Variant createMatrixRow( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant calculatePeakValues( kome::objects::Parameters* params )
 * @brief calculate peak values
 * @param params parameters object
 * @return If true, it succeeded to fill the peak matrix. (boolean)
 */
kome::objects::Variant calculatePeakValues( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant outputPeakMatrix( kome::objects::Parameters* params )
 * @brief outputs peak matrix
 * @param params parameters object
 * @return If true, it succeeded to output peak matrix.
 */
kome::objects::Variant outputPeakMatrix( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant openCreatePeakMatrixDialog( kome::objects::Parameters* )
 * @brief open create peak matrix dialog
 * @param params parameters object
 */
kome::objects::Variant openCreatePeakMatrixDialog( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant onExitPeakMatrixDialog( kome::objects::Parameters* )
 * @brief exit peak matrix dialog
 * @param params parameters object
 */
kome::objects::Variant onExitPeakMatrixDialog( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant refreshWindow( kome::objects::Parameters* )
 * @brief refreshes window
 * @rerun This value has no meanings.
 */
kome::objects::Variant refreshWindow( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant activateMatrixPeak( kome::objects::Parameters* params )
 * @brief activates the matrix peak
 * @param[in] params parameters which includes the matrix peak ID.
 * @return This value has no meanings.
*/
kome::objects::Variant activateMatrixPeak( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant addMatrixGroup( kome::objects::Parameters* params )
 * @brief add matrix group
 * @param[in] params parameters object
 * @return If true, adding matrix group is succeeded. (boolean)
*/
kome::objects::Variant addMatrixGroup( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant addMatrixGroupString( const char* szGroup, const char* szColor )
 * @brief add matrix group
 * @param[in] strGroup group name
 * @param[in] strColor color string
 * @return If true, adding matrix group is succeeded. (boolean)
*/
kome::objects::Variant addMatrixGroupString( std::string strGroup, std::string strColor );


/**
 * @fn kome::objects::Variant addMatrixViewSample( kome::objects::Parameters* params )
 * @brief add matrix sample
 * @param[in] params parameters object
 * @return If true, adding matrix sample is succeeded. (boolean)
*/
kome::objects::Variant addMatrixViewSample( kome::objects::Parameters* params );


/**
 * @fn kome::objects::Variant addMatrixViewSampleString( std::string strGroup, std::string strPath, std::string strColor );
 * @brief add matrix sample
 * @param[in] params parameters object
 * @return If true, adding matrix sample is succeeded. (boolean)
*/
kome::objects::Variant addMatrixViewSampleString( std::string strGroup, std::string strPath, std::string strColor );


/**
 * @fn kome::objects::Variant addMatrixViewSamples( kome::objects::Parameters* params )
 * @brief add matrix sample
 * @param[in] params parameters object
 * @return If true, This function is succeeded. (boolean)
*/
kome::objects::Variant addMatrixViewSamples( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setMatrixNormFilterRt( kome::objects::Parameters* params )
 * @brief add matrix sample
 * @param[in] params parameters object
 * @return If true, This function is succeeded. (boolean)
*/
kome::objects::Variant setMatrixNormFilterRt( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setMatrixNormFilterMz( kome::objects::Parameters* params )
 * @brief add matrix sample
 * @param[in] params parameters object
 * @return If true, This function is succeeded. (boolean)
*/
kome::objects::Variant setMatrixNormFilterMz( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant execMatrixNormSpec( kome::objects::Parameters* params )
 * @brief add matrix sample
 * @param[in] params parameters object
 * @return If true, This function is succeeded. (boolean)
*/
kome::objects::Variant execMatrixNormSpec( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant execMatrixNorm2D( kome::objects::Parameters* params )
 * @brief add matrix sample
 * @param[in] params parameters object
 * @return If true, This function is succeeded. (boolean)
*/
kome::objects::Variant execMatrixNorm2D( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant execMatrixNormSpec2D( kome::objects::Parameters* params )
 * @brief add matrix sample
 * @param[in] params parameters object
 * @param[in] detect "peak_spec" or "peak_2d"
 * @return If true, This function is succeeded. (boolean)
*/
kome::objects::Variant execMatrixNormSpec2D( kome::objects::Parameters* params, const char* detect );

/**
 * @fn kome::objects::Variant execMatrixRtAlignment( kome::objects::Parameters* params )
 * @brief add matrix sample
 * @param[in] params parameters object
 * @return If true, This function is succeeded. (boolean)
*/
kome::objects::Variant execMatrixRtAlignment( kome::objects::Parameters* params );


/**
 * @fn void paringKakko( kome::objects::SettingParameterValues* settings )
 * @brief paring kakko
 * @param[in/out] settings settings
*/
void paringKakko( kome::objects::SettingParameterValues* settings );

/**
 * @fn kome::objects::Variant setMatrixPeakPositions( kome::objects::Parameters* params )
 * @brief add matrix sample
 * @param[in] params parameters object
 * @return If true, This function is succeeded. (boolean)
*/
kome::objects::Variant setMatrixPeakPositions( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setMatrixPeakValues( kome::objects::Parameters* params )
 * @brief add matrix sample
 * @param[in] params parameters object
 * @return If true, This function is succeeded. (boolean)
*/
kome::objects::Variant setMatrixPeakValues( kome::objects::Parameters* params );


/**
 * @fn kome::objects::Variant setMatrixPeakScaling( kome::objects::Parameters* params )
 * @brief add matrix sample
 * @param[in] params parameters object
 * @return If true, This function is succeeded. (boolean)
*/
kome::objects::Variant setMatrixPeakScaling( kome::objects::Parameters* params );


/**
 * @fn kome::objects::Variant setMatrixPeakAnalysis( kome::objects::Parameters* params )
 * @brief add matrix sample
 * @param[in] params parameters object
 * @return If true, This function is succeeded. (boolean)
*/
kome::objects::Variant setMatrixPeakAnalysis( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant delMatrixDupPeaks( kome::objects::Parameters* params )
 * @brief add matrix sample
 * @param[in] params parameters object
 * @return If true, This function is succeeded. (boolean)
*/
kome::objects::Variant delMatrixDupPeaks( kome::objects::Parameters* params );

/**
 * @fn void getSubstance(
 		std::vector< int > searches,
 		int searchId,
 		std::vector< int > peaks, 
 		std::vector< int >& subtrance
 	)
 * @brief get substance
 * @param[in] searches
 * @param[in] searchId
 * @param[in] peaks
 * @param[out] subtrance
*/
void getSubstance(
	std::vector< int > searches,
	int searchId,
	std::vector< int > peaks, 
	std::vector< int >& subtrance
);

/**
 * @fn void DeleteMatrixPeak( int matrixId, std::vector< int > resPeaks, std::vector<int> substance  )
 * @brief delete matrix peak
 * @param matrixId
 * @param resPeaks
 * @param substance
 */
void DeleteMatrixPeak( int matrixId, std::vector< int > resPeaks, std::vector<int> substance );

/** peak matrix infomation */
struct Info{
	int peakId;
	bool substance;
	double sum;
};

void getTotalValueOfPeak( int matrixId, int peak , double& sum );

/**
 * @fn kome::objects::Variant savePeakMatrixDataFile( kome::objects::Parameters* params )
 * @brief save pealk matrix data 
 * @param params parameters object
 * @return If true, it succeeded to save the peak matrix. (boolean)
 */
kome::objects::Variant savePeakMatrixDataFile( kome::objects::Parameters* params );

/**
 * @fn bool getPeakValStatus( const char* name )
 * @brief get the peak value status
 * @param peak
 */
bool getPeakValStatus( const char* name );


#ifdef __unix
	}
#endif  // __unix

#endif// __KOME_MATRIX_VIEW_FUNCTIONS_H__
