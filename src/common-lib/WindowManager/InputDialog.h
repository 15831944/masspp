/**
 * @file InputDialog.h
 * @brief interfaces of InputDialog class
 *
 * @author S.Tanaka
 * @date 2007.03.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_INPUT_DIALOG_H__
#define __KOME_WINDOW_INPUT_DIALOG_H__


#include <wx/wx.h>


namespace kome {
	namespace window {

		/**
		 * @class InputDialog
		 * @brief simple value input dialog
		 */
		class WINDOW_MANAGER_CLASS InputDialog : public wxDialog {
		public:
			/**
			 * @fn InputDialog( wxWindow* parent, const char* title )
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] title title
			 */
			InputDialog( wxWindow* parent, const char* title );

			/**
			 * @fn virtual ~InputDialog()
			 * @brief destructor
			 */
			virtual ~InputDialog();

		public:
			/** data type */
			typedef enum {
				TYPE_BOOL,
				TYPE_INT,
				TYPE_DOUBLE,
				TYPE_STRING,
				TYPE_SELECTION
			} DataType;

		protected:
			/** message */
			std::string m_message;

			/** input information */
			struct InputInfo {
				std::string name;
				std::string value;
				DataType type;
				kome::core::NumberRestriction restriction;
				unsigned int id;
			};

			/** inputs */
			std::vector< InputInfo > m_inputs;			

		public:
			/**
			 * @fn void setMessage( const char* message )
			 * @brief sets message
			 * @param[in] message message
			 */
			void setMessage( const char* message );

			/**
			 * @fn const char* getMessage()
			 * @brief return message
			 */
			const char* getMessage();

			/**
			 * @fn void addInput(
					const char* name,
					DataType type,
					const char* defaultValue,
					kome::core::NumberRestriction* restriction = NULL
				)
			 * @brief add input information
			 * @param[in] name input name
			 * @param[in] type data type
			 * @param[in] defaultValue default value
			 * @param[in] restriction number restriction
			 */
			void addInput(
				const char* name,
				DataType type,
				const char* defaultValue,
				kome::core::NumberRestriction* restriction = NULL
			);

			/**
			 * @fn unsigned int getNumberOfInputs()
			 * @brief gets the number of inputs
			 * @return the number of inputs
			 */
			unsigned int getNumberOfInputs();

			/**
			 * @fn const char* getInputName( const int index )
			 * @brief gets input name
			 * @param[in] index input index
			 * @return input name (If specified index is illegal, this method returns NULL.)
			 */
			const char* getInputName( const int index );

			/**
			 * @fn const char* getInputValue( const int index )
			 * @brief gets input value
			 * @param[in] index input index
			 * @return input value (If specified index is illegal, this method returns NULL.)
			 */
			const char* getInputValue( const int index );

			/**
			 * @fn bool getBoolValue( const int index, const bool dfVal )
			 * @brief gets boolean value
			 * @param[in] index input index
			 * @param[in] dfVal default value
			 * @return boolean value (If specified index is illegal, this method returns default value.)
			 */
			bool getBoolValue( const int index, const bool dfVal );

			/**
			 * @fn double getDoubleValue( const int index, const double dfVal )
			 * @brief gets double value
			 * @param[in] index input index
			 * @param[in] dfVal default value
			 * @return double value (If specified index or value is illegal, this method returns default value.)
			 */
			double getDoubleValue( const int index, const double dfVal );

			/**
			 * @fn double getIntValue( const int index, const int dfVal )
			 * @brief gets integer value
			 * @param[in] index input index
			 * @param[in] dfVal default value
			 * @return integer value (If specified index or value is illegal, this method returns default value.)
			 */
			double getIntValue( const int index, const int dfVal );

		protected:
			/**
			 * @fn void create()
			 * @brief creates dialog
			 */
			void create();

			/**
			 * @fn wxSizer* createTopSizer()
			 * @brief creates top size
			 * @return top sizer
			 */
			wxSizer* createTopSizer();

			/**
			 * @fn wxSizer* createInputSizer()
			 * @brief creates input sizer
			 * @return input sizer
			 */
			wxSizer* createInputSizer();

			/**
			 * @fn void addBoolInput( wxSizer* sizer, InputInfo& info )
			 * @brief adds boolean input
			 * @param[in] sizer the sizer to be added
			 * @param[in] info input information
			 */
			void addBoolInput( wxSizer* sizer, InputInfo& info );

			/**
			 * @fn void addIntInput( wxSizer* sizer, InputInfo& info )
			 * @brief adds integer input
			 * @param[in] sizer the sizer to be added
			 * @param[in] info input information
			 */
			void addIntInput( wxSizer* sizer, InputInfo& info );

			/**
			 * @fn void addDoubleInput( wxSizer* sizer, InputInfo& info )
			 * @brief adds double input
			 * @param[in] sizer the sizer to be added
			 * @param[in] info input information
			 */
			void addDoubleInput( wxSizer* sizer, InputInfo& info );

			/**
			 * @fn void addStringInput( wxSizer* sizer, InputInfo& info )
			 * @brief adds string input
			 * @param[in] sizer the sizer to be added
			 * @param[in] info input information
			 */
			void addStringInput( wxSizer* sizer, InputInfo& info );

			/**
			 * @fn void addSelectInput( wxSizer* sizer, InputInfo& info )
			 * @brief adds select input
			 * @param[in] sizer the sizer to be added
			 * @param[in] info input information
			 */
			void addSelectInput( wxSizer* sizer, InputInfo& info );

		protected:
			/**
			 * @fn virtual bool Validate()
			 * @brief validates the values
			 * @return If any of the valudations failed, this method returns false.
			 */
			virtual bool Validate();

			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();

		protected:
			/**
			 * @fn void onInitDialog( wxInitDialogEvent& evt )
			 * @brief This method is called on init dialog event
			 * @param[in] evt init dialog event
			 */
			void onInitDialog( wxInitDialogEvent& evt );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}


#endif		// __KOME_WINDOW_INPUT_DIALOG_H__
