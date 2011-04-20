namespace Roket3D.ContentEditors
{
    partial class ControlColorSelectorDropDown
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.SuspendLayout();
            // 
            // ControlColorSelectorDropDown
            // 
            this.AutoSize = false;
            this.Size = new System.Drawing.Size(384, 256);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.ControlColorSelectorDropDown_Paint);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.ControlColorSelectorDropDown_MouseMove);
            this.Click += new System.EventHandler(this.ControlColorSelectorDropDown_Click);
            this.ResumeLayout(false);

        }

        #endregion
    }
}
