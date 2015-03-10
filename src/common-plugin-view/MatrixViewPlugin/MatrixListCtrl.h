/**
 * @file MatrixListCtrl.h
 * @brief interfaces of MatrixListCtrl class
 *
 * @author S.Tanaka
 * @date 2008.02.26
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_MATRIX_LIST_CTRL_H__
#define __KOME_MATRIX_LIST_CTRL_H__


namespace kome {
	namespace matrix {
		namespace view {

			/**
			 * @class MatrixListCtrl
			 * @brief matrix result dialog class
			 */
			class MatrixListCtrl : public kome::window::GridListCtrl {
			public:
				/**
				 * @fn MatrixListCtrl(
						wxWindow* parent,
						const unsigned int width,
						const unsigned int height
					)
				* @brief constructor
				* @param parent parent window
				* @param showTrash If true, this list control shows peaks in the trash.
				* @param width widow width
				* @param height window height
				*/
				MatrixListCtrl(
					wxWindow* parent,
					const unsigned int width,
					const unsigned int height,
					kome::window::DialogProgress* progress
				);

				/**
				 * @fn virtual ~MatrixListCtrl()
				 * @brief destructor
				 */
				virtual ~MatrixListCtrl();

			protected:
				/** appended columns */
				std::vector< int > m_appendedCols;

				/** samples */
				std::vector< int > m_samples;

				/** RT column */
				int m_rtCol;

				/** m/z column */
				int m_mzCol;

				/** charge column */
				int m_chargeCol;

				/** substance column */
				int m_substanceCol;

				/** appended base column */
				int m_appendedCol;

				/** substance map */
				std::map< int, std::string > m_substanceMap;

				/** focused list attr */
				wxListItemAttr m_focusedAttr;

				/** unfocused list attr */
				wxListItemAttr m_normalAttr;

				kome::window::DialogProgress* m_pProgress;
			protected:
				/**
				 * @fn virtual void onCreate()
				 * @brief This method is called by create method (override method)
				 */
				virtual void onCreate();

				/**
				 * @fn virtual int getInt( const long long data, const int column ) const
				 * @brief gets integer value from data (override method)
				 * @param data data
				 * @param column column number
				 * @return integer value
				 */
				virtual int getInt( const long long data, const int column ) const;

				/**
				 * @fn virtual double getDouble( const long long data, const int column ) const
				 * @brief gets double value from data (override method)
				 * @param data data
				 * @param column column number
				 */
				virtual double getDouble( const long long data, const int column ) const;

				/**
				 * @fn virtual std::string getString( const long long data, const int column ) const
				 * @brief gets character string value (override method)
				 * @param data data
				 * @param column column number
				 */
				virtual std::string getString( const long long data, const int column ) const;

				/**
				 * @fn virtual wxMenu* createMenu()
				 * @brief creates menu (override method)
				 * @return created menu
				 */
				virtual wxMenu* createMenu();

				/**
				 * @fn virtual void onActivateData( const long long data )
				 * @brief This method is called when the data is activated (abstract method)
				 * @param[in] data activated data
				 */
				virtual void onActivateData( const long long data );

			protected:
				/**
				 * @fn void onSelectItem( wxListEvent& evt )
				 * @brief リストからアイテムを選択
				 * @param evt list event
				 */
				void onSelectItem( wxListEvent& evt );

				/**
				 * @fn void onDselectItem( wxListEvent& evt )
				 * @brief リストの選択解除
				 * @param evt list event
				 */
				void onDselectItem( wxListEvent& evt );

			public:
				/**
				 * @fn irtual wxListItemAttr* OnGetItemAttr(long item) const
				 * @brief gets list item attribute (Overridde method)
				 * @param[in] item item number
				 * @return list item attribute
				 */
				virtual wxListItemAttr* OnGetItemAttr( long item ) const;
				
			private:
				DECLARE_EVENT_TABLE();
			};
		}
	}
}


#endif		// __KOME_MATRIX_LIST_CTRL_H__
