using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Roket3D.ContentEditors
{
    public partial class DesignerImageForm : EditorBase
    {
        private Boolean isDrawing = false;
        private Point oldDrawingPoint = new Point(-1, -1);
        private DesignerImageFormToolEnum currentTool = DesignerImageFormToolEnum.PENCIL;
        private Int32 brushSize = 2;
        private System.Drawing.Drawing2D.CompositingMode brushCompositing = System.Drawing.Drawing2D.CompositingMode.SourceOver;
        private Boolean brushReverseApply = false;

        public DesignerImageForm()
        {
            InitializeComponent();

            // Force the toolbars to arrange themselves correctly.
            colorAndBrushToolStrip.Location = new Point(3, 0);

            this.zoomLevelToolStripComboBox.SelectedIndex = 4; // 100%
        }

        public DesignerImageForm(Bitmap image)
        {
            InitializeComponent();

            this.imageEditor.LoadImage(image);

            this.zoomLevelToolStripComboBox.SelectedIndex = 4; // 100%
        }

        private void imageEditor_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left || e.Button == MouseButtons.Right)
            {
                isDrawing = true;
                this.imageEditor.UseFrontBrush = (e.Button == MouseButtons.Left);

                Point realPoint = imageEditor.CalculatePixelFromZoomedScreen(e.Location);
                if (realPoint.X != -1 && realPoint.Y != -1 && realPoint != oldDrawingPoint)
                {
                    this.DrawOperation(realPoint);
                    oldDrawingPoint = realPoint;
                }
            }
        }

        private void imageEditor_MouseMove(object sender, MouseEventArgs e)
        {
            if (isDrawing)
            {
                Point realPoint = imageEditor.CalculatePixelFromZoomedScreen(e.Location);
                if (realPoint.X != -1 && realPoint.Y != -1 && realPoint != oldDrawingPoint)
                {
                    this.DrawOperation(realPoint);
                    oldDrawingPoint = realPoint;
                }
            }
        }

        private void DrawOperation(Point p)
        {
            switch (currentTool)
            {
                case DesignerImageFormToolEnum.PENCIL:
                    if (oldDrawingPoint.X == -1 || oldDrawingPoint.Y == -1)
                        return;

                    if (!this.brushReverseApply)
                    {
                        Graphics peG = Graphics.FromImage(imageEditor.Pixels);
                        peG.CompositingMode = brushCompositing;
                        Pen pp;
                        if (this.imageEditor.UseFrontBrush)
                            pp = new Pen(new SolidBrush(frontBrushControlColorSelector.SelectedColor));
                        else
                            pp = new Pen(new SolidBrush(backBrushControlColorSelector.SelectedColor));
                        peG.DrawLine(pp, oldDrawingPoint, p);
                        peG.Flush();
                        imageEditor.Refresh();
                    }
                    else
                    {
                        // We need to draw our line onto another bitmap and then
                        // draw our existing image onto that bitmap.
                        Bitmap nO = new Bitmap(imageEditor.Pixels.Width, imageEditor.Pixels.Height);
                        Graphics nOG = Graphics.FromImage(nO);
                        Graphics peG = Graphics.FromImage(imageEditor.Pixels);
                        nOG.CompositingMode = brushCompositing;
                        peG.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceCopy;
                        Pen pp;
                        if (this.imageEditor.UseFrontBrush)
                            pp = new Pen(new SolidBrush(frontBrushControlColorSelector.SelectedColor));
                        else
                            pp = new Pen(new SolidBrush(backBrushControlColorSelector.SelectedColor));
                        nOG.DrawLine(pp, oldDrawingPoint, p);
                        nOG.DrawImage(imageEditor.Pixels, new Rectangle(0, 0, imageEditor.Pixels.Width, imageEditor.Pixels.Height));
                        nOG.Flush();
                        peG.DrawImage(nO, new Rectangle(0, 0, imageEditor.Pixels.Width, imageEditor.Pixels.Height));
                        peG.Flush();
                        nOG.Dispose();
                        nO.Dispose();
                        imageEditor.Refresh();
                    }

                    break;
                case DesignerImageFormToolEnum.PENCIL_PER_PIXEL:
                    imageEditor.DrawPixel(p.X, p.Y);
                    break;
                case DesignerImageFormToolEnum.BRUSH:
                    Bitmap n = new Bitmap(brushSize, brushSize);
                    Graphics nG = Graphics.FromImage(n);
                    Graphics pG = Graphics.FromImage(imageEditor.Pixels);
                    Point tlCorner = new Point(p.X - brushSize / 2, p.Y - brushSize / 2);
                    if (this.imageEditor.UseFrontBrush)
                        nG.Clear(frontBrushControlColorSelector.SelectedColor);
                    else
                        nG.Clear(backBrushControlColorSelector.SelectedColor);
                    nG.Flush();
                    nG.Dispose();
                    pG.CompositingMode = brushCompositing;
                    pG.DrawImage(n,new Rectangle(tlCorner.X, tlCorner.Y, brushSize, brushSize));
                    pG.Flush();
                    pG.Dispose();
                    imageEditor.Refresh();
                    break;
                case DesignerImageFormToolEnum.FILL:
                    imageEditor.FloodFill(p.X, p.Y);
                    break;
            }
        }

        private void imageEditor_MouseUp(object sender, MouseEventArgs e)
        {
            isDrawing = false;
            oldDrawingPoint = new Point(-1, -1);
        }

        private void frontBrushControlColorSelector_SelectedColorChanged(object sender, EventArgs e)
        {
            imageEditor.SetFrontBrush(frontBrushControlColorSelector.SelectedColor);
        }

        private void backBrushControlColorSelector_SelectedColorChanged(object sender, EventArgs e)
        {
            imageEditor.SetBackBrush(backBrushControlColorSelector.SelectedColor);
        }

        private void pencilToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SetTool(DesignerImageFormToolEnum.PENCIL);
        }

        private void pencilPerPixelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SetTool(DesignerImageFormToolEnum.PENCIL_PER_PIXEL);
        }

        private void brushToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SetTool(DesignerImageFormToolEnum.BRUSH);
        }

        private void fillToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SetTool(DesignerImageFormToolEnum.FILL);
        }

        private void SetTool(DesignerImageFormToolEnum designerImageFormToolEnum)
        {
            pencilToolStripMenuItem.Checked = false;
            pencilPerPixelToolStripMenuItem.Checked = false;
            brushToolStripMenuItem.Checked = false;
            fillToolStripMenuItem.Checked = false;
            brushSizeToolStripTextBox.Visible = false;
            blendModeToolStripSplitButton.Enabled = true;
            drawInterpolationToolStripMenuItem.Enabled = true;
            drawBelowToolStripMenuItem.Enabled = true;
            this.brushReverseApply = false;

            switch (designerImageFormToolEnum)
            {
                case DesignerImageFormToolEnum.PENCIL:
                    pencilToolStripMenuItem.Checked = true;
                    drawingToolToolStripSplitButton.Image =
                        pencilToolStripMenuItem.Image;
                    switch (imageEditor.BlendMode)
                    {
                        case BlendMode.DRAW_ABOVE:
                            this.brushCompositing = System.Drawing.Drawing2D.CompositingMode.SourceOver;
                            break;
                        case BlendMode.DRAW_INTERPOLATE:
                            SetInterpolation(BlendMode.DRAW_ABOVE);
                            break;
                        case BlendMode.DRAW_BELOW:
                            this.brushReverseApply = true;
                            this.brushCompositing = System.Drawing.Drawing2D.CompositingMode.SourceOver;
                            break;
                        case BlendMode.DRAW_OVERWRITE:
                            this.brushCompositing = System.Drawing.Drawing2D.CompositingMode.SourceCopy;
                            break;
                    }
                    drawInterpolationToolStripMenuItem.Enabled = false;
                    break;
                case DesignerImageFormToolEnum.PENCIL_PER_PIXEL:
                    pencilPerPixelToolStripMenuItem.Checked = true;
                    drawingToolToolStripSplitButton.Image =
                        pencilPerPixelToolStripMenuItem.Image;
                    break;
                case DesignerImageFormToolEnum.BRUSH:
                    brushToolStripMenuItem.Checked = true;
                    drawingToolToolStripSplitButton.Image =
                        brushToolStripMenuItem.Image;
                    brushSizeToolStripTextBox.Visible = true;
                    switch (imageEditor.BlendMode)
                    {
                        case BlendMode.DRAW_ABOVE:
                            this.brushCompositing = System.Drawing.Drawing2D.CompositingMode.SourceOver;
                            break;
                        case BlendMode.DRAW_INTERPOLATE:
                        case BlendMode.DRAW_BELOW:
                            SetInterpolation(BlendMode.DRAW_ABOVE);
                            break;
                        case BlendMode.DRAW_OVERWRITE:
                            this.brushCompositing = System.Drawing.Drawing2D.CompositingMode.SourceCopy;
                            break;
                    }
                    drawInterpolationToolStripMenuItem.Enabled = false;
                    drawBelowToolStripMenuItem.Enabled = false;
                    break;
                case DesignerImageFormToolEnum.FILL:
                    fillToolStripMenuItem.Checked = true;
                    drawingToolToolStripSplitButton.Image =
                        fillToolStripMenuItem.Image;
                    SetInterpolation(BlendMode.DRAW_OVERWRITE);
                    blendModeToolStripSplitButton.Enabled = false;
                    break;
            }

            currentTool = designerImageFormToolEnum;
        }

        private void drawAboveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SetInterpolation(BlendMode.DRAW_ABOVE);
        }

        private void drawInterpolationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SetInterpolation(BlendMode.DRAW_INTERPOLATE);
        }

        private void drawBelowToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SetInterpolation(BlendMode.DRAW_BELOW);
        }

        private void overwriteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SetInterpolation(BlendMode.DRAW_OVERWRITE);
        }

        private void SetInterpolation(BlendMode blendMode)
        {
            imageEditor.BlendMode = blendMode;
            drawAboveToolStripMenuItem.Checked = false;
            drawInterpolationToolStripMenuItem.Checked = false;
            drawBelowToolStripMenuItem.Checked = false;
            overwriteToolStripMenuItem.Checked = false;
            this.brushReverseApply = false;

            switch (blendMode)
            {
                case BlendMode.DRAW_ABOVE:
                    drawAboveToolStripMenuItem.Checked = true;
                    blendModeToolStripSplitButton.Image =
                        drawAboveToolStripMenuItem.Image;
                    this.brushCompositing = System.Drawing.Drawing2D.CompositingMode.SourceOver;
                    break;
                case BlendMode.DRAW_INTERPOLATE:
                    drawInterpolationToolStripMenuItem.Checked = true;
                    blendModeToolStripSplitButton.Image =
                        drawInterpolationToolStripMenuItem.Image;
                    this.brushCompositing = System.Drawing.Drawing2D.CompositingMode.SourceOver;
                    break;
                case BlendMode.DRAW_BELOW:
                    drawBelowToolStripMenuItem.Checked = true;
                    blendModeToolStripSplitButton.Image =
                        drawBelowToolStripMenuItem.Image;
                    this.brushCompositing = System.Drawing.Drawing2D.CompositingMode.SourceOver;
                    if (currentTool == DesignerImageFormToolEnum.PENCIL)
                        this.brushReverseApply = true;
                    break;
                case BlendMode.DRAW_OVERWRITE:
                    overwriteToolStripMenuItem.Checked = true;
                    blendModeToolStripSplitButton.Image =
                        overwriteToolStripMenuItem.Image;
                    this.brushCompositing = System.Drawing.Drawing2D.CompositingMode.SourceCopy;
                    break;
            }
        }

        private void zoomLevelToolStripComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            String zoomLevel = zoomLevelToolStripComboBox.Text;

            // strip ending %
            zoomLevel = zoomLevel.Trim('%');

            // convert to number
            Double zoomLevelN = 100;
            try
            {
                zoomLevelN = Convert.ToDouble(zoomLevel);
            }
            catch (FormatException)
            {
                // nothing to do .. we're just going
                // to set it to 100%
            }

            imageEditor.Zoom = zoomLevelN / 100;
            imageEditor.Refresh();
        }

        private void brushSizeToolStripTextBox_TextChanged(object sender, EventArgs e)
        {
            if (brushSizeToolStripTextBox.Text == "") return;
            try
            {
                Int32 newBrushSize = Convert.ToInt32(brushSizeToolStripTextBox.Text);
                this.brushSize = newBrushSize;
            }
            catch (FormatException)
            {
                MessageBox.Show("Please enter a valid integer number.", "Invalid Brush Size", MessageBoxButtons.OK, MessageBoxIcon.Error);
                brushSizeToolStripTextBox.Text = this.brushSize.ToString();
            }
        }
    }

    public enum DesignerImageFormToolEnum
    {
        PENCIL,
        PENCIL_PER_PIXEL,
        BRUSH,
        FILL
    }
}
