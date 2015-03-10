/**
 * @file DataMapRangeOperation.h
 * @brief DataMap Range Operation class
 *
 * @author M.Izumi
 * @date 2013.05.14
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_RANGE_DATAMAPRANGE_OPERATION_H__
#define __KOME_RANGE_DATAMAPRANGE_OPERATION_H__

#include "DataMapRangeManager.h"

namespace kome {
	namespace operation {
		namespace range {
			
			/**
			 * @class DataMapRangeOperation
			 * @brief DataMap Range Operation class
			 */
			class DataMapRangeOperation : public kome::operation::Operation {
			public:
				/**
				 * @fn DataMapRangeOperation()
				 * @brief constructor
				 */
				DataMapRangeOperation();

				/**
				 * @fn virtual ~DataMapRangeOperation()
				 * @brief destructor
				 */
				virtual ~DataMapRangeOperation();

			protected:
				/** start m/z */
				double m_startMz;

				/** end m/z */
				double m_endMz;

				/** start rt */
				double m_startRt;
				
				/** end rt */
				double m_endRt;

				/** data map info */
				kome::objects::DataMapInfo* m_dataMapInfo;
				
				/** canvas */
				wxWindow* m_canvas;
				
				/** reset flag */
				bool m_bReset;

			public:
				/**
				 * @fn void setStartMz( double startMz )
				 * @brief set start m/z 
				 * @param startMz 
				 */
				void setStartMz( double startMz );

				/**
				 * @fn double getStartMz()
				 * @brief get start m/z
				 * @return start m/z
				 */
				double getStartMz();

				/**
				 * @fn void setEndMz( double endMz )
				 * @brief set end m/z
				 * @return end m/z
				 */
				void setEndMz( double endMz );

				/**
				 * @fn double getEndMz()
				 * @brief get end m/z
				 * @return end m/z
				 */
				double getEndMz();

				/**
				 * @fn void setStartRt( double startRt)
				 * @brief set start rt
				 * @param startRt
				 */
				void setStartRt( double startRt );

				/**
				 * @fn double getStartRt()
				 * @brief get start rt
				 * @param start rt
				 */
				double getStartRt();

				/**
				 * @fn void setEndRt( double endRt )
				 * @brief set end rt
				 * @param endRt
				 */
				void setEndRt( double endRt );

				/**
				 * @fn double getEndRt()
				 * @brief get end rt
				 * @return end rt
				 */
				double getEndRt();

				/**
				 * @fn void setCanvas( wxWindow* canvas )
				 * @brief set canvas
				 * @param canvas
				 */
				void setCanvas( wxWindow* canvas );

				/**
				 * @fn wxWindow* getCanvas()
				 * @brief get canvas
				 * @return canvas
				 */
				wxWindow* getCanvas();

				/**
				 * @fn void setDataMapInfo( kome::objects::DataMapInfo* dataMapInfo )
				 * @brief set data map info
				 * @param dataMapInfo
				 */
				void setDataMapInfo( kome::objects::DataMapInfo* dataMapInfo );

				/**
				 * @fn kome::objects::DataMapInfo* getDataMapInfo()
				 * @brief get data map info
				 * @return datamapInfo
				 */
				kome::objects::DataMapInfo* getDataMapInfo();

				/**
				 * @fn void setbReset( bool bReset )
				 * @brief set bReset
				 * @param bRest
				 */
				void setbReset( bool bReset );

				/** 
				 * @fn bool getbReset()
				 * @brief get rest flg
				 * @return rest flg
				 */
				bool getbReset();
				
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
				 * @fn virtual std::string onGetDescription()
				 * @brief get discription (override method)
				 * @return discription
				 */
				virtual std::string onGetDescription();
			
				/**
				 * @fn virtual std::string onGetParametersString()
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
}

#endif // __KOME_RANGE_DATAMAPRANGE_OPERATION_H__
