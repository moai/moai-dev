namespace MOAI.Tools
{
    partial class ErrorListTool
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ErrorListTool));
            this.c_ErrorList = new System.Windows.Forms.ListView();
            this.c_ErrorIconColumn = new System.Windows.Forms.ColumnHeader();
            this.c_TextLineNumberColumn = new System.Windows.Forms.ColumnHeader();
            this.c_TextColumnColumn = new System.Windows.Forms.ColumnHeader();
            this.c_ErrorDescriptionColumn = new System.Windows.Forms.ColumnHeader();
            this.c_ErrorProjectColumn = new System.Windows.Forms.ColumnHeader();
            this.c_Images = new System.Windows.Forms.ImageList(this.components);
            this.SuspendLayout();
            // 
            // c_ErrorList
            // 
            this.c_ErrorList.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.c_ErrorList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.c_ErrorIconColumn,
            this.c_TextLineNumberColumn,
            this.c_TextColumnColumn,
            this.c_ErrorDescriptionColumn,
            this.c_ErrorProjectColumn});
            this.c_ErrorList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.c_ErrorList.FullRowSelect = true;
            this.c_ErrorList.Location = new System.Drawing.Point(0, 0);
            this.c_ErrorList.MultiSelect = false;
            this.c_ErrorList.Name = "c_ErrorList";
            this.c_ErrorList.Size = new System.Drawing.Size(802, 210);
            this.c_ErrorList.SmallImageList = this.c_Images;
            this.c_ErrorList.Sorting = System.Windows.Forms.SortOrder.Descending;
            this.c_ErrorList.TabIndex = 0;
            this.c_ErrorList.UseCompatibleStateImageBehavior = false;
            this.c_ErrorList.View = System.Windows.Forms.View.Details;
            // 
            // c_ErrorIconColumn
            // 
            this.c_ErrorIconColumn.Text = "";
            this.c_ErrorIconColumn.Width = 24;
            // 
            // c_TextLineNumberColumn
            // 
            this.c_TextLineNumberColumn.Text = "Line";
            this.c_TextLineNumberColumn.Width = 80;
            // 
            // c_TextColumnColumn
            // 
            this.c_TextColumnColumn.Text = "Column";
            this.c_TextColumnColumn.Width = 80;
            // 
            // c_ErrorDescriptionColumn
            // 
            this.c_ErrorDescriptionColumn.Text = "Description";
            this.c_ErrorDescriptionColumn.Width = 360;
            // 
            // c_ErrorProjectColumn
            // 
            this.c_ErrorProjectColumn.Text = "Project";
            this.c_ErrorProjectColumn.Width = 120;
            // 
            // c_Images
            // 
            this.c_Images.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("c_Images.ImageStream")));
            this.c_Images.TransparentColor = System.Drawing.Color.Transparent;
            this.c_Images.Images.SetKeyName(0, "Error");
            this.c_Images.Images.SetKeyName(1, "Warning");
            this.c_Images.Images.SetKeyName(2, "Info");
            // 
            // ToolErrorList
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(802, 210);
            this.Controls.Add(this.c_ErrorList);
            this.DockAreas = ((DockPanelSuite.DockAreas)(((((DockPanelSuite.DockAreas.Float | DockPanelSuite.DockAreas.DockLeft)
                        | DockPanelSuite.DockAreas.DockRight)
                        | DockPanelSuite.DockAreas.DockTop)
                        | DockPanelSuite.DockAreas.DockBottom)));
            this.DoubleBuffered = true;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "ToolErrorList";
            this.TabText = "Error List";
            this.Text = "Error List";
            this.Resize += new System.EventHandler(this.ToolErrorList_Resize);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ColumnHeader c_ErrorIconColumn;
        private System.Windows.Forms.ColumnHeader c_ErrorDescriptionColumn;
        private System.Windows.Forms.ColumnHeader c_TextLineNumberColumn;
        private System.Windows.Forms.ColumnHeader c_TextColumnColumn;
        private System.Windows.Forms.ColumnHeader c_ErrorProjectColumn;
        private System.Windows.Forms.ImageList c_Images;
        public System.Windows.Forms.ListView c_ErrorList;

    }
}