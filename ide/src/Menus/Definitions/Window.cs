using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MOAI.Menus.Definitions.Window
{
    class NewWindow : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "New Window";
            this.Enabled = false;
        }
    }

    class Split : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.windows_split;
            this.Text = "Split";
            this.Enabled = false;
        }
    }

    class SetFloating : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "Floating";
            this.Enabled = false;
        }
    }

    class SetDockable : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "Dockable";
            this.Enabled = false;
        }
    }

    class SetTabbedDocument : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "Tabbed Document";
            this.Enabled = false;
        }
    }

    class SetAutoHide : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.windows_autohide;
            this.Text = "Auto Hide";
            this.Enabled = false;
        }
    }

    class Hide : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "Hide";
            this.Enabled = false;
        }
    }

    class AutoHideAll : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "Auto Hide All";
            this.Enabled = false;
        }
    }

    class NewHorizontalTabGroup : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.windows_tile_horizontal;
            this.Text = "New Horizontal Tab Group";
            this.Enabled = false;
        }
    }

    class NewVerticalTabGroup : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.windows_tile_vertical;
            this.Text = "New Vertical Tab Group";
            this.Enabled = false;
        }
    }

    class CloseAll : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "Close All Documents";
            this.Enabled = false;
        }
    }

    class Reset : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "Reset Window Layout";
            this.Enabled = false;
        }
    }

    class Windows : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = null;
            this.Text = "Windows...";
            this.Enabled = false;
        }
    }
}
