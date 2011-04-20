namespace Roket3D.ContentEditors
{
    partial class DesignerModelForm
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.PropertyEditor = new System.Windows.Forms.PropertyGrid();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.Control;
            this.panel1.Controls.Add(this.PropertyEditor);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Right;
            this.panel1.Location = new System.Drawing.Point(474, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(250, 504);
            this.panel1.TabIndex = 3;
            // 
            // PropertyEditor
            // 
            this.PropertyEditor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.PropertyEditor.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PropertyEditor.Location = new System.Drawing.Point(0, 0);
            this.PropertyEditor.Name = "PropertyEditor";
            this.PropertyEditor.PropertySort = System.Windows.Forms.PropertySort.Alphabetical;
            this.PropertyEditor.Size = new System.Drawing.Size(250, 504);
            this.PropertyEditor.TabIndex = 0;
            // 
            // DesignerModelForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ControlDark;
            this.ClientSize = new System.Drawing.Size(724, 504);
            this.Controls.Add(this.panel1);
            this.DockAreas = DockPanelSuite.DockAreas.Document;
            this.DoubleBuffered = true;
            this.Name = "DesignerModelForm";
            this.TabText = "Model Designer";
            this.Text = "Model Designer";
            this.Load += new System.EventHandler(this.DesignerModelForm_Load);
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.DesignerModelForm_MouseUp);
            this.Shown += new System.EventHandler(this.DesignerModelForm_Shown);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.DesignerModelForm_MouseDown);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.DesignerModelForm_FormClosing);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.DesignerModelForm_MouseMove);
            this.ResizeEnd += new System.EventHandler(this.DesignerModelForm_ResizeEnd);
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.PropertyGrid PropertyEditor;


    }
}