/**
 * @file PeakPositions.h
 * @brief Peak Positions class
 *
 * @author M.Izumi
 * @date 2013.12.09
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_IDENT_TOOL_PEAK_POSITONS_H__
#define __KOME_IDENT_TOOL_PEAK_POSITONS_H__

namespace kome {
	namespace ident {
		
		/**
		 * @class PeakPositions
		 * @brief peak information class
		 */
		class IDENT_TOOLS_CLASS PeakPositions {
		public:
			/**
			 * @struct PeakInfo
			 * @brief peak information
			 */
			struct PeakInfo {
				double rt;
				double mz;
				int charge;		
			};

		public:
			/**
			 * @fn PeakPositions()
			 * @brief constructor
			 */
			PeakPositions();

			/**
			 * @fn virtual ~PeakPositions()
			 * @brief destructor
			 */
			virtual ~PeakPositions();

		protected:			
			/** peak info */
			std::vector< PeakInfo > m_peakInfos;
			
			/**
			 * @struct PropertyInfo
			 * @brief property information
			 */
			struct PropertyInfo {
				int peakIndex;
				std::string name;
				std::string value;
				int type;
			};

			/** property infos */
			std::vector< PropertyInfo > m_propertys;

			/** peak value info */
			struct PeakValueInfo {
				double peakVal;
				kome::objects::PeakElement* elt;
				int profileId;
			};
			
			/** peak value info */
			std::map< std::pair< int, int >, PeakValueInfo > m_peakValueInfo;

			/** matrix sample id */
			std::vector< int > m_matrixSampleId;

		protected:
			/** profile ID map */
			static std::map< long, int > m_profileMap;				
		
		public:
			/**
			 * @fn int addPeakPosition( double rt, double mz, int charge )
			 * @brief add peak positon
			 * @param[in] rt
			 * @param[in] mz
			 * @param[in] charge
			 * @return peak index
			 */
			int addPeakPosition( double rt, double mz, int charge );
			
			/**
			 * @fn double getPeakPositonMz( int peakIndex )
			 * @brief get the peak positon m/z
			 * @param[in] peakIndex peak index
			 * @return the peak position m/z
			 */
			double getPeakPositionMz( int peakIndex );

			/**
			 * @fn double getPeakPositionRt( int peakIndex )
			 * @brief get the peak position rt
			 * @param[in] peakIndex peak index
			 * @return the peak position rt
			 */
			double getPeakPositionRt( int peakIndex );

			/**
			 * @fn int getPeakPositonCharge( int peakIndex )
			 * @brief get the peak position charge
			 * @param[in] peakIndex index
			 * @return charge
			 */
			int getPeakPositonCharge( int peakIndex );

			/**
			 * @fn void setPeakValueInfo( 
						int peakIndex, 
						int sampleId,
						double peak, 
						kome::core::XYData& xyData,
						kome::objects::PeakElement* element,
						bool chromFlg
					)
			 * @brief set the peak value info
			 * @param[in] peakIndex
			 * @param[in] sampleId
			 * @param[in] peak
			 * @param[in] xyData
			 * @param[in] element
			 * @param[in] chromFlg
			 */
			void setPeakValueInfo( 
				int peakIndex, 
				int sampleId,
				double peak, 
				kome::core::XYData& xyData,
				kome::objects::PeakElement* element,
				bool chromFlg,
				int profiledId = -1
			);
			
			/**
			 * @fn double getPeakValue( int peakIndex, int sampleId )
			 * @brief get the peak value
			 * @param[in] peakIndex
			 * @param[in] sampleId
			 * @return peak value
			 */
			double getPeakValue( int peakIndex, int sampleId );
			
			/**
			 * @fn int getNumberOfPeaks()
			 * @brief get the number of peaks
			 * @return number of peaks
			 */
			int getNumberOfPeaks();

			/**
			 * @fn int getProfile( int peakIndex, int sampleId )
			 * @brief get the profile
			 * @param[in] peakIndex
			 * @param[in] sampleId
			 * @return profile
			 */
			int getProfile( int peakIndex, int sampleId );
			
			/**
			 * @fn kome::objects::PeakElement* getPeak( int peakIndex, int sampleId )
			 * @brief get the peak
			 * @param[in] peakIndex
			 * @param[in] sampleId
			 * @return peak
			 */
			kome::objects::PeakElement* getPeak( int peakIndex, int sampleId );
			
			/**
			 * @fn int getNumberOfMatrixSamples()
			 * @brief get the number of matrix samples
			 * @return number of matrix samples
			 */
			int getNumberOfMatrixSamples();
			
			/**
			 * @fn int getMatrixSampleId( int sampleIndex )
			 * @brief get the matrix sample id
			 * @param[in] sampleIndex
			 * @return sample id
			 */
			int getMatrixSampleId( int sampleIndex );

			/**
			 * @fn void setPeakProperty( int peakIndex, const char* propName, const char* prop, int type )
			 * @brief set the peak property
			 * @param[in] peakIndex
			 * @param[in] propName
			 * @param[in] prop
			 * @param[in] type
			 */
			void setPeakProperty( int peakIndex, const char* propName, const char* prop, int type );

			/**
			 * @fn int getNumberOfProperties()
			 * @brief get the number of properties
			 * @return number of properties
			 */
			int getNumberOfProperties();

			/**
			 * @fn const char* getPropertyName( int index )
			 * @brief get the property name
			 * @param[in] index
			 * @return property name
			 */
			const char* getPropertyName( int index );

			/**
			 * @fn const char* getPropertyValue( int peakIndex, const char* propName )
			 * @brief get the property value
			 * @param[in] paeakIndex
			 * @param[in] propName
			 * @return property value
			 */
			const char* getPropertyValue( int peakIndex, const char* propName );

			/**
			 * @fn int getPropertyType( int peakIndex )
			 * @brief get the property type
			 * @param[in] peakIndex
			 * @return property type
			 */
			int getPropertyType( int peakIndex );

		};
	}
}

#endif // __KOME_IDENT_TOOL_PEAK_POSITONS_H__
