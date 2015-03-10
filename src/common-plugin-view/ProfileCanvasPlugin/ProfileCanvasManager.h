/**
 * @file ProfileCanvasManager.h
 * @brief interfaces of ProfileCanvasManager class
 *
 * @author M.Izumi
 * @date 2012.02.27
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_VIEW_PROFILE_CANVAS_MANAGER_H__
#define __KOME_VIEW_PROFILE_CANVAS_MANAGER_H__

namespace kome {
	namespace view {
		class ProfileCanvasOperation;
		/**
		 * @class ProfileCanvasManager
		 * @brief profile canvas management class
		 */
		class ProfileCanvasManager : public kome::objects::DefaultDataManager {
		protected:
			/**
			 * @fn ProfileCanvasManager()
			 * @brief constructor
			 */
			ProfileCanvasManager();

			/**
			 * @fn virtual ~ProfileCanvasManager()
			 * @brief destructor
			 */
			virtual ~ProfileCanvasManager();

		protected:
			kome::objects::Spectrum* m_spec;
			kome::objects::Chromatogram* m_chrom;

		public:
			void setSpec( kome::objects::Spectrum* spec ){ m_spec = spec; }
			kome::objects::Spectrum* getSpec(){ return m_spec; }

			void setChrom( kome::objects::Chromatogram* chrom ){ m_chrom = chrom; }
			kome::objects::Chromatogram* getChrom(){ return m_chrom; }
		protected:
			/**
			 * @fn virtual void onOpenSpectrum( kome::objects::Spectrum* spec ) 
			 * @brief This method is called when a spectrum is opened. (override method)
			 * @param[in] spec spectrum object to be opened
			 */
			virtual void onOpenSpectrum( kome::objects::Spectrum* spec ) ;

			/**
			 * @fn virtual void onOpenChromatogram( kome::objects::Chromatogram* chrom ) 
			 * @brief This method is called when a chromatogram is opened. (override method)
			 * @param[in] chrom chromatogram object to be opened
			 */
			virtual void onOpenChromatogram( kome::objects::Chromatogram* chrom ) ;
		public:

			/**
			 * @fn static ProfileCanvasManager& getInstance()
			 * @brief gets object
			 * @return object (This is the only object.)
			 */
			static ProfileCanvasManager& getInstance();
		};
	}
}

#endif //__KOME_VIEW_PROFILE_CANVAS_MANAGER_H__
