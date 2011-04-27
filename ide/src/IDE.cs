using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using DockPanelSuite;
using LuaInterface;
using System.IO;
using MOAI.Designers;

namespace MOAI
{
    public partial class IDE : Form
    {
        private Manager p_Manager = null;

        /// <summary>
        /// Creates a new main IDE.
        /// </summary>
        public IDE(Manager manager)
        {
            InitializeComponent();
            this.p_Manager = manager;

            // Listen for the creation of new designers.
            this.p_Manager.DesignersManager.DesignerCreated += (sender, e) =>
                {
                    this.ShowDock(e.Designer, DockState.Document);
                    this.OnActiveTabChanged();
                };
            this.p_Manager.DesignersManager.DesignerRefocused += (sender, e) =>
                {
                    e.Designer.Activate();
                    this.OnActiveTabChanged();
                };
        }

        /// <summary>
        /// This function allows DockContents to place themselves within the
        /// DockPanel of the main IDE.
        /// </summary>
        /// <param name="dc">The DockContent to place in the IDE.</param>
        /// <param name="state">Where and how the DockContent should be placed.</param>
        public void ShowDock(DockContent dc, DockState state)
        {
            dc.Show(this.c_DockWorkspace, state);
        }

        /// <summary>
        /// This event is raised when the form is first shown.
        /// </summary>
        /// <param name="sender">The sender of the event.</param>
        /// <param name="e">The event information.</param>
        private void IDE_Load(object sender, EventArgs e)
        {
            // Load the main menu.
            this.MainMenuStrip = this.p_Manager.MenuManager.GetMainMenu();
            this.Controls.Add(this.MainMenuStrip);

            // Load the tool bar.
            this.c_ToolStripContainer.TopToolStripPanel.Controls.Add(this.p_Manager.MenuManager.GetToolBar());

            // Set up the workspace.
            this.p_Manager.ToolsManager.Show(typeof(Tools.OutputTool));
            this.p_Manager.ToolsManager.Show(typeof(Tools.ErrorListTool));
            this.p_Manager.ToolsManager.Show(typeof(Tools.SolutionExplorerTool));

            // Show the start page.
            this.ShowDock(new Designers.Start.Designer(this.p_Manager, null), DockState.Document);
        }

        /// <summary>
        /// This event is raised when the IDE is maximized or restored.
        /// </summary>
        /// <param name="sender">Sender of the event.</param>
        /// <param name="e">The event information.</param>
        private void IDE_StyleChanged(object sender, EventArgs e)
        {
            if (this.WindowState == FormWindowState.Maximized ||
                this.WindowState == FormWindowState.Normal)
            {
                this.OnResizeEnd(e);
            }
        }

        /// <summary>
        /// This event is fired when the active tab changes.
        /// </summary>
        /// <param name="sender">Sender of the event.</param>
        /// <param name="e">The event information.</param>
        private void c_DockWorkspace_ActiveTabChanged(object sender, EventArgs e)
        {
            this.OnActiveTabChanged();
        }

        /// <summary>
        /// This event is raised when the active tab changes.
        /// </summary>
        public event EventHandler ActiveTabChanged;
        protected void OnActiveTabChanged()
        {
            if (this.ActiveTabChanged != null)
                this.ActiveTabChanged(this, new EventArgs());
        }

        /// <summary>
        /// The currently active tab.
        /// </summary>
        public IDockContent ActiveTab
        {
            get
            {
                return this.c_DockWorkspace.ActiveTab;
            }
        }
    }
}
