/**
 * @file PointsManager.h
 * @brief interfaces of PointsManager class
 *
 * @author S.Tanaka
 * @date 2007.12.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_POINTS_MANAGER_H__
#define __KOME_OBJECTS_POINTS_MANAGER_H__


#include "DefaultDataManager.h"

#include <vector>
#include <map>
#include <utility>


namespace kome {
	namespace objects {

		class Sample;
		class Spectrum;
		class Chromatogram;
		class XYDataOperation;
		class DataMapInfo;

		/**
		 * @class PointsManager
		 * @brief data points management class
		 */
		class DATA_OBJECTS_CLASS PointsManager : public DefaultDataManager {
		protected:
			/**
			 * @fn PointsManager()
			 * @brief constructor
			 */
			PointsManager();

			/**
			 * @fn virtual ~PointsManager()
			 * @brief destructor
			 */
			virtual ~PointsManager();

		protected:
			/** operation count map */
			std::map< XYDataOperation*, int > m_countMap;

			/** spectrum data map */
			std::map< Spectrum*, kome::core::DataPoints > m_specPointsMap;

			/** chromatogram data map */
			std::map< Chromatogram*, kome::core::DataPoints > m_chromPointsMap;

			/** sample data map */
			std::map< Sample*, kome::core::DataPoints > m_samplePointsMap;

			/** spectrum operation map */
			std::map< Spectrum*, std::vector< XYDataOperation* > > m_specOperationMap;

			/** chromatogram operatin map */
			std::map< Chromatogram*, std::vector< XYDataOperation* > > m_chromOperationMap;

			/** sample operation map */
			std::map< Sample*, std::vector< XYDataOperation* > > m_sampleOperationMap;

		public:
			/**
			 * @fn kome::core::XYData* getXYData( Spectrum* spec )
			 * @brief gets xy data of specified spectrum
			 * @param[in] spec spectrum
			 * @return xy data object of specifid spectrum
			 */
			kome::core::XYData* getXYData( Spectrum* spec );

			/**
			 * @fn kome::core::XYData* getXYData( Chromatogram* chrom )
			 * @brief gets xy data of specified chromatogram
			 * @param[in] chrom chromatogram
			 * @return xy data object of specifide chromatogram
			 */
			kome::core::XYData* getXYData( Chromatogram* chrom );

			/**
			 * @fn void deleteXYData( Spectrum* spec )
			 * @brief deletes xy data of specified spectrum
			 * @param[in] spec spectrum
			 */
			void deleteXYData( Spectrum* spec );

			/**
			 * @fn void deleteXYData( Chromatogram* chrom )
			 * @brief deletes xy data of specified chromatogram
			 * @param[in] chrom chromatogram
			 */
			void deleteXYData( Chromatogram* chrom );
						
			/**
			 * @fn void addOperation( Spectrum* spec, XYDataOperation* operation )
			 * @brief adds data operation to specified spectrum
			 * @param[in] spec spectrum
			 * @param[in] operation data operation object
			 */
			void addOperation( Spectrum* spec, XYDataOperation* operation );

			/**
			 * @fn void addOperation( Chromatogram* chrom, XYDataOperation* operation )
			 * @brief adds data operatin to specified chromatogram
			 * @param[in] chrom chromatogram
			 * @param[in] operation data operation object
			 */
			void addOperation( Chromatogram* chrom, XYDataOperation* operation );

			/**
			 * @fn void addOperation( Sample* sample, XYDataOperation* operation )
			 * @brief adds data operatin to specified sample
			 * @param[in] sample sample object
			 * @param[in] operation data operation object
			 */
			void addOperation( Sample* sample, XYDataOperation* operation );

			/**
			 * @fn void removeOperation( Spectrum* spec, XYDataOperation* operation )
			 * @brief removes data operation from specified spectrum
			 * @param[in] spec spectrum
			 * @param[in] operation data operation object
			 */
			void removeOperation( Spectrum* spec, XYDataOperation* operation );

			/**
			 * @fn void removeOperation( Chromatogram* chrom, XYDataOperation* operation )
			 * @brief removes data operation from specified chromtogram
			 * @param[in] chrom chromatogram
			 * @param[in] operation data operation object
			 */
			void removeOperation( Chromatogram* chrom, XYDataOperation* operation );

			/**
			 * @fn void removeOperation( Sample* sample, XYDataOperation* operation )
			 * @brief removes data operation from specified sample
			 * @param[in] sample sample object
			 * @param[in] operation data operation object
			 */
			void removeOperation( Sample* sample, XYDataOperation* operation );

			/**
			 * @fn void clearOperations( Spectrum* spec )
			 * @brief clears data operation of specified spectrum
			 * @param[in] spec spectrum
			 */
			void clearOperations( Spectrum* spec );

			/**
			 * @fn void clearOperations( Chromatogram* chrom )
			 * @brief clears data operation of specified chromatogram
			 * @param[in] chrom chromatogram
			 */
			void clearOperations( Chromatogram* chrom );
			
			/**
			 * @fn void clearOperations( Sample* sample )
			 * @brief clears data operation of specified sample
			 * @param[in] sample sample object
			 */
			void clearOperations( Sample* sample );

			/**
			 * @fn unsigned int getNumberOfOperations( Spectrum* spec )
			 * @brief gets the number of operations of specified spectrum
			 * @param[in] spec spectrum
			 * @return the number of operations of specified spectrum
			 */
			unsigned int getNumberOfOperations( Spectrum* spec );

			/**
			 * @fn unsigned int getNumberOfOperations( Chromatogram* chrom )
			 * @brief gets the number of operations of specified chromatogram
			 * @param[in] chrom chromatogram
			 * @return the number of specified chromatogram
			 */
			unsigned int getNumberOfOperations( Chromatogram* chrom );

			/**
			 * @fn unsigned int getNumberOfOperations( Sample* sample )
			 * @brief gets the number of operations of specified sample
			 * @param[in] sample sample object
			 * @return the number of specified sample
			 */
			unsigned int getNumberOfOperations( Sample* sample );

			/**
			 * @fn XYDataOperation* getOperation( Spectrum* spec, const unsigned int index )
			 * @brief gets operation of specified spectrum
			 * @param[in] spec spectrum
			 * @param[in] index operation index
			 * @return operation (If NULL, specified spectrum has no operations or the index is illegal.)
			 */
			XYDataOperation* getOperation( Spectrum* spec, const unsigned int index );

			/**
			 * @fn XYDataOperation* getOperation( Chromatogram* chrom, const unsigned int index )
			 * @brief gets operation of specified chromatogram
			 * @param[in] chrom chromatogram
			 * @param[in] index chromatogram index
			 * @return operation (If NULL, specified chromatogram has no operations or the index is illegal.)
			 */
			XYDataOperation* getOperation( Chromatogram* chrom, const unsigned int index );

			/**
			 * @fn XYDataOperation* getOperation( Sample* sample, const unsigned int index )
			 * @brief gets operation of specified sample
			 * @param[in] sample sample object
			 * @param[in] index sample index
			 * @return operation (If NULL, specified sample has no operations or the index is illegal.)
			 */
			XYDataOperation* getOperation( Sample* sample, const unsigned int index );

		protected:
			/**
			 * @fn void deleteOperation( XYDataOperation* operation )
			 * @brief deletes operation object
			 * @param[in] operation operation object to be deleted
			 */
			void deleteOperation( XYDataOperation* operation );

			/**
			 * @fn static bool lessOperation( XYDataOperation* opt0, XYDataOperation* opt1 )
			 * @brief compare index of XYDataOperation to sort
			 * @param[in] opt0 XYDataOperation to be compared
			 * @param[in] opt1 XYDataOperation to compare
			 * @return If opt0 sorts before opt1, this method returns true.
			 */
			static bool lessOperation( XYDataOperation* opt0, XYDataOperation* opt1 );
		protected:
			/**
			 * @fn virtual void onCloseSample( Sample* sample, const bool deleting )
			 * @brief This method is called when a sample is closed. (override method)
			 * @param[in] sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSample( Sample* sample, const bool deleting );

			/**
			 * @fn virtual void onCloseSpectrum( Spectrum* spec, const bool deleting )
			 * @brief This method is called when a spectrum is closed. (override method)
			 * @param[in] spec spectrum object to be opened
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSpectrum( Spectrum* spec, const bool deleting );

			/**
			 * @fn virtual void onCloseChromatogram( Chromatogram* chrom, const bool deleting )
			 * @brief This method is called when a chromatogram is closed. (override method)
			 * @param[in] chrom chromatogram object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseChromatogram( Chromatogram* chrom, const bool deleting );

		public:
			/**
			 * @fn static PointsManager& getInstance()
			 * @brief get data points manager object (This is the only object.)
			 * @return data points manager object
			 */
			static PointsManager& getInstance();
		};
	}
}


#endif		// __KOME_OBJECTS_POINTS_MANAGER_H__
