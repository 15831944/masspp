/// <summary>
/// [FILE] WellInfo.cs
/// [ABSTRACT] Well情報格納クラス
/// Copyright (C) 2012/10/29	Yosihiro Yamada@KTLAST Shimadzu Corp.
/// </summary>
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace LdiaTools.AcqResults
{
	/// <summary>
	/// wellの情報
	/// </summary>
	internal struct WellInfo
	{
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="wellName">well名</param>
		/// <param name="rt">RT</param>
        /// <param name="locationPos">well位置</param>
        public WellInfo(string wellName, double rt, PointF locationPos)
		{
			_wellName = wellName.Trim();
			_rt = rt;
            _locationPos = locationPos;
		}

		/// <summary> well名 </summary>
		public string WellName { get { return _wellName; } }

		/// <summary> RT (retentuon time)  </summary>
		public double RT { get { return _rt; } }

        /// <summary> well位置  </summary>
        public PointF LOCATION_POS { get { return _locationPos; } }

		/// <summary>
		/// 文字列変換
		/// </summary>
		/// <returns>Well名、RT</returns>
		public override string ToString()
		{
			string str = string.Format("{0}, {1}", this.WellName, this.RT);

			return str;
		}

		#region private fileds
		/* well name of MS1 spectrum*/
		private readonly string _wellName;

        /* RT (retentuon time) */
        private readonly double _rt;

        /* well位置 */
        private readonly PointF _locationPos;
        #endregion
	};
}

