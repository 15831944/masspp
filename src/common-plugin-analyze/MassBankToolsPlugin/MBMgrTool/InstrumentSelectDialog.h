/**
 * @file InstrumentSelectDialog.h
 * @brief interfaces of MassBank Instrument Selecet Dialog
 *
 * @author M.Fukuda
 * @date 2011.08.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_MASBANK_INSTRUMEMT_SELECT_DIALOG_H__
#define __KOME_MASBANK_INSTRUMEMT_SELECT_DIALOG_H__

namespace kome {
	namespace massbank {

		class InstrumentCheckList;
		class InstrumentMgr;

		/**
		 * @class InstrumentSelectDialog
		 * @brief Instrument Type Select class
		 */
		class InstrumentSelectDialog : public kome::window::StandardDialog {

		public:
			// @brief constructor
			// @param[in] parent parent window
			InstrumentSelectDialog( wxWindow* parent );

			//@brief destructor
			virtual ~InstrumentSelectDialog();

		protected:
			// check List Pointer
			InstrumentCheckList* my_list;
			InstrumentMgr* m_instMgr;

		protected:
			// * @brief creates main sizer (override method)
			// * @return main sizer
			virtual wxSizer* createMainSizer();
			
			// * @brief on click Serach Button as OK.
			// * @return If 1 more than selected return True.
			virtual bool onOK();

		public:
			/**
			 * @brief get List Status (get checked item ids).
			 * @param[out] selected items ids
			 */
			void getMyListStatus(std::vector<long long>& selectDatas);

		private:
			DECLARE_EVENT_TABLE();
		};
		
		/**
		 * @class InstrumentCheckList
		 * @brief Instrument Type Check List Class
		 */
		class InstrumentCheckList : public kome::window::CheckGridListCtrl {
		public:
			
			InstrumentCheckList( wxWindow* parent, InstrumentMgr* m_instMgr);
			virtual ~InstrumentCheckList();

		private:
			// copy mgr's Instrument Vector Point
			std::vector< std::string > m_orgInstList;
			InstrumentMgr* m_instMgr;

		protected:
			virtual void onCreate();
			virtual std::string getString(const long long data, const int column) const;

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif // __KOME_MASBANK_INSTRUMEMT_SELECT_DIALOG_H__
