/**
 * @file RangeList.h
 * @brief interfarces of RangeList class
 *
 * @author S.Tanaka
 * @date 2009.01.05
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_RANGE_LIST_H__
#define __KOME_CORE_RANGE_LIST_H__


#include <vector>
#include <utility>


namespace kome {
	namespace core {

		/**
		 * @class RangeList
		 * @brief range list mangement class
		 */
		class CORE_TOOLKITS_CLASS RangeList {
		public:
			/**
			 * @typedef DefaultType
			 * @brief default type
			 */
			typedef enum {
				DEFAULT_NONE,
				DEFAULT_ALL
			} DefaultType;

		public:
			/**
			 * @fn RangeList( const DefaultType type = DEFAULT_NONE )
			 * @brief constructor
			 */
			RangeList( const DefaultType type = DEFAULT_NONE );

			/**
			 * @fn virtual ~RangeList()
			 * @brief destructor
			 */
			virtual ~RangeList();

		protected:
			/** default type */
			DefaultType m_type;

		protected:
			/**
			 * @union NumberValue
			 * @brief number value
			 */
			union NumberValue {
				int intVal;
				double dblVal;
			};

			/** ranges */
			std::vector< std::pair< NumberValue, NumberValue > > m_ranges;

		public:
			/**
			 * @fn void addIntRange( const int start, const int end )
			 * @brief adds integer range
			 * @param[in] start range start
			 * @param[in] end range end
			 */
			void addIntRange( const int start, const int end );

			/**
			 * @fn void addDoubleRange( const double start, const double end )
			 * @brief adds double range
			 * @param[in] start range start
			 * @param[in] end range end
			 */
			void addDoubleRange( const double start, const double end );

			/**
			 * @fn void clearRanges()
			 * @brief clears range
			 */
			void clearRanges();

			/**
			 * @fn unsigned int getNumberOfRanges()
			 * @brief gets the number of ranges
			 * @return the number of ranges
			 */
			unsigned int getNumberOfRanges();

			/**
			 * @fn int getStartInt( const unsigned int idx )
			 * @brief gets range start value
			 * @param[in] idx range index
			 * @return range start value
			 */
			int getStartInt( const unsigned int idx );

			/**
			 * @fn int getEndInt( const unsigned int idx )
			 * @brief gets range end value
			 * @param[in] idx range index
			 * @return range end value
			 */
			int getEndInt( const unsigned int idx );

			/**
			 * @fn double getStartDouble( const unsigned int idx )
			 * @brief gets range start value
			 * @param[in] idx range index
			 * @return range start value
			 */
			double getStartDouble( const unsigned int idx );

			/**
			 * @fn double getEndDouble( const unsigned int idx )
			 * @brief gets range end value
			 * @param[in] idx range index
			 * @return range end value
			 */
			double getEndDouble( const unsigned int idx );

		public:
			/**
			 * @fn bool checkIntValue( const int val )
			 * @brief checks integer vlaue
			 * @param[in] val integer value
			 * @return If true, the specified vaue is included in ranges list
			 */
			bool checkIntValue( const int val );

			/**
			 * @fn bool checkDoubleValue( const double val )
			 * @brief checks double value
			 * @param[in] val double value
			 * @return If true, the specified value is included in ranges list
			 */
			bool checkDoubleValue( const double val );

		public:
			/**
			 * @fn void importIntRanges( const char* str )
			 * @brief import integer ranges from character string
			 * @param[in] str integer ranges character string
			 */
			void importIntRanges( const char* str );

			/** 
			 * @fn void importDoubleRanges( const char* str )
			 * @brief import double ranges from character string
			 * @param[in] str double ranges character string
			 */
			void importDoubleRanges( const char* str );

			/**
			 * @fn std::string exportIntRanges()
			 * @brief exports integer ranges to character string
			 * @return integer ranges character string
			 */
			std::string exportIntRanges();

			/**
			 * @fn std::string exportDoubleRanges()
			 * @brief exports double ranges to character string
			 * @return double ranges character string
			 */
			std::string exportDoubleRanges();

		public:
			/**
			 * @fn DefaultType getType()
			 * @brief gets default type
			 * @return default type
			 */
			DefaultType getType();
		};
	}
}


#endif	// __KOME_CORE_RANGE_LIST_H__
