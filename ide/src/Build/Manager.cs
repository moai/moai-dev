using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MOAI.Build
{
    public class Manager
    {
        MOAI.Manager p_Parent = null;

        /// <summary>
        /// Creates a new Manager class for managing the build process.
        /// </summary>
        /// <param name="parent">The main MOAI manager which owns this build manager.</param>
        public Manager(MOAI.Manager parent)
        {
            this.p_Parent = parent;
        }

        /// <summary>
        /// The main MOAI manager that owns this build manager.
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
