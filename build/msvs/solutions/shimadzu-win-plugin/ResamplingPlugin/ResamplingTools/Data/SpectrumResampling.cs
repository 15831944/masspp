/// <summary>
/// [FILE] SpectrumResampling.cs
/// [ABSTRACT]  Resampling plugin - Spectrum for Resampling class
/// Copyright (C) 2013-08-1 Shimadzu
/// </summary>

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using kome.clr;

namespace ResamplingPlugin.ResamplingTools.Data
{
    class SpectrumResampling : ClrSpectrumBase
    {
        #region --- Variables ------------------------------------------
        /// <summary>ClrDataPoints</summary> 
        private ClrDataPoints _pts = null;
        #endregion

        #region --- Construction ---------------------------------------
        /// <summary>
        /// Initializes a new instance of the SpectrumResampling class
        /// </summary>
        /// <param name="sample">SampleWrapper</param>
        /// <param name="name">Spectrum name</param>
        public SpectrumResampling(SampleWrapper sample, string name)
            : base(sample, name)
        {
        }
        #endregion

        #region --- Public methods ------------------------------------

        /// <summary>
        /// add xySetData to XYDataWrapper class
        /// add data that set before calling this method to XYDataWrapper class
        /// between min and max
        /// </summary>
        /// <param name="xyData">add to class</param>
        /// <param name="minX">min(if this argument is below 0,min is not checked)</param>
        /// <param name="maxX">max(if this argument is below 0,max is not checked)</param>
        public override void onGetXYData(XYDataWrapper xyData, double minX, double maxX)
        {
            uint num;
            if (_pts == null)
                return;
            xyData.clearPoints();
            num = _pts.getLength();
            xyData.reserve(num);
            for (uint n = 0; n < num; n++)
            {
                if (((minX < 0) || (minX <= _pts.getX(n))) && ((maxX < 0) || (_pts.getX(n) <= maxX)))
                {
                    xyData.addPoint(_pts.getX(n), _pts.getY(n));
                }
            }
        }


        /// <summary>
        /// get x range(min x and max x)
        /// </summary>
        /// <param name="minX">min</param>
        /// <param name="maxX">max</param>
        public override void onGetXRange(ref double minX, ref double maxX)
        {
            double minXX = -1.0;
            double maxXX = -1.0;

            if (_pts == null)
                return;

            // get x range
            for (uint n = 0; n < _pts.getLength(); n++)
            {
                if (minXX < 0.0 || _pts.getX(n) < minXX)
                {
                    minXX = _pts.getX(n);
                }
                if (maxXX < 0.0 || _pts.getX(n) > maxXX)
                {
                    maxXX = _pts.getX(n);
                }
            }
            minX = minXX;
            maxX = maxXX;
        }


        /// <summary>
        /// get total intensity(between min and max)
        /// </summary>
        /// <param name="minX">min(if this argument is below 0,min is not checked)</param>
        /// <param name="maxX">max(if this argument is below 0,max is not checked)</param>
        /// <returns>intensity</returns>
        public override double onGetTotalIntensity(double minX, double maxX)
        {
            double intensity = 0.0;

            if (_pts == null)
                return intensity;
            for (uint n = 0; n < _pts.getLength(); n++)
            {
                if (((minX < 0) || (minX <= _pts.getX(n))) && ((maxX < 0) || (_pts.getX(n) <= maxX)))
                {
                    intensity += _pts.getY(n);
                }
            }
            return intensity;
        }

        /// <summary>
        /// get max intensity(between min and max)
        /// </summary>
        /// <param name="minX">min(if this argument is below 0,min is not checked)</param>
        /// <param name="maxX">max(if this argument is below 0,max is not checked)</param>
        /// <returns>intensity</returns>
        public override double onGetMaxIntensity(double minX, double maxX)
        {
            double intensity = double.MinValue;
            if (_pts == null)
                return intensity;
            for (uint n = 0; n < _pts.getLength(); n++)
            {
                if (((minX < 0) || (minX <= _pts.getX(n))) && ((maxX < 0) || (_pts.getX(n) <= maxX)))
                {
                    if (intensity < _pts.getY(n))
                    {
                        intensity = _pts.getY(n);
                    }
                }
            }
            return intensity;
        }


        /// <summary>
        /// set Datapoints for onGetXYData
        /// </summary>
        /// <param name="pts">ClrDataPoints</param>
        public void SetData(ClrDataPoints pts)
        {
            _pts = pts;
        }

        #endregion
    }
}
