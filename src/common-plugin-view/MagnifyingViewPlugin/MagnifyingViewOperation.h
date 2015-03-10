/**
 * @file MagnifyingViewOperation.h
 * @brief interfaces of MagnifyingViewOperation class
 *
 * @author S.Tanaka
 * @date 2012.04.02
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_VIEW_MAGNIFYING_VIEW_OPERATION_H__
#define __KOME_VIEW_MAGNIFYING_VIEW_OPERATION_H__


namespace kome {
	namespace view {

		/**
		 * @class MagnifyingViewOperation
		 * @brief change range plug-in management class
		 */
		class MagnifyingViewOperation : public kome::operation::Operation {
		public:
			/**
			 * @fn MagnifyingViewOperation()
			 * @brief constructor
			 */
			MagnifyingViewOperation();

			/**
			 * @fn virtual ~MagnifyingViewOperation()
			 * @brief destructor
			 */
			virtual ~MagnifyingViewOperation();

		protected:
			/** spectrum */
			kome::objects::Spectrum* m_spec;

			/** index */
			int m_idx;

			/** start position */
			double m_start;

			/** end position */
			double m_end;

			/** scale (If negative, clear the mafnifying view ) */
			double m_scale;

		public:
			/**
			 * @fn void setSpectrum( kome::objects::Spectrum* spec )
			 * @brief sets the magnified spectrum
			 * @param[in] spec magnified spectrum
			 */
			void setSpectrum( kome::objects::Spectrum* spec );

			/**
			 * @fn kome::objects::Spectrum* getSpectrum();
			 * @brief gets the mignified spectrum
			 * @return magnified spectrum
			 */
			kome::objects::Spectrum* getSpectrum();

			/**
			 * @fn void setScaleIndex( const int idx )
			 * @brief sets the scale index
			 * @param[in] idx scale index
			 */
			void setScaleIndex( const int idx );

			/**
			 * @fn int getScaleIndex()
			 * @brief gets the scale index
			 * @param[in] idx scale index
			 */
			int getScaleIndex();

			/**
			 * @fn void setRange( const double start, const double end )
			 * @brief sets the magnifying range
			 * @param[in] start the start of the range
			 * @param[in] end the end of the range
			 */
			void setRange( const double start, const double end );

			/**
			 * @fn double getRangeStart()
			 * @brief get the start of the range
			 * @return the start of the range
			 */
			double getRangeStart();

			/**
			 * @fn double getRangeEnd()
			 * @brief gets the end of the range
			 * @return the end of the range
			 */
			double getRangeEnd();

			/**
			 * @fn void setScale( const double scale )
			 * @brief sets the magnifying scale
			 * @param[in] scale the magnifying scale
			 */
			void setScale( const double scale );

			/**
			 * @fn double getScale()
			 * @brief gets the magnifying scale
			 * @return magnifying scale
			 */
			double getScale();

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

#endif		// __KOME_VIEW_MAGNIFYING_VIEW_OPERATION_H__
