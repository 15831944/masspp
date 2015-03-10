/**
 * @file GenelateChormOperaton.h
 * @brief Genelate Chorm Operation class
 *
 * @author M.Izumi
 * @date 2013.07.05
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_GENELATE_CHROM_OPERATION_H__
#define __KOME_GENELATE_CHROM_OPERATION_H__


namespace kome {
	namespace operation {
		namespace sel_range {
			class GenelateChormOperaton : public kome::operation::Operation {
			public:
				/**
				 * @fn GenelateChormOperaton()
				 * @brief constructor
				 */
				GenelateChormOperaton();

				/**
				 * @fn virtual ~GenelateChormOperaton()
				 * @brief destructor
				 */
				virtual ~GenelateChormOperaton();

			protected:
				kome::objects::DataGroupNode* m_group;
				double m_startPos;
				double m_endPos;
				std::string m_paramName;
				std::string m_title;
				kome::objects::Chromatogram* m_chrom;
			public:
				void setGroup( kome::objects::DataGroupNode* group ){ m_group = group; }
				kome::objects::DataGroupNode* getGroiup() { return m_group; }

				void setStartPos( double startPos ){ m_startPos = startPos; }
				double getStartPos(){ return m_startPos; }

				void setEndPos( double endPos ){ m_endPos = endPos; }
				double getEndPos(){ return m_endPos; }

				void setParamName( const char* paramName ){ m_paramName = paramName; }
				const char* getParamName(){ return m_paramName.c_str(); }

				void setTitle( const char* title ) { m_title = NVL( title, "" ); }
				const char* getTitle() { return m_title.c_str(); }

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

#endif	// __KOME_GENELATE_CHROM_OPERATION_H__
