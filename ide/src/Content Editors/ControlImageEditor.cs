using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Drawing2D;
using System.IO;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;

namespace Roket3D.ContentEditors
{
    public partial class ControlImageEditor : System.Windows.Forms.UserControl
    {
        public Bitmap Pixels;
        public double Zoom = 1;

        private TextureBrush TransparencyBrush = new TextureBrush(Properties.Resources.image_transparency_tile);
        private SolidBrush FrontBrush = new SolidBrush(Color.White);
        private SolidBrush BackBrush = new SolidBrush(Color.White);
        public Boolean UseFrontBrush = true;
        public BlendMode BlendMode = BlendMode.DRAW_ABOVE;

        #region Initalization / Rendering

        public ControlImageEditor()
        {
            InitializeComponent();

            Pixels = new Bitmap(64, 64);
            this.DoubleBuffered = true;
        }

        public ControlImageEditor(Int32 width, Int32 height)
        {
            InitializeComponent();

            Pixels = new Bitmap(width, height);
        }

        public ControlImageEditor(Bitmap existingImage)
        {
            InitializeComponent();

            Pixels = existingImage;
            // Because the only format that can be edited out-of-the-box
            // is JPEG, we're going to convert the image to editable format
            // implicitly.
            ConvertToEditableFormat();
        }

        public void LoadImage(Bitmap existingImage)
        {
            Pixels = existingImage;
            // Because the only format that can be edited out-of-the-box
            // is JPEG, we're going to convert the image to editable format
            // implicitly.
            ConvertToEditableFormat();
        }

        private void ConvertToEditableFormat()
        {
            Bitmap newB = new Bitmap(Pixels.Width, Pixels.Height);
            Graphics convB = Graphics.FromImage(newB);
            convB.Clear(Color.Transparent);
            convB.DrawImage(Pixels, new Rectangle(0, 0, Pixels.Width, Pixels.Height));
            convB.Flush(FlushIntention.Flush);
            convB.Dispose();
            Pixels = newB;
        }

        private void ControlImageEditor_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
            e.Graphics.Clear(this.BackColor);
            e.Graphics.FillRectangle(TransparencyBrush,
                    0.0f,
                    0.0f,
                    (float)(Pixels.Width * Zoom),
                    (float)(Pixels.Height * Zoom)
                    );

            e.Graphics.DrawImage(
                Pixels,
                new RectangleF(0.0f, 0.0f,
                    (float)(Pixels.Width * Zoom),
                    (float)(Pixels.Height * Zoom)),
                new RectangleF(-0.5f, -0.5f, (float)Pixels.Width, (float)Pixels.Height),
                GraphicsUnit.Pixel
                );
            e.Graphics.Flush(System.Drawing.Drawing2D.FlushIntention.Sync);
        }

        #endregion

        #region Settings

        public void SetFrontBrush(Color col)
        {
            FrontBrush.Color = col;
        }

        public void SetBackBrush(Color col)
        {
            BackBrush.Color = col;
        }

        public Color GetFrontBrush()
        {
            return FrontBrush.Color;
        }

        public Color GetBackBrush()
        {
            return BackBrush.Color;
        }

        #endregion

        #region Calculation Functions

        public Point CalculatePixelFromZoomedScreen(Point p)
        {
            if (p.X >= Pixels.Width * Zoom || p.Y >= Pixels.Height * Zoom || p.X < 0 || p.Y < 0)
                return new Point(-1,-1);

            return new Point((Int32)(p.X / Zoom), (Int32)(p.Y / Zoom));
        }

        #endregion

        #region Drawing Functions

        public void DrawPixel(Int32 x, Int32 y)
        {
            Color acol = Color.Transparent;

            if (UseFrontBrush)
                acol = FrontBrush.Color;
            else
                acol = BackBrush.Color;

            Color rcol = Color.Transparent;

            switch (this.BlendMode)
            {
                case BlendMode.DRAW_ABOVE:
                    rcol = ControlImageEditor.InterpolateColors(
                             Pixels.GetPixel(x, y),
                             acol
                             );
                    break;
                case BlendMode.DRAW_INTERPOLATE:
                    rcol = ControlImageEditor.InterpolateColors(
                             Pixels.GetPixel(x, y),
                             acol,
                             0.5f
                             );
                    break;
                case BlendMode.DRAW_BELOW:
                    rcol = ControlImageEditor.InterpolateColors(
                             acol,
                             Pixels.GetPixel(x, y)
                             );
                    break;
                case BlendMode.DRAW_OVERWRITE:
                    rcol = acol;
                    break;
            }

            try
            {
                Pixels.SetPixel(x, y, rcol);
            }
            catch (InvalidOperationException)
            {
                if (MessageBox.Show(@"It appears that the image you are trying to edit is not in a format that is
editable by Roket3D.  We can attempt to convert the image to editable
format.  Would you like to do this now?", "Image Not Editable", MessageBoxButtons.YesNo) == DialogResult.Yes)
                {
                    ConvertToEditableFormat();
                }
            }

            this.Refresh();
        }

        public void FloodFill(Int32 x, Int32 y)
        {
            Color c;
            if (this.UseFrontBrush)
                c = this.FrontBrush.Color;
            else
                c = this.BackBrush.Color;
            _FloodFill(this.Pixels, x, y, c);
            this.Refresh();
        }

        private void _FloodFill(Bitmap bitmap, int x, int y, Color color)
        {
            BitmapData data = bitmap.LockBits(
                new Rectangle(0, 0, bitmap.Width, bitmap.Height),
                ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            int[] bits = new int[data.Stride / 4 * data.Height];
            Marshal.Copy(data.Scan0, bits, 0, bits.Length);

            LinkedList<Point> check = new LinkedList<Point>();
            int floodTo = color.ToArgb();
            int floodFrom = bits[x + y * data.Stride / 4];
            bits[x + y * data.Stride / 4] = floodTo;

            if (floodFrom != floodTo)
            {
                check.AddLast(new Point(x, y));
                while (check.Count > 0)
                {
                    Point cur = check.First.Value;
                    check.RemoveFirst();

                    foreach (Point off in new Point[] {
                new Point(0, -1), new Point(0, 1), 
                new Point(-1, 0), new Point(1, 0)})
                    {
                        Point next = new Point(cur.X + off.X, cur.Y + off.Y);
                        if (next.X >= 0 && next.Y >= 0 &&
                            next.X < data.Width &&
                            next.Y < data.Height)
                        {
                            if (bits[next.X + next.Y * data.Stride / 4] == floodFrom)
                            {
                                check.AddLast(next);
                                bits[next.X + next.Y * data.Stride / 4] = floodTo;
                            }
                        }
                    }
                }
            }

            Marshal.Copy(bits, 0, data.Scan0, bits.Length);
            bitmap.UnlockBits(data);
        }

        #endregion

        #region Interpolation

        public static Color InterpolateColors(Color a, Color b)
        {
            // Clamp values
            double amount = (double)b.A / (double)255;
            amount = (amount < 0) ? 0 : amount;
            amount = (amount > 1) ? 1 : amount;

            double inv = 1.0f - amount;

            Int32 nA = b.A + a.A;
            if (nA < 0) nA = 0;
            if (nA > 255) nA = 255;

            return Color.FromArgb(
                nA,
                (Int32)(a.R * inv + b.R * amount),
                (Int32)(a.G * inv + b.G * amount),
                (Int32)(a.B * inv + b.B * amount)
                );
        }

        public static Color InterpolateColors(Color a, Color b, float amount)
        {
            // Clamp values
            amount = (amount < 0) ? 0 : amount;
            amount = (amount > 1) ? 1 : amount;

            double inv = 1.0f - amount;

            return Color.FromArgb(
                (Int32)(b.A * inv + a.A * amount),
                (Int32)(b.R * inv + a.R * amount),
                (Int32)(b.G * inv + a.G * amount),
                (Int32)(b.B * inv + a.B * amount)
                );
        }

        #endregion
    }

    public enum BlendMode
    {
        DRAW_ABOVE,
        DRAW_INTERPOLATE,
        DRAW_BELOW,
        DRAW_OVERWRITE
    }
}
