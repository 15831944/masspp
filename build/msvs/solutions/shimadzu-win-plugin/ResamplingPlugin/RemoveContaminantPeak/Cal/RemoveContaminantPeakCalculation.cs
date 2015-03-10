/// <summary>
/// [FILE] RemoveContaminantPeak.cs
/// [ABSTRACT] Resampling plugin - Calculate remove contaminant peak class
/// Copyright (C) 2013-08-02 Shimadzu
/// </summary>

using System;
using System.Collections.Generic;
using System.Linq;
using kome.clr;
using ResamplingPlugin.RemoveContaminantPeak.Data;
using ResamplingPlugin.ResamplingTools.Cal;
using ResamplingPlugin.ResamplingTools.Data;
using System.IO;

namespace ResamplingPlugin.RemoveContaminantPeak.Cal
{
    /// <summary>
    /// Calculate remove contaminant peak class.
    /// </summary>
    class RemoveContaminantPeakCalculation
    {
        #region --- Defines --------------------------------------------

        /// <summary>
        /// Function type string of Baseline subtraction.
        /// </summary>
        private const string FUNCTIONTYPE_BASELINE = "XYDATA_BASELINE";

        /// <summary>
        /// progress ID
        /// </summary>
        public enum PROGRESS_REMOVECONTAMINANT_PHASE
        {
            PREPARE,
            REMOVE_CONTAMINANT_PEAK,
            WRITE_TO_TEMPORARY,
            READ_FROM_TEMPORARY,
            COMPLETE,
            CANCELED
        }

        #endregion

        #region --- Events ---------------------------------------------

        /// <summary> Event for Update Progressbar (Resampling)
        ///           (PROGRESS ID,current spectrum number,max spectrum number)
        /// </summary>
        public event Action<PROGRESS_REMOVECONTAMINANT_PHASE, int, int> UpdateProgress;

        /// <summary> Event for Update Progressbar (Resampling)
        ///           (PROGRESS ID,max or current spectrum number,current sample number,max sample number)
        /// </summary>
        public event Action<ResamplingCalculation.PROGRESS_RESAMPLING_PHASE, int, int, int> UpdateResamplingProgressTemp;


        #endregion

        #region --- Variables ------------------------------------------

        /// <summary>
        /// Target of operation.
        /// </summary>
        protected ClrVariant _clrVariant;

        /// <summary>
        /// Calculator of resampling.
        /// </summary>
        private ResamplingCalculation _resampling;

        /// <summary>
        /// Parameters for resampling.
        /// </summary>
        private ResamplingParameter _resamplingParam;

        /// <summary>
        /// Parameters for baseline subtraction.
        /// </summary>
        private BaselineSubtractionParameter _baselineParam;

        /// <summary>
        /// Thread Stop Flag
        /// </summary>
        private bool _StopFlag;


        #endregion

        #region --- Properties -----------------------------------------

        /// <summary>
        /// Gets Latest result of remove contaminant peak.
        /// </summary>
        public RemoveResult Result
        {
            get;
            protected set;
        }

        /// <summary>
        /// Gets or Sets Stop Flag.
        /// </summary>
        public bool StopFlag
        {
            get { return _StopFlag; }
            set { _StopFlag = value; _resampling.StopFlag = value; }
        }

        #endregion

        #region --- Construction ---------------------------------------

        /// <summary>
        /// Initializes a new instance of the SpectrumCalculationManager class
        /// </summary>
        /// <param name="clrVariant">Target of operation</param>
        /// <exception cref="ArgumentNullException"></exception>
        public RemoveContaminantPeakCalculation(ClrVariant clrVariant,
            BaselineSubtractionParameter baselineParam,
            ResamplingParameter resamplingParam)
        {
            //Each arguments validation.
            if (clrVariant == null)
            {
                throw new ArgumentNullException("clrVariant");
            }
            if (baselineParam == null)
            {
                throw new ArgumentNullException("baselineParam");
            }
            if (resamplingParam == null)
            {
                throw new ArgumentNullException("resamplingParam");
            }
            _clrVariant = clrVariant;
            _resamplingParam = resamplingParam;
            _resampling = new ResamplingCalculation(_resamplingParam);
            _baselineParam = baselineParam;
        }

        ~RemoveContaminantPeakCalculation()
        {
            _resamplingParam.ClearXYDataList();
        }

        #endregion

        #region --- Public Methods -------------------------------------

        /// <summary>
        /// Execute Remove Contaminant Peak.
        /// </summary>
        /// <exception cref="ArgumentException"></exception>
        /// <exception cref="ArgumentNullException"></exception>
        public void Execute()
        {
            //Update Message
            if (UpdateProgress != null)
            {
                UpdateProgress(
                    PROGRESS_REMOVECONTAMINANT_PHASE.PREPARE,
                    (int)0, (int)1);
            }

            //Create Temporary Folder.
            string tempDir = CreateTempDir();
            if (tempDir == null)
            {
                return;
            }

            //Initialize.
            StopFlag = false;
            if (Result != null)
            {
                Result.ClearResult();
            }
            Result = null;

            //Get root node
            ClrMsDataVariant msObj = new ClrMsDataVariant(_clrVariant);
            DataGroupNodeWrapper dgnw = msObj.getSample().getRootDataGroupNode();

            //Get all MS1 Spectrums.
            List<SpectrumWrapper> ms1List = new List<SpectrumWrapper>();
            ReadSpectrum(dgnw, ref ms1List);

            //If count of MS1 Spectrums is 0, quit remove contaminant peak.
            if (ms1List.Count <= 0)
            {
                //Update Message
                if (UpdateProgress != null)
                {
                    UpdateProgress(
                        PROGRESS_REMOVECONTAMINANT_PHASE.CANCELED, 1, 1);
                }
                msObj.Dispose();
                return;
            }

            //Create Resampling parameters.
            foreach (SpectrumWrapper sw in ms1List)
            {
                ClrDataPoints pts = new ClrDataPoints();
                sw.getXYData(pts, false);
                _resamplingParam.AddXYData(pts);
            }

            //Get wrappers
            SettingParameterValuesWrapper spvw = new SettingParameterValuesWrapper();
            PluginFunctionItemWrapper pfiw = PluginManagerWrapper.getInstance().getFunctionItem(
                FUNCTIONTYPE_BASELINE, _baselineParam.Algorithm, null);

            //Convert parameters to string for SettingParameterValuesWrapper.
            string paramString = _baselineParam.ABCWindowWidth.ToString() + ","
                + _baselineParam.NoiseFactor.ToString() + ","
                + _baselineParam.SmoothingWidth.ToString();
            pfiw.getSettingsPage().setParameterString(spvw, paramString);

            if (StopFlag)
            {
                _resamplingParam.ClearXYDataList();
                return;
            }

            _resampling.UpdateResamplingProgress += UpdateSpecCalcResamplingProgress;

            //Resampling.
            _resampling.Execute();

            if (StopFlag)
            {
                _resamplingParam.ClearXYDataList();
                return;
            }

            List<ClrDataPoints> resamplingList = _resampling.Result.GetAllResultData();

            _resampling.UpdateResamplingProgress -= UpdateSpecCalcResamplingProgress;

            //Dispose.
            _resamplingParam.ClearXYDataList();

            //if resampling result is empty, quit remove contaminant peak.
            if (resamplingList.Count == 0)
            {
                _resampling.Result.Dispose();
                return;
            }

            // Countermeasure to OutOfBoundsException.
            // Max of List and Array Index is "int".
            // Max Index of XYDataPoints is bigger than maximum integer.
            // Maximum uint = (Maximum int) * 2 + 1.
            ClrDataPoints[] subtractLowerXICList = null;
            ClrDataPoints[] subtractHigherXICList = null;
            ClrDataPoints subtractHighestXICList = null;

            bool complete = true;
            // Create Baseline subtracted XIC.
            if (CreateBaselineSubtractedXIC(
                pfiw, resamplingList, ms1List, spvw,
                ref subtractLowerXICList,
                ref subtractHigherXICList,
                ref subtractHighestXICList))
            {
                complete = CreateTempXICFiles(subtractLowerXICList,
                    subtractHigherXICList, subtractHighestXICList,
                    resamplingList, tempDir);
            }
            else
            {
                complete = false;
            }

            int scanNumbers = resamplingList.Count;

            ClearClrDataPoints(
                subtractLowerXICList,
                subtractHigherXICList,
                subtractHighestXICList,
                resamplingList, spvw);

            if (StopFlag || !complete)
            {
                return;
            }

            if (UpdateProgress != null)
            {
                UpdateProgress(
                    PROGRESS_REMOVECONTAMINANT_PHASE.COMPLETE,
                    1, 1);
            }

            Result = CreateRemoveResult(scanNumbers, tempDir, ms1List);
            return;

        }

        /// <summary>
        /// Submit latest result to sample tree.
        /// </summary>
        public void SubmitResultToTree()
        {
            // No Result
            if (Result == null)
                return;

            ClrMsDataVariant msObj = new ClrMsDataVariant(_clrVariant);
            DataGroupNodeWrapper dgnw = msObj.getSample().getRootDataGroupNode();

            //Add result to sample tree.
            SampleSetResampling sampleSet = new SampleSetResampling();
            SampleResampling sample = new SampleResampling(sampleSet);
            sample.openSample();
            DataGroupNodeWrapper root = sample.getRootDataGroupNode();

            sample.setName("Contaminant peak removed " + msObj.getSample().getName());
            root.setName("Contaminant peak removed " + dgnw.getName());

            int cnt = 0;
            for (int i = 0; i < Result.Spectrums.Count(); i++)
            {
                ClrDataPoints pts = Result.DataPoints[i];
                SpectrumWrapper sw = Result.Spectrums[i];
                ClrDataPoints xyd = new ClrDataPoints();
                SpectrumResampling spec;

                if (Result.Spectrums.Count == 1)
                {
                    spec = new SpectrumResampling(sample, sw.getName());
                }
                else
                {
                    cnt++;
                    spec = new SpectrumResampling(sample, Result.Spectrums[i].getName() + " (" + cnt.ToString() + ")");
                }
                spec.getXYData(xyd, false);
                spec.SetData(pts);
                spec.onGetXYData(xyd, -1.0, -1.0);
                spec.setMinX(pts.getMinX());
                spec.setMaxX(pts.getMaxX());
                spec.setMaxIntensity(pts.getMaxY());
                spec.setMsStage((int)sw.getMsStage());
                sample.setSampleIndex((int)0);
                root.addSpectrum(spec);
            }
            sampleSet.addSample(sample);
            ClrPluginCallTool.onOpenSample(sample);
        }

        #endregion

        #region --- Private Methods -------------------------------------

        /// <summary>
        /// Clear ClrDataPoints.
        /// </summary>
        /// <param name="subtractLowerXICList">Subtracted Lower XIC List</param>
        /// <param name="subtractHigherXICList">Subtracted Higher XIC List</param>
        /// <param name="subtractHighestXICList">Subtracted Highest XIC List</param>
        /// <param name="resamplingList">Resampling List</param>
        /// <param name="spvw">SettingParameterValuesWrapper</param>
        private void ClearClrDataPoints(
            ClrDataPoints[] subtractLowerXICList,
            ClrDataPoints[] subtractHigherXICList,
            ClrDataPoints subtractHighestXICList,
            List<ClrDataPoints> resamplingList,
            SettingParameterValuesWrapper spvw
            )
        {
            foreach (ClrDataPoints pts in subtractLowerXICList)
            {
                if (pts == null)
                {
                    break;
                }
                pts.clearPoints();
                pts.Dispose();
            }

            foreach (ClrDataPoints pts in subtractHigherXICList)
            {
                pts.clearPoints();
                pts.Dispose();
            }

            foreach (ClrDataPoints pts in resamplingList)
            {
                pts.clearPoints();
                pts.Dispose();
            }

            if (subtractHighestXICList != null)
            {
                subtractHighestXICList.clearPoints();
                subtractHighestXICList.Dispose();
            }

            spvw.clear();
            spvw.Dispose();
        }

        /// <summary>
        /// Create Remove Contaminant Peak Result.
        /// </summary>
        /// <param name="scanNumbers">Scan Numbers</param>
        /// <param name="tempDir">Temporal Directory</param>
        /// <param name="ms1List">MS1 List</param>
        /// <returns>Remove Contaminant Peak Result</returns>
        private RemoveResult CreateRemoveResult(
            int scanNumbers, string tempDir,
            List<SpectrumWrapper> ms1List)
        {
            RemoveResult result = new RemoveResult();

            for (int i = 0; i < scanNumbers; i++)
            {
                if (UpdateProgress != null)
                {
                    UpdateProgress(
                        PROGRESS_REMOVECONTAMINANT_PHASE.READ_FROM_TEMPORARY,
                        i, scanNumbers);
                }
                using (StreamReader sr = new StreamReader(tempDir + @"\Scan" + i.ToString() + ".csv"))
                {
                    ClrDataPoints pts = new ClrDataPoints();
                    while (!sr.EndOfStream)
                    {
                        string[] splitLine = sr.ReadLine().Split(',');
                        double x = double.Parse(splitLine[0]);
                        double y = double.Parse(splitLine[1]);
                        pts.addPoint(x, y);
                    }
                    result.AddResult(pts, ms1List[i]);
                }
            }

            return result;
        }

        /// <summary>
        /// Create Temporal XIC Files.
        /// </summary>
        /// <param name="subtractLowerXICList">Subtracted Lower XIC List</param>
        /// <param name="subtractHigherXICList">Subtracted Higher XIC List</param>
        /// <param name="subtractHighestXICList">Subtracted Highest XIC List</param>
        /// <param name="resamplingList">Resampling List</param>
        /// <param name="tempDir">Temporal Directory</param>
        /// <returns>Result</returns>
        private bool CreateTempXICFiles(
            ClrDataPoints[] subtractLowerXICList,
            ClrDataPoints[] subtractHigherXICList,
            ClrDataPoints subtractHighestXICList,
            List<ClrDataPoints> resamplingList,
            string tempDir)
        {
            uint count = (uint)(subtractHighestXICList == null ? 0 : 1);
            count += (uint)subtractLowerXICList.Length;
            count += (uint)subtractHigherXICList.Length;

            //Replace intensity of resampled spectrum
            for (int i = 0; i < resamplingList.Count; i++)
            {
                if (UpdateProgress != null)
                {
                    UpdateProgress(
                        PROGRESS_REMOVECONTAMINANT_PHASE.WRITE_TO_TEMPORARY,
                        i, resamplingList.Count);
                }
                if (StopFlag)
                {
                    return false;
                }
                try
                {
                    using (StreamWriter sw = new StreamWriter(tempDir + @"\Scan" + i.ToString() + ".csv"))
                    {
                        for (uint j = 0; j < count; j++)
                        {
                            int xicIndex = 0;
                            double x = resamplingList[i].getX(j);

                            if (x < _resamplingParam.OutputLowerRange || _resamplingParam.OutputHigherRange < x)
                            {
                                continue;
                            }

                            if (j <= int.MaxValue)
                            {
                                sw.WriteLine(x.ToString() + "," + subtractLowerXICList[(int)j].getY((uint)xicIndex).ToString());
                            }
                            else if (j == uint.MaxValue)
                            {
                                sw.WriteLine(x + "," + subtractHighestXICList.getY((uint)xicIndex).ToString());
                            }
                            else
                            {
                                sw.WriteLine(x + "," + subtractHigherXICList[(int)(j - int.MaxValue)].getY((uint)xicIndex).ToString());
                            }
                            xicIndex++;
                        }
                    }
                }
                catch
                {
                    return false;
                }
            }
            return true;
        }

        /// <summary>
        /// Create temporary directory.
        /// </summary>
        /// <returns>Directory path(If failed, return null)</returns>
        private string CreateTempDir()
        {
            string tempDir = ClrMsppManager.getInstance().getTmpDir() + @"\ResamplingPlugin";

            if (Directory.Exists(tempDir))
            {
                return tempDir;
            }

            if (Directory.CreateDirectory(tempDir).Exists)
            {
                return tempDir;
            }
            return null;
        }

        /// <summary>
        /// Create Baseline subtracted xic.
        /// </summary>
        /// <param name="pfiw">PluginFunctionItemWrapper</param>
        /// <param name="resamplingList">Resampling List</param>
        /// <param name="ms1List">MS1 List</param>
        /// <param name="spvw">SettingParameterValuesWrapper</param>
        /// <param name="subtractLowerXICList">output Subtracted lower XIC List</param>
        /// <param name="subtractHigherXICList">output Subtracted Highter XIC List</param>
        /// <param name="subtractHighestXICList">output Subtracted Highest XIC List</param>
        /// <returns>Result</returns>
        private bool CreateBaselineSubtractedXIC(
            PluginFunctionItemWrapper pfiw,
            List<ClrDataPoints> resamplingList,
            List<SpectrumWrapper> ms1List,
            SettingParameterValuesWrapper spvw,
            ref ClrDataPoints[] subtractLowerXICList,
            ref ClrDataPoints[] subtractHigherXICList,
            ref ClrDataPoints subtractHighestXICList
            )
        {

            uint resamplingLength = resamplingList[0].getLength();

            int lowerIndex = (resamplingLength < int.MaxValue) ? (int)resamplingLength : int.MaxValue;
            int higherIndex = (0 < (int)(resamplingLength - lowerIndex)) ? (int)(resamplingLength - lowerIndex) : 0;

            subtractLowerXICList = new ClrDataPoints[lowerIndex];
            subtractHigherXICList = new ClrDataPoints[higherIndex];

            for (uint i = 0; i < resamplingLength; i++)
            {
                //Update message.
                if (UpdateProgress != null)
                {
                    UpdateProgress(
                        PROGRESS_REMOVECONTAMINANT_PHASE.REMOVE_CONTAMINANT_PEAK,
                        (int)i, (int)resamplingLength);
                }

                //Cancel.
                if (StopFlag)
                {
                    break;
                }

                //Create XIC.
                ClrDataPoints pts = new ClrDataPoints();
                for (int j = 0; j < resamplingList.Count; j++)
                {
                    //Cancel.
                    if (StopFlag)
                    {
                        break;
                    }
                    pts.addPoint(ms1List[j].getRt(), resamplingList[j].getY(i));
                }
                //Cancel.
                if (StopFlag)
                {
                    pts.clearPoints();
                    pts.Dispose();
                    break;
                }

                //Baseline Subtraction
                ClrParameters param = new ClrParameters();
                ClrDataPoints baseline = new ClrDataPoints();
                ClrPluginCallTool.setXYData(param, pts);
                ClrPluginCallTool.setBaseline(param, baseline);
                ClrPluginCallTool.setSettingValues(param, spvw);
                ClrVariant baselineResult = pfiw.getCall().invoke(param);

                //Cancel.
                if (StopFlag)
                {
                    param.Dispose();
                    baseline.clearPoints();
                    baseline.Dispose();
                    break;
                }

                //Resampling baseline.
                ClrDataPoints subtracted = new ClrDataPoints();
                ResamplingParameter resamplingParam = new ResamplingParameter();
                ResamplingCalculation resamplingCalc = new ResamplingCalculation(resamplingParam);
                resamplingParam.ResamplingMode = ResamplingParameter.SamplingMode.FIRST;
                resamplingParam.Interpolation = _resamplingParam.Interpolation;
                resamplingParam.AddXYData(pts);
                resamplingParam.AddXYData(baseline);
                resamplingCalc.Execute();
                ClrDataPoints resampledBaseline = resamplingCalc.Result.GetResultData(1);

                for (uint j = 0; j < resampledBaseline.getLength(); j++)
                {
                    double intensity = Math.Max(pts.getY(j) - resampledBaseline.getY(j), 0);
                    subtracted.addPoint(pts.getX(j), intensity);
                }

                // Countermeasure to OutOfBoundsException.
                if (i < int.MaxValue)
                {
                    subtractLowerXICList[i] = subtracted;
                }
                else if (int.MaxValue < i && i < (uint)int.MaxValue * 2)
                {
                    subtractHigherXICList[i - int.MaxValue] = subtracted;
                }
                else
                {
                    subtractHighestXICList = subtracted;
                }
                resamplingParam.ClearXYDataList();
                resamplingCalc.Result.ClearXYDataList();
            }
            return !StopFlag;
        }

        /// <summary>
        /// Read Spectrum(only ms1)
        /// </summary>
        /// <param name="node">DataGroupNodeWrapper</param>
        /// <param name="listReadSpec">SpectrumWrapper List</param>
        private void ReadSpectrum(DataGroupNodeWrapper node, ref List<SpectrumWrapper> listReadSpec)
        {
            uint numSpec = node.getNumberOfSpectra();
            for (uint n = 0; n < numSpec; n++)
            {
                SpectrumWrapper spec = node.getSpectrum(n);
                if (spec.getMsStage() == 1)
                {
                    listReadSpec.Add(spec);
                }
            }

            uint numChild = node.getNumberOfChildren();
            for (uint n = 0; n < numChild; n++)
            {
                DataGroupNodeWrapper child = node.getChild(n);
                if (child != null)
                {
                    ReadSpectrum(child, ref listReadSpec);
                }
            }

        }

        /// <summary>
        /// Update Spectrum Calculation(Resampling) Progress
        /// </summary>
        /// <param name="id">PROGRESS_RESAMPLINGT_PHASE id</param>
        /// <param name="value">progress bar value or max</param>
        /// <param name="cursmplnum">current number for text</param>
        /// <param name="maxsmplnum">max number for text</param>
        private void UpdateSpecCalcResamplingProgress(
            ResamplingCalculation.PROGRESS_RESAMPLING_PHASE id,
            int value, int cursmplnum, int maxsmplnum)
        {
            UpdateResamplingProgressTemp(id, value, cursmplnum, maxsmplnum);
        }

        #endregion

    }
}
