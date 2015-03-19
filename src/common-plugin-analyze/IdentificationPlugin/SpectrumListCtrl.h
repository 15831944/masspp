/**
 * @file SpectrumListCtrl.h
 * @brief interfaces of SpectrumListCtrl class
 *
 * @author H.Parry
 * @date 2012.01.30
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_IDENT_SPECTRUM_LIST_CTRL_H__
#define __KOME_IDENT_SPECTRUM_LIST_CTRL_H__


namespace kome {
	namespace ident {

		/**
		 * @class SpectrumListCtrl
		 * @brief peak position list control class
		 */
		class SpectrumListCtrl : public kome::window::CheckGridListCtrl {
		public:
			/**
			 * @fn SpectrumListCtrl( wxWindow* parent, const int width, const int height )
			 * @brief constructor
			 * @param parent parent window
			 * @param width width
			 * @param height height
			 */
			SpectrumListCtrl( wxWindow* parent, const int width, const int height );

			/**
			 * @fn virtual ~SpectrumListCtrl()
			 * @brief destructor
			 */
			virtual ~SpectrumListCtrl();

		protected:
			/** Spectrum column */
			int m_spectrumCol;

			/** RT column */
			int m_rtCol;

			/** m/z column */
			int m_stageCol;

			/** charge column */
			int m_precursorCol;

			/** appended base column */
			int m_appendedCol;

		protected:
			/** displayed spectra */
			std::vector< kome::objects::Spectrum* > m_spectra;

		public:
			/**
			 * @fn unsigned int getNumberOfSpectra()
			 * @brief gets the number of spectra
			 * @return the number of spectra
			 */
			unsigned int getNumberOfSpectra();

			/**
			 * @fn kome::objects::Spectrum* getSpectrum( const unsigned int idx )
			 * @brief gets the spectrum
			 * @param[in] idx spectrum index
			 * @return spectrum
			 */
			kome::objects::Spectrum* getSpectrum( const unsigned int idx );

			/**
			 * @fn void clearSpectra()
			 * @brief clears spectra
			 */
			void clearSpectra();

			/**
			 * @fn void addSpectrum( kome::objects::Spectrum* spec )
			 * @brief adds spectrum
			 * @param[in] spec spectrum
			 */
			void addSpectrum( kome::objects::Spectrum* spec );

		protected:
			/**
			 * @fn virtual void onCreate()
			 * @brief This method is called by create method
			 */
			virtual void onCreate();

			/**
			 * @fn virtual int getInt( const long long data, const int column ) const
			 * @brief gets integer value from data (override method)
			 * @param data data
			 * @param column column number
			 * @return integer value
			 */
			virtual int getInt( const long long data, const int column ) const;

			/**
			 * @fn virtual double getDouble( const long long data, const int column ) const
			 * @brief gets double value from data
			 * @param data data
			 * @param column column number
			 */
			virtual double getDouble( const long long data, const int column ) const;

			/**
			 * @fn virtual std::string getString( const long long data, const int column ) const
			 * @brief gets character string value (override method)
			 * @param data data
			 * @param column column number
			 */
			virtual std::string getString( const long long data, const int column ) const;

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif	// __KOME_IDENT_SPECTRUM_LIST_CTRL_H__
