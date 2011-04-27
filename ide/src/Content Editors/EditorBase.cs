using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DockPanelSuite;

namespace Roket3D.ContentEditors
{
    public class EditorBase : DockContent
    {
        public String Path = null;
        public Roket3D.Xml.File File = null;
        public EditorProperties Properties = new EditorProperties(false);
        
        // Functions that are called by MainForm
        // after a resize or when designers need
        // to forcable close their Irrlicht thread.
        public virtual void TerminateThread() {}
        public virtual void OnResizeEnd() { }
        // DONE: Create an event in DockPanelSuite for every
        //       time the active tab changes since there doesn't
        //       seem to be a reliable way to detect this at the
        //       moment.
        public virtual void OnActiveTabChanged() { }
        // Events for saving / modifing resources from the main toolbar.
        public virtual void OnSaveFile() { }
        public virtual void OnSaveFile(String SaveAsName) { }
    }

    public class EditorProperties
    {
        public Boolean CanSave = false;

        public EditorProperties(Boolean CanSave)
        {
            this.CanSave = CanSave;
        }
    }
}
