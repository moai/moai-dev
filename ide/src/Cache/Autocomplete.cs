using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using MOAI.Collections;
using System.IO;
using System.Drawing;
using System.Windows.Forms;

namespace MOAI.Cache
{
    public class AutoComplete
    {
        private List<Item> p_Items = new List<Item>();

        /// <summary>
        /// The items in the global namespace.
        /// </summary>
        public List<Item> Items
        {
            get
            {
                return this.p_Items;
            }
        }

        /// <summary>
        /// A list of images to be used in the auto-complete list.
        /// </summary>
        public ImageList Images
        {
            get
            {
                ImageList ret = new ImageList();
                ret.ImageSize = new Size(16, 16);
                ret.ColorDepth = ColorDepth.Depth24Bit;
                ret.Images.Add(Properties.Resources.autocomplete_namespace);
                ret.Images.Add(Properties.Resources.autocomplete_class);
                return ret;
            }
        }

        /// <summary>
        /// Returns the list of items as a list of strings based on the current context.
        /// </summary>
        /// <param name="context">The context of the list, i.e. 'e.MyProperty' or null for global context.</param>
        /// <returns>A list of strings to be used in autocomplete.</returns>
        public List<string> GetList(string context)
        {
            if (context != null)
            {
                string[] ss = context.Split(new char[] { '.' });
                Item ii = null;
                bool first = true;
                foreach (string s in ss)
                {
                    bool found = false;
                    if (first)
                    {
                        // Search both our items and the local scope.
                        foreach (Item i in this.p_Items)
                        {
                            if (i.Name == s)
                            {
                                ii = i;
                                found = true;
                                break;
                            }
                        }

                        if (!found)
                            return null;

                        first = false;
                        continue;
                    }

                    // Search the current item's list for the subitem.
                    foreach (Item i in ii.Items)
                    {
                        if (i.Name == s)
                        {
                            ii = i;
                            found = true;
                            break;
                        }
                    }

                    if (!found)
                        return null;
                }

                List<string> ret = new List<string>();
                foreach (Item i in ii.Items)
                {
                    if (i is Namespace)
                        ret.Add(i.Name + "?0");
                    else if (i is Class)
                        ret.Add(i.Name + "?1");
                    else
                        ret.Add(i.Name);
                }
                return ret;
            }
            else
            {
                List<string> ret = new List<string>();
                foreach (Item i in this.p_Items)
                {
                    if (i is Namespace)
                        ret.Add(i.Name + "?0");
                    else if (i is Class)
                        ret.Add(i.Name + "?1");
                    else
                        ret.Add(i.Name);
                }
                return ret;
            }
        }

        /// <summary>
        /// Imports the list of items from the XML root document.
        /// </summary>
        public void Import()
        {
            Tree t = null;
            using (XmlReader reader = XmlReader.Create(Program.Manager.Settings["RootPath"] + "\\Completion Cache\\root.xml"))
            {
                t = Tree.FromXml(reader);
            }

            if (t == null)
                // TODO: Add some error handling here.
                return;

            List<Node> items = t.GetChildElement("cache").GetChildElements(null);
            foreach (Node i in items)
            {
                if (i.Name == "namespace")
                    this.p_Items.Add(new Namespace(i.GetText().Value.Trim()));
                else if (i.Name == "class")
                    this.p_Items.Add(new Class(i.GetText().Value.Trim()));
                else if (i.Name == "function")
                    this.p_Items.Add(new Function(i.GetText().Value.Trim()));
            }
        }
    }

    public class Item
    {
        private List<Item> p_Items = new List<Item>();
        private string p_Name = null;

        /// <summary>
        /// Base constructor for item, accepting the name of the item as input.
        /// The base constructor will automatically read all of the subitems from
        /// the appropriate XML file.
        /// </summary>
        /// <param name="name"></param>
        protected Item(string name)
        {
            this.p_Name = name;

            this.Setup("");
        }

        /// <summary>
        /// Base constructor for item, accepting the name of the item and container
        /// namespace as input.
        /// </summary>
        /// <param name="name"></param>
        protected Item(string name, string bas)
        {
            this.p_Name = name;

            this.Setup(bas + ".");
        }

        /// <summary>
        /// Initalizes the item.
        /// </summary>
        private void Setup(string bas)
        {
            Tree t = null;
            try
            {
                using (XmlReader reader = XmlReader.Create(Program.Manager.Settings["RootPath"] + "\\Completion Cache\\" + bas + this.p_Name + ".xml"))
                {
                    t = Tree.FromXml(reader);
                }
            }
            catch (IOException)
            {
                // File does not exist, so this item doesn't have any sub-items.
                return;
            }

            if (t == null)
                // TODO: Add some error handling here.
                return;

            List<Node> items = t.GetChildElement("cache").GetChildElements(null);
            foreach (Node i in items)
            {
                if (i.Name == "namespace")
                    this.p_Items.Add(new Namespace(i.GetText().Value.Trim(), this.Name));
                else if (i.Name == "class")
                    this.p_Items.Add(new Class(i.GetText().Value.Trim(), this.Name));
                else if (i.Name == "function")
                    this.p_Items.Add(new Function(i.GetText().Value.Trim(), this.Name));
            }
        }

        /// <summary>
        /// The name of this item to be displayed in the autocomplete list.
        /// </summary>
        public string Name
        {
            get
            {
                return this.p_Name;
            }
            protected set
            {
                this.p_Name = value;
            }
        }

        /// <summary>
        /// The sub-items of this item.
        /// </summary>
        public List<Item> Items
        {
            get
            {
                return this.p_Items;
            }
        }
    }

    public class Namespace : Item
    {
        public Namespace(string name)
            : base(name)
        {
        }

        public Namespace(string name, string bas)
            : base(name, bas)
        {
        }
    }

    public class Class : Item
    {
        public Class(string name)
            : base(name)
        {
        }

        public Class(string name, string bas)
            : base(name, bas)
        {
        }
    }

    public class Function : Item
    {
        public Function(string name)
            : base(name)
        {
        }

        public Function(string name, string bas)
            : base(name, bas)
        {
        }
    }
}
