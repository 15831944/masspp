/// <summary>
/// [FILE] DataSimcaGraphHistory.cs
/// [ABSTRACT] SIMCA Plugin Graph zoom history data class
/// Copyright (C) 2012-12-18 Shimadzu
/// </summary>

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimcaDisplay.Data
{
    /// <summary>
    ///  Graph zoom history item
    /// </summary>
    internal class SimcaGraphZoomHistoryItem
    {
        #region --- Construction ---------------------------------------
        /// <summary>
        /// Initializes a new instance of the SimcaGraphZoomHistoryItem class
        /// </summary>
        public SimcaGraphZoomHistoryItem()
        {
        }
        #endregion

        #region --- Properties -----------------------------------------
        /// <summary>
        /// Gets or sets minimum value (X axis)
        /// </summary>
        public double MinX { get; set; }

        /// <summary>
        /// Gets or sets maximum value (X axis)
        /// </summary>
        public double MaxX { get; set; }

        /// <summary>
        /// Gets or sets minimum value (Y axis)
        /// </summary>
        public double MinY { get; set; }

        /// <summary>
        /// Gets or sets maximum value (Y axis)
        /// </summary>
        public double MaxY { get; set; }
        #endregion

        #region --- Public Methods -------------------------------------
        /// <summary>
        /// Clone
        /// </summary>
        /// <returns>clone instance</returns>
        public SimcaGraphZoomHistoryItem Clone()
        {
            SimcaGraphZoomHistoryItem item = new SimcaGraphZoomHistoryItem();

            item.MinX = this.MinX;
            item.MaxX = this.MaxX;
            item.MinY = this.MinY;
            item.MaxY = this.MaxY;

            return item;
        }
        #endregion
    }

    /// <summary>
    /// Graph zoom history
    /// </summary>
    internal class SimcaGraphZoomHistory
    {
        #region --- Variables ------------------------------------------
        /// <summary>
        ///  zoom history item list
        /// </summary>
        private List<SimcaGraphZoomHistoryItem> _graphZoomHistory = new List<SimcaGraphZoomHistoryItem>();
        #endregion

        #region --- Construction ---------------------------------------
        /// <summary>
        /// Initializes a new instance of the SimcaGraphZoomHistory class
        /// </summary>
        public SimcaGraphZoomHistory()
        {
            _graphZoomHistory.Clear();
        }
        #endregion

        #region --- Public Methods -------------------------------------
        /// <summary>
        /// Clear
        /// </summary>
        public void Clear()
        {
            _graphZoomHistory.Clear();
        }

        /// <summary>
        /// GetCount
        /// </summary>
        /// <returns>item number</returns>
        public int GetCount()
        {
            return _graphZoomHistory.Count();
        }

        /// <summary>
        /// Push item
        /// </summary>
        /// <param name="item">History item</param>
        public void PushItem(SimcaGraphZoomHistoryItem item)
        {
            SimcaGraphZoomHistoryItem newItem = new SimcaGraphZoomHistoryItem();
            newItem = item.Clone();
            _graphZoomHistory.Add(newItem);
        }

        /// <summary>
        /// Pop item
        /// </summary>
        /// <param name="item">History item</param>
        /// <returns>T=Success F=Failure</returns>
        public bool PopItem(out SimcaGraphZoomHistoryItem item)
        {
            if (_graphZoomHistory.Count() == 0)
            {
                item = null;
                return false;
            }

            SimcaGraphZoomHistoryItem lastItem = _graphZoomHistory.Last();
            item = lastItem.Clone();
            _graphZoomHistory.Remove(lastItem);
            return true;
        }

        /// <summary>
        /// ToString
        /// </summary>
        /// <returns>string data</returns>
        public override string ToString()
        {
            StringBuilder result = new StringBuilder();

            result.Clear();

            result.Append("GraphZoomHistory:\n");
            int count = _graphZoomHistory.Count();
            foreach (SimcaGraphZoomHistoryItem item in _graphZoomHistory)
            {
                result.Append(string.Format("\tBefore {0}:\n", count));
                result.Append(string.Format("\t\tMinX:{0}\n", item.MinX));
                result.Append(string.Format("\t\tMaxX:{0}\n", item.MaxX));
                result.Append(string.Format("\t\tMinY:{0}\n", item.MinY));
                result.Append(string.Format("\t\tMaxY:{0}\n", item.MaxY));
                count--;
            }

            return result.ToString();
        }
        #endregion
    }
}
