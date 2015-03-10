/**
 * @file RecordElemSubTags.h
 * @brief AC,MS属性のサブタグ情報等を定義・作成する
 *
 * @author M.Fukuda
 * @date 2013.01.18
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

class RecordElemSubTags {
	typedef std::vector<std::string> StrArr;
public:
	RecordElemSubTags(){}
	~RecordElemSubTags(){}
private:
	StrArr specTags_;
	StrArr specNGTags_;
	StrArr chromTags_;
	StrArr chromNGTags_;
	StrArr ionTags_;
	StrArr ionNGTags_;
	StrArr IonArr_;
public:

private:
	void initMe();
	void initSubSpec();
	void initSubChrom();
	void initSubIon();
	void initIonTypes();
};
