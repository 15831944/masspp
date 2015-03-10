/// <summary>
/// [FILE] SpectrumCalculation.cs
/// [ABSTRACT]  Resampling plugin - Calculate resampling class.
/// Copyright (C) 2013-08-1 Shimadzu
/// </summary>

using System;
using System.Collections.Generic;
using kome.clr;
using ResamplingPlugin.ResamplingTools.Cal;
using ResamplingPlugin.ResamplingTools.Data;
using ResamplingPlugin.SpectrumCalculation.Data;

namespace ResamplingPlugin.SpectrumCalculation.Cal
{
    /// <summary>Spectrum Calculation class</summary>
    class SpectrumCalculation
    {
        #region --- Constants --------------------------------------------
        /// <summary>operation settings</summary>
        public enum OperationSettings
        {
            /// <summary>Average</summary>
            AVERAGE = 0,
            /// <summary>Sum</summary>
            SUM,
            /// <summary>Subtract from first spectrum</summary>
            SUBTRACT,
            /// <summary>Divide by first spectrum</summary>
            DIVIDE
        }

        /// <summary>
        /// Averaged spectrum name.
        /// </summary>
        private const string SPECTRUMNAME_AVERAGED = "Average";

        /// <summary>
        /// Summed spectrum name.
        /// </summary>
        private const string SPECTRUMNAME_SUMMED = "Summed";

        /// <summary>
        /// Subtracted spectrum name.
        /// </summary>
        private const string SPECTRUMNAME_SUBTRACTED = "Subtracted";

        /// <summary>
        /// Divided spectrum name.
        /// </summary>
        private const string SPECTRUMNAME_DIVIDED = "Divided";

        #endregion

        #region --- Variables ------------------------------------------
        /// <summary>ResamplingPrameter class</summary>
        ResamplingParameter _param;
        /// <summary>
        /// ResamplingCaluculation class</summary>
        ResamplingCalculation _resampCal;
        /// <summary>SpectrumWrapper List</summary>
        private List<SpectrumWrapper> _specList = new List<SpectrumWrapper>();
        /// <summary>Operation Setting</summary>
        private OperationSettings _oprationSetting = OperationSettings.AVERAGE;
        /// <summary>Progress Stop Flag</summary>
        private bool _stopFlag = false;
        private SpectrumCalculationResult _resultSpecCal;
        #endregion

        #region --- Construction ---------------------------------------
        /// <summary>
        /// Initializes a new instance of the SpectrumCalculation class
        /// </summary>
        /// <param name="pram">ResamplingPrameter</param>
        public SpectrumCalculation(ResamplingParameter param)
        {
            _param = param;
            _resampCal = new ResamplingCalculation(param);
        }

        #endregion

        #region --- Properties -----------------------------------------

        /// <summary>Gets and sets a value indicating resampling mode.</summary>
        public OperationSettings OperationSetting
        {
            get { return _oprationSetting; }
            set { _oprationSetting = value; }
        }

        /// <summary>Thread Stop</summary>
        public bool IsThreadStopping
        {
            get { return _stopFlag; }
            set { _stopFlag = value; _resampCal.StopFlag = value; }
        }

        /// <summary>get ResamplingCaluculation instance</summary>
        public ResamplingCalculation GetResamplingCalculation
        {
            get { return _resampCal; }
        }


        /// <summary>get ResultSpecCalculation instance</summary>
        public SpectrumCalculationResult GetResultSpecCalculation
        {
            get { return _resultSpecCal; }
        }

        #endregion

        #region --- Public methods ------------------------------------

        /// <summary>Execute</summary>
        public void Execute()
        {
            _resultSpecCal = null;
            SetXYDataToResamplingCalculation();
            if (_stopFlag == true)
                return;
            _resampCal.Execute();
            if (_stopFlag == true)
                return;
            ResamplingResult resultRs = _resampCal.Result;
            _resultSpecCal = Operation(resultRs);
            if (_stopFlag == true)
                return;

        }

        /// <summary>
        /// add spec to SpectrumWrapper list
        /// </summary>
        /// <param name="spec">SpectrumWrapper</param>
        public void AddSpecData(SpectrumWrapper spec)
        {
            _specList.Add(spec);
        }

        /// <summary>
        /// get number of SpectrumWrapper list
        /// </summary>
        /// <returns></returns>
        public int GetSpecDataNum()
        {
            return _specList.Count;
        }

        /// <summary>
        /// clear SpectrumWrapper list
        /// </summary>
        public void ClearSpecData()
        {
            _specList.Clear();
        }

        /// <summary>
        /// Display Calculation Results on tree view
        /// </summary>
        public void DisplayCalculationResult()
        {
            // No Result
            if (_resultSpecCal == null)
                return;

            //Add result to sample tree.
            SampleSetResampling sampleSet = new SampleSetResampling();
            SampleResampling sample = new SampleResampling(sampleSet);
            sample.openSample();
            DataGroupNodeWrapper root = sample.getRootDataGroupNode();

            sample.setName(_resultSpecCal.Spectrums[0].getName() + " Sample");
            root.setName(_resultSpecCal.Spectrums[0].getName() + " Sample");

            int cnt = 0;
            foreach (SpectrumResampling sr in _resultSpecCal.Spectrums)
            {
                ClrDataPoints xyd = new ClrDataPoints();
                SpectrumResampling spec;

                if (_resultSpecCal.Spectrums.Count == 1)
                {
                    spec = new SpectrumResampling(sample, sr.getName() + " Spectrum");
                }
                else
                {
                    cnt++;
                    spec = new SpectrumResampling(sample, sr.getName() + " Spectrum_" + cnt.ToString());
                }
                ClrDataPoints pts = new ClrDataPoints();
                sr.getXYData(pts, false);
                spec.getXYData(xyd, false);
                spec.SetData(pts);
                spec.setMinX(pts.getMinX());
                spec.setMaxX(pts.getMaxX());
                spec.setMaxIntensity(pts.getMaxY());
                spec.onGetXYData(xyd, -1.0, -1.0);
                spec.setMsStage((int)sr.getMsStage());
                sample.setSampleIndex((int)0);
                root.addSpectrum(spec);
            }
            sampleSet.addSample(sample);
            ClrPluginCallTool.onOpenSample(sample);
        }


        #endregion

        #region --- Private methods ------------------------------------

        /// <summary>
        /// Set XYData To Resampling Calculation Class
        /// </summary>
        private void SetXYDataToResamplingCalculation()
        {
            _param.ClearXYDataList();
            foreach (SpectrumWrapper spec in _specList)
            {
                if (_stopFlag == true)
                    break;
                ClrDataPoints pts = new ClrDataPoints();
                spec.getXYData(pts, false);
                _param.AddXYData(pts);
            }
        }

        /// <summary>
        /// Operation(Average/Sum/Subtract/Divide)
        /// </summary>
        /// <param name="result">ResultResampling Class</param>
        /// <returns>ResultSpectrumCalculation</returns>
        private SpectrumCalculationResult Operation(ResamplingResult result)
        {
            switch (this.OperationSetting)
            {
                case OperationSettings.AVERAGE:
                    return OperationAverage(result);
                case OperationSettings.SUM:
                    return OperationSum(result);
                case OperationSettings.SUBTRACT:
                    return OperationSubtract(result);
                case OperationSettings.DIVIDE:
                    return OperationDivide(result);
                default:
                    throw new ArgumentException("OperationSettings");
            }

        }

        /// <summary>
        /// Average Operation
        /// </summary>
        /// <param name="result">ResultResampling Class</param>
        /// <returns>ResultSpectrumCalculation</returns>
        private SpectrumCalculationResult OperationAverage(ResamplingResult result)
        {
            if (!ValidateResult(result))
            {
                return null;
            }
            SpectrumCalculationResult spectrumResult = new SpectrumCalculationResult();
            ClrDataPoints sum = CalculateDataPointsSum(result);
            ClrDataPoints average = new ClrDataPoints();
            for (uint i = 0; i < sum.getLength(); i++)
            {
                if (_stopFlag == true)
                    return null;
                average.addPoint(
                    sum.getX(i),
                    sum.getY(i) / result.GetXYDataNum());
            }
            spectrumResult.AddResult(average, _specList[0], SPECTRUMNAME_AVERAGED);
            return spectrumResult;
        }

        /// <summary>
        /// Sum Operation
        /// </summary>
        /// <param name="result">ResultResampling Class</param>
        /// <returns>ResultSpectrumCalculation</returns>
        private SpectrumCalculationResult OperationSum(ResamplingResult result)
        {
            if (!ValidateResult(result))
            {
                return null;
            }
            SpectrumCalculationResult spectrumResult = new SpectrumCalculationResult();
            spectrumResult.AddResult(CalculateDataPointsSum(result), _specList[0], SPECTRUMNAME_SUMMED);
            if (_stopFlag == true)
                return null;
            return spectrumResult;
        }

        /// <summary>
        /// Subtract Operation
        /// </summary>
        /// <param name="result">ResultResampling Class</param>
        /// <returns>ResultSpectrumCalculation</returns>
        private SpectrumCalculationResult OperationSubtract(ResamplingResult result)
        {
            if (!ValidateResult(result))
            {
                return null;
            }
            SpectrumCalculationResult spectrumResult = new SpectrumCalculationResult();
            ClrDataPoints basePts = result.GetResultData(0);
            List<ClrDataPoints> subtract = new List<ClrDataPoints>();
            for (int i = 1; i < result.GetXYDataNum(); i++)
            {
                ClrDataPoints temp = new ClrDataPoints();
                for (uint j = 0; j < basePts.getLength(); j++)
                {
                    if (_stopFlag == true)
                        return null;
                    temp.addPoint(basePts.getX(j),
                        result.GetResultData(i).getY(j) - basePts.getY(j));
                }
                subtract.Add(temp);
            }
            for (int i = 0; i < subtract.Count; i++)
            {
                if (_stopFlag == true)
                    return null;
                spectrumResult.AddResult(subtract[i], _specList[i + 1], SPECTRUMNAME_SUBTRACTED);
            }
            return spectrumResult;
        }

        /// <summary>
        /// Divide Operation
        /// </summary>
        /// <param name="result">ResultResampling Class</param>
        /// <returns>ResultSpectrumCalculation</returns>
        private SpectrumCalculationResult OperationDivide(ResamplingResult result)
        {
            if (!ValidateResult(result))
            {
                return null;
            }
            SpectrumCalculationResult spectrumResult = new SpectrumCalculationResult();
            ClrDataPoints basePts = new ClrDataPoints();
            ClrDataPoints firstPts = result.GetResultData(0);
            double threshold = firstPts.getMaxY() * Math.Pow(10, -6);
            for (uint i = 0; i < firstPts.getLength(); i++)
            {
                if (_stopFlag == true)
                    return null;
                double value = Math.Max(firstPts.getY(i), threshold);
                basePts.addPoint(firstPts.getX(i), value);
            }

            List<ClrDataPoints> subtract = new List<ClrDataPoints>();
            for (int i = 1; i < result.GetXYDataNum(); i++)
            {
                ClrDataPoints temp = new ClrDataPoints();
                for (uint j = 0; j < basePts.getLength(); j++)
                {
                    if (_stopFlag == true)
                        return null;
                    temp.addPoint(basePts.getX(j),
                        result.GetResultData(i).getY(j) / basePts.getY(j));
                }
                subtract.Add(temp);
            }
            //2013/11/26 - del
            //spectrumResult.AddResult(basePts, _specList[0], SPECTRUMNAME_DIVIDED);
            //enddel
            for (int i = 0; i < subtract.Count; i++)
            {
                if (_stopFlag == true)
                    return null;
                spectrumResult.AddResult(subtract[i], _specList[i + 1], SPECTRUMNAME_DIVIDED);
            }
            return spectrumResult;

        }

        /// <summary>
        /// Calculate Sum of Data Points
        /// </summary>
        /// <param name="result">ResultResampling Class</param>
        /// <returns>ClrDataPoints</returns>
        private ClrDataPoints CalculateDataPointsSum(ResamplingResult result)
        {
            ClrDataPoints sum = result.GetResultData(0);
            ClrDataPoints temp;
            for (int i = 1; i < result.GetXYDataNum(); i++)
            {
                temp = sum;
                sum = new ClrDataPoints();
                ClrDataPoints data = result.GetResultData(i);

                for (uint j = 0; j < data.getLength(); j++)
                {
                    if (_stopFlag == true)
                        return null;
                    sum.addPoint(data.getX(j), data.getY(j) + temp.getY(j));
                }
            }
            return sum;
        }

        /// <summary>
        /// Check ResultResampling.
        /// </summary>
        /// <param name="result">Target of validation</param>
        /// <returns>Result of validation</returns>
        private bool ValidateResult(ResamplingResult result)
        {
            if (result == null)
            {
                return false;
            }

            return (result.GetXYDataNum() != 0);
        }

        #endregion

    }
}
