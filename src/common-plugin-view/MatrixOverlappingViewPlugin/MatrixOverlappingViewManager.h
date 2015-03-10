/**
 * @file MatrixOverlappingViewManager.h
 * @brief interfaces of MatrixOverlappingViewManager class
 * 
 * @author M.Izumi
 * @date 2012.10.16 
 *
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_MATRIX_OVERLAPPINGVIEW_MANAGER_H__
#define __KOME_MATRIX_OVERLAPPINGVIEW_MANAGER_H__

namespace kome {
	namespace matrix {
		namespace overlapping {
			
			class ProfilePanel;

			/**
			 * @class MatrixOverlappingViewManager
			 * @brief Overlapping View Manager
			 */
			class MatrixOverlappingViewManager {
			protected:
				/**
				 * @fn MatrixOverlappingViewManager()
				 * @brief constructor
				 */
				MatrixOverlappingViewManager();
				
				/**
				 * @fn virtual ~MatrixOverlappingViewManager()
				 * @brief destructor
				 */
				virtual ~MatrixOverlappingViewManager();

			protected:
				/** Profile Panel */
				ProfilePanel* m_panel;

			public:
				/**
				 * @fn bool isPanel()
				 * @brief パネルに値が入っているか
				 * @return true:値が入っている false:NULL
				 */
				bool isPanel();
				/**
				 * @fn void setPanel( ProfilePanel* panel )
				 * @brief set the ProfilePanel
				 * @param panel ProfilePanel dialog
				 */
				void setPanel( ProfilePanel* panel );

				/**
				 * @fn ProfilePanel* getPanel()
				 * @brief get the ProfilePanel
				 * @return ProfilePanel dialog
				 */
				ProfilePanel* getPanel();

				/**
				 * @fn void createPanel( wxWindow* parent, int peakId )
				 * @brief create Plot Panel
				 * @param parent parent window
				 * @param peakId peak id
				 */
				void createPanel( wxWindow* parent, int peakId );
			public:
				/**
				 * @fn static MatrixOverlappingViewManager& getInstance()
				 * @brief gets Matrix Overlapping View plug-in management object
				 * @return Matrix Overlapping View plug-in management object (This is the only object.)
				 */
				static MatrixOverlappingViewManager& getInstance();
			};
		}
	}
}

#endif // __KOME_MATRIX_OVERLAPPINGVIEW_MANAGER_H__
