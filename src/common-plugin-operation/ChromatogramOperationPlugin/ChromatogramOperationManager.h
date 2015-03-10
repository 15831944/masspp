/**
 * @file ChromatogramOperationManager.h
 * @brief interfaces of ChromatogramOperationManager class
 *
 * @author S.Tanaka
 * @date 2007.04.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OPERATION_CHROMATOGRAM_MANAGER_H__
#define __KOME_OPERATION_CHROMATOGRAM_MANAGER_H__


#include <map>
#include <set>


#define CHROM_TOLERANCE_PARAM_NAME		 "tolerance"
#define CHROM_INTERVAL_PARAM_NAME		 "interval"

#define CHROM_RANGE_PARAM_NAME			 "range"

namespace kome {
	namespace operation {
		namespace chrom {

			/**
			 * @class ChromatogramOperationManager
			 * @brief chromatogram operation management class
			 */
			class ChromatogramOperationManager : public kome::objects::DefaultDataManager {
			protected:
				/**
				 * @fn ChromatogramOperationManager()
				 * @brief constructor
				 */
				ChromatogramOperationManager();

				/**
				 * @fn virtual ~ChromatogramOperationManager()
				 * @brief destructor
				 */
				virtual ~ChromatogramOperationManager();

			protected:
				/** chromatogram filter */
				unsigned char m_filter;

				/** chromatogram set */
				std::set< kome::objects::Chromatogram* > m_chromSet;

			public:
				/**
				 * @fn void setFilter( const unsigned char filter )
				 * @brief sets chromatogram filter
				 * @param filter chromatogram filter
				 */
				void setFilter( const unsigned char filter );

				/**
				 * @fn unsigned char getFilter()
				 * @brief gets chromatogram filter
				 * @return chromatogram filer
				 */
				unsigned char getFilter();

			protected:
				/**
				 * @fn void onUpdateFilter()
				 * @brief This method is called when the chromatogram filter is updated
				 */
				void onUpdateFilter();

				/**
				 * @fn void updateChromSet()
				 * @brief updates chromatogram set
				 */
				void updateChromSet();

			public:
				/**
				 * @fn kome::objects::Chromatogram* createMC(
						kome::objects::DataGroupNode* group,
						const double startMz,
						const double endMz
					)
				 * @brief creates MC object
				 * @param group source group
				 * @param startMz start m/z
				 * @param endMz end m/z
				 * @return chromatogram object
				 */
				kome::objects::Chromatogram* createMC(
					kome::objects::DataGroupNode* group,
					const double startMz,
					const double endMz
				);


				kome::objects::Chromatogram* createMCGroup(
					kome::objects::DataGroupNode* group,
					const double startMz,
					const double endMz
				);

				/**
				 * @fn kome::objects::Chromatogram* createBPC(
						kome::objects::DataGroupNode* group,
						const double startMz,
						const double endMz
					)
				 * @brief creates BPC object
				 * @param group source group
				 * @param startMz start m/z
				 * @param endMz end m/z
				 * @return chromatogram object
				 */
				kome::objects::Chromatogram* createBPC(
					kome::objects::DataGroupNode* group,
					const double startMz,
					const double endMz
				);

				// create BPC Group
				kome::objects::Chromatogram* createBPCGroup(
					kome::objects::DataGroupNode* group,
					const double startMz,
					const double endMz
				);

				/**
				 * @fn kome::objects::Chromatogram* createProcessedMC(
				 		kome::objects::DataGroupNode* group,
						const double startMz,
						const double endMz
					)
				 * @brief creates Processed MC object
				 * @param group source group
				 * @param startMz start m/z
				 * @param endMz end m/z
				 * @return chromatogram object
				 */
				kome::objects::Chromatogram* createProcessedMC(
					kome::objects::DataGroupNode* group,
					const double startMz,
					const double endMz
				);

			public:
				/**
				 * @fn bool openAppendingChromMCDialog( kome::objects::SettingParameterValues& settings )
				 * @brief open appending chrom mc dialog
				 * @param settings the object to store parameters
				 * @return If true, the OK button is clicked.
				 */
				bool openAppendingChromMCDialog( kome::objects::SettingParameterValues& settings );	// 2010/08/30 APPEND OKADA
				
				/**
				 * @fn bool openAppendingChromBPCDialog( kome::objects::SettingParameterValues& settings )
				 * @brief open appending chrom bpc dialog
				 * @param settings the object to store parameters
				 * @return If true, the OK button is clicked.
				 */
				bool openAppendingChromBPCDialog( kome::objects::SettingParameterValues& settings );// 2010/08/30 APPEND OKADA

				/**
				 * @fn bool openAppendingChromMcBpcDialog( kome::objects::SettingParameterValues& settings )
				 * @brief open appending chrom mc bpc dialog
				 * @param settings the object to store parameters
				 * @return If true, the OK button is clicked.
				 */
				bool ChromatogramOperationManager::openAppendingChromMcBpcDialog( kome::objects::SettingParameterValues& settings, int nTitleKind );	// 2010/09/03 APPEND OKADA

				/**
				 * @fn void addChromNodes(
						kome::objects::DataGroupNode& group,
						const double tol,
						const double interval,
						const bool bpc
					)
				 * @brief adds chromatogram nodes
				 * @param group group
				 * @param filter stage filter
				 * @param tol m/z tolerance
				 * @param interval m/z interval
				 * @param bpc flag
				 */
				void addChromNodes(
					kome::objects::DataGroupNode& group,
					const double tol,
					const double interval,
					const bool bpc
				);

			protected:
				/**
				 * @fn kome::objects::SpectraChromatogram* createChromatogram(
						kome::objects::DataGroupNode* group,
						const double startMz,
						const double endMz
					)
				 * @brief creates chromatogram object
				 * @param group source group
				 * @param startMz start m/z
				 * @param endMz end m/z
				 * @parma bProcess true: processed MC
				 * @return chromatogram object
				 */
				kome::objects::SpectraChromatogram* createChromatogram(
					kome::objects::DataGroupNode* group,
					const double startMz,
					const double endMz,
					bool bProces = false 
				);

			// 2010/09/02 APPEND OKADA ------->
			protected:
				/** 
				 * @fn kome::objects::SpectraChromatogram* ChromatogramOperationManager::createChromatogramGroup(
							kome::objects::DataGroupNode* group,
							const double startMz,
							const double endMz,
							bool bProcess = false 
						)
				 * @brief create chromatogram group
				 * @param group
				 * @param startMz
				 * @param endMz
				 * @param bProcess
				 * @return SpectraChromatogram
				 */
				kome::objects::SpectraChromatogram* ChromatogramOperationManager::createChromatogramGroup(
					kome::objects::DataGroupNode* group,
					const double startMz,
					const double endMz,
					bool bProcess = false 
				);
			// 2010/09/02 APPEND OKADA <-------


			protected:
				/**
				 * @fn virtual void onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting )
				 * @brief This method is called when a chromatogram is closed. (override method)
				 * @param chrom chromatogram object to be closed
				 * @param[in] deleting If true, the specified object is being deleted now.
				 */
				virtual void onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting );

            public:
				//2012/02/24 add by t.okuno -> This plugin distributes 'MC' and 'BPC'.
				/**
				 * @fn void createGenerateChromatogram( kome::objects::DataGroupNode* group,
														const double startMz,
														const double endMz )
				 * @brief creates Generate Chromatogram dialog
				 * @param group source group
				 * @param startMz start m/z
				 * @param endMz end m/z
				 */
				void createGenerateChromatogram( kome::objects::DataGroupNode* group,
												 const double startMz,
												 const double endMz );

				
				//2012/02/24 add by t.okuno <- This plugin distributes 'MC' and 'BPC'.

			public:
				/**
				 * @fn static ChromatogramOperationManager& getInstance()
				 * @brief gets chromatogram operation management object
				 * @return chromatogram operation management object (This is the only object.)
				 */
				static ChromatogramOperationManager& getInstance();
			};
		}
	}
}

#endif		// __KOME_OPERATION_CHROMATOGRAM_MANAGER_H__
