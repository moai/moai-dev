namespace Roket3D.ContentEditors
{
    partial class DesignerImageForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DesignerImageForm));
            this.imageEditor = new Roket3D.ContentEditors.ControlImageEditor();
            this.toolStripContainer1 = new System.Windows.Forms.ToolStripContainer();
            this.colorAndBrushToolStrip = new System.Windows.Forms.ToolStrip();
            this.frontBrushControlColorSelectorDropDown = new Roket3D.ContentEditors.ControlColorSelectorDropDown();
            this.backBrushControlColorSelectorDropDown = new Roket3D.ContentEditors.ControlColorSelectorDropDown();
            this.zoomLevelToolStripComboBox = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.brushSizeToolStripTextBox = new System.Windows.Forms.ToolStripTextBox();
            this.textToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.newImageToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.importImageToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.frontBrushControlColorSelector = new Roket3D.ContentEditors.ControlColorSelector();
            this.backBrushControlColorSelector = new Roket3D.ContentEditors.ControlColorSelector();
            this.blendModeToolStripSplitButton = new System.Windows.Forms.ToolStripSplitButton();
            this.drawAboveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.drawInterpolationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.drawBelowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.overwriteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.drawingToolToolStripSplitButton = new System.Windows.Forms.ToolStripSplitButton();
            this.pencilToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.pencilPerPixelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.brushToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fillToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripContainer1.TopToolStripPanel.SuspendLayout();
            this.toolStripContainer1.SuspendLayout();
            this.colorAndBrushToolStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // imageEditor
            // 
            this.imageEditor.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.imageEditor.BackColor = System.Drawing.SystemColors.AppWorkspace;
            this.imageEditor.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.imageEditor.Location = new System.Drawing.Point(0, 26);
            this.imageEditor.Name = "imageEditor";
            this.imageEditor.Size = new System.Drawing.Size(585, 496);
            this.imageEditor.TabIndex = 0;
            this.imageEditor.MouseMove += new System.Windows.Forms.MouseEventHandler(this.imageEditor_MouseMove);
            this.imageEditor.MouseDown += new System.Windows.Forms.MouseEventHandler(this.imageEditor_MouseDown);
            this.imageEditor.MouseUp += new System.Windows.Forms.MouseEventHandler(this.imageEditor_MouseUp);
            // 
            // toolStripContainer1
            // 
            this.toolStripContainer1.BottomToolStripPanelVisible = false;
            // 
            // toolStripContainer1.ContentPanel
            // 
            this.toolStripContainer1.ContentPanel.Size = new System.Drawing.Size(585, 1);
            this.toolStripContainer1.Dock = System.Windows.Forms.DockStyle.Top;
            this.toolStripContainer1.LeftToolStripPanelVisible = false;
            this.toolStripContainer1.Location = new System.Drawing.Point(0, 0);
            this.toolStripContainer1.Name = "toolStripContainer1";
            this.toolStripContainer1.RightToolStripPanelVisible = false;
            this.toolStripContainer1.Size = new System.Drawing.Size(585, 26);
            this.toolStripContainer1.TabIndex = 1;
            this.toolStripContainer1.Text = "toolStripContainer1";
            // 
            // toolStripContainer1.TopToolStripPanel
            // 
            this.toolStripContainer1.TopToolStripPanel.Controls.Add(this.colorAndBrushToolStrip);
            // 
            // colorAndBrushToolStrip
            // 
            this.colorAndBrushToolStrip.Dock = System.Windows.Forms.DockStyle.None;
            this.colorAndBrushToolStrip.GripMargin = new System.Windows.Forms.Padding(8, 2, 2, 2);
            this.colorAndBrushToolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.colorAndBrushToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newImageToolStripButton,
            this.importImageToolStripButton,
            this.toolStripSeparator2,
            this.frontBrushControlColorSelector,
            this.backBrushControlColorSelector,
            this.blendModeToolStripSplitButton,
            this.zoomLevelToolStripComboBox,
            this.toolStripSeparator1,
            this.drawingToolToolStripSplitButton,
            this.brushSizeToolStripTextBox});
            this.colorAndBrushToolStrip.Location = new System.Drawing.Point(3, 0);
            this.colorAndBrushToolStrip.Name = "colorAndBrushToolStrip";
            this.colorAndBrushToolStrip.Size = new System.Drawing.Size(324, 25);
            this.colorAndBrushToolStrip.TabIndex = 2;
            // 
            // frontBrushControlColorSelectorDropDown
            // 
            this.frontBrushControlColorSelectorDropDown.AutoSize = false;
            this.frontBrushControlColorSelectorDropDown.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.Table;
            this.frontBrushControlColorSelectorDropDown.Name = "controlColorSelectorDropDown";
            this.frontBrushControlColorSelectorDropDown.OwnerItem = this.frontBrushControlColorSelector;
            this.frontBrushControlColorSelectorDropDown.Selector = this.frontBrushControlColorSelector;
            this.frontBrushControlColorSelectorDropDown.Size = new System.Drawing.Size(384, 384);
            // 
            // backBrushControlColorSelectorDropDown
            // 
            this.backBrushControlColorSelectorDropDown.AutoSize = false;
            this.backBrushControlColorSelectorDropDown.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.Table;
            this.backBrushControlColorSelectorDropDown.Name = "backBrushControlColorSelectorDropDown";
            this.backBrushControlColorSelectorDropDown.OwnerItem = this.backBrushControlColorSelector;
            this.backBrushControlColorSelectorDropDown.Selector = this.backBrushControlColorSelector;
            this.backBrushControlColorSelectorDropDown.Size = new System.Drawing.Size(384, 384);
            // 
            // zoomLevelToolStripComboBox
            // 
            this.zoomLevelToolStripComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.zoomLevelToolStripComboBox.DropDownWidth = 50;
            this.zoomLevelToolStripComboBox.Items.AddRange(new object[] {
            "10%",
            "25%",
            "50%",
            "75%",
            "100%",
            "150%",
            "200%",
            "300%",
            "400%",
            "800%",
            "1600%",
            "3200%",
            "6400%"});
            this.zoomLevelToolStripComboBox.Name = "zoomLevelToolStripComboBox";
            this.zoomLevelToolStripComboBox.Size = new System.Drawing.Size(75, 25);
            this.zoomLevelToolStripComboBox.ToolTipText = "Select Zoom Level";
            this.zoomLevelToolStripComboBox.SelectedIndexChanged += new System.EventHandler(this.zoomLevelToolStripComboBox_SelectedIndexChanged);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // brushSizeToolStripTextBox
            // 
            this.brushSizeToolStripTextBox.Name = "brushSizeToolStripTextBox";
            this.brushSizeToolStripTextBox.Size = new System.Drawing.Size(25, 25);
            this.brushSizeToolStripTextBox.Text = "2";
            this.brushSizeToolStripTextBox.Visible = false;
            this.brushSizeToolStripTextBox.TextChanged += new System.EventHandler(this.brushSizeToolStripTextBox_TextChanged);
            // 
            // textToolStripButton
            // 
            this.textToolStripButton.Name = "textToolStripButton";
            this.textToolStripButton.Size = new System.Drawing.Size(31, 17);
            this.textToolStripButton.Text = "text";
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // newImageToolStripButton
            // 
            this.newImageToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.newImageToolStripButton.Image = global::Roket3D.Properties.Resources.image_new;
            this.newImageToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.newImageToolStripButton.Name = "newImageToolStripButton";
            this.newImageToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.newImageToolStripButton.Text = "New Image";
            // 
            // importImageToolStripButton
            // 
            this.importImageToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.importImageToolStripButton.Image = global::Roket3D.Properties.Resources.image_import;
            this.importImageToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.importImageToolStripButton.Name = "importImageToolStripButton";
            this.importImageToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.importImageToolStripButton.Text = "Import Image";
            // 
            // frontBrushControlColorSelector
            // 
            this.frontBrushControlColorSelector.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.frontBrushControlColorSelector.DropDown = this.frontBrushControlColorSelectorDropDown;
            this.frontBrushControlColorSelector.Image = ((System.Drawing.Image)(resources.GetObject("frontBrushControlColorSelector.Image")));
            this.frontBrushControlColorSelector.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.frontBrushControlColorSelector.Name = "frontBrushControlColorSelector";
            this.frontBrushControlColorSelector.SelectedColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(255)))), ((int)(((byte)(255)))));
            this.frontBrushControlColorSelector.Size = new System.Drawing.Size(32, 22);
            this.frontBrushControlColorSelector.Text = "Front Brush Color";
            this.frontBrushControlColorSelector.SelectedColorChanged += new Roket3D.ContentEditors.ControlColorSelector.SelectedColorChangedEventHandler(this.frontBrushControlColorSelector_SelectedColorChanged);
            // 
            // backBrushControlColorSelector
            // 
            this.backBrushControlColorSelector.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.backBrushControlColorSelector.DropDown = this.backBrushControlColorSelectorDropDown;
            this.backBrushControlColorSelector.Image = ((System.Drawing.Image)(resources.GetObject("backBrushControlColorSelector.Image")));
            this.backBrushControlColorSelector.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.backBrushControlColorSelector.Name = "backBrushControlColorSelector";
            this.backBrushControlColorSelector.SelectedColor = System.Drawing.Color.Transparent;
            this.backBrushControlColorSelector.Size = new System.Drawing.Size(32, 22);
            this.backBrushControlColorSelector.Text = "Back Brush Color";
            this.backBrushControlColorSelector.SelectedColorChanged += new Roket3D.ContentEditors.ControlColorSelector.SelectedColorChangedEventHandler(this.backBrushControlColorSelector_SelectedColorChanged);
            // 
            // blendModeToolStripSplitButton
            // 
            this.blendModeToolStripSplitButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.blendModeToolStripSplitButton.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.drawAboveToolStripMenuItem,
            this.drawInterpolationToolStripMenuItem,
            this.drawBelowToolStripMenuItem,
            this.toolStripMenuItem1,
            this.overwriteToolStripMenuItem});
            this.blendModeToolStripSplitButton.Image = global::Roket3D.Properties.Resources.draw_above;
            this.blendModeToolStripSplitButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.blendModeToolStripSplitButton.Name = "blendModeToolStripSplitButton";
            this.blendModeToolStripSplitButton.Size = new System.Drawing.Size(32, 22);
            this.blendModeToolStripSplitButton.Text = "Drawing Blend Mode";
            // 
            // drawAboveToolStripMenuItem
            // 
            this.drawAboveToolStripMenuItem.Checked = true;
            this.drawAboveToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.drawAboveToolStripMenuItem.Image = global::Roket3D.Properties.Resources.draw_above;
            this.drawAboveToolStripMenuItem.Name = "drawAboveToolStripMenuItem";
            this.drawAboveToolStripMenuItem.Size = new System.Drawing.Size(224, 22);
            this.drawAboveToolStripMenuItem.Text = "Draw Above";
            this.drawAboveToolStripMenuItem.Click += new System.EventHandler(this.drawAboveToolStripMenuItem_Click);
            // 
            // drawInterpolationToolStripMenuItem
            // 
            this.drawInterpolationToolStripMenuItem.Enabled = false;
            this.drawInterpolationToolStripMenuItem.Image = global::Roket3D.Properties.Resources.draw_interpolate;
            this.drawInterpolationToolStripMenuItem.Name = "drawInterpolationToolStripMenuItem";
            this.drawInterpolationToolStripMenuItem.Size = new System.Drawing.Size(224, 22);
            this.drawInterpolationToolStripMenuItem.Text = "Draw with 50% Interpolation";
            this.drawInterpolationToolStripMenuItem.Click += new System.EventHandler(this.drawInterpolationToolStripMenuItem_Click);
            // 
            // drawBelowToolStripMenuItem
            // 
            this.drawBelowToolStripMenuItem.Image = global::Roket3D.Properties.Resources.draw_below;
            this.drawBelowToolStripMenuItem.Name = "drawBelowToolStripMenuItem";
            this.drawBelowToolStripMenuItem.Size = new System.Drawing.Size(224, 22);
            this.drawBelowToolStripMenuItem.Text = "Draw Below";
            this.drawBelowToolStripMenuItem.Click += new System.EventHandler(this.drawBelowToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(221, 6);
            // 
            // overwriteToolStripMenuItem
            // 
            this.overwriteToolStripMenuItem.Image = global::Roket3D.Properties.Resources.draw_overwrite;
            this.overwriteToolStripMenuItem.Name = "overwriteToolStripMenuItem";
            this.overwriteToolStripMenuItem.Size = new System.Drawing.Size(224, 22);
            this.overwriteToolStripMenuItem.Text = "Overwrite";
            this.overwriteToolStripMenuItem.Click += new System.EventHandler(this.overwriteToolStripMenuItem_Click);
            // 
            // drawingToolToolStripSplitButton
            // 
            this.drawingToolToolStripSplitButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.drawingToolToolStripSplitButton.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.pencilToolStripMenuItem,
            this.pencilPerPixelToolStripMenuItem,
            this.brushToolStripMenuItem,
            this.fillToolStripMenuItem});
            this.drawingToolToolStripSplitButton.Image = global::Roket3D.Properties.Resources.pencil_continous;
            this.drawingToolToolStripSplitButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.drawingToolToolStripSplitButton.Name = "drawingToolToolStripSplitButton";
            this.drawingToolToolStripSplitButton.Size = new System.Drawing.Size(32, 22);
            this.drawingToolToolStripSplitButton.Text = "Select Drawing Tool";
            // 
            // pencilToolStripMenuItem
            // 
            this.pencilToolStripMenuItem.Checked = true;
            this.pencilToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.pencilToolStripMenuItem.Image = global::Roket3D.Properties.Resources.pencil_continous;
            this.pencilToolStripMenuItem.Name = "pencilToolStripMenuItem";
            this.pencilToolStripMenuItem.Size = new System.Drawing.Size(164, 22);
            this.pencilToolStripMenuItem.Text = "Pencil";
            this.pencilToolStripMenuItem.Click += new System.EventHandler(this.pencilToolStripMenuItem_Click);
            // 
            // pencilPerPixelToolStripMenuItem
            // 
            this.pencilPerPixelToolStripMenuItem.Image = global::Roket3D.Properties.Resources.pencil_per_pixel;
            this.pencilPerPixelToolStripMenuItem.Name = "pencilPerPixelToolStripMenuItem";
            this.pencilPerPixelToolStripMenuItem.Size = new System.Drawing.Size(164, 22);
            this.pencilPerPixelToolStripMenuItem.Text = "Pencil (Per Pixel)";
            this.pencilPerPixelToolStripMenuItem.Click += new System.EventHandler(this.pencilPerPixelToolStripMenuItem_Click);
            // 
            // brushToolStripMenuItem
            // 
            this.brushToolStripMenuItem.Image = global::Roket3D.Properties.Resources.image_brush;
            this.brushToolStripMenuItem.Name = "brushToolStripMenuItem";
            this.brushToolStripMenuItem.Size = new System.Drawing.Size(164, 22);
            this.brushToolStripMenuItem.Text = "Brush";
            this.brushToolStripMenuItem.Click += new System.EventHandler(this.brushToolStripMenuItem_Click);
            // 
            // fillToolStripMenuItem
            // 
            this.fillToolStripMenuItem.Image = global::Roket3D.Properties.Resources.image_fill;
            this.fillToolStripMenuItem.Name = "fillToolStripMenuItem";
            this.fillToolStripMenuItem.Size = new System.Drawing.Size(164, 22);
            this.fillToolStripMenuItem.Text = "Fill";
            this.fillToolStripMenuItem.Click += new System.EventHandler(this.fillToolStripMenuItem_Click);
            // 
            // DesignerImageForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(585, 522);
            this.Controls.Add(this.toolStripContainer1);
            this.Controls.Add(this.imageEditor);
            this.Name = "DesignerImageForm";
            this.TabText = "DesignerImageForm";
            this.Text = "DesignerImageForm";
            this.toolStripContainer1.TopToolStripPanel.ResumeLayout(false);
            this.toolStripContainer1.TopToolStripPanel.PerformLayout();
            this.toolStripContainer1.ResumeLayout(false);
            this.toolStripContainer1.PerformLayout();
            this.colorAndBrushToolStrip.ResumeLayout(false);
            this.colorAndBrushToolStrip.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private Roket3D.ContentEditors.ControlImageEditor imageEditor;
        private System.Windows.Forms.ToolStripContainer toolStripContainer1;
        private System.Windows.Forms.ToolStripSplitButton drawingToolToolStripSplitButton;
        private System.Windows.Forms.ToolStripMenuItem pencilToolStripMenuItem;
        private ControlColorSelector frontBrushControlColorSelector;
        private ControlColorSelectorDropDown frontBrushControlColorSelectorDropDown;
        private ControlColorSelectorDropDown backBrushControlColorSelectorDropDown;
        private System.Windows.Forms.ToolStripButton textToolStripButton;
        private ControlColorSelector backBrushControlColorSelector;
        private System.Windows.Forms.ToolStrip colorAndBrushToolStrip;
        private System.Windows.Forms.ToolStripMenuItem brushToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fillToolStripMenuItem;
        private System.Windows.Forms.ToolStripTextBox brushSizeToolStripTextBox;
        private System.Windows.Forms.ToolStripSplitButton blendModeToolStripSplitButton;
        private System.Windows.Forms.ToolStripMenuItem drawAboveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem drawInterpolationToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem drawBelowToolStripMenuItem;
        private System.Windows.Forms.ToolStripComboBox zoomLevelToolStripComboBox;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem overwriteToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem pencilPerPixelToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton newImageToolStripButton;
        private System.Windows.Forms.ToolStripButton importImageToolStripButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
    }
}