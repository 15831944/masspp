/**
 * @file DataGroupNodeWrapper.h
 * @brief interfaces of DataGroupNodeWrapper class
 *
 * @author S.Tanaka
 * @date 2006.08.31
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_DATA_GROUP_NODE_WRAPPER_H__
#define __KOME_CLR_DATA_GROUP_NODE_WRAPPER_H__


#include "DataSetWrapper.h"


namespace kome {
	namespace clr {

		ref class SampleWrapper;
		ref class SpectrumWrapper;
		ref class PropertiesWrapper;
		ref class ChromatogramWrapper;

		/**
		 * @class DataGroupNodeWrapper
		 * @brief DataGroupNode wrapper class to user on CLR environment
		 */
		public ref class DataGroupNodeWrapper : public DataSetWrapper {
		public:
			/**
			 * @fn DataGroupNodeWrapper( kome::objects::DataGroupNode& group )
			 * @brief constructor
			 * @brief group base object
			 */
			DataGroupNodeWrapper( kome::objects::DataGroupNode& group );

			/**
			 * @fn virtual ~DataGroupNodeWrapper()
			 * @brief destructor
			 */
			virtual ~DataGroupNodeWrapper();

		protected:
			/** base object */
			kome::objects::DataGroupNode& m_group;

		public:
			/**
			 * @fn kome::objects::DataGroupNode& getDataGroupNode()
			 * @brief gets base object
			 * @return the pointer to the base object
			 */
			kome::objects::DataGroupNode& getDataGroupNode();

		public:
			/**
			 * @fn void setId( int id )
			 * @brief sets group id
			 * @param[in] id group id
			 */
			void setId( int id );

			/**
			 * @fn int getId()
			 * @brief gets gropu id
			 * @return group id
			 */
			int getId();

		public:
			/**
			 * @fn void setName( System::String^ name )
			 * @brief sets group name
			 * @param[in] name group name
			 */
			void setName( System::String^ name );

			/** 
			 * @fn System::String^ getName()
			 * @brief gets group name
			 * @return group name
			 */
			System::String^ getName();

			/**
			 * @fn void setGroupIndex( int index )
			 * @brief sets group index
			 * @param[in] index group index
			 */
			void setGroupIndex( int index );

			/**
			 * @fn int getGroupIndex()
			 * @brief gets group index
			 * @return group index
			 */
			int getGroupIndex();

			/**
			 * @fn DataGroupNodeWrapper^ getParentGroup()
			 * @brief get parent spectrum group
			 * @return parent spectrum group. (If this is root group, this method returns NULL.)
			 */
			DataGroupNodeWrapper^ getParentGroup();

			/**
			 * @fn DataGroupNodeWrapper^ createChildGroup( System::String^ name )
			 * @brief creates new child group object
			 * @param[in] name child group name
			 * @return the pointer to new child object
			 */
			DataGroupNodeWrapper^ createChildGroup( System::String^ name );

			/**
			 * @fn void removeChildGroup( DataGroupNodeWrapper^ child )
			 * @brief removes child group
			 * @param[in] child child group to be removed.
			 */
			void removeChildGroup( DataGroupNodeWrapper^ child );

			/**
			 * @fn unsigned int getNumberOfChildren()
			 * @brief gets the number of children
			 * @return the number of children
			 */
			unsigned int getNumberOfChildren();

			/**
			 * @fn DataGroupNodeWrapper^ getChild( unsigned int index )
			 * @brief gets child group
			 * @param[in] index child group index
			 * @return the pointer to the child group. (If index is illegal number, this method returns NULL.)
			 */
			DataGroupNodeWrapper^ getChild( unsigned int index );

			/**
			 * @fn void getDataSet( DataSetWrapper^ dataSet )
			 * @brief gets data set that contains this group.
					  (getSpectrum method cannot get spectra that belong to child group.)
			 * @param[out] dataSet data set object to store spectrum information
			 */
			void getDataSet( DataSetWrapper^ dataSet );

			/**
			 * @fn void setAutoScanNumber( bool autoScanNumber )
			 * @brief sets the auto scan number flag
			 * @param[in] autoScanNumber auto scan number flag
			 */
			 void setAutoScanNumber( bool autoScanNumber );

			 /**
			  * @fn bool isAutoScanNumber()
			  * @brief gets the auto scan number flag
			  * @return auto scan number flag
			  */
			 bool isAutoScanNumber();

		public:
			/**
			 * @fn void setSpecXTitle( System::String^ title )
			 * @brief sets spectrum x axis title
			 * @param[in] title spectrum x axis title
			 */
			void setSpecXTitle( System::String^ title );

			/**
			 * @fn System::String^ getSpecXTitle()
			 * @brief gets spectrum x axis title
			 * @return spectrum x axis title
			 */
			System::String^ getSpecXTitle();

			/**
			 * @fn void setSpecYTitle( System::String^ title )
			 * @brief sets spectrum y axis title
			 * @param[in] title spectrum y axis title
			 */
			void setSpecYTitle( System::String^ title );

			/**
			 * @fn System::String^ getSpecYTitle()
			 * @brief gets spectrum y axis title
			 * @return spectrum y axis title
			 */
			System::String^ getSpecYTitle();

			/**
			 * @fn void setChromXTitle( System::String^ title )
			 * @brief sets chromatogram x axis title
			 * @param[in] title chromatogram x axis title
			 */
			void setChromXTitle( System::String^ title );

			/**
			 * @fn System::String^ getChromXTitle()
			 * @brief gets chromatogram x axis title
			 * @return chromatogram x axis title
			 */
			System::String^ getChromXTitle();

			/**
			 * @fn void setChromYTitle( System::String^ title )
			 * @brief sets chromatogram y axis title
			 * @param[in] title chromatogram y axis title
			 */
			void setChromYTitle( System::String^ title );

			/**
			 * @fn System::String^ getChromYTitle()
			 * @brief gets chromatogram y axis title
			 * @return chromatogram y axis title
			 */
			System::String^ getChromYTitle();

			/**
			 * @fn unsigned int getLevel()
			 * @brief gets group level
			 * @return group level
			 */
			unsigned int getLevel();

			/**
			 * @fn PropertiesWrapper^ getProperties()
			 * @brief gets properties
			 * @return properties
			 */
			PropertiesWrapper^ getProperties();
		};
	}
}

#endif	// __KOME_CLR_DATA_GROUP_NODE_WRAPPER_H__
