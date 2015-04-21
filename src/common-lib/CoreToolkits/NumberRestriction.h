/**
 * @file NumberRestriction.h
 * @brief interfaces of NumberRestriction class
 *
 * @author S.Tanaka
 * @date 2007.02.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_NUMBER_RESTRICTION_H__
#define __KOME_CORE_NUMBER_RESTRICTION_H__


#include <string>
#include <boost/optional.hpp>


namespace kome {
	namespace core {
		/**
		 * @class NumberRestriction
		 * @brief the restriction of number class
		 */
		class CORE_TOOLKITS_CLASS NumberRestriction {
		public:
			/**
			 * @fn NumberRestriction()
			 * @brief constructor
			 */
			NumberRestriction();

			/**
			 * @fn virtual ~NumberRestriction()
			 * @brief destructor
			 */
			virtual ~NumberRestriction();

		protected:
			/** inclusive minimum value */
			boost::optional< double > m_minInclusive;
			/** inclusive maximum value */
			boost::optional< double > m_maxInclusive;
			/** exclusive minimum value */
			boost::optional< double > m_minExclusive;
			/** exclusive maximum value */
			boost::optional< double > m_maxExclusive;

		public:
			/**
			 * @fn void reset()
			 * @brief resets restriction
			 */
			void reset();

			/**
			 * @fn void setInclusiveMinValue( const double value )
			 * @brief sets inclusive min value
			 * @param[in] value inclusive min value
			 */
			void setInclusiveMinValue( const double value );

			/**
			 * @fn bool hasInclusiveMinValue()
			 * @brief judges whether this argument inclusive min value.
			 * @return If this argument has inclusive min value, this method returns true.
			 */
			bool hasInclusiveMinValue();

			/**
			 * @fn double getInclusiveMinValue( const double dfValue = 0.0)
			 * @brief gets min inclusive value 
			 * @param[in] dfValue default value
			 * @return If this argument has inclusive min value, this method returns it, otherwise returns default value.
			 */
			double getInclusiveMinValue( const double dfValue = 0.0 );

			/**
			 * @fn void setInclusiveMaxValue( const double value )
			 * @brief sets inclusive max value
			 * @param[in] value inclusive max value
			 */
			void setInclusiveMaxValue( const double value );

			/**
			 * @fn bool hasInclusiveMaxValue()
			 * @brief judges whether this argument inclusive max value.
			 * @return If this argument has inclusive max value, this method returns true.
			 */
			bool hasInclusiveMaxValue();

			/**
			 * @fn double getInclusiveMaxValue( const double dfValue = 0.0 )
			 * @brief gets max inclusive value 
			 * @param[in] dfValue default value
			 * @return If this argument has inclusive max value, this method returns it, otherwise returns default value.
			 */
			double getInclusiveMaxValue( const double dfValue = 0.0 );

				/**
			 * @fn void setExclusiveMinValue( const double value )
			 * @brief sets exclusive min value
			 * @param[in] value exclusive min value
			 */
			void setExclusiveMinValue( const double value );

			/**
			 * @fn bool hasExclusiveMinValue()
			 * @brief judges whether this argument exclusive min value.
			 * @return If this argument has exclusive min value, this method returns true.
			 */
			bool hasExclusiveMinValue();

			/**
			 * @fn double getExclusiveMinValue( const double dfValue = 0.0)
			 * @brief gets min exclusive value 
			 * @param[in] dfValue default value
			 * @return If this argument has exclusive min value, this method returns it, otherwise returns default value.
			 */
			double getExclusiveMinValue( const double dfValue = 0.0 );

			/**
			 * @fn void setExclusiveMaxValue( const double value )
			 * @brief sets exclusive max value
			 * @param[in] value exclusive max value
			 */
			void setExclusiveMaxValue( const double value );

			/**
			 * @fn bool hasExclusiveMaxValue()
			 * @brief judges whether this argument exclusive max value.
			 * @return If this argument has exclusive max value, this method returns true.
			 */
			bool hasExclusiveMaxValue();

			/**
			 * @fn double getExclusiveMaxValue( const double dfValue = 0.0 )
			 * @brief gets max exclusive value 
			 * @param[in] dfValue default value
			 * @return If this argument has exclusive max value, this method returns it, otherwise returns default value.
			 */
			double getExclusiveMaxValue( const double dfValue = 0.0 );

			/**
			 * @fn double getLowerBound( const double dfValue = 0.0 )
			 * @brief gets lower bound value
			 * @param[in] dfValue default value
			 * @return If this argument has lower bound value, this method returns it, otherwise returns default value.
			 */
			double getLowerBound( const double dfValue = 0.0 );

			/**
			 * @fn double getUpperBound( const double dfValue = 0.0 )
			 * @brief gets upper bound value
			 * @param[in] dfValue default value
			 * @return If this argument has upper bound value, this method returns it, otherwise returns default value.
			 */
			double getUpperBound( const double dfValue = 0.0 );

			/**
			 * @fn bool checkDoubleValue( const double value )
			 * @brief checks real number value is in the specified range
			 * @param[in] value value to be checked
			 * @return If value is in the specified range, this method returns true.
			 */
			bool checkDoubleValue( const double value );

			/**
			 * @fn bool checkIntValue( const int value )
			 * @brief checks integer value is in the specified range
			 * @param[in] value value to be checked
			 * @return If value is in the specified range, this method returns true.
			 */
			bool checkIntValue( const int value );

			/**
			 * @fn std::string getRangeStr( const char* name )
			 * @brief gets character string that means value range
			 * @param[in] name number value name
			 * @return character string that means value range
			 */
			std::string getRangeStr( const char* name );
		};
	}
}

#endif		// __KOME_CORE_NUMBER_RESTRICTION_H__
