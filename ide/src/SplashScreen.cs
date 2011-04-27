using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CodeProject;
using System.Drawing;
using System.Windows.Forms;
using System.Drawing.Drawing2D;

namespace MOAI
{
    public partial class SplashScreen : FloatingWindow
    {
        private TextureBrush m_BackgroundBrush;
        private SolidBrush m_TextBrush = new SolidBrush(Color.Black);
        private Font m_TextFont;
        private Image m_ProgressOutsideBrush;
        private Image m_ProgressInsideBrush;
        private Int32 m_Progress = 0;
        private String m_Message = "Initalizing MOAI IDE...";

        public SplashScreen()
        {
            // Set size and position
            this.Size = new Size(438, 258);
            this.Location = new Point(
                    Screen.PrimaryScreen.Bounds.X + Screen.PrimaryScreen.Bounds.Width / 2 - (215 + 8),
                    Screen.PrimaryScreen.Bounds.Y + Screen.PrimaryScreen.Bounds.Height / 2 - (125 + 8)
                    );

            Image texBack = Properties.Resources.splashscreen_back;
            this.m_ProgressOutsideBrush = Properties.Resources.splashscreen_progress_outside;
            this.m_ProgressInsideBrush = Properties.Resources.splashscreen_progress_inside;
            this.m_BackgroundBrush = new TextureBrush(texBack);

            this.m_TextFont = SystemFonts.DialogFont;// new Font(FontFamily.GenericSerif, 10, FontStyle.Regular, GraphicsUnit.Pixel);
        }

        public new void Invalidate()
        {
            base.Invalidate();
        }

        public new void Show()
        {
            base.ShowAnimate(CodeProject.FloatingWindow.AnimateMode.Blend, 500);
        }
        public new void Hide()
        {
            HideAnimate(CodeProject.FloatingWindow.AnimateMode.Blend, 500);
        }

        public void SetProgress(Int32 progress, String message)
        {
            this.m_Progress = progress;
            this.m_Message = message;
            this.Invalidate();
        }

        protected override void PerformPaint(PaintEventArgs e)
        {
            if (base.Handle != IntPtr.Zero)
            {
                Graphics g = e.Graphics;
                g.SmoothingMode = SmoothingMode.HighQuality;

                // Draw background
                g.FillRectangle(this.m_BackgroundBrush, this.Bound);

                // Draw progress bar
                g.DrawImage(this.m_ProgressOutsideBrush, 10, 188, 1, 4);
                g.DrawImage(this.m_ProgressInsideBrush, 11, 188, 1, 4);
                g.DrawImage(this.m_ProgressInsideBrush, (Int32)(12 + (float)this.m_Progress / 100 * 406 + 1), 188, 1, 4);
                g.DrawImage(this.m_ProgressOutsideBrush, (Int32)(12 + (float)this.m_Progress / 100 * 406 + 2), 188, 1, 4);

                g.DrawImage(this.m_ProgressInsideBrush, 12, 188, (Int32)(1 + (float)this.m_Progress / 100 * 406), 4);

                // Draw message
                g.DrawString(this.m_Message, this.m_TextFont, this.m_TextBrush, 9, 163);
                g.DrawString(@"MOAI IDE is an open source project, available under an MIT license.  The MIT license
only pertains to the IDE and engine.  Games created with MOAI may be licensed", this.m_TextFont, this.m_TextBrush, 9, 202);
                g.DrawString(@"

as the author wishes.  See the About box for more information.", this.m_TextFont, this.m_TextBrush, 9, 203);

                //g.DrawImageUnscaled(this.progressOutsideBrush, 20 + this.Progress / 100 * 408, 188);
                //g.DrawImage(this.progressInsideBrush, 13, 188, 20, 4);

                //text
                //g.DrawString(String.Format("{0} v{1}", Application.ProductName, Application.ProductVersion), mTextFont, mTextBrush, 40, 20);
                //g.DrawString(mMessageText, SystemFonts.DialogFont, mTextBrush, 40, this.Height - 40);

                //progress bar
                //float w = this.Width - 10 * 2;
                //g.DrawLine(mPenProgress, 40, this.Height - 20, 40 + w * mCompletionFraction, this.Height - 20);
                //line changed to texture:
                //g.DrawImageUnscaledAndClipped(
                //    Properties.Resources.image_load_progress_texture,
                //    new Rectangle(10, 36, (int)(w * mCompletionFraction), Properties.Resources.image_load_progress_texture.Height));
            }
        }
    }
}
