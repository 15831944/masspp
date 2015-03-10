/**
* @file FileSampleReader.h
* @brief File Sample Reader
* @author OKADA, H.
* @date 2012/07/20
* Copyright (C) 2014 Shimadzu Corporation All rights reserved.
*/


#ifndef _FILE_SAMPLE_READER_
#define _FILE_SAMPLE_READER_

#include "stdafx.h"

// ファイル名を複数指定する際の区切り文字
#define STR_FILE_STRINGS_SEPARATOR	"|"	

namespace kome {
	namespace io {

		/** 
		 * @class FileSampleReader
		 * @ brief File Sample Reader class
		 */
		class FileSampleReader : public kome::objects::SampleReader{
			
		public:
			/**
			 * @fn FileSampleReader();
			 * @brief constructor
			 */
			FileSampleReader();
			
			/**
			 * @fn ~FileSampleReader();
			 * @brief destructor
			 */
			virtual ~FileSampleReader();
			
		private:

		protected:

			/**
			 * @fn std::string onSelectKeys( void )
			 * @brief RawDataFile ダイアログを開きファイルパスを1個以上選択する。各々のキーをファイルパスとする。
			 */
			std::string onSelectKeys( void );

			/**
			 * @fn kome::objects::Sample* onOpenData( std::string strKey )
			 * @brief キーをファイルパスとしてサンプルを取得する。
			 * @param strKey
			 */
			kome::objects::Sample* onOpenData( std::string strKey );

		};
	}
}


#endif
