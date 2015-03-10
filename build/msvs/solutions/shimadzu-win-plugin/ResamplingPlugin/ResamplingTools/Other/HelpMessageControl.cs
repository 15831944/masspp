/// <summary>
/// [FILE] HelpMessageControl.cs
/// [ABSTRACT] LDIA for Pep plugin - Help message control class
/// Copyright (C) 2013-05-23 Shimadzu
/// </summary>

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace ResamplingPlugin.ResamplingTools.Other
{
    /// <summary> Help message control class </summary>
    internal class HelpMessageControl
    {
        #region --- API ------------------------------------------------
        /// <summary>
        /// Windows Form Point.
        /// </summary>
        /// <param name="Point">point</param>
        /// <returns>pointer</returns>
        [DllImport("user32.dll")]
        private static extern IntPtr WindowFromPoint(POINT Point);

        /// <summary>
        /// Point structure.
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        public struct POINT
        {
            public int x;
            public int y;
        }
        #endregion

        #region --- Structures -----------------------------------------
        /// <summary> Help item </summary>
        private struct HelpItem
        {
            internal Control ctl;
            internal string msg;
        }
        #endregion

        #region --- Constants ------------------------------------------
        /// <summary> Default Scan Interval (200msec) </summary>
        private const int DEFAULT_INTERVAL = 200;

        /// <summary> Message clear index </summary>
        private const int MESSAGE_CLEAR = -1;
        #endregion

        #region --- Properties -----------------------------------------
        /// <summary> Scan interval[msec] </summary>
        internal int ScanInterval
        {
            get
            {
                return _scanInterval;
            }
            set
            {
                _scanInterval = value;
                _scanTimer.Interval = _scanInterval;
            }
        }
        private int _scanInterval = DEFAULT_INTERVAL;

        /// <summary> Scan interval[msec] </summary>
        internal bool Enabled
        {
            get
            {
                return _enabled;
            }
            set
            {
                _enabled = value;
                _scanTimer.Enabled = _enabled;
            }
        }
        private bool _enabled = false;
        #endregion

        #region --- Variables ------------------------------------------
        /// <summary> Help item list </summary>
        private List<HelpItem> _helpList = new List<HelpItem>();

        /// <summary> Scan timer </summary>
        private Timer _scanTimer = new Timer();

        /// <summary> Last message index </summary>
        private int _lastMessageIndex = MESSAGE_CLEAR;
        #endregion

        #region --- Events ---------------------------------------------
        /// <summary> Event ChangeMessage </summary>
        internal event Action<string> ChangeMessage;
        #endregion

        #region --- Construction ---------------------------------------
        internal HelpMessageControl()
        {
            _helpList.Clear();
            _scanTimer.Enabled = _enabled;
            _scanTimer.Interval = _scanInterval;
            _scanTimer.Tick += ScanTimer_Tick;
        }
        #endregion

        #region --- Public Methods -------------------------------------
        /// <summary>
        /// Clear list
        /// </summary>
        internal void ClearList()
        {
            _helpList.Clear();
        }

        /// <summary>
        /// Add list
        /// </summary>
        /// <param name="ctl">Control</param>
        /// <param name="msg">Message</param>
        internal void AddList(Control ctl, string msg)
        {
            HelpItem hi = new HelpItem();
            hi.ctl = ctl;
            hi.msg = msg;
            _helpList.Add(hi);
        }
        #endregion

        #region --- Private Events -------------------------------------
        /// <summary>
        /// ScanTimer Tick event handler
        /// </summary>
        /// <param name="sender">object</param>
        /// <param name="e">EventArgs</param>
        private void ScanTimer_Tick(object sender, EventArgs e)
        {
            POINT mousePos = new POINT();
            mousePos.x = Cursor.Position.X;
            mousePos.y = Cursor.Position.Y;

            IntPtr handle = WindowFromPoint(mousePos);

            int messageIndex = 0;
            bool findControl = false;
            try
            {
                foreach (HelpItem item in _helpList)
                {
                    if (item.ctl.Handle == handle)
                    {
                        findControl = true;
                        if (_lastMessageIndex != messageIndex)
                        {
                            _lastMessageIndex = messageIndex;
                            ChangeMessage(item.msg);
                        }
                        break;
                    }
                    messageIndex++;
                }
                if (!findControl)
                {
                    if (_lastMessageIndex != MESSAGE_CLEAR)
                    {
                        _lastMessageIndex = MESSAGE_CLEAR;
                        ChangeMessage(string.Empty);
                    }
                }
            }
            catch { }
        }
        #endregion
    }
}
