/**
 * @file ImageCanvas.cpp
 * @brief Chemical structural formula drow canvas
 *
 * @author M.Fukuda
 * @date 2013.12.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ImageCanvas.h"

#include "noimage.xpm"

BEGIN_EVENT_TABLE(ImageCanvas, Canvas )
END_EVENT_TABLE()

ImageCanvas::ImageCanvas(
	wxWindow* parent,
	const int width,
	const int height
) : kome::window::Canvas(parent, wxID_ANY, width, height)
	, parent_(parent)
	, image_(new wxImage(noimage_xpm))
	, imageCv_(noimage_xpm)
	, old_width_(0)
	, old_height_(0)
	, hasPngFg_(false)
{}

ImageCanvas::~ImageCanvas()
{}

void ImageCanvas::setImageFile(const std::string& path)
{
	bool ret = false;
	if(!path.empty()) {
//		image_.release();
		ret = image_->LoadFile(wxT(path), wxBITMAP_TYPE_PNG);
		// これが実質キャンバスのサイズになる。
		// 遠くになると切れてしまうらしい。
		image_->Resize(wxSize(400,300),wxPoint(-1,-1));
	}
	if(ret) {
		hasPngFg_ = true;
	} else {
		image_.release();
		image_.reset(new wxImage(noimage_xpm));
		hasPngFg_ = false;
	}
	doResize(GetSize(),false);
	Refresh();
}

void ImageCanvas::resizeMe(
	const int width, 
	const int height
){
}

void ImageCanvas::resizeAuto(void)
{
}

void ImageCanvas::onDraw(wxDC& dc)
{
	dc.DrawBitmap(imageCv_, 0, 0);
}

void ImageCanvas::onSize(
		const unsigned int width, 
		const unsigned int height
){
	if(hasPngFg_) {
		doResize(GetSize(),true);
	}
}

void ImageCanvas::doResize(
	wxSize& wsize,
	const bool evtFg
){
	int c_width  = wsize.GetWidth();
	int c_height  = wsize.GetHeight();
	if (c_width > 0 && c_height > 0) {
		if(evtFg && c_width == old_width_ && c_height == old_height_) {
			return;
		}
		onResized(c_width,c_height);
	}
}

void ImageCanvas::onResized(
	const unsigned int width, 
	const unsigned int height
){
	old_width_ = width;
	old_height_ = height;
	imageCv_.Destroy();
	imageCv_ = image_->Scale(old_width_,old_height_,wxIMAGE_QUALITY_HIGH);	
//	image_->Rescale(old_width_,old_height_,wxIMAGE_QUALITY_HIGH);	
//	image_->Rescale(old_width_,old_height_);	
	Refresh();
}

