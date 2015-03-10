/**
 * @file TppPepXMLHandler.cpp
 * @brief implements of TppPepXMLHandler class
 *
 * @author N.Tashiro
 * @date 2013.03.15
 * 
 * Copyright(C) 2013 Shimadzu. All rights reserved.
 */

#include "stdafx.h"
#include "TppPepXMLHandler.h"

using namespace kome::Tpp;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace kome::xml;

//constructor
TppPepXMLHandler::TppPepXMLHandler(T_pep *p){
	m_pep = p;
	InitMember();	
}

//destructor
TppPepXMLHandler::~TppPepXMLHandler(){

}

void TppPepXMLHandler::onStartDocument(){
	InitMember();
}

void TppPepXMLHandler::onStartElement(const char* name, kome::core::Properties& attrs){
			m_nCurrentTagLayer++;


		int msms_run_summary_index = m_pep->sizeMSMSRunSummary() - 1;

		int spectrum_query_index = msms_run_summary_index < 0 ? -1 : m_pep->MSMSRunSummary(
										msms_run_summary_index).sizeSpectrumQuery() - 1;

		int search_result_index = spectrum_query_index < 0 ? -1 : m_pep->MSMSRunSummary(
										msms_run_summary_index).SpectrumQuery(
											spectrum_query_index).sizeSearchResult() - 1;

		int search_hit_index = search_result_index < 0 ? -1 : m_pep->MSMSRunSummary(
										msms_run_summary_index).SpectrumQuery(
											spectrum_query_index).SearchResult(
												search_result_index).sizeSearchHit() - 1;

		int search_score_index = search_hit_index < 0 ? -1 : m_pep->MSMSRunSummary(
										msms_run_summary_index).SpectrumQuery(
											spectrum_query_index).SearchResult(
												search_result_index).SearchHit(
													search_hit_index).sizeSearchSore() - 1;

		int analysis_result_index = search_score_index < 0 ? -1 : m_pep->MSMSRunSummary(
										msms_run_summary_index).SpectrumQuery(
											spectrum_query_index).SearchResult(
												search_result_index).SearchHit(
													search_hit_index).sizeAnalysisResult() - 1;

		int peptideprophet_result_index = analysis_result_index < 0 ? -1 : m_pep->MSMSRunSummary(
										msms_run_summary_index).SpectrumQuery(
											spectrum_query_index).SearchResult(
												search_result_index).SearchHit(
													search_hit_index).AnalysisResult(
														analysis_result_index).sizePeptideProphetResult() - 1;
		
		int search_score_summary_index = peptideprophet_result_index< 0 ? -1 : m_pep->MSMSRunSummary(
										msms_run_summary_index).SpectrumQuery(
											spectrum_query_index).SearchResult(
												search_result_index).SearchHit(
													search_hit_index).AnalysisResult(
														analysis_result_index).PeptideProphetResult(
																peptideprophet_result_index).sizeSearchScoreSummary() - 1;
		// msms_run_summary tag
		if (strcmp (TAG_msms_run_summary, name) == 0)
		{
			T_msms_run_summary msms_run_summary;

			msms_run_summary.setTagLayer (m_nCurrentTagLayer);
			msms_run_summary.setTagName(name);
			msms_run_summary.setAttr(attrs);
			m_pep->insertMSMSRunSummary(msms_run_summary);
		}

		// spectrum_query tag
		if (strcmp (TAG_spectrum_query, name) == 0 && msms_run_summary_index >= 0)
		{
			T_spectrum_query spectrum_query;
			spectrum_query.setTagLayer (m_nCurrentTagLayer);
			spectrum_query.setTagName(name);
			spectrum_query.setAttr(attrs);

			// is under one of msms_run_summary tag?
			if (spectrum_query.getTagLayer() == m_pep->MSMSRunSummary(msms_run_summary_index).getTagLayer() + 1 )
				m_pep->MSMSRunSummary(msms_run_summary_index).insertSpectrumQuery(spectrum_query);
		}

		// search_result tag
		if (strcmp (TAG_search_result, name) == 0 && spectrum_query_index >= 0)
		{
			T_search_result search_result;
			search_result.setTagLayer (m_nCurrentTagLayer);
			search_result.setTagName(name);
			search_result.setAttr(attrs);

			m_pep->MSMSRunSummary(msms_run_summary_index).SpectrumQuery(
					spectrum_query_index).insertSearchResult(search_result);
		}

		// search_hit tag
		if (strcmp (TAG_search_hit, name) == 0 && search_result_index >= 0)
		{
			T_search_hit search_hit;

			search_hit.SpectrumQuery =m_pep->MSMSRunSummary(msms_run_summary_index).SpectrumQuery(
												spectrum_query_index).getAttrValue ("spectrum");

			search_hit.setTagLayer (m_nCurrentTagLayer);
			search_hit.setTagName(name);
			search_hit.setAttr(attrs);

			m_pep->MSMSRunSummary(msms_run_summary_index).SpectrumQuery(
					spectrum_query_index).SearchResult(search_result_index).insertSearchHit(search_hit);
		}
		// search_score tag
		if (strcmp (TAG_search_score, name) == 0 && search_hit_index >= 0)
		{
			T_search_score search_score;
			search_score.setTagLayer (m_nCurrentTagLayer);
			search_score.setTagName(name);
			search_score.setAttr(attrs);

			m_pep->MSMSRunSummary(msms_run_summary_index).SpectrumQuery(
					spectrum_query_index).SearchResult(search_result_index).SearchHit(
								search_hit_index).insertSearchSore(search_score);
		}

		// analysis_result tag
		if (strcmp (TAG_analysis_result, name) == 0 && search_hit_index >= 0)
		{
			T_analysis_result analysis_result;
			analysis_result.setTagLayer (m_nCurrentTagLayer);
			analysis_result.setTagName(name);
			analysis_result.setAttr(attrs);

			m_pep->MSMSRunSummary(msms_run_summary_index).SpectrumQuery(
					spectrum_query_index).SearchResult(search_result_index).SearchHit(
								search_hit_index).insertAnalysisResult(analysis_result);
		}
		// peptideprophet_result tag
		if (strcmp (TAG_peptideprophet_result, name) == 0 && analysis_result_index >= 0)
		{
			T_peptideprophet_result peptideprophet_result;
			peptideprophet_result.setTagLayer (m_nCurrentTagLayer);
			peptideprophet_result.setTagName(name);
			peptideprophet_result.setAttr(attrs);

			m_pep->MSMSRunSummary(msms_run_summary_index).SpectrumQuery(
					spectrum_query_index).SearchResult(search_result_index).SearchHit(
							search_hit_index).AnalysisResult(
									analysis_result_index).insertPeptideProphetResult(peptideprophet_result);
		}
		// interprophet_result tag
		if (strcmp (TAG_interprophet_result, name) == 0 && analysis_result_index >= 0)
		{
			T_interprophet_result interprophet_result;
			interprophet_result.setTagLayer (m_nCurrentTagLayer);
			interprophet_result.setTagName(name);
			interprophet_result.setAttr(attrs);

			m_pep->MSMSRunSummary(msms_run_summary_index).SpectrumQuery(
					spectrum_query_index).SearchResult(search_result_index).SearchHit(
							search_hit_index).AnalysisResult(
									analysis_result_index).insertInterProphetResult(interprophet_result);
		}

		// search_score_summary tag
		if (strcmp (TAG_search_score_summary, name) == 0 && peptideprophet_result_index >= 0)
		{
			T_search_score_summary search_score_summary;
			search_score_summary.setTagLayer (m_nCurrentTagLayer);
			search_score_summary.setTagName(name);
			search_score_summary.setAttr(attrs);

			m_pep->MSMSRunSummary(msms_run_summary_index).SpectrumQuery(
					spectrum_query_index).SearchResult(search_result_index).SearchHit(
								search_hit_index).AnalysisResult(analysis_result_index).PeptideProphetResult(
										peptideprophet_result_index).insertSearchScoreSummary(search_score_summary);
		}
		// parameter tag
		if (strcmp (TAG_parameter, name) == 0 && search_score_summary_index >= 0)
		{
			T_parameter parameter;
			parameter.setTagLayer (m_nCurrentTagLayer);
			parameter.setTagName(name);
			parameter.setAttr(attrs);

			m_pep->MSMSRunSummary(msms_run_summary_index).SpectrumQuery(
					spectrum_query_index).SearchResult(search_result_index).SearchHit(
								search_hit_index).AnalysisResult(analysis_result_index).PeptideProphetResult(
										peptideprophet_result_index).SearchScoreSummary(
											search_score_summary_index).insertParameter(parameter);
		}

}

void TppPepXMLHandler::onEndDocument(){

}

void TppPepXMLHandler::onEndElement(const char* name, const char* text){
	m_nCurrentTagLayer--;
}

