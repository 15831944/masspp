/**
 * @file OperationLogFunctions.h
 * @brief interfaces of OperationManager plug-in functions
 *
 * @author M.Izumi
 * @date 2011.12.08
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_OPERATION_LOG_FUNCTIONS_H__
#define __KOME_OPERATION_LOG_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant toggleOperationDialog( kome::objects::Parameters* )
 * @brief toggles the visibility of the operation log view
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant toggleOperationDialog( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant checkOperationLogDialog( kome::objects::Parameters* )
 * @brief check the visibility
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant checkOperationLogDialog( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant ProcUndo( kome::objects::Parameters* )
 * @brief Processing　Undo 　
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant ProcUndo( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant ProcRedo( kome::objects::Parameters* )
 * @brief Processing　Redo 　
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant ProcRedo( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant SendOptInfo( kome::objects::Parameters* )
 * @brief Log Listの更新 　
 * @return This value has no meanings.
 */
kome::objects::Variant SendOptInfo( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant exitGui( kome::objects::Parameters* )
 * @brief exits GUI
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant exitGui( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant initOperationLogView( kome::objects::Parameters* )
 * @brief initializes operation log view
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant initOperationLogView( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant enableMenuUndo( kome::objects::Parameters* params )
 * @briefenable menu Undo
 * @return return value (enable menu Undo.)
 */
kome::objects::Variant enableMenuUndo( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant enableMenuRedo( kome::objects::Parameters* params )
 * @brief enable menu Redo
 * @return return value (enable menu Redo.)
 */
kome::objects::Variant enableMenuRedo( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getOperationList( kome::objects::Parameters* params )
 * @brief Batch処理を行なうOperationをリストから取得
 * @retun return batch info
 */
kome::objects::Variant getOperationList( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant runBatchOperations( kome::objects::Parameters* params )
 * @brief  run batch operations [ %msppcmd -operation operation parameter ]
 * @return This value has no meanings.
 */
kome::objects::Variant runBatchOperations( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant runBatchOperationXml( kome::objects::Parameters* params )
 * @brief  run batch operations [ %msppcmd -operation_xml XML File ]
 * @return This value has no meanings.
 */
kome::objects::Variant runBatchOperationXml( kome::objects::Parameters* params );

#if 1
/**
 * @kome::objects::Variant batchSetFunction( kome::objects::Parameters* params );
 * @brief batch_set BatchInfo*へのポインタを返す
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant batchSetFunction( kome::objects::Parameters* params );
#else
// @date 2013/08/19 <Add> OKADA ------->
/**
 * @fn kome::plugin::BatchInfo* batchSetFunction( kome::objects::Sample* acSample )
 * @brief batch_set BatchInfo*へのポインタを返す
 * @param acSample active sample
 * @return BatchInfo*へのポインタ
 */
kome::plugin::BatchInfo* batchSetFunction( kome::objects::Sample* acSample );
// @date 2013/08/19 <Add> OKADA <-------
#endif



/**
 * @fn kome::objects::Variant SaveMspXmlFile( kome::objects::Parameters* params )
 * @brief save mspXml file
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant SaveMspXmlFile( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant SaveAsMspXmlFile( kome::objects::Parameters* params )
 * @brief save as mspXml file
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant SaveAsMspXmlFile( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant openMspXmlFile( kome::objects::Parameters* params )
 * @brief open mspXml File
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant openMspXmlFile( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant exitFuncSaveMspXml( kome::objects::Parameters*  )
 * @brief exit function save mspXml
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant exitFuncSaveMspXml( kome::objects::Parameters*  );

#ifdef __unix
	}
#endif  // __unix



#endif		// __KOME_OPERATION_LOG_FUNCTIONS_H__
