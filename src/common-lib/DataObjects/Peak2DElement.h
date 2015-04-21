/**
 * @file Peak2DElement.h
 * @brief interfaces of Peak2DElement class
 *
 * @author S.Tanaka
 * @date 2007.10.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_PEAK_2D_ELEMENT_H__
#define __KOME_OBJECTS_PEAK_2D_ELEMENT_H__


#include <vector>
#include <boost/function.hpp>

#include "PeakElement.h"


namespace kome {
	namespace objects {

		class Peaks2D;
		class PeaksCluster2D;
		class PeakElement;

		/**
		 * @class Peak2DElement
		 * @brief 2-dimentional peak element
		 */
		class DATA_OBJECTS_CLASS Peak2DElement {
		public:
			/**
			 * @fn Peak2DElement( Peaks2D* peaks )
			 * @brief constructor
			 * @param[in] peaks peaks object
			 */
			Peak2DElement( Peaks2D* peaks );

			/**
			 * @fn virtual ~Peak2DElement()
			 * @brief destructor
			 */
			virtual ~Peak2DElement();

		protected:
			/** RT */
			double m_rt;

			/** start RT */
			double m_startRt;

			/** end RT */
			double m_endRt;

			/** m/z */
			double m_mz;

			/** start m/z */
			double m_startMz;

			/** end m/z */
			double m_endMz;

			/** intensity */
			double m_intensity;

			/** charge */
			int m_charge;

			/** fwhm */
			double m_fwhm;

			/** area */
			double m_area;

			/** peaks */
			Peaks2D* m_peaks;

			/** cluster */
			PeaksCluster2D* m_cluster;

			/** element data ID */
			int m_elmId;

			/** peak2d id */
			int m_peak2dId;

			/** precursor mass */
			double m_precursor;

		protected:
			/**
			 * @struct DataInfo
			 * @brief data information
			 */
			struct DataInfo {
				PeakElement peak;
				int profile;
				int sampleId;
				double peakVal;
			};

			/**
			 * @struct ParamInfo
			 * @brief parameter information
			 */
			struct ParamInfo {
				int type;
				std::string name;
				std::string value;
			};

			/** data */
			std::vector< DataInfo > m_dataList;

			/** parameters */
			std::vector< ParamInfo > m_params;

		public:
			/**
			 * @fn void setRt( const double rt, const double startRt, const double endRt );
			 * @brief sets RT
			 * @param[in] rt RT
			 * @param[in] startRt start RT
			 * @param[in] endRt end RT
			 */
			void setRt( const double rt, const double startRt, const double endRt );

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
			 * @fn void setMz( const double mz, const double startMz, const double endMz );
			 * @brief sets m/z
			 * @param[in] mz m/z
			 * @param[in] startMz start m/z
			 * @param[in] endMz endMz
			 */
			void setMz( const double mz, const double startMz, const double endMz );

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
			 * @fn void setIntensity( const double intensity )
			 * @brief sets intensity
			 * @param[in] intensity intensity
			 */
			void setIntensity( const double intensity );

			/**
			 * @fn double getIntensity()
			 * @brief gets intensity 
			 * @return intensity
			 */
			double getIntensity();

			/**
			 * @fn void setCluster( PeaksCluster2D* cluster )
			 * @brief sets peaks cluster
			 * @param[in] cluster peaks cluster
			 */
			void setCluster( PeaksCluster2D* cluster );

			/**
			 * @fn PeaksCluster2D* getCluster()
			 * @brief gets cluster
			 * @return cluster
			 */
			PeaksCluster2D* getCluster();
			
			/**
			 * @fn void setPrecursorMass( const double precursor )
			 * @brief sets the precursor mass
			 * @param[in] precursor precursor mass
			 */
			 void setPrecursorMass( const double precursor );

			 /**
			  * @fn double getPrecursorMass()
			  * @brief gets the precursor mass
			  * @return precursor mass
			  */
			 double getPrecursorMass();

			 /**
			  * @fn void setCharge( const int charge )
			  * @brief sets the charge state
			  * @param[in] charge charge state
			  */
			 void setCharge( const int charge );

			 /**
			  * @fn int getCharge()
			  * @brief gets the charge state
			  * @return charge state
			  */
			 int getCharge();

			 /**
			  * @fn void setFwhm( const double fwhm )
			  * @brief sets FWHM
			  * @param[in] fwhm
			  */
			 void setFwhm( const double fwhm );

			 /**
			  * @fn double getFwhm()
			  * @brief gets FWHM
			  * @return FWHM
			  */
			 double getFwhm();

			 /**
			  * @fn void setArea( const double area )
			  * @brief sets area
			  * @param[in] area
			  */
			 void setArea( const double area );

			 /**
			  * @fn double getArea()
			  * @brief gets area
			  * @return area
			  */
			 double getArea();

			/**
			 * @fn void setId( int id )
			 * @brief sets peak2d id
			 * @param[in] id peak2d id
			 */
			void setId( int id );

			/**
			 * @fn int getId()
			 * @brief gets peak2d id
			 * @return peak2d id
			 */
			int getId();

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
			 * @fn void addData( const int sampleId, const int profileId, const double peakVal, PeakElement* peak )
			 * @brief adds data
			 * @param[in] sampleId sample ID
			 * @param[in] profileId profile ID
			 * @param[in] peakVal peak value
			 * @param[in] peak peak peak information
			 */
			void addData( const int sampleId, const int profileId, const double peakVal, PeakElement* peak );

			/**
			 * @fn void setData( const int sampleId, const int profileId, const double peakVal, PeakElement* peak )
			 * @brief sets data
			 * @param[in] sampleId sample ID
			 * @param[in] profileId profile ID
			 * @param[in] peakVal peak value
			 * @param[in] peak peak peak information
			 */
			void setData( const int sampleId, const int profileId, const double peakVal, PeakElement* peak );

			/**
			 * @fn PeakElement* getDataPeak( const unsigned int idx = 0 )
			 * @brief gets data peak
			 * @param[in] idx data index
			 * @return data peak
			 */
			PeakElement* getDataPeak( const unsigned int idx = 0 );

			/**
			 * @fn int getDataProfileId( const unsigned int idx = 0 )
			 * @brief gets data profile ID
			 * @param[in] idx data index
			 * @return profile ID
			 */
			int getDataProfileId( const unsigned int idx = 0 );

			/**
			 * @fn int getDataSampleId( const unsigned int idx = 0 )
			 * @brief gets data sample ID
			 * @param[in] idx data index
			 * @return sample ID
			 */
			int getDataSampleId( const unsigned int idx = 0 );

			/**
			 * @fn double getDataPeakValue( const unsigned int idx = 0 )
			 * @brief gets the data peak value
			 * @param[in] idx data index
			 * @return peak value
			 */
			double getDataPeakValue( const unsigned int idx = 0 );

		public:
			/**
			 * @fn void clearParameters()
			 * @brief clears parameters
			 */
			void clearParameters();

			/**
			 * @fn void setParameter( const char* name, const char* value, const int type = 0 )
			 * @brief sets parameters
			 * @param[in] name parameter name
			 * @param[in] value parameter value
			 * @param[in] type type
			 */
			void setParameter( const char* name, const char* value, const int type = 0 );

			/**
			 * @fn unsigned int getNumberOfParameters()
			 * @brief gets the number of parameters
			 * @return the number of paramters
			 */
			unsigned int getNumberOfParameters();

			/**
			 * @fn const char* getParameterName( const int idx )
			 * @brief gets parameter name
			 * @param[in] idx parameter index (If NULL, the index is illegal.)
			 */
			const char* getParameterName( const int idx );

			/**
			 * @fn const char* getParameterValue( const char* name )
			 * @brief gets parameter value
			 * @param[in] name parameter name
			 * @return parameter value
			 */
			const char* getParameterValue( const char* name );

			/**
			 * @fn const char* getParameterValue( const int idx )
			 * @brief gets parameter value
			 * @param[in] idx parameter index
			 * @return parameter value
			 */
			const char* getParameterValue( const int idx );

			/**
			 * @fn int getParameterType( const char* name )
			 * @brief gets parameter type
			 * @param[in] name parameter name
			 * @return parameter type
			 */
			int getParameterType( const char* name );

			/**
			 * @fn int getParameterType( const int idx )
			 * @brief gets parameter type
			 * @param[in] idx parameter index
			 * @return parameter type
			 */
			int getParameterType( const int idx );

		protected:
			/**
			 * @fn int searchParameterIndex( const char* name )
			 * @brief searches parameter index
			 * @param[in] name parameter name
			 * @return parameter index
			 */
			int searchParameterIndex( const char* name );
		};
	}
}


#endif		// __KOME_OBJECTS_PEAK_2D_ELEMENT_H__
