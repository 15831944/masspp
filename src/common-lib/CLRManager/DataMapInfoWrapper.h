/**
 * @file DataMapInfoWrapper.h
 * @brief interfaces of DataMapInfoWrapper class
 *
 * @author S.Tanaka
 * @date 2007.09.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_DATA_MAP_INFO_WRAPPER_H__
#define __KOME_CLR_DATA_MAP_INFO_WRAPPER_H__


namespace kome { 
	namespace clr {

		ref class DataGroupNodeWrapper;
		ref class DataSetWrapper;
		ref class SpectrumWrapper;
		ref class ProgressWrapper;

		/**
		 * @class DataMapInfoWrapper
		 * @brief data manager wrapper class to use on CLR environment
		 */
		public ref class DataMapInfoWrapper {
		public:
			/**
			 * @fn DataMapInfoWrapper( kome::objects::DataMapInfo& dataMap )
			 * @brief constructor
			 * @param[in] dataMap data manager object
			 */
			DataMapInfoWrapper( kome::objects::DataMapInfo& dataMap );

			/**
			 * @fn virtual ~DataMapInfoWrapper()
			 * @brief destructor
			 */
			virtual ~DataMapInfoWrapper();

		protected:
			/** data manager object */
			kome::objects::DataMapInfo& m_dataMap;

		public:
			/**
			 * @fn kome::objects::DataMapInfo& getDataMapInfo()
			 * @brief gets data manager object
			 * @return data manager object
			 */
			kome::objects::DataMapInfo& getDataMapInfo();

		public:
			/**
			 * @fn unsigned int getRowSize()
			 * @brief gets row size
			 * @return row size
			 */
			unsigned int getRowSize();

			/**
			 * @fn unsigned int getColSize()
			 * @brief gets column size
			 * @return column size
			 */
			unsigned int getColSize();

			/**
			 * @fn DataGroupNodeWrapper^ getGroup()
			 * @brief gets spectrum group
			 * @return spectrum group
			 */
			DataGroupNodeWrapper^ getGroup();

			/**
			 * @fn double getMinMz()
			 * @brief gets min m/z
			 * @return min m/z
			 */
			double getMinMz();

			/**
			 * @fn double getMaxMz()
			 * @brief gets max m/z
			 * @return max m/z
			 */
			double getMaxMz();

			/**
			 * @fn double getMinRt()
			 * @brief gets min RT
			 * @return min RT
			 */
			double getMinRt();

			/**
			 * @fn double getMaxRt()
			 * @brief gets max RT
			 * @return max RT
			 */
			double getMaxRt();

			/**
			 * @fn double getMinIntensity()
			 * @brief gets min intensity
			 * @return min intensity
			 */
			double getMinIntensity();

			/**
			 * @fn double getMaxIntensity()
			 * @brief gets max intensity
			 * @return max intensity
			 */
			double getMaxIntensity();

			/**
			 * @fn double getIntensity( unsigned int row, unsigned int col )
			 * @brief gets the intensity of the image map
			 * @param[in] row row (RT) index
			 * @param[in] col column (m/z) index
			 * @return the intensity of the image map
			 */			
			double getIntensity( unsigned int row, unsigned int col );

		public:
			/**
			 * @fn void setCompleted( bool completed )
			 * @brief sets completed flag
			 * @param[in] completed completed flag value
			 */
			void setCompleted( bool completed );

			/**
			 * @fn bool isCompleted()
			 * @brief gets completed flag
			 * @return completed flag value
			 */
			bool isCompleted();

			/**
			 * @fn void setIntensityRange( double minInt, double maxInt )
			 * @brief sets intensity range
			 * @param[in] minInt min intensity
			 * @param[in] maxInt max intensity
			 */
			void setIntensityRange( double minInt, double maxInt );

			/**
			 * @fn bool setRange(
					double minMz,
					double maxMz,
					double minRt,
					double maxRt,
					ProgressWrapper^ progress
				)
			 * @brief sets data map range
			 * @param[in] minMz min m/z (If negative value, use default value.)
			 * @param[in] maxMz max m/z (If negative value, use default value.)
			 * @param[in] minRt min m/z (If negative value, use default value.)
			 * @param[in] maxRt max m/z (If negative value, use default value.)
			 * @param[in] filterName data map filter name
			 * @param[out] progress the object to show progress
			 * @return If true, map is created completly.
			 */
			bool setRange(
				double minMz,
				double maxMz,
				double minRt,
				double maxRt,
				const char* filterName,
				ProgressWrapper^ progress
			);

		public:
			/**
			 * @fn void setAutoCommit( bool autoCommit )
			 * @brief sets auto commit flag
			 * @param[in] autoCommit auto commit flag
			 */
			void setAutoCommit( bool autoCommit );

			/**
			 * @fn bool isAutoCommit()
			 * @brief gets auto commit flag
			 * @return auto commit flag
			 */
			bool isAutoCommit();

			/**
			 * @fn void commit()
			 * @brief commit data
			 */
			void commit();

			/**
			 * @fn void rollback()
			 * @brief rollback data
			 */
			void rollback();
		};
	}
}

#endif		// __KOME_CLR_DATA_MAP_INFO_WRAPPER_H__
