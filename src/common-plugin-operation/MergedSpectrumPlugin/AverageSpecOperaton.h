/**
 * @file AverageSpecOperaton.h
 * @brief averaged spectrum  Operation class
 *
 * @author M.Izumi
 * @date 2013.08.05
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_AVERAGED_SPEC_OPERATION_H__
#define __KOME_AVERAGED_SPEC_OPERATION_H__

namespace kome {
	namespace operation {
		namespace merged {

			class AverageSpecOperaton : public kome::operation::Operation {
			public:
				AverageSpecOperaton();

				virtual ~AverageSpecOperaton();

			protected:
				kome::objects::Chromatogram* m_chrom;
				double m_startRt;
				double m_endRt;
				
				kome::objects::Spectrum* m_spec;

			public:
				void setChrom( kome::objects::Chromatogram* chrom ){ m_chrom = chrom; }
				kome::objects::Chromatogram* getChrom(){ return m_chrom; }

				void setStartRt( double rt ){ m_startRt = rt; }
				double getStartRt(){ return m_startRt; }

				void setEndRt( double rt ){ m_endRt = rt; }
				double getEndRt(){ return m_endRt; }


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



#endif // __KOME_AVERAGED_SPEC_OPERATION_H__
