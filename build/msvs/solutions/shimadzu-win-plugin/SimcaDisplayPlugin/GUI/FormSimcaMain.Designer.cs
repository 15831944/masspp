namespace SimcaDisplay.Gui
{
    partial class FormSimcaMain
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this._statusStrip = new System.Windows.Forms.StatusStrip();
            this.toolStripLabelDummy = new System.Windows.Forms.ToolStripStatusLabel();
            this._toolStripLabelPos = new System.Windows.Forms.ToolStripStatusLabel();
            this._label1 = new System.Windows.Forms.Label();
            this._comboBoxSelMatrix = new System.Windows.Forms.ComboBox();
            this._checkBoxSyncQuant = new System.Windows.Forms.CheckBox();
            this._groupBox1 = new System.Windows.Forms.GroupBox();
            this._checkBoxShowTables = new System.Windows.Forms.CheckBox();
            this._buttonSelSPlot = new System.Windows.Forms.Button();
            this._labelSPlotPath = new System.Windows.Forms.Label();
            this._labelSPlot = new System.Windows.Forms.Label();
            this._buttonSelLoading = new System.Windows.Forms.Button();
            this._labelLoadingPath = new System.Windows.Forms.Label();
            this._labelLoading = new System.Windows.Forms.Label();
            this._buttonSelScore = new System.Windows.Forms.Button();
            this._labelScorePath = new System.Windows.Forms.Label();
            this._labelScore = new System.Windows.Forms.Label();
            this._tabControlPlot = new System.Windows.Forms.TabControl();
            this.tabScore = new System.Windows.Forms.TabPage();
            this.tabLoading = new System.Windows.Forms.TabPage();
            this.tabSPlot = new System.Windows.Forms.TabPage();
            this._buttonClose = new System.Windows.Forms.Button();
            this._labelSizeCheck = new System.Windows.Forms.Label();
            this._graphScore = new SimcaDisplay.Gui.UserControlSimcaGraph();
            this._graphLoading = new SimcaDisplay.Gui.UserControlSimcaGraph();
            this._graphSPlot = new SimcaDisplay.Gui.UserControlSimcaGraph();
            this._statusStrip.SuspendLayout();
            this._groupBox1.SuspendLayout();
            this._tabControlPlot.SuspendLayout();
            this.tabScore.SuspendLayout();
            this.tabLoading.SuspendLayout();
            this.tabSPlot.SuspendLayout();
            this.SuspendLayout();
            // 
            // _statusStrip
            // 
            this._statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabelDummy,
            this._toolStripLabelPos});
            this._statusStrip.Location = new System.Drawing.Point(0, 518);
            this._statusStrip.Name = "_statusStrip";
            this._statusStrip.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this._statusStrip.Size = new System.Drawing.Size(759, 23);
            this._statusStrip.TabIndex = 0;
            this._statusStrip.Text = "statusStrip";
            // 
            // toolStripLabelDummy
            // 
            this.toolStripLabelDummy.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripLabelDummy.Name = "toolStripLabelDummy";
            this.toolStripLabelDummy.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.toolStripLabelDummy.Size = new System.Drawing.Size(692, 18);
            this.toolStripLabelDummy.Spring = true;
            // 
            // _toolStripLabelPos
            // 
            this._toolStripLabelPos.Name = "_toolStripLabelPos";
            this._toolStripLabelPos.Size = new System.Drawing.Size(52, 18);
            this._toolStripLabelPos.Text = "(---,---)";
            // 
            // _label1
            // 
            this._label1.AutoSize = true;
            this._label1.Location = new System.Drawing.Point(12, 9);
            this._label1.Name = "_label1";
            this._label1.Size = new System.Drawing.Size(77, 12);
            this._label1.TabIndex = 1;
            this._label1.Text = "Select　Matrix";
            // 
            // _comboBoxSelMatrix
            // 
            this._comboBoxSelMatrix.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this._comboBoxSelMatrix.Enabled = false;
            this._comboBoxSelMatrix.FormattingEnabled = true;
            this._comboBoxSelMatrix.Items.AddRange(new object[] {
            "aaa",
            "bbb",
            "ccc"});
            this._comboBoxSelMatrix.Location = new System.Drawing.Point(116, 6);
            this._comboBoxSelMatrix.Name = "_comboBoxSelMatrix";
            this._comboBoxSelMatrix.Size = new System.Drawing.Size(284, 20);
            this._comboBoxSelMatrix.TabIndex = 2;
            // 
            // _checkBoxSyncQuant
            // 
            this._checkBoxSyncQuant.AutoSize = true;
            this._checkBoxSyncQuant.Checked = true;
            this._checkBoxSyncQuant.CheckState = System.Windows.Forms.CheckState.Checked;
            this._checkBoxSyncQuant.Location = new System.Drawing.Point(421, 9);
            this._checkBoxSyncQuant.Name = "_checkBoxSyncQuant";
            this._checkBoxSyncQuant.Size = new System.Drawing.Size(153, 16);
            this._checkBoxSyncQuant.TabIndex = 3;
            this._checkBoxSyncQuant.Text = "Sync. Quantitation Dialog";
            this._checkBoxSyncQuant.UseVisualStyleBackColor = true;
            this._checkBoxSyncQuant.CheckedChanged += new System.EventHandler(this.CheckBoxSyncQuant_CheckedChanged);
            // 
            // _groupBox1
            // 
            this._groupBox1.Controls.Add(this._checkBoxShowTables);
            this._groupBox1.Controls.Add(this._buttonSelSPlot);
            this._groupBox1.Controls.Add(this._labelSPlotPath);
            this._groupBox1.Controls.Add(this._labelSPlot);
            this._groupBox1.Controls.Add(this._buttonSelLoading);
            this._groupBox1.Controls.Add(this._labelLoadingPath);
            this._groupBox1.Controls.Add(this._labelLoading);
            this._groupBox1.Controls.Add(this._buttonSelScore);
            this._groupBox1.Controls.Add(this._labelScorePath);
            this._groupBox1.Controls.Add(this._labelScore);
            this._groupBox1.Location = new System.Drawing.Point(4, 32);
            this._groupBox1.Name = "_groupBox1";
            this._groupBox1.Size = new System.Drawing.Size(611, 117);
            this._groupBox1.TabIndex = 4;
            this._groupBox1.TabStop = false;
            this._groupBox1.Text = "SIMCA Result";
            // 
            // _checkBoxShowTables
            // 
            this._checkBoxShowTables.Appearance = System.Windows.Forms.Appearance.Button;
            this._checkBoxShowTables.Location = new System.Drawing.Point(489, 53);
            this._checkBoxShowTables.Name = "_checkBoxShowTables";
            this._checkBoxShowTables.Size = new System.Drawing.Size(89, 47);
            this._checkBoxShowTables.TabIndex = 9;
            this._checkBoxShowTables.Text = "Show Tables...";
            this._checkBoxShowTables.UseVisualStyleBackColor = true;
            this._checkBoxShowTables.CheckedChanged += new System.EventHandler(this.CheckBoxShowTables_CheckedChanged);
            // 
            // _buttonSelSPlot
            // 
            this._buttonSelSPlot.Location = new System.Drawing.Point(395, 77);
            this._buttonSelSPlot.Name = "_buttonSelSPlot";
            this._buttonSelSPlot.Size = new System.Drawing.Size(75, 23);
            this._buttonSelSPlot.TabIndex = 8;
            this._buttonSelSPlot.Text = "Select...";
            this._buttonSelSPlot.UseVisualStyleBackColor = true;
            this._buttonSelSPlot.Click += new System.EventHandler(this.ButtonSelSPlot_Click);
            // 
            // _labelSPlotPath
            // 
            this._labelSPlotPath.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this._labelSPlotPath.Location = new System.Drawing.Point(84, 78);
            this._labelSPlotPath.Name = "_labelSPlotPath";
            this._labelSPlotPath.Size = new System.Drawing.Size(302, 21);
            this._labelSPlotPath.TabIndex = 7;
            this._labelSPlotPath.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // _labelSPlot
            // 
            this._labelSPlot.AutoSize = true;
            this._labelSPlot.Location = new System.Drawing.Point(15, 83);
            this._labelSPlot.Name = "_labelSPlot";
            this._labelSPlot.Size = new System.Drawing.Size(36, 12);
            this._labelSPlot.TabIndex = 6;
            this._labelSPlot.Text = "S Plot";
            // 
            // _buttonSelLoading
            // 
            this._buttonSelLoading.Location = new System.Drawing.Point(395, 47);
            this._buttonSelLoading.Name = "_buttonSelLoading";
            this._buttonSelLoading.Size = new System.Drawing.Size(75, 23);
            this._buttonSelLoading.TabIndex = 5;
            this._buttonSelLoading.Text = "Select...";
            this._buttonSelLoading.UseVisualStyleBackColor = true;
            this._buttonSelLoading.Click += new System.EventHandler(this.ButtonSelLoading_Click);
            // 
            // _labelLoadingPath
            // 
            this._labelLoadingPath.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this._labelLoadingPath.Location = new System.Drawing.Point(84, 48);
            this._labelLoadingPath.Name = "_labelLoadingPath";
            this._labelLoadingPath.Size = new System.Drawing.Size(302, 21);
            this._labelLoadingPath.TabIndex = 4;
            this._labelLoadingPath.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // _labelLoading
            // 
            this._labelLoading.AutoSize = true;
            this._labelLoading.Location = new System.Drawing.Point(15, 53);
            this._labelLoading.Name = "_labelLoading";
            this._labelLoading.Size = new System.Drawing.Size(44, 12);
            this._labelLoading.TabIndex = 3;
            this._labelLoading.Text = "Loading";
            // 
            // _buttonSelScore
            // 
            this._buttonSelScore.Location = new System.Drawing.Point(395, 19);
            this._buttonSelScore.Name = "_buttonSelScore";
            this._buttonSelScore.Size = new System.Drawing.Size(75, 23);
            this._buttonSelScore.TabIndex = 2;
            this._buttonSelScore.Text = "Select...";
            this._buttonSelScore.UseVisualStyleBackColor = true;
            this._buttonSelScore.Click += new System.EventHandler(this.ButtonSelScore_Click);
            // 
            // _labelScorePath
            // 
            this._labelScorePath.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this._labelScorePath.Font = new System.Drawing.Font("MS UI Gothic", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this._labelScorePath.Location = new System.Drawing.Point(84, 20);
            this._labelScorePath.Name = "_labelScorePath";
            this._labelScorePath.Size = new System.Drawing.Size(302, 21);
            this._labelScorePath.TabIndex = 1;
            this._labelScorePath.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // _labelScore
            // 
            this._labelScore.AutoSize = true;
            this._labelScore.Font = new System.Drawing.Font("MS UI Gothic", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this._labelScore.Location = new System.Drawing.Point(15, 25);
            this._labelScore.Name = "_labelScore";
            this._labelScore.Size = new System.Drawing.Size(39, 12);
            this._labelScore.TabIndex = 0;
            this._labelScore.Text = "Score";
            // 
            // _tabControlPlot
            // 
            this._tabControlPlot.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this._tabControlPlot.Controls.Add(this.tabScore);
            this._tabControlPlot.Controls.Add(this.tabLoading);
            this._tabControlPlot.Controls.Add(this.tabSPlot);
            this._tabControlPlot.Location = new System.Drawing.Point(0, 155);
            this._tabControlPlot.Name = "_tabControlPlot";
            this._tabControlPlot.SelectedIndex = 0;
            this._tabControlPlot.Size = new System.Drawing.Size(759, 325);
            this._tabControlPlot.TabIndex = 5;
            this._tabControlPlot.SelectedIndexChanged += new System.EventHandler(this.TabControlPlot_SelectedIndexChanged);
            // 
            // tabScore
            // 
            this.tabScore.BackColor = System.Drawing.SystemColors.Control;
            this.tabScore.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.tabScore.Controls.Add(this._graphScore);
            this.tabScore.Location = new System.Drawing.Point(4, 22);
            this.tabScore.Name = "tabScore";
            this.tabScore.Padding = new System.Windows.Forms.Padding(3);
            this.tabScore.Size = new System.Drawing.Size(751, 299);
            this.tabScore.TabIndex = 0;
            this.tabScore.Text = "Score Plot";
            // 
            // tabLoading
            // 
            this.tabLoading.BackColor = System.Drawing.SystemColors.Control;
            this.tabLoading.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.tabLoading.Controls.Add(this._graphLoading);
            this.tabLoading.Location = new System.Drawing.Point(4, 22);
            this.tabLoading.Name = "tabLoading";
            this.tabLoading.Padding = new System.Windows.Forms.Padding(3);
            this.tabLoading.Size = new System.Drawing.Size(751, 299);
            this.tabLoading.TabIndex = 1;
            this.tabLoading.Text = "Loading Plot";
            // 
            // tabSPlot
            // 
            this.tabSPlot.BackColor = System.Drawing.SystemColors.Control;
            this.tabSPlot.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.tabSPlot.Controls.Add(this._graphSPlot);
            this.tabSPlot.Location = new System.Drawing.Point(4, 22);
            this.tabSPlot.Name = "tabSPlot";
            this.tabSPlot.Size = new System.Drawing.Size(751, 299);
            this.tabSPlot.TabIndex = 2;
            this.tabSPlot.Text = "S Plot";
            // 
            // _buttonClose
            // 
            this._buttonClose.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this._buttonClose.Location = new System.Drawing.Point(673, 486);
            this._buttonClose.Name = "_buttonClose";
            this._buttonClose.Size = new System.Drawing.Size(82, 23);
            this._buttonClose.TabIndex = 6;
            this._buttonClose.Text = "Close";
            this._buttonClose.UseVisualStyleBackColor = true;
            this._buttonClose.Click += new System.EventHandler(this.ButtonClose_Click);
            // 
            // _labelSizeCheck
            // 
            this._labelSizeCheck.AutoSize = true;
            this._labelSizeCheck.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this._labelSizeCheck.Font = new System.Drawing.Font("MS UI Gothic", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this._labelSizeCheck.Location = new System.Drawing.Point(172, 486);
            this._labelSizeCheck.Name = "_labelSizeCheck";
            this._labelSizeCheck.Size = new System.Drawing.Size(2, 14);
            this._labelSizeCheck.TabIndex = 7;
            this._labelSizeCheck.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this._labelSizeCheck.Visible = false;
            // 
            // _graphScore
            // 
            this._graphScore.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this._graphScore.DataType = SimcaDisplay.Data.SimcaData.SIMCA_DATA_TYPE.SCORE_PLOT;
            this._graphScore.Location = new System.Drawing.Point(1, 1);
            this._graphScore.Name = "_graphScore";
            this._graphScore.Size = new System.Drawing.Size(745, 293);
            this._graphScore.TabIndex = 0;
            // 
            // _graphLoading
            // 
            this._graphLoading.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this._graphLoading.DataType = SimcaDisplay.Data.SimcaData.SIMCA_DATA_TYPE.SCORE_PLOT;
            this._graphLoading.Location = new System.Drawing.Point(1, 1);
            this._graphLoading.Name = "_graphLoading";
            this._graphLoading.Size = new System.Drawing.Size(745, 293);
            this._graphLoading.TabIndex = 1;
            // 
            // _graphSPlot
            // 
            this._graphSPlot.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this._graphSPlot.DataType = SimcaDisplay.Data.SimcaData.SIMCA_DATA_TYPE.SCORE_PLOT;
            this._graphSPlot.Location = new System.Drawing.Point(1, 1);
            this._graphSPlot.Name = "_graphSPlot";
            this._graphSPlot.Size = new System.Drawing.Size(745, 293);
            this._graphSPlot.TabIndex = 1;
            // 
            // FormSimcaMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(759, 541);
            this.Controls.Add(this._labelSizeCheck);
            this.Controls.Add(this._buttonClose);
            this.Controls.Add(this._tabControlPlot);
            this.Controls.Add(this._groupBox1);
            this.Controls.Add(this._checkBoxSyncQuant);
            this.Controls.Add(this._comboBoxSelMatrix);
            this.Controls.Add(this._label1);
            this.Controls.Add(this._statusStrip);
            this.Name = "FormSimcaMain";
            this.Text = "Display SIMCA Result";
            this.Activated += new System.EventHandler(this.FormSimcaMain_Activated);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.FormSimcaMain_FormClosed);
            this._statusStrip.ResumeLayout(false);
            this._statusStrip.PerformLayout();
            this._groupBox1.ResumeLayout(false);
            this._groupBox1.PerformLayout();
            this._tabControlPlot.ResumeLayout(false);
            this.tabScore.ResumeLayout(false);
            this.tabLoading.ResumeLayout(false);
            this.tabSPlot.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.StatusStrip _statusStrip;
        private System.Windows.Forms.Label _label1;
        private System.Windows.Forms.ComboBox _comboBoxSelMatrix;
        private System.Windows.Forms.CheckBox _checkBoxSyncQuant;
        private System.Windows.Forms.GroupBox _groupBox1;
        private System.Windows.Forms.Button _buttonSelLoading;
        private System.Windows.Forms.Label _labelLoadingPath;
        private System.Windows.Forms.Label _labelLoading;
        private System.Windows.Forms.Button _buttonSelScore;
        private System.Windows.Forms.Label _labelScorePath;
        private System.Windows.Forms.Label _labelScore;
        private System.Windows.Forms.ToolStripStatusLabel toolStripLabelDummy;
        private System.Windows.Forms.CheckBox _checkBoxShowTables;
        private System.Windows.Forms.Button _buttonSelSPlot;
        private System.Windows.Forms.Label _labelSPlotPath;
        private System.Windows.Forms.Label _labelSPlot;
        private System.Windows.Forms.ToolStripStatusLabel _toolStripLabelPos;
        private System.Windows.Forms.TabControl _tabControlPlot;
        private System.Windows.Forms.TabPage tabScore;
        private System.Windows.Forms.TabPage tabLoading;
        private System.Windows.Forms.TabPage tabSPlot;
        private System.Windows.Forms.Button _buttonClose;
        private UserControlSimcaGraph _graphScore;
        private UserControlSimcaGraph _graphLoading;
        private UserControlSimcaGraph _graphSPlot;
        private System.Windows.Forms.Label _labelSizeCheck;
    }
}

