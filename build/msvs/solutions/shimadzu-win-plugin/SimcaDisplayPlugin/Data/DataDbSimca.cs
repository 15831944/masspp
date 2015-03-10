/// <summary>
/// [FILE] SimcaDbData.cs
/// [ABSTRACT] SIMCA Plugin DB Data class
/// Copyright (C) 2012-12-18 Shimadzu
/// </summary>

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows;

// Mass++ namespace
using kome.clr;

namespace SimcaDisplay.Data
{
    /// <summary>
    /// SIMCA DB data
    /// </summary>
    internal class SimcaDbData
    {
        #region --- Constants ------------------------------------------
        /// <summary>
        /// Matrix ID minimum value
        /// </summary>
        private const int MATRIX_ID_MIN = 0;

        /// <summary>
        /// Matrix ID(an unset up value)
        /// </summary>
        private const int MATRIX_ID_NONE = MATRIX_ID_MIN - 1;

        /// <summary>
        /// Matrix ID 0 is reserved by un-choosing.
        /// </summary>
        private const int MATRIX_ID_RESERVE_COUNT = 1;

        /// <summary>
        /// No data value
        /// </summary>
        private const int NO_DATA = -1;
        #endregion

        #region --- Variables ------------------------------------------
        /// <summary>
        /// Matrix ID list
        /// </summary>
        private List<int> _matrixIdList = new List<int>();

        /// <summary>
        /// Matrix name list
        /// </summary>
        private List<string> _matrixNameList = new List<string>();

        /// <summary>
        /// Group ID list
        /// </summary>
        private List<int> _groupIdList = new List<int>();

        /// <summary>
        /// Group name list
        /// </summary>
        private List<string> _groupNameList = new List<string>();

        /// <summary>
        /// Group color list
        /// </summary>
        private List<Color> _groupColorList = new List<Color>();

        /// <summary>
        /// Sample ID list
        /// </summary>
        private List<List<int>> _sampleIdList = new List<List<int>>();

        /// <summary>
        /// Sample name list
        /// </summary>
        private List<List<string>> _sampleNameList = new List<List<string>>();

        /// <summary>
        /// Peak ID list
        /// </summary>
        private List<int> _peakIdList = new List<int>();

        /// <summary>
        /// Peak RT value list
        /// </summary>
        private List<double> _peakRtList = new List<double>();

        /// <summary>
        /// Peak m/z value list
        /// </summary>
        private List<double> _peakMzList = new List<double>();

        #endregion

        #region --- Construction ---------------------------------------
        /// <summary>
        /// Initializes a new instance of the SimcaDbData class
        /// </summary>
        public SimcaDbData()
        {
            Clear();
        }
        #endregion

        #region --- Properties -----------------------------------------
        /// <summary>
        /// Gets selected Matrices ID
        /// </summary>
        public int SelectedMatrixId { get; private set; }

        #endregion

        #region --- Public Methods -------------------------------------
        /// <summary>
        /// DB Read
        /// </summary>
        public void ReadDb()
        {
            Clear();

            IdentificationManagerWrapper managerWrapper = IdentificationManagerWrapper.getInstance();

            // Get matrix ID list
            int[] matrixIdArray = managerWrapper.getMatrices();
            if (matrixIdArray == null)
            {
                this.SelectedMatrixId = MATRIX_ID_NONE;
                return;
            }

            _matrixIdList = matrixIdArray.ToList();

            // Get matrix name list
            foreach (int item in _matrixIdList)
            {   // the number of Matrix ID loop
                _matrixNameList.Add(managerWrapper.getMatrixName(item));
            }

            // Get selected matrix
            this.SelectedMatrixId = managerWrapper.getSelectedMatrix();

            GetDBData(this.SelectedMatrixId);
        }

        /// <summary>
        /// ReadDb(Matrix ID specification)
        /// </summary>
        /// <param name="selMatrixId">Matrix ID</param>
        public void ReadDb(int selMatrixId)
        {
            Clear();

            IdentificationManagerWrapper managerWrapper = IdentificationManagerWrapper.getInstance();

            // Get matrix ID list
            int[] matrixIdArray = managerWrapper.getMatrices();
            if (matrixIdArray == null)
            {
                this.SelectedMatrixId = MATRIX_ID_NONE;
                return;
            }

            _matrixIdList = matrixIdArray.ToList();

            // Get matrix name list
            foreach (int item in _matrixIdList)
            {   // the number of Matrix ID loop
                _matrixNameList.Add(managerWrapper.getMatrixName(item));
            }

            foreach (int matrixId in _matrixIdList)
            {
                if (matrixId == selMatrixId)
                {
                    this.SelectedMatrixId = selMatrixId;
                    GetDBData(this.SelectedMatrixId);
                    return;
                }
            }
        }

        /// <summary>
        /// Check DB update 
        /// </summary>
        /// <returns>T:changed F:not changed</returns>
        public bool IsChanged()
        {
            SimcaDbData newData = new SimcaDbData();

            newData.ReadDb();   // Read DB

            return CheckChanged(newData);
        }

        /// <summary>
        /// Check DB update(Matrix ID specification)
        /// </summary>
        /// <param name="selMatrixId">Matrix ID</param>
        /// <returns>>T:changed F:not changed</returns>
        public bool IsChanged(int selMatrixId)
        {
            SimcaDbData newData = new SimcaDbData();

            newData.ReadDb(selMatrixId);   // Read DB

            return CheckChanged(newData);
        }

        /// <summary>
        /// Get matrix name array
        /// </summary>
        /// <returns>matrix name array</returns>
        public string[] GetMatrixNameArray()
        {
            string[] res = _matrixNameList.ToArray();

            return res;
        }

        /// <summary>
        /// Get selected matrix index
        /// </summary>
        /// <returns>selected matrix index(When it does not exist:-1)</returns>
        public int GetSelectedMatrixIndex()
        {
            int res = NO_DATA;

            if (this.SelectedMatrixId >= MATRIX_ID_MIN)
            {   // Matrix Selected
                res = _matrixIdList.FindIndex(id => this.SelectedMatrixId == id);
            }

            return res;
        }

        /// <summary>
        /// Find sample name
        /// </summary>
        /// <param name="key">sample name</param>
        /// <param name="groupIdx">group index</param>
        /// <param name="sampleIdx">sample index</param>
        /// <returns>T:success F:fail</returns>
        public bool FindSampleName(string key, out int groupIdx, out int sampleIdx)
        {
            groupIdx = 0;
            sampleIdx = 0;

            if (this.SelectedMatrixId < MATRIX_ID_MIN)
            {   // Matrix not Selected
                return false;
            }

            string[] arrayKey = key.Split(':');

            foreach (List<string> nameList in _sampleNameList)
            {   // the number of sample name list loop
                sampleIdx = nameList.FindIndex(name => SampleNameMatrixComparison(arrayKey, name));
                if (sampleIdx >= 0)
                {
                    return true;
                }

                groupIdx++;
            }

            return false;
        }

        /// <summary>
        /// Get group color
        /// </summary>
        /// <param name="index">group index(0～_groupColorList.Count()-1)</param>
        /// <returns>group color</returns>
        public Color GetGroupColor(int index)
        {
            Debug.Assert(
                ((0 <= index) && (index < _groupColorList.Count())),
                "SimcaDbData",
                string.Format("GetGroupColor: index={0}", index));

            return _groupColorList[index];
        }

        /// <summary>
        /// Get group ID
        /// </summary>
        /// <param name="index">group index(0～_groupIdList.Count()-1)</param>
        /// <returns>group ID</returns>
        public int GetGroupId(int index)
        {
            Debug.Assert(
                ((0 <= index) && (index < _groupIdList.Count())),
                "SimcaDbData",
                string.Format("GetGroupId: index={0}", index));

            return _groupIdList[index];
        }

        /// <summary>
        /// Get group name
        /// </summary>
        /// <param name="index">group index(0～_groupNameList.Count()-1)</param>
        /// <returns>group name</returns>
        public string GetGroupName(int index)
        {
            Debug.Assert(
                ((0 <= index) && (index < _groupNameList.Count())),
                "SimcaDbData",
                string.Format("GetGroupName: index={0}", index));

            return _groupNameList[index];
        }

        /// <summary>
        /// Get sample ID
        /// </summary>
        /// <param name="groupIdx">group index(0～_sampleIdList.Count()-1)</param>
        /// <param name="sampleIdx">sample index(0～_sampleIdList[groupIdx].Count()-1)</param>
        /// <returns>sample ID</returns>
        public int GetSampleId(int groupIdx, int sampleIdx)
        {
            Debug.Assert(
                ((0 <= groupIdx) && (groupIdx < _sampleIdList.Count())),
                "SimcaDbData",
                string.Format("GetSampleId: groupIdx={0}", groupIdx));

            Debug.Assert(
                ((0 <= sampleIdx) && (sampleIdx < _sampleIdList[groupIdx].Count())),
                "SimcaDbData",
                string.Format("GetSampleId: sampleIdx={0}", sampleIdx));

            return _sampleIdList[groupIdx][sampleIdx];
        }

        /// <summary>
        /// Find nearest peak ID
        /// </summary>
        /// <param name="rt">RT value</param>
        /// <param name="mz">m/z value</param>
        /// <returns>peak ID</returns>
        public int FindPeakId(double rt, double mz)
        {
            int idx = 0;
            int minId = NO_DATA;
            double minValue = double.MaxValue;
            double work = 0.0;

            foreach (int item in _peakIdList)
            {   // the number of peak ID loop
                double gapRt = _peakRtList[idx] - rt;
                double gapMz = _peakMzList[idx] - mz;
                work = (gapRt * gapRt) + (gapMz * gapMz);
                if (work < minValue)
                {
                    minValue = work;
                    minId = item;
                }

                idx++;
            }

            return minId;
        }

		/// <summary>
		/// Find nearest peak ID
		/// </summary>
		/// <param name="mz">m/z value</param>
		/// <returns>peak ID</returns>
		public int FindPeakId(double mz)
		{
			int idx = 0;
			int minId = NO_DATA;
			double minValue = double.MaxValue;
			double work = 0.0;

			foreach (int item in _peakIdList)
			{   // the number of peak ID loop
				double gapMz = _peakMzList[idx] - mz;
				work = (gapMz * gapMz);
				if (work < minValue)
				{
					minValue = work;
					minId = item;
				}

				idx++;
			}

			return minId;
		}

		/// <summary>
        /// Convert selected matrix index to ID
        /// </summary>
        /// <param name="index">index(0～_matrixIdList.Count()-1-MATRIX_ID_RESERVE_COUNT</param>
        /// <returns>selected matrix ID</returns>
        public int ConvSelectedMatrixIndexToId(int index)
        {
            Debug.Assert(
                ((0 <= index) && (index < (_matrixIdList.Count() + MATRIX_ID_RESERVE_COUNT))),
                "SimcaDbData",
                string.Format("ConvSelectedMatrixIndexToId: index={0}", index));

            // 0 is reserved by un-choosing.
            if (index == 0)
            {
                return MATRIX_ID_NONE;
            }

            return _matrixIdList[index - MATRIX_ID_RESERVE_COUNT];
        }

        /// <summary>
        /// ToString
        /// </summary>
        /// <returns>string data</returns>
        public override string ToString()
        {
            StringBuilder result = new StringBuilder();
            int index;

            result.Clear();

            // SelectedMatrixId
            result.Append("SelectedMatrixId:\n\t");
            result.Append(this.SelectedMatrixId.ToString());

            // _matrixIdList
            result.Append("\n_matrixIdList:\n\t");
            foreach (int item in _matrixIdList)
            {
                result.Append(item.ToString() + ",");
            }

            // _matrixNameList
            result.Append("\n_matrixNameList:\n\t");
            foreach (string item in _matrixNameList)
            {
                result.Append(item + ",");
            }

            // _groupIdList
            result.Append("\n_groupIdList:\n\t");
            foreach (int item in _groupIdList)
            {
                result.Append(item.ToString() + ",");
            }

            // _groupNameList
            result.Append("\n_groupNameList:\n\t");
            foreach (string item in _groupNameList)
            {
                result.Append(item + ",");
            }

            // _groupColorList
            result.Append("\n_groupColorList:\n\t");
            foreach (Color item in _groupColorList)
            {
                result.Append(item.ToString() + ",");
            }

            // _sampleIdList
            result.Append("\n_sampleIdList:");
            index = 0;
            foreach (List<int> item in _sampleIdList)
            {
                result.Append(string.Format("\n\tGroup {0}:\n\t\t", _groupIdList[index]));
                foreach (int val in item)
                {
                    result.Append(val.ToString() + ",");
                }

                index++;
            }

            // _sampleNameList
            result.Append("\n_sampleNameList:");
            index = 0;
            foreach (List<string> item in _sampleNameList)
            {
                result.Append(string.Format("\n\tGroup {0}:\n\t\t", _groupIdList[index]));
                foreach (string val in item)
                {
                    result.Append(val + ",");
                }

                index++;
            }

            // _peakIdList
            result.Append("\n_peakIdList:\n\t");
            foreach (int item in _peakIdList)
            {
                result.Append(item.ToString() + ",");
            }

            // _peakRtList
            result.Append("\n_peakRtList:\n\t");
            foreach (double item in _peakRtList)
            {
                result.Append(item.ToString() + ",");
            }

            // _peakMzList
            result.Append("\n_peakMzList:\n\t");
            foreach (double item in _peakMzList)
            {
                result.Append(item.ToString() + ",");
            }

            result.Append("\n");
            return result.ToString();
        }

        #endregion

        #region --- Private Methods ------------------------------------
        /// <summary>
        /// Clear
        /// </summary>
        private void Clear()
        {
            this.SelectedMatrixId = MATRIX_ID_NONE;
            _matrixIdList.Clear();
            _matrixNameList.Clear();
            _groupIdList.Clear();
            _groupNameList.Clear();
            _groupColorList.Clear();
            _sampleIdList.Clear();
            _sampleNameList.Clear();
            _peakIdList.Clear();
            _peakRtList.Clear();
            _peakMzList.Clear();
        }

        /// <summary>
        /// Get DB data
        /// </summary>
        /// <param name="selMatrixId">Selected matrix ID</param>
        private void GetDBData(int selMatrixId)
        {
            if (selMatrixId >= MATRIX_ID_MIN)
            {   // Matrix Selected
                IdentificationManagerWrapper managerWrapper = IdentificationManagerWrapper.getInstance();
                
                // Get group ID list
                int[] groupIdArray = managerWrapper.getGroups(selMatrixId);

                if (groupIdArray != null)
                {
                    _groupIdList = groupIdArray.ToList();

                    // Get group name list, group color list
                    foreach (int item in _groupIdList)
                    {   // the number of group ID loop
                        _groupNameList.Add(managerWrapper.getGroupName(item));
                        _groupColorList.Add(managerWrapper.getGroupColor(item));

                        // Get sample ID list
                        int[] sampleIdArray = managerWrapper.getMatrixSamplesFromGroup(item);
                        if (sampleIdArray == null)
                        {
                            return;
                        }

                        _sampleIdList.Add(sampleIdArray.ToList());

                        // Get sample name list
                        List<string> sampleNameList = new List<string>();
                        sampleNameList.Clear();
                        foreach (int sampleId in _sampleIdList.Last())
                        {   // the number of sample ID loop
                            string sampleName = managerWrapper.getMatrixSampleName(sampleId);
                            sampleNameList.Add(sampleName);
                        }

                        _sampleNameList.Add(sampleNameList);
                    }
                }

                // Get peak ID list
                int[] peakIdArray = managerWrapper.getMatrixPeaks(selMatrixId);
                if (peakIdArray != null)
                {
                    _peakIdList = peakIdArray.ToList();
                    foreach (int item in _peakIdList)
                    {   // the number of peak ID loop
                        _peakRtList.Add(managerWrapper.getMatrixPeakRt(item));
                        _peakMzList.Add(managerWrapper.getMatrixPeakMz(item));
                    }
                }
            }
        }

        /// <summary>
        /// Check DB data changed
        /// </summary>
        /// <param name="newData">New read data</param>
        /// <returns>T:changed F:not changed</returns>
        private bool CheckChanged(SimcaDbData newData)
        {
            bool changed = false;

            if ((!newData._matrixIdList.SequenceEqual(_matrixIdList))
             || (!newData._matrixNameList.SequenceEqual(_matrixNameList))
             || (newData.SelectedMatrixId != this.SelectedMatrixId)
             || (!newData._groupIdList.SequenceEqual(_groupIdList))
             || (!newData._groupNameList.SequenceEqual(_groupNameList))
             || (!newData._groupColorList.SequenceEqual(_groupColorList))
             || (!newData._peakIdList.SequenceEqual(_peakIdList))
             || (!newData._peakRtList.SequenceEqual(_peakRtList))
             || (!newData._peakMzList.SequenceEqual(_peakMzList)))
            {
                changed = true;
            }

            if (newData._sampleIdList.Count() == _sampleIdList.Count())
            {
                for (int lp = 0; lp < newData._sampleIdList.Count(); lp++)
                {
                    if (!newData._sampleIdList[lp].SequenceEqual(_sampleIdList[lp]))
                    {
                        changed = true;
                    }
                }
            }
            else
            {
                changed = true;
            }

            if (newData._sampleNameList.Count() == _sampleNameList.Count())
            {
                for (int lp = 0; lp < newData._sampleNameList.Count(); lp++)
                {
                    if (!newData._sampleNameList[lp].SequenceEqual(_sampleNameList[lp]))
                    {
                        changed = true;
                    }
                }
            }
            else
            {
                changed = true;
            }

            // Update Data
            if (changed)
            {
                this.SelectedMatrixId = newData.SelectedMatrixId;
                _matrixIdList = newData._matrixIdList;
                _matrixNameList = newData._matrixNameList;
                _groupIdList = newData._groupIdList;
                _groupNameList = newData._groupNameList;
                _groupColorList = newData._groupColorList;
                _sampleIdList = newData._sampleIdList;
                _sampleNameList = newData._sampleNameList;
                _peakIdList = newData._peakIdList;
                _peakRtList = newData._peakRtList;
                _peakMzList = newData._peakMzList;
            }

            return changed;
        }

        /// <summary>
        /// Sample name matrix comparison
        /// </summary>
        /// <param name="arrayKey">DB name array</param>
        /// <param name="src">Sample name</param>
        /// <returns>T=Coincidence, F=Disagreement</returns>
        private bool SampleNameMatrixComparison(string[] arrayKey, string src)
        {
            // Matrix comparison of 2x2(2x2, 1x2, 2x1, 1x1)
            string[] arrayName = src.Split(':');
            if ((arrayKey.Count() >= 2) && (arrayName.Count() >= 2))
            {   // 2x2 [0]==[0] && [1]==[1]
                return arrayKey[0].Trim().Equals(arrayName[0].Trim()) && arrayKey[1].Trim().Equals(arrayName[1].Trim());
            }
            else if ((arrayKey.Count() == 1) && (arrayName.Count() >= 2))
            {   // 1x2 [0]==[1]
                return arrayKey[0].Trim().Equals(arrayName[1].Trim());
            }
            else if ((arrayKey.Count() >= 2) && (arrayName.Count() == 1))
            {   // 2x1 [1]==[0]
                return arrayKey[1].Trim().Equals(arrayName[0].Trim());
            }
            else if ((arrayKey.Count() == 1) && (arrayName.Count() == 1))
            {   // 1x1 [0]==[0]
                return arrayKey[0].Trim().Equals(arrayName[0].Trim());
            }
            else
            {   // 0x1, 1x0, 0x0
                return false;
            }
        }

        #endregion
    }
}
