/**
 * @file PositionImporter.h
 * @brief interfaces of PositionImporter class
 *
 * @author S.Tanaka
 * @date 2008.12.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PEAK_POSITIONS_FILE_IMPORT_H__
#define __KOME_PEAK_POSITIONS_FILE_IMPORT_H__


namespace kome {
	namespace position {
		namespace file {

			/**
			 * @class PositionImporter
			 * @brief weighted average position class
			 */
			class PositionImporter {
			public:
				/**
				 * @fn PositionImporter()
				 * @brief constructor
				 */
				PositionImporter( kome::objects::SettingParameterValues* settings );

				/**
				 * @fn virtual ~PositionImporter()
				 * @brief destructor
				 */
				virtual ~PositionImporter();

			protected:
				/** RT column */
				int m_rtCol;

				/** m/z column */
				int m_mzCol;

				/** charge column */
				int m_chargeCol;

				/**
				 * @struct ColumnInfo
				 * @brief column information
				 */
				struct ColumnInfo {
					std::string name;
					kome::ident::ColumnDataType type;
					int col_number;
					int id;
				};

			protected:
				/** setting parameters */
				kome::objects::SettingParameterValues* m_settings;

				/** columns */
				std::vector< ColumnInfo > m_columns;

			protected:
				/** file path */
				std::string m_filePath;

				/** separator */
				std::string m_separator;

			public:
				/**
				 * @fn void execute( kome::objects::Peaks2D* peaks )
				 * @brief import peak positions
				 * @param[out] peaks the object to store peak positions
				 */
				void execute( kome::objects::Peaks2D* peaks );

			protected:
				/**
				 * @fn void tokenLine( const char* line, std::vector< std::string >& tokens )
				 * @brief tokens line
				 * @param line the character string of line
				 * @param tokens the array to store tokens
				 */
				void tokenLine( const char* line, std::vector< std::string >& tokens );

			protected:
				/**
				 * @fn void setParameters()
				 * @brief sets parametes from settings object
				 */
				void setParameters();
			};
		}
	}
}

#endif	// __KOME_PEAK_POSITIONS_FILE_IMPORT_H__
