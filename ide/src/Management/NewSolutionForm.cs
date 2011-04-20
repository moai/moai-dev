using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace Roket3D
{
    public partial class NewSolutionForm : Form
    {
        public NewSolutionForm()
        {
            InitializeComponent();

            if (!Directory.Exists(Program.Manager.Settings["DefaultProjectArea"]))
            {
                Directory.CreateDirectory(Program.Manager.Settings["DefaultProjectArea"]);
            }
            c_SolutionLocationTextBox.Text = Program.Manager.Settings["DefaultProjectArea"];
            UpdateProjectFolderLabel();
        }

        private void UseSolutionFolderCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (c_UseSolutionFolderCheckBox.Checked)
            {
                c_FolderLabel.Text = "Solution Location:";
                c_SolutionFolderTextBox.Enabled = true;
                c_SolutionFolderTextBox.ReadOnly = false;
            }
            else
            {
                c_FolderLabel.Text = "Project Location:";
                c_SolutionFolderTextBox.Enabled = false;
                c_SolutionFolderTextBox.ReadOnly = true;
            }
            UpdateProjectFolderLabel();
        }

        private void UpdateProjectFolderLabel()
        {
            String fL = c_SolutionLocationTextBox.Text;
            if (!fL.EndsWith("\\"))
                fL += "\\";
            if (c_UseSolutionFolderCheckBox.Checked)
            {
                c_SolutionLocationLabel.Text = fL + c_SolutionFolderTextBox.Text + "\\" + c_ProjectNameTextBox.Text;
            }
            else
            {
                c_SolutionLocationLabel.Text = fL + c_ProjectNameTextBox.Text;
            }
        }

        private void ProjectNameTextBox_TextChanged(object sender, EventArgs e)
        {
            UpdateProjectFolderLabel();
            c_ProjectNameInvalidPictureBox.Visible = !this.IsValidName(c_ProjectNameTextBox.Text);
        }

        private void SolutionLocationTextBox_TextChanged(object sender, EventArgs e)
        {
            UpdateProjectFolderLabel();
            c_SolutionLocationInvalidPictureBox.Visible = !System.IO.Directory.Exists(c_SolutionLocationTextBox.Text);
        }

        private void SolutionFolderTextBox_TextChanged(object sender, EventArgs e)
        {
            UpdateProjectFolderLabel();
            c_SolutionFolderInvalidPictureBox.Visible = !this.IsValidName(c_SolutionFolderTextBox.Text);
        }

        private void NewSolutionForm_Load(object sender, EventArgs e)
        {
            // Select Managed by default.
            c_ProjectTypeListView.SelectedIndices.Clear();
            c_ProjectTypeListView.SelectedIndices.Add(0);
        }

        private Boolean IsValidName(String Name)
        {
            return (Name.IndexOfAny(new char[] { '\\', '/', ':', '*', '?', '"', '<', '>', '|' }) == -1);
        }

        private void BrowseSolutionLocationButton_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog fbd = new FolderBrowserDialog();
            fbd.Description = "Select a location to store the project or solution folder in.";
            if (!Directory.Exists(Program.Manager.Settings["DefaultProjectArea"]))
            {
                Directory.CreateDirectory(Program.Manager.Settings["DefaultProjectArea"]);
            }
            fbd.SelectedPath = Program.Manager.Settings["DefaultProjectArea"];
            fbd.ShowNewFolderButton = true;
            if (fbd.ShowDialog() == DialogResult.OK)
            {
                c_SolutionLocationTextBox.Text = fbd.SelectedPath;
            }
        }

        private void ProjectTypeListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            c_ProjectTypeInvalidPictureBox.Visible = (c_ProjectTypeListView.SelectedItems.Count == 0);

            if (c_ProjectTypeListView.SelectedItems.Count == 1)
            {
                ListViewItem selectedItem = null;
                Boolean shouldProjectChangeName = false;
                Boolean shouldSolutionChangeName = false;
                selectedItem = c_ProjectTypeListView.SelectedItems[0];

                switch (c_ProjectNameTextBox.Text)
                {
                    case "ManagedGame":
                    case "UnmanagedGame":
                        shouldProjectChangeName = true;
                        break;
                }
                switch (c_SolutionFolderTextBox.Text)
                {
                    case "ManagedSolution":
                    case "UnmanagedSolution":
                        shouldSolutionChangeName = true;
                        break;
                }

                switch (selectedItem.Text)
                {
                    case "Managed Lua":
                        if (shouldProjectChangeName)
                            c_ProjectNameTextBox.Text = "ManagedGame";
                        if (shouldSolutionChangeName)
                            c_SolutionFolderTextBox.Text = "ManagedSolution";
                        c_TemplateDescriptionTextBox.Text = "Creates an IDE managed, Roket3D project with Lua as the scripting language.";
                        break;
                    case "Unmanaged Lua":
                        if (shouldProjectChangeName)
                            c_ProjectNameTextBox.Text = "UnmanagedGame";
                        if (shouldSolutionChangeName)
                            c_SolutionFolderTextBox.Text = "UnmanagedSolution";
                        c_TemplateDescriptionTextBox.Text = "Creates an unmanaged Roket3D project with Lua as the scripting language.  Some features may not be available.";
                        break;
                }
            }
        }

        private void OKButton_Click(object sender, EventArgs e)
        {
            if (c_ProjectNameInvalidPictureBox.Visible || c_ProjectTypeInvalidPictureBox.Visible ||
                c_SolutionFolderInvalidPictureBox.Visible || c_SolutionLocationInvalidPictureBox.Visible)
            {
                MessageBox.Show("One or more of the provided fields is invalid.  Make sure that all of the fields are valid and a project type is selected and try again.",
                    "Invalid Information Provided", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void CancelButton_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        static public Boolean HandleNewProjectCreation(NewSolutionForm nsf)
        {
            String projectName = nsf.c_ProjectNameTextBox.Text;
            String solutionName = null;
            if (nsf.c_UseSolutionFolderCheckBox.Checked)
                solutionName = nsf.c_SolutionFolderTextBox.Text;
            else
                solutionName = projectName;

            String projectFileLocation = nsf.c_SolutionLocationLabel.Text + "\\" + projectName + ".rproj";
            String projectFolderLocation = nsf.c_SolutionLocationLabel.Text;
            String solutionFileLocation = null;
            String solutionFolderLocation = null;

            String fL = nsf.c_SolutionLocationTextBox.Text;
            if (!fL.EndsWith("\\"))
                fL += "\\";

            if (nsf.c_UseSolutionFolderCheckBox.Checked)
            {
                solutionFileLocation = fL + nsf.c_SolutionFolderTextBox.Text + "\\" + solutionName + ".rsln";
                solutionFolderLocation = fL + nsf.c_SolutionFolderTextBox.Text;
            }
            else
            {
                solutionFileLocation = nsf.c_SolutionLocationLabel.Text + "\\" + solutionName + ".rsln";
                solutionFolderLocation = nsf.c_SolutionLocationLabel.Text;
            }

            // Create the directories
            if (!System.IO.Directory.Exists(solutionFolderLocation))
            {
                System.IO.Directory.CreateDirectory(solutionFolderLocation);
            }
            if (!System.IO.Directory.Exists(projectFolderLocation))
            {
                System.IO.Directory.CreateDirectory(projectFolderLocation);
            }

            // TODO: Make use of the selected project type.
            //Program.Manager.ActiveSolution.New(solutionName, solutionFileLocation, solutionFolderLocation,
            //                                        projectName, projectFileLocation, projectFolderLocation);
            //Program.Manager.ActiveSolution.SolutionExplorer.ReloadTree();
            //Program.Manager.ActiveSolution.SendMenuEvent(MenuEvent.SOLUTION_OPEN);

            // TODO: Capture exceptions and report false if the creation failed.
            return true;
        }
    }
}
