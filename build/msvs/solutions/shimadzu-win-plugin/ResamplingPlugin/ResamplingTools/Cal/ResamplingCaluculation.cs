/// <summary>
/// [FILE] SpectrumResampling.cs
/// [ABSTRACT]  Resampling plugin - Calculate resampling class.
/// Copyright (C) 2013-08-1 Shimadzu
/// </summary>

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using kome.clr;
using ResamplingPlugin.ResamplingTools.Data;
using System.Runtime.InteropServices;
using System.IO;

namespace ResamplingPlugin.ResamplingTools.Cal
{
    /// <summary>
    /// Calculate resampling class.
    /// </summary>
    class ResamplingCalculation
    {
        #region --- Constants ------------------------------------------

        /// <summary> Types of Interpolate. </summary>
        enum InterpolateMethod
        {
            /// <summary>Nearest</summary>
            NEAREST,
            /// <summary>linear interpolation </summary>
            LINEAR,
            /// <summary>Cubic spline data interpolation</summary>
            SPLINE,
            ///<summary>Piecewise Cubic Hermite Interpolating Polynomial</summary>
            PCHIP
        }

        /// <summary> Types of Extrapolate. </summary>
        enum ExtrapolateType
        {
            /// <summary> Fill Nan </summary>
            FILL_NAN,
            /// <summary> Extrapolate </summary>
            EXTRAPOLATE,
            /// <summary> Fill Zero</summary>
            FILL_ZERO
        }

        /// <summary> progress ID </summary>
        public enum PROGRESS_RESAMPLING_PHASE
        {
            /// <summary>Resampling Start</summary>
            RESAMPLING_START,
            /// <summary>Resampling</summary>
            RESAMPLING_DOING,
            /// <summary>Resampling End</summary>
            RESAMPLING_END,
        }
        #endregion

        #region --- Variables ------------------------------------------

        /// <summary>Result Resampling</summary>
        private ResamplingResult _result = new ResamplingResult();

        /// <summary>
        /// Parameters of resampling.
        /// </summary>
        private ResamplingParameter _param = null;

        #endregion

        #region --- Properties -----------------------------------------

        /// <summary>
        /// Gets a result of resampling.
        /// </summary>
        public ResamplingResult Result
        {
            get { return _result; }
        }

        /// <summary>
        /// Gets a number of completed resampling..
        /// </summary>
        public int Progress
        {
            get;
            private set;
        }

        /// <summary>
        /// Gets and sets a value indicating resampling mode.
        /// </summary>
        public bool StopFlag
        {
            get;
            set;
        }

        #endregion

        #region --- Events ---------------------------------------------
        /// <summary> Event for Update Progressbar (Resampling)
        ///           (PROGRESS ID,max or current spectrum number,current sample number,max sample number)
        /// </summary>
        public event Action<PROGRESS_RESAMPLING_PHASE, int, int, int> UpdateResamplingProgress;
        #endregion

        #region --- Construction ---------------------------------------

        /// <summary>
        /// Initializes a new instance of the ResamplingCalculation class.
        /// </summary>
        /// <param name="param"></param>
        public ResamplingCalculation(ResamplingParameter param)
        {
            if (param == null)
            {
                throw new ArgumentNullException("param");
            }
            _param = param;
        }

        #endregion

        #region --- Public Methods -------------------------------------

        /// <summary>
        /// Execute Resampling.
        /// </summary>
        public void Execute()
        {
            Progress = 0;

            if (_param == null)
            {
                return;
            }

            if (_result != null)
            {
                _result.Dispose();
            }

            _result = new ResamplingResult();

            //Calculate resampling point.
            double[] xi = CreateResamplingPoint();
            if (xi == null)
            {
                return;
            }
            for (int i = 0; i < _param.GetXYDataNum(); i++)
            {
                if (StopFlag)
                {
                    return;
                }
                ClrDataPoints xyData = _param.GetXYData(i);

                double[] yi;
                double[,] data;
                yi = new double[xi.Length];
                ConvertXYDataToXYArray(xyData, out data);
                InterpolateMethod method = _param.Interpolation ==
                    ResamplingParameter.InterpolationAlgorithm.LINEAR ?
                    InterpolateMethod.LINEAR : InterpolateMethod.PCHIP;

                if (UpdateResamplingProgress != null)
                {
                    UpdateResamplingProgress(PROGRESS_RESAMPLING_PHASE.RESAMPLING_START, 0, i + 1, _param.GetXYDataNum());
                }
                WrapMatlabInterp1(yi, data.GetLength(1), data, xi.Length, xi, method, ExtrapolateType.FILL_ZERO);
                _result.AddData(xi, yi);
                if (UpdateResamplingProgress != null)
                {
                    UpdateResamplingProgress(PROGRESS_RESAMPLING_PHASE.RESAMPLING_DOING, 0, i + 1, _param.GetXYDataNum());
                }

                Progress++;
            }
            if (UpdateResamplingProgress != null)
            {
                UpdateResamplingProgress(PROGRESS_RESAMPLING_PHASE.RESAMPLING_END, 0, _param.GetXYDataNum(), _param.GetXYDataNum());
            }
        }

        #endregion

        #region --- Private Methods -------------------------------------

        /// <summary>
        /// Execute Resampling.
        /// </summary>
        /// <param name="interYArray">output array</param>
        /// <param name="dataSize">count of input data</param>
        /// <param name="dataArray">input data</param>
        /// <param name="interSize">count of output sampling point</param>
        /// <param name="interXArray">output sampling points</param>
        /// <param name="Method">inteporlate method</param>
        /// <param name="Extrap_value">extrapolate type</param>
        [DllImport("wrap_call_Interp1.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void WrapMatlabInterp1(double[] interYArray, int dataSize, double[,] dataArray, int interSize, double[] interXArray,
            InterpolateMethod Method, ExtrapolateType Extrap_value);

        /// <summary>
        /// Convert XYData to 2 dimensions array.
        /// </summary>
        /// <param name="xydata">XYData</param>
        /// <param name="dataArray">output Data Array.(result:{{x1, x2, …, xn},{y1, y2, …, yn}})</param>
        private void ConvertXYDataToXYArray(
            XYDataWrapper xydata, out double[,] dataArray)
        {
            List<double[]> list = new List<double[]>();
            for (uint i = 0; i < xydata.getLength(); i++)
            {
                list.Add(new double[] { xydata.getX(i), xydata.getY(i) });
            }
            list.Sort((a, b) => a[0].CompareTo(b[0]));
            var q = from p in list
                    group p[1] by p[0];

            dataArray = new double[2, q.Count()];

            int index = 0;

            foreach (var r in q)
            {
                dataArray[0, index] = r.Key;
                dataArray[1, index] = (from s in r select s).Average();
                index++;
            }
        }

        /// <summary>
        /// Create Resampling point.
        /// </summary>
        /// <returns>Resampling point.</returns>
        private double[] CreateResamplingPoint()
        {
            double[] result = null;

            switch (_param.ResamplingMode)
            {
                case ResamplingParameter.SamplingMode.FIRST:
                    result = CreateSameArrayFirstSpectrumDifference();
                    break;
                case ResamplingParameter.SamplingMode.MINIMUIM:
                case ResamplingParameter.SamplingMode.AVERAGE:
                case ResamplingParameter.SamplingMode.SPECIFIED:
                    result = CreateCommonDifferenceArray();
                    break;
                default:
                    throw new ArgumentException();
            }

            return result;
        }

        /// <summary>
        /// Create Same array that first spectrum difference.
        /// </summary>
        /// <returns></returns>
        private double[] CreateSameArrayFirstSpectrumDifference()
        {
            ClrDataPoints xydata = _param.GetXYData(0);
            if (xydata == null)
            {
                return null;
            }
            uint length = xydata.getLength();
            uint count = 0;
            double lower = GetLowerOutputRange();
            double higher = GetHigherOutputRange();
            for (uint i = 0; i < length; i++)
            {
                if ((xydata.getX(i) < lower) || (higher < xydata.getX(i)))
                {
                    continue;
                }
                if (i == 0)
                {
                    count++;
                }
                else if (xydata.getX(i) != xydata.getX(i - 1))
                {
                    count++;
                }
            }
            if (count == 0)
            {
                return null;
            }

            double[] result = new double[count];
            uint index = 0;
            for (uint i = 0; i < length; i++)
            {
                if ((xydata.getX(i) < lower) || (higher < xydata.getX(i)))
                {
                    continue;
                }
                if ((xydata.getX(i) == 0)
                    || !result.Contains(xydata.getX(i)))
                {
                    result[index++] = xydata.getX(i);
                }
            }
            return result;
        }

        /// <summary>
        /// Create common difference array.
        /// </summary>
        /// <returns>Array</returns>
        private double[] CreateCommonDifferenceArray()
        {
            if ((_param.ResamplingMode == ResamplingParameter.SamplingMode.SPECIFIED)
                && !_param.SpecifiedInterval.HasValue)
            {
                throw new ArgumentNullException("SpecifiedInterval");
            }

            double lower = GetLowerOutputRange();
            double higher = GetHigherOutputRange();
            double interval;
            switch (_param.ResamplingMode)
            {
                case ResamplingParameter.SamplingMode.AVERAGE:
                    interval = GetAverageInterval();
                    break;
                case ResamplingParameter.SamplingMode.MINIMUIM:
                    interval = GetMinimumInterval();
                    break;
                case ResamplingParameter.SamplingMode.SPECIFIED:
                    interval = _param.SpecifiedInterval.Value;
                    break;
                default:
                    throw new ArgumentException("ResamplingMode");
            }

            if ((higher <= lower) || (interval <= 0))
            {
                throw new ArgumentException("Outputrange or interval");
            }

            uint length = (uint)(Math.Floor((higher - lower) / interval)) + 1;
            double[] result = new double[length];

            uint index = 0;
            double value = lower;
            while (value <= higher)
            {
                result[index++] = value;
                value = lower + index * interval;
            }
            return result;
        }

        /// <summary>
        /// Gets the interval average of all spectrums.
        /// </summary>
        /// <returns>Interval</returns>
        private double GetAverageInterval()
        {
            double sum = 0;
            int count = 0;

            for (int i = 0; i < _param.GetXYDataNum(); i++)
            {
                ClrDataPoints xydata = _param.GetXYData(i);
                if (xydata.getLength() <= 1)
                {
                    continue;
                }

                uint length = xydata.getLength();
                for (uint j = 1; j < length; j++)
                {
                    double distance = Math.Abs(xydata.getX(j) - xydata.getX(j - 1));
                    if (distance == 0)
                    {
                        continue;
                    }
                    sum += distance;
                    count++;
                }
            }
            return sum / count;
        }

        /// <summary>
        /// Gets the interval minimum of all spectrums.
        /// </summary>
        /// <returns>Interval</returns>
        private double GetMinimumInterval()
        {
            double result = double.MaxValue;

            for (int i = 0; i < _param.GetXYDataNum(); i++)
            {
                ClrDataPoints xydata = _param.GetXYData(i);
                uint length = xydata.getLength();
                for (uint j = 1; j < length; j++)
                {
                    double distance = Math.Abs(xydata.getX(j) - xydata.getX(j - 1));
                    if (distance == 0)
                    {
                        continue;
                    }
                    result = Math.Min(distance, result);
                }

            }

            return result;
        }

        /// <summary>
        /// Get Lower output range.
        /// </summary>
        /// <returns>Output range.</returns>
        private double GetLowerOutputRange()
        {
            if (_param.OutputLowerRange.HasValue)
            {
                return _param.OutputLowerRange.Value;
            }

            double range = _param.GetXYData(0).getMinX();
            
            if (_param.ResamplingMode != ResamplingParameter.SamplingMode.FIRST)
            {
                for (int i = 1; i < _param.GetXYDataNum(); i++)
                {
                    double value = _param.GetXYData(i).getMinX();
                    if (value < range)
                    {
                        range = value;
                    }
                }
            }

            return range;
        }

        /// <summary>
        /// Get Higher output range.
        /// </summary>
        /// <returns>Output range.</returns>
        private double GetHigherOutputRange()
        {
            if (_param.OutputHigherRange.HasValue)
            {
                return _param.OutputHigherRange.Value;
            }

            double range = _param.GetXYData(0).getMaxX();

            if (_param.ResamplingMode != ResamplingParameter.SamplingMode.FIRST)
            {
                for (int i = 1; i < _param.GetXYDataNum(); i++)
                {
                    double value = _param.GetXYData(i).getMaxX();
                    if (range < value)
                    {
                        range = value;
                    }
                }
            }

            return range;
        }

        #endregion

    }
}
