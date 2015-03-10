/**
 * @file MzRangeFilterOperation.h
 * @brief MzRangeFilter Operation class
 *
 * @author M.Izumi
 * @date 2013.05.20
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_MZRANGE_FILTER_OPERATION_H__
#define __KOME_MZRANGE_FILTER_OPERATION_H__

namespace kome {
	namespace mzfilter {

		/**
		 * @class MzRangeFilterOperation
		 * @brief m/z Range Filter Operation
		 */
		class MzRangeFilterOperation :  public kome::operation::Operation {
		
		public:
			/**
			 * @fn MzRangeFilterOperation()
			 * @brief constructor
			 */
			MzRangeFilterOperation();

			/**
			 * @fn virtual ~MzRangeFilterOperation()
			 * @brief destructor
			 */
			virtual ~MzRangeFilterOperation();

		protected:
			/** start m/z */
			double m_startMz;

			/** emd m/z */
			double m_endMz;

			/** spectrum objects */
			std::vector< kome::objects::Spectrum* > m_spectra;

			/** MzRangeFilter operation */
			std::map< kome::objects::Spectrum*, std::vector< kome::operation::MzRangeFilter* > > m_filters;
		
		public:
			/**
			 * @fn void setStartMz( double startMz )
			 * @brief sets start m/z
			 * @param satarMz
			 */
			void setStartMz( double startMz );

			/**
			 * @fn double getStartMz()
			 * @brief gets satart m/z 
			 * @return start m/z 
			 */
			double getStartMz();

			/**
			 * @fn void setEndMz( double endMz )
			 * @brief sets end m/z 
			 * @param end m/z
			 */
			void setEndMz( double endMz );

			/**
			 * @fn double getEndMz()
			 * @brief get end m/z
			 * @return end m/z
			 */
			double getEndMz();

			/**
			 * @fn void setSpectra( std::vector< kome::objects::Spectrum* > spectra )
			 * @brief sets spectra
			 * @param spectra
			 */
			void setSpectra( std::vector< kome::objects::Spectrum* > spectra );

			/**
			 * @fn std::vector< kome::objects::Spectrum* > getSpectra()
			 * @brief gets spectra
			 * @return spectra
			 */
			std::vector< kome::objects::Spectrum* > getSpectra();

		protected:
			/**
			 * @fn void addOperation( kome::objects::Spectrum* spec )
			 * @brief add operation
			 * @param spec
			 */
			void addOperation( kome::objects::Spectrum* spec );
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


#endif // __KOME_MZRANGE_FILTER_OPERATION_H__
