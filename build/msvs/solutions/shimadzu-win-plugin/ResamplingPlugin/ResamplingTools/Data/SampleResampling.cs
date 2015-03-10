/// <summary>
/// [FILE] SampleResampling.cs
/// [ABSTRACT] Sample for Resampling class
/// Copyright (C) 2013-08-1 Shimadzu
/// </summary>
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using kome.clr;

namespace ResamplingPlugin.ResamplingTools.Data
{
    /// <summary>
    /// Sample class for Resampling class
    /// </summary>
    class SampleResampling : ClrSampleBase
    {
        #region --- Construction ---------------------------------------
        /// <summary>
        /// Initializes a new instance of the SampleResampling class
        /// </summary>
        /// <param name="set">SampleSetWrapper</param>
        public SampleResampling(SampleSetWrapper set)
            : base(set)
        {
            //No additional processing are required for this class derivation.
        }
        #endregion

        #region --- Public methods ------------------------------------

        /// <summary>
        /// onOpenSample
        /// </summary>
        /// <param name="rootGroup"></param>
        /// <returns></returns>
        public override bool onOpenSample(DataGroupNodeWrapper rootGroup)
        {
            //No additional processing are required for this class derivation.
            return true;
        }

        /// <summary>
        /// onCloseSample
        /// </summary>
        /// <returns></returns>
        public override bool onCloseSample()
        {
            //No additional processing are required for this class derivation.
            return true;
        }
        #endregion
    }
}
