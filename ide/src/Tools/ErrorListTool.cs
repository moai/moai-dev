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
    public partial class ErrorListTool : Tool
    {
        public ErrorListTool()
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

        private void ToolErrorList_Resize(object sender, EventArgs e)
        {
            c_ErrorIconColumn.Width = 24;
            c_ErrorDescriptionColumn.Width = c_ErrorList.Width - 24 - 16 - 120 - 60 * 2;
            c_TextLineNumberColumn.Width = 60;
            c_TextColumnColumn.Width = 60;
            c_ErrorProjectColumn.Width = 120;
        }

        public void ClearErrorsForFile(String filename, String project)
        {
            ListViewGroup fileGroup = null;
            foreach (ListViewGroup g in c_ErrorList.Groups)
            {
                if (g.Header == filename)
                {
                    fileGroup = g;
                }
            }

            if (fileGroup == null) return;

            for (Int32 i = 0; i < fileGroup.Items.Count; i += 1)
            {
                if (fileGroup.Items[i].SubItems[4].Text == project)
                {
                    ListViewItem li = fileGroup.Items[i];
                    fileGroup.Items.Remove(li);
                    c_ErrorList.Items.Remove(li);
                    i -= 1; // readjust current index because we removed one.
                }
            }
        }

        public void AddError(String description, String filename, Int32 line, Int32 column, String project)
        {
            // Get the group.
            ListViewGroup fileGroup = null;
            foreach (ListViewGroup g in c_ErrorList.Groups)
            {
                if (g.Header == filename)
                {
                    fileGroup = g;
                }
            }
            if (fileGroup == null)
            {
                fileGroup = c_ErrorList.Groups.Add("dynamicGroup" + filename.Replace(".",""), filename);
            }

            ListViewItem newError = c_ErrorList.Items.Add("", "Error");
            newError.SubItems.Add(line.ToString());
            newError.SubItems.Add(column.ToString());
            newError.SubItems.Add(description);
            newError.SubItems.Add(project);
            newError.Group = fileGroup;
        }
    }
}
