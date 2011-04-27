using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MOAI.Menus.Definitions.Views
{
    class Code : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.view_code;
            this.Text = "Code";
            this.Enabled = false;
        }

        public override void OnLoad()
        {
            if (this.MenuItem != null)
            {
                this.MenuItem.Checked = true;
            }
        }
    }

    class Designer : Action
    {
        public override void OnSetSettings()
        {
            this.Implemented = false;
            this.ItemIcon = Properties.Resources.view_designer;
            this.Text = "Designer";
            this.Enabled = false;
        }

        public override void OnLoad()
        {
            if (this.MenuItem != null)
            {
                this.MenuItem.Checked = false;
            }
        }
    }
}
