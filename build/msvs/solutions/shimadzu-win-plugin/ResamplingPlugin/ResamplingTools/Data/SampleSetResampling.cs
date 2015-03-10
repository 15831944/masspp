/// <summary>
/// [FILE] SampleSetResampling.cs
/// [ABSTRACT] Sample set for Resampling class
/// Copyright (C) 2013-08-1 Shimadzu
/// </summary>
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using kome.clr;

namespace ResamplingPlugin.ResamplingTools.Data
{
    class SampleSetResampling : ClrSampleSetBase
    {
        #region --- Public methods ------------------------------------

        /// <summary>
        /// onOpenFile
        /// </summary>
        /// <param name="path"></param>
        /// <returns></returns>
        public override bool onOpenFile(string path)
        {
            //No additional processing are required for this class derivation.
            return true;
        }

        /// <summary>
        /// onCloseFile
        /// </summary>
        /// <returns></returns>
        public override bool onCloseFile()
        {
            //No additional processing are required for this class derivation.
            return true;
        }

        /// <summary>
        /// onOpenSample
        /// </summary>
        /// <param name="sample"></param>
        /// <returns></returns>
        public override bool onOpenSample(SampleWrapper sample)
        {
            //No additional processing are required for this class derivation.
            return true;
        }

        /// <summary>
        /// onCloseSample
        /// </summary>
        /// <param name="sample"></param>
        /// <returns></returns>
        public override bool onCloseSample(SampleWrapper sample)
        {
            //No additional processing are required for this class derivation.
            return true;
        }
        #endregion
    }
}
