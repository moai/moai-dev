using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Roket3D.Build
{
    public class Manager
    {
        Roket3D.Manager p_Parent = null;

        /// <summary>
        /// Creates a new Manager class for managing the build process.
        /// </summary>
        /// <param name="parent">The main Roket3D manager which owns this build manager.</param>
        public Manager(Roket3D.Manager parent)
        {
            this.p_Parent = parent;
        }

        /// <summary>
        /// The main Roket3D manager that owns this build manager.
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
