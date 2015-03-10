/// <summary>
/// [FILE] SimcaDisplayPlugin.cs
/// [ABSTRACT] SIMCA Plugin Mass++ access class
/// Copyright (C) 2012-12-18 Shimadzu
/// </summary>

namespace SimcaDisplay
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Windows.Forms;

    // Mass++ namespace
    using kome.clr;
    using SimcaDisplay.Gui;

    /// <summary>
    /// SIMCA Display manager class
    /// </summary>
    sealed class SimcaDisplayManager
    {
        #region --- Variables ------------------------------------------
        /// <summary>
        /// SimcaDisplayManager instance
        /// </summary>
        private static SimcaDisplayManager _manager = new SimcaDisplayManager();

        /// <summary>
        /// FormSimcaMain instance
        /// </summary>
        private FormSimcaMain _formSimcaDisplay = null;
        #endregion

        #region --- Construction ---------------------------------------
        /// <summary>
        /// Initializes a new instance of the SimcaDisplayManager class
        /// </summary>
        private SimcaDisplayManager()
        {
        }
        #endregion

        #region --- Public Methods -------------------------------------
        /// <summary>
        /// Display main form
        /// </summary>
        public static void DisplayMainForm()
        {
            if (_manager._formSimcaDisplay == null)
            {
                _manager._formSimcaDisplay = new FormSimcaMain();
                _manager._formSimcaDisplay.DisplayClosed += _manager.FormSimcaDisplay_DisplayClosed;
                _manager._formSimcaDisplay.Show();
            }
            else
            {
                _manager._formSimcaDisplay.Activate();
            }
        }
        #endregion

        #region --- Private Events -------------------------------------
        /// <summary>
        /// FormSimcaDisplay DisplayClosed event handler
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">FormClosedEventArgs</param>
        private void FormSimcaDisplay_DisplayClosed(object sender, FormClosedEventArgs e)
        {
            if (_manager._formSimcaDisplay != null)
            {
                _manager._formSimcaDisplay.DisplayClosed -= _manager.FormSimcaDisplay_DisplayClosed;
                _manager._formSimcaDisplay.Dispose();
                _manager._formSimcaDisplay = null;
            }
        }
        #endregion
    }
}
