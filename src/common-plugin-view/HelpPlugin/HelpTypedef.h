/**
 * @file HelpTypedef.h
 * @brief Declaration of structure
 * @author Y.Murata
 * @date 2011.12.21
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __HELP_TYPEDEF_H__
#define __HELP_TYPEDEF_H__

// HHC file object structures
typedef struct tagST_HELP_OBJECT{
	int					nID;			// Their ID
	int					nParentID;		// ID of the parent
	std::vector< int >	nChildId;		// ID of the child
	std::string			strName;		// The name of the tree node
	std::string			strLocal;		// The HTML to dispaly full path
} ST_HELP_OBJECT, *LPST_HELP_OBJECT;
			
// HHK file object structures
typedef struct tagST_HELP_KEY_OBJECT{
	std::string			strIdxName;		// Keywords that appear
	std::string			strKeyName;		// Keyword Information
	std::string			strLocal;		// The absolute path of the HTML display
} ST_HELP_KEY_OBJECT, *LPST_HELP_KEY_OBJECT;


#endif		// __HELP_TYPEDEF_H__
