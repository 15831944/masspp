/**
 * @file DataMapChangeDispRangeDialog.h
 * @brief interface of DataMapChangeDispRangeDialog class
 * @author M.Izumi
 * @date 2013.02.04
 *
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_OPERATION_RANGE_DATAMAP_CHANGE_DISP_RANGE_DIALOG_H__
#define __KOME_OPERATION_RANGE_DATAMAP_CHANGE_DISP_RANGE_DIALOG_H__

#include <wx/wx.h>

namespace kome {
	namespace operation {
		namespace range{

		class DataMapChangeDispRangeDialog : public kome::window::ParametersAndHelpDialog {
		public:
			/**
			 * @fn DataMapChangeDispRangeDialog(wxWindow* parent)
			 * @brief constructor
			 * @param parent
			 */
			DataMapChangeDispRangeDialog(wxWindow* parent);

			/**
			 * @fn virtual ~DataMapChangeDispRangeDialog()
			 * @brief destructor
			 */
			virtual ~DataMapChangeDispRangeDialog();

		protected:
			/** parameters */
			kome::plugin::ParameterSettings m_params;

			/** settings */
			kome::objects::SettingParameterValues m_settings;

			/** start rt */
			double m_startRt;
			/** end rt */
			double m_endRt;
			/** start m/z */
			double m_startMz;
			/** end m/z */
			double m_endMz;
			
		public:
			/**
			 * @fn double getStartRt()
			 * @brief get start rt
			 * @return start rt
			 */
			double getStartRt(){ return m_startRt; }

			/**
			 * @fn void setStartRt
			 * @brief set start rt
			 * @param start
			 */
			void setStartRt( double start ){ m_startRt = start; }

			/**
			 * @fn double getEndRt()
			 * @brief get end rt
			 * @return end rt
			 */
			double getEndRt(){ return m_endRt; }

			/**
			 * @fn void setEndRt
			 * @brief set end rt
			 * @param end
			 */
			void setEndRt( double end ){ m_endRt = end; }

			/**
			 * @fn double getStartMz()
			 * @brief get start m/z 
			 * @return start m/z
			 */
			double getStartMz(){ return m_startMz; }

			/**
			 * @fn void setStartMz( double start )
			 * @brief set start m/z 
			 * @param start
			 */
			void setStartMz( double start ){ m_startMz = start; }

			/**
			 * @fn double getEndMz()
			 * @brief get end m/z
			 * @return end m/z
			 */
			double getEndMz(){ return m_endMz; }

			/**
			 * @fn void setEndMz( double end )
			 * @brief set end m/z
			 * @param end
			 */
			void setEndMz( double end ){ m_endMz = end; }
		
		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer (override method)
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();
		
			/**
			 * @fn virtual bool TransferDataToWindow()
			 * @brief transfer value data to window controls
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataToWindow();

		protected:
			/**
			 * @fn bool setRange( const char* paramName, double& startPos, double& endPos )
			 * @brief set range
			 * @param[in] paramName
			 * @param[out] startPos
			 * @param[out] endPos
			 */
			bool setRange( const char* paramName, double& startPos, double& endPos );
		private:
			DECLARE_EVENT_TABLE();
		};


		}
	}
}



#endif	//  __KOME_OPERATION_RANGE_DATAMAP_CHANGE_DISP_RANGE_DIALOG_H__
