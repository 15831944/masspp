namespace ResamplingPlugin.SpectrumCalculation.GUI
{
    partial class SpectrumCalculationDialog
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this._rbnRTmz = new System.Windows.Forms.RadioButton();
            this._rbnSpectrumName = new System.Windows.Forms.RadioButton();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this._btnResamling = new System.Windows.Forms.Button();
            this._rbnDivide = new System.Windows.Forms.RadioButton();
            this._rbnSum = new System.Windows.Forms.RadioButton();
            this._rbnSubtract = new System.Windows.Forms.RadioButton();
            this._rbnAverage = new System.Windows.Forms.RadioButton();
            this._btnCalc = new System.Windows.Forms.Button();
            this._btnCancel = new System.Windows.Forms.Button();
            this._spectrumCalculationGrid = new System.Windows.Forms.DataGridView();
            this._fileName_ComboBox_Column = new System.Windows.Forms.DataGridViewComboBoxColumn();
            this._selFileID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this._spectrumName_ComboBox_Column = new System.Windows.Forms.DataGridViewComboBoxColumn();
            this._rT_TextBox_Column = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this._precusor_TextBox_Column = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this._rT_ComboBox_Column = new System.Windows.Forms.DataGridViewComboBoxColumn();
            this._precursor_ComboBox_Column = new System.Windows.Forms.DataGridViewComboBoxColumn();
            this._spectrumName_TextBox_Column = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.deleteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this._textBoxHelpMessage = new System.Windows.Forms.TextBox();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this._spectrumCalculationGrid)).BeginInit();
            this.contextMenuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this._rbnRTmz);
            this.groupBox1.Controls.Add(this._rbnSpectrumName);
            this.groupBox1.Location = new System.Drawing.Point(664, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(254, 50);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Select Spectra by";
            // 
            // _rbnRTmz
            // 
            this._rbnRTmz.AutoSize = true;
            this._rbnRTmz.Location = new System.Drawing.Point(158, 18);
            this._rbnRTmz.Name = "_rbnRTmz";
            this._rbnRTmz.Size = new System.Drawing.Size(64, 16);
            this._rbnRTmz.TabIndex = 1;
            this._rbnRTmz.TabStop = true;
            this._rbnRTmz.Text = "RT, m/z";
            this._rbnRTmz.UseVisualStyleBackColor = true;
            this._rbnRTmz.CheckedChanged += new System.EventHandler(this.SelectRbn_CheckedChanged);
            // 
            // _rbnSpectrumName
            // 
            this._rbnSpectrumName.AutoSize = true;
            this._rbnSpectrumName.Location = new System.Drawing.Point(21, 18);
            this._rbnSpectrumName.Name = "_rbnSpectrumName";
            this._rbnSpectrumName.Size = new System.Drawing.Size(81, 16);
            this._rbnSpectrumName.TabIndex = 0;
            this._rbnSpectrumName.TabStop = true;
            this._rbnSpectrumName.Text = "Spectrum Name";
            this._rbnSpectrumName.UseVisualStyleBackColor = true;
            this._rbnSpectrumName.CheckedChanged += new System.EventHandler(this.SelectRbn_CheckedChanged);
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.groupBox2.Controls.Add(this._btnResamling);
            this.groupBox2.Controls.Add(this._rbnDivide);
            this.groupBox2.Controls.Add(this._rbnSum);
            this.groupBox2.Controls.Add(this._rbnSubtract);
            this.groupBox2.Controls.Add(this._rbnAverage);
            this.groupBox2.Location = new System.Drawing.Point(33, 490);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(505, 100);
            this.groupBox2.TabIndex = 2;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Operation Settings";
            // 
            // _btnResamling
            // 
            this._btnResamling.Location = new System.Drawing.Point(379, 38);
            this._btnResamling.Name = "_btnResamling";
            this._btnResamling.Size = new System.Drawing.Size(88, 23);
            this._btnResamling.TabIndex = 4;
            this._btnResamling.Text = "Resampling ...";
            this._btnResamling.UseVisualStyleBackColor = true;
            this._btnResamling.Click += new System.EventHandler(this.BtnResamling_Click);
            // 
            // _rbnDivide
            // 
            this._rbnDivide.AutoSize = true;
            this._rbnDivide.Location = new System.Drawing.Point(116, 62);
            this._rbnDivide.Name = "_rbnDivide";
            this._rbnDivide.Size = new System.Drawing.Size(147, 16);
            this._rbnDivide.TabIndex = 3;
            this._rbnDivide.TabStop = true;
            this._rbnDivide.Text = "Divide by first spectrum";
            this._rbnDivide.UseVisualStyleBackColor = true;
            // 
            // _rbnSum
            // 
            this._rbnSum.AutoSize = true;
            this._rbnSum.Location = new System.Drawing.Point(28, 62);
            this._rbnSum.Name = "_rbnSum";
            this._rbnSum.Size = new System.Drawing.Size(45, 16);
            this._rbnSum.TabIndex = 2;
            this._rbnSum.TabStop = true;
            this._rbnSum.Text = "Sum";
            this._rbnSum.UseVisualStyleBackColor = true;
            // 
            // _rbnSubtract
            // 
            this._rbnSubtract.AutoSize = true;
            this._rbnSubtract.Location = new System.Drawing.Point(116, 29);
            this._rbnSubtract.Name = "_rbnSubtract";
            this._rbnSubtract.Size = new System.Drawing.Size(158, 16);
            this._rbnSubtract.TabIndex = 1;
            this._rbnSubtract.TabStop = true;
            this._rbnSubtract.Text = "Subtract by first spectrum";
            this._rbnSubtract.UseVisualStyleBackColor = true;
            // 
            // _rbnAverage
            // 
            this._rbnAverage.AutoSize = true;
            this._rbnAverage.Location = new System.Drawing.Point(28, 29);
            this._rbnAverage.Name = "_rbnAverage";
            this._rbnAverage.Size = new System.Drawing.Size(65, 16);
            this._rbnAverage.TabIndex = 0;
            this._rbnAverage.TabStop = true;
            this._rbnAverage.Text = "Average";
            this._rbnAverage.UseVisualStyleBackColor = true;
            // 
            // _btnCalc
            // 
            this._btnCalc.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this._btnCalc.Location = new System.Drawing.Point(822, 527);
            this._btnCalc.Name = "_btnCalc";
            this._btnCalc.Size = new System.Drawing.Size(75, 23);
            this._btnCalc.TabIndex = 3;
            this._btnCalc.Text = "Calc.";
            this._btnCalc.UseVisualStyleBackColor = true;
            this._btnCalc.Click += new System.EventHandler(this.BtnCalc_Click);
            // 
            // _btnCancel
            // 
            this._btnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this._btnCancel.Location = new System.Drawing.Point(932, 527);
            this._btnCancel.Name = "_btnCancel";
            this._btnCancel.Size = new System.Drawing.Size(75, 23);
            this._btnCancel.TabIndex = 4;
            this._btnCancel.Text = "Cancel";
            this._btnCancel.UseVisualStyleBackColor = true;
            this._btnCancel.Click += new System.EventHandler(this.BtnCancel_Click);
            // 
            // _SpectrumCalculationGrid
            // 
            this._spectrumCalculationGrid.AllowUserToAddRows = false;
            this._spectrumCalculationGrid.AllowUserToDeleteRows = false;
            this._spectrumCalculationGrid.AllowUserToResizeColumns = false;
            this._spectrumCalculationGrid.AllowUserToResizeRows = false;
            this._spectrumCalculationGrid.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._spectrumCalculationGrid.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this._spectrumCalculationGrid.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this._fileName_ComboBox_Column,
            this._selFileID,
            this._spectrumName_ComboBox_Column,
            this._rT_TextBox_Column,
            this._precusor_TextBox_Column,
            this._rT_ComboBox_Column,
            this._precursor_ComboBox_Column,
            this._spectrumName_TextBox_Column});
            this._spectrumCalculationGrid.ContextMenuStrip = this.contextMenuStrip1;
            this._spectrumCalculationGrid.Location = new System.Drawing.Point(12, 82);
            this._spectrumCalculationGrid.MultiSelect = false;
            this._spectrumCalculationGrid.Name = "_SpectrumCalculationGrid";
            this._spectrumCalculationGrid.RowHeadersWidthSizeMode = System.Windows.Forms.DataGridViewRowHeadersWidthSizeMode.DisableResizing;
            this._spectrumCalculationGrid.RowTemplate.Height = 21;
            this._spectrumCalculationGrid.Size = new System.Drawing.Size(1101, 244);
            this._spectrumCalculationGrid.TabIndex = 1;
            this._spectrumCalculationGrid.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.SpectrumCalculationGrid_CellClick);
            this._spectrumCalculationGrid.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.DataGridView_CellEndEdit);
            this._spectrumCalculationGrid.EditingControlShowing += new System.Windows.Forms.DataGridViewEditingControlShowingEventHandler(this.DataGridView_EditingControlShowing);
            // 
            // FileName_ComboBox_Column
            // 
            this._fileName_ComboBox_Column.AutoComplete = false;
            this._fileName_ComboBox_Column.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this._fileName_ComboBox_Column.FillWeight = 200F;
            this._fileName_ComboBox_Column.HeaderText = "File Name";
            this._fileName_ComboBox_Column.Items.AddRange(new object[] {
            "(Select MS File)",
            "Browse..."});
            this._fileName_ComboBox_Column.Name = "FileName_ComboBox_Column";
            this._fileName_ComboBox_Column.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            // 
            // SelFileID
            // 
            this._selFileID.HeaderText = "SelID";
            this._selFileID.Name = "SelFileID";
            this._selFileID.Visible = false;
            // 
            // SpectrumName_ComboBox_Column
            // 
            this._spectrumName_ComboBox_Column.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this._spectrumName_ComboBox_Column.HeaderText = "Spectrum Name";
            this._spectrumName_ComboBox_Column.Name = "SpectrumName_ComboBox_Column";
            this._spectrumName_ComboBox_Column.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            // 
            // RT_TextBox_Column
            // 
            this._rT_TextBox_Column.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this._rT_TextBox_Column.HeaderText = "RT";
            this._rT_TextBox_Column.Name = "RT_TextBox_Column";
            this._rT_TextBox_Column.ReadOnly = true;
            this._rT_TextBox_Column.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            this._rT_TextBox_Column.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // Precusor_TextBox_Column
            // 
            this._precusor_TextBox_Column.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this._precusor_TextBox_Column.HeaderText = "Precursor";
            this._precusor_TextBox_Column.Name = "Precusor_TextBox_Column";
            this._precusor_TextBox_Column.ReadOnly = true;
            this._precusor_TextBox_Column.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            this._precusor_TextBox_Column.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // RT_ComboBox_Column
            // 
            this._rT_ComboBox_Column.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this._rT_ComboBox_Column.HeaderText = "RT";
            this._rT_ComboBox_Column.Name = "RT_ComboBox_Column";
            this._rT_ComboBox_Column.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            this._rT_ComboBox_Column.Visible = false;
            // 
            // Precursor_ComboBox_Column
            // 
            this._precursor_ComboBox_Column.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this._precursor_ComboBox_Column.HeaderText = "Precursor";
            this._precursor_ComboBox_Column.Name = "Precursor_ComboBox_Column";
            this._precursor_ComboBox_Column.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            this._precursor_ComboBox_Column.Visible = false;
            // 
            // SpectrumName_TextBox_Column
            // 
            this._spectrumName_TextBox_Column.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this._spectrumName_TextBox_Column.HeaderText = "Spectrum Name";
            this._spectrumName_TextBox_Column.Name = "SpectrumName_TextBox_Column";
            this._spectrumName_TextBox_Column.ReadOnly = true;
            this._spectrumName_TextBox_Column.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            this._spectrumName_TextBox_Column.Visible = false;
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.deleteToolStripMenuItem});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(115, 26);
            // 
            // deleteToolStripMenuItem
            // 
            this.deleteToolStripMenuItem.Name = "deleteToolStripMenuItem";
            this.deleteToolStripMenuItem.Size = new System.Drawing.Size(114, 22);
            this.deleteToolStripMenuItem.Text = "Delete";
            this.deleteToolStripMenuItem.Click += new System.EventHandler(this.DeleteToolStripMenuItem_Click);
            // 
            // textBox1
            // 
            this.textBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBox1.Location = new System.Drawing.Point(0, 0);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(546, 121);
            this.textBox1.TabIndex = 12;
            // 
            // _textBoxHelpMessage
            // 
            this._textBoxHelpMessage.Dock = System.Windows.Forms.DockStyle.Fill;
            this._textBoxHelpMessage.Location = new System.Drawing.Point(0, 0);
            this._textBoxHelpMessage.Multiline = true;
            this._textBoxHelpMessage.Name = "_textBoxHelpMessage";
            this._textBoxHelpMessage.ReadOnly = true;
            this._textBoxHelpMessage.Size = new System.Drawing.Size(551, 121);
            this._textBoxHelpMessage.TabIndex = 11;
            // 
            // splitContainer1
            // 
            this.splitContainer1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.splitContainer1.Location = new System.Drawing.Point(12, 345);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this._textBoxHelpMessage);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.textBox1);
            this.splitContainer1.Size = new System.Drawing.Size(1101, 121);
            this.splitContainer1.SplitterDistance = 551;
            this.splitContainer1.TabIndex = 13;
            // 
            // SpectrumCalculationDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1125, 596);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this._btnCancel);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this._btnCalc);
            this.Controls.Add(this._spectrumCalculationGrid);
            this.Name = "SpectrumCalculationDialog";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Spectrum Calculation";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.SpectrumCalculationDialog_FormClosed);
            this.Load += new System.EventHandler(this.SpectrumCalculationDialog_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this._spectrumCalculationGrid)).EndInit();
            this.contextMenuStrip1.ResumeLayout(false);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton _rbnRTmz;
        private System.Windows.Forms.RadioButton _rbnSpectrumName;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button _btnResamling;
        private System.Windows.Forms.RadioButton _rbnDivide;
        private System.Windows.Forms.RadioButton _rbnSum;
        private System.Windows.Forms.RadioButton _rbnSubtract;
        private System.Windows.Forms.RadioButton _rbnAverage;
        private System.Windows.Forms.Button _btnCalc;
        private System.Windows.Forms.Button _btnCancel;
        private System.Windows.Forms.DataGridView _spectrumCalculationGrid;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem deleteToolStripMenuItem;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox _textBoxHelpMessage;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.DataGridViewComboBoxColumn _fileName_ComboBox_Column;
        private System.Windows.Forms.DataGridViewTextBoxColumn _selFileID;
        private System.Windows.Forms.DataGridViewComboBoxColumn _spectrumName_ComboBox_Column;
        private System.Windows.Forms.DataGridViewTextBoxColumn _rT_TextBox_Column;
        private System.Windows.Forms.DataGridViewTextBoxColumn _precusor_TextBox_Column;
        private System.Windows.Forms.DataGridViewComboBoxColumn _rT_ComboBox_Column;
        private System.Windows.Forms.DataGridViewComboBoxColumn _precursor_ComboBox_Column;
        private System.Windows.Forms.DataGridViewTextBoxColumn _spectrumName_TextBox_Column;
    }
}