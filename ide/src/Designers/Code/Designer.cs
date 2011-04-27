using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MOAI.Management;

namespace MOAI.Designers.Code
{
    public partial class Designer : MOAI.Designers.Designer
    {
        private List<LuaError> m_Errors = new List<LuaError>();
        private CodeEditor c_CodeEditor = null;
        private ToolTip c_ToolTip = new ToolTip();
        private string m_SavedText = null;

        /// <summary>
        /// Creates a new code editor.
        /// </summary>
        /// <param name="manager">The main MOAI manager.</param>
        /// <param name="file">The associated file.</param>
        public Designer(MOAI.Manager manager, File file)
            : base(manager, file)
        {
            InitializeComponent();

            // This editor can save as long as the file is writable.
            this.CanSave = !file.FileInfo.IsReadOnly;

            this.c_CodeEditor = new CodeEditor(
                manager.CacheManager,
                (file.FileInfo.Extension.Substring(1) == "lua" ||
                 file.FileInfo.Extension.Substring(1) == "rks" ||
                 file.FileInfo.Extension.Substring(1) == "rs")
                );
            this.c_CodeEditor.DwellStart += new EventHandler<ScintillaNet.ScintillaMouseEventArgs>(c_CodeEditor_DwellStart);
            this.c_CodeEditor.DwellEnd += new EventHandler<ScintillaNet.ScintillaMouseEventArgs>(c_CodeEditor_DwellEnd);
            this.c_CodeEditor.KeyUp += new KeyEventHandler(c_CodeEditor_KeyUp);
            this.c_CodeEditor.SyntaxCheckRequested += new EventHandler(c_CodeEditor_SyntaxCheckRequested);

            // Now load the file data.
            using (System.IO.StreamReader reader = new System.IO.StreamReader(file.FileInfo.FullName))
            {
                this.c_CodeEditor.Text = reader.ReadToEnd();
            }
            this.m_SavedText = this.c_CodeEditor.Text;
            this.TabText = this.File.FileInfo.Name;

            this.Controls.Add(this.c_CodeEditor);
        }

        /// <summary>
        /// This event is raised when the user has placed their mouse in the same position
        /// for a specified amount of time.
        /// </summary>
        /// <param name="sender">The sender of the event.</param>
        /// <param name="e">The mouse event information.</param>
        private void c_CodeEditor_DwellStart(object sender, ScintillaNet.ScintillaMouseEventArgs e)
        {
            // The user may want to find out what the error is.
            foreach (LuaError err in this.m_Errors)
            {
                if (e.Position >= err.IndicatorIndex && e.Position <= err.Index + err.Length)
                {
                    this.c_ToolTip.UseFading = false;
                    this.c_ToolTip.UseAnimation = false;
                    this.c_ToolTip.Show(err.ErrorMsg, this.c_CodeEditor, e.X + 20, e.Y + 10);
                }
            }
        }

        /// <summary>
        /// This event is raised when the user starts moving their mouse again after
        /// DwellStart has been previously raised.
        /// </summary>
        /// <param name="sender">The sender of the event.</param>
        /// <param name="e">The mouse event information.</param>
        private void c_CodeEditor_DwellEnd(object sender, ScintillaNet.ScintillaMouseEventArgs e)
        {
            this.c_ToolTip.Hide(this.c_CodeEditor);
        }

        /// <summary>
        /// This event is raised when the user releases a key while the code editor
        /// has focus.
        /// </summary>
        /// <param name="sender">The sender of the event.</param>
        /// <param name="e">The keyboard event information.</param>
        void c_CodeEditor_KeyUp(object sender, KeyEventArgs e)
        {
            if (this.c_CodeEditor.Text != this.m_SavedText)
                this.TabText = this.File.FileInfo.Name + " *";
            else
                this.TabText = this.File.FileInfo.Name;
        }

        /// <summary>
        /// This event is raised when a syntax check needs to be performed on the
        /// current document.
        /// </summary>
        /// <param name="sender">The sender of the event.</param>
        /// <param name="e">The event information.</param>
        private void c_CodeEditor_SyntaxCheckRequested(object sender, EventArgs e)
        {
            // Prevent flicker.
            if (this.c_CodeEditor.CanSyntaxCheck)
            {
                // TODO: Implement a refresh call to the Error List tool window.
            }

            // Clear all the indicators.
            this.c_CodeEditor.GetRange(0, this.c_CodeEditor.Text.Length).ClearIndicator(0);

            // Get errors.
            m_Errors = SyntaxChecker.Check(this.c_CodeEditor.Text, this.c_CodeEditor);

            // Apply highlighting to the errors.
            // TODO: Use a class variable to store the current filename.
            if (this.c_CodeEditor.CanSyntaxCheck)
            {
                // TODO: Clear all of the errors from the Error List tool window for
                //       this specific file.
            }
            foreach (LuaError err in m_Errors)
            {
                // Indicators have an offset by one.
                this.c_CodeEditor.GetRange(err.IndicatorIndex, err.Index + err.Length).SetIndicator(0);

                // TODO: Add the error to the error list.
                if (this.c_CodeEditor.CanSyntaxCheck)
                {
                }
            }

            // Prevent flicker.
            if (this.c_CodeEditor.CanSyntaxCheck)
            {
                // TODO: Implement a refresh call to the Error List tool window.
            }
        }

        /// <summary>
        /// This function is called after the IDE has finished resizing itself, or the
        /// total size of this dock content has otherwise changed.
        /// </summary>
        protected override void OnResize()
        {
        }

        /// <summary>
        /// This function is called after the active tab has changed.
        /// </summary>
        protected override void OnTabChanged()
        {
        }

        /// <summary>
        /// This function is called after the file that this designer owns has been saved to disk.
        /// </summary>
        public override void OnSaveFile()
        {
            using (System.IO.StreamWriter writer = new System.IO.StreamWriter(this.File.FileInfo.FullName, false))
            {
                writer.WriteLine(this.c_CodeEditor.Text);
            }
            this.TabText = this.File.FileInfo.Name;
            this.m_SavedText = this.c_CodeEditor.Text;
        }
    }
}
