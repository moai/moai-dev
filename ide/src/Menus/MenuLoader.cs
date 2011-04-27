using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;
using System.Windows.Forms;
using System.Reflection;

namespace MOAI.Menus
{
    class MenuLoader
    {
        private XmlReader m_Reader;
        public MenuStrip MainMenu = new MenuStrip();
        public ToolStrip ToolBar = new ToolStrip();

        private ToolStripMenuItem c_ActiveMenuItem = null;
        private ToolStripDropDownButton c_ActiveDropDown = null;
        private ToolStripComboBox c_ActiveComboBox = null;
        private ToolStripItem c_ActiveItem = null;
        private Assembly m_CurrentAssembly = Assembly.GetExecutingAssembly();
        private Menus.Manager m_Manager;

        public MenuLoader(Menus.Manager manager)
        {
            this.m_Manager = manager;

            if (!File.Exists(Program.Manager.Settings["RootPath"] + "\\Menus.xml"))
            {
                throw new Exception("Menu information file was not found.  Please make sure Menus.xml exists in the application directory.");
            }

            // style menubar and toolbar
            // TODO: Finish style system.
            //this.MainMenu.Renderer = new MenuRenderer();
            //this.ToolBar.Renderer = new ToolBarRenderer();

            this.m_Reader = XmlReader.Create(new StreamReader(Program.Manager.Settings["RootPath"] + "\\Menus.xml"));
            
            while (this.m_Reader.Read())
            {
                switch (this.m_Reader.NodeType)
                {
                    case XmlNodeType.Element:
                        switch (this.m_Reader.Name)
                        {
                            case "menus":
                            case "menubar":
                            case "toolbar":
                                // Nothing to do here.
                                break;
                            case "menuitem":
                                this.AddMenuItem(this.m_Reader.GetAttribute("text"));

                                this.AddReflectionHandler(this.m_Reader.GetAttribute("action"));

                                if (this.m_Reader.IsEmptyElement)
                                {
                                    // Automatically end element.
                                    c_ActiveItem = c_ActiveItem.OwnerItem;
                                    this.UpdateObjects();

                                    // The parent had some menu items, therefore we
                                    // enable it regardless of whether it has an action.
                                    if (c_ActiveItem != null && !(c_ActiveItem is ToolStripDropDownButton))
                                        c_ActiveItem.Enabled = true;
                                }
                                break;
                            case "menuseperator":
                                this.AddMenuItem("-");

                                if (this.m_Reader.IsEmptyElement)
                                {
                                    // Automatically end element.
                                    c_ActiveItem = c_ActiveItem.OwnerItem;
                                    this.UpdateObjects();

                                    // The parent had some menu items, therefore we
                                    // enable it regardless of whether it has an action.
                                    if (c_ActiveItem != null && !(c_ActiveItem is ToolStripDropDownButton))
                                        c_ActiveItem.Enabled = true;
                                }
                                break;
                            case "toolitem":
                                if (this.m_Reader.GetAttribute("type") == "dropdown")
                                    this.AddToolDropDown(this.m_Reader.GetAttribute("text"));
                                else
                                    this.AddToolItem(this.m_Reader.GetAttribute("text"));

                                this.AddReflectionHandler(this.m_Reader.GetAttribute("action"));

                                if (this.m_Reader.IsEmptyElement)
                                {
                                    // Automatically end element.
                                    c_ActiveItem = c_ActiveItem.OwnerItem;
                                    this.UpdateObjects();

                                    // The parent had some menu items, therefore we
                                    // enable it regardless of whether it has an action.
                                    if (c_ActiveItem != null)
                                        c_ActiveItem.Enabled = true;
                                }
                                break;
                            case "toolseperator":
                                this.AddToolItem("-");

                                if (this.m_Reader.IsEmptyElement)
                                {
                                    // Automatically end element.
                                    c_ActiveItem = c_ActiveItem.OwnerItem;
                                    this.UpdateObjects();

                                    // The parent had some menu items, therefore we
                                    // enable it regardless of whether it has an action.
                                    if (c_ActiveItem != null)
                                        c_ActiveItem.Enabled = true;
                                }
                                break;
                            case "toolcombo":
                                this.AddToolComboBox(this.m_Reader.GetAttribute("text"));

                                if (this.m_Reader.GetAttribute("editable") == "false")
                                    c_ActiveComboBox.DropDownStyle = ComboBoxStyle.DropDownList;

                                //this.AddReflectionHandler(this.Reader.GetAttribute("action"));

                                if (this.m_Reader.IsEmptyElement)
                                {
                                    // Automatically end element.
                                    c_ActiveItem = c_ActiveItem.OwnerItem;
                                    this.UpdateObjects();

                                    // The parent had some menu items, therefore we
                                    // enable it regardless of whether it has an action.
                                    if (c_ActiveItem != null)
                                        c_ActiveItem.Enabled = true;
                                }
                                break;
                            case "text":
                                this.UpdateObjects();

                                if (c_ActiveComboBox != null)
                                {
                                    Type enumType = m_CurrentAssembly.GetType(this.m_Reader.GetAttribute("type"));
                                    if (enumType != null)
                                    {
                                        Array enumValues = Enum.GetValues(enumType);
                                        foreach (object o in enumValues)
                                        {
                                            String name = Enum.GetName(enumType, o);
                                            if (this.m_Reader.GetAttribute("type") + "." + name == this.m_Reader.GetAttribute("value"))
                                            {
                                                // HACK: This could probably be organised better by
                                                //       using classes instead of enums, but oh well..
                                                switch (this.m_Reader.GetAttribute("type"))
                                                {
                                                    case "MOAI.Compilation.BuildMode":
                                                        c_ActiveComboBox.Items.Add(
                                                            new EnumWrapper(
                                                                (Int32)o,
                                                                new List<String>() { "Debug", "Release" })
                                                            );
                                                        break;
                                                    default:
                                                        c_ActiveComboBox.Items.Add(o);
                                                        break;
                                                }
                                            }
                                        }
                                    }
                                }
                                break;
                        }
                        break;
                    case XmlNodeType.EndElement:
                        switch (this.m_Reader.Name)
                        {
                            case "menus":
                            case "menubar":
                            case "toolbar":
                                // Nothing to do here.
                                break;
                            case "menuitem":
                                c_ActiveItem = c_ActiveItem.OwnerItem;
                                this.UpdateObjects();

                                // The parent had some menu items, therefore we
                                // enable it regardless of whether it has an action.
                                if (c_ActiveItem != null && !(c_ActiveItem is ToolStripDropDownButton))
                                    c_ActiveItem.Enabled = true;
                                break;
                            case "toolitem":
                                c_ActiveItem = c_ActiveItem.OwnerItem;
                                this.UpdateObjects();

                                // The parent had some menu items, therefore we
                                // enable it regardless of whether it has an action.
                                if (c_ActiveItem != null)
                                    c_ActiveItem.Enabled = true;
                                break;
                            case "menuseperator":
                                c_ActiveItem = c_ActiveItem.OwnerItem;
                                this.UpdateObjects();

                                // The parent had some menu items, therefore we
                                // enable it regardless of whether it has an action.
                                if (c_ActiveItem != null && !(c_ActiveItem is ToolStripDropDownButton))
                                    c_ActiveItem.Enabled = true;
                                break;
                            case "toolseperator":
                                c_ActiveItem = c_ActiveItem.OwnerItem;
                                this.UpdateObjects();

                                // The parent had some menu items, therefore we
                                // enable it regardless of whether it has an action.
                                if (c_ActiveItem != null)
                                    c_ActiveItem.Enabled = true;
                                break;
                        }
                        break;
                }
            }

            this.m_Reader.Close();
        }

        private void AddMenuItem(String text)
        {
            if (c_ActiveItem == null)
            {
                c_ActiveItem = this.MainMenu.Items.Add(text);
                this.UpdateObjects();
            }
            else if (c_ActiveDropDown != null)
            {
                // Add to the drop down.
                c_ActiveItem = c_ActiveDropDown.DropDownItems.Add(text);
                this.UpdateObjects();
            }
            else
            {
                c_ActiveItem = c_ActiveMenuItem.DropDownItems.Add(text);
                this.UpdateObjects();
            }
        }

        private void AddToolItem(String text)
        {
            c_ActiveItem = this.ToolBar.Items.Add(text);
            c_ActiveItem.TextImageRelation = TextImageRelation.ImageAboveText;
            c_ActiveItem.Text = "";
            c_ActiveItem.Image = Properties.Resources.tools_unknown;
            this.UpdateObjects();
        }

        private void AddToolComboBox(String text)
        {
            c_ActiveComboBox = new ToolStripComboBox(text);
            c_ActiveItem = c_ActiveComboBox;
            c_ActiveItem.TextImageRelation = TextImageRelation.ImageAboveText;
            c_ActiveItem.Text = "";
            c_ActiveItem.Image = null;
            this.ToolBar.Items.Add(c_ActiveComboBox);
            this.UpdateObjects();
        }

        private void AddToolDropDown(String text)
        {
            c_ActiveDropDown = new ToolStripDropDownButton(text);
            c_ActiveItem = c_ActiveDropDown;
            c_ActiveItem.TextImageRelation = TextImageRelation.ImageAboveText;
            c_ActiveItem.Text = "";
            c_ActiveItem.Image = Properties.Resources.tools_unknown;
            this.ToolBar.Items.Add(c_ActiveDropDown);
            this.UpdateObjects();
        }

        private void AddReflectionHandler(String actionName)
        {
            if (actionName != null)
            {
                // Use reflection to associate the menu item with a defined
                // action.
                Type actionType = m_CurrentAssembly.GetType("MOAI.Menus.Definitions." + actionName);
                if (actionType != null)
                {
                    object actionObj = Activator.CreateInstance(actionType);
                    if (actionObj is Action)
                    {
                        Action action = (Action)actionObj;
                        String resString = action.Text;
                        if (resString != null)
                        {
                            if (c_ActiveMenuItem != null)
                                c_ActiveItem.Text = resString;
                            else
                                c_ActiveItem.ToolTipText = resString;
                        }
                        action.SetItem(this.c_ActiveMenuItem, this.c_ActiveItem);
                        if (this.c_ActiveMenuItem != null)
                        {
                            this.c_ActiveMenuItem.ShortcutKeys = action.Shortcut;
                            this.c_ActiveMenuItem.ShowShortcutKeys = true;
                        }
                        c_ActiveItem.Click += new EventHandler(delegate(object sender, EventArgs e)
                            {
                                action.OnActivate();
                            }
                        );
                        this.m_Manager.AssociateEvents(action);
                        //this.m_Manager.Parent.IDEWindow.MenuActions.Add(action);

                        action.OnSetSettings();
                        c_ActiveItem.Enabled = action.Enabled;
                        if (action.ItemIcon != null)
                            c_ActiveItem.Image = action.ItemIcon;
                        action.OnLoad();
                    }
                    else
                        c_ActiveItem.Enabled = false;
                }
                else
                    c_ActiveItem.Enabled = false;
            }
            else
                c_ActiveItem.Enabled = false;
        }

        private void UpdateObjects()
        {
            if (c_ActiveItem is ToolStripMenuItem)
                c_ActiveMenuItem = (ToolStripMenuItem)c_ActiveItem;
            else
                c_ActiveMenuItem = null;
            
            if (c_ActiveItem is ToolStripDropDownButton)
                c_ActiveDropDown = (ToolStripDropDownButton)c_ActiveItem;
            else
                c_ActiveDropDown = null;

            if (c_ActiveItem is ToolStripComboBox)
                c_ActiveComboBox = (ToolStripComboBox)c_ActiveItem;
            else
                c_ActiveComboBox = null;                     
        }
    }
}
