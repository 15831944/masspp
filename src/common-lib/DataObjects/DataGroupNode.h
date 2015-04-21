/**
 * @file DataGroupNode.h
 * @brief interfaces of DataGroupNode class
 *
 * @author S.Tanaka
 * @date 2006.08.31
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_DATA_GROUP_NODE_H__
#define __KOME_OBJECTS_DATA_GROUP_NODE_H__


#include <string>
#include <vector>
#include <set>

#include "DataSet.h"


namespace kome {
	namespace objects {

		class Sample;
		class Chromatogram;

		/**
		 * @class DataGroupNode
		 * @brief group of spectrum management class
		 */
		class DATA_OBJECTS_CLASS DataGroupNode : public DataSet {
		public:
			/**
			 * @fn DataGroupNode( Sample* sample, const char* name )
			 * @brief constructor
			 * @param[in] sample sample object that has this spectrum group
			 * @param[in] name spectrum group name
			 */
			DataGroupNode( Sample* sample, const char* name );

			/**
			 * @fn virtual ~DataGroupNode()
			 * @brief destructor
			 */
			virtual ~DataGroupNode();
		
		protected:
			/** parent group */
			DataGroupNode* m_parent;

			/** group name */
			std::string m_name;

			/** group index */
			int m_index;

			/** level */
			short m_level;

			/** the array of child groups */
			std::vector< DataGroupNode* > m_children;

			/** spectrum x axis title */
			std::string m_specXTitle;
			/** spectrum y axis title */
			std::string m_specYTitle;

			/** chromatogram x axis title */
			std::string m_chromXTitle;
			/** chromatogram y axis title */
			std::string m_chromYTitle;

			/** properties */
			kome::core::Properties m_properties;

			/** user properties */
			kome::core::Properties m_userProperties;
			
			/** group id */
			int m_groupId;

			/** auto scan number */
			bool m_autoScanNumber;

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
			 * @fn void setName( const char* name )
			 * @brief sets group name
			 * @param[in] name group name
			 */
			void setName( const char* name );

			/** 
			 * @fn const char* getName()
			 * @brief gets group name
			 * @return group name
			 */
			const char* getName();

			/**
			 * @fn void setGroupIndex( const int index )
			 * @brief sets group index
			 * @param[in] index group index
			 */
			void setGroupIndex( const int index );

			/**
			 * @fn int getGroupIndex()
			 * @brief gets group index
			 * @return group index
			 */
			int getGroupIndex();

			/**
			 * @fn DataGroupNode* getParentGroup()
			 * @brief get parent spectrum group
			 * @return parent spectrum group. (If this is root group, this method returns NULL.)
			 */
			DataGroupNode* getParentGroup();

			/**
			 * @fn DataGroupNode* createChildGroup( const char* name )
			 * @brief creates new child group object
			 * @param[in] name child group name
			 * @return the pointer to new child object
			 */
			DataGroupNode* createChildGroup( const char* name );

			/**
			 * @fn void removeChildGroup( DataGroupNode* child )
			 * @brief removes child group
			 * @param[in] child child group to be removed.
			 */
			void removeChildGroup( DataGroupNode* child );

			/**
			 * @fn unsigned int getNumberOfChildren()
			 * @brief gets the number of children
			 * @return the number of children
			 */
			unsigned int getNumberOfChildren();

			/**
			 * @fn DataGroupNode* getChild( const unsigned int index )
			 * @brief gets child group
			 * @param[in] index child group index
			 * @return the pointer to the child group. (If index is illegal number, this method returns NULL.)
			 */
			DataGroupNode* getChild( const unsigned int index );

			/**
			 * @fn void getDataSet( DataSet* dataSet )
			 * @brief gets spectra that contains this group.
					  (getSpectrum method cannot get spectra that belong to child group.)
			 * @param[out] dataSet data set object to store spectrum information
			 */
			void getDataSet( DataSet* dataSet );

			/**
			 * @fn void setAutoScanNumber( const bool autoScanNumber )
			 * @brief sets the auto scan number flag
			 * @param[in] autoScanNumber auto scan number flag
			 */
			 void setAutoScanNumber( const bool autoScanNumber );

			 /**
			  * @fn bool isAutoScanNumber()
			  * @brief gets the auto scan number flag
			  * @return auto scan number flag
			  */
			 bool isAutoScanNumber();

		public:
			/**
			 * @fn void setSpecXTitle( const char* title )
			 * @brief sets spectrum x axis title
			 * @param[in] title spectrum x axis title
			 */
			void setSpecXTitle( const char* title );

			/**
			 * @fn const char* getSpecXTitle()
			 * @brief gets spectrum x axis title
			 * @return spectrum x axis title
			 */
			const char* getSpecXTitle();

			/**
			 * @fn void setSpecYTitle( const char* title )
			 * @brief sets spectrum y axis title
			 * @param[in] title spectrum y axis title
			 */
			void setSpecYTitle( const char* title );

			/**
			 * @fn const char* getSpecYTitle()
			 * @brief gets spectrum y axis title
			 * @return spectrum y axis title
			 */
			const char* getSpecYTitle();

			/**
			 * @fn void setChromXTitle( const char* title )
			 * @brief sets chromatogram x axis title
			 * @param[in] title chromatogram x axis title
			 */
			void setChromXTitle( const char* title );

			/**
			 * @fn const char* getChromXTitle()
			 * @brief gets chromatogram x axis title
			 * @return chromatogram x axis title
			 */
			const char* getChromXTitle();

			/**
			 * @fn void setChromYTitle( const char* title )
			 * @brief sets chromatogram y axis title
			 * @param[in] title chromatogram y axis title
			 */
			void setChromYTitle( const char* title );

			/**
			 * @fn const char* getChromYTitle()
			 * @brief gets chromatogram y axis title
			 * @return chromatogram y axis title
			 */
			const char* getChromYTitle();

			/**
			 * @fn unsigned int getLevel()
			 * @brief gets group level
			 * @return group level
			 */
			unsigned int getLevel();

			/**
			 * @fn kome::core::Properties& getProperties()
			 * @brief gets properties
			 * @return properties
			 */
			kome::core::Properties& getProperties();

			/**
			 * @fn kome::core::Properties& getUserProperties()
			 * @brief gets user properties
			 * @return properties
			 */
			kome::core::Properties& getUserProperties();

		protected:
			/**
			 * @fn void setParentDataGroupNode( DataGroupNode* parent )
			 * @brief sets parent spectrum group
			 * @param[in] parent parent spectrum group
			 */
			void setParentDataGroupNode( DataGroupNode* parent );

			/**
			 * @fn void createDefaultChromatogram()
			 * @brief creates default chromatogram
			 */
			void createDefaultChromatogram();

		public:
			/**
			 * @fn virtual void sortSpectra()
			 * @brief sorts spectra in retention time order (override method)
			 */
			virtual void sortSpectra();

			/**
			 * @fn virtual void insertSpectrum( Spectrum* spectrum, const int idx )
			 * @brief adds spectrum to group (override method )
			 * @param[in] spectrum spectrum object to be added
			 * @param[in] idx insert position
			 */
			virtual void insertSpectrum( Spectrum* spectrum, const int idx );

			/**
			 * @fn virtual void insertChromatogram( Chromatogram* chrom, const int idx )
			 * @brief adds chromatogram (override method)
			 * @param[in] chrom chromatogram
			 * @param[in] idx insert position
			 */
			virtual void insertChromatogram( Chromatogram* chrom, const int idx );
		};
	}
}

#endif	// __KOME_OBJECTS_DATA_GROUP_NODE_H__
