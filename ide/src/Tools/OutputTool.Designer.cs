namespace MOAI.Tools
{
    partial class OutputTool
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(OutputTool));
            this.c_BuildOutputTextBox = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // c_BuildOutputTextBox
            // 
            this.c_BuildOutputTextBox.BackColor = System.Drawing.Color.White;
            this.c_BuildOutputTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.c_BuildOutputTextBox.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_BuildOutputTextBox.Location = new System.Drawing.Point(0, 0);
            this.c_BuildOutputTextBox.Multiline = true;
            this.c_BuildOutputTextBox.Name = "c_BuildOutputTextBox";
            this.c_BuildOutputTextBox.ReadOnly = true;
            this.c_BuildOutputTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.c_BuildOutputTextBox.Size = new System.Drawing.Size(802, 210);
            this.c_BuildOutputTextBox.TabIndex = 0;
            // 
            // OutputTool
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(802, 210);
            this.Controls.Add(this.c_BuildOutputTextBox);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "OutputTool";
            this.TabText = "Output";
            this.Text = "Output";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox c_BuildOutputTextBox;
    }
}