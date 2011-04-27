using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Reflection;
using MOAI.Templates;

namespace MOAI
{
    /// <summary>
    /// The new solution form.
    /// TODO: Clean up this code!
    /// </summary>
    public partial class NewSolutionForm : Form
    {
        private Dictionary<string, string> m_TemplateDescriptionMappings = new Dictionary<string, string>();
        private List<Type> m_TemplateTypes = new List<Type>();
        private BaseTemplate p_SelectedTemplate = null;

        public NewSolutionForm()
        {
            this.InitializeComponent();

            // Ensure that the default project area exists.
            if (!Directory.Exists(Program.Manager.Settings["DefaultProjectArea"]))
            {
                Directory.CreateDirectory(Program.Manager.Settings["DefaultProjectArea"]);
            }

            // Set the solution location text box to the default location area.
            this.c_SolutionLocationTextBox.Text = Program.Manager.Settings["DefaultProjectArea"];

            // Recalculate the project folder.
            this.RecalculateProjectFolderLabel();
        }

        /// <summary>
        /// The creation information generated from the form.
        /// </summary>
        public SolutionCreationData Result
        {
            get
            {
                SolutionCreationData data = new SolutionCreationData();
                data.Name = this.c_ProjectNameTextBox.Text;
                data.Path = this.c_SolutionLocationTextBox.Text;
                data.SolutionDirName = this.c_SolutionFolderTextBox.Text;
                data.SolutionDirUsed = this.c_UseSolutionFolderCheckBox.Checked;
                data.Template = this.p_SelectedTemplate;
                return data;
            }
        }

        /// <summary>
        /// Recalculates the current project path depending on what settings
        /// are currently chosen in the form.
        /// </summary>
        private void RecalculateProjectFolderLabel()
        {
            // Get the selected destination path.
            string dest = this.c_SolutionLocationTextBox.Text;
            if (!dest.EndsWith("\\"))
                dest += "\\";

            // Work out what we need to append to it to get the resultant value.
            if (this.c_UseSolutionFolderCheckBox.Checked)
                this.c_SolutionLocationLabel.Text = dest + this.c_SolutionFolderTextBox.Text + "\\" + this.c_ProjectNameTextBox.Text;
            else
                this.c_SolutionLocationLabel.Text = dest + this.c_ProjectNameTextBox.Text;
        }

        /// <summary>
        /// Initializes the list of templates available.
        /// </summary>
        private void InitializeTemplates()
        {
            // Clear existing images and items.
            this.c_ProjectTypeListView.Items.Clear();
            this.c_ImageList.Images.Clear();

            // Use reflection to get a list of available templates in this IDE.
            this.m_TemplateTypes.Clear();
            Type[] types = Assembly.GetExecutingAssembly().GetTypes();
            foreach (Type t in types)
            {
                if (t.BaseType == typeof(BaseTemplate))
                    this.m_TemplateTypes.Add(t);
            }

            // Now add a list of templates to the new solution form.
            foreach (Type t in this.m_TemplateTypes)
            {
                BaseTemplate b = t.GetConstructor(Type.EmptyTypes).Invoke(null) as BaseTemplate;

                // Create a list view icon.
                ListViewItem lvi = new ListViewItem();
                lvi.Text = b.TemplateName;
                lvi.Group = c_ProjectTypeListView.Groups[0];
                lvi.Name = "Item_" + b.GetType().Name;
                if (b.TemplateIcon != null)
                {
                    this.c_ImageList.Images.Add(b.TemplateIcon);
                    lvi.ImageIndex = this.c_ImageList.Images.Count - 1;
                }
                this.m_TemplateDescriptionMappings.Add(b.TemplateName, b.TemplateDescription);

                // Add it.
                this.c_ProjectTypeListView.Items.Add(lvi);
            }
        }

        /// <summary>
        /// Determines whether a particular string value contains invalid path characters.
        /// </summary>
        /// <param name="name">The string to check.</param>
        /// <returns>Whether the string contains one or more of '\/:*?"<>|'.</returns>
        private Boolean IsValidName(string name)
        {
            return (name.IndexOfAny(new char[] { '\\', '/', ':', '*', '?', '"', '<', '>', '|' }) == -1);
        }

        #region Form Events

        /// <summary>
        /// This event is raised when the "Use Solution Folder" checkbox is toggled.
        /// </summary>
        /// <param name="sender">Sender of the event.</param>
        /// <param name="e">The event information.</param>
        private void c_UseSolutionFolderCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            // Determine what state to place the form in.
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

            // Recalculate the project folder.
            this.RecalculateProjectFolderLabel();
        }

        /// <summary>
        /// This event is raised when the Project Name text box is changed.
        /// </summary>
        /// <param name="sender">Sender of the event.</param>
        /// <param name="e">The event information.</param>
        private void c_ProjectNameTextBox_TextChanged(object sender, EventArgs e)
        {
            // Recalculate the project folder.
            this.RecalculateProjectFolderLabel();

            // Determine whether the current value is invalid.
            this.c_ProjectNameInvalidPictureBox.Visible = !this.IsValidName(this.c_ProjectNameTextBox.Text);
        }

        /// <summary>
        /// This event is raised when the Solution Location text box is changed.
        /// </summary>
        /// <param name="sender">Sender of the event.</param>
        /// <param name="e">The event information.</param>
        private void c_SolutionLocationTextBox_TextChanged(object sender, EventArgs e)
        {
            // Recalculate the project folder.
            this.RecalculateProjectFolderLabel();

            // Determine whether the current value is invalid.
            this.c_SolutionLocationInvalidPictureBox.Visible = !Directory.Exists(this.c_SolutionLocationTextBox.Text);
        }

        /// <summary>
        /// This event is raised when the Solution Folder text box is changed.
        /// </summary>
        /// <param name="sender">Sender of the event.</param>
        /// <param name="e">The event information.</param>
        private void c_SolutionFolderTextBox_TextChanged(object sender, EventArgs e)
        {
            // Recalculate the project folder.
            this.RecalculateProjectFolderLabel();

            // Determine whether the current value is invalid.
            this.c_SolutionFolderInvalidPictureBox.Visible = !this.IsValidName(this.c_SolutionFolderTextBox.Text);
        }

        /// <summary>
        /// This event is raised when the form is loaded.
        /// </summary>
        /// <param name="sender">Sender of the event.</param>
        /// <param name="e">The event information.</param>
        private void NewSolutionForm_Load(object sender, EventArgs e)
        {
            // Initialize the template options.
            this.InitializeTemplates();

            // Select the first template by default.
            this.c_ProjectTypeListView.SelectedIndices.Clear();
            this.c_ProjectTypeListView.SelectedIndices.Add(0);
        }

        /// <summary>
        /// This event is raised when "..." button next to the Solution Location text box is clicked.
        /// </summary>
        /// <param name="sender">Sender of the event.</param>
        /// <param name="e">The event information.</param>
        private void c_BrowseSolutionLocationButton_Click(object sender, EventArgs e)
        {
            // Check to make sure that the default project area directory exists.
            if (!Directory.Exists(Program.Manager.Settings["DefaultProjectArea"]))
                Directory.CreateDirectory(Program.Manager.Settings["DefaultProjectArea"]);

            // Create a new folder browser dialog.
            FolderBrowserDialog fbd = new FolderBrowserDialog();
            fbd.Description = "Select a location to store the project or solution folder in.";
            fbd.SelectedPath = Program.Manager.Settings["DefaultProjectArea"];
            fbd.ShowNewFolderButton = true;

            // Ask the user to select a folder.
            if (fbd.ShowDialog() == DialogResult.OK)
                this.c_SolutionLocationTextBox.Text = fbd.SelectedPath;
        }

        /// <summary>
        /// This event is raised when the type of template to use is changed.
        /// </summary>
        /// <param name="sender">Sender of the event.</param>
        /// <param name="e">The event information.</param>
        private void c_ProjectTypeListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Determine whether or not the selection is invalid.
            this.c_ProjectTypeInvalidPictureBox.Visible = (this.c_ProjectTypeListView.SelectedItems.Count == 0);

            // Only attempt to determine the description if there is one
            // item selected.
            if (this.c_ProjectTypeListView.SelectedItems.Count == 1)
            {
                // Get the name of the selected item and pair it up with
                // the appropriate BaseTemplate.
                string name = this.c_ProjectTypeListView.SelectedItems[0].Name;
                this.p_SelectedTemplate = null;
                foreach (Type t in this.m_TemplateTypes)
                    if (name == "Item_" + t.Name)
                    {
                        this.p_SelectedTemplate = t.GetConstructor(Type.EmptyTypes).Invoke(null) as BaseTemplate;
                        break;
                    }

                // Check to make sure that we've actually got a template.
                if (this.p_SelectedTemplate == null)
                {
                    this.c_ProjectTypeInvalidPictureBox.Visible = true;
                    this.c_TemplateDescriptionTextBox.Text = "Unknown template type selected.";
                    return;
                }

                // Update the description area.
                this.c_TemplateDescriptionTextBox.Text = "Creates " + this.p_SelectedTemplate.TemplateDescription;
            }
        }

        /// <summary>
        /// This event is raised when the OK button is pressed.
        /// </summary>
        /// <param name="sender">Sender of the event.</param>
        /// <param name="e">The event information.</param>
        private void c_OKButton_Click(object sender, EventArgs e)
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

        /// <summary>
        /// This event is raised when the Cancel button is pressed.
        /// </summary>
        /// <param name="sender">Sender of the event.</param>
        /// <param name="e">The event information.</param>
        private void c_CancelButton_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        #endregion
    }
}
