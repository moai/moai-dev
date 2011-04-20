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
    public partial class ControlColorSelectorDropDown : ToolStripDropDown
    {
        private Bitmap colorPalette = Properties.Resources.color_palette;
        private TextureBrush TransparencyBrush = new TextureBrush(Properties.Resources.image_transparency_tile);
        private ControlColorSelector _Selector = null;

        public ControlColorSelector Selector
        {
            get
            {
                return this._Selector;
            }
            set
            {
                this._Selector = value;
            }
        }

        public ControlColorSelectorDropDown()
        {
            InitializeComponent();

            this.LayoutStyle = ToolStripLayoutStyle.Table;
            this.Items.Clear();
            this.Items.Add(new ControlClassSelectorDummyItem());
        }

        private void ControlColorSelectorDropDown_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.FillRectangle(
                TransparencyBrush,
                new Rectangle(
                    0,
                    0,
                    colorPalette.Width,
                    colorPalette.Height
                    )
                );
            e.Graphics.DrawImage(colorPalette, 0, 0, 384, 384);
        }

        private void ControlColorSelectorDropDown_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void ControlColorSelectorDropDown_MouseMove(object sender, MouseEventArgs e)
        {
            Selector.SelectedColor = colorPalette.GetPixel(e.X, e.Y);
            Selector.OnSelectedColorChanged(new EventArgs());
            Selector.Invalidate();
        }
    }

    public class ControlClassSelectorDummyItem : ToolStripItem
    {
        private TextureBrush TransparencyBrush = new TextureBrush(Properties.Resources.image_transparency_tile);

        public ControlClassSelectorDummyItem()
        {
            this.AutoSize = false;
            this.Size = new Size(0,0);
            this.Paint += ControlClassSelectorDummyItem_Paint;
        }

        private void ControlClassSelectorDummyItem_Paint(object sender, PaintEventArgs e)
        {
        }

    }
}
