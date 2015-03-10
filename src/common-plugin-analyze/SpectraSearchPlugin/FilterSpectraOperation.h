/**
 * @file FilterSpectraOperation.h
 * @brief Filter Spectra Operation class
 *
 * @author M.Izumi
 * @date 2012.01.30
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_SEARCH_FILTER_SPECTRA_OPERATION_H__
#define __KOME_SEARCH_FILTER_SPECTRA_OPERATION_H__


#include "SpectraSearchFilter.h"
#include "SpectraSearchManager.h"

namespace kome {
	namespace search {

		/**
		 * @class FilterSpectraOperation
		 * @brief fileter spectra operation
		 */
		class FilterSpectraOperation : public kome::operation::Operation{
		public:
			/**
			 * @fn FilterSpectraOperation()
			 * @brief constructor
			 */
			FilterSpectraOperation();

			/**
			 * @fn virtual ~FilterSpectraOperation()
			 * @brief destructor
			 */
			virtual ~FilterSpectraOperation();
		
		protected:
			/** old previous data file path */
			std::string m_oldPrevPath;
			
			/** search type */
			SpectraSearchManager::SearchType m_type;
			
			/** filter */
			SpectraSearchFilter m_filter;

			/** operation descript */
			std::string m_strOptDescript;
			
			/** string range */
			std::string m_strRange;

		public:			
			void setType( SpectraSearchManager::SearchType type );

			SpectraSearchManager::SearchType getType();
			
			void setFilter( SpectraSearchFilter filter );
			SpectraSearchFilter getFilter();

			void setDescript( const char* strDescript );
			const char* getDescript();

			void setRange( const char* strRange );
			const char* getRange();

		protected:
			/**
			* @fn virtual bool onExecute()
			* @brief execute operation
			* @return If true, it succeeded to perform search.
			*/
			virtual bool onExecute();

			/**
			* @fn virtual void onLoadCondition( boost::function< int ( void*, int ) > readFun )
			* @brief load the saved state with respect to to search engine operations
			* @param[in] readFun function to read saved state
			*/
			virtual void onLoadCondition( boost::function< int ( void*, int ) > readFun );

			/**
			* @fn virtual void onSaveCondition( boost::function< int ( void*, int ) > writeFun )
			* @brief save the current state with respect to search engine operations
			* @param[in] writeFun function to save state
			*/
			virtual void onSaveCondition( boost::function< int ( void*, int ) > writeFun  );

			/**
			 * @fn virtual const char* onGetDescription()
			 * @brief get discription (override method)
			 * @return discription
			 */
			virtual std::string onGetDescription();
			
			/**
			 * @fn virtual const char* onGetParametersString()
			 * @brief get parameters string (override method)
			 * @return parameters string 
			 */
			virtual std::string onGetParametersString();
			
			/**
			 * @fn virtual void onSetParametersString( const char* strParam )
			 * @brief set parameters string  (override method)
			 * @param[in] strParam parameters string
			 */
			virtual void onSetParametersString( const char* strParam );
			
		};
	}
}

#endif	// __KOME_SEARCH_FILTER_SPECTRA_OPERATION_H__
