/**
 * @file AlignmentManager.h
 * @brief interfaces of AlignmentManager class
 *
 * @author S.Tanaka
 * @date 2007.08.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_ALIGNMENT_MANAGER_H__
#define __KOME_ALIGNMENT_MANAGER_H__


#include <stack>
#include <utility>


namespace kome {
	namespace alignment {

		/**
		 * @class AlignmentManager
		 * @brief alignment management class
		 */
		class AlignmentManager {
		protected:
			/**
			 * @fn AlignmentManager()
			 * @brief constructor
			 */
			AlignmentManager();

			/**
			 * @fn virtual ~AlignmentManager()
			 * @brief destructor
			 */
			virtual ~AlignmentManager();

		protected:
			/** control chromatogram */
			kome::operation::TransformedChromatogram* m_ctrlChrom;

			/** result control chromatogram */
			kome::objects::Chromatogram* m_ctrlChromResult;

			/** treatment chromatogram */
			kome::operation::TransformedChromatogram* m_trmtChrom;

			/** result treatment chromatogram */
			kome::objects::Chromatogram* m_trmtChromResult;

			/** correction dummy chromatogram */
			kome::objects::Chromatogram* m_corrDummyChrom;

			/** alignment object */
			kome::operation::Alignment* m_alignment;

			/* raw control sample object */
			kome::objects::Sample* m_RawCtrlSample;     // @date 2013.05.13 <Add> Fujita 

			/* raw treatment sample object */
			kome::objects::Sample* m_RawTrmtSample;     // @date 2013.05.13 <Add> Fujita 

			/* control sample object */
			kome::objects::Sample* m_ScaledCtrlSample;     // @date 2013.05.13 <Add> Fujita 

			/* treamtmemt sample object */
			kome::objects::Sample* m_ScaledTrmtSample;     // @date 2013.05.13 <Add> Fujita 


			/** cursor position */
			int m_cursorPos;

			/** correlation points */
			kome::core::DataPoints m_corrPts;

			/** drag start position */
			int m_startPos;
			/** drag end position */
			int m_endPos;

			/** range stack */
			std::stack< std::pair< double, double > > m_rangeStack;

			/** raw control TICs **/
			std::vector< double > m_RawCtrlTics;				// @date 2013.05.14 <Add> Fujita 

			/** raw treatment TICs **/
			std::vector< double > m_RawTrmtTics;				// @date 2013.05.14 <Add> Fujita 

			/** flg whether m_RawCtrlTics and m_RawTrmtTics were filled **/
			int m_flg;											// @date 2013.05.27 <Add> Fujita



		public:
			/**
			 * @fn void clearChromatograms()
			 * @brief clears chromatogram
			 */
			void clearChromatograms();

			/**
			 * @fn kome::objects::Chromatogram* getControlChrom()
			 * @brief gets control chromatogram
			 * @return control chromatogram
			 */
			kome::objects::Chromatogram* getControlChrom();

			/**
			 * @fn kome::objects::Chromatogram* getResultControlChrom()
			 * @brief gets result control chromatogram
			 * @return result control chromatogram
			 */
			kome::objects::Chromatogram* getResultControlChrom();

			/**
			 * @fn kome::objects::Chromatogram* getTreatmentChrom()
			 * @brief gets treatment chromatogram
			 * @return treatment chromatogram
			 */
			kome::objects::Chromatogram* getTreatmentChrom();

			/**
			 * @fn kome::objects::Chromatogram* getResultTreatmentChrom()
			 * @brief gets result treatment chromatogram
			 * @return result treatment chromatogram
			 */
			kome::objects::Chromatogram* getResultTreatmentChrom();

			/**
			 * @fn void clearCorrection()
			 * @brief clears correction data
			 */
			void clearCorrection();

			/**
			 * @fn kome::objects::Chromatogram* getCorrectionDummyChrom()
			 * @brief gets dummy chromatogram object to draw correction graph
			 * @return dummy chromatogram to draw correction graph
			 */
			kome::objects::Chromatogram* getCorrectionDummyChrom();

		protected:
			/**
			 * @fn void unsetAlignment()
			 * @brief unsets alignment
			 */
			void unsetAlignment();

		public:
			/**
			 * @fn void selectCtrlChrom( kome::objects::Chromatogram* chrom )
			 * @breif selects control chromatogram
			 * @param chrom control chromatogram
			 */
			void selectCtrlChrom( kome::objects::Chromatogram* chrom );

			/**
			 * @fn void selectTrmtChrom( kome::objects::Chromatogram* chrom )
			 * @brief selects treatment chromatogram
			 * @param chrom treatment chromatogram
			 */
			void selectTrmtChrom( kome::objects::Chromatogram* chrom );

		public:
			/**
			 * @fn double getSrcMaxIntensity()
			 * @brief gets source max intensity
			 * @return max itnensity
			 */
			double getSrcMaxIntensity();

			/**
			 * @fn double getSrcMinRt()
			 * @brief gets source min RT
			 * @return min RT
			 */
			double getSrcMinRt();

			/**
			 * @fn double getSrcMaxRt()
			 * @brief gets source max RT
			 * @return max RT
			 */
			double getSrcMaxRt();

			/**
			 * @fn double getResultMaxIntensity()
			 * @brief gets result max intensity
			 * @return max itnensity
			 */
			double getResultMaxIntensity();

			/**
			 * @fn double getResultMinRt()
			 * @brief gets result min RT
			 * @return min RT
			 */
			double getResultMinRt();

			/**
			 * @fn double getResultMaxRt()
			 * @brief gets result max RT
			 * @return max RT
			 */
			double getResultMaxRt();

		public:
			/**
			 * @fn void setCursorPos( const int pos )
			 * @brief sets cursor position
			 * @param pos cursor position
			 */
			void setCursorPos( const int pos );

			/**
			 * @fn int getCursorPos()
			 * @brief gets corsor posotion
			 * @return cursor position
			 */
			int getCursorPos();

			/**
			 * @fn void setDragStartPos( const int pos )
			 * @brief sets drag start position
			 * @param pos drag start position
			 */
			void setDragStartPos( const int pos );

			/**
			 * @fn int getDragStartPos()
			 * @brief gets drag start position
			 * @return drag start position
			 */
			int getDragStartPos();

			/**
			 * @fn void setDragEndPos( const int pos )
			 * @brief sets drag end position
			 * @param pos drag end poistion
			 */
			void setDragEndPos( const int pos );

			/**
			 * @fn int getDragEndPos()
			 * @brief gets drag end position
			 * @return drag end position
			 */
			int getDragEndPos();

			/**
			 * @fn void pushRange( const double start, const double end )
			 * @brief pushes range
			 * @param start start of range
			 * @param end end of range
			 */
			void pushRange( const double start, const double end );

			/**
			 * @fn void popRange()
			 * @brief pops range
			 */
			void popRange();

			/**
			 * @fn void clearRange()
			 * @brief clears range
			 */
			void clearRange();

			/**
			 * @fn double getStart();
			 * @brief gets start of range
			 * @return start of range
			 */
			double getStart();

			/**
			 * @fn double getEnd()
			 * @brief gets end of range
			 * @return end of range
			 */
			double getEnd();

		public:
			/**
			 * @fn kome::core::XYData& getCorrectionPoints()
			 * @brief gets correlation points
			 * @return correction points
			 */
			kome::core::XYData& getCorrectionPoints();

			/**
			 * @fn void updateCorrectionPoints( kome::core::Progress& progress )
			 * @brief updates correction points
			 * @brief progress the object to show progress
			 */
			void updateCorrectionPoints( kome::core::Progress& progress );

		public:
			/**
			 * @fn bool prepareAlignment(
					kome::plugin::PluginFunctionItem& item,
					kome::objects::SettingParameterValues* settings,
					kome::core::Progress& progress
				)
			 * @brief prepares alignment
			 * @param item alignment item object
			 * @param settings setting parameter values
			 * @return If true, it succeeded to alignment
			 */
			bool prepareAlignment(
				kome::plugin::PluginFunctionItem& alignment_item,
				kome::objects::SettingParameterValues* alignment_settings,
				kome::plugin::PluginFunctionItem& scaling_item,              // @date 2013.04.04 <Add> Fujita  scaling—p‚Ìitem‚Æsettings‚ð’Ç‰Á
				kome::objects::SettingParameterValues* scaling_settings,     // @date 2013.04.04 <Add> Fujita
				kome::core::Progress& progress
			);

		protected:
			/**
			 * @fn void clearControl()
			 * @brief clears control chromatogram
			 */
			void clearControl();

			/**
			 * @fn void clearTreatment()
			 * @brief clears treatment chromatogram
			 */
			void clearTreatment();

		public:
			/**
			 * @fn static AlignmentManager& getInstance()
			 * @brief gets linear alignment manager
			 * @return linear alignment manager class (This is the only object.)
			 */
			static AlignmentManager& getInstance();

		public:
			/**
			 * @fn void ReinstateTicValue()
			 * @brief reinstate TIC value from scaled value to raw data. This mehtod is called in destructor.
			 */
			void AlignmentManager::ReinstateTicValue();


		};
	}
}

#endif		// __KOME_ALIGNMENT_MANAGER_H__
