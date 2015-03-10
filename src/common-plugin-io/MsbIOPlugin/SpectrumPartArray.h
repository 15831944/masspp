/**
 * @file SpectrumPartArray.h
 * @brief interfaces of SpectrumPartArray class
 *
 * @author S.Tanaka
 * @date 2008.07.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_MSB_SPECTRUM_PART_ARRAY_H__
#define __KOME_IO_MSB_SPECTRUM_PART_ARRAY_H__


#include "SpectrumPart.h"

#include <vector>


namespace kome {
	namespace io {
		namespace msb {

			/**
			 * @class SpectrumPart
			 * @brief partial datapoints of spectrum information
			 */
//			class SpectrumPartArray : public kome::core::XYData {
			class SpectrumPartArray {
			public:
				/**
				 * @fn SpectrumPartArray()
				 * @brief constructor
				 */
				SpectrumPartArray();

				/**
				 * @fn virtual ~SpectrumPartArray()
				 * @brief destructor
				 */
				virtual ~SpectrumPartArray();

			protected:
				/** spectrum */
				kome::objects::Spectrum* m_spec;

				/** spectrum parts */
				std::vector< SpectrumPart > m_parts;

			public:
				/** 
				 * @fn void setSpectrum( kome::objects::Spectrum* spec )			 
				 * @brief sets spectrum
				 * @param spec spectrum
				 */
				void setSpectrum( kome::objects::Spectrum* spec );

				/**
				 * @fn kome::objects::Spectrum* getSpectrum()
				 * @brief gets spectrum
				 * @return spectrum
				 */
				kome::objects::Spectrum* getSpectrum();

			public:
				/**
				 * @fn SpectrumPart& addPart()
				 * @brief add spectrum part
				 * @return new spectrum part object
				 */
				SpectrumPart& addPart();

				/**
				 * @fn unsigned int getNumberOfParts()
				 * @brief gets the number of spectrum parts
				 * @return the number of spectrum parts
				 */
				unsigned int getNumberOfParts();

				/**
				 * @fn SpectrumPart* getPart( const unsigned int index )
				 * @brief gets spectrum part
				 * @param index spectrum part index
				 * @return spectrum part (If NULL, the index is illegal.)
				 */
				SpectrumPart* getPart( const unsigned int index );

				/**
				 * @fn int getPartIndex( const double mz )
				 * @brief gets index of spectrum part that includes mz
				 * @return spectrum part index
				 */
				int getPartIndex( const double mz );

		protected:
			/**
			 * @fn virtual bool onLoadData( boost::function< int ( void*, int ) > readFun )
			 * @brief loads data from file (override method)
			 * @param readFun read function
			 * @return If true, it succeeded to load the data.
			 */
			virtual bool onLoadData( boost::function< int ( void*, int ) > readFun );

			/**
			 * @fn virtual bool onSaveData( boost::function< int ( void*, int ) > writeFun )
			 * @brief saves data to file (override method)
			 * @param writeFun write function
			 * @return If true, it succeeded to save the data
			 */
			virtual bool onSaveData( boost::function< int ( void*, int ) > writeFun );

		protected:
				/**
				 * @fn virtual void onClearPoints()
				 * @brief This method is called by clearPoints method. (override method)
				 */
				virtual void onClearPoints();

				/**
				 * @fn virtual void onAddPoint( const double x, const double y )
				 * @brief This method is called by addPoint method. (override method)
				 * @param x x coordinate of point to be added
				 * @param y y coordinate of point to be added
				 */
				virtual void onAddPoint( const double x, const double y );

				/**
				 * @fn virtual void onInsertPoint( const unsigned int index, const double x, const double y )
				 * @brief This method is called by insertPoint method. (override method)
				 * @param index insert position
				 * @param x x coordinate of point
				 * @param y y coordinate of point
				 */
				virtual void onInsertPoint( const unsigned int index, const double x, const double y );

				/**
				 * @fn virtual void onDeletePoint( const unsigned int index )
				 * @brief This method is called by deletePoint method. (override method)
				 * @param index point index
				 */
				virtual void onDeletePoint( const unsigned int index );

				/**
				 * @fn virtual unsigned int onGetLength()
				 * @brief this method is called by getLength method (override method)
				 * @return the number of points
				 */
				virtual unsigned int onGetLength();

				/**
				 * @fn virtual double onGetX( const unsigned int index )
				 * @brief This method is called by getX method. (override method)
				 * @param index the index of point
				 * @return x coordinate
				 */
				virtual double onGetX( const unsigned int index );

				/**
				 * @fn virtual double onGetY( const unsigned int index )
				 * @brief This method is called by getY method. (override method)
				 * @param index the index of point
				 * @return y coordiante
				 */
				virtual double onGetY( const unsigned int index );

				/**
				 * @fn virtual void onReserve( const unsigned int num )
				 * @brief This method is called by reserve method. (override method)
				 * @param num of points to be reserved
				 */
				virtual void onReserve( const unsigned int num );

			protected:
				/**
				 * @fn static int comparePart( SpectrumPart* part, double& mz )
				 * @brief compare spectrum part and mz for binary search
				 * @param part spectrum part to be compared
				 * @param mz m/z to compare
				 * @return - If range of part includes mz, this method returns 0.
						- If minimum value of part is greater than mz, this method returns 1.
						- If maximum value of part is less than mz, this method returns -1.
				 */
				static int comparePart( SpectrumPart* part, double& mz );
			};
		}
	}
}

#endif	//  __KOME_IO_MSB_SPECTRUM_PART_ARRAY_H__
