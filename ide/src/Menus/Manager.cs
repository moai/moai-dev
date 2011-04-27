using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace MOAI.Menus
{
    public class Manager
    {
        private MOAI.Manager p_Parent = null;
        private MenuLoader m_Loader = null;

        /// <summary>
        /// Creates a new menu manager.
        /// </summary>
        public Manager(MOAI.Manager parent)
        {
            this.p_Parent = parent;
            this.m_Loader = new MenuLoader(this);
        }

        /// <summary>
        /// Retrieves the main menu to be associated with the IDE window.
        /// </summary>
        /// <returns>The main menu to be associated with the IDE window.</returns>
        public MenuStrip GetMainMenu()
        {
            return this.m_Loader.MainMenu;
        }

        /// <summary>
        /// Retrieves the toolbar to be associated with the IDE window.
        /// </summary>
        /// <returns>The toolbar to be associated with the IDE window.</returns>
        public ToolStrip GetToolBar()
        {
            return this.m_Loader.ToolBar;
        }

        public void AssociateEvents(Action action)
        {
            this.p_Parent.OnSolutionLoaded += new EventHandler((sender, e) =>
                {
                    action.OnSolutionLoaded();
                });
            this.p_Parent.OnSolutionUnloaded += new EventHandler((sender, e) =>
                {
                    action.OnSolutionUnloaded();
                });
            this.p_Parent.DesignersManager.DesignerOpened += new MOAI.Designers.Manager.DesignerEventHandler((sender, e) =>
                {
                    if (this.p_Parent.IDEWindow.ActiveTab is Designers.Designer)
                        action.OnTabChanged(this.p_Parent.IDEWindow.ActiveTab as Designers.Designer);
                });
            this.p_Parent.IDEWindow.ActiveTabChanged += new EventHandler((sender, e) =>
                {
                    if (this.p_Parent.IDEWindow.ActiveTab is Designers.Designer)
                        action.OnTabChanged(this.p_Parent.IDEWindow.ActiveTab as Designers.Designer);
                });
        }

        /// <summary>
        /// The main MOAI manager that owns this menu manager.
        /// </summary>
        public MOAI.Manager Parent
        {
            get
            {
                return this.p_Parent;
            }
        }
    }
}
