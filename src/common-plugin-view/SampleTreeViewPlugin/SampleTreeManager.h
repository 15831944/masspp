/**
 * @file SampleTreeManager.h
 * @brief interfaces of SampleTreeManager class
 *
 * @author S.Tanaka
 * @date 2008.01.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_RAW_DATA_TREE_MANAGER_H__
#define __KOME_VIEW_RAW_DATA_TREE_MANAGER_H__


#include <map>
#include <wx/treectrl.h>


namespace kome {
	namespace view {

		/**
		 * @class SampleTreeManager
		 * @brief sample tree control management class
		 */
		class SampleTreeManager : public kome::objects::DefaultDataManager {
		protected:
			/**
			 * @fn SampleTreeManager()
			 * @brief constructor
			 */
			SampleTreeManager();

			/**
			 * @fn virtual ~SampleTreeManager()
			 * @brief destructor
			 */
			virtual ~SampleTreeManager();

		protected:
			/** samples */
			std::vector< std::pair< kome::objects::Sample*, wxTreeCtrl* > > m_samples;

			/** group map */
			std::map< kome::objects::DataGroupNode*, wxTreeItemId > m_groupMap;

			/** note book  */
			wxNotebook* m_book;

			/** É^Éuêî */
			int m_page;

			/** tree action flag */
			bool m_treeActionFlg;

		public:
			/**
			 * @fn void setTreeActionFlag( const bool flg )
			 * @brief sets the tree action flag
			 * @param[in] flg tree action flag value
			 */
			void setTreeActionFlag( const bool flg );
			
		public:
			/**
			 * @fn void createTreeCtrl( kome::objects::Sample* sample )
			 * @brief creates tree ctrl
			 * @param[in] sample sample
			 */
			void createTreeCtrl( kome::objects::Sample* sample );

			/**
			 * @fn wxNotebook* getNoteBook()
			 * @brief gets notebook
			 * @return the pointer to notebook
			 */
			wxNotebook* getNoteBook();

			/**
			 * @fn void createNoteBook()
			 * @brief create NoteBook
			 */
			void createNoteBook();

			/**
			 * @fn bool hasView()
			 * @brief judges the tree view is created
			 * @return If true, the tree view is created
			 */
			bool hasView();

			/**
			 * @fn void releaseView()
			 * @brief releases tree view
			 */
			void releaseView();

			/**
			 * @fn void updateNodes( kome::objects::DataGroupNode* group )
			 * @brief updates nodes
			 * @param group group
			 */
			void updateNodes( kome::objects::DataGroupNode* group );

			/**
			 * @fn void updateSampleNode( kome::objects::Sample* sample )
			 * @brief updates sample node
			 * @param sample sample node
			 */
			void updateSampleNode( kome::objects::Sample* sample );

		public:
			/**
			 * @fn void selectSample( kome::objects::Sample* sample )
			 * @brief gets the sample
			 */
			void selectSample( kome::objects::Sample* sample );

			/**
			 * @fn kome::objects::Sample* getSample( const int idx )
			 * @brief gets the sample
			 * @param[in] idx index
			 * @return sample
			 */
			kome::objects::Sample* getSample( const int idx );

			/**
			 * @fn void selectSpectrum( kome::objects::Spectrum* spec )
			 * @brief selects the spectrum
			 * @param[in] spec selected spectrum
			 */
			void selectSpectrum( kome::objects::Spectrum* spec );

			/**
			 * @fn void selectChromatogram( kome::objects::Chromatogram* chrom )
			 * @brief selects the chromatogram
			 * @param[in] chrom selected chromatogram
			 */
			void selectChromatogram( kome::objects::Chromatogram* chrom );

		protected:
			/**
			 * @fn int getSampleIndex( kome::objects::Sample* sample )
			 * @brief gets the sample index
			 * @param[in] sample sample
			 * @return sample index
			 */
			int getSampleIndex( kome::objects::Sample* sample );

		public:
			/**
			 * @fn wxTreeCtrl* getTreeCtrl( kome::objects::Sample* sample )
			 * @brief gets the tree control
			 * @param[in] sample
			 * @return tree control
			 */
			wxTreeCtrl* getTreeCtrl( kome::objects::Sample* sample );

		protected:
			/**
			 * @fn wxTreeItemId addTreeItem( wxTreeCtrl* tree, kome::objects::DataGroupNode* group, wxTreeItemId& parentId )
			 * @brief adds tree item
			 * @param[in] tree tree control
			 * @param group parent spectrum group
			 * @param parentId parent tree item ID
			 */
			wxTreeItemId addTreeItem( wxTreeCtrl* tree, kome::objects::DataGroupNode* group, wxTreeItemId* parentId );

			/**
			 * @fn void addChildren( wxTreeCtrl* tree, kome::objects::DataGroupNode* group, wxTreeItemId& groupId )
			 * @brief adds child nodes to tree item
			 * @param[in] tree tree control
			 * @param group group
			 * @param groupId group tree item ID
			 */
			void addChildren( wxTreeCtrl* tree, kome::objects::DataGroupNode* group, wxTreeItemId& groupId );

		protected:
			/**
			 * @fn virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting )
			 * @brief This method is called when a sample is closed. (override method)
			 * @param sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting );

		public:
			/**
			 * @fn static SampleTreeManager& getInstance()
			 * @brief gets object
			 * @return SampleTreeManager object (This is the only object.)
			 */
			static SampleTreeManager& getInstance();
		};
	}
}

#endif	// __KOME_VIEW_RAW_DATA_TREE_MANAGER_H__
