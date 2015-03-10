/// <summary>
/// [FILE] DataSimca.cs
/// [ABSTRACT] SIMCA Plugin Data class
/// Copyright (C) 2012-12-18 Shimadzu
/// </summary>

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows;
using Microsoft.VisualBasic.FileIO;

namespace SimcaDisplay.Data
{
    /// <summary>
    /// SIMCA data item
    /// </summary>
    internal class SimcaDataItem
    {
        #region --- Constants ------------------------------------------
        /// <summary>
        /// No data
        /// </summary>
        private const int NO_DATA = -1;
        #endregion

        #region --- Variables ------------------------------------------
        /// <summary>
        /// File data list
        /// </summary>
        private List<string> _readStr = new List<string>();
        #endregion

        #region --- Construction ---------------------------------------
        /// <summary>
        /// Initializes a new instance of the SimcaDataItem class
        /// </summary>
        public SimcaDataItem()
        {
            Clear();
        }
        #endregion

        #region --- Properties -----------------------------------------
        /// <summary>
        /// Gets or sets a value indicating whether title line flag.
        /// </summary>
        public bool IsTitle { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether point color
        /// </summary>
        public Color Color { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether point group ID
        /// </summary>
        public int GroupId { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether sample ID
        /// </summary>
        public int SampleId { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether peak ID
        /// </summary>
        public int PeakId { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether point group name
        /// </summary>
        public string GroupName { get; set; }
        #endregion

        #region --- Public Methods -------------------------------------
        /// <summary>
        /// Set Data
        /// </summary>
        /// <param name="readStr">File data(1 Line)</param>
        /// <param name="col">Group color</param>
        /// <param name="groupId">Group ID</param>
        /// <param name="peakId">Peak ID</param>
        public void SetData(string[] readStr, Color col, int groupId, int peakId)
        {
            _readStr.Clear();
            this.IsTitle = false;
            this.Color = col;
            this.GroupId = groupId;
            this.PeakId = peakId;
            if (readStr != null)
            {
                foreach (string item in readStr)
                {
                    _readStr.Add(item.Trim());
                }
            }
            else
            {
                _readStr.Add("");
            }
        }

        /// <summary>
        /// Try to get string value
        /// </summary>
        /// <param name="index">Data index(Column index)(0～_readStr.Count - 1)</param>
        /// <param name="item">Data item</param>
        /// <returns>T=Success F=Failure</returns>
        public bool TryGetStrValue(int index, out string item)
        {
            item = string.Empty;
            if (_readStr.Count <= index)
            {
                return false;
            }

            item = _readStr[index];
            return true;
        }

        /// <summary>
        /// Try to get double value
        /// </summary>
        /// <param name="index">Data index(Column index)</param>
        /// <param name="item">Data item</param>
        /// <returns>T=Success F=Failure</returns>
        public bool TryGetDoubleValue(int index, out double item)
        {
            item = 0.0;
            if (_readStr.Count <= index)
            {
                return false;
            }

            if (_readStr[index] == string.Empty)
            {
                return false;
            }

            if (!double.TryParse(_readStr[index], out item))
            {
                return false;
            }

            return true;
        }

        /// <summary>
        /// Try to get integer value
        /// </summary>
        /// <param name="index">Data index(Column index)</param>
        /// <param name="item">Data</param>
        /// <returns>T=Success, F=Failure</returns>
        public bool TryGetIntValue(int index, out int item)
        {
            item = 0;
            if (_readStr.Count <= index)
            {
                return false;
            }

            if (!int.TryParse(_readStr[index], out item))
            {
                return false;
            }
            
            return true;
        }

        /// <summary>
        /// Get read string
        /// </summary>
        /// <returns>read string</returns>
        public string GetReadStr()
        {
            StringBuilder workStr = new StringBuilder();
            workStr.Clear();

            foreach (string item in _readStr)
            {
                if (workStr.Length != 0)
                {
                    workStr.Append(",");
                }

                workStr.Append(item);
            }

            if (workStr.Length != 0)
            {
                workStr.AppendLine(string.Empty);
            }

            return workStr.ToString();
        }
        #endregion

        #region --- Private Methods ------------------------------------
        /// <summary>
        /// Clear
        /// </summary>
        private void Clear()
        {
            this.IsTitle = false;
            this.Color = Color.LightGreen;   // default color
            this.GroupId = NO_DATA;          // none group
            this.SampleId = NO_DATA;
            this.PeakId = NO_DATA;
            this.GroupName = "(none)";

            _readStr.Clear();
        }
        #endregion
    }

    /// <summary>
    /// SIMCA data (file data)
    /// </summary>
    internal class SimcaData
    {
        #region --- Constants ------------------------------------------
        /// <summary>
        /// Data type
        /// </summary>
        public enum SIMCA_DATA_TYPE
        {
            /// <summary>
            /// Score Plot
            /// </summary>
            SCORE_PLOT = 0,

            /// <summary>
            /// Loading Plot
            /// </summary>
            LOADING_PLOT = 1,

            /// <summary>
            /// S Plot
            /// </summary>
            S_PLOT = 2
        }

        /// <summary>
        /// Read error code
        /// </summary>
        public enum SIMCA_READ_ERR_CODE
        {
            /// <summary>
            /// Success
            /// </summary>
            OK,

            /// <summary>
            /// NG(Data format)
            /// </summary>
            NG_FORMAT,

            /// <summary>
            /// NG(No data)
            /// </summary>
            NG_DATA,

            /// <summary>
            /// NG(Class)
            /// </summary>
            NG_CLASS,

            /// <summary>
            /// NG(File Read)
            /// </summary>
            NG_READ,

            /// <summary>
            /// Skip data
            /// </summary>
            SKIP
        }

        /// <summary>
        /// Primary ID Index
        /// </summary>
        private const int PRIMARY_ID_INDEX = 0;
        #endregion

        #region --- Variables ------------------------------------------
        /// <summary>
        /// SIMCA data item list
        /// </summary>
        private List<SimcaDataItem> _simcaDataList = new List<SimcaDataItem>();

        /// <summary>
        /// secondary Id column index list
        /// </summary>
        private List<int> _secondaryIdIndexList = new List<int>();

        /// <summary>
        /// data column index list
        /// </summary>
        private List<int> _dataIndexList = new List<int>();

        /// <summary>
        /// data column name list
        /// </summary>
        private List<string> _dataNameList = new List<string>(); 

        /// <summary>
        /// class column index
        /// </summary>
        private int _classIndex;

        /// <summary>
        /// primary ID match pattern
        /// </summary>
        private Regex[] _primaryIdMatchPtn =
        {
            new Regex(@"(.*)\(Primary\)"),  // SIMCA_DATA_TYPE.SCORE_PLOT
            new Regex(@"(.*)\(Primary\)"),  // SIMCA_DATA_TYPE.LOADING_PLOT
            new Regex(@"(.*)\(Primary\)")   // SIMCA_DATA_TYPE.S_PLOT
        };

        /// <summary>
        /// secondary ID match pattern
        /// </summary>
        private Regex[] _secondaryIdMatchPtn =
        {
            new Regex(@"(.*)\(.*\)$"),       // SIMCA_DATA_TYPE.SCORE_PLOT
            new Regex(@"(.*)\(.*\)$"),       // SIMCA_DATA_TYPE.LOADING_PLOT
            new Regex(@"(.*)\(.*\)$")        // SIMCA_DATA_TYPE.S_PLOT
        };

        /// <summary>
        /// class match pattern
        /// </summary>
        private Regex[] _classMatchPtn =
        {
            new Regex(@"[cC]lass"),         // SIMCA_DATA_TYPE.SCORE_PLOT
            new Regex(@"[cC]lass"),         // SIMCA_DATA_TYPE.LOADING_PLOT
            new Regex(@"[cC]lass")          // SIMCA_DATA_TYPE.S_PLOT
        };

        /// <summary>
        /// data match pattern
        /// </summary>
        private Regex[] _dataMatchPtn =
        {
            new Regex(@"([0-9]*(\.)?[0-9]* \* )*(.*)\.(.*\[[0-9]*\])"),         // SIMCA_DATA_TYPE.SCORE_PLOT
            new Regex(@"([0-9]*(\.)?[0-9]* \* )*(.*)\.(.*\[[0-9]*\])"),         // SIMCA_DATA_TYPE.LOADING_PLOT
            new Regex(@"([0-9]*(\.)?[0-9]* \* )*(.*)\.(.*\[[0-9]*\])"),         // SIMCA_DATA_TYPE.S_PLOT(Axis X)
            new Regex(@"([0-9]*(\.)?[0-9]* \* )*(.*)\.(.*\(corr\)\[[0-9]*\])")  // SIMCA_DATA_TYPE.S_PLOT(Axis Y)
        };

        /// <summary>
        /// Template to genarate RT header match pattern
        /// </summary>
        private string _rtPtnTemplate = @"{0}\([rR][tT]\)$";

        #endregion

        #region --- Construction ---------------------------------------
        /// <summary>
        /// Initializes a new instance of the SimcaData class (default)
        /// </summary>
        public SimcaData()
        {
            Clear();
        }

        /// <summary>
        /// Initializes a new instance of the SimcaData class
        /// </summary>
        /// <param name="type">data type</param>
        public SimcaData(SIMCA_DATA_TYPE type)
        {
            Clear();
            this.DataType = type;
        }
        #endregion

        #region --- Properties -----------------------------------------
        /// <summary>
        /// Gets data type
        /// </summary>
        public SIMCA_DATA_TYPE DataType { get; private set; }

        /// <summary>
        /// Gets or sets axis X data index
        /// </summary>
        public int AxisXDataIndex { get; set; }

        /// <summary>
        /// Gets or sets axis Y data index
        /// </summary>
        public int AxisYDataIndex { get; set; }

        /// <summary>
        /// Gets column count
        /// </summary>
        public int ColumnCount { get; private set; }

		/// <summary>
		/// Gets whether this data had RT or not
		/// </summary>
		public bool HasRt { get; private set; }

		#endregion

        #region --- Public Methods -------------------------------------
        /// <summary>
        /// Clear
        /// </summary>
        public void Clear()
        {
            _simcaDataList.Clear();
            _secondaryIdIndexList.Clear();
            _dataIndexList.Clear();
            _dataNameList.Clear();
            _classIndex = -1;
            this.ColumnCount = 0;
            this.AxisXDataIndex = 0;
            this.AxisYDataIndex = 1;
			this.HasRt = false;
        }

        /// <summary>
        /// Add data(1 line)
        /// </summary>
        /// <param name="newData">1 line data</param>
        /// <param name="dbData">DB data</param>
        /// <returns>error code</returns>
        public SIMCA_READ_ERR_CODE Add(string[] newData, SimcaDbData dbData)
        {
            SIMCA_READ_ERR_CODE result = SIMCA_READ_ERR_CODE.OK;
            
            SimcaDataItem item = new SimcaDataItem();
            item.SetData(newData, Color.LightGreen, -1, -1);

            // Split line data
            if (newData == null)
            {
                return SIMCA_READ_ERR_CODE.NG_FORMAT;
            }

            if (_simcaDataList.Count() == 0)
            {   // title line
                item.IsTitle = true;

                // Set column count
                this.ColumnCount = newData.Count();

                // Set index data
                result = SetIndexData(newData);
            }
            else
            {   // data line
                if (this.ColumnCount != newData.Count())
                {
                    result = SIMCA_READ_ERR_CODE.NG_FORMAT;
                }
                else
                {
                    // Check loading plot skip data
                    result = CheckSkipData(dbData, item);

                    // Set peak ID
                    if (result == SIMCA_READ_ERR_CODE.OK)
                    {
                        result = SetPeakId(dbData, item);
                    }

                    // Set score plot parameter
                    if (result == SIMCA_READ_ERR_CODE.OK)
                    {
                        result = SetScorePlotParam(dbData, item);
                    }
                }
            }

            if (result == SIMCA_READ_ERR_CODE.OK)
            {
                _simcaDataList.Add(item);
            }

            if (result == SIMCA_READ_ERR_CODE.SKIP)
            {
                result = SIMCA_READ_ERR_CODE.OK;
            }

            return result;
        }

        /// <summary>
        /// Update
        /// </summary>
        /// <param name="dbData">DB data</param>
        /// <returns>error code</returns>
        public SIMCA_READ_ERR_CODE Update(SimcaDbData dbData)
        {
            SIMCA_READ_ERR_CODE result = SIMCA_READ_ERR_CODE.OK;

            foreach (SimcaDataItem item in _simcaDataList)
            {
                if (item.IsTitle)
                {
                    continue;
                }

                // Set peak ID
                result = SetPeakId(dbData, item);
                if (result == SIMCA_READ_ERR_CODE.OK)
                {
                    // Set score plot parameter
                    result = SetScorePlotParam(dbData, item);
                }

                if (result != SIMCA_READ_ERR_CODE.OK)
                {
                    break;
                }
            }

            return result;
        }

        /// <summary>
        /// Set paste data
        /// </summary>
        /// <param name="paste">paste data</param>
        /// <param name="dbdata">data base data</param>
        /// <returns>error code</returns>
        public SIMCA_READ_ERR_CODE SetPasteData(string paste, SimcaDbData dbdata)
        {
            SIMCA_READ_ERR_CODE result = SIMCA_READ_ERR_CODE.NG_FORMAT;

            // Empty data
            if (paste == string.Empty)
            {
                return result;
            }

            // DB data
            SimcaDbData tempDbData = dbdata;
            if (dbdata == null)
            {
                tempDbData = new SimcaDbData();
            }

            // Set data
            try
            {
                Stream stream = new MemoryStream(System.Text.Encoding.GetEncoding("Shift_JIS").GetBytes(paste));

                using (TextFieldParser parser = new TextFieldParser(
                     stream,
                     System.Text.Encoding.GetEncoding("Shift_JIS")))
                {
                    parser.TextFieldType = FieldType.Delimited;
                    parser.SetDelimiters(",");
                    while (parser.EndOfData == false)
                    {
                        result = Add(parser.ReadFields(), tempDbData);
                        if (result != SimcaData.SIMCA_READ_ERR_CODE.OK)
                        {
                            return result;
                        }
                    }
                }
            }
            catch (Exception)
            {
                return SimcaData.SIMCA_READ_ERR_CODE.NG_READ;
            }

            // Check data
            if (GetDataCount() <= 0)
            {
                result = SimcaData.SIMCA_READ_ERR_CODE.NG_FORMAT;
            }

            if (SIMCA_READ_ERR_CODE.OK != result)
            {
                return result;
            }

            result = CheckData(tempDbData.GetSelectedMatrixIndex());
            if (SIMCA_READ_ERR_CODE.NG_CLASS == result)
            {
                if (dbdata == null)
                {
                    result = SIMCA_READ_ERR_CODE.OK;
                }
            }

            return result;
        }

        /// <summary>
        /// Get data count
        /// </summary>
        /// <returns>data count</returns>
        public int GetDataCount()
        {
            return _simcaDataList.Count() - 1;
        }

        /// <summary>
        /// Get group name
        /// </summary>
        /// <param name="index">data index(0～_simcaDataList.Count - 1)</param>
        /// <returns>group name</returns>
        public string GetGroupName(int index)
        {
            Debug.Assert(
                ((0 <= index) && (index < _simcaDataList.Count())),
                "SimcaData",
                string.Format("GetGroupName: index={0}", index));

            return _simcaDataList[index].GroupName;
        }

        /// <summary>
        /// Get group color
        /// </summary>
        /// <param name="index">data index(0～_simcaDataList.Count - 1)</param>
        /// <returns>group color</returns>
        public Color GetGroupColor(int index)
        {
            Debug.Assert(
                ((0 <= index) && (index < _simcaDataList.Count())),
                "SimcaData",
                string.Format("GetGroupColor: index={0}", index));

            return _simcaDataList[index].Color;
        }

        /// <summary>
        /// Try to get X data value
        /// </summary>
        /// <param name="index">data index</param>
        /// <param name="value">X data value</param>
        /// <returns>T=Success F=Failure</returns>
        public bool TryGetXData(int index, out double value)
        {
            value = 0;
            if ((index < 0) || (_simcaDataList.Count <= index))
            {
                return false;
            }
            
            if (_simcaDataList[index].TryGetDoubleValue(_dataIndexList[this.AxisXDataIndex], out value))
            {
                return true;
            }

            return false;
        }

        /// <summary>
        /// Try to get Y data value
        /// </summary>
        /// <param name="index">data index</param>
        /// <param name="value">Y data value</param>
        /// <returns>T=Success F=Failure</returns>
        public bool TryGetYData(int index, out double value)
        {
            value = 0;
            if ((index < 0) || (_simcaDataList.Count <= index))
            {
                return false;
            }
            
            int idx = this.AxisYDataIndex;
            int add = 0;
            if (this.DataType == SIMCA_DATA_TYPE.S_PLOT)
            {
                idx = this.AxisXDataIndex;
                add = 1;
            }

            if (_simcaDataList[index].TryGetDoubleValue(_dataIndexList[idx] + add, out value))
            {
                return true;
            }

            return false;
        }

        /// <summary>
        /// Get peak ID
        /// </summary>
        /// <param name="index">data index(0～_simcaDataList.Count - 1)</param>
        /// <returns>peak ID</returns>
        public int GetPeakId(int index)
        {
            Debug.Assert(
                ((0 <= index) && (index < _simcaDataList.Count())),
                "SimcaData",
                string.Format("GetPeakId: index={0}", index));

            return _simcaDataList[index].PeakId;
        }

        /// <summary>
        /// Get axis X name
        /// </summary>
        /// <returns>axis X name</returns>
        public string GetAxisXName()
        {
            return _dataNameList[this.AxisXDataIndex];
        }

        /// <summary>
        /// Get axis Y name
        /// </summary>
        /// <returns>axis Y name</returns>
        public string GetAxisYName()
        {
            if (this.DataType == SIMCA_DATA_TYPE.S_PLOT)
            {
                return _dataNameList[this.AxisXDataIndex].Replace("[", "(corr)[");
            }

            return _dataNameList[this.AxisYDataIndex];
        }

        /// <summary>
        /// Check data
        /// </summary>
        /// <param name="matrixIndex">Matrix index</param>
        /// <returns>>error code</returns>
        public SIMCA_READ_ERR_CODE CheckData(int matrixIndex)
        {
            // Check data count
            if (GetDataCount() < 1)
            {
                return SIMCA_READ_ERR_CODE.NG_FORMAT;
            }

            // Check data index list
            if ((_dataIndexList.Count() == 1) && (this.DataType != SIMCA_DATA_TYPE.S_PLOT))
            {
                return SIMCA_READ_ERR_CODE.NG_DATA;
            }

            if (_dataIndexList.Count() < 1)
            {
                return SIMCA_READ_ERR_CODE.NG_FORMAT;
            }

            // Check data value
            double dmy;
            foreach (SimcaDataItem item in _simcaDataList)
            {
                if (item.IsTitle)
                {
                    continue;
                }

                foreach (int index in _dataIndexList)
                {
                    if (!item.TryGetDoubleValue(index, out dmy))
                    {
                        return SIMCA_READ_ERR_CODE.NG_FORMAT;
                    }

                    // The check of a S prot corr sequence
                    if (this.DataType == SIMCA_DATA_TYPE.S_PLOT)
                    {
                        if (!item.TryGetDoubleValue(index + 1, out dmy))
                        {
                            return SIMCA_READ_ERR_CODE.NG_FORMAT;
                        }
                    }
                }
            }

            if (this.DataType != SIMCA_DATA_TYPE.SCORE_PLOT)
            {
                return SIMCA_READ_ERR_CODE.OK;
            }

            // Check Class
            if (matrixIndex <= 0)
            {
                return SIMCA_READ_ERR_CODE.OK;
            }

            if (_classIndex < 0)
            {
                return SIMCA_READ_ERR_CODE.NG_FORMAT;
            }

            Dictionary<int, int> checkDic = new Dictionary<int, int>();
            Dictionary<int, int> checkDicRev = new Dictionary<int, int>();
            checkDic.Clear();
            checkDicRev.Clear();
            foreach (SimcaDataItem item in _simcaDataList)
            {
                if (item.IsTitle)
                {
                    continue;
                }

                int classId;
                if (item.TryGetIntValue(_classIndex, out classId))
                {
                    if (checkDic.ContainsKey(item.GroupId))
                    {
                        int value;
                        checkDic.TryGetValue(item.GroupId, out value);
                        if (value != classId)
                        {
                            return SIMCA_READ_ERR_CODE.NG_CLASS;
                        }
                    }
                    else
                    {
                        checkDic.Add(item.GroupId, classId);
                    }

                    if (checkDicRev.ContainsKey(classId))
                    {
                        int value;
                        checkDicRev.TryGetValue(classId, out value);
                        if (value != item.GroupId)
                        {
                            return SIMCA_READ_ERR_CODE.NG_CLASS;
                        }
                    }
                    else
                    {
                        checkDicRev.Add(classId, item.GroupId);
                    }
                }
                else
                {
                    return SIMCA_READ_ERR_CODE.NG_FORMAT;
                }
            }

            return SIMCA_READ_ERR_CODE.OK;
        }

        /// <summary>
        /// Get axis name array
        /// </summary>
        /// <returns>axis name array</returns>
        public string[] GetAxisNameArray()
        {
            if (_dataNameList.Count() == 0)
            {
                return null;
            }

            return _dataNameList.ToArray();
        }

        /// <summary>
        /// Get string value
        /// </summary>
        /// <param name="col">column</param>
        /// <param name="row">row</param>
        /// <returns>value</returns>
        public string GetStrValue(int col, int row)
        {
            string result = string.Empty;
            if (col >= this.ColumnCount)
            {
                return result;
            }

            if (row >= _simcaDataList.Count())
            {
                return result;
            }

            _simcaDataList[row].TryGetStrValue(col, out result);

            return result;
        }

        /// <summary>
        /// Get grid back color
        /// </summary>
        /// <returns>color array</returns>
        public Color[] GetGridBkColor()
        {
            Color[] backColor = new Color[this.ColumnCount];

            // Set default value
            for (int i = 0; i < this.ColumnCount; i++)
            {
                backColor[i] = Color.LightGray;
            }

            // Primary ID
            if (this.DataType == SIMCA_DATA_TYPE.SCORE_PLOT)
            {
                backColor[0] = Color.Yellow;
            }
            else
            {
                backColor[0] = Color.Green;
            }

            // Secondary Id
            foreach (int index in _secondaryIdIndexList)
            {
                if (this.DataType == SIMCA_DATA_TYPE.SCORE_PLOT)
                {
                    backColor[index] = Color.LightYellow;
                }
                else
                {
                    backColor[index] = Color.LightGreen;
                }
            }

            // Class
            if (_classIndex >= 0)
            {
                backColor[_classIndex] = Color.White;
            }

            // Data
            foreach (int index in _dataIndexList)
            {
                backColor[index] = Color.White;
                if (this.DataType == SIMCA_DATA_TYPE.S_PLOT)
                {
                    backColor[index + 1] = Color.White;
                }
            }

            return backColor;
        }

        /// <summary>
        /// Get grid font style
        /// </summary>
        /// <returns>font style array</returns>
        public FontStyle[] GetGridFontStyle()
        {
            FontStyle[] style = new FontStyle[this.ColumnCount];

            // Set default value
            for (int i = 0; i < this.ColumnCount; i++)
            {
                style[i] = FontStyle.Regular;
            }

            // Primary ID
            style[0] = FontStyle.Bold;
            
            // Class
            if (_classIndex >= 0)
            {
                style[_classIndex] = FontStyle.Bold;
            }

            return style;
        }

        /// <summary>
        /// ToString
        /// </summary>
        /// <returns>string data</returns>
        public override string ToString()
        {
            StringBuilder result = new StringBuilder();

            result.Clear();

            // DataType
            result.Append("DataType:\n\t");
            result.Append(this.DataType.ToString());
            
            // ColumnCount
            result.Append("\nColumnCount:\n\t");
            result.Append(this.ColumnCount.ToString());
            
            // AxisXDataIndex
            result.Append("\nAxisXDataIndex:\n\t");
            result.Append(this.AxisXDataIndex.ToString());
            
            // AxisYDataIndex
            result.Append("\nAxisYDataIndex:\n\t");
            result.Append(this.AxisYDataIndex.ToString());
            
            // _secondaryIdIndexList
            result.Append("\n_secondaryIdIndexList:\n\t");
            foreach (int item in _secondaryIdIndexList)
            {
                result.Append(item.ToString() + ",");
            }
            
            // _dataIndexList
            result.Append("\n_dataIndexList:\n\t");
            foreach (int item in _dataIndexList)
            {
                result.Append(item.ToString() + ",");
            }
            
            // _dataNameList
            result.Append("\n_dataNameList:\n\t");
            foreach (string item in _dataNameList)
            {
                result.Append(item.ToString() + ",");
            }
            
            // _classIndex
            result.Append("\n_classIndex:\n\t");
            result.Append(_classIndex.ToString());
            
            // _simcaDataList
            result.Append("\n_simcaDataList:\n");
            foreach (SimcaDataItem item in _simcaDataList)
            {
                result.Append("\t" + item.GetReadStr());
            }

            return result.ToString();
        }

        #endregion

        #region --- Private Methods ------------------------------------
        /// <summary>
        /// Set index data
        /// </summary>
        /// <param name="newData">1 line data</param>
        /// <returns>error code</returns>
        private SIMCA_READ_ERR_CODE SetIndexData(string[] newData)
        {
            // index clear
            _secondaryIdIndexList.Clear();
            _dataIndexList.Clear();
            _dataNameList.Clear();
            _classIndex = -1;

            if (newData == null)
            {
                return SIMCA_READ_ERR_CODE.NG_FORMAT;
            }

            // Primary ID
            if (!_primaryIdMatchPtn[(int)this.DataType].IsMatch(newData[0]))
            {
                return SIMCA_READ_ERR_CODE.NG_FORMAT;
            }

            bool searchSPlotCorr = false;
            for (int lp = 1; lp < this.ColumnCount; lp++)
            {   // column search
                // S Plot corr data
                if (searchSPlotCorr)
                {
                    if (!_dataMatchPtn[(int)SIMCA_DATA_TYPE.S_PLOT + 1].IsMatch(newData[lp]))
                    {
                        return SIMCA_READ_ERR_CODE.NG_FORMAT;
                    }

                    searchSPlotCorr = false;
                    continue;
                }

                // Secondary ID
                if (_secondaryIdMatchPtn[(int)this.DataType].IsMatch(newData[lp]))
                {
                    //// Since S Plot corr data is also hit, it checks. 
                    //if (_dataMatchPtn[(int)SIMCA_DATA_TYPE.S_PLOT + 1].IsMatch(arrayData[lp]))
                    //{
                    //    return SIMCA_READ_ERR_CODE.NG_FORMAT;
                    //}

					if (IsRtColumnHeader(newData[lp]))
					{
						this.HasRt = true;
					}

                    _secondaryIdIndexList.Add(lp);
                    continue;
                }

                // Class
                if (_classMatchPtn[(int)this.DataType].IsMatch(newData[lp]))
                {
                    if (_classIndex != -1)
                    {
                        return SIMCA_READ_ERR_CODE.NG_FORMAT;
                    }

                    if (this.DataType == SIMCA_DATA_TYPE.SCORE_PLOT)
                    {
                        _classIndex = lp;
                    }

                    continue;
                }

                // data
                if (_dataMatchPtn[(int)this.DataType].IsMatch(newData[lp]))
                {
                    _dataIndexList.Add(lp);
                    _dataNameList.Add(ConvDataName(newData[lp]));

                    if (this.DataType == SIMCA_DATA_TYPE.S_PLOT)
                    {
                        searchSPlotCorr = true;
                    }
                }
            }

            // The last corr data does not exist
            if (searchSPlotCorr)
            {
                return SIMCA_READ_ERR_CODE.NG_FORMAT;
            }

            // Data count check
            if (this.DataType == SIMCA_DATA_TYPE.S_PLOT)
            {
                if (_dataIndexList.Count() == 0)
                {   // no data
                    return SIMCA_READ_ERR_CODE.NG_FORMAT;
                }
            }
            else
            {
                if (_dataIndexList.Count() == 1)
                {   // no data
                    return SIMCA_READ_ERR_CODE.NG_DATA;
                }

                if ((_classIndex == -1) && (this.DataType == SIMCA_DATA_TYPE.SCORE_PLOT))
                {
                    return SIMCA_READ_ERR_CODE.NG_FORMAT;
                }
            }

            return SIMCA_READ_ERR_CODE.OK;
        }

        /// <summary>
        /// Check skip data
        /// </summary>
        /// <param name="dbData">DB data</param>
        /// <param name="item">File data item</param>
        /// <returns>error code</returns>
        private SIMCA_READ_ERR_CODE CheckSkipData(SimcaDbData dbData, SimcaDataItem item)
        {
            if (this.DataType == SIMCA_DATA_TYPE.SCORE_PLOT)
            {
                return SIMCA_READ_ERR_CODE.OK;
            }

            double dummy;
            int count = 0;
            foreach (int index in _secondaryIdIndexList)
            {
                if (!item.TryGetDoubleValue(index, out dummy))
                {
                    return SIMCA_READ_ERR_CODE.SKIP;
                }

                count++;
                if (count >= 2)
                {
                    break;
                }
            }
            foreach (int index in _dataIndexList)
            {
                if (!item.TryGetDoubleValue(index, out dummy))
                {
                    return SIMCA_READ_ERR_CODE.SKIP;
                }
                if (this.DataType == SIMCA_DATA_TYPE.S_PLOT)
                {
                    if (!item.TryGetDoubleValue(index + 1, out dummy))
                    {
                        return SIMCA_READ_ERR_CODE.SKIP;
                    }
                }
            }
            return SIMCA_READ_ERR_CODE.OK;
        }

        /// <summary>
        /// Set peak ID
        /// </summary>
        /// <param name="dbData">DB data</param>
        /// <param name="item">File data item</param>
        /// <returns>error code</returns>
        private SIMCA_READ_ERR_CODE SetPeakId(SimcaDbData dbData, SimcaDataItem item)
        {
            if (this.DataType != SIMCA_DATA_TYPE.SCORE_PLOT)
            {
                item.PeakId = -1;
				if (this.HasRt && _secondaryIdIndexList.Count() >= 2)
                {
					double rt, mz;
					if (!item.TryGetDoubleValue(_secondaryIdIndexList[0], out rt))
					{
						return SIMCA_READ_ERR_CODE.NG_FORMAT;
					}
					if (!item.TryGetDoubleValue(_secondaryIdIndexList[1], out mz))
					{
						return SIMCA_READ_ERR_CODE.NG_FORMAT;
					}
					item.PeakId = dbData.FindPeakId(rt, mz);
				}
				else if (!this.HasRt && _secondaryIdIndexList.Count() >= 1)
				{
					double mz;
					if (!item.TryGetDoubleValue(_secondaryIdIndexList[0], out mz))
					{
						return SIMCA_READ_ERR_CODE.NG_FORMAT;
					}
					item.PeakId = dbData.FindPeakId(mz);
				}
            }

            return SIMCA_READ_ERR_CODE.OK;
        }

        /// <summary>
        /// Set score plot parameter
        /// </summary>
        /// <param name="dbData">DB data</param>
        /// <param name="item">File data item</param>
        /// <returns>T=Success F=Fail</returns>
        private SIMCA_READ_ERR_CODE SetScorePlotParam(SimcaDbData dbData, SimcaDataItem item)
        {
            if (this.DataType != SIMCA_DATA_TYPE.SCORE_PLOT)
            {   // type is different
                return SIMCA_READ_ERR_CODE.OK;
            }

            item.Color = Color.LightGreen;
            item.GroupId = -1;
            item.SampleId = -1;
            item.GroupName = string.Empty;

            // Get primary Id (string)
            string primaryId;
            if (!item.TryGetStrValue(PRIMARY_ID_INDEX, out primaryId))
            {
                return SIMCA_READ_ERR_CODE.NG_FORMAT;
            }

            // Find sample name(sample name == primary Id)
            int groupIdx;
            int sampleIdx;
            if (dbData.FindSampleName(primaryId, out groupIdx, out sampleIdx))
            {
                item.Color = dbData.GetGroupColor(groupIdx);
                item.GroupId = dbData.GetGroupId(groupIdx);
                item.SampleId = dbData.GetSampleId(groupIdx, sampleIdx);
                item.GroupName = dbData.GetGroupName(groupIdx);
            }

            return SIMCA_READ_ERR_CODE.OK;
        }

        /// <summary>
        /// Convert data name 
        /// </summary>
        /// <param name="src">string</param>
        /// <returns>data name</returns>
        private string ConvDataName(string src)
        {
            return _dataMatchPtn[(int)this.DataType].Replace(src, @"$1$4");
        }

		/// <summary>
		/// Judge whether header can be comprehend as a header of RT column. 
		/// </summary>
		/// <param name="src">string</param>
		/// <returns>data name</returns>
		private bool IsRtColumnHeader(string header)
		{
			if (_dataIndexList.Count() > 0)	// Not header row.
			{
				return false;
			}
			else if (this.HasRt)	// another column is RT Column
			{
				return false;
			}
			else
			{
				string matchStr = _secondaryIdMatchPtn[(int)this.DataType].Replace(header, "$1");
				string regStr = string.Format(_rtPtnTemplate, Regex.Escape(matchStr));
				Regex rtMatchPtn = new Regex(regStr);

				bool isMatch = rtMatchPtn.IsMatch(header);
				return isMatch;
			}
		}

        #endregion
    }
}
