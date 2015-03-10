/**
 * @file BatchButtonDef.h
 * @brief define of Batch Processing Plugin
 *
 * @author OKADA, H
 * @date 2012-02-30
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#ifndef __BATCH_BUTTON_DEF_H__
#define __BATCH_BUTTON_DEF_H__


// button id
#define ID_MATRIX_ADD_BUTTON			2000
#define ID_MATRIX_EDIT_BUTTON			2000+1
#define ID_MATRIX_SAVE_BUTTON			2000+2
#define ID_MATRIX_SAVEAS_BUTTON			2000+3
#define ID_MATRIX_DELETE_BUTTON			2000+4
#define ID_BAT_CONF_CPANE				2000+5

#define ID_BAT_SELECT_BUTTON			2000+6	// ADD

#define ID_BAT_EDIT_BUTTON				2000+7
#define ID_BAT_DELETE_BUTTON			2000+8
#define ID_BAT_UP_BUTTON				2000+9
#define ID_BAT_DOWN_BUTTON				2000+10

#define ID_BAT_JOB_LIST_BUTTON			2000+11
#define ID_BAT_JOB_LIST_CLOSE_BUTTON	2000+12

#define ID_BAT_JOB_LIST_DELETE_BUTTON	2000+13


#define ID_SUBMIT_JOB_SUBMIT_BUTTON			2000+14
#define ID_SUBMIT_JOB_CANCEL_BUTTON			2000+15

#define ID_NAME_TEXT						2000+101
#define ID_BATCH_SRV_COMBOBOX				2000+102
#define ID_COMMENT_NAME_TEXT				2000+103

#define	ID_SUBMIT_JOB_ADD_BUTTON			2000+104
#define	ID_SUBMIT_JOB_DELETE_BUTTON			2000+105
#define	ID_SUBMIT_JOB_UP_BUTTON				2000+106
#define	ID_SUBMIT_JOB_DOWN_BUTTON			2000+107
#define	ID_SUBMIT_JOB_OUTPUT_FORMATS_BUTTON	2000+108
#define	ID_SUBMIT_JOB_FOLDER_OPEN_BUTTON	2000+109	// @date 2012/08/03 <Add> OKADA

#define ID_NAME_NEW_METHOD_TEXT				2000+110
#define ID_RADIOBUTTON_NEW_METHOD_TEXT		2000+111
#define ID_RADIOBUTTON_NEW_TEMPLATE_TEXT	2000+112
#define ID_RADIOBUTTON_NEW_BLANK_TEXT		2000+113

#define ID_COMBOBOX_METHOD					2000+114

#define ID_COMBOBOX_NEW_METHOD				2000+115
#define ID_COMBOBOX_NEW_TEMPLATE			2000+116

#define ID_NAME_EDIT_METHOD_TEXT			2000+117

#define ID_COMBOBOX_DATA_TYPE				2000+118	// @date 2012/08/02 <Add> OKADA


#define ID_CHKBX_TOP					2500
#define ID_TEXT_FMT_TOP					2600


// XMLファイル出力用の各タグ名の定義
#define XML_HEADER				"<?xml version=\"1.0\"?>\n"
#define XML_BAT_START			"<batch>\n"
#define XML_BAT_END				"</batch>\n"
#define XML_NAME_START			"\t<name>"
#define XML_NAME_END			"</name>\n"
#define XML_DATE_START			"\t<date>"
#define XML_DATE_END			"</date>\n"
#define XML_FUNCTIONS_START		"\t<functions>\n"
#define XML_FUNCTIONS_END		"\t</functions>\n"
#define XML_BATCHFUN_CALL_START	"\t\t<batch-fun call=\"%s\">\n"
#define XML_BATCHFUN_CALL_END	"\t\t</batch-fun>\n"

#define XML_PARAMS_START		"\t\t\t<params>\n"
#define XML_PARAMS_END			"\t\t\t</params>\n"
#define XML_PARAM_START			"\t\t\t\t<param>\n"
#define XML_PARAM_END			"\t\t\t\t</param>\n"
#define XML_PARAM_NAME			"\t\t\t\t\t<param-name>%s</param-name>\n"
#define XML_PARAM_VALUE			"\t\t\t\t\t<param-value>%s</param-value>\n"


// XML最終保存ファイル iniファイル保存用
#define SECTION_XML		"BatchLastXml"
#define DEFAULT_XML_KEY	"DEFAULT_XML"
#define DEFAULT_VALUEY	"DEFAULT"


// OutputFormats INIファイル保存用
#define SECTION_FMT			"BatchOutputFormats"
#define KEY_PARAM			"_PARAM"		// @date 2013/05/22 <Add> OKADA
#define KEY_OUTPUT_FORMAT	"OUTPUT_FORMAT"	// @date 2013/05/22 <Add> OKADA
#define SECTION_METHOD_FMT	"BatchOutputFormats_Method"	// @date 2013/05/22 <Add> OKADA

#define KEY_FMT_MIS_TXT	"SAVE_FORMAT_MIS_TXT"
#define KEY_FMT_MSB		"SAVE_FORMAT_MSB"
#define KEY_FMT_MZML	"SAVE_FORMAT_MZML"
#define KEY_FMT_MZXML	"SAVE_FORMAT_MZXML"
#define KEY_FMT_PNG		"SAVE_FORMAT_PNG"
#define KEY_FMT_TSVTXT	"SAVE_FORMAT_TSVTXT"

#define DEFAULT_FMT_VALUEY	"DEFAULT"


#endif
