/// <summary>
/// [FILE] BaselineSubtractionParameter.cs
/// [ABSTRACT] Resampling plugin - Result class of Remove contaminant peak.
/// Copyright (C) 2013-08-09 Shimadzu
/// </summary>

using System.Collections.Generic;
using System.Collections.ObjectModel;
using kome.clr;
using ResamplingPlugin.ResamplingTools.Dtata;

namespace ResamplingPlugin.RemoveContaminantPeak.Data
{
    /// <summary>
    /// Result class of Remove contaminant peak.
    /// </summary>
    class ResultRemove
    {
        #region --- Variables ------------------------------------------

        /// <summary>
        /// List of result spectrums.
        /// </summary>
        List<SpectrumWrapper> _specList = new List<SpectrumWrapper>();

        /// <summary>
        /// List of result spectrums.
        /// </summary>
        List<ClrDataPoints> _ptsList = new List<ClrDataPoints>();
        
        #endregion

        #region --- Construction ---------------------------------------

        ~ResultRemove()
        {
            ClearResult();
        }

        #endregion

        #region --- Properties -----------------------------------------

        /// <summary>
        /// Gets the read only list of Spectrums.
        /// </summary>
        public List<SpectrumWrapper> Spectrums
        {
            get
            {
                return _specList;
            }
        }

        /// <summary>
        /// Gets the read only list of Spectrums.
        /// </summary>
        public List<ClrDataPoints> DataPoints
        {
            get
            {
                return _ptsList;
            }
        }

        #endregion


        #region --- Public Methods -------------------------------------

        /// <summary>
        /// Add result data points.
        /// </summary>
        /// <param name="points"></param>
        public void AddResult(ClrDataPoints points, SpectrumWrapper baseSpectrum)
        {
            _ptsList.Add(points);
            _specList.Add(baseSpectrum);
        }

        /// <summary>
        /// Clear all result.
        /// </summary>
        public void ClearResult()
        {
            foreach (SpectrumWrapper sw in _specList)
            {
                sw.deleteXYData();
                sw.Dispose();
            }
            foreach (ClrDataPoints pts in _ptsList)
            {
                pts.clearPoints();
                pts.Dispose();
            }
        }

        #endregion

    }
}
