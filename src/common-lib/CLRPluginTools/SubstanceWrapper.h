/**
 * @file SubstanceWrapper.h
 * @brief interfaces of SubstanceWrapper class
 *
 * @author S.Tanaka
 * @date 2012.11.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#ifndef __KOME_CLR_SUBSTANCE_WRAPPER_H__
#define __KOME_CLR_SUBSTANCE_WRAPPER_H__


namespace kome {
	namespace clr { 


		/**
		 * @class SubstanceWrapper
		 * @brief substance wrapper class to use on CLR environment
		 */
		public ref class SubstanceWrapper {
		public:
			/**
			 * @fn SubstanceWrapper( kome::ident::Substance& substance )
			 * @briefructor
			 * @param[in] substance substance object
			 */
			SubstanceWrapper( kome::ident::Substance& substance );

			/**
			 * @fn virtual ~SubstanceWrapper()
			 * @brief destructor
			 */
			virtual ~SubstanceWrapper();

		protected:
			/** substance object */
			kome::ident::Substance& m_substance;

		public:
			/** substance type */
			ENUM_CLASS SubstanceType {
				UNKNOWN = 0,
				PROTEIN = 1,
				PEPTIDE = 2,
				METABOLITE = 3
			};

		public:
			/**
			 * @fn kome::ident::Substance& getSubstance()
			 * @brief gets substance object
			 * @return substance object
			 */
			kome::ident::Substance& getSubstance();

		public:
			/**
			 * @fn System::String^ getAccession()
			 * @brief get the accession name
			 * @return accession
			 */
			System::String^ getAccession();

			/**
			 * @fn System::String^ getName()
			 * @brief get the substance name
			 * @return name
			 */
			System::String^ getName();

			/**
			 * @fn SubstanceType getType()
			 * @brief get the substance type
			 * @return type
			 */
			SubstanceType getType();

			/**
			 * @fn SubstanceWrapper^ getParentSubstance()
			 * @brief get the parent substance (e.g. peptide -> protein)
			 * @return pointer to Substance object
			 */
			SubstanceWrapper^ getParentSubstance();

			/**
			 * @fnsetParentSubstance( SubstanceWrapper^ parent )
			 * @brief set the parent substance (e.g. peptide -> protein)
			 * @param[in] parent pointer to a Substance object
			 */
			void setParentSubstance( SubstanceWrapper^ parent );

			/**
			 * @fn int getNumberOfChildSubstances()
			 * @brief get the number of child substances (e.g. protein -> peptides)
			 * @return child substance count
			 */
			int getNumberOfChildSubstances();

			/**
			 * @fn cli::array< SubstanceWrapper^ >^ getChildSubstances();
			 * @brief get the child substances (e.g. protein -> peptide)
			 * @return the array of child substances
			 */
			cli::array< SubstanceWrapper^ >^ getChildSubstances();

			/**
			 * @fn PropertiesWrapper^ getProperties()
			 * @brief get the substance properties object
			 * @return properties
			 */
			PropertiesWrapper^ getProperties();
		};
	}
}


#endif		// __KOME_CLR_SUBSTANCE_WRAPPER_H__

