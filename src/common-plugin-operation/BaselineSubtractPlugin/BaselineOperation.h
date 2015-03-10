/**
 * @file BaselineOperation.h
 * @brief Baseline Subtraction Operation class
 *
 * @author M.Izumi
 * @date 2011.12.16
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_BASELINE_SPECTROUMBASELINE_OPERATION_H__
#define __KOME_BASELINE_SPECTROUMBASELINE_OPERATION_H__


namespace kome {
	namespace baseline {
		/**
		 * @class BaselineOperation
		 * @brief Baseline operation class
		 */
		class BaselineOperation : public kome::operation::Operation{
		public:
			/**
			 * @fn BaselineOperation()
			 * @brief constructor
			 */
			BaselineOperation();

			/**
			 * @fn virtual ~BaselineOperation()
			 * @brief destructor
			 */
			virtual ~BaselineOperation();
		
		protected:
			
			/** chromatogram object */
			kome::objects::Chromatogram* m_chrom;
			
			/** spectrum opject */ 
			kome::objects::Spectrum* m_spec;
			
			/** baseline subtract function item */
			kome::plugin::PluginFunctionItem* m_item;

			/** setting parameter values */
			kome::objects::SettingParameterValues  m_settings;

			/** xyData operation */
			kome::objects::XYDataOperation* m_xyOpt;

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
			 * @fn void setFuncItem( kome::plugin::PluginFunctionItem* item )
			 * @brief set baseline subtract function item
			 * @param[in] item baseline function item
			 */
			void setFuncItem( kome::plugin::PluginFunctionItem* item );

			/**
			 * @fn kome::plugin::PluginFunctionItem* getFuncItem()
			 * @brief get baseline function item
			 * @return baseline function item
			 */
			kome::plugin::PluginFunctionItem* getFuncItem();

			/**
			 * @fn void setBaselineSettings( kome::objects::SettingParameterValues* paramValue )
			 * @brief set baseline setting parameter values
			 * @param[in] paramValue setting parameter values
			 */
			void setBaselineSettings( kome::objects::SettingParameterValues* paramValue );

			/**
			 * @fn void kome::objects::SettingParameterValues getBaselineSettings()
			 * @brief get baseline setting parameter values
			 * @return setting parameter values
			 */
			kome::objects::SettingParameterValues getBaselineSettings();

			/**
			 * @fn void setxyDataOperation(kome::objects::XYDataOperation* opt)
			 * @brief set xyDataOperation 
			 * @param[in] opt XYDataOperation object
			 */
			void setxyDataOperation(kome::objects::XYDataOperation* opt);

			/**
			 * @fn kome::objects::XYDataOperation* getxyDataOperation()
			 * @brief get xyDataOperation
			 * @return XYDataOperation object
			 */
			kome::objects::XYDataOperation* getxyDataOperation();
		protected:
			/**
			 * @fn void updateSpec( kome::objects::Spectrum* spec, boost::function< int ( void*, int ) > readFun )
			 * @brief update spectrum xyData
			 * @param[in] spec spectrum object
			 * @param[in] readFun function to read saved state
			 */
			void updateSpec( kome::objects::Spectrum* spec, boost::function< int ( void*, int ) > readFun );
	
			/**
			 * @fn void updateSpec( kome::objects::Chromatogram* chrom, boost::function< int ( void*, int ) > readFun )
			 * @brief update spectrum xyData
			 * @param[in] chrom chromatogram object
			 * @param[in] readFun function to read saved state
			 */
			void updateChrom( kome::objects::Chromatogram* chrom, boost::function< int ( void*, int ) > readFun );

			/**
			 * @fn void editXYDataOperation( kome::objects::Spectrum* spec )
			 * @brief Undo/Redo時にXYDataOperationの追加、削除処理を行う
			 * @param[in] spec spectrum objects
			 */
			void editXYDataOperation( kome::objects::Spectrum* spec );

			
			/**
			 * @fn void editXYDataOperation( kome::objects::Chromatogram* chrom )
			 * @brief Undo/Redo時にXYDataOperationの追加、削除処理を行う
			 * @param[in] chrom chromatogram object
			 */
			void editXYDataOperation( kome::objects::Chromatogram* chrom );
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

#endif	// __KOME_BASELINE_SPECTROUMBASELINE_OPERATION_H__
