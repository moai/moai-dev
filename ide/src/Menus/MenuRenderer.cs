using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace MOAI.Menus
{
    class MenuRenderer : ToolStripRenderer
    {
        private LinearGradientBrush topBrush = new LinearGradientBrush(new Point(0, 0), new Point(0, 26), Color.FromArgb(97, 97, 97), Color.FromArgb(75, 75, 75));

        protected override void Initialize(ToolStrip ts)
        {
            base.Initialize(ts);
        }

        protected override void OnRenderToolStripBackground(
            ToolStripRenderEventArgs e)
        {
            base.OnRenderToolStripBackground(e);

            if (e.ToolStrip is MenuStrip)
            {
                e.Graphics.FillRectangle(topBrush, new Rectangle(0, 0, e.ToolStrip.Width, e.ToolStrip.Height));
            }
        }
    }
}
