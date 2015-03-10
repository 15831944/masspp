/**
 * @file FileIOPluginOperation.h
 * @brief File Io Operation class
 *
 * @author M.Izumi
 * @date 2012.01.25
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_IO_EXPORT_SAMPLE_OPERATION_H__
#define __KOME_IO_EXPORT_SAMPLE_OPERATION_H__

namespace kome {
	namespace io {
		/**
		 * @class FileIOPluginOperation
		 * @brief FileIOPlugin Operation class
		 */
		class FileIoOperation : public kome::operation::Operation{
		public:
			/**
			 * @fn FileIoOperation()
			 * @brief constructor
			 */
			FileIoOperation();

			/**
			 * @fn virtual ~FileIOManager()
			 * @brief destructor
			 */
			virtual ~FileIoOperation();

		protected:
			/** file path */
			std::string m_filePath;

			/** index */
			int m_index;

			/** data set object */
			kome::objects::DataSet m_dataSet;

			/** target */
			int m_target;

			/** plugin function item */
			kome::plugin::PluginFunctionItem* m_item;

			/** save settings */
			kome::objects::SettingParameterValues m_saveSettings;

			kome::core::Progress* m_progress; // @date 2014.07.08 <Add> M.Izumi 
		public:
			/**
			 * @fn void setIndex( int index )
			 * @brief set index
			 * @param[in] index
			 */
			void setIndex( int index );

			/**
			 * @fn int getIndex()
			 * @brief get index
			 * @return index
			 */
			int getIndex();

			/**
			 * @fn void setFilePath( const char* path )
			 * @brief set file path
			 * @param[in] path
			 */
			void setFilePath( const char* path );
			
			/**
			 * @fn std::string getFilePath()
			 * @brief get file path
			 * @return file path
			 */
			std::string getFilePath();
			
			/**
			 * @fn void setDataSet( kome::objects::DataSet dataSet )
			 * @brief set data set object
			 * @param[in] dataset DataSet objects
			 */
			void setDataSet( kome::objects::DataSet dataSet );
			
			/**
			 * @fn kome::objects::DataSet getDataSet()
			 * @brief get data set objects
			 * @return dataset objects
			 */
			kome::objects::DataSet getDataSet();
			
			/**
			 * @fn void setTarget( int target )
			 * @brief set target
			 * @param[in] target select target
			 */
			void setTarget( int target );
			
			/**
			 * @fn int getTarget()
			 * @brief get target
			 * @param[in] target select target
			 */
			int getTarget();
			
			/**
			 * @fn void setFuncItem( kome::plugin::PluginFunctionItem* item )
			 * @brief set  plugin function item
			 * @param[in] item PluginFunction Item
			 */
			void setFuncItem( kome::plugin::PluginFunctionItem* item );
			
			/**
			 * @fn kome::plugin::PluginFunctionItem* getFuncItem()
			 * @brief get plugin function item
			 * @return item PluginFunction Item
			 */
			kome::plugin::PluginFunctionItem* getFuncItem();

			/**
			 * @fn void setSaveSettings( kome::objects::SettingParameterValues* settigns )
			 * @brief set save settings
			 * @param[in] settings
			 */
			void setSaveSettings( kome::objects::SettingParameterValues* settigns ){
				m_saveSettings = *settigns;
			}

			/**
			 * @fn kome::objects::SettingParameterValues getSaveSettings()
			 * @brief get save settings
			 * @return save settings
			 */
			kome::objects::SettingParameterValues getSaveSettings(){
				return m_saveSettings;
			}

			/** 
			 * @fn void setProgress( kome::core::Progress* progress )
			 * @brief set progress
			 * @param progress
			 */
			void setProgress( kome::core::Progress* progress );

			/**
			 * @fn kome::core::Progress* getProgress()
			 * @brief get progress
			 * @return progress
			 */
			kome::core::Progress* getProgress();
			
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


#endif	// __KOME_IO_EXPORT_SAMPLE_OPERATION_H__
