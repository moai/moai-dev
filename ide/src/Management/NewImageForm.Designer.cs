namespace MOAI.ManagementOld
{
    partial class NewImageForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(NewImageForm));
            this.label1 = new System.Windows.Forms.Label();
            this.fileFormatComboBox = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.widthTextBox = new System.Windows.Forms.TextBox();
            this.heightTextBox = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.sizeInconsistentWarningPictureBox = new System.Windows.Forms.PictureBox();
            this.heightInvalidPictureBox = new System.Windows.Forms.PictureBox();
            this.widthInvalidPictureBox = new System.Windows.Forms.PictureBox();
            this.sizeInconsistentWarningLabel = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.sizeInconsistentWarningPictureBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.heightInvalidPictureBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.widthInvalidPictureBox)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(61, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "File Format:";
            // 
            // fileFormatComboBox
            // 
            this.fileFormatComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.fileFormatComboBox.FormattingEnabled = true;
            this.fileFormatComboBox.Items.AddRange(new object[] {
            "PNG",
            "BMP",
            "JPG",
            "TGA",
            "PCX",
            "PSD"});
            this.fileFormatComboBox.Location = new System.Drawing.Point(126, 12);
            this.fileFormatComboBox.Name = "fileFormatComboBox";
            this.fileFormatComboBox.Size = new System.Drawing.Size(146, 21);
            this.fileFormatComboBox.TabIndex = 1;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 42);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(73, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Width (pixels):";
            // 
            // widthTextBox
            // 
            this.widthTextBox.Location = new System.Drawing.Point(152, 39);
            this.widthTextBox.Name = "widthTextBox";
            this.widthTextBox.Size = new System.Drawing.Size(120, 20);
            this.widthTextBox.TabIndex = 4;
            this.widthTextBox.Text = "256";
            this.widthTextBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // heightTextBox
            // 
            this.heightTextBox.Location = new System.Drawing.Point(152, 65);
            this.heightTextBox.Name = "heightTextBox";
            this.heightTextBox.Size = new System.Drawing.Size(120, 20);
            this.heightTextBox.TabIndex = 15;
            this.heightTextBox.Text = "256";
            this.heightTextBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 68);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(76, 13);
            this.label3.TabIndex = 14;
            this.label3.Text = "Height (pixels):";
            // 
            // sizeInconsistentWarningPictureBox
            // 
            this.sizeInconsistentWarningPictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.sizeInconsistentWarningPictureBox.Image = global::MOAI.Properties.Resources.error;
            this.sizeInconsistentWarningPictureBox.Location = new System.Drawing.Point(15, 90);
            this.sizeInconsistentWarningPictureBox.Name = "sizeInconsistentWarningPictureBox";
            this.sizeInconsistentWarningPictureBox.Size = new System.Drawing.Size(20, 20);
            this.sizeInconsistentWarningPictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.sizeInconsistentWarningPictureBox.TabIndex = 17;
            this.sizeInconsistentWarningPictureBox.TabStop = false;
            this.sizeInconsistentWarningPictureBox.Visible = false;
            // 
            // heightInvalidPictureBox
            // 
            this.heightInvalidPictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.heightInvalidPictureBox.Image = global::MOAI.Properties.Resources.icon_invalid;
            this.heightInvalidPictureBox.Location = new System.Drawing.Point(126, 65);
            this.heightInvalidPictureBox.Name = "heightInvalidPictureBox";
            this.heightInvalidPictureBox.Size = new System.Drawing.Size(20, 20);
            this.heightInvalidPictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.heightInvalidPictureBox.TabIndex = 16;
            this.heightInvalidPictureBox.TabStop = false;
            this.heightInvalidPictureBox.Visible = false;
            // 
            // widthInvalidPictureBox
            // 
            this.widthInvalidPictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.widthInvalidPictureBox.Image = global::MOAI.Properties.Resources.icon_invalid;
            this.widthInvalidPictureBox.Location = new System.Drawing.Point(126, 39);
            this.widthInvalidPictureBox.Name = "widthInvalidPictureBox";
            this.widthInvalidPictureBox.Size = new System.Drawing.Size(20, 20);
            this.widthInvalidPictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.widthInvalidPictureBox.TabIndex = 13;
            this.widthInvalidPictureBox.TabStop = false;
            this.widthInvalidPictureBox.Visible = false;
            // 
            // sizeInconsistentWarningLabel
            // 
            this.sizeInconsistentWarningLabel.Location = new System.Drawing.Point(39, 91);
            this.sizeInconsistentWarningLabel.Name = "sizeInconsistentWarningLabel";
            this.sizeInconsistentWarningLabel.Size = new System.Drawing.Size(233, 57);
            this.sizeInconsistentWarningLabel.TabIndex = 18;
            this.sizeInconsistentWarningLabel.Text = "Warning!  One or more of the dimensions you\r\nhave set for this image are not powe" +
                "rs of 2.\r\nThis may cause issues when using the image as\r\na texture for use on mo" +
                "dels.";
            // 
            // NewImageForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 305);
            this.Controls.Add(this.sizeInconsistentWarningLabel);
            this.Controls.Add(this.sizeInconsistentWarningPictureBox);
            this.Controls.Add(this.heightInvalidPictureBox);
            this.Controls.Add(this.heightTextBox);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.widthInvalidPictureBox);
            this.Controls.Add(this.widthTextBox);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.fileFormatComboBox);
            this.Controls.Add(this.label1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "NewImageForm";
            this.Text = "New Image";
            ((System.ComponentModel.ISupportInitialize)(this.sizeInconsistentWarningPictureBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.heightInvalidPictureBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.widthInvalidPictureBox)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox fileFormatComboBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox widthTextBox;
        private System.Windows.Forms.PictureBox widthInvalidPictureBox;
        private System.Windows.Forms.PictureBox heightInvalidPictureBox;
        private System.Windows.Forms.TextBox heightTextBox;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.PictureBox sizeInconsistentWarningPictureBox;
        private System.Windows.Forms.Label sizeInconsistentWarningLabel;

    }
}