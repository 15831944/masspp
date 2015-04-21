/**
 * @file MgfManager.h
 * @brief interfaces of MgfManager class
 *
 * @author S.Tanaka
 * @date 2008.10.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IDENT_MGF_MANAGER_H__
#define __KOME_IDENT_MGF_MANAGER_H__


namespace kome {
	namespace ident {

		/**
		 * @class MgfManager
		 * @brief mascot generic format IO management class
		 */
		class IDENT_TOOLS_CLASS MgfManager {
		protected:
			/**
			 * @fn MgfManager()
			 * @brief constructor
			 */
			MgfManager();

			/**
			 * @fn virtual ~MgfManager()
			 * @brief destructor
			 */
			virtual ~MgfManager();

	    public:
			/**
			 * @fn void writeMisHeader( FILE* fp )
			 * @brief writes Mis Header
			 * @param[in] fp file descriptor
			 */
			void writeMisHeader( FILE* fp );

			/**
			 * @fn void writeMisPeaks( FILE* fp, SpecInfo* spec, kome::objects::Peaks* peaks )
			 * @brief writes MIS peaks
			 * @param[in] fp file descriptor to write
			 * @param[in] spec spectrum information
			 * @param[in] peaks peaks information
			 */
			void writeMisPeaks( FILE* fp, SpecInfo* spec, kome::objects::Peaks* peaks ); 

		public:
			/**
			 * @fn bool writePmf( const char* path, kome::objects::Peaks* const peakList )
			 * @brief write the specified peak list to a PMF file at the specified path
			 * @param[in] path output file path
			 * @param[in] peakList peak list
			 * @return If true, it succeeded to write the PMF file.
			 */
			bool writePmf( const char* path, kome::objects::Peaks* const peakList );

		public:
			/**
			 * @fn std::string getSpectrumTitle( SpecInfo* spec, const int charge )
			 * @brief gets the spectrum title
			 * @param[in] spec spectrum information
			 * @param[in] charge charge state
			 * @return spectrum title
			 */
			std::string getSpectrumTitle( SpecInfo* spec, const int charge );

		public:
			/**
			 * @fn static MgfManager& getInstance()
			 * @brief gets object
			 * @return MgfManager object. (This is the only object.)
			 */
			static MgfManager& getInstance();
		};
	}
}

#endif	// __KOME_IDENT_MGF_MANAGER_H__
