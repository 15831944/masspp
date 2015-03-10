/**
 * @file SpecChromWindowManager.h
 * @brief interfaces of SpecChromWindowManager class
 *
 * @author S.Tanaka
 * @date 2013.05.21
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_VIEW_SPEC_CHROM_WINDOW_MANAGER_H__
#define __KOME_VIEW_SPEC_CHROM_WINDOW_MANAGER_H__


namespace kome {
	namespace view {

		/**
		 * @class SpecChromWindowManager
		 * @brief AppndChildFrame plug-in management class
		 */
		class SpecChromWindowManager : public kome::objects::DefaultDataManager {
		protected:
			/**
			 * @fn SpecChromWindowManager()
			 * @brief constructor
			 */
			SpecChromWindowManager();

			/**
			 * @fn virtual ~SpecChromWindowManager()
			 * @brief destructor
			 */
			virtual ~SpecChromWindowManager();

		protected:
			/** sample */
			kome::objects::Sample* m_sample;

			/** chromatogram frame */
			kome::window::ChildFrame* m_chromFrame;

			/** MS spectrum frame */
			kome::window::ChildFrame* m_msSpecFrame;

			/** MS/MS spectrum frames */
			std::vector< kome::window::ChildFrame* > m_msmsSpecFrames;

			/** data map frames */
			std::vector< kome::window::ChildFrame* > m_dataMapFrames;

			/** client size */
			kome::core::Point< int > m_clientSize;

			/** start RT */
			double m_startRt;

			/** end RT */
			double m_endRt;

			/** link mode */
			bool m_linkMode;

		public:
			/**
			 * @fn void setRt( const double start, const double end )
			 * @brief sets RT
			 * @param[in] start start RT
			 * @param[in] end end RT
			 */
			void setRt( const double start, const double end );

			/**
			 * @fn double getStartRt()
			 * @brief gets start RT
			 */
			double getStartRt();

			/**
			 * @fn double getEndRt()
			 * @brief gets end RT
			 */
			double getEndRt();

			/**
			 * @fn void setLinkMode( const bool flg )
			 * @brief sets the link mode
			 * @param[in] flg If true, link mode
			 */
			void setLinkMode( const bool flg );

			/**
			 * @fn bool isLinkMode()
			 * @brief If true, link mode
			 */
			bool isLinkMode();

			/**
			 * @fn void setClientSize( const int width, const int height )
			 * @brief sets the client size
			 * @param[in] width width
			 * @param[in] height height
			 */
			void setClientSize( const int width, const int height );

			/**
			 * @fn void layoutChildren()
			 * @brief layouts child frames
			 */
			void layoutChildren();

			/**
			 * @fn void closeAllChildren()
			 * @brief closes all frames
			 */
			void closeAllChildren();

			/**
			 * @fn void init()
			 * @brief initializes members
			 */
			void init();

			/**
			 * @fn void checkMemberFrames()
			 * @brief check member frames
			 */
			void checkMemberFrames();

		public:
			/**
			 * @fn void openFrame( kome::objects::Chromatogram* chrom )
			 * @brief opens frame
			 * @param[in] chrom chromatogram
			 */
			void openFrame( kome::objects::Chromatogram* chrom );

			/**
			 * @fn void openFrame( kome::objects::Spectrum* spec )
			 * @brief opens frame
			 * @param[in] spec spectrum
			 */
			void openFrame( kome::objects::Spectrum* spec );

			/**
			 * @fn void openFrame( kome::objects::DataMapInfo* dataMap )
			 * @brief opens frame
			 * @param[in] dataMap data map
			 */
			void openFrame( kome::objects::DataMapInfo* dataMap );

			/**
			 * @fn kome::objects::Spectrum* getSpectra( kome::objects::Spectrum* baseSpec, std::vector< kome::objects::Spectrum* >& msmsSpectra )
			 * @brief gets spectra
			 * @param[in] baseSpec base spectrum
			 * @param[out] msmsSpectra the array to store MS/MS spectra
			 * @return MS spectrum
			 */
			 kome::objects::Spectrum* getSpectra( kome::objects::Spectrum* baseSpec, std::vector< kome::objects::Spectrum* >& msmsSpectra );

		public:
			/**
			 * @fn void nextSpectrum()
			 * @brief opens next spectrum
			 */
			void nextSpectrum();

			/**
			 * @fn void prevSpectrum()
			 * @brief opens previous spectrum
			 */
			void prevSpectrum();

		protected:
			/**
			 * @fn kome::window::ChildFrame* makeFrame()
			 * @brief makes child frame
			 * @return child frame
			 */
			kome::window::ChildFrame* makeFrame();

		public:
			/**
			 * @fn static SpecChromWindowManager& getInstance()
			 * @brief gets object
			 * @return object (This is the only object.)
			 */
			static SpecChromWindowManager& getInstance();

		};
	}
}

#endif	// __KOME_VIEW_SPEC_CHROM_WINDOW_MANAGER_H__
