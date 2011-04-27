using System;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using System.Collections.Generic;
using DockPanelSuite;

namespace System.Windows.Forms
{
    [Flags]
    public enum DockBorders
    {
        None = 0,
        Left = 1 << 1,
        Top = 1 << 2,
        Right = 1 << 3,
        Bottom = 1 << 4
    }

    public class DockPanelControl : UserControl
    {
        DockBorders borders;

        public DockPanelControl()
        {
            Borders = DockBorders.Left | DockBorders.Right;
        }

        private DockBorders Borders
        {
            get { return borders; }
            set
            {
                borders = value;
                this.Padding = new Padding((borders & DockBorders.Left) > 0 ? 1 : 0, (borders & DockBorders.Top) > 0 ? 1 : 0, (borders & DockBorders.Right) > 0 ? 1 : 0, (borders & DockBorders.Bottom) > 0 ? 1 : 0);
            }
        }

        /// <summary>
        /// Actual painting is done here
        /// </summary>
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            this.CheckDockPosition();
            if ((borders & DockBorders.Left) > 0)
            {
                e.Graphics.DrawLine(SystemPens.ControlDark, e.ClipRectangle.Left, e.ClipRectangle.Top, e.ClipRectangle.Left, e.ClipRectangle.Bottom + 1);
            }
            if ((borders & DockBorders.Top) > 0)
            {
                e.Graphics.DrawLine(SystemPens.ControlDark, e.ClipRectangle.Left, e.ClipRectangle.Top, e.ClipRectangle.Right, e.ClipRectangle.Top);
            }
            if ((borders & DockBorders.Right) > 0)
            {
                e.Graphics.DrawLine(SystemPens.ControlDark, e.ClipRectangle.Right - 1, e.ClipRectangle.Top, e.ClipRectangle.Right - 1, e.ClipRectangle.Bottom + 1);
            }
            if ((borders & DockBorders.Bottom) > 0)
            {
                e.Graphics.DrawLine(SystemPens.ControlDark, e.ClipRectangle.Left, e.ClipRectangle.Bottom - 1, e.ClipRectangle.Right, e.ClipRectangle.Bottom - 1);
            }
        }

        /// <summary>
        /// Special logic that draws the borders for content
        /// </summary>
        private void CheckDockPosition()
        {
            Boolean isOnlyTab;
            DockContent dock = this.Parent as DockContent;
            if (dock == null || dock.Pane == null) return;
            if (dock.IsFloat)
            {
                isOnlyTab = this.CountPanels(false) == 1;
                if (isOnlyTab) Borders = DockBorders.Left | DockBorders.Top | DockBorders.Right | DockBorders.Bottom;
                else Borders = DockBorders.Left | DockBorders.Top | DockBorders.Right;
            }
            else
            {
                isOnlyTab = this.CountPanels(true) == 1;
                if (isOnlyTab) Borders = DockBorders.Left | DockBorders.Bottom | DockBorders.Right;
                else Borders = DockBorders.Left | DockBorders.Right;
            }
        }

        /// <summary>
        /// Counts the the contents excluding hidden and floating (option) windows
        /// </summary>
        private Int32 CountPanels(Boolean includeFloats)
        {
            Int32 count = 0;
            DockContent dock = this.Parent as DockContent;
            for (Int32 i = 0; i < dock.Pane.Contents.Count; i++)
            {
                if (dock.Pane.Contents[i] is DockContent)
                {
                    IDockContent current = dock.Pane.Contents[i];
                    if (includeFloats && !current.DockHandler.IsHidden && !current.DockHandler.IsFloat) count++;
                    else if (!includeFloats && !current.DockHandler.IsHidden) count++;
                }
            }
            return count;
        }

    }

}
