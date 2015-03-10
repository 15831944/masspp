/**
 * @file ProfileRangeOperation.h
 * @brief Profile Range Operation class
 *
 * @author M.Izumi
 * @date 2012.03.02
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_RANGE_PROFILERANGE_OPERATION_H__
#define __KOME_RANGE_PROFILERANGE_OPERATION_H__

#include "ProfileRangeManager.h"

namespace kome {
	namespace operation {
		namespace range {
			/**
			 * @class ProfileRangeOperation
			 * @brief Profile Range Operation class
			 */
			class ProfileRangeOperation : public kome::operation::Operation{
			public:
				/**
				 * @fn ProfileRangeOperation()
				 * @brief constructor
				 */
				ProfileRangeOperation();

				/**
				 * @fn virtual ~ProfileRangeOperation()
				 * @brief destructor
				 */
				virtual ~ProfileRangeOperation();
		
			protected:
				/** old previous data file path */
				std::string m_oldPrevPath;
			
				/** chromatogram object */
				kome::objects::Chromatogram* m_chrom;
			
				/** spectrum opject */ 
				kome::objects::Spectrum* m_spec;
				
				/** min x */
				double m_minX;
				/** max x */
				double m_maxX;
				/** min y */
				double m_minY;
				/** max y */
				double m_maxY;

				/** canvas */
				wxWindow* m_canvas;

				/** reset flag */
				bool m_bReset;
		
				/** range info */
				ProfileRangeManager::RangeInfo* m_range;

				/** index */
				int m_index;
			public:
				/**
				 * @fn void setChrom( kome::objects::Chromatogram* chrom )
				 * @brief set chromatogram
				 * @pram[in] chrom chromatogram object
				 */
				void setChrom( kome::objects::Chromatogram* chrom );

				/**
				 * @fn kome::objects::Chromatogram* getChrom()
				 * @brief gets chromatogram
				 * @return chromatogram object
				 */
				kome::objects::Chromatogram* getChrom();
			
				/**
				 * @fn void setSpec( kome::objects::Spectrum* spec )
				 * @brief set spectrum
				 * @param[in] spec spectrum object
				 */
				void setSpec( kome::objects::Spectrum* spec );

				/**
				 * @fn kome::objects::Spectrum* getSpec()
				 * @brief gets spectrum
				 * @return spectrum object
				 */
				kome::objects::Spectrum* getSpec();
								
				/**
				 * @fn void setCanvas( wxWindow* canvas )
				 * @brief set canvas
				 * @param[in] canvas canvas 
				 */
				void setCanvas( wxWindow* canvas );

				/**
				 * @fn wxWindow* getCanvas()
				 * @brief get canvas
				 * @return canvas canvas
				 */
				wxWindow* getCanvas();

				/**
				 * @fn void setbReset( bool reset )
				 * @brief wet reset flag
				 * @param reset
				 */
				void setbReset( bool reset );
				
				/**
				 * @fn bool getbRest()
				 * @brief get rest 
				 */
				bool getbRest();
		
				/**
				 * @fn void setRangeInfo( ProfileRangeManager::RangeInfo* range )
				 * @brief set range info
				 * @param range
				 */
				void setRangeInfo( ProfileRangeManager::RangeInfo* range );

				/**
				 * @fn ProfileRangeManager::RangeInfo* getRangeInfo()
				 * @brief get range info
				 */
				ProfileRangeManager::RangeInfo* getRangeInfo();

				/**
				 * @fn void setIndex( int index )
				 * @brief set index
				 * @param index
				 */
				void setIndex( int index );

				/**
				 * @fn int getIndex()
				 * @brief get index
				 */
				int getIndex();

				/**
				 * @fn void setMinX( double minX )
				 * @brief set min x
				 * @param minX
				 */
				void setMinX( double minX );

				/**
				 * @fn double getMinX()
				 * @brief get min x
				 */
				double getMinX();

				/**
				 * @fn void setMaxX( double maxX )
				 * @brief set max x
				 * @param maxX
				 */
				void setMaxX( double maxX );

				/**
				 * @fn double getMaxX()
				 * @brief get max x
				 */
				double getMaxX();

				/**
				 * @fn void setMinY( double minY )
				 * @brief set min y
				 * @param minY
				 */
				void setMinY( double minY );

				/**
				 * @fn double getMinY()
				 * @brief get min y
				 */
				double getMinY();

				/**
				 * @fn void setMaxY( double maxY )
				 * @brief set max y
				 * @param maxY
				 */
				void setMaxY( double maxY );

				/**
				 * @fn double getMaxY()
				 * @brief get max y
				 */
				double getMaxY();

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

#endif	// __KOME_RANGE_PROFILERANGE_OPERATION_H__
