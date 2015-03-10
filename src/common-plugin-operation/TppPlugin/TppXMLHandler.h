/**
* @file TppXMLHandler.h
* @brief interfaces of TppXMLHandler class
*
* @author N.Tashiro
* @date 2013.03.15
* 
* Copyright(C) 2013 Shimadzu. All rights reserved.
*/


#ifndef __KOME_TPP_XMLHANDLER_H__
#define __KOME_TPP_XMLHANDLER_H__

#include "boost/archive/binary_oarchive.hpp"
#include "boost/archive/binary_iarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"
#include <fstream>

namespace kome {
	namespace Tpp{
		class TppXMLHandler : public kome::xml::XmlHandler {

		public:
			//result type(X!Tandem or Mascot)
			enum ResultType 
			{
				Tandem,
				Mascot
			};

			//base struct
			struct T_base{
			private:
				friend class boost::serialization::access;
				template<class Archive> void serialize( Archive& ar, unsigned int ver )
				{
					try {
						static_cast<void>(ver);
						ar & m_sTagName;
						ar & m_nTagLayer;
						ar & m_mapAttr;
					} catch (exception &e) {
						;
					}
				}
			protected:
				//tag name
				std::string							m_sTagName;

				//tag layer number
				int									m_nTagLayer;

				//attributes map
				std::map<std::string, std::string>	m_mapAttr;

				//itelator of attributes map
				std::map<std::string, std::string>::iterator	m_it_mapAttr;
			public:

				/**
				 * @fn ~T_base()
				 * @brief destructor.
				 **/
				~T_base()
				{
				}

				/**
				 * @fn ~T_base()
				 * @brief constructor.
				 **/
				T_base(void)
				{

					m_sTagName = "";
					m_nTagLayer = 0;

					m_mapAttr.clear();	m_it_mapAttr = m_mapAttr.begin();
				}

				/**
				 * @fn void setTagLayer (int n)
				 * @brief set tag layer number
				 */
				void setTagLayer (int n){
					m_nTagLayer = n;
				}

				/**
				 * @fn int getTagLayer()
				 * @brief get tag layer number
				 * @return tag layer number
				 */
				int	getTagLayer (){
					return m_nTagLayer;	
				}	

				/**
				 * @fn int getTagLayer()
				 * @brief set tag name.
				 */
				void setTagName(std::string s){
					m_sTagName = s;
				}

				/**
				 * @fn std::string	getTagName()
				 * @brief get tag name.
				 * @return tag name
				 */
				std::string	getTagName(){
					return m_sTagName;
				}

				/**
				 * @fn void setAttr (kome::core::Properties& attrs)
				 * @brief Set the properties (attributes) obtained from XMLHandler
				 */
				void setAttr (kome::core::Properties& attrs)
				{
					for (int i = 0; i < attrs.getNumberOfProperties(); i++)
					{
						setAttrValue (attrs.getKey(i), attrs.getValue(i));
					}
				}

				/**
				 * @fn bool isAttr (std::string sAttr)
				 * @brief Checks if the contain the specified attribute.
				 * @return true if sAttr is attribute
				 */
				bool isAttr (std::string sAttr)
				{
					if (m_mapAttr.empty() == true || m_mapAttr.count(sAttr) <= 0)
						return false;
					return true;
				}


				/**
				 * @fn std::string getAttrValue (std::string sAttr)
				 * @brief Retrieves the value of the attribute with the specified
				 * @return the value of the attribute with the specified
				 */
				std::string getAttrValue (std::string sAttr)
				{
					if (m_mapAttr.empty() == true || m_mapAttr.count(sAttr) <= 0)
						return "";
					return m_mapAttr[sAttr];
				}

				/**
				 * @fn void setAttrValue (std::string sAttr, std::string sValue)
				 * @brief Set the value of the attribute with the specified
				 */
				void setAttrValue (std::string sAttr, std::string sValue){
					m_mapAttr[sAttr] = sValue;	
				}

				/**
				 * @fn std::vector<std::string> getAttrList()
				 * @brief Gets a list of attributes
				 * @return list of attributes
				 */
				std::vector<std::string> getAttrList(){
					std::vector<std::string> sArray;
					for (m_it_mapAttr = m_mapAttr.begin(); m_it_mapAttr!= m_mapAttr.end(); m_it_mapAttr++)
					{
						std::string sKey = m_it_mapAttr->first;
						sArray.insert(sArray.end(), sKey);	
					}
					return sArray;
				}

			};

			// protein_summary>protein_group>protein>peptide
			#define TAG_peptide_parent_protein "peptide_parent_protein"
			struct T_peptide_parent_protein : T_base	{
			private:
				friend class boost::serialization::access;

				/**
				 * @fn template<class Archive> void serialize( Archive& ar, unsigned int ver )
				 * @brief Serialize T_base
				 **/
				template<class Archive> void serialize( Archive& ar, unsigned int ver )
				{
					{	ar & m_mapAttr;		ar & m_sTagName;	ar & m_nTagLayer;	}
				}
			};

			// protein_summary>protein_group>protein>
			#define TAG_peptide "peptide"
			struct T_peptide : T_base
			{
			private:
				friend class boost::serialization::access;
				template<class Archive>
				
				/**
				 * @fn void serialize( Archive& ar, unsigned int ver )
				 * @brief Serialize T_base
				 **/
				void serialize( Archive& ar, unsigned int ver )
				{
					{	ar & m_mapAttr;		ar & m_sTagName;	ar & m_nTagLayer;	}
					ar & m_ar_peptide_parent_protein;
				}
			protected:
				//Parent protein sequence of the peptide
				std::vector<T_peptide_parent_protein> m_ar_peptide_parent_protein;

			public:

				/**
				 * @fn void insertPeptideParentProtein (T_peptide_parent_protein row)
				 * @brief insert peptide parent protein.
				 **/
				void insertPeptideParentProtein (T_peptide_parent_protein row){
					m_ar_peptide_parent_protein.insert (m_ar_peptide_parent_protein.end(), row);
				}

				/**
				 * @fn T_peptide_parent_protein& operator[](unsigned int index)
				 * @brief Serialize T_base
				 **/
				T_peptide_parent_protein& operator[](unsigned int index){
					return m_ar_peptide_parent_protein[index];
				}
				int			size()	{	return m_ar_peptide_parent_protein.size();		}
			} ;

			// protein_summary>protein_group>protein>
			// msms_pipeline_analysis>msms_run_summary>spectrum_query>search_result>search_hit>analysis_result>peptideprophet_result>search_score_summary
			#define TAG_parameter "parameter"
			struct T_parameter : T_base		{
			private:
				friend class boost::serialization::access;

				/**
				 * @fn template<class Archive> void serialize( Archive& ar, unsigned int ver )
				 * @brief Serialize T_base
				 **/
				template<class Archive> void serialize( Archive& ar, unsigned int ver )
				{
					{	ar & m_mapAttr;		ar & m_sTagName;	ar & m_nTagLayer;	}
				}
			};

			// protein_summary>protein_group>protein>
			// protein_summary>protein_group>protein>indistinguishable_protein
			#define TAG_annotation "annotation"
			struct T_annotation : T_base	{
			private:
				friend class boost::serialization::access;

				/**
				 * @fn template<class Archive> void serialize( Archive& ar, unsigned int ver )
				 * @brief Serialize T_base
				 **/
				template<class Archive> void serialize( Archive& ar, unsigned int ver )
				{
					{	ar & m_mapAttr;		ar & m_sTagName;	ar & m_nTagLayer;	}
				}
			};

			// protein_summary>protein_group>protein>
			#define TAG_indistinguishable_protein "indistinguishable_protein"
			struct T_indistinguishable_protein : T_base
			{
			private:
				friend class boost::serialization::access;
				
				/**
				 * @fn template<class Archive> void serialize( Archive& ar, unsigned int ver )
				 * @brief Serialize T_base
				 **/
				template<class Archive>	void serialize( Archive& ar, unsigned int ver )
				{
					{	ar & m_mapAttr;		ar & m_sTagName;	ar & m_nTagLayer;	}	// T_base要素をシリアライズ
					ar & m_ar_annotation;
				}
			protected:

				//annotations
				std::vector<T_annotation>					m_ar_annotation;

			public:

				/**
				 * @fn T_indistinguishable_protein(void)
				 * @brief constructor
				 **/
				T_indistinguishable_protein(void)
				{
					m_ar_annotation.clear();
				}

				/**
				 * @fn void insertAnnotation (T_annotation row)
				 * @brief insert annotation
				 **/
				void insertAnnotation (T_annotation row){
					m_ar_annotation.insert (m_ar_annotation.end(), row);
				}

				/**
				 * @fn T_annotation Annotation(int index)
				 * @brief get annotation of index
				 * @return annotation
				 **/
				T_annotation Annotation(int index){
					return m_ar_annotation[index];
				}

				/**
				 * @fn int sizeAnnotation()
				 * @brief get size of annotations
				 * @return size of annotations
				 **/
				int sizeAnnotation(){
					return m_ar_annotation.size();
				}

			} ;

			// protein_summary>protein_group>
			#define TAG_protein "protein"
			struct T_protein : T_base
			{
			private:
				friend class boost::serialization::access;

				/**
				 * @fn template<class Archive> void serialize( Archive& ar, unsigned int ver )
				 * @brief Serialize T_base
				 **/
				template<class Archive>	void serialize( Archive& ar, unsigned int ver )
				{
					try {
						{	ar & m_mapAttr;		ar & m_sTagName;	ar & m_nTagLayer;	}
						ar & m_ar_peptide;
						ar & m_ar_parameter;
						ar & m_ar_indistinguishable_protein;
						ar & m_ar_annotation;
					} catch (exception )
					{

					}
				}


			protected:

				// array of peptide
				std::vector<T_peptide>						m_ar_peptide;

				// array of parameter
				std::vector<T_parameter>					m_ar_parameter;

				// array of indistinguishable_protein
				std::vector<T_indistinguishable_protein>	m_ar_indistinguishable_protein;

				// array of annotation
				std::vector<T_annotation>					m_ar_annotation;

			public:
				/**
				 * @fn T_protein (void) : T_base()
				 * @brief constructor
				 **/
				T_protein (void) : T_base()
				{
					m_ar_peptide.clear();
					m_ar_parameter.clear();
					m_ar_indistinguishable_protein.clear();
					m_ar_annotation.clear();
				}

				/**
				 * @fn void insertPeptide (T_peptide row)
				 * @brief insert peptide
				 **/
				void insertPeptide (T_peptide row){
					m_ar_peptide.insert (m_ar_peptide.end(), row);
				}
				
				/**
				 * @fn void insertParameter(T_parameter row)
				 * @brief insert parameter
				 **/
				void insertParameter(T_parameter row) {
					m_ar_parameter.insert(m_ar_parameter.end(), row);
				}

				/**
				 * @fn void insertAnnotation(T_annotation row)
				 * @brief insert annotation
				 */
				void insertAnnotation (T_annotation row){
					m_ar_annotation.insert (m_ar_annotation.end(), row);
				}

				/**
				 * @fn T_parameter &Parameter(int index)
				 * @brief get parameter of index
				 * @return parameter of index
				 **/
				T_parameter &Parameter(int index){
					return m_ar_parameter[index];
				} 
				
				/**
				 * @fn int sizeParameter()
				 * @brief get size of parameter
				 * @return size of parameter
				 **/
				int sizeParameter(){
					return m_ar_parameter.size();
				}

				/**
				 * @fn T_annotation &Annotation (int index)
				 * @brief get annotation of index
				 * @return annotation of index
				 **/
				T_annotation &Annotation (int index){
					return m_ar_annotation[index];
				} 
			
				/**
				 * @fn int sizeAnnotation()
				 * @brief get size of annotation
				 * @return size of annotation
				 **/
				int	sizeAnnotation(){
					return m_ar_annotation.size();
				}

				/**
				 * @fn T_indistinguishable_protein	&IndistinguishableProteins(int index)
				 * @brief get indeistinguishable protein of index
				 * @return indeistinguishable protein of index
				 */
				T_indistinguishable_protein	&IndistinguishableProteins(int index) {
					return m_ar_indistinguishable_protein[index]; 
				}

				/**
				 * @fn int	sizeIndistinguishableProteins()
				 * @brief get size of indeistinguishable protein
				 * @return size of indeistinguishable protein
				 */
				int	sizeIndistinguishableProteins()	{
					return m_ar_indistinguishable_protein.size();
				}

				void	insertIndistinguishableProteins(T_indistinguishable_protein row) {
					m_ar_indistinguishable_protein.insert(m_ar_indistinguishable_protein.end(), row); } 

				T_peptide			&Peptide(int index)	{ return m_ar_peptide[index];	}
				int					sizePeptide()		{ return m_ar_peptide.size();	}


				// proteinタグのpercent_coverage属性を返す
				// modify specification
				// change tag from percent_coverage to pct_spectrum_ids
				double getPercentCovrage ()
				{
					double db = string2double(this->getAttrValue("pct_spectrum_ids"));
					return db;
				}

				// proteinタグのn_indistinguishable_proteins属性を返す
				// modify specification
				// change tag from n_indistinguishable_proteins to total_number_peptides
				int getIndentifiedPeptide()
				{
					int val = string2int(this->getAttrValue("total_number_peptides"));
					return val;
				}
				// proteinレコードのprobability属性を返す
				double getProteinProphetProbability()
				{
					double db = string2double(this->getAttrValue("probability"));
					return db;
				}

				// プロテインに含まれるペプチド構造体のポインタ配列を取得
				std::vector<T_peptide*> getPeptidePtrs ()
				{
					std::vector<T_peptide*>	p_ar;
					for (int nPeptideIndex = 0;  nPeptideIndex < sizePeptide(); nPeptideIndex++)
						p_ar.insert(p_ar.end(), &Peptide(nPeptideIndex));
					return p_ar;
				}
				// protein>peptideレコードに含まれるすべてのpeptide_sequenceを取得する
				std::vector<std::string> getPeptideNames ()
				{
					std::vector<std::string> s_ar;
					std::vector<T_peptide*> p_ar = getPeptidePtrs ();
					for (int i = 0; i < p_ar.size(); i++)
						s_ar.insert(s_ar.end(), p_ar[i]->getAttrValue("peptide_sequence"));
					return s_ar;
				}
				// proteinに含まれる全てのindistinguishable_proteinレコードのprotein_name名を返す
				std::vector<std::string> getIndistinguishableProteinsNames()
				{
					std::vector<std::string> ar_tmp;

					for (int nIndistinguishableProteinsIndex = 0; nIndistinguishableProteinsIndex < sizeIndistinguishableProteins (); 
						nIndistinguishableProteinsIndex ++)
					{

						T_indistinguishable_protein *indistinguishable_protein = 
							&IndistinguishableProteins(nIndistinguishableProteinsIndex);

						ar_tmp.insert(ar_tmp.end(), indistinguishable_protein->getAttrValue("protein_name"));
					}
					return ar_tmp;
				}
			} ;

			// protein_summary>
			#define TAG_protein_group "protein_group"
			struct T_protein_group : T_base
			{
			private:
				friend class boost::serialization::access;
				template<class Archive>
				void serialize( Archive& ar, unsigned int ver )
				{
					{	ar & m_mapAttr;		ar & m_sTagName;	ar & m_nTagLayer;	}	// T_base要素をシリアライズ
					ar & m_ar_protein;
				}
			protected:
				std::vector<T_protein>						m_ar_protein;						// T_protein配列
			public:
				T_protein_group (void) : T_base()
				{	m_ar_protein.clear();		}
				void	insertProtein (T_protein row)	{	m_ar_protein.insert (m_ar_protein.end(), row);	}

				T_protein	&Protein (int index)		{	return m_ar_protein[index];		}
				int			sizeProtein()				{	return m_ar_protein.size();		}


				std::vector<std::string> getPeptideSequence(int nProteinIndex)	{	return Protein(nProteinIndex).getPeptideNames();	}
				std::vector<std::vector<std::string>> getPeptideSequenceAll()
				{
					std::vector<std::vector<std::string>> arar_tmp;
					for (int nProteinIndex = 0; nProteinIndex < sizeProtein(); nProteinIndex++)
						arar_tmp.insert(arar_tmp.end(), getPeptideSequence (nProteinIndex));
					return arar_tmp;
				}

				// protein_group>proteinに含まれるprotein_name名を返す
				std::vector<std::string> getProteinNames()
				{
					std::vector<std::string> ar_tmp;
					for (int nProteinIndex = 0; nProteinIndex < sizeProtein(); nProteinIndex++)
					{
						T_protein* protein = &Protein(nProteinIndex);
						ar_tmp.insert(ar_tmp.end(), protein->getAttrValue("protein_name"));
					}
					return ar_tmp;
				}
			};

			// prot file table
			class T_prot
			{
			private:
				friend class boost::serialization::access;
				template<class Archive>
				void serialize( Archive& ar, unsigned int ver )
				{
					ar & m_ar_protein_group;
				}
			private:
				std::vector<T_protein_group> m_ar_protein_group;		// T_protein_group配列
			public:
				T_prot(void)	{	m_ar_protein_group.clear();		}
				void insertProteinGroup(T_protein_group row)	{	m_ar_protein_group.insert(m_ar_protein_group.end(), row);	}
				T_protein_group &ProteinGroup(int index)		{	return m_ar_protein_group[index];	}
				int			sizeProteinGroup()					{	return m_ar_protein_group.size();	}
			};

			// msms_pipeline_analysis>msms_run_summary>spectrum_query>search_result>search_hit>analysis_result>peptideprophet_result	
			#define TAG_search_score_summary "search_score_summary"
			struct  T_search_score_summary : T_base
			{
			private:
				friend class boost::serialization::access;
				template<class Archive>
				void serialize( Archive& ar, unsigned int ver )
				{
					{	ar & m_mapAttr;		ar & m_sTagName;	ar & m_nTagLayer;	}	// T_base要素をシリアライズ
					ar & m_ar_parameter;
				}
			protected:
				std::vector<T_parameter>		m_ar_parameter;					// T_parameter配列	
			public:
				T_search_score_summary (void)	{	m_ar_parameter.clear();		}
				void	insertParameter(T_parameter row)		{	m_ar_parameter.insert(m_ar_parameter.end(), row);	}
				T_parameter				&Parameter (int index)	{	return m_ar_parameter[index];	} 
				int						sizeParameter()			{	return m_ar_parameter.size();	}
			};

			// msms_pipeline_analysis>msms_run_summary>spectrum_query>search_result>search_hit>analysis_result
			#define TAG_peptideprophet_result "peptideprophet_result"
			struct  T_peptideprophet_result : T_base
			{
			private:
				friend class boost::serialization::access;
				template<class Archive>
				void serialize( Archive& ar, unsigned int ver )
				{
					{	ar & m_mapAttr;		ar & m_sTagName;	ar & m_nTagLayer;	}	// T_base要素をシリアライズ
					ar & m_ar_search_score_summary;
				}
			protected:
				std::vector<T_search_score_summary>		m_ar_search_score_summary;	// T_search_score_summary配列	
			public:
				T_peptideprophet_result (void) {	m_ar_search_score_summary.clear();		}

				void	insertSearchScoreSummary (T_search_score_summary row)	{	m_ar_search_score_summary.insert (m_ar_search_score_summary.end(), row);	}
				T_search_score_summary	&SearchScoreSummary (int index)		{	return m_ar_search_score_summary[index];	} 
				int						sizeSearchScoreSummary()				{	return m_ar_search_score_summary.size();	}

			};

			// msms_pipeline_analysis>msms_run_summary>spectrum_query>search_result>search_hit>analysis_result
			#define TAG_interprophet_result "interprophet_result"
			struct  T_interprophet_result: T_peptideprophet_result {};

			// msms_pipeline_analysis>msms_run_summary>spectrum_query>search_result>search_hit
			#define TAG_analysis_result "analysis_result"
			struct  T_analysis_result : T_base
			{
			private:
				friend class boost::serialization::access;
				template<class Archive>
				void serialize( Archive& ar, unsigned int ver )
				{
					{	ar & m_mapAttr;		ar & m_sTagName;	ar & m_nTagLayer;	}	// T_base要素をシリアライズ
					ar & m_ar_peptideprophet_result;
					ar & m_ar_interprophet_result;
				}

			protected:
				std::vector<T_peptideprophet_result>		m_ar_peptideprophet_result;	// T_peptideprophet_result配列	
				std::vector<T_interprophet_result>		m_ar_interprophet_result;	// T_interprophet_result配列	


			public:
				T_analysis_result(void) {	m_ar_peptideprophet_result.clear();		
				m_ar_interprophet_result.clear();
				}

				void	insertPeptideProphetResult (T_peptideprophet_result row)	{	m_ar_peptideprophet_result.insert (m_ar_peptideprophet_result.end(), row);	}
				T_peptideprophet_result	&PeptideProphetResult (int index)		{	return m_ar_peptideprophet_result[index];	} 
				int						sizePeptideProphetResult()				{	return m_ar_peptideprophet_result.size();	}

				void	insertInterProphetResult (T_interprophet_result row)	{
					m_ar_interprophet_result.insert (m_ar_interprophet_result.end(), row);	
					int size = m_ar_interprophet_result.size();	
					
				}
				T_interprophet_result	&InterProphetResult (int index)		{

					return m_ar_interprophet_result[index];	
				} 
				int						sizeInterProphetResult()				{

					int size = m_ar_interprophet_result.size();	
					return m_ar_interprophet_result.size();	

				}


			};

			// msms_pipeline_analysis>msms_run_summary>spectrum_query>search_result>search_hit>
			#define TAG_search_score "search_score"
			struct  T_search_score : T_base	{
			private:
				friend class boost::serialization::access;
				template<class Archive> void serialize( Archive& ar, unsigned int ver )
				{
					{	ar & m_mapAttr;		ar & m_sTagName;	ar & m_nTagLayer;	}	// T_base要素をシリアライズ
				}
			};

			// msms_pipeline_analysis>msms_run_summary>spectrum_query>search_result>
			#define TAG_search_hit "search_hit"
			struct  T_search_hit : T_base
			{
			private:
				friend class boost::serialization::access;
				template<class Archive>
				void serialize( Archive& ar, unsigned int ver )
				{
					{	ar & m_mapAttr;		ar & m_sTagName;	ar & m_nTagLayer;	}	// T_base要素をシリアライズ
					ar & m_ar_analysis_result;
					ar & m_ar_search_score;
					ar & SpectrumQuery;
				}

			protected:

				std::vector<T_analysis_result>			m_ar_analysis_result;		// T_analysis_result配列	
				std::vector<T_search_score>				m_ar_search_score;			// T_search_score配列
			
			public:

				// for debug
				std::string SpectrumQuery;	// search hitが属する spectrum queryタグの spectrum属性値が入る（search hit識別用）

				T_search_hit (void)		{
					m_ar_search_score.clear();	
					m_ar_analysis_result.clear();
				}
				void	insertSearchSore (T_search_score row)	{	m_ar_search_score.insert (m_ar_search_score.end(), row);	}
				T_search_score	&SearchSore (int index)		{	return m_ar_search_score[index];	}
				int				sizeSearchSore()			{	return m_ar_search_score.size();		}


				void	insertAnalysisResult(T_analysis_result row)			{	m_ar_analysis_result.insert (m_ar_analysis_result.end(), row);	}
				T_analysis_result	&AnalysisResult (int index)		{	return m_ar_analysis_result[index];	} 
				int					sizeAnalysisResult()				{	return m_ar_analysis_result.size();	}

				// peptide名を返す（前後も文字も付けておく）
				std::string getPeptideName ()
				{
					return getAttrValue("peptide_prev_aa") + "_" + getAttrValue("peptide") +
						"_" + getAttrValue("peptide_next_aa");
				}

				int getSearchSoreIndexMascot ()
				{
					for (int i = 0; i < sizeSearchSore(); i++)
						if (strcmp(SearchSore(i).getAttrValue("name").c_str(), "ionscore" ) == 0)
							return i;
					return -1;
				}

				int getSearchSoreIndexTandem ()
				{
					for (int i = 0; i < sizeSearchSore(); i++)
						if (strcmp(SearchSore(i).getAttrValue("name").c_str(), "hyperscore" ) == 0)
							return i;
					return -1;
				}

				bool isMascot(std::string sSearchScoreName)
				{
					return strcmp(getSearchScoreName ().c_str(),"ionscore") == 0 ? true : false;
				}

				bool isTandem(std::string sSearchScoreName)
				{
					return strcmp(getSearchScoreName ().c_str(),"hyperscore") == 0 ? true : false;
				}

				std::string getSearchScoreName ()
				{
					std::string sRet = "";
					for (int i = 0; i < sizeSearchSore(); i++)
					{
						std::string sNameValue = SearchSore(i).getAttrValue("name");
						if (strcmp(sNameValue.c_str(), "hyperscore" ) == 0 || 
							strcmp(sNameValue.c_str(), "ionscore" ) == 0 )
						{
							sRet=  sNameValue;
							break;
						}
					}
					return sRet;
				}

				bool isTandem()	{	return getSearchSoreIndexTandem () == -1 ? false : true;	}

				bool isMascot()	{	return getSearchSoreIndexMascot () == -1 ? false : true;	}

				double getPeptideProphetMascotProbability ()
				{
					double dbRet = 0.0;
					if (sizeAnalysisResult() > 0 && AnalysisResult(0).sizePeptideProphetResult() > 0 && isMascot() == true)
					{
						dbRet =string2double(AnalysisResult(0).
							PeptideProphetResult(0).getAttrValue("probability"));
					}
					return dbRet;
				}

				double getPeptideProphetTandemProbability ()
				{
					double dbRet = 0.0;
					for (int nAnalysisResultIndex = 0; nAnalysisResultIndex  < sizeAnalysisResult(); nAnalysisResultIndex ++)
					{
						if (strcmp(AnalysisResult(nAnalysisResultIndex ).getAttrValue("analysis").c_str() , "peptideprophet") == 0)
						{
							dbRet = string2double(AnalysisResult(nAnalysisResultIndex ).
								PeptideProphetResult(0).getAttrValue("probability"));
						}
					}
					return dbRet;
				}

				double getInterProphetResultProbability ()
				{
					double dbRet = 0.0;

					for (int nAnalysisResultIndex  = 0; nAnalysisResultIndex  < sizeAnalysisResult(); nAnalysisResultIndex ++)
					{
						if (strcmp(AnalysisResult(nAnalysisResultIndex ).getAttrValue("analysis").c_str() , "interprophet") == 0)
						{
							dbRet = string2double(AnalysisResult(nAnalysisResultIndex ).
								InterProphetResult(0).getAttrValue("probability"));
						}
					}

					return dbRet;

				}

				double getMascotScore()
				{
					double dbRet = 0;
					for (int i = 0; i < sizeSearchSore(); i++)	// nameが"iconscore"である行の "value"値を探す
					{
						if (strcmp ("ionscore", SearchSore(i).getAttrValue("name").c_str()) == 0)
							dbRet = string2double(SearchSore(i).getAttrValue("value"));
					}
					return dbRet;		
				}

				double getTandemScore()
				{
					double dbRet = 0;
					for (int i = 0; i < sizeSearchSore(); i++)	// nameが"expect"である行の "value"値を探す
					{
						if (strcmp ("expect", SearchSore(i).getAttrValue("name").c_str()) == 0)
							dbRet = string2double(SearchSore(i).getAttrValue("value"));
					}
					return dbRet;		
				}

			};

			// msms_pipeline_analysis>msms_run_summary>spectrum_query
			#define TAG_search_result "search_result"

			struct  T_search_result : T_base
			{
			private:
				friend class boost::serialization::access;
				template<class Archive>
				void serialize( Archive& ar, unsigned int ver )
				{
					{	ar & m_mapAttr;		ar & m_sTagName;	ar & m_nTagLayer;	}	// T_base要素をシリアライズ
					ar & m_ar_search_hit;
				}
			protected:
				std::vector<T_search_hit> m_ar_search_hit;		// T_search_hit配列
			public:
				T_search_result (void)	{	m_ar_search_hit.clear();	}

				void	insertSearchHit (T_search_hit row)	{	m_ar_search_hit.insert (m_ar_search_hit.end(), row);	}
				T_search_hit	&SearchHit (int index)		{	return m_ar_search_hit[index];	}
				int				sizeSearchHit()				{	return m_ar_search_hit.size();		}
			};

			// msms_pipeline_analysis>msms_run_summary
			#define TAG_spectrum_query "spectrum_query"
			struct  T_spectrum_query : T_base
			{
			private:
				friend class boost::serialization::access;
				template<class Archive>
				void serialize( Archive& ar, unsigned int ver )
				{
					{	ar & m_mapAttr;		ar & m_sTagName;	ar & m_nTagLayer;	}	// T_base要素をシリアライズ
					ar & m_ar_search_result;
				}
			protected:
				std::vector<T_search_result> m_ar_search_result;		// T_search_result配列
			public:
				T_spectrum_query (void)	{	m_ar_search_result.clear();		}
				void insertSearchResult(T_search_result row)	{	m_ar_search_result.insert(m_ar_search_result.end(), row);	}
				T_search_result &SearchResult(int index)		{	return m_ar_search_result[index];	}
				int			sizeSearchResult()					{	return m_ar_search_result.size();	}
			};

			// msms_pipeline_analysis>
			#define TAG_msms_run_summary "msms_run_summary"
			struct  T_msms_run_summary : T_base	 
			{
			private:
				friend class boost::serialization::access;
				template<class Archive>
				void serialize( Archive& ar, unsigned int ver )
				{
					{	ar & m_mapAttr;		ar & m_sTagName;	ar & m_nTagLayer;	}	// T_base要素をシリアライズ
					ar & m_ar_spectrum_query;
				}
			protected:
				std::vector<T_spectrum_query> m_ar_spectrum_query;	// T_spectrum_query配列
			public:
				T_msms_run_summary (void)	{	m_ar_spectrum_query.clear();	}

				void insertSpectrumQuery(T_spectrum_query row)	{	m_ar_spectrum_query.insert(m_ar_spectrum_query.end(), row);	}
				T_spectrum_query &SpectrumQuery(int index)		{	return m_ar_spectrum_query[index];	}
				int			sizeSpectrumQuery()					{	return m_ar_spectrum_query.size();	}

			};

			// pep file table
			struct T_pep 
			{
			private:
				friend class boost::serialization::access;
				template<class Archive>
				void serialize( Archive& ar, unsigned int ver )
				{
					ar & m_ar_msms_run_summary;
				}
			protected:
				std::vector<T_msms_run_summary> m_ar_msms_run_summary ;		// T_msms_run_summary配列
			public:
				T_pep(void)	{	m_ar_msms_run_summary.clear();	}

				void insertMSMSRunSummary(T_msms_run_summary row){	m_ar_msms_run_summary.insert(m_ar_msms_run_summary.end(), row);	}
				T_msms_run_summary	&MSMSRunSummary(int index)	{	return m_ar_msms_run_summary[index];	}
				int					sizeMSMSRunSummary()		{	return m_ar_msms_run_summary.size();	}


				// 指定した　peptide_sequenceが一致する search_hit を列挙する
				std::vector<T_search_hit*> getSearchHitPtr(std::vector<std::string> s_ar_peptide_sequence)
				{
					std::vector<T_search_hit*> ar_search_hit_ptr;
					ar_search_hit_ptr.clear();

					std::map<std::string, bool> mapSpectrumQueryCheck;

					// パラメータの数でループ
					for (int nPeptideSequenceIndex = 0; nPeptideSequenceIndex < s_ar_peptide_sequence.size(); nPeptideSequenceIndex++)
					{
						std::string s_peptide_sequence = s_ar_peptide_sequence [nPeptideSequenceIndex ];

						for (int i = 0; i < m_ar_msms_run_summary.size(); i++)
						{
							for (int j = 0; j < m_ar_msms_run_summary[i].sizeSpectrumQuery(); j++)
							{
								for (int k = 0; k < m_ar_msms_run_summary[i].SpectrumQuery(j).sizeSearchResult(); k++)
								{
									for (int l = 0; l < m_ar_msms_run_summary[i].SpectrumQuery(j).SearchResult(k).sizeSearchHit(); l++)
									{
										T_search_hit *p_search_hit = &m_ar_msms_run_summary[i].SpectrumQuery(j).SearchResult(k).SearchHit(l);

										if (strcmp( p_search_hit->getAttrValue("peptide").c_str() ,s_peptide_sequence.c_str()) == 0)
										{
											//同一の spectrum_queryのspectrumに対する、ペプチドの同定結果については、行を追加しない
											std::string s_check_key = p_search_hit->SpectrumQuery + " + " + s_peptide_sequence;	// クエリ名＋ペプチド名

											if (mapSpectrumQueryCheck.find(s_check_key ) == mapSpectrumQueryCheck.end())
											{
												//TRACE ("check insert %s \n", s_check_key.c_str());
												mapSpectrumQueryCheck[s_check_key ] = true;
												ar_search_hit_ptr.insert(ar_search_hit_ptr.end(), p_search_hit);
											}
										}
									}
								}
							}
						}
					}
					return ar_search_hit_ptr;
				}

			};

			// ProphetDetailResult
			struct T_ProphetDetailResult {
			private:
				friend class boost::serialization::access;
				template<class Archive>
				void serialize( Archive& ar, unsigned int ver )
				{
					ar & type;
					ar & Probability;
					ar & Score;
					ar & PeptideSequence;
					ar & InterProphetProbability;

					ar & SpectrumQuery;
				}
			protected:
			public:
				ResultType	type;
				double		Probability;
				double		Score;
				std::string		PeptideSequence;
				double		InterProphetProbability;

				// for debug
				std::string		SpectrumQuery;

				T_ProphetDetailResult (void)
				{
				}
			};

			// Fasta
			struct T_fasta {
			protected:
				std::map<std::string, int> m_map_fasta_index;

				std::string		m_sFastaFileName;
			public:
				T_fasta (std::string sFastaFileName)
				{
					m_map_fasta_index.clear();
					m_sFastaFileName = sFastaFileName;
				}

				void makeIndexFile (std::string sFastaFileName, std::string sFastaIndexFile)
				{
					m_map_fasta_index.clear();
					std::ifstream inFastaIndex(sFastaFileName, std::ios::binary);
					std::string sLineTemp = "";

					bool bEof = false;

					int cnt = 0;
					while (!bEof)
					{

						std::string sBuf = "";

						std::ifstream::pos_type pos = inFastaIndex.tellg();
						std::getline(inFastaIndex, sBuf);
						std::ifstream::pos_type pos2 = inFastaIndex.tellg();

						bEof = inFastaIndex.eof();
						if (sBuf.size() > 0 &&  sBuf[0] == '>' )
						{
							std::list<std::string> lstBuf = split (sBuf, " ");
							if (lstBuf.size() > 0)
							{
								std::string sKey = lstBuf.front();

								if (sKey.size() > 0)
								{
									sKey = split(sKey, "|").back();
//									sKey = sKey.substr(1);
									m_map_fasta_index[sKey] = (int)pos;
									cnt ++;
								}
							}
						}
					}
					inFastaIndex.close();

					std::ofstream out(sFastaIndexFile, std::ios::binary);
					boost::archive::binary_oarchive oa(out);
					oa <<  m_map_fasta_index;
					out.close();
					;
				}

				struct T_char {
				public:
					RGBQUAD		col;
					std::string		c;
				};
				// getSequence で取得されたシーケンスに、s_ar_peptide_keyで指定するキーワード文字部分に着色を行い html文字で返す
				std::vector<std::string> formatHtmlSequence (std::vector<std::string> s_ar_Sequence, std::vector<std::string> s_ar_peptide_key)
				{
					std::vector<T_char> ar_all;

					std::string sAll = "";
					for (int i = 1; i < s_ar_Sequence.size(); i++)
						sAll+= s_ar_Sequence[i];

					std::string sHtml = "";

					int nMatchCharCount = 0;

					std::string sCR = "";

					std::vector<std::string> s_ar_Result;

					// シーケンス文字列の文字数でループ（総当たり）
					for (int i = 0; i < sAll.size(); )	
					{
						bool bFound =false;
						// マッチングさせたいキーの数でループ
						for (int nKeyIndex = 0; nKeyIndex < s_ar_peptide_key.size(); nKeyIndex++)
						{
							std::string sKey = s_ar_peptide_key[nKeyIndex];
							int nKeyLen = sKey.size();
							std::string sTmp = sAll.substr(i, nKeyLen);

							if (strcmp(sTmp.c_str(), sKey.c_str()) == 0)	// キーワードが一致
							{
								for (int j = 0; j < sTmp.size() ; j++)
								{
									T_char c;
									c.col.rgbRed = 255; c.col.rgbGreen = 0; c.col.rgbBlue = 0; c.col.rgbReserved = 0;  
									c.c = sTmp.substr(j, 1);
									ar_all.push_back(c);
								}
								i+= nKeyLen;
								nMatchCharCount+= nKeyLen;
								bFound = true;
								break;
								;
							}
						}
						if (bFound == false) 
						{
							T_char c;
							c.col.rgbRed = 0; c.col.rgbGreen = 0; c.col.rgbBlue = 0; c.col.rgbReserved = 0;  
							c.c = sAll.substr(i,1);
							ar_all.push_back(c);
							i++;
						}
					}

					// 上記の着直後のデータを元に、htmlを生成していく
					s_ar_Result.push_back("<table border=0 width=640 align=left>"+ sCR);

					for (int i = 0; i < ar_all.size(); )
					{
						std::string sTmp = "";

						sHtml+="<tr align=left>"+ sCR;

						// 行番号（一番左のカラム）
						{
							sTmp+="<td align=right>"+ sCR;
							sTmp+= FMT ("%d", i + 1);
							sTmp+="&nbsp;&nbsp;&nbsp;"+ sCR;	// スペースを入れる
							sTmp+="</td>"+ sCR;
						}
						for (int j = 0; j < 5; j++)	// 10文字ｘ5ブロック取り出す
						{
							int prev_col = -1;
							sTmp+="<td>"+ sCR;

							for (int k = 0; k < 10; k++)	// 10文字づつ取り出す
							{
								if (i >= ar_all.size())	//　データの最後に達したら抜ける
									break;
								T_char *c = &ar_all[i];

								int col = (c->col.rgbRed << 16) + (c->col.rgbGreen << 8) + c->col.rgbBlue ;

								bool b_col_change = false;
								if (prev_col != col)
								{
									sTmp+="</FONT>";
									prev_col = col;
									b_col_change  = true;
								}
								if (b_col_change == true)	// 直前の色と違っていたら、カラータグを入れる
									sTmp+= FMT("<FONT color=\\\"%X\\\">", col);

								sTmp+= c->c;
								i++;
							}
							sTmp+="</FONT>";
							sTmp+="</td>"+ sCR;
						}
						sHtml+= sTmp;
						sHtml+="</tr>"+ sCR;

						//2013/3/22 tashiro add ->
						s_ar_Result.push_back(sHtml);
						sHtml = "";
						// / 2013/3/22 tashiro add
					}

					{
						double dbCoverage = ((double)nMatchCharCount / (double)sAll.size()) * 100;

						sHtml+="<tr></tr>"+ sCR;
						sHtml+="<tr></tr>"+ sCR;
						sHtml+="<tr></tr>"+ sCR;

						sHtml+="<tr>" + sCR;
						sHtml+="<td align=right>Coverage=</td>" + sCR;
						sHtml+= FMT ("<td>%.0f%%</td>", dbCoverage);

						sHtml+="</tr>"+ sCR;
					}

					sHtml+= "</table>"+ sCR;
					//2013/3/22 tashiro add
					s_ar_Result.push_back(sHtml);
					sHtml = "";
					return s_ar_Result;
					// / 2013/3/22 tashiro add
				}

				// インデックス情報からprotein名に該当するfastaファイルのシーク位置を取得して
				// fastaファイルをシークさせて、そこからシーケンス情報を取り出す
				std::vector<std::string> getSequence (std::string sProteinName)
				{
					std::vector<std::string> s_ar_tmp;

					// プロテイン名に対応するインデックス位置を取得する

					if (m_map_fasta_index.find( sProteinName) != m_map_fasta_index.end())	// データがあるとき
					{
						unsigned int seek_pos = m_map_fasta_index[sProteinName];
						std::ifstream inFasta(m_sFastaFileName, std::ios::binary);
						inFasta.seekg(seek_pos);

						bool bBegin = false;
						bool bEof = false;
						while (!bEof)
						{
							std::string sBuf = "";
							std::getline(inFasta , sBuf);
							bEof = inFasta.eof();
							if (sBuf.size() > 0 && sBuf[0] == '>' )
							{
								if (bBegin == true)	// 2件目に入ったら抜ける
									break;

								bBegin = true;
							}
							if (bBegin == true)	s_ar_tmp.push_back(sBuf);
						}
						inFasta.close();
						;
					}
					return s_ar_tmp;
				}

				bool isLoad()
				{
					return m_map_fasta_index.size() == 0 ? false : true;
				}

				void load(void)
				{
					// インデックスファイルのチェック

					std::string sFastaIndexFileName = m_sFastaFileName+ ".idx";


					std::ifstream inFastaIndex(sFastaIndexFileName, std::ios::binary);
					if (inFastaIndex.is_open() == false)		// インデックスファイルがなければ作成
					{
						this->makeIndexFile(m_sFastaFileName, sFastaIndexFileName);
					} else {
						m_map_fasta_index.clear();
						boost::archive::binary_iarchive ia(inFastaIndex);	// シリアライズされたfastaファイルを読み込む
						ia >> m_map_fasta_index;
						inFastaIndex.close();
					}
				}
			};

			// ProphetResult
			struct T_ProphetResult {
			private:
				friend class boost::serialization::access;
				template<class Archive>
				void serialize( Archive& ar, unsigned int ver )
				{

					ar & m_detailresult;

					ar & ProteinGroupNo;
					ar & Protein;
					ar & IndistinguishableProteins;
					ar & ProteinProphetProbability;
					ar & PercentCoverage;
					ar & IdentifiedPeptides;
				}

			protected:
				std::vector<T_ProphetDetailResult>	m_detailresult;

			public:

				void sortInterProphetProbability(void)	{
					std::sort(m_detailresult.begin(),m_detailresult.end(), &TppXMLHandler::compareInterProphetProbability);	
				}

				T_ProphetResult (void)
				{

				}
				double getMascotScore(void)
				{
					double db = 0;
					for (int i = 0; i < m_detailresult.size(); i++)
					{
						if (Detail(i).type == ResultType::Mascot)
							db+= Detail (i).Score;
					}
					return db;
				}
				double getTandemScore(void)
				{
					double db = 0;
					for (int i = 0; i < m_detailresult.size(); i++)
					{
						if (Detail(i).type == ResultType::Tandem)
							db+= Detail (i).Score;
					}
					return db;
				}

				void insertDetail(T_ProphetDetailResult row)	{	m_detailresult.insert(m_detailresult.end(), row);	}
				T_ProphetDetailResult	&Detail(int index)	{	return m_detailresult[index];	}
				int								sizeDetail()		{	return m_detailresult.size();	}
				void							clearDetail()		{	m_detailresult.clear();	}

				int				ProteinGroupNo;

				std::string			Protein;
				std::vector<std::string>	IndistinguishableProteins;	// protein_group>proteinに含まれるindistinguishable_protein全レコードのprotein_name属性

				double			ProteinProphetProbability;	
				double			PercentCoverage;		
				int				IdentifiedPeptides;		
			};

			
			struct T_ResultHistgram
			{
			};

			struct T_DetailResultHistgram
			{
				int						ProteinSeqNo;
				T_ProphetDetailResult	*DetailResult;	//	全てのdetailresultポインタ配列を作る
			};

		public:

			/**
			 * @fn TppXMLHandler()
			 * @brief constructor
			 */
			TppXMLHandler();

			/**
			 * @fn ~TppXMLHandler()
			 * @brief destructor
			 */
			~TppXMLHandler();

			/**
			 * @fn compareInterProphetProbability( 
			 *	kome::Tpp::TppXMLHandler::T_ProphetDetailResult& left,
			 *	kome::Tpp::TppXMLHandler::T_ProphetDetailResult& right);
			 * @brief compare 2 prophet detail result
			 * @return left > right
			 */
			static bool TppXMLHandler::compareInterProphetProbability( 
				kome::Tpp::TppXMLHandler::T_ProphetDetailResult& left,
				kome::Tpp::TppXMLHandler::T_ProphetDetailResult& right);

			static bool TppXMLHandler::compareProbability(
				kome::Tpp::TppXMLHandler::T_ProphetResult& left,
				kome::Tpp::TppXMLHandler::T_ProphetResult& right);

			static bool TppXMLHandler::comareDetailResultHisgam(
				TppXMLHandler::T_DetailResultHistgram& left,
				TppXMLHandler::T_DetailResultHistgram& right);

			/**
			 * @fn ToWideStr(const std::string &strMult)
			 * @brief std::string → std::wstring
			 * @return result translate
			 */
			static std::wstring ToWideStr(const std::string &strMult);

			/**
			 * @fn split(std::string str, std::string delim)
			 * @brief split selected list with delim
			 * @return splitted list
			 */
			static std::list<std::string> split(std::string str, std::string delim);

			/**
			 * @fn string2double(std::string s)
			 * @brief translate string to double
			 * @return result translate
			 */
			static double string2double(std::string s);

			/**
			 * @fn string2int(std::string s)
			 * @brief translate string to int
			 * @return result translate
			 */
			static int string2int(std::string s);

			/**
			 * @fn string2int(std::string s)
			 * @brief translate string to int
			 * @return result translate
			 */
			static std::string convertCSV (std::vector<std::string> string_ar, int fixed_len);

			static std::string convertCSV (std::vector<std::string> db_ar);

			static std::string convertCSV (std::vector<double> db_ar);

			static std::string convertCSV (std::vector<double> db_ar, int fixed_len);

			static std::string convertCSV (std::vector<int> int_ar);

			static std::string convertCSV (std::vector<int> int_ar, int fixed_len);

		protected:

			//current tag layer
			int m_nCurrentTagLayer;	

			/**
			 * @fn InitMember()
			 * @brief initialize members
			 */
			void InitMember();

		};
	}
}

#endif // __KOME_TPP_PEPXMLHANDLER_H__
