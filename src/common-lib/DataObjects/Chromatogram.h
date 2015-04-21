/**
 * @file Chromatogram.h
 * @brief interfaces of Chromatogram class
 *
 * @author S.Tanaka
 * @date 2006.10.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_CHROMATOGRAM_H__
#define __KOME_OBJECTS_CHROMATOGRAM_H__


#include "DataObjectsCommon.h"

#include <vector>


namespace kome {
	namespace objects {

		class Sample;
		class DataGroupNode;
		class Spectrum;
		class DataSet;

		/**
		 * @class Chromatogram 
		 * @brief chromatogram information management class
		 */
		class DATA_OBJECTS_CLASS Chromatogram {
		public:
			/**
			 * @fn Chromatogram( Sample* sample )
			 * @brief constructor
			 * @param[in] sample
			 */
			Chromatogram( Sample* sample );

			/**
			 * @fn virtual ~Chromatogram()
			 * @brief destructor
			 */
			virtual ~Chromatogram();

		protected:
			/** sample */
			Sample* m_sample;

			/** group */
			DataGroupNode* m_group;

			/** name */
			std::string m_name;

			/** m/z */
			double m_mz;

			/** icon name */
			std::string m_icon;

			/** title */
			std::string m_title;

			/** properties */
			kome::core::Properties m_props;

			/** user properties */
			kome::core::Properties m_userProps;

			/** auto created flag */
			bool m_autoCreated;

			/** visible */
			bool m_visible;

			/** original chromatogram */
			Chromatogram* m_orgChrom;
			
			/** operation flag */
			bool m_op;

			/** chromatogram id */
			int m_chromId;

			/** Q1 Mass */
			double m_q1;

			/** Q3 Mass */
			double m_q3;

		protected:
			/** common properties */
			static std::vector< std::string > m_commonProps;
				
		public:
			/**
			 * @fn void setOperationFlag( const bool op )
			 * @brief sets the operation flag value
			 * @param[in] op operation flag value
			 */
			void setOperationFlag( const bool op ){ m_op = op; }

			/**
			 * @fn bool getOperationFlag()
			 * @brief gets the operation flag value
			 * @return operation flag value
			 */
			bool getOperationFlag(){ return m_op; }
		public:
			/**
			 * @fn void setId( int id )
			 * @brief sets chromatogram id
			 * @param[in] id chromatogram id
			 */
			void setId( int id );
			
			/**
			 * @fn int getId()
			 * @brief gets chromatogram id
			 * @return chromatogram id
			 */
			int getId();
		public:
			/**
			 * @fn Sample* getSample()
			 * @brief gets sample
			 * @return sample
			 */
			Sample* getSample();

			/**
			 * @fn void setName( const char* name )
			 * @brief sets chromatogram name
			 * @param[in] name chromatogram name
			 */
			void setName( const char* name );

			/**
			 * @fn const char* getName()
			 * @brief gets chromatogram name
			 * @return chromatogram name
			 */
			const char* getName();

			/**
			 * @fn void setMz( const double mz )
			 * @brief sets m/z
			 * @param[in] mz m/z
			 */
			void setMz( const double mz );

			/**
			 * @fn double getMz()
			 * @brief gets m/z
			 * @return m/z
			 */
			double getMz();

			/**
			 * @fn void setGroup( DataGroupNode* group )
			 * @brief sets spectrum group
			 * @return spectrum group
			 */
			void setGroup( DataGroupNode* group );

			/**
			 * @fn DataGroupNode* getGroup()
			 * @brief gets spectrum group
			 * @return spectrum group
			 */
			DataGroupNode* getGroup();

			/**
			 * @fn void setIcon( const char* icon )
			 * @brief sets icon name
			 * @return icon name
			 */
			void setIcon( const char* icon );

			/**
			 * @fn const char* getIcon()
			 * @brief gets icon name
			 * @return icon name
			 */
			const char* getIcon();

			/**
			 * @fn void setTitle( const char* title )
			 * @brief sets title
			 * @param[in] title title
			 */
			void setTitle( const char* title );

			/**
			 * @fn const char* getTitle()
			 * @brief gets title
			 * @return title
			 */
			const char* getTitle();

			/**
			 * @fn kome::core::Properties& getProperties()
			 * @brief gets properties
			 * @return properties object
			 */
			kome::core::Properties& getProperties();

			/**
			 * @fn kome::core::Properties& getUserProperties()
			 * @brief gets user properties
			 * @return properties object
			 */
			kome::core::Properties& getUserProperties();

			/**
			 * @fn void setAutoCreated( const bool autoCreated = true )
			 * @brief sets auto created flag
			 * @param[in] autoCreated auto created flag value
			 */
			void setAutoCreated( const bool autoCreated = true );

			/**
			 * @fn bool isAutoCreated()
			 * @brief gets auto created flag value
			 * @return auto created flag value
			 */
			bool isAutoCreated();

			/**
			 * @fn void setVisible( const bool visible )
			 * @brief sets the visible flag
			 * @param[in] visible visible flag value
			 */
			void setVisible( const bool visible );

			/**
			 * @fn bool isVisible()
			 * @brief gets the visible flag value
			 * @return visible flag value
			 */
			 bool isVisible();

			/**
			 * @fn void setOrgChromatogram( Chromatogram* chrom )
			 * @brief sets original chromatogram
			 * @param[in] chrom original chromatogram
			 */
			void setOrgChromatogram( Chromatogram* chrom );

			/**
			 * @fn Chromatogram* getOrgChromatogram()
			 * @brief gets original chromatogram
			 * @return original chromatogram
			 */
			Chromatogram* getOrgChromatogram();

		public:
			/**
			 * @fn void setQ1( const double q1 )
			 * @brief sets the Q1 Mass
			 * @param[in] q1 Q1 Mass
			 */
			void setQ1( const double q1 );

			/**
			 * @fn double getQ1()
			 * @brief gets the Q1 Mass
			 * @return Q1 Mass
			 */
			double getQ1();

			/**
			 * @fn void setQ3( const double q3 )
			 * @brief sets the Q3 Mass
			 * @param[in] q3 Q3 Mass
			 */
			void setQ3( const double q3 );

			/**
			 * @fn double getQ3()
			 * @brief gets the Q3 Mass
			 * @return Q3 Mass
			 */
			double getQ3();

		public:			
			/**
			 * @fn kome::core::XYData* getXYData()
			 * @brief gets xy data from data manager
			 * @return xy data object
			 */
			kome::core::XYData* getXYData();

			/**
			 * @fn void deleteXYData()
			 * @brief deletes xy data of data manager
			 */
			void deleteXYData();

			/**
			 * @fn void getXYData( kome::core::XYData* const xyData, const bool op )
			 * @brief gets chromatogram data points
			 * @param[out] xyData object to store data points
			 * @param[in] op If true, getting operated data points
			 */
			void getXYData( kome::core::XYData* const xyData, const bool op );

		public:
			/**
			 * @fn Spectrum* searchSpectrum( const double rt, const SearchType search = SEARCH_NEAR )
			 * @brief searches spectrum
			 * @param[in] rt RT
			 * @param[in] search search type
			 * @return searched spectrum (If specified spectrum is not found, this method returns NULL.)
			 */
			Spectrum* searchSpectrum( const double rt, const SearchType search = SEARCH_NEAR );

			/**
			 * @fn void getSpectra(
						DataSet& dataSet,
						const double startRt,
						const double endRt,
						const SearchType startSearch = SEARCH_NEAR,
						const SearchType endSearch = SEARCH_NEAR
				)
			 * @brief get spectra in specified RT range
			 * @param[out] dataSet data set object to store spectra
			 * @param[in] startRt start RT
			 * @param[in] endRt end RT
			 * @param[in] startSearch start search type
			 * @param[in] endSearch end search type
			 */
			void getSpectra(
					DataSet& dataSet,
					const double startRt,
					const double endRt,
					const SearchType startSearch = SEARCH_NEAR,
					const SearchType endSearch = SEARCH_NEAR
			);

			/**
			 * @fn double getMass( const unsigned int index )
			 * @brief gets mass
			 * @param[in] index data index
			 * @return mass
			 */
			double getMass( const unsigned int index );

			/**
			 * @fn int getMsStage( const unsigned int index )
			 * @brief gets MS stage
			 * @param[in] index data index
			 * @return MS stage.
			 */
			int getMsStage( const unsigned int index );

			/**
			 * @fn double getPrecursor( const unsigned int index )
			 * @brief gets precursor mass
			 * @param[in] index data index
			 * @return precursor mass
			 */
			double getPrecursor( const unsigned int index );

		public:
			/**
			 * @fn void getProperties( kome::core::Properties& properties )
			 * @brief gets spectrum and spectrum group properties
			 * @param[out] properties properties object to store properties
			 */
			void getProperties( kome::core::Properties& properties );

			/**
			 * @fn void getUserProperties( kome::core::Properties& userProperties )
			 * @brief gets chromatogram and chromatogram group user properties
			 * @param[out] userProperties properties object to store user properties
			 */
			void getUserProperties( kome::core::Properties& userProperties );

		public:
			/**
			 * @fn static bool isCommonProperty( const char* key )
			 * @brief check whther the specified property key is common property or not
			 * @param[in] key parameter key
			 * @return If true, specified key is common property key
			 */
			static bool isCommonProperty( const char* key );

		protected:
			/**
			 * @fn virtual void onGetXYData( kome::core::XYData* const xyData ) = 0
			 * @brief This method is called by getXYData method (abstract method)
			 * @param[out] xyData object to store data points
			 */
			virtual void onGetXYData( kome::core::XYData* const xyData ) = 0;

			/**
			 * @fn virtual void onGetSpectra(
					DataSet& dataSet,
					const double startRt,
					const double endRt,
					const SearchType startSearch,
					const SearchType endSearch
				) = 0
			 * @brief This method is called by searchSpectrum or getDataSet method (abstract method)
			 * @param[out] dataSet data set object to store spectra
			 * @param[in] startRt start RT
			 * @param[in] endRt end RT
			 * @param[in] startSearch start search type
			 * @param[in] endSearch end search type
			 */
			virtual void onGetSpectra(
				DataSet& dataSet,
				const double startRt,
				const double endRt,
				const SearchType startSearch,
				const SearchType endSearch
			) = 0;

			/**
			 * @fn virtual double onGetMass( const unsigned int index ) = 0
			 * @brief This method is called by getMass method (abstract method)
			 * @param[in] index data index
			 * @return mass
			 */
			virtual double onGetMass( const unsigned int index ) = 0;
			
			/**
			 * @fn virtual int onGetMsStage( const unsigned int index ) = 0
			 * @brief This method is called by getMsStage method (abstract method)
			 * @param[in] index data index
			 * @return MS stage.
			 */
			virtual int onGetMsStage( const unsigned int index ) = 0;

			/**
			 * @fn virtual double onGetPrecursor( const unsigned int index ) = 0
			 * @brief This method is called by getPrecursor method (abstract method)
			 * @param[in] index data index
			 * @return precursor mass
			 */
			virtual double onGetPrecursor( const unsigned int index ) = 0;
		};
	}
}

#endif	// __KOME_OBJECTS_CHROMATOGRAM_H__
