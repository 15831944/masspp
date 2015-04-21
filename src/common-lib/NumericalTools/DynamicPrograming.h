/**
 * @file DynamicPrograming.h
 * @brief interfaces of DynamicPrograming class
 *
 * @author S.Tanaka
 * @date 2007.08.24
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NUMERIC_DYNAMIC_PROGRAMING_H__
#define __KOME_NUMERIC_DYNAMIC_PROGRAMING_H__


#include <boost/function.hpp>


namespace kome {
	namespace numeric {

		/**
		 * @class DynamicPrograming
		 * @brief Dynamic Programing algorithm class
		 */
		class NUMERICAL_TOOLS_CLASS DynamicPrograming {
		public:
			/**
			 * @fn DynamicPrograming( const int row, const int col )
			 * @brief constructor
			 * @param[in] row row size
			 * @param[in] col col size
			 */
			DynamicPrograming( const int row, const int col );

			/**
			 * @fn virtual ~DynamicPrograming()
			 * @brief destructor
			 */
			virtual ~DynamicPrograming();

		protected:
			/** row size */
			int m_row;

			/** col size */
			int m_col;

			/**
			 * @typedef TraceBackDir
			 * @brief traceback direction
			 */
			typedef enum {
				DIR_NONE,
				DIR_UP,
				DIR_LEFT,
				DIR_UPLEFT
			} TraceBackDir;

			/**
			 * @struct ScoreInfo
			 * @brief score information
			 */
			struct ScoreInfo{
				double acScore;
				double elScore;
				bool valid;
				TraceBackDir dir;
			};

			/** calculate array */
			ScoreInfo* m_scoreArray;

		protected:
			/** the function to get row title */
			boost::function< std::string ( int ) > m_rowTitleFun;

			/** the function to get column title */
			boost::function< std::string ( int ) > m_colTitleFun;

		public:
			/**
			 * @fn double getAccumulatedScore( const int row, const int col )
			 * @brief gets accumulated score
			 * @param[in] row row number (-1 <= row <= [row size - 1])
			 * @param[in] col col number (-1 <= col <= [col size - 1])
			 * @return score
			 */
			double getAccumulatedScore( const int row, const int col );

			/**
			 * @fn double getElementScore( const int row, const int col )
			 * @brief gets element score
			 * @param[in] row row number (0 <= row <= [row size - 1])
			 * @param[in] col col number (0 <= col <= [col size - 1])
			 * @return score
			 */
			double getElementScore( const int row, const int col );

			/**
			 * @fn bool isValid( const int row, const int col )
			 * @brief judges whether valid score or not
			 * @param[in] row row number (-1 <= row <= [row size - 1])
			 * @param[in] col col number (-1 <= col <= [col size - 1])
			 * @return If true, specified element is valid
			 */
			bool isValid( const int row, const int col );

			/**
			 * @fn bool calculate(
					boost::function< double( int, int ) > scoreFun,
					const double gapPenalty,
					const double invalidScore = -1000.0,
					kome::core::Progress* progress = NULL
				)
			 * @brief calculates
			 * @param[in] scoreFun the function to get score
			 * @param[in] gapPenalty gap penalty
			 * @param[in] invalidScore invalid score
			 * @param[out] progress progress object to display progress
			 * @return If true, it succeeded to calculate.
			 */
			bool calculate(
				boost::function< double( int, int ) > scoreFun,
				const double gapPenalty,
				const double invalidScore = -1000.0,
				kome::core::Progress* progress = NULL
			);

			/**
			 * @fn void traceback( boost::function< void ( int, int ) > pushFun )
			 * @brief traceback result
			 * @param[in] pushFun the function to store result
			 */
			void traceback( boost::function< void ( int, int ) > pushFun );

		public:
			/**
			 * @fn void setRowTitleFunction( boost::function< std::string ( int ) > rowTitleFun )
			 * @brief sets the function to get row title
			 * @param[in] rowTitleFun the function to get row title
			 */
			void setRowTitleFunction( boost::function< std::string ( int ) > rowTitleFun );

			/**
			 * @fn void setColTitleFunction( boost::function< std::string ( int ) > colTitleFun )
			 * @brief sets the function to get column title
			 * @param[in] colTitleFun the function to get column title
			 */
			void setColTitleFunction( boost::function< std::string ( int ) > colTitleFun );

		protected:
			/**
			 * @fn int getIndex( const int row, const int col )
			 * @brief gets index of the array
			 * @param[in] row row number
			 * @param[in] col col number
			 * @return index of the array
			 */
			int getIndex( const int row, const int col );

			/**
			 * @fn void initArray()
			 * @brief initializes array
			 */
			void initArray();

			/**
			 * @fn void writeLog()
			 * @brief writes log
			 */
			void writeLog();

		protected:
			/**
			 * @fn static std::string getDefaultRowTitle( const int row )
			 * @brief gets default row title
			 * @param[in] row row number
			 * @return default row title
			 */
			static std::string getDefaultRowTitle( const int row );

			/**
			 * @fn static std::string getDefaultColTitle( const int col )
			 * @brief gets default column title
			 * @return default column title
			 */
			static std::string getDefaultColTitle( const int col );
		};
	}
}

#endif		// __KOME_NUMERIC_DYNAMIC_PROGRAMING_H__
