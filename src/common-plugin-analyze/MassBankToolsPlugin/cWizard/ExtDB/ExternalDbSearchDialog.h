/**
 * @file .ExternalDbSearchDialog.h
 * @brief Extarnal Database Search Dialog
 *
 * @author M.Fukuda
 * @date 2013.12.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

namespace kome {
	namespace massbank {
		
		class SearchExternalDbCheckList : public kome::window::CheckGridListCtrl {
		public:
			// constructor
			SearchExternalDbCheckList(
				wxWindow* parent,
				const std::vector<kome::ident::ExternalDB*>& dbs);

			// destructor			
			~SearchExternalDbCheckList(void);

		private:
			// Uncopyable
			SearchExternalDbCheckList(const SearchExternalDbCheckList&);
			SearchExternalDbCheckList operator=(const SearchExternalDbCheckList&); 

		private:
			// member variable
			const std::vector<kome::ident::ExternalDB*>& dbs_;
			unsigned int keggIdx_;
			bool announcefirsttimeFg_;
		public:			
			kome::ident::ExternalDB* getExternalDB(const unsigned int idx) const;
			const bool getSelectedDB(std::vector<kome::ident::ExternalDB*>& refArr);

		protected:
			// virtual (overwide) functions
			virtual void onCreate();
			virtual std::string getString(const long long data, const int column) const;

			virtual void onSelectAll();
			virtual void onSelectItem(const int item);

		private:
			void onSetupCheckList(void);
			const bool showKEGGAccessConfirmation();

			void setParameterIni(const std::string& val);
			const std::string getParameterIni();


		private:
			DECLARE_EVENT_TABLE()
		};

		// ----------------------------------------------------------
		//
		// ---- DIALOG ----
		//
		// ----------------------------------------------------------

		class SearchExternalDbDialog : public kome::window::StandardDialog {

		public:
			// constructor
			SearchExternalDbDialog(
				wxWindow* parent,
				const std::vector<kome::ident::ExternalDB*>& dbs,
				const std::string& defKeyword);

			// destructor
			~SearchExternalDbDialog(void);

		private:
			// Uncopyable
			SearchExternalDbDialog(const SearchExternalDbDialog&);
			SearchExternalDbDialog operator=(const SearchExternalDbDialog&);

		private:
			// member variable
			SearchExternalDbCheckList* cklist_;
			wxTextCtrl* searchKey_;

			std::string searchKeyword_;
			const std::vector<kome::ident::ExternalDB*>& dbs_;
			std::vector<kome::ident::ExternalDB*> extDbs_;

		public:
			std::string getSearchKeyword() const {return searchKeyword_;}

			void getExtDbs(std::vector<kome::ident::ExternalDB*>& extDbs) {extDbs = extDbs_;}

		protected:
			// virtual (overwide) functions
			virtual bool onOK();
			virtual wxSizer* createMainSizer();
			virtual bool TransferDataFromWindow();

		private:
			void checkKEGGAccess();

		private:
			DECLARE_EVENT_TABLE();
		};

	}
}
