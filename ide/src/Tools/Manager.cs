using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DockPanelSuite;

namespace MOAI.Tools
{
    public class Manager
    {
        List<Tool> m_Windows;
        MOAI.Manager p_Parent = null;

        /// <summary>
        /// Creates a new Manager class for managing the tool windows which
        /// are shown on the screen.
        /// </summary>
        /// <param name="parent">The main MOAI manager which owns this tool manager.</param>
        public Manager(MOAI.Manager parent)
        {
            this.m_Windows = new List<Tool>();
            this.p_Parent = parent;
        }

        /// <summary>
        /// Shows the specified tool window by provide it's type.  This function
        /// has no effect if the tool window is already visible.
        /// </summary>
        /// <param name="window">The type of tool window to be shown.</param>
        public void Show(Type window)
        {
            foreach (DockContent d in this.m_Windows)
            {
                if (d.GetType() == window)
                {
                    d.Show();
                    return;
                }
            }

            System.Reflection.ConstructorInfo constructor = null;
            Tool c = null;

            // Construct the new tool window.
            constructor = window.GetConstructor(new Type[] { });
            if (constructor == null)
            {
                constructor = window.GetConstructor(new Type[] { typeof(Tools.Manager) });
                c = constructor.Invoke(new object[] { this }) as Tool;
            }
            else
                c = constructor.Invoke(null) as Tool;

            // Add the solution loaded/unloaded events.
            this.p_Parent.OnSolutionLoaded += new EventHandler((sender, e) =>
                {
                    c.OnSolutionLoaded();
                });
            this.p_Parent.OnSolutionUnloaded += new EventHandler((sender, e) =>
            {
                c.OnSolutionUnloaded();
            });

            this.p_Parent.IDEWindow.ShowDock(c, c.DefaultState);
            this.m_Windows.Add(c);
        }

        /// <summary>
        /// Retrieves the specified tool window by it's type, or returns null if
        /// the tool window does not exist.
        /// </summary>
        /// <param name="window">The type of tool window to be retrieve.</param>
        /// <returns>The tool window, or null if none exists.</returns>
        public DockContent Get(Type window)
        {
            foreach (DockContent d in this.m_Windows)
            {
                if (d.GetType() == window)
                    return d;
            }

            return null;
        }

        /// <summary>
        /// The main MOAI manager that owns this tool manager.
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
