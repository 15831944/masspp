/**********************************************************
Copyright(C) 2006-2012 Eisai Co., Ltd. All rights reserved.
-----------------------------------------------------------
   @addtogroup C++ CommonFunctions
	@file       Algo_inline.h
	@brief      Lib関数で遅い部分を近似アルゴリズムで高速化
	@date       2012.12.05
	@author     M.Fukuda
	@version    1.0.0
    @note       Boost等、外部ライブラリに関わる処理は入れない。
**********************************************************/


//	std::sqrtの計算が遅いので
//	ニュートン法を使った近似アルゴリズムで計算
//
//	大元の文件
//	http://www.riken.jp/brict/Ijiri/study/fastsqrt.html
//
//	更にその元の論文
//		[1] David Eberly, Fast Inverse Square Root (Revisited), 
//			http://www.geometrictools.com/Documentation/ FastInverseSqrt.pdf, 1/2002-.
//
//  イテレーション２回で　
//    std::sqrtと小数点第6位まで 同じ かつ倍以上速度が出る。
//  イテレーション3回で
//    std::sqrtと小数点第8-9位まで 同じ かつstd::sqrtより速い
//  誤差が許容できて速度が必要なら此方を推奨

// 高速化されている　sqrt
inline double t_sqrtD(const double &x) {
	double xHalf = 0.5 * x;
	long long int tmp = 0x5FE6EB50C7B537AAl - ( *(long long int*)&x >> 1);
	double xRes = * (double*)&tmp;

	//イテレーション
	xRes *= (1.5 - (xHalf * xRes * xRes));
	xRes *= (1.5 - (xHalf * xRes * xRes));
	return xRes * x;
}
