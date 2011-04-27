using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace MOAI.Menus
{
    class ToolBarRenderer : ToolStripRenderer
    {
        private SolidBrush toolBrush = new SolidBrush(Color.FromArgb(75, 75, 75));

        protected override void Initialize(ToolStrip ts)
        {
            base.Initialize(ts);
        }

        protected override void OnRenderToolStripContentPanelBackground(ToolStripContentPanelRenderEventArgs e)
        {
            base.OnRenderToolStripContentPanelBackground(e);

            e.Graphics.FillRectangle(toolBrush, new Rectangle(0, 0, e.ToolStripContentPanel.Width, e.ToolStripContentPanel.Height));
            e.Graphics.DrawEllipse(new Pen(Color.Red), 0, 0, 25, 25);
        }

        protected override void OnRenderToolStripPanelBackground(ToolStripPanelRenderEventArgs e)
        {
            base.OnRenderToolStripPanelBackground(e);

            e.Graphics.FillRectangle(toolBrush, new Rectangle(0, 0, e.ToolStripPanel.Width, e.ToolStripPanel.Height));
            e.Graphics.DrawEllipse(new Pen(Color.Red), 0, 0, 25, 25);
        }

        protected override void OnRenderToolStripBackground(
            ToolStripRenderEventArgs e)
        {
            base.OnRenderToolStripBackground(e);

            if (e.ToolStrip is ToolStrip)
            {
                e.Graphics.FillRectangle(toolBrush, new Rectangle(0, 0, e.ToolStrip.Width, e.ToolStrip.Height));
            }
        }
    }
}
