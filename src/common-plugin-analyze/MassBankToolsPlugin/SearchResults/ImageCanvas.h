/**
 * @file ImageCanvas.h
 * @brief Chemical structural formula drow canvas
 *
 * @author M.Fukuda
 * @date 2013.12.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

// ó^Ç¶ÇÁÇÍÇΩ ç\ë¢éÆÇÃPNGÇï`âÊÇ∑ÇÈ
#pragma once

class wxImage;

class ImageCanvas : public kome::window::Canvas {
public:
	ImageCanvas(
		wxWindow* parent,
		const int width,
		const int height
		);

	~ImageCanvas();

private:
	// Uncopyable
	ImageCanvas(const ImageCanvas&);
	ImageCanvas operator=(const ImageCanvas&); 

private:
	std::unique_ptr<wxImage> image_;
	wxImage imageCv_;
	wxWindow* parent_;
	int old_width_;
	int old_height_;
	bool hasPngFg_;

public:
	void setImageFile(const std::string& path);
	void resizeMe(const int width, const int height);
	void resizeAuto(void);

protected:
	virtual void onDraw(wxDC& dc);

	virtual void onSize(
		const unsigned int width, 
		const unsigned int height);

	void doResize(
		wxSize& wsize,
		const bool evtFg);

	void onResized(
		const unsigned int width, 
		const unsigned int height);


private:
	DECLARE_EVENT_TABLE()
};
