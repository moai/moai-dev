namespace MOAI.Tools
{
    partial class SolutionExplorerTool
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SolutionExplorerTool));
            this.c_SolutionTree = new System.Windows.Forms.TreeView();
            this.SuspendLayout();
            // 
            // c_SolutionTree
            // 
            this.c_SolutionTree.Dock = System.Windows.Forms.DockStyle.Fill;
            this.c_SolutionTree.Location = new System.Drawing.Point(0, 0);
            this.c_SolutionTree.Name = "c_SolutionTree";
            this.c_SolutionTree.Size = new System.Drawing.Size(246, 475);
            this.c_SolutionTree.TabIndex = 0;
            this.c_SolutionTree.DoubleClick += new System.EventHandler(this.c_SolutionTree_DoubleClick);
            this.c_SolutionTree.MouseUp += new System.Windows.Forms.MouseEventHandler(this.c_SolutionTree_MouseUp);
            // 
            // SolutionExplorerTool
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(246, 475);
            this.Controls.Add(this.c_SolutionTree);
            this.DockAreas = ((DockPanelSuite.DockAreas)(((((DockPanelSuite.DockAreas.Float | DockPanelSuite.DockAreas.DockLeft)
                        | DockPanelSuite.DockAreas.DockRight)
                        | DockPanelSuite.DockAreas.DockTop)
                        | DockPanelSuite.DockAreas.DockBottom)));
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "SolutionExplorerTool";
            this.TabText = "Solution Explorer";
            this.Text = "Solution Explorer";
            this.Load += new System.EventHandler(this.ToolSolutionExplorer_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TreeView c_SolutionTree;
    }
}