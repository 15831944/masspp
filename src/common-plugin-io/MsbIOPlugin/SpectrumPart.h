/**
 * @file SpectrumPart.h
 * @brief interfaces of SpectrumPart class
 *
 * @author S.Tanaka
 * @date 2007.02.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_MSB_SPECTRUM_PART_H__
#define __KOME_IO_MSB_SPECTRUM_PART_H__


namespace kome {
	namespace io {
		namespace msb {

			/**
			 * @class SpectrumPart
			 * @brief partial datapoints of spectrum information
			 */
			class SpectrumPart {
			public:
				/**
				 * @fn SpectrumPart()
				 * @brief constructor
				 */
				SpectrumPart();

				/**
				 * @fn virtual ~SpectrumPart()
				 * @brief destructor
				 */
				virtual ~SpectrumPart();

			protected:
				/** m/z */
				float m_mz;
				/** minimum m/z */
				float m_minMz;
				/** maximum m/z */
				float m_maxMz;
				/** total intensity */
				float m_totalIntensity;
				/** min intensity */
				float m_minIntensity;
				/** max intensity */
				float m_maxIntensity;
				/** the number of points */
				unsigned int m_length;
				/** m/z data align */
				unsigned int m_msAlign;
				/** intensity align */
				unsigned int m_intAlign;

			public:
				/**
				 * @fn void setMz( const float mz )
				 * @brief sets m/z
				 * @param mz m/z
				 */
				void setMz( const float mz );

				/**
				 * @fn float getMz()
				 * @brief get m/z
				 * @return m/z
				 */
				float getMz();

				/**
				 * @fn void setMinMz( const float mz )
				 * @brief sets min m/z
				 * @param mz min m/z
				 */
				void setMinMz( const float mz );

				/**
				 * @fn float getMinMz()
				 * @brief gets min m/z
				 * @return min m/z
				 */
				float getMinMz();

				/**
				 * @fn void setMaxMz( const float mz )
				 * @brief sets max m/z
				 * @param mz max m/z
				 */						 
				void setMaxMz( const float mz );

				/**
				 * @fn float getMaxMz()
				 * @brief gets max m/z
				 * @return max m/z
				 */
				float getMaxMz();

				/**
				 * @fn void setTotalIntensity( const float totalIntensity )
				 * @brief sets total intensity
				 * @param totalIntensity total intensity
				 */
				void setTotalIntensity( const float totalIntensity );

				/**
				 * @fn float getTotalIntensity()
				 * @brief gets total intensity
				 * @return total intensity
				 */
				float getTotalIntensity();

				/**
				 * @fn void setMinIntensity( const float intensity )
				 * @brief sets min intensity
				 * @param intensity min intensity
				 */
				void setMinIntensity( const float intensity );

				/**
				 * @fn float getMinIntensity()
				 * @brief gets min intensity
				 * @return min intensity
				 */
				float getMinIntensity();

				/**
				 * @fn void setMaxIntensity( const float intensity )
				 * @brief sets max intensity
				 * @param intensity max intensity
				 */
				void setMaxIntensity( const float intensity );

				/**
				 * @fn float getMaxIntensity()
				 * @brief gets max intensity
				 * @return max intensity
				 */
				float getMaxIntensity();

				/**
				 * @fn void setLength( const unsigned int length )
				 * @brief sets the number of points
				 * @param length the number of points
				 */
				void setLength( const unsigned int length );

				/**
				 * @fn unsigned int getLength()
				 * @brief gets the number of points
				 * @return the number of points
				 */
				unsigned int getLength();

				/**
				 * @fn void setMsAlign( const unsigned int align )
				 * @brief sets m/z data align
				 * @param align m/z data align
				 */
				void setMsAlign( const unsigned int align );

				/**
				 * @fn unsigned int getMsAlign()
				 * @brief gets m/z data align
				 * @return m/z data align
				 */
				unsigned int getMsAlign();

				/**
				 * @fn void setIntAlign( const unsigned int align )
				 * @brief sets intensity data align
				 * @param align intensity data align
				 */
				void setIntAlign( const unsigned int align );

				/**
				 * @fn unsigned int getIntAlign()
				 * @brief gets intensity data align
				 * @return intensity data align
				 */
				unsigned int getIntAlign();

			};
		}
	}
}

#endif		// __KOME_IO_MSB_SPECTRUM_PART_H__

