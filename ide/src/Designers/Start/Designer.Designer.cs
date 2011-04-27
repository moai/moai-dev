namespace MOAI.Designers.Start
{
    partial class Designer
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
            this.c_WebBrowser = new System.Windows.Forms.WebBrowser();
            this.SuspendLayout();
            // 
            // c_WebBrowser
            // 
            this.c_WebBrowser.Dock = System.Windows.Forms.DockStyle.Fill;
            this.c_WebBrowser.Location = new System.Drawing.Point(0, 0);
            this.c_WebBrowser.MinimumSize = new System.Drawing.Size(20, 20);
            this.c_WebBrowser.Name = "c_WebBrowser";
            this.c_WebBrowser.Size = new System.Drawing.Size(292, 273);
            this.c_WebBrowser.TabIndex = 0;
            this.c_WebBrowser.Navigating += new System.Windows.Forms.WebBrowserNavigatingEventHandler(this.c_WebBrowser_Navigating);
            // 
            // Designer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 273);
            this.Controls.Add(this.c_WebBrowser);
            this.Name = "Designer";
            this.TabText = "Designer";
            this.Text = "Designer";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.WebBrowser c_WebBrowser;
    }
}