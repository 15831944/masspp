/**
 * @file PeakDetectOperation.h
 * @brief Peak Detect Operation class
 *
 * @author M.Izumi
 * @date 2012.01.31
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_LABELING_PEAK_DETECT_OPERATION_H__
#define __KOME_LABELING_PEAK_DETECT_OPERATION_H__

namespace kome {
	namespace labeling {

		/**
		 * @class PeakDetectOperation
		 * @brief peak detect operaton class
		 */
		class PeakDetectOperation : public kome::operation::Operation{
		public:
			/**
			 * @fn PeakDetectOperation()
			 * @brief constructor
			 */
			PeakDetectOperation();

			/**
			 * @fn virtual ~PeakDetectOperation()
			 * @brief destructor
			 */
			virtual ~PeakDetectOperation();
		
		protected:
			/** spectrum opjects */ 
			std::vector<kome::objects::Spectrum*> m_spectra;

			/** chromatogram opject */ 
			kome::objects::Chromatogram* m_chrom;

			/** group opject */ 
			kome::objects::DataGroupNode* m_group;
			
			/**  peak detector */
			kome::plugin::PluginFunctionItem*		 m_peakDetector;

			/** charge detector */
			kome::plugin::PluginFunctionItem*		 m_chargeDetector;
						
			/** label settings */
			kome::objects::SettingParameterValues	 m_labelSettings;
			
			/** charge settings */
			kome::objects::SettingParameterValues	 m_chargeSettings;		   
			
			/** target */
			int m_target;

		public:
			/**
			 * @fn std::vector<kome::objects::Spectrum*> getSpectra()
			 * @brief gets spectra
			 * @return spectrum objects
			 */
			std::vector<kome::objects::Spectrum*> getSpectra();	

			/**
			 * @fn void setSpectra( std::vector<kome::objects::Spectrum*> spectra )
			 * @brief sets spectra
			 * @param[in] spec spectrum objects
			 */
			void setSpectra( std::vector<kome::objects::Spectrum*> spectra );
	
			/**
			 * @fn kome::objects::Chromatogram* getChrom()
			 * @brief gets chromatogram
			 * @return chromatogram object
			 */
			kome::objects::Chromatogram* getChrom();

			/**
			 * @fn void setChrom( kome::objects::Chromatogram* chrom )
			 * @brief sets chromatogram
			 * @pram[in] chrom chromatogram object
			 */
			void setChrom( kome::objects::Chromatogram* chrom );

			/**
			 * @fn kome::objects::DataGroupNode* getGroup()
			 * @brief gets group
			 * @return gropu object
			 */
			kome::objects::DataGroupNode* getGroup();

			/**
			 * @fn void setGroup( kome::objects::DataGroupNode* group )
			 * @brief sets group
			 * @pram[in] group group object
			 */
			void setGroup( kome::objects::DataGroupNode* group );
			
			/**
			 * @fn kome::plugin::PluginFunctionItem* getPeakDetect()
			 * @brief gets peak detector
			 * @return peak detector
			 */
			kome::plugin::PluginFunctionItem* getPeakDetect();

			/**
			 * @fn void setPeakDetect( kome::plugin::PluginFunctionItem* item )
			 * @brief sets peak detector
			 * @param[in] item peak detect function
			 */
			void setPeakDetect( kome::plugin::PluginFunctionItem* item );

			/**
			 * @fn kome::plugin::PluginFunctionItem* getChargeDetect()
			 * @brief gets charge detect function
			 * @return charge detect function
			 */
			kome::plugin::PluginFunctionItem* getChargeDetect();
			
			/**
			 * @fn void setChargeDetct( kome::plugin::PluginFunctionItem* item )
			 * @brief sets charge detect function
			 * @param[in] item charge detect function
			 */
			void setChargeDetct( kome::plugin::PluginFunctionItem* item );

			/**
			 * @fn kome::objects::SettingParameterValues getLabelSettings()
			 * @brief gets label settings
			 * @return label settings
			 */
			kome::objects::SettingParameterValues getLabelSettings();
			
			/**
			 * @fn void setLabelSettings( kome::objects::SettingParameterValues labelSettings )
			 * @brief sets label settings
			 * @param[in] labelSettings peak label settings
			 */
			void setLabelSettings( kome::objects::SettingParameterValues labelSettings );

			/**
			 * @fn kome::objects::SettingParameterValues getChargeSettings()
			 * @brief gets charge settings
			 * @return charge settings
			 */
			kome::objects::SettingParameterValues getChargeSettings();
			
			/**
			 * @fn void setChargeSettings( kome::objects::SettingParameterValues chargeSettings )
			 * @brief sets charge settings
			 * @param[in] chargeSettings charge settings
			 */
			void setChargeSettings( kome::objects::SettingParameterValues chargeSettings );

			/**
			 * @fn int getTarget()
			 * @brief gets target
			 * @return select target
			 */
			int getTarget();
			
			/**
			 * @fn void setTarget( int target )
			 * @brief sets target
			 * @param[in] target
			 */
			void setTarget( int target );
		
		protected:
			/**
			 * @fn void loadSpecPeak( boost::function< int ( void*, int ) > readFun )
			 * @brief スペクトル ピーク検出の状態を読込
			 * @param[in] readFun function to read saved state
			 */
			void loadSpecPeak( boost::function< int ( void*, int ) > readFun );

			/**
			 * @fn void saveSpecPeak( boost::function< int ( void*, int ) > writeFun )
			 * @brief スペクトル ピーク検出の状態を書込
			 * @param[in] writeFun function to save state
			 */
			void saveSpecPeak( boost::function< int ( void*, int ) > writeFun );

			/**
			 * @fn void loadChromPeak( boost::function< int ( void*, int ) > readFun )
			 * @brief クロマトグラム ピーク検出の状態を読込
			 * @param[in] readFun function to read saved state
			 */
			void loadChromPeak( boost::function< int ( void*, int ) > readFun );

			/**
			 * @fn void saveChromPeak( boost::function< int ( void*, int ) > writeFun )
			 * @brief クロマトグラム ピーク検出の状態を書込
			 * @param[in] writeFun function to save state
			 */
			void saveChromPeak( boost::function< int ( void*, int ) > writeFun );
			
			/**
			 * @fn void loadPeakAndCluster( boost::function< int ( void*, int ) > readFun )
			 * @brief グループ ピーク検出とクラスタの状態を読込
			 * @param[in] readFun function to read saved state
			 */
			void loadPeakAndCluster( boost::function< int ( void*, int ) > readFun );

			/**
			 * @fn void savePeakAndCluster( boost::function< int ( void*, int ) > writeFun )
			 * @brief グループ ピーク検出とクラスタの状態を書込
			 * @param[in] writeFun function to save state
			 */
			void savePeakAndCluster( boost::function< int ( void*, int ) > writeFun );
			
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

		private:
			/**
			 * @fn bool specPeakDetectFun()
			 * @brief spectrum peak detect function
			 * @retunr true Peak検出成功
			 */		
			bool specPeakDetectFun();

			/**
			 * @fn bool chromPeakDetectFun()
			 * @brief chromatogram peak detect function
			 * @retunr true Peak検出成功
			 */	
			bool chromPeakDetectFun();
			
			/**
			 * @fn bool peak2DDetectFun()
			 * @brief 2d peak detect function
			 * @retunr true Peak検出成功
			 */	
			bool peak2DDetectFun();
			
			/**
			 * @fn std::string getSpecDescription()
			 * @brief gets spectrum description
			 * @return spectrum description
			 */	
			std::string getSpecDescription();
			
			/**
			 * @fn std::string getChromDescription()
			 * @brief gets chromatogram description
			 * @return chromatogram descripiton
			 */	
			std::string getChromDescription();
			
			/**
			 * @fn std::string get2dDescription()
			 * @brief gets 2d description
			 * @return 2d description
			 */	
			std::string get2dDescription();

			/**
			 * @fn std::string getSpecParametersString()
			 * @brief gets spectrum parameters string
			 * @return spectrum parameters string
			 */	
			std::string getSpecParametersString();
			
			/**
			 * @fn std::string getChromParametersString()
			 * @brief gets chromatogram parameters string
			 * @return chromatogram parameters string
			 */	
			std::string getChromParametersString();
			
			/**
			 * @fn std::string get2dParametersString()
			 * @brief gets 2d parameters string
			 * @return 2d parameters string
			 */	
			std::string get2dParametersString();

			/**
			 * @fn void setSpecParametersString( const char* strParam )
			 * @brief sets spectrum parameters string
			 * @params[in] strParam parameters string
			 */	
			void setSpecParametersString( const char* strParam );

			/**
			 * @fn void setChromParametersString( const char* strParam )
			 * @brief sets chromatogram parameters string
			 * @params[in] strParam parameters string
			 */	
			void setChromParametersString( const char* strParam );

			/**
			 * @fn void set2dParametersString( const char* strParam )
			 * @brief sets 2d parameters string
			 * @param[in] strParam parameters string
			 */	
			void set2dParametersString( const char* strParam );
			
			/**
			 * @fn void getTargetSpectra( kome::objects::DataGroupNode* group )
			 * @brief gets target spectra
			 * @params[in] group group objects
			 */	
			void getTargetSpectra( kome::objects::DataGroupNode* group );
		};
	}
}

#endif	// __KOME_LABELING_PEAK_DETECT_OPERATION_H__
