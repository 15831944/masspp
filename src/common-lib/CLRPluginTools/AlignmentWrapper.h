/**
 * @file AlignmentWrapper.h
 * @brief interfaces of AlignmentWrapper class
 *
 * @author S.Tanaka
 * @date 2013.01.22
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#ifndef __KOME_CLR_ALIGNMENT_WRAPPER_H__
#define __KOME_CLR_ALIGNMENT_WRAPPER_H__



namespace kome {
	namespace clr {

		/**
		 * @class AlignmentWrapper
		 * @brief alignment wrapper class to use on CLR environment
		 */
		public ref class AlignmentWrapper {
		public:
			/**
			 * @fn AlignmentWrapper( kome::operation::Alignment& align )
			 * @briefructor
			 * @param[in] align alignment object
			 */
			AlignmentWrapper( kome::operation::Alignment& align );

			/**
			 * @fn virtual ~AlignmentWrapper()
			 * @brief destructor
			 */
			virtual ~AlignmentWrapper();

		protected:
			/** alignment object */
			kome::operation::Alignment& m_align;

		public:
			/**
			 * @fn kome::operation::Alignment& getAlignment()
			 * @brief gets search engine object
			 * @return search engine object
			 */
			kome::operation::Alignment& getAlignment();

		public:
			/**
			 * @fn void prepare( SampleWrapper^ standard, SampleWrapper^ treatment, ProgressWrapper^ progress )
			 * @brief prepares alignment
			 * @param[in] standard standard sample
			 * @param[in] treatment treatment sample
			 * @param[in,out] progress the object to show the progress
			 */
			void prepare( SampleWrapper^ standard, SampleWrapper^ treatment, ProgressWrapper^ progress );

			/**
			 * @fn double convertRt( double rt, double mz )
			 * @brief gets aligned RT
			 * @param[in] rt RT before alignment
			 * @param[in] mz m/z before alignment
			 * @return aligned RT
			 */
			double convertRt( double rt, double mz );

			/**			
			 * @fn double convertMz( double rt, double mz )
			 * @brief gets aligned m/z
			 * @param[in] rt RT before alignment
			 * @param[in] mz m/z before alignment
			 * @return aligned m/z
			 */
			double convertMz( double rt, double mz );

			/**
			 * @fn double invertRt( double rt, double mz )
			 * @brief gets RT before alignment
			 * @param[in] rt aligned RT
			 * @param[in] mz aligned m/z
			 * @return RT before alignment
			 */
			double invertRt( double rt, double mz );

			/**
			 * @fn double invertMz( double rt, double mz )
			 * @brief gets m/z before alignment
			 * @param[in] rt aligned RT
			 * @param[in] mz aligned m/z
			 * @return m/z before alignemtn
			 */
			double invertMz( double rt, double mz );
		};
	}
}


#endif		// __KOME_CLR_ALIGNMENT_WRAPPER_H__

