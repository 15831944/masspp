/**
 * @file GlycanAnalysisStructureCanvas.h
 * @brief Canvas class of Glycan Structure
 *
 * @author N.Tashiro
 * @date 2012.04.02
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_GLYCAN_ANALYSIS_STRUCTURE_CANVAS_H__
#define __KOME_GLYCAN_ANALYSIS_STRUCTURE_CANVAS_H__

#include "stdafx.h"
#include "GlycanAnalysisStructures.h"

/** canvas size (width) */
#define GLYCAN_CANVAS_WIDTH		300

/** canvas size (height) */
#define GLYCAN_CANVAS_HEIGHT	300

// 2014/02/20 Add by nakaya The addition of a display item  ->
/** rank canvas size (width) */
#define RANK_CANVAS_WIDTH		80

/** rank canvas size (height) */
#define RANK_CANVAS_HEIGHT		GLYCAN_CANVAS_HEIGHT

/** assigned canvas size (width) */
#define ASSIGNED_CANVAS_WIDTH		150

/** assigned canvas size (height) */
#define ASSIGNED_CANVAS_HEIGHT		GLYCAN_CANVAS_HEIGHT

/** score canvas size (width) */
#define SCORE_CANVAS_WIDTH		120

/** score canvas size (height) */
#define SCORE_CANVAS_HEIGHT		GLYCAN_CANVAS_HEIGHT
// 214/02/19 Add by nakaya The addition of a display item  <-

using namespace kome::glycan;
using namespace kome::window;
using namespace kome::img;

namespace kome{
	namespace glycan{
		class GlycanAnalysisStructureCanvas : public CanvasBase {
			
		public:
			/**
			 * @fn GlycanAnalysisStructureCanvas(wxWindow* parent, GLYCAN_STRUCTURE* result, unsigned int focus, const char* label, bool bLinked, const char* anion, Annotation* annotation);
			 * @brief constructor
			 * @param[in,out] parent window
			 * @param[in] glycan structure address
			 * @param[in] targeting node id
			 * @param[in] label text
			 * @param[in] enable to display hyperlink
			 * @param[in] anion text
			 * @param[in] Annotation address
			 */
			GlycanAnalysisStructureCanvas(wxWindow* parent, GLYCAN_STRUCTURE* result, unsigned int focus, const char* label, bool bLinked, const char* anion, Annotation* annotation);

			/**
			 * @fn ~GlycanAnalysisStructureCanvas()
			 * @brief destructor
			 */
			~GlycanAnalysisStructureCanvas();

			/**
			 * @fn virtual void onDraw(wxDC& dc)
			 * @brief This method is called when draw event (override method)
			 * @param[in,out] dc device context
			 */
			virtual void onDraw(wxDC& dc);

			/**
			 * @fn onRightClick(wxMouseEvent& evt)
			 * @brief call the menu function of parent
			 */
			void onRightClick(wxMouseEvent& evt);

			/**
			 * @fn onLeftClick(wxMouseEvent& evt)
			 * @brief call the menu function of parent
			 */
			void onLeftClick(wxMouseEvent& evt);

			/**
			 * @fn drawStructure()
			 * @brief draw glycan structure
			 * @return return false if it failed to draw.
			 */
// 2014/02/24 Change by nakaya Change of a selective state ->
			//bool drawStructure();
			bool drawStructure(bool bSelect, bool bCreateBmp);
// 2014/02/24 Change by nakaya Change of a selective state <-

			/**
			 * @fn getGNumber()
			 * @return return GNumber text
			 */
			const char* getGNumber();

		private:
			/**
			 * @fn getRoot((std::vector<GLYCAN_NODE> *nodeList, std::vector<GLYCAN_EDGE> *edgeList)
			 * @brief pick nodes to display
			 * @return return false if it failed to pick nodes.
			 */
			bool getRoot(std::vector<GLYCAN_NODE> *nodeList, std::vector<GLYCAN_EDGE> *edgeList);

		public:
			/** bitmap for copy figure */
			wxBitmap m_bitmap;

// 2014/02/24 Add by nakaya Change of a selective state ->
			/** select flag */
			bool m_bSelect;
// 2014/02/24 Add by nakaya Change of a selective state <-

		private:
			/** parent window */
			wxWindow* m_parent;

			/** target glycan structure */
			GLYCAN_STRUCTURE m_structure;

			/** labeling parameter */
			std::string m_strLabel;

			/** anion parameter */
			std::string m_strAnion;

			/** target root node */
			unsigned int m_unFocus;

			/** enable to display GNumber */
			bool m_bDisplayGNumber;

			/** annotations */
			Annotation m_annotation;

// 2014/02/24 Add by nakaya Change of a selective state ->
			/** bmp create flag */
			bool m_bCreateBmp;
// 2014/02/24 Add by nakaya Change of a selective state <-

		private:
			DECLARE_EVENT_TABLE()
		};

// 2014/02/20 Add by nakaya The addition of a display item  ->
		class TextCanvas : public CanvasBase {
			
		public:
			/**
			 * @fn TextCanvas(wxWindow* parent, const char* label, const int width, const int height);
			 * @brief constructor
			 * @param[in,out] parent window
			 * @param[in] label text
			 * @param[in] canvas width
			 * @param[in] canvas height
			 */
			TextCanvas(wxWindow* parent, GLYCAN_STRUCTURE* result, const char* label, const int width, const int height);

			/**
			 * @fn ~TextCanvas()
			 * @brief destructor
			 */
			~TextCanvas();

			/**
			 * @fn virtual void onDraw(wxDC& dc)
			 * @brief This method is called when draw event (override method)
			 * @param[in,out] dc device context
			 */
			virtual void onDraw(wxDC& dc);

			/**
			 * @fn drawStructure()
			 * @brief draw glycan structure
			 * @return return false if it failed to draw.
			 */
			bool drawText(bool bSelect);

			/**
			 * @fn onRightClick(wxMouseEvent& evt)
			 * @brief call the menu function of parent
			 */
			void onRightClick(wxMouseEvent& evt);

			/**
			 * @fn onLeftClick(wxMouseEvent& evt)
			 * @brief call the menu function of parent
			 */
			void onLeftClick(wxMouseEvent& evt);
		public:
			/** select flag */
			bool m_bSelect;

		private:
			/** target glycan structure */
			GLYCAN_STRUCTURE m_structure;

			/** canvas width */
			int m_nCanvasWidth;

			/** canvas height */
			int m_nCanvasHeight;

			/** parent window */
			wxWindow* m_parent;

			/** labeling parameter */
			std::string m_strLabel;

		private:
			DECLARE_EVENT_TABLE()
		};
	}
// 2014/02/20 Add by nakaya The addition of a display item  <-
}

#endif	// __KOME_GLYCAN_ANALYSIS_STRUCTURE_CANVAS_H__
