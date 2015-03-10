/**
 * @file GlycanAnalysisManager.h
 * @brief interfaces of GlycanAnalysisPlugin plug-in function
 *
 * @author N.Tashiro
 * @date 2012.04.02
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_GLYCAN_ANALYSIS_MANAGER_H__
#define __KOME_GLYCAN_ANALYSIS_MANAGER_H__

#include "GlycanAnalysisStructures.h"
#include "GlycanAnalysisDialog.h"
#include "GlycanAnalysisStructureDialog.h"
#include "stdafx.h"

namespace kome{
	namespace glycan{
		class GlycanAnalysisManager{
		protected:
			/**
			 * @fn GlycanManager()
			 * @brief constructor
			 */
			GlycanAnalysisManager();

			/**
			 * @fn virtual ~GlycanManager()
			 * @brief destructor
			 */
			virtual ~GlycanAnalysisManager();
			
		public:

			/**
			 * @fn bool openDialog( kome::objects::Parameters* params );
			 * @brief open glycan analysis dialog
			 * @param[in] parameters
			 * @return if failed to open, return false.
			 */
			bool openDialog( kome::objects::Parameters* params );
			
			/**
			 * @fn bool openDialog( kome::objects::Parameters* params );
			 * @brief open glycan analysis dialog
			 * @param[in] parameters
			 * @return if failed to open, return false.
			 */
			bool openStructureDialog( kome::objects::Parameters* params );

			/**
			 * @fn bool closeDialog()
			 * @brief close glycan analysis dialog
			 * @return return true only.
			 */
			bool closeDialog();

			/**
			 * @fn bool closeDialog( kome::objects::Parameters* params );
			 * @brief close glycan analysis dialog
			 * @param[in] parameters
			 * @return if failed to close, return false.
			 */
			bool closeDialog( kome::objects::Parameters* params );

			/**
			 * @fn bool closeStructureDialog()
			 * @brief close glycan structure dialog
			 * @return return true only.
			 */
			bool closeStructureDialog();

			/**
			 * @fn bool closeStructureDialog( kome::objects::Parameters* params );
			 * @brief close glycan structure dialog
			 * @param[in] parameters
			 * @return if failed to close, return false.
			 */
			bool closeStructureDialog( kome::objects::Parameters* params );

			/**
			 * @fn bool clearAnnotations()
			 * @brief clear annotations
			 */
			void clearAnnotations();

			/**
			 * @fn bool refreshSpectrum()
			 * @brief redraw the spectrum and annotations
			 */
			void refreshSpectrum();

			/**
			 * @fn bool isEnable()
			 * @brief check for can be analyzed
			 * @return if files to use for analysis is not found, return false until quit program
			 *         (until quit program)
			 */
			bool isEnable();

			/**
			 * @fn bool drawAnnotation(kome::objects::Parameters* params)
			 * @brief draw annotations on spectrum
			 * @param[in] parameters
			 * @return if failed to draw, return false
			 */
			bool drawAnnotation(kome::objects::Parameters* params);

			/**
			 * @fn static GlycanManager& getInstance()
			 * @brief gets glycan analysis management object.
			 * @return glycan analysis management object. (This is the only object.)
			 */
			static GlycanAnalysisManager& getInstance();
			
			/**
			 * @fn bool parseKCFFile(unsigned int unGNumber, GLYCAN_STRUCTURE* glycanStructure)
			 * @brief parse target KCF File
			 * @params[in] GNumber
			 * @params[out] Glycan Structure
			 * @return if failed, return false.
			 */
			bool parseKCFFile(unsigned int unGNumber, GLYCAN_STRUCTURE* glycanStructure);

			
			
			/**
			 * @fn bool getSpectrum()
			 * @brief return spectrum
			 * @return spectrum
			 */
			kome::objects::Spectrum* getSpectrum();

			/**
			 * @fn bool calculate(kome::objects::Peaks* peaks, GlycanParams* params)
			 * @brief calculate for glycan analysis
			 * @param[in] peaks
			 * @param[in] params for calculation
			 * @return if failed to calculate, return false
			 */
			bool calculate( kome::objects::Peaks* peaks, GlycanParams* params );

			/**
			 * @fn std::vector< NGlycan >* getGlycans()
			 * @brief return glycans
			 * @return glycans
			 */
			std::vector< NGlycan >* getGlycans();

			/**
			 * @fn std::vector< NGlycan >* getGlycans()
			 * @brief return glycans
			 * @param[out] parameters
			 * @return glycans
			 */
			void GlycanAnalysisManager::setParams( GlycanParams* params);

		private:
			/**
			 * @fn void drawAnnotationString(kome::img::Graphics* graphics,unsigned int unGlycanID,unsigned int unAnnotationID, unsigned int x, unsigned int y)
			 * @brief draw annotations (called by drawAnnotation() )
			 * @param[in] graphics
			 * @param[in] index of glycan
			 * @param[in] index of annotation
			 * @param[in] x Coord
			 * @param[in] y Coord
			 */
			void drawAnnotationString(kome::img::Graphics* graphics,unsigned int unGlycanID,unsigned int unAnnotationID, unsigned int x, unsigned int y);

			/**
			 * @fn void avoidAnnotation(unsigned int* x, unsigned int* y, unsigned int currentWidth, unsigned int nTextMainHeight)
			 * @brief calculate coord to draw annotation(called by drawAnnotationString() )
			 * @param[out] x Coord
			 * @param[out] y Coord
			 * @param[in] text width
			 * @param[in] text height
			 */
			void avoidAnnotation(unsigned int* x, unsigned int* y, unsigned int currentWidth, unsigned int nTextMainHeight, unsigned int unRoofHeight);

			double sumTree(std::vector<GLYCAN_NODE>* _gn, std::vector<GLYCAN_EDGE>* _ge, unsigned int _startID);
			bool peakCheck(kome::objects::Peaks* peaks, const double _d, const double _tol);
			double intensityCheck(kome::objects::Peaks* peaks, const double _d, const double _tol);
			void assignAnnotation(Annotation* _an, double _mz, int _nodeNum, double _x1, double _x2, double _x3, double _y1, double _y2, double _y3);
			bool parseModifiedGlycoCTFile(unsigned int _unGNumber);

// 2014/02/20 Add by nakaya The addition of a display item  ->
			/**
			 * @fn void SortDouble( std::vector< double >& SortTarget, bool bType, std::vector< int >& Index )
			 * @brief This method does sort double type.
			 * @param[in/out] sort target
			 * @param[in] true:Ascending order false:Descending order
			 * @param[out] sort index array of result
			 */
			void SortDouble( std::vector< double >& SortTarget, bool bType, std::vector< int >& Index );
// 2014/02/20 Add by nakaya The addition of a display item  <-

		private:

			/** enable to analysis */
			bool m_bEnable;
			
			/** analysis dialog */
			kome::glycan::GlycanAnalysisDialog* m_dlg;

			/** structure dialog */
			kome::glycan::GlycanAnalysisStructureDialog* m_structureDlg;

			/** labeling list */
			std::vector< GLYCAN_LABEL > m_labelList;

			/** anion list */
			std::vector< GLYCAN_ANION > m_anionList;

			/** parameters for analysis */
			GlycanParams m_params; 

			/** analysis results */
			std::vector< NGlycan >	nGlycans;

			/** current spectrum */
			kome::objects::Spectrum* m_spec;

			/** coord of annotations */
			std::vector< unsigned int > m_dXList;
			std::vector< unsigned int > m_dYList;

			/** width of annotations*/
			std::vector< unsigned int > m_dWidthList;
			
			/** canvas height */
			unsigned int m_canvasHeightRange;

			/** Annotation height*/
			unsigned int m_annotationHeight;
		};

	}
}

#endif	// __KOME_GLYCAN_ANALYSIS_MANAGER_H__