/// <summary>
/// [FILE] FormSimcaSelectAxis.cs
/// [ABSTRACT] SIMCA Plugin Grid display form class
/// Copyright (C) 2012-12-18 Shimadzu
/// </summary>

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using SimcaDisplay.Data;

namespace SimcaDisplay.Gui
{
    /// <summary>
    /// Select axis form class
    /// </summary>
    internal partial class FormSimcaSelectAxis : Form
    {
        #region --- Constants ------------------------------------------
        /// <summary>
        /// The number of the maximum selection branches.
        /// </summary>
        private const int MAX_SELECT_NO = 10;
        
        /// <summary>
        /// The minimum of the height of form.
        /// </summary>
        private const int FORM_HEIGHT_MIN = 120;
        
        /// <summary>
        /// The additional value of the height of form. 
        /// </summary>
        private const int FORM_HEIGHT_ADD = 22;
        
        /// <summary>
        /// The minimum of the width of form.
        /// </summary>
        private const int FORM_WIDTH_MIN = 228;
        
        /// <summary>
        /// The height of a radio button.
        /// </summary>
        private const int RADIO_BTN_HEIGHT = 18;
        
        /// <summary>
        /// The width of a radio button.
        /// </summary>
        private const int RADIO_BTN_WIDTH = 172;
        
        /// <summary>
        /// Radio button location(TOP)
        /// </summary>
        private const int RADIO_BTN_LOC_TOP = 18;
        
        /// <summary>
        /// Radio button location(LEFT)
        /// </summary>
        private const int RADIO_BTN_LOC_LEFT = 18;
        
        /// <summary>
        /// Radio button location(additional value)
        /// </summary>
        private const int RADIO_BTN_LOC_ADD = 22;

        /// <summary>
        /// Radio button property(AutoSize)
        /// </summary>
        private const bool RADIO_BTN_AUTO_SIZE = false;
        
        /// <summary>
        /// Radio button property(Name) 1st Axis
        /// </summary>
        private const string RADIO_BTN_NAME_1ST = "_radioBotton1st_";

        /// <summary>
        /// Radio button property(Name) 2nd Axis
        /// </summary>
        private const string RADIO_BTN_NAME_2ND = "_radioBotton2nd_";
        #endregion

        #region --- Properties -----------------------------------------
        /// <summary>
        /// Gets selected axis X index
        /// </summary>
        public int SelectAxisIndexX { get; private set; }

        /// <summary>
        /// Gets selected axis Y index
        /// </summary>
        public int SelectAxisIndexY { get; private set; }
        #endregion

        #region --- Variables ------------------------------------------
        /// <summary>
        /// Radio button array(1st axis)
        /// </summary>
        private RadioButton[] _radioButtonArray1st = null;

        /// <summary>
        /// Radio button array(2nd axis)
        /// </summary>
        private RadioButton[] _radioButtonArray2nd = null;

        /// <summary>
        /// Data type
        /// </summary>
        private SimcaData.SIMCA_DATA_TYPE _dataType = SimcaData.SIMCA_DATA_TYPE.SCORE_PLOT;
        #endregion

        #region --- Construction ---------------------------------------
        /// <summary>
        /// Initializes a new instance of the FormSimcaSelectAxis class
        /// </summary>
        public FormSimcaSelectAxis()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Initializes a new instance of the FormSimcaSelectAxis class
        /// </summary>
        /// <param name="data">SIMCA data</param>
        public FormSimcaSelectAxis(SimcaData data)
        {
            InitializeComponent();

            Size formSize = this.Size;

            string[] axisNameArray = data.GetAxisNameArray();
            if (axisNameArray == null)
            {
                return;
            }

            _dataType = data.DataType;

            this.SuspendLayout();

            // Create option button (1st(X) Axis) 
            _radioButtonArray1st = new RadioButton[axisNameArray.Count()];
            formSize.Height = FORM_HEIGHT_MIN;
            for (int index = 0; index < axisNameArray.Count(); index++)
            {
                formSize.Height += FORM_HEIGHT_ADD;

                // Create instance
                RadioButton btn = _radioButtonArray1st[index] = new RadioButton();

                // Set properties
                btn.AutoSize = RADIO_BTN_AUTO_SIZE;
                btn.Name = RADIO_BTN_NAME_1ST + index.ToString();
                btn.Text = axisNameArray[index];
                btn.Size = new Size(RADIO_BTN_WIDTH, RADIO_BTN_HEIGHT);
                btn.Location = new Point(
                    RADIO_BTN_LOC_LEFT,
                    (index * RADIO_BTN_LOC_ADD) + RADIO_BTN_LOC_TOP);

                // Set event
                btn.CheckedChanged += this.RadioButtonArray1st_CheckedChanged;

                if ((index + 1) >= MAX_SELECT_NO)
                {
                    break;
                }
            }

            // Control is added to a group box
            _groupBox1st.Controls.AddRange(_radioButtonArray1st);

            // Create option button (2nd(Y) Axis) 
            if (_dataType == SimcaData.SIMCA_DATA_TYPE.S_PLOT)
            {
                _groupBox2nd.Visible = false;
                formSize.Width = FORM_WIDTH_MIN;
            }
            else
            {
                _radioButtonArray2nd = new RadioButton[axisNameArray.Count()];
                for (int index = 0; index < axisNameArray.Count(); index++)
                {
                    // Create instance
                    RadioButton btn = _radioButtonArray2nd[index] = new RadioButton();
                    
                    // Set properties
                    btn.AutoSize = RADIO_BTN_AUTO_SIZE;
                    btn.Name = RADIO_BTN_NAME_2ND + index.ToString();
                    btn.Text = axisNameArray[index];
                    btn.Size = new Size(RADIO_BTN_WIDTH, RADIO_BTN_HEIGHT);
                    btn.Location = new Point(
                        RADIO_BTN_LOC_LEFT,
                        (index * RADIO_BTN_LOC_ADD) + RADIO_BTN_LOC_TOP);

                    // Set event
                    btn.CheckedChanged += this.RadioButtonArray2nd_CheckedChanged;

                    if ((index + 1) >= MAX_SELECT_NO)
                    {
                        break;
                    }
                }

                // Set select index
                _radioButtonArray2nd[data.AxisYDataIndex].Checked = true;
                this.SelectAxisIndexY = data.AxisYDataIndex;

                // Control is added to a group box
                _groupBox2nd.Controls.AddRange(_radioButtonArray2nd);
            }

            // Set select index
            _radioButtonArray1st[data.AxisXDataIndex].Checked = true;
            this.SelectAxisIndexX = data.AxisXDataIndex;

            this.ResumeLayout(false);

            this.Size = formSize;
        }
        #endregion

        #region --- Private Events -------------------------------------
        /// <summary>
        /// Radio button array(1st) CheckedChanged event handler
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event args</param>
        private void RadioButtonArray1st_CheckedChanged(object sender, EventArgs e)
        {
            int index = 0;
            foreach (RadioButton item in _radioButtonArray1st)
            {
                if (item.Checked)
                {
                    this.SelectAxisIndexX = index;

                    if (_dataType == SimcaData.SIMCA_DATA_TYPE.S_PLOT)
                    {
                        _buttonOk.Enabled = true;
                    }
                    else
                    {
                        _buttonOk.Enabled = !_radioButtonArray2nd[index].Checked;
                    }

                    break;
                }

                index++;
            }
        }

        /// <summary>
        /// Radio button array(2nd) CheckedChanged event handler
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event args</param>
        private void RadioButtonArray2nd_CheckedChanged(object sender, EventArgs e)
        {
            int index = 0;
            foreach (RadioButton item in _radioButtonArray2nd)
            {
                if (item.Checked)
                {
                    this.SelectAxisIndexY = index;
                    _buttonOk.Enabled = !_radioButtonArray1st[index].Checked;
                    break;
                }

                index++;
            }
        }
        #endregion
    }
}
