/**
 * @file TextManager.h
 * @brief interfaces of TextManager class
 *
 * @author S.Tanaka
 * @date 2010.01.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_TEXT_MANAGER_H__
#define __KOME_IO_TEXT_MANAGER_H__


#include <set>


namespace kome {
	namespace io {
		namespace text {
			class TextSampleSet;

			/**
			 * @class TextManager
			 * @brief Text output management class
			 */
			class TextManager {
			protected:
				/**
				 * @fn TextManager()
				 * @brief constructor
				 */
				TextManager();

				/**
				 * @fn virtual ~TextManager()
				 * @brief destructor
				 */
				virtual ~TextManager();

			protected:
				/** the set of text sample set */
				std::set< TextSampleSet* > m_fileSet;

			public:
				/**
				 * @fn void addSampleSet( TextSampleSet* sampleSet )
				 * @brief adds sample set
				 * @param sampeSet sample set object to be added
				 */
				void addSampleSet( TextSampleSet* sampleSet );

				/**
				 * @fn void deleteSampleSet( TextSampleSet* sampleSet )
				 * @brief deletes sample set
				 * @param sampleSet sample set object to be deleted
				 */
				void deleteSampleSet( TextSampleSet* sampleSet );

			public:
				/**
				 * @fn static TextManager& getInstance()
				 * @brief gets Text manager object
				 * @return Text manager object (This is the only object.)
				 */
				static TextManager& getInstance();
			};
		}
	}
}


#endif	// __KOME_IO_TEXT_MANAGER_H__
