/**
 * @file SpectrumGetter.h
 * @brief interfaces of SpectrumGetter class
 *
 * @author S.Tanaka
 * @date 2012.08.02
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_IDENT_SPECTRUM_GETTER_H__
#define __KOME_IDENT_SPECTRUM_GETTER_H__


namespace kome {
	namespace ident {

		/**
		 * @class SpectrumGetter
		 * @brief spectrum getter class
		 */
		class SpectrumGetter {
		public:
			/**
			 * @fn SpectrumGetter()
			 * @brief constructor
			 */
			SpectrumGetter();

			/**
			 * @fn virtual ~SpectrumGetter()
			 * @brief destructor
			 */
			 virtual ~SpectrumGetter();

		public:
			/**
			 * @fn virtual bool next( kome::core::Progress& progress ) = 0
			 * @brief prepares next spectrum (abstract method)
			 * @param[out] progress the object to show the progress
			 * @return If true, the next spectrum exists.
			 */
			virtual bool next( kome::core::Progress& progress ) = 0;

			/**
			 * @fp virtual kome::objects::Spectrum* getSpectrum() = 0
			 * @brief gets the spectrum (abstract method)
			 * @return spectrum
			 */
			virtual kome::objects::Spectrum* getSpectrum() = 0;

			/**
			 * @fn virtual unsigned int getNumberOfPeakIds() = 0
			 * @brief gets the number of peak IDs (abstract method)
			 * @return the number of peak Ids
			 */
			virtual unsigned int getNumberOfPeakIds() = 0;

			/**
			 * @fn virtual int getPeakId( const unsigned int idx ) = 0
			 * @brief gets the peak ID (abstract method)
			 * @param[in] idx peak ID index
			 * @return peak ID
			 */
			virtual int getPeakId( const unsigned int idx ) = 0;

			/**
			 * @fn virtual void setParameters( const char* s ) = 0
			 * @brief sets parameters from character string (abstract method)
			 * @param[in] s character string
			 */
			virtual void setParameters( const char* s ) = 0;

			/**
			 * @fn virtual std::string getParameters() = 0
			 * @brief gets parameters (abstract method)
			 * @return parameter character string
			 */
			virtual std::string getParameters() = 0;
		};
	}
}


#endif    // __KOME_IDENT_SPECTRUM_GETTER_H__
