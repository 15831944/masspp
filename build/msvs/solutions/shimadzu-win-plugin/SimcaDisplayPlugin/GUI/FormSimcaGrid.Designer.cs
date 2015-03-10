namespace SimcaDisplay.Gui
{
    partial class FormSimcaGrid
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
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            this._buttonApply = new System.Windows.Forms.Button();
            this._buttonSave = new System.Windows.Forms.Button();
            this._buttonClose = new System.Windows.Forms.Button();
            this._tabControlGrid = new System.Windows.Forms.TabControl();
            this.tabScore = new System.Windows.Forms.TabPage();
            this._dataGridViewScore = new System.Windows.Forms.DataGridView();
            this._contextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this._menuItemPaste = new System.Windows.Forms.ToolStripMenuItem();
            this.tabLoading = new System.Windows.Forms.TabPage();
            this._dataGridViewLoading = new System.Windows.Forms.DataGridView();
            this.tabSPlot = new System.Windows.Forms.TabPage();
            this._dataGridViewSPlot = new System.Windows.Forms.DataGridView();
            this._tabControlGrid.SuspendLayout();
            this.tabScore.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this._dataGridViewScore)).BeginInit();
            this._contextMenuStrip.SuspendLayout();
            this.tabLoading.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this._dataGridViewLoading)).BeginInit();
            this.tabSPlot.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this._dataGridViewSPlot)).BeginInit();
            this.SuspendLayout();
            // 
            // _buttonApply
            // 
            this._buttonApply.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this._buttonApply.Enabled = false;
            this._buttonApply.Location = new System.Drawing.Point(166, 353);
            this._buttonApply.Name = "_buttonApply";
            this._buttonApply.Size = new System.Drawing.Size(90, 29);
            this._buttonApply.TabIndex = 0;
            this._buttonApply.Text = "Apply";
            this._buttonApply.UseVisualStyleBackColor = true;
            this._buttonApply.Click += new System.EventHandler(this.ButtonApply_Click);
            // 
            // _buttonSave
            // 
            this._buttonSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this._buttonSave.Enabled = false;
            this._buttonSave.Location = new System.Drawing.Point(273, 353);
            this._buttonSave.Name = "_buttonSave";
            this._buttonSave.Size = new System.Drawing.Size(90, 29);
            this._buttonSave.TabIndex = 1;
            this._buttonSave.Text = "Save as ...";
            this._buttonSave.UseVisualStyleBackColor = true;
            this._buttonSave.Click += new System.EventHandler(this.ButtonSave_Click);
            // 
            // _buttonClose
            // 
            this._buttonClose.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this._buttonClose.Location = new System.Drawing.Point(462, 353);
            this._buttonClose.Name = "_buttonClose";
            this._buttonClose.Size = new System.Drawing.Size(90, 29);
            this._buttonClose.TabIndex = 2;
            this._buttonClose.Text = "Close";
            this._buttonClose.UseVisualStyleBackColor = true;
            this._buttonClose.Click += new System.EventHandler(this.ButtonClose_Click);
            // 
            // _tabControlGrid
            // 
            this._tabControlGrid.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this._tabControlGrid.Controls.Add(this.tabScore);
            this._tabControlGrid.Controls.Add(this.tabLoading);
            this._tabControlGrid.Controls.Add(this.tabSPlot);
            this._tabControlGrid.Location = new System.Drawing.Point(0, 0);
            this._tabControlGrid.Name = "_tabControlGrid";
            this._tabControlGrid.SelectedIndex = 0;
            this._tabControlGrid.Size = new System.Drawing.Size(585, 347);
            this._tabControlGrid.TabIndex = 3;
            this._tabControlGrid.SelectedIndexChanged += new System.EventHandler(this.TabControlGrid_SelectedIndexChanged);
            // 
            // tabScore
            // 
            this.tabScore.Controls.Add(this._dataGridViewScore);
            this.tabScore.Location = new System.Drawing.Point(4, 22);
            this.tabScore.Name = "tabScore";
            this.tabScore.Padding = new System.Windows.Forms.Padding(3);
            this.tabScore.Size = new System.Drawing.Size(577, 321);
            this.tabScore.TabIndex = 0;
            this.tabScore.Text = "Score Plot";
            this.tabScore.UseVisualStyleBackColor = true;
            // 
            // _dataGridViewScore
            // 
            this._dataGridViewScore.AllowUserToAddRows = false;
            this._dataGridViewScore.AllowUserToDeleteRows = false;
            this._dataGridViewScore.AllowUserToResizeRows = false;
            this._dataGridViewScore.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
            dataGridViewCellStyle1.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle1.BackColor = System.Drawing.SystemColors.Control;
            dataGridViewCellStyle1.Font = new System.Drawing.Font("MS UI Gothic", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            dataGridViewCellStyle1.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle1.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle1.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle1.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this._dataGridViewScore.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle1;
            this._dataGridViewScore.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this._dataGridViewScore.ContextMenuStrip = this._contextMenuStrip;
            this._dataGridViewScore.Dock = System.Windows.Forms.DockStyle.Fill;
            this._dataGridViewScore.Location = new System.Drawing.Point(3, 3);
            this._dataGridViewScore.MultiSelect = false;
            this._dataGridViewScore.Name = "_dataGridViewScore";
            this._dataGridViewScore.ReadOnly = true;
            this._dataGridViewScore.RowHeadersVisible = false;
            this._dataGridViewScore.RowTemplate.Height = 21;
            this._dataGridViewScore.Size = new System.Drawing.Size(571, 315);
            this._dataGridViewScore.TabIndex = 0;
            this._dataGridViewScore.CellValueNeeded += new System.Windows.Forms.DataGridViewCellValueEventHandler(this.DataGridViewScore_CellValueNeeded);
            // 
            // _contextMenuStrip
            // 
            this._contextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this._menuItemPaste});
            this._contextMenuStrip.Name = "contextMenuStrip";
            this._contextMenuStrip.Size = new System.Drawing.Size(103, 26);
            // 
            // _menuItemPaste
            // 
            this._menuItemPaste.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this._menuItemPaste.Font = new System.Drawing.Font("MS UI Gothic", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this._menuItemPaste.Name = "_menuItemPaste";
            this._menuItemPaste.Size = new System.Drawing.Size(102, 22);
            this._menuItemPaste.Text = "Paste";
            this._menuItemPaste.Click += new System.EventHandler(this.MenuItemPaste_Click);
            // 
            // tabLoading
            // 
            this.tabLoading.Controls.Add(this._dataGridViewLoading);
            this.tabLoading.Location = new System.Drawing.Point(4, 22);
            this.tabLoading.Name = "tabLoading";
            this.tabLoading.Padding = new System.Windows.Forms.Padding(3);
            this.tabLoading.Size = new System.Drawing.Size(577, 321);
            this.tabLoading.TabIndex = 1;
            this.tabLoading.Text = "Loading Plot";
            this.tabLoading.UseVisualStyleBackColor = true;
            // 
            // _dataGridViewLoading
            // 
            this._dataGridViewLoading.AllowUserToAddRows = false;
            this._dataGridViewLoading.AllowUserToDeleteRows = false;
            this._dataGridViewLoading.AllowUserToResizeRows = false;
            this._dataGridViewLoading.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
            this._dataGridViewLoading.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this._dataGridViewLoading.ContextMenuStrip = this._contextMenuStrip;
            this._dataGridViewLoading.Dock = System.Windows.Forms.DockStyle.Fill;
            this._dataGridViewLoading.Location = new System.Drawing.Point(3, 3);
            this._dataGridViewLoading.MultiSelect = false;
            this._dataGridViewLoading.Name = "_dataGridViewLoading";
            this._dataGridViewLoading.ReadOnly = true;
            this._dataGridViewLoading.RowHeadersVisible = false;
            this._dataGridViewLoading.RowTemplate.Height = 21;
            this._dataGridViewLoading.Size = new System.Drawing.Size(571, 315);
            this._dataGridViewLoading.TabIndex = 1;
            this._dataGridViewLoading.CellValueNeeded += new System.Windows.Forms.DataGridViewCellValueEventHandler(this.DataGridViewLoading_CellValueNeeded);
            // 
            // tabSPlot
            // 
            this.tabSPlot.Controls.Add(this._dataGridViewSPlot);
            this.tabSPlot.Location = new System.Drawing.Point(4, 22);
            this.tabSPlot.Name = "tabSPlot";
            this.tabSPlot.Padding = new System.Windows.Forms.Padding(3);
            this.tabSPlot.Size = new System.Drawing.Size(577, 321);
            this.tabSPlot.TabIndex = 2;
            this.tabSPlot.Text = "S Plot";
            this.tabSPlot.UseVisualStyleBackColor = true;
            // 
            // _dataGridViewSPlot
            // 
            this._dataGridViewSPlot.AllowUserToAddRows = false;
            this._dataGridViewSPlot.AllowUserToDeleteRows = false;
            this._dataGridViewSPlot.AllowUserToResizeRows = false;
            this._dataGridViewSPlot.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
            this._dataGridViewSPlot.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this._dataGridViewSPlot.ContextMenuStrip = this._contextMenuStrip;
            this._dataGridViewSPlot.Dock = System.Windows.Forms.DockStyle.Fill;
            this._dataGridViewSPlot.Location = new System.Drawing.Point(3, 3);
            this._dataGridViewSPlot.MultiSelect = false;
            this._dataGridViewSPlot.Name = "_dataGridViewSPlot";
            this._dataGridViewSPlot.ReadOnly = true;
            this._dataGridViewSPlot.RowHeadersVisible = false;
            this._dataGridViewSPlot.RowTemplate.Height = 21;
            this._dataGridViewSPlot.Size = new System.Drawing.Size(571, 315);
            this._dataGridViewSPlot.TabIndex = 1;
            this._dataGridViewSPlot.CellValueNeeded += new System.Windows.Forms.DataGridViewCellValueEventHandler(this.DataGridViewSPlot_CellValueNeeded);
            // 
            // FormSimcaGrid
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(585, 384);
            this.Controls.Add(this._tabControlGrid);
            this.Controls.Add(this._buttonClose);
            this.Controls.Add(this._buttonSave);
            this.Controls.Add(this._buttonApply);
            this.KeyPreview = true;
            this.Name = "FormSimcaGrid";
            this.Text = "SIMCA Results";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.FormSimcaGrid_FormClosed);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.FormSimcaGrid_KeyDown);
            this._tabControlGrid.ResumeLayout(false);
            this.tabScore.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this._dataGridViewScore)).EndInit();
            this._contextMenuStrip.ResumeLayout(false);
            this.tabLoading.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this._dataGridViewLoading)).EndInit();
            this.tabSPlot.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this._dataGridViewSPlot)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button _buttonApply;
        private System.Windows.Forms.Button _buttonSave;
        private System.Windows.Forms.Button _buttonClose;
        private System.Windows.Forms.TabControl _tabControlGrid;
        private System.Windows.Forms.TabPage tabScore;
        private System.Windows.Forms.TabPage tabLoading;
        private System.Windows.Forms.TabPage tabSPlot;
        private System.Windows.Forms.DataGridView _dataGridViewScore;
        private System.Windows.Forms.DataGridView _dataGridViewLoading;
        private System.Windows.Forms.DataGridView _dataGridViewSPlot;
        private System.Windows.Forms.ContextMenuStrip _contextMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem _menuItemPaste;
    }
}