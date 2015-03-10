/**
 * @file BackgroundSubtraction.h
 * @brief interfaces of BackgroundSubtraction class
 *
 * @author S.Tanaka
 * @date 2009.12.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_BACKGROUND_SUBTRACTION_H__
#define __KOME_BACKGROUND_SUBTRACTION_H__


namespace kome {
	namespace background {

		/**
		 * @class BackgroundSubtraction
		 * @brief background subtraction class
		 */
		class BackgroundSubtraction : public kome::objects::XYDataOperation {
		public:
			/**
			 * @fn BackgroundSubtraction()
			 * @brief constructor
			 */
			BackgroundSubtraction();

			/**
			 * @fn virtual ~BackgroundSubtraction()
			 * @brief destructor
			 */
			virtual ~BackgroundSubtraction();

		protected:
			/** background points */
			kome::core::DataPoints m_bg;

			/** sample */
			kome::objects::Sample* m_sample;

			/** min RT */
			double m_minRt;

			/** max RT */
			double m_maxRt;

		public:
			/**
			 * @fn void clearBackground()
			 * @brief clears backgroudn data
			 */
			void clearBackground();

			/**
			 * @fn void setBackground( kome::core::XYData& xyData )
			 * @brief sets background data
			 * @param xyData background data
			 */
			void setBackground( kome::core::XYData& xyData );

			/**
			 * @fn void setRt( const double minRt, const double maxRt )
			 * @brief sets the RT range
			 * @param[in] minRt min RT
			 * @param[in] maxRt max RT
			 */
			void setRt( const double minRt, const double maxRt );

		public:
			/**
			 * @fn unsigned int getNumberOfPoints()
			 * @brief gets the number of background points
			 * @return the number of background points
			 */
			unsigned int getNumberOfPoints();

			/**
			 * @fn double getX( const unsigned int idx )
			 * @brief gets the x-coordinate of the background point
			 * @param idx point index
			 * @return the x-coordinate of the background point
			 */
			double getX( const unsigned int idx );

			/**
			 * @fn double getY( const unsigned int idx )
			 * @brief gets the y-coordinate of the background point
			 * @param idx point index
			 * @@return the y-coordinate of the background point
			 */
			double getY( const unsigned int idx );


		protected:
			/**
			 * @fn void subtractBackground( kome::core::XYData& src, kome::core::XYData& dst )
			 * @brief subtracts background
			 * @param src source data points
			 * @param the object to store subtracted data
			 */
			void subtractBackground( kome::core::XYData& src, kome::core::XYData& dst );

		protected:
			/**
			 * @fn virtual void onUpdate(
					kome::core::XYData& src,
					kome::core::XYData& dst,
					kome::objects::Chromatogram& chrom
				)
			 * @brief This method is called by update method. (override method)
			 * @param src source xy data
			 * @param dst the object to store updated xy data
			 * @param chrom chromatogram
			 */
			virtual void onUpdate(
				kome::core::XYData& src,
				kome::core::XYData& dst,
				kome::objects::Chromatogram& chrom
			);

			/**
			 * @fn virtual void onUpdate(
					kome::core::XYData& src,
					kome::core::XYData& dst,
					kome::objects::Spectrum& spec
				)
			 * @brief This method is called by update method. (override method)
			 * @param src source xy data
			 * @param dst the object to store updated xy data
			 * @param spec spectrum
			 */
			virtual void onUpdate(
				kome::core::XYData& src,
				kome::core::XYData& dst,
				kome::objects::Spectrum& spec
			);

			/**
			 * @fn virtual void onUpdate(
					kome::core::XYData& src,
					kome::core::XYData& dst,
					kome::objects::Sample& 
				)
			 * @brief This method is called by update method. (override method)
			 * @param src source xy data
			 * @param dst the object to store updated xy data
			 * @param sample sample
			 */
			virtual void onUpdate(
				kome::core::XYData& src,
				kome::core::XYData& dst,
				kome::objects::Sample& sample
			);
		};
	}
}


#endif	// __KOME_BACKGROUND_SUBTRACTION_H__
