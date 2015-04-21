/**
 * @file EnzymeWrapper.h
 * @brief interfaces of EnzymeWrapper class
 *
 * @author S.Tanaka
 * @date 2012.11.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#ifndef __KOME_CLR_ENZYME_WRAPPER_H__
#define __KOME_CLR_ENZYME_WRAPPER_H__


namespace kome {
	namespace clr { 


		/**
		 * @class EnzymeWrapper
		 * @brief enzyme wrapper class to use on CLR environment
		 */
		public ref class EnzymeWrapper {
		public:
			/**
			 * @fn EnzymeWrapper( kome::ident::Enzyme& enzyme )
			 * @briefructor
			 * @param[in] enzyme object
			 */
			EnzymeWrapper( kome::ident::Enzyme& enzyme );

			/**
			 * @fn virtual ~EnzymeWrapper()
			 * @brief destructor
			 */
			virtual ~EnzymeWrapper();

		protected:
			/** enzyme object */
			kome::ident::Enzyme& m_enzyme;

		public:
			/** term type */
			ENUM_CLASS TermType {
				UNKNOWN = 0,
				C_TERM = 1,
				N_TERM = 2
			};

			/** 
			* @struct cutter
			* @brief peptide cutter by enzyme
			*/
			ref struct Cutter {
				TermType term;
				System::String^ cleave;
				System::String^ restrict;
			};

		public:
			/**
			 * @fn kome::ident::Enzyme& getEnzyme()
			 * @brief gets enzyme object
			 * @return enzyme object
			 */
			kome::ident::Enzyme& getEnzyme();

		public:
			/**
			 * @fn System::String^ getName()
			 * @brief get the enzyme name
			 * @return name
			 */
			System::String^ getName();

			/**
			 * @fn void setId( int id )
			 * @brief sets the enzyme ID
			 * @param[in] id enzyme ID
			 */
			void setId( int id );

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
			 * @fn void addCutter( TermType term, System::String^ cleave, System::String^ restrict )
			 * @brief adds cutter
			 * @param[in] term terminal type
			 * @param[in] cleave cleave
			 * @param[in] restrict restrict
			 */
			void addCutter( TermType term, System::String^ cleave, System::String^ restrict );

			/**
			 * @fn unsigned int getNumberOfCutters()
			 * @brief get number of cutters
			 * @return the number of cutters
			 */
			unsigned int getNumberOfCutters();

			/**
			 * @fn Cutter^ getCutter( unsigned int idx )
			 * @brief get the cutter
			 * @param[in] idx cutter index
			 * @return cutter
			 */
			Cutter^ getCutter( unsigned int idx );

			/**
			 * @fn void setHidden( bool hidden )
			 * @brief sets the hidden flag value
			 * @param[in] hidden hidden flag value
			 */
			void setHidden( bool hidden );

			/**
			 * @fn bool isHidden()
			 * @brief gets the hidden flag value
			 * @return hidden flag value
			 */
			bool isHidden();


		};
	}
}

#endif		// __KOME_CLR_ENZYME_WRAPPER_H__

