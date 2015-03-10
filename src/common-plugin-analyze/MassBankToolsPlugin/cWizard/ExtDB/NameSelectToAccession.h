/**
 * @file NameSelectToAccession.h
 * @brief 外部DBから名前検索した結果を選択するダイアログ 
 *
 * @author M.Fukuda
 * @date 2013.12.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

namespace kome {
	namespace massbank {

		struct Substance {
			std::string name;
			std::string db;
			std::string accession;
			bool onList;

			// constructor
			Substance(
				const std::string& name,
				const std::string& db,
				const std::string& acc)
				: name(name)
				, db(db)
				, accession(acc)
				, onList(false)
			{}
		};

		class NameSelectToAccessionDialog;


		// ------------------- *********
		//   CandidatesList
		// ------------------- *********
		class CandidatesList : public kome::window::GridListCtrl 
		{
			friend class NameSelectToAccessionDialog;

		public:
			// constructor
			CandidatesList(
				wxWindow* parent,
				std::vector<Substance>& substs);
			// destructor
			~CandidatesList();

		private:
			// Uncopyable
			CandidatesList(const CandidatesList&);
			CandidatesList operator=(const CandidatesList&); 

		private:
			// member variable
			std::vector<Substance>& substs_;

		protected:
			// virtual (overwide) functions
			virtual void onCreate();
			virtual std::string getString(const long long data, const int column) const;

		private:
			void reloadMyList();
			DECLARE_EVENT_TABLE()
		};

		// ------------------- *********
		//   Dialog
		// ------------------- *********
		class NameSelectToAccessionDialog : public kome::window::StandardDialog {
		public:
			// constructor
			NameSelectToAccessionDialog(
				wxWindow* parent,
				const std::string& keyword,
				const std::vector<std::string>& substs
				);
			// destructor
			~NameSelectToAccessionDialog();

		private:
			// Uncopyable
			NameSelectToAccessionDialog(const NameSelectToAccessionDialog&);
			NameSelectToAccessionDialog operator=(const NameSelectToAccessionDialog&); 

		private:
			// member variable
			const std::string keyword_;
			std::vector<Substance> substs_;
			CandidatesList* candlist_;
			long selectedItem_;

		protected:		
			// virtual (overwide) functions
			virtual wxSizer* createMainSizer();
			virtual bool TransferDataFromWindow();

		public:
			void doReloadMyLists();
			const bool getLinkLists(std::vector<std::pair<std::string, std::string>>& rst);
			std::string getAccession() const{return substs_[selectedItem_].accession;}
			std::string getDBName() const{return substs_[selectedItem_].db;}
			std::string getInfoName() const{return substs_[selectedItem_].name;}

		private:
			void onReloadMyLists();
			DECLARE_EVENT_TABLE();

		};
	}
}
