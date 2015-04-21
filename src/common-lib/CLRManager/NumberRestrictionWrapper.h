/**
 * @file NumberRestrictionWrapper.h
 * @brief interfaces of NumberRestrictionWrapper class
 *
 * @author S.Tanaka
 * @date 2007.02.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CLR_NUMBER_RESTRICTION_WRAPPER_H__
#define __KOME_CLR_NUMBER_RESTRICTION_WRAPPER_H__

namespace kome {
	namespace clr {
		/**
		 * @class NumberRestrictionWrapper
		 * @brief the restriction of number value information wrapper class to use on CLR environment
		 */
		public ref class NumberRestrictionWrapper {
		public:
			/**
			 * @fn NumberRestrictionWrapper( kome::core::NumberRestriction& restriction )
			 * @brief constructor
			 * @param[in] restriction restriction object to be wrapped
			 */
			NumberRestrictionWrapper( kome::core::NumberRestriction& restriction );

			/**
			 * @fn virtual ~NumberRestrictionWrapper()
			 * @brief destructor
			 */
			virtual ~NumberRestrictionWrapper();

		protected:
			/**		restriction object to be wrapped */
			kome::core::NumberRestriction& m_restriction;

		public:
			/**
			 * @fn kome::core::NumberRestriction& getNumberRestriction()
			 * @brief gets number restriction object
			 * @return number restriction object
			 */
			kome::core::NumberRestriction& getNumberRestriction();

		public:
			/**
			 * @fn void reset()
			 * @brief resets restriction
			 */
			void reset();

			/**
			 * @fn void setInclusiveMinValue( double value )
			 * @brief sets inclusive min value
			 * @param[in] value inclusive min value
			 */
			void setInclusiveMinValue( double value );

			/**
			 * @fn bool hasInclusiveMinValue()
			 * @brief judges whether this argument inclusive min value.
			 * @return If this argument has inclusive min value, this method returns true.
			 */
			bool hasInclusiveMinValue();

			/**
			 * @fn double getInclusiveMinValue( double dfValue )
			 * @brief gets min inclusive value 
			 * @param[in] dfValue default value
			 * @return If this argument has inclusive min value, this method returns it, otherwise returns default value.
			 */
			double getInclusiveMinValue( double dfValue );

			/**
			 * @fn void setInclusiveMaxValue( double value )
			 * @brief sets inclusive max value
			 * @param[in] value inclusive max value
			 */
			void setInclusiveMaxValue( double value );

			/**
			 * @fn bool hasInclusiveMaxValue()
			 * @brief judges whether this argument inclusive max value.
			 * @return If this argument has inclusive max value, this method returns true.
			 */
			bool hasInclusiveMaxValue();

			/**
			 * @fn double getInclusiveMaxValue( double dfValue )
			 * @brief gets max inclusive value 
			 * @param[in] dfValue default value
			 * @return If this argument has inclusive max value, this method returns it, otherwise returns default value.
			 */
			double getInclusiveMaxValue( double dfValue );

				/**
			 * @fn void setExclusiveMinValue( double value )
			 * @brief sets exclusive min value
			 * @param[in] value exclusive min value
			 */
			void setExclusiveMinValue( double value );

			/**
			 * @fn bool hasExclusiveMinValue()
			 * @brief judges whether this argument exclusive min value.
			 * @return If this argument has exclusive min value, this method returns true.
			 */
			bool hasExclusiveMinValue();

			/**
			 * @fn double getExclusiveMinValue( double dfValue )
			 * @brief gets min exclusive value 
			 * @param[in] dfValue default value
			 * @return If this argument has exclusive min value, this method returns it, otherwise returns default value.
			 */
			double getExclusiveMinValue( double dfValue );

			/**
			 * @fn void setExclusiveMaxValue( double value )
			 * @brief sets exclusive max value
			 * @param[in] value exclusive max value
			 */
			void setExclusiveMaxValue( double value );

			/**
			 * @fn bool hasExclusiveMaxValue()
			 * @brief judges whether this argument exclusive max value.
			 * @return If this argument has exclusive max value, this method returns true.
			 */
			bool hasExclusiveMaxValue();

			/**
			 * @fn double getExclusiveMaxValue( double dfValue )
			 * @brief gets max exclusive value 
			 * @param[in] dfValue default value
			 * @return If this argument has exclusive max value, this method returns it, otherwise returns default value.
			 */
			double getExclusiveMaxValue( double dfValue );

			/**
			 * @fn double getLowerBound( const double dfValue )
			 * @brief gets lower bound value
			 * @param[in] dfValue default value
			 * @return If this argument has lower bound value, this method returns it, otherwise returns default value.
			 */
			double getLowerBound( const double dfValue );

			/**
			 * @fn double getUpperBound( double dfValue )
			 * @brief gets upper bound value
			 * @param[in] dfValue default value
			 * @return If this argument has upper bound value, this method returns it, otherwise returns default value.
			 */
			double getUpperBound( double dfValue );

			/**
			 * @fn bool checkDoubleValue( double value )
			 * @brief checks real number value is in the specified range
			 * @param[in] value value to be checked
			 * @return If value is in the specified range, this method returns true.
			 */
			bool checkDoubleValue( double value );

			/**
			 * @fn bool checkIntValue( int value )
			 * @brief checks integer value is in the specified range
			 * @param[in] value value to be checked
			 * @return If value is in the specified range, this method returns true.
			 */
			bool checkIntValue( int value );

			/**
			 * @fn System::String^ getRangeStr( System::String^ name )
			 * @brief gets character string that means value range
			 * @param[in] name number value name
			 * @return character string that means value range
			 */
			System::String^ getRangeStr( System::String^ name );
		};
	}
}


#endif		// __KOME_CLR_NUMBER_RESTRICTION_WRAPPER_H__
