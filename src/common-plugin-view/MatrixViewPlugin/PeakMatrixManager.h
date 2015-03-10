/**
 * @file PeakMatrixManager.h
 * @brief interfaces of PeakMatrixManager class
 *
 * @author S.Tanaka
 * @date 2007.12.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PEAK_MATRIX_MANAGER_H__
#define __KOME_PEAK_MATRIX_MANAGER_H__


namespace kome {
	namespace matrix {

		
		class kome::operation::ManipulatedSample;
		/**
		 * @class PeakMatrixManager
		 * @brief peak matrix management class
		 */
		class PeakMatrixManager {
		protected:
			/**
			 * @fn PeakMatrixManager()
			 * @brief constructor
			 */
			PeakMatrixManager();

			/**
			 * @fn virtual ~PeakMatrixManager()
			 * @brief destructor
			 */
			virtual ~PeakMatrixManager();

		protected:
			/**
			 * @struct SampleFile
			 * @brief sample file information
			 */
			struct SampleFile {
				std::string path;
				std::string group;
				COLOR color;
			};

		protected:
			/** standard */
			std::string m_standard;

			/** standard group ID */
			int m_standardId;
						
		public:
			/**
			 * @fn void initRow( const int prj )
			 * @brief initializes matrix row
			 * @param prj project ID
			 */
			void initRow( const int prj );

			/**
			 * @fn void initCol( const int prj )
			 * @brief initializes matrix column
			 * @param project ID
			 */
			void initCol( const int prj );

			/**
			 * @fn void initMatrix( const int prj )
			 * @brief initializes matrix
			 * @param prj project ID
			 */
			void initMatrix( const int prj );

		public:
			/**
			 * @fn void addSampleSet(
						const char* path,
						const int prj,
						const COLOR color,
						const char* group
					)
			 * @brief adds sample set
			 * @param path sample set path
			 * @param prj project ID
			 * @param color profile color
			 * @param group group name
			 */
			 void addSampleSet(
				const char* path,
				const int prj,
				const COLOR color,
				const char* group
			);

			 /**
			  * @fn bool readSampleList( const char* path, const int prj, kome::core::Progress* progress )
			  * @brief reads sample list file
			  * @param path sample list file path
			  * @param prj project ID
			  * @param progress the object to show progress
			  * @return If true, it succeeded to read the sample list file.
			  */
			 bool readSampleList( const char* path, const int prj, kome::core::Progress* progress );

			/**
			 * @fn void setStandard( const char* path )
			 * @brief sets standard file path
			 * @param path standard file path
			 */
			void setStandard( const char* path );

			/**
			 * @fn const char* getStandard()
			 * @brief gets standard file path
			 * @return standard file path
			 */
			const char* getStandard();

			/**
			 * @fn int getStandardGroupId()
			 * @brief gets standard group ID
			 * @return standard group ID
			 */
			int getStandardGroupId();

			/**
			 * @fn void setNormalizFilter(
						int matrix,
						kome::plugin::PluginFunctionItem* item,
						kome::objects::SettingParameterValues* settings,
						const char* paramval,
						double& intentMin,
						double& intentMax,
						bool& btype,
						double& rtMin,
						double& rtMax,
						double& mzMin,
						double& mzMax
					)
			 * @brief set normalization filter
			 * @param matrix
			 * @param item
			 * @param settings
			 * @param paramval
			 * @param intentMin
			 * @param intentMax
			 * @param btype
			 * @param rtMin
			 * @param rtMax
			 * @param mzMin
			 * @param mzMax
			 */
			void setNormalizFilter(
				int matrix,
				kome::plugin::PluginFunctionItem* item,
				kome::objects::SettingParameterValues* settings,
				const char* paramval,
				double& intentMin,
				double& intentMax,
				bool& btype,
				double& rtMin,
				double& rtMax,
				double& mzMin,
				double& mzMax
			);
				
		public:
			/**
			 * @fn static bool fillMatrix(
						kome::plugin::PluginFunctionItem* peakSearch,
						kome::objects::SettingParameterValues* settings,
						const double rtTol,
						const double mzTol,
						const bool initFlg,
						std::vector< int >& groups,
						const int stdGroup,
						kome::core::Progress* progress
					)
			 * @brief fills matrix
			 * @param peakSearch peak search method
			 * @param settings peak search setting parameters
			 * @param rtTol RT tolerance
			 * @param mzTol m/z tolerance
			 * @param initFlg If true, peak values data is initialized before filling.
			 * @param groups the array of target group ID
			 * @param stdGroup standard group ID
			 * @param progress the object to show progress
			 */
			static bool fillMatrix(
				kome::plugin::PluginFunctionItem* peakSearch,
				kome::objects::SettingParameterValues* settings,
				const double rtTol,
				const double mzTol,
				const bool initFlg,
				std::vector< int >& groups,
				const int stdGroup,
				kome::core::Progress* progress,
				const char* supplement_method, // @date 2013/08/28 <Add> FUJITA
				kome::ident::PeakPositions* peakPos,	// @date 2014.01.10 <Add> M.Izum
				bool bPeakValStat,	// @date 2014.01.10 <Add> M.Izum
				std::map< int, int >& peakIdMap
			 );

			/**
			 * @fn static bool analyzePeaks(
						kome::plugin::PluginFunctionItem** analysisArr,
						kome::objects::SettingParameterValues** settingsArr,
						const unsigned int num,
						const int stdGroup,
						kome::core::Progress* progress
					)
			 * @brief analyze peaks
			 * @param[in] analysisArr the array of peak analysis
			 * @param[in] settingsArr the array of peak analysis settings
			 * @param[in] num the array size
			 * @param stdGroup standard group ID
			 * @param progress the object to show progress
			 * @return If true, it succeeded analyze peaks
			 */
			static bool analyzePeaks(
				kome::plugin::PluginFunctionItem** analysisArr,
				kome::objects::SettingParameterValues** settingsArr,
				const unsigned int num,
				const int stdGroup,
				kome::core::Progress* progress
			 );
			
			/**
			 * @fn static bool scalingPeaks(				
						kome::plugin::PluginFunctionItem** scalingArr,
						kome::objects::SettingParameterValues** settingsArr,
						const unsigned int num,
						const int stdGroup,
						kome::core::Progress* progress
					)
			 * @brief scaling peaks
			 * @param[in] scalingArr the array of peak scaling
			 * @param[in] settingsArr the array of peak scaling settings
			 * @param[in] num the array size
			 * @parma stdGroup standard group ID
			 * @param progress the object to show progress
			 * @return If true, it succeeded scaling peaks
			 */
			static bool scalingPeaks(				
				kome::plugin::PluginFunctionItem** scalingArr,
				kome::objects::SettingParameterValues** settingsArr,
				const unsigned int num,
				const int stdGroup,
				kome::core::Progress* progress
			);
			
			/**
			 * @fn static bool multivariate_analyisiPeaks(				
						kome::plugin::PluginFunctionItem** multivariate_analysisArr,
						kome::objects::SettingParameterValues** settingsArr,
						const unsigned int num,
						const int stdGroup,
						kome::core::Progress* progress
					)
			 * @brief multivariate analyisis peaks
			 * @param[in] scalingArr the array of peak scaling
			 * @param[in] settingsArr the array of multivariate analysis settings
			 * @param[in] num the array size
			 * @parma stdGroup standard group ID
			 * @param progress the object to show progress
			 * @return If true, it succeeded scaling peaks
			 */
			static bool multivariate_analysisPeaks(				
				kome::plugin::PluginFunctionItem** multivariate_analysisArr,
				kome::objects::SettingParameterValues** settingsArr,
				const unsigned int num,
				const int stdGroup,
				kome::core::Progress* progress
			);

			/**
			 * @fn static bool normalizations(
						kome::plugin::PluginFunctionItem* peakDetect,
						kome::objects::SettingParameterValues* peakDetectSettings,
						kome::plugin::PluginFunctionItem* ThrFunc,
						kome::objects::SettingParameterValues* ThrSettings,
						kome::plugin::PluginFunctionItem* RtRangeFunc,
						kome::objects::SettingParameterValues* RtRangeSettings,
						kome::plugin::PluginFunctionItem* MzRangeFunc,
						kome::objects::SettingParameterValues* MzRangeSettings,
						kome::plugin::PluginFunctionItem** normalizArr,
						kome::objects::SettingParameterValues* settingsArr,
						std::vector< int >& groups,
						const unsigned int num,
						const int stdGroup,
						kome::core::Progress* progress
					 )
			 * @brief normaliztion
			 */
			static bool normalizations(
				kome::plugin::PluginFunctionItem* peakDetect,
				kome::objects::SettingParameterValues* peakDetectSettings,
				kome::plugin::PluginFunctionItem* ThrFunc,
				kome::objects::SettingParameterValues* ThrSettings,
				kome::plugin::PluginFunctionItem* RtRangeFunc,
				kome::objects::SettingParameterValues* RtRangeSettings,
				kome::plugin::PluginFunctionItem* MzRangeFunc,
				kome::objects::SettingParameterValues* MzRangeSettings,
				kome::plugin::PluginFunctionItem** normalizArr,
				kome::objects::SettingParameterValues* settingsArr,
				std::vector< int >& groups,
				const unsigned int num,
				const int stdGroup,
				kome::core::Progress* progress
			 );

		public:
			/**
			 * @fn bool outputMatrix( const int prj, const char* path )
			 * @brief outputs matrix data to file
			 * @param prj project ID
			 * @param path output file path
			 * @return If true, it succeeded to output matrix data.
			 */
			bool outputMatrix( const int prj, const char* path );
					
		public:
			/**
			 * @fn static PeakMatrixManager& getInstance()
			 * @brief gets PeakMatrixManager object
			 * @return PeakMatrixManager object. (This is the only object.)
			 */
			static PeakMatrixManager& getInstance();
		};
	}
}

#endif	// __KOME_PEAK_MATRIX_MANAGER_H__
