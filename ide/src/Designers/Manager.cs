using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MOAI.Management;
using System.Windows.Forms;

namespace MOAI.Designers
{
    public class Manager
    {
        private MOAI.Manager p_Parent = null;
        private List<Designer> m_Designers = new List<Designer>();

        public delegate void DesignerEventHandler(object sender, DesignerEventArgs e);
        public event DesignerEventHandler DesignerCreated;
        public event DesignerEventHandler DesignerOpened;
        public event DesignerEventHandler DesignerRefocused;
        public event DesignerEventHandler DesignerClosed;

        public class DesignerEventArgs : EventArgs
        {
            private Designer p_Designer = null;

            public DesignerEventArgs(Designer designer)
            {
                this.p_Designer = designer;
            }

            public Designer Designer
            {
                get
                {
                    return this.p_Designer;
                }
            }
        }

        /// <summary>
        /// Creates a new designer manager.
        /// </summary>
        public Manager(MOAI.Manager parent)
        {
            this.p_Parent = parent;
        }

        /// <summary>
        /// Opens a new designer for the specified file, or returns an existing
        /// editor for this file.
        /// </summary>
        /// <param name="file"></param>
        /// <returns></returns>
        public Designer OpenDesigner(File file)
        {
            // Search through our existing designers to see if there's
            // already a designer open for this file.
            foreach (Designer d in this.m_Designers)
            {
                if (d.File.FileInfo.FullName == file.FileInfo.FullName)
                {
                    if (this.DesignerRefocused != null)
                        this.DesignerRefocused(this, new DesignerEventArgs(d));
                    return d;
                }
            }

            // Ensure that FileInfo property is valid (if it is not, then we
            // are dealing with a non-file).
            if (file.FileInfo == null)
                return null;

            // Detect the type of designer to create by the file's extension.
            Type t = Associations.GetDesignerType(file.FileInfo.Extension.Substring(1));
            if (t == null)
            {
                MessageBox.Show("There is no designer associated with this file type.", "No Designer", MessageBoxButtons.OK);
                return null;
            }

            // Invoke the constructor.
            Designer ds = t.GetConstructor(new Type[] { typeof(MOAI.Manager), typeof(File) }).Invoke(new object[] { this.p_Parent as object, file as object }) as Designer;
            ds.FormClosed += (sender, e) =>
                {
                    if (this.DesignerClosed != null)
                        this.DesignerClosed(this, new DesignerEventArgs(ds));
                    this.m_Designers.Remove(ds);
                };
            ds.Shown += (sender, e) =>
                {
                    if (this.DesignerOpened != null)
                        this.DesignerOpened(this, new DesignerEventArgs(ds));
                };
            this.m_Designers.Add(ds);
            if (this.DesignerCreated != null)
                this.DesignerCreated(this, new DesignerEventArgs(ds));

            return ds;
        }

        /// <summary>
        /// The main MOAI manager that owns this designer manager.
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
