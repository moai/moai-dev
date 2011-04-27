using System;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Collections.Generic;

namespace System.Windows.Forms
{
    public class DockPanelStripRenderer : ToolStripRenderer
    {
        private Boolean drawBottomBorder;
        private ProfessionalColorTable colorTable;
        static ToolStripRenderer renderer;

        public DockPanelStripRenderer() : this(true) { }

        public DockPanelStripRenderer(Boolean drawBottomBorder)
        {
            this.drawBottomBorder = drawBottomBorder;
            this.colorTable = new ProfessionalColorTable();
            // DTODO: Calculate the correct setting here.
            //UiRenderMode renderMode = PluginBase.MainForm.Settings.RenderMode;
            //if (renderMode == UiRenderMode.System) renderer = new ToolStripSystemRenderer();
            //else renderer = new ToolStripProfessionalRenderer();
            renderer = new ToolStripProfessionalRenderer();
        }

        protected override void OnRenderToolStripBackground(ToolStripRenderEventArgs e)
        {
            ToolStripRenderEventArgs ea = new ToolStripRenderEventArgs(e.Graphics, e.ToolStrip, new Rectangle(-10, -3, e.AffectedBounds.Width + 20, e.AffectedBounds.Height + 6), e.BackColor);
            renderer.DrawToolStripBackground(ea);
        }

        protected override void OnRenderToolStripBorder(ToolStripRenderEventArgs e)
        {
            if (e.ToolStrip is ToolStripDropDownMenu)
            {
                renderer.DrawToolStripBorder(e);
            }
            else if (this.drawBottomBorder)
            {
                Rectangle r = e.AffectedBounds;
                e.Graphics.DrawLine(SystemPens.ControlDark, r.Left, r.Bottom - 1, r.Right, r.Bottom - 1);
            }
        }

        protected override void OnRenderSeparator(ToolStripSeparatorRenderEventArgs e)
        {
            if (renderer is ToolStripSystemRenderer)
            {
                if (e.ToolStrip is ToolStripDropDownMenu) renderer.DrawSeparator(e);
                else
                {
                    Pen pen = new Pen(SystemColors.InactiveBorder);
                    Int32 middle = e.Item.ContentRectangle.Left + e.Item.ContentRectangle.Width / 2;
                    e.Graphics.DrawLine(pen, middle, e.Item.ContentRectangle.Top + 1, middle, e.Item.ContentRectangle.Bottom - 2);
                    pen.Dispose();
                }
            }
            else renderer.DrawSeparator(e);
        }

        protected override void OnRenderGrip(ToolStripGripRenderEventArgs e)
        {
            if (renderer is ToolStripProfessionalRenderer)
            {
                if (e.GripStyle == ToolStripGripStyle.Hidden) return;
                using (Brush lightBrush = new SolidBrush(this.colorTable.GripLight))
                {
                    Rectangle r = new Rectangle(e.GripBounds.Left, e.GripBounds.Top + 6, 2, 2);
                    for (Int32 i = 0; i < e.GripBounds.Height - 11; i += 4)
                    {
                        e.Graphics.FillRectangle(lightBrush, r);
                        r.Offset(0, 4);
                    }
                }
                using (Brush darkBrush = new SolidBrush(this.colorTable.GripDark))
                {
                    Rectangle r = new Rectangle(e.GripBounds.Left - 1, e.GripBounds.Top + 5, 2, 2);
                    for (Int32 i = 0; i < e.GripBounds.Height - 11; i += 4)
                    {
                        e.Graphics.FillRectangle(darkBrush, r);
                        r.Offset(0, 4);
                    }
                }
            }
            else renderer.DrawGrip(e);
        }

        #region Reuse Some Renderer Stuff

        protected override void OnRenderButtonBackground(ToolStripItemRenderEventArgs e)
        {
            renderer.DrawButtonBackground(e);
        }

        protected override void OnRenderDropDownButtonBackground(ToolStripItemRenderEventArgs e)
        {
            renderer.DrawDropDownButtonBackground(e);
        }

        protected override void OnRenderItemBackground(ToolStripItemRenderEventArgs e)
        {
            renderer.DrawItemBackground(e);
        }

        protected override void OnRenderItemCheck(ToolStripItemImageRenderEventArgs e)
        {
            renderer.DrawItemCheck(e);
        }

        protected override void OnRenderItemText(ToolStripItemTextRenderEventArgs e)
        {
            renderer.DrawItemText(e);
        }

        protected override void OnRenderItemImage(ToolStripItemImageRenderEventArgs e)
        {
            renderer.DrawItemImage(e);
        }

        protected override void OnRenderArrow(ToolStripArrowRenderEventArgs e)
        {
            renderer.DrawArrow(e);
        }

        protected override void OnRenderImageMargin(ToolStripRenderEventArgs e)
        {
            renderer.DrawImageMargin(e);
        }

        protected override void OnRenderLabelBackground(ToolStripItemRenderEventArgs e)
        {
            renderer.DrawLabelBackground(e);
        }

        protected override void OnRenderMenuItemBackground(ToolStripItemRenderEventArgs e)
        {
            renderer.DrawMenuItemBackground(e);
        }

        protected override void OnRenderOverflowButtonBackground(ToolStripItemRenderEventArgs e)
        {
            renderer.DrawOverflowButtonBackground(e);
        }

        protected override void OnRenderSplitButtonBackground(ToolStripItemRenderEventArgs e)
        {
            renderer.DrawSplitButton(e);
        }

        protected override void OnRenderStatusStripSizingGrip(ToolStripRenderEventArgs e)
        {
            renderer.DrawStatusStripSizingGrip(e);
        }

        protected override void OnRenderToolStripContentPanelBackground(ToolStripContentPanelRenderEventArgs e)
        {
            renderer.DrawToolStripContentPanelBackground(e);
        }

        protected override void OnRenderToolStripPanelBackground(ToolStripPanelRenderEventArgs e)
        {
            renderer.DrawToolStripPanelBackground(e);
        }

        protected override void OnRenderToolStripStatusLabelBackground(ToolStripItemRenderEventArgs e)
        {
            renderer.DrawToolStripStatusLabelBackground(e);
        }

        #endregion

    }

}
