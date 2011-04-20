using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Roket3D.Menus
{
    public class Manager
    {
        private Roket3D.Manager p_Parent = null;
        private MenuLoader m_Loader = null;

        /// <summary>
        /// Creates a new menu manager.
        /// </summary>
        public Manager(Roket3D.Manager parent)
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
            this.p_Parent.DesignersManager.DesignerOpened += new Roket3D.Designers.Manager.DesignerEventHandler((sender, e) =>
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
        /// The main Roket3D manager that owns this menu manager.
        /// </summary>
        public Roket3D.Manager Parent
        {
            get
            {
                return this.p_Parent;
            }
        }
    }
}
