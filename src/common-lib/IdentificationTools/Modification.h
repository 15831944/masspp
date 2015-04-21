/**
 * @file Modification.h
 * @brief interfaces of Modification class
 *
 * @author H.Parry
 * @date 2012.01.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */

#pragma once

namespace kome {
	namespace ident {

		/**
		 * @class Modification
		 * @brief modification information class
		 */
		class IDENT_TOOLS_CLASS Modification
		{
		private:
			/** modification name */
			std::string	m_name;

			/** mono-isotopic m/z value */
			double m_monomass;

			/** amino acid residue */
			std::string	m_aminoAcidResidue;

			/** hidden flag */
			bool m_hidden;

			/** ID */
			int m_id;

		public:
			/**
			 * @fn Modification( const char* name, const double monomass, const char* aminoAcidResidue, const bool hidden )
			 * @brief constructor
			 * @param[in] name modification name
			 * @param[in] monomass mono mass
			 * @param[in] aminoAcidResidue amino acid residue
			 * @param[in] hidden hidden flag
			 */
			Modification( const char* name, const double monomass, const char* aminoAcidResidue, const bool hidden );

			/**
			 * @fn ~Modification( void )
			 * @brief destructor
			 */
			~Modification( void );

		public:
			/**
			 * @fn std::string getName( void ) const
			 * @brief get the modification name
			 * @return name
			 */
			std::string getName( void ) const;

			/**
			 * @fn void setMonoMass( const double monoMass )
			 * @brief sets the mono mass
			 * @param[in] monoMass mono mass
			 */
			void setMonoMass( const double monoMass );

			/**
			 * @fn double getMonoMass( void ) const
			 * @brief get the mono-isotopic m/z value
			 * @return mono-isotopic m/z value
			 */
			double getMonoMass( void ) const;

			/**
			 * @fn void setAminoAcidResidue( const char* aar )
			 * @brief sets the amino acid residue
			 * @param[in] aar amino acid residue
			 */
			void setAminoAcidResidue( const char* aar );

			/**
			 * @fn std::string getAminoAcidResidue( void ) const
			 * @brief get the amino acid residue
			 * @return amino acid residue
			 */
			std::string getAminoAcidResidue( void ) const;

			/**
			 * @fn bool isHidden()
			 * @brief gets the hidden flag value
			 * @return If true, this modification is hidden modification.
			 */
			bool isHidden();

		public:
			/**
			 * @fn void setId( const int id )
			 * @brief sets modification ID
			 * @param[in] id modification ID
			 */
			void setId( const int id );

			/**
			 * @fn int getId()
			 * @brief gets modification ID
			 * @return modification ID
			 */
			int getId();

			/**
			 * @fn void setHidden( const bool hidden )
			 * @brief sets the hidden flag value
			 * @param[in] hidden hidden flag value
			 */
			void setHidden( const bool hidden );
		};
	}
}
