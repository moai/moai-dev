namespace MOAI
{
    partial class IDE
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(IDE));
            this.c_StatusStrip = new System.Windows.Forms.StatusStrip();
            this.c_ToolStripStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.c_ToolStripContainer = new System.Windows.Forms.ToolStripContainer();
            this.c_DockWorkspace = new DockPanelSuite.DockPanel();
            this.c_StatusStrip.SuspendLayout();
            this.c_ToolStripContainer.ContentPanel.SuspendLayout();
            this.c_ToolStripContainer.SuspendLayout();
            this.SuspendLayout();
            // 
            // c_StatusStrip
            // 
            this.c_StatusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.c_ToolStripStatusLabel});
            this.c_StatusStrip.Location = new System.Drawing.Point(0, 628);
            this.c_StatusStrip.Name = "c_StatusStrip";
            this.c_StatusStrip.Size = new System.Drawing.Size(852, 22);
            this.c_StatusStrip.TabIndex = 2;
            this.c_StatusStrip.Text = "statusStrip1";
            // 
            // c_ToolStripStatusLabel
            // 
            this.c_ToolStripStatusLabel.Name = "c_ToolStripStatusLabel";
            this.c_ToolStripStatusLabel.Size = new System.Drawing.Size(837, 17);
            this.c_ToolStripStatusLabel.Spring = true;
            this.c_ToolStripStatusLabel.Text = "Ready";
            this.c_ToolStripStatusLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // c_ToolStripContainer
            // 
            // 
            // c_ToolStripContainer.ContentPanel
            // 
            this.c_ToolStripContainer.ContentPanel.Controls.Add(this.c_DockWorkspace);
            this.c_ToolStripContainer.ContentPanel.Size = new System.Drawing.Size(852, 603);
            this.c_ToolStripContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.c_ToolStripContainer.Location = new System.Drawing.Point(0, 0);
            this.c_ToolStripContainer.Name = "c_ToolStripContainer";
            this.c_ToolStripContainer.Size = new System.Drawing.Size(852, 628);
            this.c_ToolStripContainer.TabIndex = 8;
            this.c_ToolStripContainer.Text = "toolStripContainer1";
            // 
            // c_DockWorkspace
            // 
            this.c_DockWorkspace.ActiveAutoHideContent = null;
            this.c_DockWorkspace.ActiveTab = null;
            this.c_DockWorkspace.Dock = System.Windows.Forms.DockStyle.Fill;
            this.c_DockWorkspace.DockBottomPortion = 200;
            this.c_DockWorkspace.DockLeftPortion = 250;
            this.c_DockWorkspace.DockRightPortion = 200;
            this.c_DockWorkspace.DockTopPortion = 150;
            this.c_DockWorkspace.DocumentStyle = DockPanelSuite.DocumentStyle.DockingWindow;
            this.c_DockWorkspace.Location = new System.Drawing.Point(0, 0);
            this.c_DockWorkspace.Name = "c_DockWorkspace";
            this.c_DockWorkspace.Size = new System.Drawing.Size(852, 603);
            this.c_DockWorkspace.TabIndex = 0;
            this.c_DockWorkspace.ActiveTabChanged += new System.EventHandler(this.c_DockWorkspace_ActiveTabChanged);
            // 
            // IDE
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(852, 650);
            this.Controls.Add(this.c_ToolStripContainer);
            this.Controls.Add(this.c_StatusStrip);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.IsMdiContainer = true;
            this.Name = "IDE";
            this.Text = "No Solution Loaded - MOAI";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.StyleChanged += new System.EventHandler(this.IDE_StyleChanged);
            this.Load += new System.EventHandler(this.IDE_Load);
            this.c_StatusStrip.ResumeLayout(false);
            this.c_StatusStrip.PerformLayout();
            this.c_ToolStripContainer.ContentPanel.ResumeLayout(false);
            this.c_ToolStripContainer.ResumeLayout(false);
            this.c_ToolStripContainer.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.StatusStrip c_StatusStrip;
        private System.Windows.Forms.ToolStripStatusLabel c_ToolStripStatusLabel;
        private System.Windows.Forms.ToolStripContainer c_ToolStripContainer;
        public DockPanelSuite.DockPanel c_DockWorkspace;
    }
}

