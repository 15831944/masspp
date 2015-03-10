/**
 * @file SampleFusionOperation.cpp
 * @brief Sample Fusion Operation class
 *
 * @author M.Izumi
 * @date 2012.03.02
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_COMPOSITION_SAMPLE_FUSION_OPERATION_H__
#define __KOME_COMPOSITION_SAMPLE_FUSION_OPERATION_H__

namespace kome {
	namespace composition {
		/**
		 * @class SampleFusionOperation
		 * @brief Sample Fusion Operation class
		 */
		class SampleFusionOperation : public kome::operation::Operation {
		
		public:
			/**
			* @fn SampleFusionOperation()
			* @brief constructor
			*/
			SampleFusionOperation();

			/**
			* @fn virtual ~SampleFusionOperation()
			* @brief destructor
			*/
			virtual ~SampleFusionOperation();

		protected:
			/** treatment group */
			kome::objects::DataGroupNode* m_trmtGroup;

			/** ctrl group */
			kome::objects::DataGroupNode* m_ctrlGroup;

			/** alignment function item */
			kome::plugin::PluginFunctionItem*  m_alignItem;

			/** alignment settings */
			kome::objects::SettingParameterValues m_alignSettings;

			/** conposition function item */
			kome::plugin::PluginFunctionItem* m_compositionItem;

			/** composition settings */
			kome::objects::SettingParameterValues m_compositionSettings;
		
		public:
			/**
			 * @fn void setTrmtGroup( kome::objects::DataGroupNode* trmtGroup )
			 * @brief set treatment group
			 * @param trmtGroup
			 */
			void setTrmtGroup( kome::objects::DataGroupNode* trmtGroup );

			/**
			 * @fn kome::objects::DataGroupNode* getTrmtGroup()
			 * @brief get treatment group
			 * @return treatment group
			 */
			kome::objects::DataGroupNode* getTrmtGroup();

			/**
			 * @fn void setCtrlGroup( kome::objects::DataGroupNode* ctrlGroup )
			 * @brief set ctrl group
			 * @param ctrl group
			 */
			void setCtrlGroup( kome::objects::DataGroupNode* ctrlGroup );

			/**
			 * @fn kome::objects::DataGroupNode* getCtrlGroup()
			 * @brief get ctrl group
			 * @return ctrl group
			 */
			kome::objects::DataGroupNode* getCtrlGroup();

			/**
			 * @fn void setAlignmentFuncItem( kome::plugin::PluginFunctionItem* item )
			 * @brief set alignment function item
			 * @param[in] item baseline function item
			 */
			void setAlignmentFuncItem( kome::plugin::PluginFunctionItem* item );

			/**
			 * @fn kome::plugin::PluginFunctionItem* getAlignmentFuncItem()
			 * @brief get alignment function item
			 * @return alignment function item
			 */
			kome::plugin::PluginFunctionItem* getAlignmentFuncItem();

			/**
			 * @fn void setAlignmentSettings( kome::objects::SettingParameterValues* settings )
			 * @brief set alignment setting parameter values
			 * @param[in] settings setting parameter values
			 */
			void setAlignmentSettings( kome::objects::SettingParameterValues* paramValue );

			/**
			 * @fn void kome::objects::SettingParameterValues getAlignmentSettings()
			 * @brief get alignment setting parameter values
			 * @return setting parameter values
			 */
			kome::objects::SettingParameterValues getAlignmentSettings();

			/**
			 * @fn void setCompositionItem( kome::plugin::PluginFunctionItem* item )
			 * @brief set composition function item
			 * @param[in] item composition function item
			 */
			void setCompositionItem( kome::plugin::PluginFunctionItem* item );

			/**
			 * @fn kome::plugin::PluginFunctionItem* getCompositionItem()
			 * @brief get composition function item
			 * @return composition function item
			 */
			kome::plugin::PluginFunctionItem* getCompositionItem();

			/**
			 * @fn void setCompositionSettings( kome::objects::SettingParameterValues* settings )
			 * @brief set composition setting parameter values
			 * @param[in] settings setting parameter values
			 */
			void setCompositionSettings( kome::objects::SettingParameterValues* settings );

			/**
			 * @fn void kome::objects::SettingParameterValues getCompositionSettings()
			 * @brief get composition setting parameter values
			 * @return setting parameter values
			 */
			kome::objects::SettingParameterValues getCompositionSettings();

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

#endif // __KOME_COMPOSITION_SAMPLE_FUSION_OPERATION_H__
