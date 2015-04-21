/**
 * @file Enzyme.h
 * @brief interfaces of Enzyme class
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
		 * @class Enzyme
		 * @brief enzyme information class
		 */
		class IDENT_TOOLS_CLASS Enzyme
		{
		public:
			/**
			 * @fn Enzyme( const char* name )
			 * @brief constructor
			 */
			Enzyme( const char* name );

			/**
			 * @fn ~Enzyme( void )
			 * @brief destructor
			 */
			~Enzyme( void );

		private:
			/** ID */
			int m_id;

			/** enzyme name */
			std::string	m_name;

			/** hidden */
			bool m_hidden;

		public:
			/**
			 * @typedef TermType
			 * @brief terminal type
			 */
			typedef enum {
				UNKNOWN = 0,
				C_TERM = 1,
				N_TERM = 2
			} TermType;

			/**
			 * @struct Cutter
			 * @brief cutter information
			 */
			struct Cutter {
				TermType	term;
				std::string	cleave;
				std::string	restrict;
			};

		protected:
			/** cutters */
			std::vector< Cutter > m_cutters;

		public:
			/**
			 * @fn std::string getName( void ) const
			 * @brief get the enzyme name
			 * @return name
			 */
			std::string getName( void ) const;

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
			 * @fn void clearCutters()
			 * @brief clears cutters
			 */
			void clearCutters();

			/**
			 * @fn void addCutter( TermType term, const char* cleave, const char* restrict )
			 * @brief adds cutter
			 * @param[in] term terminal type
			 * @param[in] cleave cleave
			 * @param[in] restrict restrict
			 */
			void addCutter( TermType term, const char* cleave, const char* restrict );

			/**
			 * @fn unsigned int getNumberOfCutters()
			 * @brief get number of cutters
			 * @return the number of cutters
			 */
			unsigned int getNumberOfCutters();


			/**
			 * @fn Cutter* getCutter( const unsigned int idx )
			 * @brief get the cutter
			 * @param[in] idx cutter index
			 * @return cutter
			 */
			Cutter* getCutter( const unsigned int idx );

			/**
			 * @fn void setHidden( const bool hidden )
			 * @brief sets the hidden flag value
			 * @param[in] hidden hidden flag value
			 */
			void setHidden( const bool hidden );

			/**
			 * @fn bool isHidden()
			 * @brief gets the hidden flag value
			 * @return hidden flag value
			 */
			bool isHidden();

		};
	}
}
