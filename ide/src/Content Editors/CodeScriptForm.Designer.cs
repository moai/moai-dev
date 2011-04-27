namespace Roket3D.ContentEditors
{
    partial class CodeScriptForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(CodeScriptForm));
            this.c_ToolStripContainer = new System.Windows.Forms.ToolStripContainer();
            this.c_InitalizationLabel = new System.Windows.Forms.Label();
            this.c_ToolStrip = new System.Windows.Forms.ToolStrip();
            this.c_SyntaxReportToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.c_SyntaxLimitTimer = new System.Windows.Forms.Timer(this.components);
            this.c_ToolStripContainer.ContentPanel.SuspendLayout();
            this.c_ToolStripContainer.TopToolStripPanel.SuspendLayout();
            this.c_ToolStripContainer.SuspendLayout();
            this.c_ToolStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // ToolStripContainer
            // 
            // 
            // ToolStripContainer.ContentPanel
            // 
            this.c_ToolStripContainer.ContentPanel.Controls.Add(this.c_InitalizationLabel);
            this.c_ToolStripContainer.ContentPanel.Size = new System.Drawing.Size(553, 534);
            this.c_ToolStripContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.c_ToolStripContainer.Location = new System.Drawing.Point(0, 0);
            this.c_ToolStripContainer.Name = "ToolStripContainer";
            this.c_ToolStripContainer.Size = new System.Drawing.Size(553, 534);
            this.c_ToolStripContainer.TabIndex = 1;
            this.c_ToolStripContainer.Text = "toolStripContainer1";
            // 
            // ToolStripContainer.TopToolStripPanel
            // 
            this.c_ToolStripContainer.TopToolStripPanel.Controls.Add(this.c_ToolStrip);
            // 
            // InitalizationLabel
            // 
            this.c_InitalizationLabel.BackColor = System.Drawing.Color.White;
            this.c_InitalizationLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.c_InitalizationLabel.Location = new System.Drawing.Point(0, 0);
            this.c_InitalizationLabel.Name = "InitalizationLabel";
            this.c_InitalizationLabel.Size = new System.Drawing.Size(553, 534);
            this.c_InitalizationLabel.TabIndex = 0;
            this.c_InitalizationLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // ToolStrip
            // 
            this.c_ToolStrip.Dock = System.Windows.Forms.DockStyle.None;
            this.c_ToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.c_SyntaxReportToolStripButton});
            this.c_ToolStrip.Location = new System.Drawing.Point(3, 0);
            this.c_ToolStrip.Name = "ToolStrip";
            this.c_ToolStrip.Size = new System.Drawing.Size(126, 25);
            this.c_ToolStrip.TabIndex = 0;
            this.c_ToolStrip.Visible = false;
            // 
            // SyntaxReportToolStripButton
            // 
            this.c_SyntaxReportToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.c_SyntaxReportToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("SyntaxReportToolStripButton.Image")));
            this.c_SyntaxReportToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.c_SyntaxReportToolStripButton.Name = "SyntaxReportToolStripButton";
            this.c_SyntaxReportToolStripButton.Size = new System.Drawing.Size(83, 22);
            this.c_SyntaxReportToolStripButton.Text = "Syntax Report";
            // 
            // SyntaxLimitTimer
            // 
            this.c_SyntaxLimitTimer.Interval = 500;
            this.c_SyntaxLimitTimer.Tick += new System.EventHandler(this.SyntaxLimitTimer_Tick);
            // 
            // CodeScriptForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(553, 534);
            this.Controls.Add(this.c_ToolStripContainer);
            this.DockAreas = DockPanelSuite.DockAreas.Document;
            this.Name = "CodeScriptForm";
            this.TabText = "CodeScriptForm";
            this.Text = "singlePlayerWorld.rkw [code]";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.CodeScriptForm_FormClosing);
            this.c_ToolStripContainer.ContentPanel.ResumeLayout(false);
            this.c_ToolStripContainer.TopToolStripPanel.ResumeLayout(false);
            this.c_ToolStripContainer.TopToolStripPanel.PerformLayout();
            this.c_ToolStripContainer.ResumeLayout(false);
            this.c_ToolStripContainer.PerformLayout();
            this.c_ToolStrip.ResumeLayout(false);
            this.c_ToolStrip.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ToolStripContainer c_ToolStripContainer;
        private System.Windows.Forms.ToolStrip c_ToolStrip;
        private System.Windows.Forms.ToolStripButton c_SyntaxReportToolStripButton;
        public ScintillaNet.Scintilla ScintillaEditor;
        private System.Windows.Forms.Timer c_SyntaxLimitTimer;
        private System.Windows.Forms.Label c_InitalizationLabel;
    }
}