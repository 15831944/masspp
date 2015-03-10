using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using kome.clr;

namespace ResamplingPlugin.ResamplingTools.Data
{
    class ResamplingResult : IDisposable
    {
        #region --- Variables ------------------------------------------
        /// <summary>xyData</summary>
        private List<ClrDataPoints> _xyDataList = new List<ClrDataPoints>();
        #endregion

        #region --- Construction ---------------------------------------

        ~ResamplingResult()
        {
            
        }

        #endregion

        #region --- Public Methods -------------------------------------

        /// <summary>
        /// add xyData to XYDataWrapper list and RT data list
        /// x arrengement number and y arrengement number have to be same
        /// </summary>
        /// <param name="x">x data arrengement</param>
        /// <param name="y">y data arrengement</param>
        /// <returns>ture:success/false:fail</returns>
        public bool AddData(double[] x, double[] y)
        {
            ClrDataPoints xyData = new ClrDataPoints();
            // Resampling Results are same number
            if (x.Length != y.Length)
                return false;
            if (_xyDataList.Count > 0)
            {
                if (x.Length != _xyDataList[0].getLength())
                    return false;
            }

            for (int n = 0; n < x.Length; n++)
            {
                xyData.addPoint(x[n], y[n]);
            }
            _xyDataList.Add(xyData);
            return true;
        }

        /// <summary>
        /// get all result datas
        /// </summary>
        /// <returns>List ClrDataPoints</returns>
        public List<ClrDataPoints> GetAllResultData()
        {
            return _xyDataList;
        }

        /// <summary>
        /// get result data (for index)
        /// </summary>
        /// <param name="index">index</param>
        /// <returns>ClrDataPoints</returns>
        public ClrDataPoints GetResultData(int index)
        {
            return _xyDataList[index];
        }

        /// <summary>
        /// get number of XYDataWrapper list
        /// </summary>
        /// <returns></returns>
        public int GetXYDataNum()
        {
            return _xyDataList.Count;
        }

        /// <summary> clear XYDataWrapper list</summary>
        public void ClearXYDataList()
        {
            foreach (ClrDataPoints pts in _xyDataList)
            {
                pts.clearPoints();
                pts.Dispose();
            }
            _xyDataList.Clear();
        }

        /// <summary>
        /// Dispose.
        /// </summary>
        public void Dispose()
        {
            if (_xyDataList != null)
            {
                foreach (ClrDataPoints pts in _xyDataList)
                {
                    pts.Dispose();
                }
            }
        }

        #endregion

    }
}
