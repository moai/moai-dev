namespace MOAI
{
    partial class NewSolutionForm
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
            System.Windows.Forms.ListViewGroup listViewGroup1 = new System.Windows.Forms.ListViewGroup("MOAI Templates", System.Windows.Forms.HorizontalAlignment.Left);
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(NewSolutionForm));
            this.c_ProjectTypeListView = new System.Windows.Forms.ListView();
            this.c_ImageList = new System.Windows.Forms.ImageList(this.components);
            this.c_TemplateDescriptionTextBox = new System.Windows.Forms.TextBox();
            this.c_NameLabel = new System.Windows.Forms.Label();
            this.c_ProjectNameTextBox = new System.Windows.Forms.TextBox();
            this.c_FlowLayoutPanel = new System.Windows.Forms.FlowLayoutPanel();
            this.c_CancelButton = new System.Windows.Forms.Button();
            this.c_OKButton = new System.Windows.Forms.Button();
            this.c_GroupBox = new System.Windows.Forms.GroupBox();
            this.c_SolutionLocationTextBox = new System.Windows.Forms.TextBox();
            this.c_FolderLabel = new System.Windows.Forms.Label();
            this.c_SolutionFolderTextBox = new System.Windows.Forms.TextBox();
            this.c_UseSolutionFolderCheckBox = new System.Windows.Forms.CheckBox();
            this.c_SolutionLocationLabel = new System.Windows.Forms.Label();
            this.c_SolutionFolderInvalidPictureBox = new System.Windows.Forms.PictureBox();
            this.c_SolutionLocationInvalidPictureBox = new System.Windows.Forms.PictureBox();
            this.c_ProjectNameInvalidPictureBox = new System.Windows.Forms.PictureBox();
            this.c_BrowseSolutionLocationButton = new System.Windows.Forms.Button();
            this.c_ProjectTypeInvalidPictureBox = new System.Windows.Forms.PictureBox();
            this.c_ProjectCreationLabel = new System.Windows.Forms.Label();
            this.c_FlowLayoutPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.c_SolutionFolderInvalidPictureBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.c_SolutionLocationInvalidPictureBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.c_ProjectNameInvalidPictureBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.c_ProjectTypeInvalidPictureBox)).BeginInit();
            this.SuspendLayout();
            // 
            // c_ProjectTypeListView
            // 
            this.c_ProjectTypeListView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            listViewGroup1.Header = "MOAI Templates";
            listViewGroup1.Name = "lvgTemplates";
            this.c_ProjectTypeListView.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] {
            listViewGroup1});
            this.c_ProjectTypeListView.HideSelection = false;
            this.c_ProjectTypeListView.LargeImageList = this.c_ImageList;
            this.c_ProjectTypeListView.Location = new System.Drawing.Point(12, 12);
            this.c_ProjectTypeListView.MultiSelect = false;
            this.c_ProjectTypeListView.Name = "c_ProjectTypeListView";
            this.c_ProjectTypeListView.Size = new System.Drawing.Size(592, 138);
            this.c_ProjectTypeListView.TabIndex = 0;
            this.c_ProjectTypeListView.UseCompatibleStateImageBehavior = false;
            this.c_ProjectTypeListView.SelectedIndexChanged += new System.EventHandler(this.c_ProjectTypeListView_SelectedIndexChanged);
            // 
            // c_ImageList
            // 
            this.c_ImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("c_ImageList.ImageStream")));
            this.c_ImageList.TransparentColor = System.Drawing.Color.Transparent;
            this.c_ImageList.Images.SetKeyName(0, "LuaManaged");
            this.c_ImageList.Images.SetKeyName(1, "LuaUnmanaged");
            // 
            // c_TemplateDescriptionTextBox
            // 
            this.c_TemplateDescriptionTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.c_TemplateDescriptionTextBox.Location = new System.Drawing.Point(13, 156);
            this.c_TemplateDescriptionTextBox.Name = "c_TemplateDescriptionTextBox";
            this.c_TemplateDescriptionTextBox.ReadOnly = true;
            this.c_TemplateDescriptionTextBox.Size = new System.Drawing.Size(591, 20);
            this.c_TemplateDescriptionTextBox.TabIndex = 1;
            // 
            // c_NameLabel
            // 
            this.c_NameLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.c_NameLabel.AutoSize = true;
            this.c_NameLabel.Location = new System.Drawing.Point(12, 186);
            this.c_NameLabel.Name = "c_NameLabel";
            this.c_NameLabel.Size = new System.Drawing.Size(38, 13);
            this.c_NameLabel.TabIndex = 2;
            this.c_NameLabel.Text = "Name:";
            // 
            // c_ProjectNameTextBox
            // 
            this.c_ProjectNameTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.c_ProjectNameTextBox.Location = new System.Drawing.Point(154, 183);
            this.c_ProjectNameTextBox.Name = "c_ProjectNameTextBox";
            this.c_ProjectNameTextBox.Size = new System.Drawing.Size(353, 20);
            this.c_ProjectNameTextBox.TabIndex = 3;
            this.c_ProjectNameTextBox.Text = "ManagedGame";
            this.c_ProjectNameTextBox.TextChanged += new System.EventHandler(this.c_ProjectNameTextBox_TextChanged);
            // 
            // c_FlowLayoutPanel
            // 
            this.c_FlowLayoutPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.c_FlowLayoutPanel.Controls.Add(this.c_CancelButton);
            this.c_FlowLayoutPanel.Controls.Add(this.c_OKButton);
            this.c_FlowLayoutPanel.FlowDirection = System.Windows.Forms.FlowDirection.RightToLeft;
            this.c_FlowLayoutPanel.Location = new System.Drawing.Point(439, 281);
            this.c_FlowLayoutPanel.Name = "c_FlowLayoutPanel";
            this.c_FlowLayoutPanel.Size = new System.Drawing.Size(168, 29);
            this.c_FlowLayoutPanel.TabIndex = 4;
            // 
            // c_CancelButton
            // 
            this.c_CancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.c_CancelButton.Location = new System.Drawing.Point(90, 3);
            this.c_CancelButton.Name = "c_CancelButton";
            this.c_CancelButton.Size = new System.Drawing.Size(75, 23);
            this.c_CancelButton.TabIndex = 1;
            this.c_CancelButton.Text = "Cancel";
            this.c_CancelButton.UseVisualStyleBackColor = true;
            this.c_CancelButton.Click += new System.EventHandler(this.c_CancelButton_Click);
            // 
            // c_OKButton
            // 
            this.c_OKButton.Location = new System.Drawing.Point(9, 3);
            this.c_OKButton.Name = "c_OKButton";
            this.c_OKButton.Size = new System.Drawing.Size(75, 23);
            this.c_OKButton.TabIndex = 0;
            this.c_OKButton.Text = "OK";
            this.c_OKButton.UseVisualStyleBackColor = true;
            this.c_OKButton.Click += new System.EventHandler(this.c_OKButton_Click);
            // 
            // c_GroupBox
            // 
            this.c_GroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.c_GroupBox.Location = new System.Drawing.Point(15, 259);
            this.c_GroupBox.Name = "c_GroupBox";
            this.c_GroupBox.Size = new System.Drawing.Size(589, 7);
            this.c_GroupBox.TabIndex = 5;
            this.c_GroupBox.TabStop = false;
            // 
            // c_SolutionLocationTextBox
            // 
            this.c_SolutionLocationTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.c_SolutionLocationTextBox.Location = new System.Drawing.Point(154, 209);
            this.c_SolutionLocationTextBox.Name = "c_SolutionLocationTextBox";
            this.c_SolutionLocationTextBox.Size = new System.Drawing.Size(353, 20);
            this.c_SolutionLocationTextBox.TabIndex = 7;
            this.c_SolutionLocationTextBox.Text = "Z:\\My Documents\\MOAI Projects\\";
            this.c_SolutionLocationTextBox.TextChanged += new System.EventHandler(this.c_SolutionLocationTextBox_TextChanged);
            // 
            // c_FolderLabel
            // 
            this.c_FolderLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.c_FolderLabel.AutoSize = true;
            this.c_FolderLabel.Location = new System.Drawing.Point(12, 212);
            this.c_FolderLabel.Name = "c_FolderLabel";
            this.c_FolderLabel.Size = new System.Drawing.Size(87, 13);
            this.c_FolderLabel.TabIndex = 6;
            this.c_FolderLabel.Text = "Project Location:";
            // 
            // c_SolutionFolderTextBox
            // 
            this.c_SolutionFolderTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.c_SolutionFolderTextBox.Enabled = false;
            this.c_SolutionFolderTextBox.Location = new System.Drawing.Point(154, 235);
            this.c_SolutionFolderTextBox.Name = "c_SolutionFolderTextBox";
            this.c_SolutionFolderTextBox.ReadOnly = true;
            this.c_SolutionFolderTextBox.Size = new System.Drawing.Size(353, 20);
            this.c_SolutionFolderTextBox.TabIndex = 9;
            this.c_SolutionFolderTextBox.Text = "ManagedSolution";
            this.c_SolutionFolderTextBox.TextChanged += new System.EventHandler(this.c_SolutionFolderTextBox_TextChanged);
            // 
            // c_UseSolutionFolderCheckBox
            // 
            this.c_UseSolutionFolderCheckBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.c_UseSolutionFolderCheckBox.AutoSize = true;
            this.c_UseSolutionFolderCheckBox.CheckAlign = System.Drawing.ContentAlignment.TopLeft;
            this.c_UseSolutionFolderCheckBox.Location = new System.Drawing.Point(15, 236);
            this.c_UseSolutionFolderCheckBox.Name = "c_UseSolutionFolderCheckBox";
            this.c_UseSolutionFolderCheckBox.Size = new System.Drawing.Size(121, 17);
            this.c_UseSolutionFolderCheckBox.TabIndex = 10;
            this.c_UseSolutionFolderCheckBox.Text = "Use Solution Folder:";
            this.c_UseSolutionFolderCheckBox.UseVisualStyleBackColor = true;
            this.c_UseSolutionFolderCheckBox.CheckedChanged += new System.EventHandler(this.c_UseSolutionFolderCheckBox_CheckedChanged);
            // 
            // c_SolutionLocationLabel
            // 
            this.c_SolutionLocationLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.c_SolutionLocationLabel.AutoEllipsis = true;
            this.c_SolutionLocationLabel.Location = new System.Drawing.Point(37, 281);
            this.c_SolutionLocationLabel.Name = "c_SolutionLocationLabel";
            this.c_SolutionLocationLabel.Size = new System.Drawing.Size(400, 41);
            this.c_SolutionLocationLabel.TabIndex = 11;
            this.c_SolutionLocationLabel.Text = "Z:\\My Documents\\MOAI Projects\\ManagedGame";
            this.c_SolutionLocationLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // c_SolutionFolderInvalidPictureBox
            // 
            this.c_SolutionFolderInvalidPictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.c_SolutionFolderInvalidPictureBox.Image = global::MOAI.Properties.Resources.icon_invalid;
            this.c_SolutionFolderInvalidPictureBox.Location = new System.Drawing.Point(513, 235);
            this.c_SolutionFolderInvalidPictureBox.Name = "c_SolutionFolderInvalidPictureBox";
            this.c_SolutionFolderInvalidPictureBox.Size = new System.Drawing.Size(20, 20);
            this.c_SolutionFolderInvalidPictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.c_SolutionFolderInvalidPictureBox.TabIndex = 14;
            this.c_SolutionFolderInvalidPictureBox.TabStop = false;
            this.c_SolutionFolderInvalidPictureBox.Visible = false;
            // 
            // c_SolutionLocationInvalidPictureBox
            // 
            this.c_SolutionLocationInvalidPictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.c_SolutionLocationInvalidPictureBox.Image = global::MOAI.Properties.Resources.icon_invalid;
            this.c_SolutionLocationInvalidPictureBox.Location = new System.Drawing.Point(545, 209);
            this.c_SolutionLocationInvalidPictureBox.Name = "c_SolutionLocationInvalidPictureBox";
            this.c_SolutionLocationInvalidPictureBox.Size = new System.Drawing.Size(20, 20);
            this.c_SolutionLocationInvalidPictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.c_SolutionLocationInvalidPictureBox.TabIndex = 13;
            this.c_SolutionLocationInvalidPictureBox.TabStop = false;
            this.c_SolutionLocationInvalidPictureBox.Visible = false;
            // 
            // c_ProjectNameInvalidPictureBox
            // 
            this.c_ProjectNameInvalidPictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.c_ProjectNameInvalidPictureBox.Image = global::MOAI.Properties.Resources.icon_invalid;
            this.c_ProjectNameInvalidPictureBox.Location = new System.Drawing.Point(513, 183);
            this.c_ProjectNameInvalidPictureBox.Name = "c_ProjectNameInvalidPictureBox";
            this.c_ProjectNameInvalidPictureBox.Size = new System.Drawing.Size(20, 20);
            this.c_ProjectNameInvalidPictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.c_ProjectNameInvalidPictureBox.TabIndex = 12;
            this.c_ProjectNameInvalidPictureBox.TabStop = false;
            this.c_ProjectNameInvalidPictureBox.Visible = false;
            // 
            // c_BrowseSolutionLocationButton
            // 
            this.c_BrowseSolutionLocationButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.c_BrowseSolutionLocationButton.Location = new System.Drawing.Point(513, 209);
            this.c_BrowseSolutionLocationButton.Name = "c_BrowseSolutionLocationButton";
            this.c_BrowseSolutionLocationButton.Size = new System.Drawing.Size(26, 20);
            this.c_BrowseSolutionLocationButton.TabIndex = 15;
            this.c_BrowseSolutionLocationButton.Text = "...";
            this.c_BrowseSolutionLocationButton.UseVisualStyleBackColor = true;
            this.c_BrowseSolutionLocationButton.Click += new System.EventHandler(this.c_BrowseSolutionLocationButton_Click);
            // 
            // c_ProjectTypeInvalidPictureBox
            // 
            this.c_ProjectTypeInvalidPictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.c_ProjectTypeInvalidPictureBox.BackColor = System.Drawing.Color.White;
            this.c_ProjectTypeInvalidPictureBox.Image = global::MOAI.Properties.Resources.icon_invalid;
            this.c_ProjectTypeInvalidPictureBox.Location = new System.Drawing.Point(581, 16);
            this.c_ProjectTypeInvalidPictureBox.Name = "c_ProjectTypeInvalidPictureBox";
            this.c_ProjectTypeInvalidPictureBox.Size = new System.Drawing.Size(20, 20);
            this.c_ProjectTypeInvalidPictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.c_ProjectTypeInvalidPictureBox.TabIndex = 16;
            this.c_ProjectTypeInvalidPictureBox.TabStop = false;
            this.c_ProjectTypeInvalidPictureBox.Visible = false;
            // 
            // c_ProjectCreationLabel
            // 
            this.c_ProjectCreationLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.c_ProjectCreationLabel.AutoEllipsis = true;
            this.c_ProjectCreationLabel.Location = new System.Drawing.Point(16, 269);
            this.c_ProjectCreationLabel.Name = "c_ProjectCreationLabel";
            this.c_ProjectCreationLabel.Size = new System.Drawing.Size(417, 14);
            this.c_ProjectCreationLabel.TabIndex = 17;
            this.c_ProjectCreationLabel.Text = "Project will be created at:";
            // 
            // NewSolutionForm
            // 
            this.AcceptButton = this.c_OKButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.c_CancelButton;
            this.ClientSize = new System.Drawing.Size(616, 322);
            this.Controls.Add(this.c_ProjectCreationLabel);
            this.Controls.Add(this.c_ProjectTypeInvalidPictureBox);
            this.Controls.Add(this.c_BrowseSolutionLocationButton);
            this.Controls.Add(this.c_SolutionFolderInvalidPictureBox);
            this.Controls.Add(this.c_SolutionLocationInvalidPictureBox);
            this.Controls.Add(this.c_ProjectNameInvalidPictureBox);
            this.Controls.Add(this.c_SolutionLocationLabel);
            this.Controls.Add(this.c_UseSolutionFolderCheckBox);
            this.Controls.Add(this.c_SolutionFolderTextBox);
            this.Controls.Add(this.c_SolutionLocationTextBox);
            this.Controls.Add(this.c_FolderLabel);
            this.Controls.Add(this.c_GroupBox);
            this.Controls.Add(this.c_FlowLayoutPanel);
            this.Controls.Add(this.c_ProjectNameTextBox);
            this.Controls.Add(this.c_NameLabel);
            this.Controls.Add(this.c_TemplateDescriptionTextBox);
            this.Controls.Add(this.c_ProjectTypeListView);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(624, 349);
            this.Name = "NewSolutionForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "New Solution";
            this.Load += new System.EventHandler(this.NewSolutionForm_Load);
            this.c_FlowLayoutPanel.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.c_SolutionFolderInvalidPictureBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.c_SolutionLocationInvalidPictureBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.c_ProjectNameInvalidPictureBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.c_ProjectTypeInvalidPictureBox)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ImageList c_ImageList;
        private System.Windows.Forms.TextBox c_TemplateDescriptionTextBox;
        private System.Windows.Forms.Label c_NameLabel;
        private System.Windows.Forms.FlowLayoutPanel c_FlowLayoutPanel;
        private System.Windows.Forms.Button c_OKButton;
        private System.Windows.Forms.Button c_CancelButton;
        private System.Windows.Forms.GroupBox c_GroupBox;
        private System.Windows.Forms.Label c_FolderLabel;
        private System.Windows.Forms.PictureBox c_ProjectNameInvalidPictureBox;
        private System.Windows.Forms.PictureBox c_SolutionLocationInvalidPictureBox;
        private System.Windows.Forms.PictureBox c_SolutionFolderInvalidPictureBox;
        private System.Windows.Forms.Button c_BrowseSolutionLocationButton;
        private System.Windows.Forms.PictureBox c_ProjectTypeInvalidPictureBox;
        private System.Windows.Forms.Label c_ProjectCreationLabel;
        private System.Windows.Forms.TextBox c_ProjectNameTextBox;
        private System.Windows.Forms.ListView c_ProjectTypeListView;
        private System.Windows.Forms.TextBox c_SolutionLocationTextBox;
        private System.Windows.Forms.TextBox c_SolutionFolderTextBox;
        private System.Windows.Forms.CheckBox c_UseSolutionFolderCheckBox;
        private System.Windows.Forms.Label c_SolutionLocationLabel;
    }
}