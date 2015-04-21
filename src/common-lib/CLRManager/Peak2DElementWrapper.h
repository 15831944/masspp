/**
 * @file Peak2DElementWrapper.h
 * @brief interfaces of Peak2DElementWrapper class
 *
 * @author S.Tanaka
 * @date 2007.10.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_PEAK_2D_ELEMENT_WRAPPER_H__
#define __KOME_CLR_PEAK_2D_ELEMENT_WRAPPER_H__


namespace kome {
	namespace clr {

		ref class PeaksCluster2DWrapper;
		ref class XYDataWrapper;
		ref class PeakElementWrapper;

		/**
		 * @class Peak2DElementWrapper
		 * @brief Peak2DElement wrapper class to use on CLR environment
		 */
		public ref class Peak2DElementWrapper {
		public:
			/**
			 * @fn Peak2DElementWrapper( kome::objects::Peak2DElement& peak )
			 * @brief constructor
			 * @param[in] peak peak object to be wrapped
			 */
			Peak2DElementWrapper( kome::objects::Peak2DElement& peak );

			/**
			 * @fn virtual ~Peak2DElementWrapper()
			 * @brief destructor
			 */
			virtual ~Peak2DElementWrapper();

		protected:
			/** peak element */
			kome::objects::Peak2DElement& m_peak;

		public:
			/**
			 * @fn kome::objects::Peak2DElement& getPeakElement()
			 * @brief gets peak element object to be wrapped
			 * @return peak element object to be wrapped
			 */
			kome::objects::Peak2DElement& getPeakElement();

		public:
			/**
			 * @fn void setRt( double rt, double startRt, double endRt );
			 * @brief sets RT
			 * @param[in] rt RT
			 * @param[in] startRt start RT
			 * @param[in] endRt end RT
			 */
			void setRt( double rt, double startRt, double endRt );

			/**
			 * @fn double getRt()
			 * @brief gets RT
			 * @return RT
			 */
			double getRt();

			/**
			 * @fn double getStartRt()
			 * @brief gets start RT
			 * @return start RT
			 */
			double getStartRt();

			/**
			 * @fn double getEndRt()
			 * @brief gets end RT
			 * @return end RT
			 */
			double getEndRt();

			/**
			 * @fn void setMz( double mz, double startMz, double endMz );
			 * @brief sets m/z
			 * @param[in] mz m/z
			 * @param[in] startMz start m/z
			 * @param[in] endMz endMz
			 */
			void setMz( double mz, double startMz, double endMz );

			/**
			 * @fn double getMz()
			 * @brief gets m/z
			 * @return m/z
			 */
			double getMz();

			/**
			 * @fn double getStartMz()
			 * @brief gets start m/z
			 * @return start m/z
			 */
			double getStartMz();

			/**
			 * @fn double getEndMz()
			 * @brief gets end m/z
			 * @return end m/z
			 */
			double getEndMz();

			/**
			 * @fn void setIntensity( double intensity )
			 * @brief sets intensity
			 * @param[in] intensity intensity
			 */
			void setIntensity( double intensity );

			/**
			 * @fn double getIntensity()
			 * @brief gets intensity 
			 * @return intensity
			 */
			double getIntensity();

			/**
			 * @fn void setCluster( PeaksCluster2DWrapper^ cluster )
			 * @brief sets peaks cluster
			 * @param[in] cluster peaks cluster
			 */
			void setCluster( PeaksCluster2DWrapper^ cluster );

			/**
			 * @fn PeaksCluster2DWrapper^ getCluster()
			 * @brief gets cluster
			 * @return cluster
			 */
			PeaksCluster2DWrapper^ getCluster();

			/**
			 * @fn void setPrecursorMass( double precursor )
			 * @brief sets the precursor mass
			 * @param[in] precursor precursor mass
			 */
			 void setPrecursorMass( double precursor );

			 /**
			  * @fn double getPrecursorMass()
			  * @brief gets the precursor mass
			  * @return precursor mass
			  */
			 double getPrecursorMass();

			 /**
			  * @fn void setCharge( int charge )
			  * @brief sets the charge state
			  * @param[in] charge charge state
			  */
			 void setCharge( int charge );

			 /**
			  * @fn int getCharge()
			  * @brief gets the charge state
			  * @return charge state
			  */
			 int getCharge();

			 /**
			  * @fn void setFwhm( double fwhm )
			  * @brief sets FWHM
			  * @param[in] fwhm
			  */
			 void setFwhm( double fwhm );

			 /**
			  * @fn double getFwhm()
			  * @brief gets FWHM
			  * @return FWHM
			  */
			 double getFwhm();

			 /**
			  * @fn void setArea( double area )
			  * @brief sets area
			  * @param[in] area
			  */
			 void setArea( double area );

			 /**
			  * @fn double getArea()
			  * @brief gets area
			  * @return area
			  */
			 double getArea();

		public:
			/**
			 * @fn void clearData()
			 * @brief clears data
			 */
			void clearData();

			/**
			 * @fn unsigned int getDataListSize()
			 * @brief gets the data list size
			 * @return data list size
			 */
			unsigned int getDataListSize();

			/**
			 * @fn void addData( int sampleId, int profileId, double peakVal, PeakElementWrapper^ peak )
			 * @brief adds data
			 * @param[in] sampleId sample ID
			 * @param[in] profileId profile ID
			 * @param[in] peakVal peak value
			 * @param[in] peak peak peak information
			 */
			void addData( int sampleId, int profileId, double peakVal, PeakElementWrapper^ peak );

			/**
			 * @fn void setData( int sampleId, int profileId, double peakVal, PeakElementWrapper^ peak )
			 * @brief sets data
			 * @param[in] sampleId sample ID
			 * @param[in] profileId profile ID
			 * @param[in] peakVal peak value
			 * @param[in] peak peak peak information
			 */
			void setData( int sampleId, int profileId, double peakVal, PeakElementWrapper^ peak );

			/**
			 * @fn PeakElementWrapper^ getDataPeak( unsigned int idx )
			 * @brief gets data peak
			 * @param[in] idx data index
			 * @return data peak
			 */
			PeakElementWrapper^ getDataPeak( unsigned int idx );

			/**
			 * @fn int getDataProfileId( unsigned int idx )
			 * @brief gets data profile ID
			 * @param[in] idx data index
			 * @return profile ID
			 */
			int getDataProfileId( unsigned int idx );

			/**
			 * @fn int getDataSampleId( unsigned int idx )
			 * @brief gets data sample ID
			 * @param[in] idx data index
			 * @return sample ID
			 */
			int getDataSampleId( unsigned int idx );

			/**
			 * @fn double getDataPeakValue( unsigned int idx )
			 * @brief gets the data peak value
			 * @param[in] idx data index
			 * @return peak value
			 */
			double getDataPeakValue( unsigned int idx );

		public:
			/**
			 * @fn void clearParameters()
			 * @brief clears parameters
			 */
			void clearParameters();

			/**
			 * @fn void setParameter( System::String^ name, System::String^ value, int type )
			 * @brief sets parameters
			 * @param[in] name parameter name
			 * @param[in] value parameter value
			 * @param[in] type parameter type
			 */
			void setParameter( System::String^ name, System::String^ value, int type );

			/**
			 * @fn unsigned int getNumberOfParameters()
			 * @brief gets the number of parameters
			 * @return the number of paramters
			 */
			unsigned int getNumberOfParameters();

			/**
			 * @fn System::String^ getParameterName( int idx )
			 * @brief gets parameter name
			 * @param[in] idx parameter index (If NULL, the index is illegal.)
			 */
			System::String^ getParameterName( int idx );

			/**
			 * @fn System::String^ getParameterValue( System::String^ name )
			 * @brief gets parameter value
			 * @param[in] name parameter name
			 * @return parameter value
			 */
			System::String^ getParameterValue( System::String^ name );

			/**
			 * @fn System::String^ getParameterValue( int idx )
			 * @brief gets parameter value
			 * @param[in] idx parameter index
			 * @return parameter value
			 */
			System::String^ getParameterValue( int idx );

			/**
			 * @fn int getParameterType( System::String^ name )
			 * @brief gets parameter type
			 * @param[in] name parameter name
			 * @return parameter type
			 */
			int getParameterType( System::String^ name );

			/**
			 * @fn int getParameterType( int idx )
			 * @brief gets parameter type
			 * @param[in] idx parameter index
			 * @return parameter type
			 */
			int getParameterType( int idx );
		};
	}
}


#endif		// __KOME_CLR_PEAK_2D_ELEMENT_WRAPPER_H__
