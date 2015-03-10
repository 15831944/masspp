/**
 * @file LcmsSpectrumBase.h
 * @brief interfaces of LcmsSpectrumBase class
 *
 * @author S.Tanaka
 * @date 2013.01.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_LCMS_SPECTRUM_BASE_H__
#define __KOME_IO_LCMS_SPECTRUM_BASE_H__


namespace kome {
	namespace io {
		namespace lcms {

			ref class LcmsSample;

			/**
			 * @class LcmsSpectrumBase
			 * @brief spectrum object class that Lcms raw data has
			 */
			public ref class LcmsSpectrumBase abstract : public kome::clr::ClrSpectrumBase {
			public:
				/**
				 * @fn LcmsSpectrumBase( LcmsSample^ sample, System::String^ name )
				 * @brief constructor
				 * @param sample sample object that has this spectrum
				 * @param name spectrum name
				 */
				LcmsSpectrumBase( LcmsSample^ sample, System::String^ name );

				/**
				 * @fn ~LcmsSpectrumBase()
				 * @brief destructor
				 */
				~LcmsSpectrumBase();

			protected:
				/** sample */
				LcmsSample^ m_sample;

				/** scan type */
				System::String^ m_scanType;

				/** precursor charge */
				int m_precCharge;

				/** segment number */
				int m_segNum;

				/** event number */
				int m_evtNum;

			public:
				/**
				 * @fn void setScanType( int type )
				 * @brief sets scan type
				 * @param type scan type number
				 */
				void setScanType( int type );

				/**
				 * @fn void setScanType( System::String^ type )
				 * @brief sets scan type
				 * @param type scan type
				 */				
				void setScanType( System::String^ type );

				/**
				 * @fn System::String^ getScanType()
				 * @brief gets scan type
				 * @return scan type
				 */
				System::String^ getScanType();

				/**
				 * @fn void setSegmentNumber( int segNum )
				 * @brief sets the segment number
				 * @param[in] segNum segment number
				 */
				void setSegmentNumber( int segNum );

				/**
				 * @fn int getSegmentNumber()
				 * @brief gets the segment number
				 * @return segment number
				 */
				int getSegmentNumber();

				/**
				 * @fn void setEventNumber( int evtNum )
				 * @brief sets the event number
				 * @param[in] evtNum event number
				 */
				void setEventNumber( int evtNum );

				/**
				 * @fn int getEventNumber()
				 * @brief gets the event number
				 * @return event number
				 */
				int getEventNumber();

			public:
				/**
				 * @fn virtual void getApiPeaks( kome::clr::PeaksWrapper^ peaks ) = 0
				 * @brief gets API peaks (abstract method)
				 * @param[out] peaks the object to store peaks information.
				 */
				virtual void getApiPeaks( kome::clr::PeaksWrapper^ peaks ) = 0;
			};
		}
	}
}

#endif	// __KOME_IO_LCMS_SPECTRUM_BASE_H__
