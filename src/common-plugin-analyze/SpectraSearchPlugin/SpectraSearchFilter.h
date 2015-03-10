/**
 * @file SpectraSearchFilter.h
 * @brief interfaces of SpectraSearchFilter class
 *
 * @author S.Tanaka
 * @date 2007.02.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_SEARCH_SPECTRA_SEARCH_FILTER_H__
#define __KOME_SEARCH_SPECTRA_SEARCH_FILTER_H__


#include <vector>
#include <boost/optional.hpp>


namespace kome {
	namespace search {

		/**
		 * @class SpectraSearchFilter
		 * @brief spectra search filter class
		 */
		class SpectraSearchFilter {
		public:
			/**
			 * @fn SpectraSearchFilter()
			 * @brief constructor
			 */
			SpectraSearchFilter();

			/**
			 * @fn virtual ~SpectraSearchFilter()
			 * @brief destructor
			 */
			virtual ~SpectraSearchFilter();

		protected:
			/** stage range */
			kome::core::RangeList m_stageRange;

			/** scan range */
			kome::core::RangeList m_scanRange;

			/** RT range */
			kome::core::RangeList m_rtRange;

			/** TIC range */
			kome::core::RangeList m_ticRange;

			/** BPC range */
			kome::core::RangeList m_bpcRange;

			/** precursor range */
			kome::core::RangeList m_precRange;

		public:
			/**
			 * @fn void setStageRange( const char* range )
			 * @brief sets stage range
			 * @param range string to import range
			 */
			void setStageRange( const char* range );

			/**
			 * @fn void resetStageRange()
			 * @brief resets stage range
			 */
			void resetStageRange();

			/**
			 * @fn void setScanRange( const char* range )
			 * @brief sets scan range
			 * @param range string to import scan range
			 */
			void setScanRange( const char* range );

			/**
			 * @fn void resetScanRange()
			 * @brief resets scan range
			 */
			void resetScanRange();

			/**
			 * @fn void setRtRange( const char* range )
			 * @brief sets RT range
			 * @param range string to import RT range
			 */
			void setRtRange( const char* range );

			/**
			* @fn void resetRtRange()
			* @brief resets RT range
			*/
			void resetRtRange();

			/**
			 * @fn void setTicRange( const char* range )
			 * @brief sets TIC range
			 * @param range string to import TIC range
			 */
			void setTicRange( const char* range );

			/**
			 * @fn void resetTicRange()
			 * @brief resets TIC range
			 */
			void resetTicRange();

			/**
			 * @fn void setBpcRange( const char* range )
			 * @brief sets BPC range
			 * @param range string to import BPC range
			 */
			void setBpcRange( const char* range );

			/**
			* @fn void resetBpcRange()
			* @brief resets BPC range
			*/
			void resetBpcRange();

			/**
			 * @fn void setPrecursorRange( const char* range )
			 * @brief sets precursor range
			 * @param range string to import range
			 */
			void setPrecursorRange( const char* range );

			/**
			 * @fn void resetPrecursorRange()
			 * @brief resets precursor range
			 */
			void resetPrecursorRange();

		public:
			/**
			 * @typedef PropertyFilterType
			 * @brief property filter type
			 */
			typedef enum {
				PROP_FILTER_UNKNOWN,
				PROP_FILTER_IS,
				PROP_FILTER_IS_NOT,
				PROP_FILTER_CONTAINS,
				PROP_FILTER_DOES_NOT_CONTAIN,
				PROP_FILTER_IS_MORE_THAN,
				PROP_FILTER_IS_NOT_MORE_THAN,
				PROP_FILTER_IS_LESS_THAN,
				PROP_FILTER_IS_NOT_LESS_THAN
			} PropertyFilterType;

		protected:
			/**
			 * @struct PropertyFilter
			 * @brief property filter
			 */
			struct PropertyFilter {
				std::string key;
				std::string value;
				PropertyFilterType type;
			};

			/** the array of properties filters */
			std::vector< PropertyFilter > m_propFilters;

		public:
			/**
			 * @fn void addPropertyFilter(
					const char* key,
					const char* value,
					PropertyFilterType type
				)
			 * @brief adds properties filter
			 * @param key property key
			 * @param value property value
			 * @param type filter type
			 */
			void addPropertyFilter(
				const char* key,
				const char* value,
				PropertyFilterType type
			);

			/**
			 * @fn unsigned int getNumberOfPropertyFilters()
			 * @brief gets the number of properties filters
			 * @return the number of property filters
			 */
			unsigned int getNumberOfPropertyFilters();

			/**
			 * @fn const char* getPropertyFilterKey( const unsigned int index )
			 * @brief gets property filter key
			 * @param index property filter index
			 * @return property filter key (If the index is illegal, this method returns NULL.)
			 */
			const char* getPropertyFilterKey( const unsigned int index );

			/**
			 * @fn const char* getPropertyFilterValue( const unsigned int index )
			 * @brief gets property filter value
			 * @param index property filter index
			 * @return property filter value (If the index is illegal, this method returns NULL.)
			 */
			const char* getPropertyFilterValue( const unsigned int index );

			/**
			 * @fn PropertyFilterType getPropertyFilterType( const unsigned int index )
			 * @brief gets property filter type
			 * @param index property filter index
			 * @return property filter type (If the index is illegal, this method returns UNKNOWN type.)
			 */
			PropertyFilterType getPropertyFilterType( const unsigned int index );

			/**
			 * @fn void removePropertyFilter( const unsigned int index )
			 * @brief removes property filter
			 * @param index property filter index 
			 */
			void removePropertyFilter( const unsigned int index );

		public:
			/**
			 * @typedef PeakBase
			 * @brief peak base point
			 */
			typedef enum {
				BASE_UNKNOWN,
				BASE_ZERO,
				BASE_PRECURSOR
			} PeakBase;

		public:
			/**
			 * @fn void addPeakFilter(
					PeakBase base,
					double pos,
					double threshold,
					double absIntensity,
					double relIntensity
				)
			 * @brief adds peak filter
			 * @param base base point
			 * @param pos peak position
			 * @param threshold m/z threshold
			 * @param absIntensity absolute intensity threshold
			 * @param relIntensity relative intensity threshold
			 */
			void addPeakFilter(
				PeakBase base,
				double pos,
				double threshold,
				double absIntensity,
				double relIntensity
			);

			/**
			 * @fn unsigned int getNumberOfPeakFilters()
			 * @brief gets the number of peak filters
			 * @return the number of peak filters
			 */
			unsigned int getNumberOfPeakFilters();

			/**
			 * @fn PeakBase getPeakBase( const unsigned int index )
			 * @brief gets the base point of peak filter
			 * @param index peak filter index
			 * @return base point of peak filter
			 */
			PeakBase getPeakBase( const unsigned int index );

			/**
			 * @fn double getPeakPosition( const unsigned int index )
			 * @brief gets the peak position
			 * @param index peak filter index
			 * @return peak position
			 */
			double getPeakPosition( const unsigned int index );

			/**
			 * @fn double getPeakPosThreshold( const unsigned int index )
			 * @brief gets the peak position threshold
			 * @param index peak filter index
			 * @return peak position threshold
			 */
			double getPeakPosThreshold( const unsigned int index );

			/**
			 * @fn double getPeakAbsIntThreshold( const unsigned int index )
			 * @brief gets absolute peak intensity threshold
			 * @param index peak filter index 
			 * @return absokute peak intensity threshold
			 */
			double getPeakAbsIntThreshold( const unsigned int index );

			/**
			 * @fn double getPeakRelIntThreshold( const unsigned int index )
			 * @brief gets relative peak intensity threshold
			 * @param index peak filter index
			 * @return relative peak intensity threshold
			 */
			double getPeakRelIntThreshold( const unsigned int index );

			/**
			 * @fn void removePeakFilter( const unsigned int index )
			 * @brief removes peak filter
			 * @param index peak filter index
			 */
			void removePeakFilter( const unsigned int index );

		public:
			/**
			 * @fn void resetAll()
			 * @brief resets all filter value
			 */
			void resetAll();

			/**
			 * @fn void filter(
					std::vector< kome::objects::Spectrum* >& srcSpectra,
					std::vector< kome::objects::Spectrum* >& dstSpectra
				)
			 * @brief executes filter
			 * @param srcSpectra source spectra
			 * @param dstSpectra the array to store filtered spectra
			 */
			void filter(
				std::vector< kome::objects::Spectrum* >& srcSpectra,
				std::vector< kome::objects::Spectrum* >& dstSpectra
			);

		protected:
			/**
			 * @fn bool allowSpectrum( kome::objects::Spectrum* spec )
			 * @brief judges this filter allows specified spectrum
			 * @param spec spectrum
			 * @return If true, this filter allows specified spectrum.
			 */
			bool allowSpectrum( kome::objects::Spectrum* spec );

			/**
			 * @fn bool allowProperties( kome::core::Properties& properties )
			 * @brief judgets this filter allows properties
			 * @param properties properties object
			 * @return If true, this filter allows specified properties
			 */
			bool allowProperties( kome::core::Properties& properties );
		};
	}
}

#endif		// __KOME_SEARCH_SPECTRA_SEARCH_FILTER_H__
