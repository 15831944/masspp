/**
* @file SampleReader.cpp
* @brief Sample Reader
* @author OKADA, H.
* @date 2012/07/18
* Copyright (C) 2014 Shimadzu Corporation All rights reserved.
* <補足説明>
*/


#include "SampleReader.h"


using kome::objects::SampleReader;


// constructor
SampleReader::SampleReader(){
}


// destructor
SampleReader::~SampleReader(){
}



// キーを選択する。複数指定の場合はカンマ( , ) 区切りとする。
std::string SampleReader::selectKeys( void ){

	std::string strTemp;

    try {
        strTemp = onSelectKeys();
    } catch (const std::runtime_error& ex) {
        strTemp = ex.what();

		//cerr << ex.what() << endl;
    }
	catch (...) {
		strTemp = "";
	}


	return strTemp;

};


// キーを引数としてサンプルを開く。
kome::objects::Sample* SampleReader::openData( std::string strKey ){
	return onOpenData( strKey );
};


