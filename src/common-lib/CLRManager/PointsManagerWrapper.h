/**
 * @file PointsManagerWrapper.h
 * @brief interfaces of PointsManagerWrapper class
 *
 * @author S.Tanaka
 * @date 2008.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_POINTS_MANAGER_WRAPPER_H__
#define __KOME_CLR_POINTS_MANAGER_WRAPPER_H__


#include "DataManagerWrapper.h"


namespace kome { 
	namespace clr {

		ref class SpectrumWrapper;
		ref class ChromatogramWrapper;
		ref class XYDataWrapper;
		ref class XYDataOperationWrapper;

		/**
		 * @class PointsManagerWrapper
		 * @brief peaks manager wrapper class to use on CLR environment
		 */
		public ref class PointsManagerWrapper : public DataManagerWrapper {
		protected:
			/**
			 * @fn PointsManagerWrapper( kome::objects::PointsManager& mgr )
			 * @brief constructor
			 * @param[in] mgr data manager object
			 */
			PointsManagerWrapper( kome::objects::PointsManager& mgr );

			/**
			 * @fn virtual ~PointsManagerWrapper()
			 * @brief destructor
			 */
			virtual ~PointsManagerWrapper();

		protected:
			/** data manager object */
			kome::objects::PointsManager& m_mgr;

		public:
			/**
			 * @fn kome::objects::PointsManager& getPointsManager()
			 * @brief gets data manager object
			 * @return data manager object
			 */
			kome::objects::PointsManager& getPointsManager();

		public:
			/**
			 * @fn XYDataWrapper^ getXYData( SpectrumWrapper^ spec )
			 * @brief gets xy data of specified spectrum
			 * @param[in] spec spectrum
			 * @return xy data object of specifid spectrum
			 */
			XYDataWrapper^ getXYData( SpectrumWrapper^ spec );

			/**
			 * @fn XYDataWrapper^ getXYData( ChromatogramWrapper^ chrom );
			 * @brief gets xy data of specified chromatogram
			 * @param[in] chrom chromatogram
			 * @return xy data object of specifide chromatogram
			 */
			XYDataWrapper^ getXYData( ChromatogramWrapper^ chrom );

			/**
			 * @fn void deleteXYData( SpectrumWrapper^ spec )
			 * @brief deletes xy data of specified spectrum
			 * @param[in] spec spectrum
			 */
			void deleteXYData( SpectrumWrapper^ spec );

			/**
			 * @fn void deleteXYData( ChromatogramWrapper^ chrom )
			 * @brief deletes xy data of specified chromatogram
			 * @param[in] chrom chromatogram
			 */
			void deleteXYData( ChromatogramWrapper^ chrom );

			/**
			 * @fn void addOperation( SpectrumWrapper^ spec, XYDataOperationWrapper^ operation )
			 * @brief adds data operation to specified spectrum
			 * @param[in] spec spectrum
			 * @param[in] operation data operation object
			 */
			void addOperation( SpectrumWrapper^ spec, XYDataOperationWrapper^ operation );

			/**
			 * @fn void addOperation( ChromatogramWrapper^ chrom, XYDataOperationWrapper^ operation )
			 * @brief adds data operatin to specified chromatogram
			 * @param[in] chrom chromatogram
			 * @param[in] operation data operation object
			 */
			void addOperation( ChromatogramWrapper^ chrom, XYDataOperationWrapper^ operation );

			/**
			 * @fn void removeOperation( SpectrumWrapper^ spec, XYDataOperationWrapper^ operation )
			 * @brief removes data operation from specified spectrum
			 * @param[in] spec spectrum
			 * @param[in] operation data operation object
			 */
			void removeOperation( SpectrumWrapper^ spec, XYDataOperationWrapper^ operation );

			/**
			 * @fn void removeOperation( ChromatogramWrapper^ chrom, XYDataOperationWrapper^ operation )
			 * @brief removes data operation from specified chromtogram
			 * @param[in] chrom chromatogram
			 * @param[in] operation data operation object
			 */
			void removeOperation( ChromatogramWrapper^ chrom, XYDataOperationWrapper^ operation );

			/**
			 * @fn void clearOperations( SpectrumWrapper^ spec )
			 * @brief clears data operation of specified spectrum
			 * @param[in] spec spectrum
			 */
			void clearOperations( SpectrumWrapper^ spec );

			/**
			 * @fn void clearOperations( ChromatogramWrapper^ chrom )
			 * @brief clears data operation of specified chromatogram
			 * @param[in] chrom chromatogram
			 */
			void clearOperations( ChromatogramWrapper^ chrom );

			/**
			 * @fn unsigned int getNumberOfOperations( SpectrumWrapper^ spec )
			 * @brief gets the number of operations of specified spectrum
			 * @param[in] spec spectrum
			 * @return the number of operations of specified spectrum
			 */
			unsigned int getNumberOfOperations( SpectrumWrapper^ spec );

			/**
			 * @fn unsigned int getNumberOfOperations( ChromatogramWrapper^ chrom )
			 * @brief gets the number of operations of specified chromatogram
			 * @param[in] chrom chromatogram
			 * @return the number of specified chromatogram
			 */
			unsigned int getNumberOfOperations( ChromatogramWrapper^ chrom );

			/**
			 * @fn XYDataOperationWrapper^ getOperation( SpectrumWrapper^ spec, unsigned int index )
			 * @brief gets operation of specified spectrum
			 * @param[in] spec spectrum
			 * @param[in] index operation index
			 * @return operation (If NULL, specified spectrum has no operations or the index is illegal.)
			 */
			XYDataOperationWrapper^ getOperation( SpectrumWrapper^ spec, unsigned int index );

			/**
			 * @fn XYDataOperationWrapper^ getOperation( ChromatogramWrapper^ chrom, unsigned int index )
			 * @brief gets operation of specified chromatogram
			 * @param[in] chrom chromatogram
			 * @param[in] index chromatogram index
			 * @return operation (If NULL, specified chromatogram has no operations or the index is illegal.)
			 */
			XYDataOperationWrapper^ getOperation( ChromatogramWrapper^ chrom, unsigned int index );
			
		public:
			/**
			 * @fn static PointsManagerWrapper^ getInstance()
			 * @brief get data points manager object (This is the only object.)
			 * @return data points manager object
			 */
			static PointsManagerWrapper^ getInstance();
		};
	}
}

#endif		// __KOME_CLR_POINTS_MANAGER_WRAPPER_H__
