/**
 * @file FileIOManager.h
 * @brief interfaces of FileIOManager class
 *
 * @author S.Tanaka
 * @date 2006.10.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_FILE_IO_MANAGER_H__
#define __KOME_IO_FILE_IO_MANAGER_H__

#include "ExportSampleDialog.h"	

namespace kome {
	namespace io {
		class FileIoOperation;

		/**
		 * @class FileIOManager
		 * @brief file export management class
		 */
		class FileIOManager {
		protected:
			/**
			 * @fn FileIOManager()
			 * @brief constructor
			 */
			FileIOManager();

			/**
			 * @fn virtual ~FileIOManager()
			 * @brief destructor
			 */
			virtual ~FileIOManager();

		protected:
			/** save function */
			kome::plugin::PluginFunctionItem* m_saveItem;

			/** save settings */
			kome::objects::SettingParameterValues* m_saveSettings;

			/** file name suffix */
			std::string m_suffix;

			/** sample selector */
			kome::core::Selector* m_sampleSelector;

			/** sample names */
			std::set< std::string > m_sampleNameSet;

			/** sample types */
			std::set< std::string > m_sampleTypeSet;
				
			kome::objects::Sample* m_openSample;
			
		public:
			void setOpenSample( kome::objects::Sample* sample );
			kome::objects::Sample* getOpenSample();
		// ---------------------------------------------> 2010.11.02 Add M.Izumi
		private:	
			/**
			 * @struct CheckListBox Information		 
			 * @brief struct that has name and Check status
			 */
			struct CheckListBoxInfo {
				std::string  strName;	// name
				bool bCheck;			// checkBox Status
			};

			/** check list box Information */
			std::vector< CheckListBoxInfo > m_ChkListInfos;    
					
		public:
			/**
			 * @fn kome::objects::SampleSet* openFile( const char* path )
			 * @brief opens raw data file
			 * @param[in] path raw data file path
			 * @param[in] progress
			 * @return sample set object
			 */
			kome::objects::SampleSet* openFile( const char* path, kome::core::Progress* progress = NULL );
		
		private:
			/**
			 * @fn void setChkListInfos( std::vector<std::string> strInfo, bool bflg )
			 * @brief Set the state of the checkboxInfo
			 * @param strInfo sample names
			 * @param bflg set check list box status
			 */
			void setChkListInfos( std::vector<std::string> strInfo, bool bflg );
	
			/**
			 * @fn void setChkListInfosAndSamples( kome::objects::Sample* sample )
			 * @brief set CheckListBoxInfo and sample
			 * @param[in] sample active sample
			 */
			void setChkListInfosAndSamples( kome::objects::Sample* sample );

			/**
			 * @fn void ShowCanvasTypeSearch()
			 * @brief Search the status of the canvas showing
			 */
			void ShowCanvasTypeSearch();
						
		public:
			/** Sample structure */
			std::vector< kome::objects::Sample* > m_samples;

			/** 
			 * @fn void setChkListInfosSpec(kome::window::Canvas* canvas)
			 * @brief Set the state of the checkboxInfo（Spectrum）
			 * @param canvas active canvas
			 */
			void setChkListInfosSpec(kome::window::Canvas* canvas);

			/** 
			 * @fn void setChkListInfosChrom(kome::window::Canvas* canvas)
			 * @brief Set the state of the checkboxInfo（Chromatogram）
			 * @param canvas active canvas
			 */
			void setChkListInfosChrom(kome::window::Canvas* canvas);

		
		// <--------------------------------------------- 2010.11.02 Add M.Izumi
		public:
			/**
			 * @fn void setSaveItem( kome::plugin::PluginFunctionItem* item )
			 * @brief sets save item
			 * @param[in] item save item
			 */
			void setSaveItem( kome::plugin::PluginFunctionItem* item );

			/**
			 * @fn kome::plugin::PluginFunctionItem* getSaveItem()
			 * @brief gets save item
			 * @return save item
			 */
			kome::plugin::PluginFunctionItem* getSaveItem();

			/**
			 * @fn void setStatusSettings                  
			 * @brief sets data save settings.
			 */
			void setSaveSettings(kome::objects::SettingParameterValues* saveSettings);

			/**
			 * @fn kome::objects::SettingParameterValues getStatusSettings()                  
			 * @brief gets data save settings.
			 * @return kome::objects::SettingParameterValues
			 */
			kome::objects::SettingParameterValues getSaveSettings();

		public:
			/**
			 * @fn void setGuiSelector()
			 * @brief sets GUI selector
			 */
			void setGuiSelector();

			/**
			 * @fn kome::core::Selector* getSelector()
			 * @brief gets sample selector
			 * @return sample selector
			 */
			kome::core::Selector* getSelector();

			/**
			 * @fn void clearSampleNames()
			 * @brief clears sample names
			 */
			void clearSampleNames();

			/**
			 * @fn void addSampleName( const char* name )
			 * @brief adds sample name
			 * @param name sample name
			 */
			void addSampleName( const char* name );

			/**
			 * @fn void addSampleType( const char* name )
			 * @brief adds sample type
			 * @param name sample type
			 */
			void addSampleType( const char* name );

			/**
			 * @fn void openSelectedSample( kome::objects::SampleSet* sampleSet )
			 * @brief opens selected sample
			 * @param sampleSet sample set
			 */
			void openSelectedSample( kome::objects::SampleSet* sampleSet, kome::core::Progress* progress );

		public:

			/**
			 * @fn bool openSampleCloseCheckDaialog( kome::objects::Sample* sample )
			 * @brief open SampleCloseCheckDialog 
			 * @param[in] sample active sample
			 * @return If it failed to open the dialog, this method returns false
			 */
			bool openSampleCloseCheckDaialog( kome::objects::Sample* sample );		// 2010.11.02 Add M.Izumi

			
			/**
			 * @fn bool openSampleSet()
			 * @brief opens sample set
			 * @return If it failed to open the file, this method returns false
			 */
			bool openSampleSet();

			/**
			 * @fn bool exportSpectraWithDialog( kome::objects::DataSet& dataSet, kome::plugin::PluginFunctionItem* item, const bool checkData, bool* cancelFlg )
			 * @brief exports spectra with dialog
			 * @param spectra spectra object
			 * @param saveFormat　save Format
			 * @param checkData　whether to check for chromatogram / spectrum data
			 * @param[out] cancelFlg the pointer to store canceled flag
			 * @return If it failed to export, this method returns false.
			 */
			bool exportSpectraWithDialog( kome::objects::DataSet& dataSet, kome::plugin::PluginFunctionItem* item, const bool checkData, bool* cancelFlg );

			/**
			 * @fn bool exportSpectra(
						kome::objects::DataSet& dataSet,
						kome::plugin::PluginFunctionItem* saveItem,
						const char* path,
						kome::core::Progress& progress
					)
			 * @brief exports spectra information to file
			 * @param spectra spectra object
			 * @param saveItem save item
			 * @param path save path
			 * @param progress progress
			 * @return If it failed to save, this method returns false.
			 */
			bool exportSpectra(
				kome::objects::DataSet& dataSet,
				kome::plugin::PluginFunctionItem* saveItem,
				const char* path,
				kome::core::Progress& progress,
				kome::objects::SettingParameterValues* saveSettings     // @date 2012/8/20 <Add> FUJITA
			);

			/**
			 * @fn void closeSample( kome::objects::Sample* sample )
			 * @brief closes sample
			 * @param sample sample object to be closed
			 */
			void closeSample( kome::objects::Sample* sample );

			/**
			 * @fn void saveSample( kome::objects::Sample* sample )
			 * @brief saves sample
			 * @param sample sample object to be saved
			 */
			bool saveSample( kome::objects::Sample* sample );

			/**
			 * @fn kome::plugin::PluginFunctionItem* FindPluginManagerFunctionItem(
						const std::string& fileExtension
					)
			 * @brief finds the plugin manager function to save a file with the specified fileExtension
			 * @param fileExtension extension of file to save
			 * @return Pointer to the plugin function item, or NULL if one cannot be found.
			 */
			kome::plugin::PluginFunctionItem* FindPluginManagerFunctionItem(const std::string& fileExtension);
					
			/**
			 * @fn void setSuffix( const char* suffix )
			 * @brief sets file name suffix
			 * @param suffix
			 */
			void setSuffix( const char* suffix );

			/**
			 * @fn const char* getSuffix()
			 * @brief gets file name suffix
			 * @return suffix
			 */
			const char* getSuffix();

			/**
			 * @fn void setFileDivisionFlag( const bool flag )
			 * @brief sets file division flag
			 * @param flag the value of file division flag
			 */
			void setFileDivisionFlag( const bool flag );

			/**
			 * @fn bool getFileDivisionFlag()
			 * @brief gets file division flag
			 * @return the value of file division flag
			 */
			bool getFileDivisionFlag();

		public:
			/**
			 * @fn std::string getSaveFilters()
			 * @brief gets save filter
			 * @return save filters
			 */
			std::string getSaveFilters();

		public:
			/**
			 * @fn bool saveSharedData( const char* path )
			 * @brief saves shared data
			 * @param path output path
			 * @return If true, it succeeded to wrrite the active spectra data.
			 */
			bool saveSharedData( const char* path );

			/**
			 * @fn void removeOperation( FileIoOperation* opt )
			 * @brief FileIoOperation object to remove
			 * @param[in] opt Operation to remove
			 */
			void removeOperation( FileIoOperation* opt );

		protected:
			/**
			 * @fn bool saveFile(
					kome::objects::DataSet* dataSet,
					const char* dir,
					const char* fileName,
					const char* ext
				)
			 * @brief saves spectra data to one file
			 * @param spectra spectra to be saved
			 * @param dir output file directory
			 * @param fileName output file name
			 * @param ext output file extension
			 */
			bool saveFile(
				kome::objects::DataSet* dataSet,
				const char* dir,
				const char* fileName,
				const char* ext
			);

			/**
			 * @fn void getSpectrumFilePath(
					kome::objects::Sample* sample,
					kome::objects::DataSet* dataSet,
					const char* dir,
					const char* fileName,
					const char* ext,
					std::map< kome::objects::Spectrum*, std::string >& pathMap
				)
			 * @brief gets spectrum file path
			 * @param sample sample
			 * @param spectra spectra
			 * @param dir output file directory
			 * @param fileName output file name
			 * @param ext output file extension
			 * @param pathMap the map to store spectrum file path
			 */
			void getSpectrumFilePath(
				kome::objects::Sample* sample,
				kome::objects::DataSet* dataSet,
				const char* dir,
				const char* fileName,
				const char* ext,
				std::map< kome::objects::Spectrum*, std::string >& pathMap
			);

		public:
			/**
			 * @fn static FileIOManager& getInstance()
			 * @brief gets object.
			 * @return FileIOManager object (This is the only object.)
			 */
			static FileIOManager& getInstance();	
		};
	}
}


#endif		//  __KOME_IO_FILE_IO_MANAGER_H__
