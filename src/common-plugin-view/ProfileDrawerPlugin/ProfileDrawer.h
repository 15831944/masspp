/**
 * @file ProfileDrawer.h
 * @brief interfaces of ProfileDrawer class
 *
 * @author S.Tanaka
 * @date 2006.09.29
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_PROFILE_DRAWER_H__
#define __KOME_VIEW_PROFILE_DRAWER_H__


#include <list>
#include <string>


namespace kome {
	namespace view {
		/**
		 * @class ProfileDrawer
		 * @brief class to draw profile
		 */
		class ProfileDrawer : public kome::objects::DefaultDataManager {
		protected:
			/**
			 * @fn ProfileDrawer()
			 * @brief constructor
			 */
			ProfileDrawer();

			/**
			 * @fn virtual ~ProfileDrawer()
			 * @brief destructor
			 */
			virtual ~ProfileDrawer();

		protected:
			/**
			 * @struct DrawData
			 * @brief draw data
			 */
			struct DrawData {
				int px;
				double x;
				double leftY;
				double topY;
				double bottomY;
				double rightY;
			};

		protected:
			/** spectrum data points information to draw */
			std::map< kome::objects::Spectrum*, kome::img::DrawingXYData* > m_spectrumDrawMap;

			/** chromatogram data points information to draw */
			std::map< kome::objects::Chromatogram*, kome::img::DrawingXYData* > m_chromatogramDrawMap;

		public:
			/**
			 * @fn void drawSpectrum(
					kome::objects::Spectrum* spectrum,
					kome::img::Graphics* graphics
				)
			 * @brief draws spectrum profile
			 * @param spectrum spectrum information
			 * @param graphics graphics object to draw
			 */
			void drawSpectrum(
				kome::objects::Spectrum* spectrum,
				kome::img::Graphics* graphics
			);

			/**
			 * @fn void drawChromatogram(
					kome::objects::Chromatogram* chromatogram,
					kome::img::Graphics* graphics
				)
			 * @brief draws chromatogram profile
			 * @param chromatogram chromatogram information
			 * @param graphics graphics object to draw
			 */
			void drawChromatogram(
				kome::objects::Chromatogram* chromatogram,
				kome::img::Graphics* graphics
			);

		protected:
			/**
			 * @fn void drawProfile(
					kome::img::DrawingXYData* drawData,
					kome::img::Graphics* g,
					double maxSpacing
				)
			 * @brief draws profile
			 * @param drawData profile data object
			 * @param g graphics object to draw
			 * @param maxSpacing max spacing (Inserting zero intensity points)
			 */
			void drawProfile(
				kome::img::DrawingXYData* drawData,
				kome::img::Graphics* g,
				double maxSpacing
			);

			/**
			 * @fn void drawProfileChrom(
					kome::img::DrawingXYData* drawData,
					kome::img::Graphics* g,
					double maxSpacing
				)
			 * @brief draws profile
			 * @param drawData profile data object
			 * @param g graphics object to draw
			 * @param maxSpacing max spacing (Inserting zero intensity points)
			 */
			void drawProfileChrom(
				kome::img::DrawingXYData* drawData,
				kome::img::Graphics* g,
				double maxSpacing
			);

		protected:
			/**
			 * @fn void drawImpulse(
					kome::img::DrawingXYData* drawData,
					kome::img::Graphics* g
				)
			 * @brief draws profile
			 * @param drawData profile data object
			 * @param g graphics object to draw
			 * @param maxSpacing max spacing (Inserting zero intensity points)
			 */
			void drawImpulse(
				kome::img::DrawingXYData* drawData,
				kome::img::Graphics* g
			);
		public:
			/**
			 * @fn void deleteSpectrumData( kome::objects::Spectrum* spectrum )
			 * @brief deletes spectrum data
			 * @param spectrum spectrum object to be deleted
			 */
			void deleteSpectrumData( kome::objects::Spectrum* spectrum );

			/**
			 * @fn void deleteChromatogramData( kome::objects::Chromatogram* chromatogram )
			 * @brief deletes chromatogram data
			 * @param chromatogram chromatogram object to be deleted
			 */
			void deleteChromatogramData( kome::objects::Chromatogram* chromatogram );

		protected:
			/**
			 * @fn virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting )
			 * @brief This method is called when a sample is closed. (override method)
			 * @param sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting );

			/**
			 * @fn virtual void onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting )
			 * @brief This method is called when a spectrum is closed. (override method)
			 * @param spec spectrum object to be opened
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting );

			/**
			 * @fn virtual void onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting )
			 * @brief This method is called when a chromatogram is closed. (override method)
			 * @param chrom chromatogram object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting );

		public:
			/**
			 * @fn static ProfileDrawer& getInstance()
			 * @brief gets instance. (This is the only object.)
			 * @return ProfileDrawer object
			 */
			static ProfileDrawer& getInstance();
		};
	}
}

#endif		//  __KOME_VIEW_PROFILE_DRAWER_H__
