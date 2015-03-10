/**
 * @file PeakEditOperation.h
 * @brief Peak Edit Operation class
 *
 * @author M.Izumi
 * @date 2013.05.28
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_PEAKS_PEAK_EDIT_OPERATION_H__
#define __KOME_PEAKS_PEAK_EDIT_OPERATION_H__

namespace kome {
	namespace peaks {
		namespace edit {

			class PeakEditOperation : public kome::operation::Operation{
			public:
				/**
				 * @fn PeakEditOperation()
				 * @brief constructor
				 */
				PeakEditOperation();

				/**
				 * @fn virtual ~PeakEditOperation()
				 * @brief destructor
				 */
				virtual ~PeakEditOperation();

			protected:
				kome::objects::Spectrum* m_spec;
				kome::objects::Chromatogram* m_chrom;
				kome::img::Graphics* m_graphics; 
				kome::evt::MouseEvent* m_mouseEvt;
				int m_flgStartEdirSpec;
				int m_flgStartEdirChrom;
			public:
				
				void setSpec( kome::objects::Spectrum* spec );
				kome::objects::Spectrum* getSpec();
				
				void setChrom( kome::objects::Chromatogram* chrom );
				kome::objects::Chromatogram* getChrom();

				void setGraphics( kome::img::Graphics* g );
				kome::img::Graphics* getGraphics();

				void setMouseEvt( kome::evt::MouseEvent* evt );
				kome::evt::MouseEvent* getMouseEvt();

				void setFlgStartEdirSpec( int flgStartEditSpec );
				int getFlgStartEditSpec();
				
				void setFlgStartEdirChrom( int flgStartEditChrom );
				int getFlgStartEditChrom();

			protected:
				void deletePeak( int call );
				bool specEditPeaks();
				bool chromEditPeaks();
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


#endif // __KOME_PEAKS_PEAK_EDIT_OPERATION_H__
