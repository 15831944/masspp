/// <summary>
/// [FILE] ResultSpectrumCalculation.cs
/// [ABSTRACT] Resampling plugin - Data class for Spectrum Calculation Result
/// Copyright (C) 2013-08-01 Shimadzu
/// </summary>
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ResamplingPlugin.ResamplingTools.Data;
using System.Collections.ObjectModel;
using kome.clr;

namespace ResamplingPlugin.SpectrumCalculation.Data
{
    /// <summary>ResultSpectrumCalculation class</summary>
    class SpectrumCalculationResult
    {
        #region --- Properties -----------------------------------------

        /// <summary>
        /// Gets the read only list of Spectrums.
        /// </summary>
        public ReadOnlyCollection<SpectrumWrapper> Spectrums
        {
            get
            {
                return new ReadOnlyCollection<SpectrumWrapper>(_list);
            }
        }

        #endregion

        #region --- Variables ------------------------------------------

        /// <summary>
        /// List of result spectrums.
        /// </summary>
        List<SpectrumWrapper> _list = new List<SpectrumWrapper>();

        #endregion

        #region --- Public Methods -------------------------------------

        /// <summary>
        /// Add result data points(added data is clone).
        /// </summary>
        /// <param name="points"></param>
        public void AddResult(ClrDataPoints points, SpectrumWrapper baseSpectrum, string name)
        {
            SpectrumResampling spectrum = new SpectrumResampling(baseSpectrum.getSample(), name);
            spectrum.SetData(points);
            _list.Add(spectrum);

        }

        #endregion

    }
}
