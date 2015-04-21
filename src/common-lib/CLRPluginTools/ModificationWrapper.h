/**
 * @file ModificationWrapper.h
 * @brief interfaces of ModificationWrapper class
 *
 * @author S.Tanaka
 * @date 2012.11.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#ifndef __KOME_CLR_MODIFICATION_WRAPPER_H__
#define __KOME_CLR_MODIFICATION_WRAPPER_H__


namespace kome {
	namespace clr { 


		/**
		 * @class ModificationWrapper
		 * @brief modification wrapper class to use on CLR environment
		 */
		public ref class ModificationWrapper {
		public:
			/**
			 * @fn ModificationWrapper( kome::ident::Modification& modification )
			 * @briefructor
			 * @param[in] modification modification object
			 */
			ModificationWrapper( kome::ident::Modification& modification );

			/**
			 * @fn virtual ~ModificationWrapper()
			 * @brief destructor
			 */
			virtual ~ModificationWrapper();

		protected:
			/** modification object */
			kome::ident::Modification& m_modification;

		public:
			/**
			 * @fn kome::ident::Modification& getModification()
			 * @brief gets modification object
			 * @return modification object
			 */
			kome::ident::Modification& getModification();

		public:
			/**
			 * @fn System::String^ getName()
			 * @brief get the modification name
			 * @return name
			 */
			System::String^ getName();

			/**
			 * @fn void setMonoMass( double monoMass )
			 * @brief sets the mono mass
			 * @param[in] monoMass mono mass
			 */
			void setMonoMass( double monoMass );

			/**
			 * @fn double getMonoMass()
			 * @brief get the mono-isotopic m/z value
			 * @return mono-isotopic m/z value
			 */
			double getMonoMass();

			/**
			 * @fn void setAminoAcidResidue( System::String^ aar )
			 * @brief sets the amino acid residue
			 * @param[in] aar amino acid residue
			 */
			void setAminoAcidResidue( System::String^ aar );

			/**
			 * @fn System::String^ getAminoAcidResidue()
			 * @brief get the amino acid residue
			 * @return amino acid residue
			 */
			System::String^ getAminoAcidResidue();

			/**
			 * @fn bool isHidden()
			 * @brief gets the hidden flag value
			 * @return If true, this modification is hidden modification.
			 */
			bool isHidden();

		public:
			/**
			 * @fn void setId( const int id )
			 * @brief sets the enzyme ID
			 * @param[in] id enzyme ID
			 */
			void setId( const int id );

			/**
			 * @fn int getId()
			 * @brief gets enzyme ID
			 * @return enzyme ID
			 */
			int getId();

			/**
			 * @fn void setHidden( bool hidden )
			 * @brief sets the hidden flag value
			 * @param[in] hidden hidden flag value
			 */
			void setHidden( bool hidden );
		};
	}
}

#endif		// __KOME_CLR_MODIFICATION_WRAPPER_H__

