/**
 * @file SampleUserPropertiesOperation.h
 * @brief interfaces of SampleUserPropertiesOperation class
 *
 * @author M.Izumi
 * @date 2012.05.13
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_SAMPLE_USER_PROPERTIES_OPERATION_H__
#define __KOME_SAMPLE_USER_PROPERTIES_OPERATION_H__

#include <vector>


namespace kome {
	namespace properties { 

		class DetailsViewManager;

		class SampleUserPropertiesOperation : public kome::operation::Operation {
		public:
			/**
			 * @fn SampleUserPropertiesOperation()
			 * @brief constructor
			 */
			SampleUserPropertiesOperation();
			
			/**
			 * @fn virtual ~SampleUserPropertiesOperation()
			 * @brief destructor
			 */
			virtual ~SampleUserPropertiesOperation();

		protected:
			/** spectrum object */
			kome::objects::Spectrum* m_spec;

			/** chromatogram object */
			kome::objects::Chromatogram* m_chrom;

			/** group object */
			kome::objects::DataGroupNode* m_group;
			/** is edited flag */
			bool m_bIsEdited;

			/** validate */
			bool m_bValidate;

			/** properties[key : value] */
			std::vector< std::pair< std::string, std::string > > m_properties;
		public:
			/** 
			 * @fn void setSpec( kome::objects::Spectrum* spec )
			 * @brief sets spectrum
			 * @param spec
			 */
			void setSpec( kome::objects::Spectrum* spec );

			/**
			 * @fn kome::objects::Spectrum* getSpec()
			 * @brief gets spectrum
			 * @return spectrum
			 */
			kome::objects::Spectrum* getSpec();

			/**
			 * @fn void setChrom( kome::objects::Chromatogram* chrom )
			 * @brief sets chromatogram
			 * @param chrom
			 */
			void setChrom( kome::objects::Chromatogram* chrom );

			/**
			 * @fn kome::objects::Chromatogram* getChrom()
			 * @brief gets chromatogram
			 * @param chromatogram
			 */
			kome::objects::Chromatogram* getChrom();

			/**
			 * @fn void setGroup( kome::objects::DataGroupNode* group )
			 * @brief sets group 
			 * @param group
			 */
			void setGroup( kome::objects::DataGroupNode* group );

			/**
			 * @fn kome::objects::DataGroupNode* getGroup()
			 * @brief gets group
			 */
			kome::objects::DataGroupNode* getGroup();

			/**
			 * @fn void setEdit( bool bedit )
			 * @brief sets edit
			 * @param bedit
			 */
			void setEdit( bool bedit ){ m_bIsEdited = bedit; }
			
			/**
			 * @fn bool getEdit()
			 * @brief gets edit
			 * @param bedit
			 */
			bool getEdit(){ return m_bIsEdited; }

			/**
			 * @fn void setValidate( bool validate )
			 * @brief sets validate
			 * @param validate
			 */
			void setValidate( bool validate ){ m_bValidate = validate; }

			/**
			 * @fn bool getValidate()
			 * @brief gets validate
			 * @return validate
			 */
			bool getValidate(){ return m_bValidate; }

			/**
			 * @fn void setProperties( std::vector< std::pair< std::string, std::string > > props )
			 * @brief sets properties
			 * @param props
			 */
			void setProperties( std::vector< std::pair< std::string, std::string > > props ){ m_properties = props; }
			
			/**
			 * @fn std::vector< std::pair< std::string, std::string > > getProperties()
			 * @brief gets properties
			 */
			std::vector< std::pair< std::string, std::string > > getProperties(){ return m_properties; }

		protected:
			/**
			 * @fn void SaveUserProperties()
			 * @brief save user properties
			 */ 
			void SaveUserProperties();

			/**
			 * @fn void updatePropetiesView()
			 * @brief update propeties view
			 */
		    void updatePropetiesView();

		protected:
			
			/**
			* @fn virtual bool onExecute()
			* @brief execute operation
			* @return If true, it succeeded to perform search.
			*/
			virtual bool onExecute();

			/**
			* @fn virtual void onLoadCondition( boost::function< int ( void*, int ) > readFun )
			* @brief load the saved state with respect to to search engine operations
			* @param[in] readFun function to read saved state
			*/
			virtual void onLoadCondition( boost::function< int ( void*, int ) > readFun );

			/**
			* @fn virtual void onSaveCondition( boost::function< int ( void*, int ) > writeFun )
			* @brief save the current state with respect to search engine operations
			* @param[in] writeFun function to save state
			*/
			virtual void onSaveCondition( boost::function< int ( void*, int ) > writeFun  );

			/**
			 * @fn virtual const char* onGetDescription()
			 * @brief get discription (override method)
			 * @return discription
			 */
			virtual std::string onGetDescription();
			
			/**
			 * @fn virtual const char* onGetParametersString()
			 * @brief get parameters string (override method)
			 * @return parameters string 
			 */
			virtual std::string onGetParametersString();
			
			/**
			 * @fn virtual void onSetParametersString( const char* strParam )
			 * @brief set parameters string  (override method)
			 * @param[in] strParam parameters string
			 */
			virtual void onSetParametersString( const char* strParam );
			
		};
	}
}


#endif // __KOME_SAMPLE_USER_PROPERTIES_OPERATION_H__
