/**
 * @file XYDataOperation.h
 * @brief interfaces of XYDataOperation class
 *
 * @author S.Tanaka
 * @date 2007.03.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_XY_DATA_OPERATION_H__
#define __KOME_OBJECTS_XY_DATA_OPERATION_H__


#include <string>


namespace kome {
	namespace objects {
		

		class Spectrum;
		class Chromatogram;
		class Sample;

		/**
		 * @class XYDataOperation
		 * @brief xy data operation class
		 */
		class DATA_OBJECTS_CLASS XYDataOperation {
		public:
			/**
			 * @fn XYDataOperation()
			 * @brief constructor
			 */
			XYDataOperation();

			/**
			 * @fn virtual ~XYDataOperation()
			 * @brief destructor
			 */
			virtual ~XYDataOperation();

		protected:
			/** name */
			std::string m_name;
			
			/** index */
			int m_index;

		public:
			/**
			 * @fn void setName( const char* name )
			 * @brief sets name
			 * @param[in] name name 
			 */
			void setName( const char* name );

			/**
			 * @fn const char* getName()
			 * @brief gets name
			 * @return name
			 */
			const char* getName();

			/**
			 * @fn void setIndex( int index )
			 * @brief set index
			 * @param[in] index operation index
			 */
			void setIndex( int index );

			/**
			 * @fn int getIndex()
			 * @brief get index
			 * @return operation index
			 */
			int getIndex();


		public:
			/**
			 * @fn void update( kome::core::XYData& src, kome::core::XYData& dst, Chromatogram& chrom )
			 * @brief updates xy data
			 * @param[in] src source xy data
			 * @param[out] dst the object to store updated xy data
			 * @param[in] chrom chromatogram
			 */
			void update( kome::core::XYData& src, kome::core::XYData& dst, Chromatogram& chrom );

			/**
			 * @fn void update( kome::core::XYData& src, kome::core::XYData& dst, Spectrum& spec )
			 * @brief updates xy data
			 * @param[in] src source xy data
			 * @param[out] dst the object to store updated xy data
			 * @param[in] spec spectrum
			 */
			void update( kome::core::XYData& src, kome::core::XYData& dst, Spectrum& spec );
			
			/**
			 * @fn void update( kome::core::XYData& src, kome::core::XYData& dst, Sample& sample )
			 * @brief updates xy data
			 * @param[in] src source xy data
			 * @param[out] dst the object to store updated xy data
			 * @param[in] sample sample
			 */
			void update( kome::core::XYData& src, kome::core::XYData& dst, Sample& sample );

		protected:
			/**
			 * @fn virtual void onUpdate(
					kome::core::XYData& src,
					kome::core::XYData& dst,
					kome::objects::Chromatogram& chrom
				) = 0
			 * @brief This method is called by update method. (abstract method)
			 * @param[in] src source xy data
			 * @param[out] dst the object to store updated xy data
			 * @param[in] chrom chromatogram
			 */
			virtual void onUpdate(
				kome::core::XYData& src,
				kome::core::XYData& dst,
				kome::objects::Chromatogram& chrom
			) = 0;

			/**
			 * @fn virtual void onUpdate(
					kome::core::XYData& src,
					kome::core::XYData& dst,
					kome::objects::Spectrum& spec
				) = 0
			 * @brief This method is called by update method. (abstract method)
			 * @param[in] src source xy data
			 * @param[out] dst the object to store updated xy data
			 * @param[in] spec spectrum
			 */
			virtual void onUpdate(
				kome::core::XYData& src,
				kome::core::XYData& dst,
				kome::objects::Spectrum& spec
			) = 0;

			/**
			 * @fn virtual void onUpdate(
					kome::core::XYData& src,
					kome::core::XYData& dst,
					kome::objects::Sample& sample
				) = 0
			 * @brief This method is called by update method. (abstract method)
			 * @param[in] src source xy data
			 * @param[out] dst the object to store updated xy data
			 * @param[in] sample sample
			 */
			virtual void onUpdate(
				kome::core::XYData& src,
				kome::core::XYData& dst,
				kome::objects::Sample& sample
			) = 0;
		};
	}
}

#endif		// __KOME_OBJECTS_XY_DATA_OPERATION_H__
