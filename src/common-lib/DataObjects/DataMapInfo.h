/**
 * @file DataMapInfo.h
 * @brief interfaces of GraphInfo class
 *
 * @author S.Tanaka
 * @date 2007.09.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IMG_DATA_MAP_INFO_H__
#define __KOME_IMG_DATA_MAP_INFO_H__


#include <vector>


namespace kome {
	namespace objects {

		class Sample;
		class DataGroupNode;
		class Spectrum;
		class DataSet;

		/**
		 * @class DataMapInfo
		 * @brief deta map information
		 */
		class DATA_OBJECTS_CLASS DataMapInfo {
		public:
			/**
			 * @fn DataMapInfo( DataGroupNode& group, const unsigned int row, const unsigned int col )
			 * @brief constructor
			 * @param[in] group spectrum group
			 * @param[in] row row size
			 * @param[in] col column size
			 */
			DataMapInfo( DataGroupNode& group, const unsigned int row, const unsigned int col );

			/**
			 * @fn virtual ~DataMapInfo()
			 * @brief destructor
			 */
			virtual ~DataMapInfo();

		protected:
			/** row size */
			unsigned int m_row;
			/** column size */
			unsigned int m_col;
			/** raw data */
			DataGroupNode* m_group;

			/** data map */
			double* m_map;

			/** min m/z */
			double m_minMz;
			/** max m/z */
			double m_maxMz;
			/** min RT */
			double m_minRt;
			/** max RT */
			double m_maxRt;
			/** min intensity */
			double m_minIntensity;
			/** max intensity */
			double m_maxIntensity;

			/** completed flag */
			bool m_completed;			

			/**
			 * @struct TmpData
			 * @brief temporary data
			 */
			struct TmpData {
				double* intensities;
				double minMz;
				double maxMz;
				double minRt;
				double maxRt;
				double minIntensity;
				double maxIntensity;
			};

			/** temporary data */
			TmpData* m_tmp;

			/** auto commit flag */
			bool m_autoCommit;

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
			 * @fn DataGroupNode& getGroup()
			 * @brief gets raw data
			 * @return raw data
			 */
			DataGroupNode& getGroup();

			/**
			 * @fn Sample* getSample()
			 * @brief gets sample
			 * @return sample
			 */
			Sample* getSample();

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
			 * @fn double getIntensity( const unsigned int row, const unsigned int col )
			 * @brief gets the intensity of the image map
			 * @param[in] row row (RT) index
			 * @param[in] col column (m/z) index
			 * @return the intensity of the image map
			 */			
			double getIntensity( const unsigned int row, const unsigned int col );

		public:
			/**
			 * @fn void setCompleted( const bool completed )
			 * @brief sets completed flag
			 * @param[in] completed completed flag value
			 */
			void setCompleted( const bool completed );

			/**
			 * @fn bool isCompleted()
			 * @brief gets completed flag
			 * @return completed flag value
			 */
			bool isCompleted();

			/**
			 * @fn void setIntensityRange( const double minInt, const double maxInt )
			 * @brief sets intensity range
			 * @param[in] minInt min intensity
			 * @param[in] maxInt max intensity
			 */
			void setIntensityRange( const double minInt, const double maxInt );

			/**
			 * @fn bool setRange(
					double minMz,
					double maxMz,
					double minRt,
					double maxRt,
					kome::core::Progress& progress
				)
			 * @brief sets data map range
			 * @param[in] minMz min m/z (If negative value, it uses default value instead.)
			 * @param[in] maxMz max m/z (If negative value, it uses default value instead.)
			 * @param[in] minRt min m/z (If negative value, it uses default value instead.)
			 * @param[in] maxRt max m/z (If negative value, it uses default value instead.)
			 * @param[in] filterName filter name
			 * @param[out] progress the object to show progress
			 * @return If true, map is created completly.
			 */
			bool setRange(
				double minMz,
				double maxMz,
				double minRt,
				double maxRt,
				const char* filterName,
				kome::core::Progress& progress
			);

		public:
			/**
			 * @fn void setAutoCommit( const bool autoCommit )
			 * @brief sets auto commit flag
			 * @param[in] autoCommit auto commit flag
			 */
			void setAutoCommit( const bool autoCommit );

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

		public:
			/**
			 * @fn bool saveDataMap( const char* path )
			 * @brief saves data map
			 * @param[in] path data map file path to save
			 * @return If true, it succeeded to save data map
			 */
			bool saveDataMap( const char* path );

			/**
			 * @fn bool loadDataMap( const char* path )
			 * @brief loads data map
			 * @param[in] path data map file path to load
			 * @return If true, it succeeded to load data map
			 */
			bool loadDataMap( const char* path );

		protected:
			/**
			 * @fn void createImageMap(
						std::vector< kome::objects::Spectrum* >& spectra,
						kome::core::Progress& progress
					)
			 * @brief creates image map
			 * @param[in] spectra spectrum array
			 * @param[out] progress the object to show progress
			 */
			void createImageMap(
				std::vector< kome::objects::Spectrum* >& spectra,
				kome::core::Progress& progress
			);

			/**
			 * @fn void getSpecIntensities(
					Spectrum* spec,
					double* intensities,
					int size
				);
			 * @brief gets intensities of spectrum
			 * @param[in] spec spectrum
			 * @param[out] intensities the array to store intensities
			 * @param[in] size array size
			 */
			void getSpecIntensities(
				Spectrum* spec,
				double* intensities,
				int size
			);

			/**
			 * @fn void getSpecArray( const char* filterName, std::vector< kome::objects::Spectrum* >& tmpSpecArray )
			 * @brief get spectrum array
			 * @param[in] filterName filter name
			 * @param[in] tmpSpecArray spectrum array
			 */
			void getSpecArray( const char* filterName, std::vector< kome::objects::Spectrum* >& tmpSpecArray );
		protected:
			/**
			 * @fn void createTmpData()
			 * @brief creates temporary data
			 */
			void createTmpData();

			/**
			 * @fn void deleteTmpData()
			 * @brief deletes temporary data
			 */
			void deleteTmpData();

		protected:
			/**
			 * @fn static bool lessSpec( Spectrum* spec0, Spectrum* spec1 )
			 * @brief compare RT of spectrum to sort
			 * @param[in] spec0 spectrum to be compared
			 * @param[in] spec1 spectrum to compare
			 * @return If spec0 sorts before spec1, this method returns true.
			 */
			static bool lessSpec( Spectrum* spec0, Spectrum* spec1 );
		};
	}
}

#endif		// __KOME_IMG_DATA_MAP_INFO_H__
