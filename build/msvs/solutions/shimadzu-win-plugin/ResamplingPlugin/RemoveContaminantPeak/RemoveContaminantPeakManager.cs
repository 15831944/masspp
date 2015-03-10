/// <summary>
/// [FILE] RemoveContaminantPeakManager.cs
/// [ABSTRACT] Remove Contaninant Peak Manager Mass++ access class
/// Copyright (C) 2013-05-31 Shimadzu
/// </summary>

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

// Mass++ namespace
using kome.clr;
using ResamplingPlugin.RemoveContaminantPeak.GUI;

namespace ResamplingPlugin.RemoveContaminantPeak
{
    /// <summary>
    /// Remove contaminant peak manager class.
    /// </summary>
    class RemoveContaminantPeakManager
    {
        #region --- Variables ------------------------------------------

        /// <summary>
        /// RemoveContaminantPeakManager instance
        /// </summary>
        private static RemoveContaminantPeakManager _manager = new RemoveContaminantPeakManager();

        /// <summary>
        /// RemoveContaminantPeakDialog instance
        /// </summary>
        private RemoveContaminantPeakDialog _dlgRemovContamiPeak = null;

        #endregion

        #region --- Construction ---------------------------------------

        /// <summary>
        /// Initializes a new instance of the RemoveContaminantPeakManager class
        /// </summary>
        private RemoveContaminantPeakManager()
        {
        }
        #endregion

        #region --- Public Methods -------------------------------------

        /// <summary>
        /// Display Remove Contaminant Peak Dialog
        /// </summary>
        public static void DisplayDlgRemovContamiPeak(ClrVariant clrVar)
        {
            if (_manager._dlgRemovContamiPeak == null)
            {
                _manager._dlgRemovContamiPeak = new RemoveContaminantPeakDialog(clrVar);
                _manager._dlgRemovContamiPeak.DisplayClosed += _manager.RemoveContaminantPeakDialog_DisplayClosed;
                _manager._dlgRemovContamiPeak.Show();
            }
            else
            {
                _manager._dlgRemovContamiPeak.UpdateParam(clrVar);
                _manager._dlgRemovContamiPeak.Activate();
            }
        }
        #endregion

        #region --- Private Events -------------------------------------

        /// <summary>
        /// RemoveContaminantPeakDialog DisplayClosed event handler
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">FormClosedEventArgs</param>
        private void RemoveContaminantPeakDialog_DisplayClosed(object sender, FormClosedEventArgs e)
        {
            if (_manager._dlgRemovContamiPeak != null)
            {
                _manager._dlgRemovContamiPeak.DisplayClosed -= _manager.RemoveContaminantPeakDialog_DisplayClosed;
                _manager._dlgRemovContamiPeak.Dispose();
                _manager._dlgRemovContamiPeak = null;
            }
        }
        #endregion
    }
}


