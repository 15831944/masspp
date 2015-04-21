/**
 * @file SampleWrapper.h
 * @brief interfaces of SampleWrapper class
 *
 * @author S.Tanaka
 * @date 2007.12.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_SAMPLE_WRAPPER_H__
#define __KOME_CLR_SAMPLE_WRAPPER_H__


namespace kome {
	namespace clr {

		ref class SampleSetWrapper;
		ref class XYDataWrapper;
		ref class DataGroupNodeWrapper;
		ref class PeaksWrapper;
		ref class SpectrumWrapper;
		ref class DataSetWrapper;
		
		/**
		 * @class SampleWrapper
		 * @brief Sample object wrapper class to use on CLR environment
		 */
		public ref class SampleWrapper {
		public:
			/**
			 * @fn SampleWrapper( kome::objects::Sample& sample )
			 * @brief constructor
			 * @param[in] sample Sample object
			 */
			SampleWrapper( kome::objects::Sample& sample );

			/**
			 * @fn virtual ~SampleWrapper()
			 * @brief destructor
			 */
			virtual ~SampleWrapper();

		protected:
			/** Sample object */
			kome::objects::Sample& m_sample;

		public:
			/**
			 * @fn kome::objects::Sample& getSample()
			 * @brief gets sample object to be wrapped
			 * @return sample object
			 */
			kome::objects::Sample& getSample();

		public:
			/**
			 * @fn SampleSetWrapper^ getSampleSet()
			 * @brief gets sample set object
			 * @return sample set
			 */
			SampleSetWrapper^ getSampleSet();

			/**
			 * @fn void setSampleIndex( int index )
			 * @brief sets sample index
			 * @param[in] index sample index
			 */
			void setSampleIndex( int index );

			/**
			 * @fn int getSampleIndex()
			 * @brief gets sample index
			 * @return sample index
			 */
			int getSampleIndex();

			/**
			 * @fn void setInstrument( System::String^ instrument )
			 * @brief sets instrument name
			 * @param[in] instrument instrument name
			 */
			void setInstrument( System::String^ instrument );

			/**
			 * @fn System::String^ getInstrument()
			 * @brief gets instrument name
			 * @return instrument name
			 */
			System::String^ getInstrument();

			/**
			 * @fn void setMsCompany( System::String^ company )
			 * @brief sets MS company
			 * @param[in] company MS company
			 */
			void setMsCompany( System::String^ company );

			/**
			 * @fn System::String^ getMsCompany()
			 * @brief gets MS company
			 * @return MS company
			 */
			System::String^ getMsCompany();

			/**
			 * @fn void setName( System::String^ name )
			 * @brief sets sample name
			 * @param[in] name sample name
			 */
			void setName( System::String^ namee );

			/**
			 * @fn System::String^ getName()
			 * @brief gets sample name
			 * @return sample name
			 */
			System::String^ getName();

			/**
			 * @fn void setType( System::String^ type )
			 * @brief sets sample type
			 * @return sample type
			 */
			void setType( System::String^ type );

			/**
			 * @fn System::String^ getType()
			 * @brief gets sample type
			 * @return sample type
			 */
			System::String^ getType();

			/**
			 * @fn void setSoftwareName( System::String^ name )
			 * @brief sets software name
			 * @param[in] name software name
			 */
			void setSoftwareName( System::String^ name );

			/**
			 * @fn System::String^ getSoftwareName()
			 * @brief gets software name
			 * @return software name
			 */
			System::String^ getSoftwareName();

			/**
			 * @fn void setSoftwareVersion( System::String^ version )
			 * @brief sets software version
			 * @param[in] version software version
			 */
			void setSoftwareVersion( System::String^ version );

			/**
			 * @fn System::String^ getSoftwareVersion()
			 * @brief gets software version
			 * @return software version
			 */
			System::String^ getSoftwareVersion();

			/**
			 * @fn void setOpened( bool opened )
			 * @brief sets opened flag
			 * @param[in] opened opened flag value
			 */
			void setOpened( bool opened );

			/**
			 * @fn bool isOpened()
			 * @brief judges whther this sample is opened or not
			 * @return If true, this sample is opened.
			 */
			bool isOpened();

			/**
			 * @fn DataGroupNodeWrapper^ getRootDataGroupNode()
			 * @brief gets root spectrum group
			 * @return root spectrum group
			 */
			DataGroupNodeWrapper^ getRootDataGroupNode();

			/**
			 * @fn DataSetWrapper^ getHiddenDataSet()
			 * @brief gets the hidden data set
			 * @return hidden data set
			 */
			DataSetWrapper^ getHiddenDataSet();

			/**
			 * @fn unsigned int getNumberOfGroups()
			 * @brief gets the number of spectrum groups
			 * @return the number of spectrum groups
			 */
			unsigned int getNumberOfGroups();

			/**
			 * @fn DataGroupNodeWrapper^ getGroup( unsigned int index )
			 * @brief gets spectrum group
			 * @param[in] index spectrum group index
			 * @return spectrum group (If NULL, the index is illegal.)
			 */
			DataGroupNodeWrapper^ getGroup( unsigned int index );

			/**
			 * @fn void setParallelReadable( bool readable )
			 * @brief sets parallel readable or not
			 * @param[in] readable If true, this sample is parallel readabale.
			 */
			void setParallelReadable( bool readable );

			/**
			 * @fn bool isParallelReadable()
			 * @brief gets whether this sample is parallel readable or not
			 * @return If true, this sample is parallel readable.
			 */
			bool isParallelReadable();

		public:
			/**
			 * @fn bool openSample()
			 * @brief opens sample
			 * @return If true, it succeeded to open this sample.
			 */
			bool openSample();

			/**
			 * @fn bool closeSample()
			 * @brief closes sample
			 * @return If true, it succeeded to close this sample.
			 */
			bool closeSample();

		public:
			/**
			 * @fn bool isSupportedAPIPeaks()
			 * @brief judges whether API peak detection is supported on this sample.
			 * @return If true, API peak detection is supported.
			 */
			bool isSupportedAPIPeaks();

			/**
			 * @fn void detectPeaksByAPI( SpectrumWrapper^ spec, PeaksWrapper^ peaks )
			 * @brief detect peaks using Application Programming Interface
			 * @param[in] spec spectrum to detect peaks
			 * @param[out] peaks the object to store peaks information
			 */
			void detectPeaksByAPI( SpectrumWrapper^ spec, PeaksWrapper^ peaks );

		public:
			/**
			 * @fn void setSampleId( int id )
			 * @brief set sample id
			 * @param[in] id sample id
			 */
			void setSampleId( int id );
			
			/**
			 * @fn int getSampleId()
			 * @brief get sample id
			 * @return sample id
			 */
			int getSampleId();


		public:
			/**
			 * @fn static bool isCommonProperty( System::String^ key )
			 * @brief check whther the specified property key is common property or not
			 * @param[in] key parameter key
			 * @return If true, specified key is common property key
			 */
			static bool isCommonProperty( System::String^ key );
		};
	}
}

#endif	// __KOME_CLR_SAMPLE_WRAPPER_H__
