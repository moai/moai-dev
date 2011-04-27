using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using DockPanelSuite;

namespace MOAI.Tools
{
    public partial class OutputTool : Tool
    {
        public OutputTool()
        {
            InitializeComponent();
        }

        public override DockState DefaultState
        {
            get
            {
                return DockState.DockBottom;
            }
        }

        public void AddLogEntry(string message)
        {
            this.c_BuildOutputTextBox.Text += message + @"
";
            this.c_BuildOutputTextBox.SelectionStart = c_BuildOutputTextBox.Text.Length;
            this.c_BuildOutputTextBox.SelectionLength = 0;
            this.c_BuildOutputTextBox.ScrollToCaret();
            this.Activate();
        }

        public void ClearLog()
        {
            this.c_BuildOutputTextBox.Text = "";
        }
    }
}
