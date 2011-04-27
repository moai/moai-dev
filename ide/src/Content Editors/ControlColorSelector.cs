using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Roket3D.ContentEditors
{
    public partial class ControlColorSelector : System.Windows.Forms.ToolStripSplitButton
    {
        private Color _SelectedColor = Color.White;
        public Color SelectedColor
        {
            get
            {
                return this._SelectedColor;
            }
            set
            {
                this._SelectedColor = value;
            }
        }

        public ControlColorSelector()
        {
            InitializeComponent();
            this.Image = new Bitmap(16, 16);
            this.DropDown = null;
        }

        private void ControlColorSelector_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.DrawImage(
                Properties.Resources.image_transparency_tile,
                Margin.Left + 3,
                Margin.Top + 3,
                14,
                14
                );

            e.Graphics.FillRectangle(
                new SolidBrush(this.SelectedColor),
                new Rectangle(
                    Margin.Left + 3,
                    Margin.Top + 3,
                    14,
                    14
                    )
                );

            // Draw border
            #region Draw Border
            e.Graphics.DrawLine(
                new Pen(SystemColors.ControlDarkDark),
                new Point(
                    Margin.Left + 2,
                    Margin.Top + 2
                    ),
                new Point(
                    Margin.Left + 2 + 15,
                    Margin.Top + 2
                    )
                );
            e.Graphics.DrawLine(
                new Pen(SystemColors.ControlDarkDark),
                new Point(
                    Margin.Left + 2,
                    Margin.Top + 2
                    ),
                new Point(
                    Margin.Left + 2,
                    Margin.Top + 2 + 15
                    )
                );
            e.Graphics.DrawLine(
                new Pen(SystemColors.ControlDark),
                new Point(
                    Margin.Left + 2 + 15,
                    Margin.Top + 2
                    ),
                new Point(
                    Margin.Left + 2 + 15,
                    Margin.Top + 2 + 15
                    )
                );
            e.Graphics.DrawLine(
                new Pen(SystemColors.ControlDark),
                new Point(
                    Margin.Left + 2,
                    Margin.Top + 2 + 15
                    ),
                new Point(
                    Margin.Left + 2 + 15,
                    Margin.Top + 2 + 15
                    )
                );
            #endregion
        }

        #region Events
        // Specify the selected color changed handler.
        public event SelectedColorChangedEventHandler SelectedColorChanged;

        public delegate void SelectedColorChangedEventHandler(object sender, EventArgs e);

        public virtual void OnSelectedColorChanged(EventArgs e)
        {
            if (SelectedColorChanged != null)
                SelectedColorChanged(this, e);
        }
        #endregion
    }
}
