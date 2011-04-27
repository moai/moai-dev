using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Web;

namespace MOAI.Collections
{
    public class Node
    {
        private Node p_Parent;
        private string p_Type;
        private string p_Name;
        private string p_Value;
        private bool p_WriteAsCDATA = false;
        private Dictionary<string, string> p_Attributes;
        private List<Node> p_Children = new List<Node>();

        /// <summary>
        /// Creates a new representation of a tree node.
        /// </summary>
        /// <param name="parent">The parent node of this node.</param>
        /// <param name="type">The type of this node, either 'element' or 'text'.</param>
        /// <param name="name">The name of this node (i.e. xml element type).</param>
        /// <param name="value">The value of this node (i.e. text data).</param>
        /// <param name="attributes">A dictionary of attributes associated with the node.</param>
        public Node(Node parent, string type, string name, string value, Dictionary<string, string> attributes)
        {
            if (attributes == null)
                throw new ArgumentException("The attributes argument must not be null.");

            this.p_Parent = parent;
            this.p_Type = type;
            this.p_Name = name;
            this.p_Value = value;
            this.p_Attributes = attributes;
        }

        /// <summary>
        /// Creates a new representation of a tree node.
        /// </summary>
        /// <param name="parent">The parent node of this node.</param>
        /// <param name="type">The type of this node, either 'element' or 'text'.</param>
        /// <param name="name">The name of this node (i.e. xml element type).</param>
        /// <param name="value">The value of this node (i.e. text data).</param>
        public Node(Node parent, string type, string name, string value)
        {
            this.p_Parent = parent;
            this.p_Type = type;
            this.p_Name = name;
            this.p_Value = value;
            this.p_Attributes = new Dictionary<string, string>();
        }

        /// <summary>
        /// Returns an XML representation of this node.
        /// </summary>
        /// <returns>An XML representation of this node.</returns>
        public string GetXml(string indent)
        {
            if (this.p_Type == "element")
            {
                string xml = indent;
                xml += "<" + this.p_Name;
                foreach (KeyValuePair<string, string> k in this.p_Attributes)
                {
                    xml += " " + k.Key + "=\"" + HttpUtility.HtmlEncode(k.Value) + "\"";
                }
                xml += ">\n";
                foreach (Node n in this.p_Children)
                    xml += n.GetXml(indent + "  ") + "\n";
                xml += indent + "</" + this.p_Name + ">";
                return xml;
            }
            else if (this.p_Type == "text")
            {
                string[] lines = this.p_Value.Split('\n');
                string text = "";
                for (int i = 0; i < lines.Length; i += 1)
                    lines[i] = indent + lines[i].Trim();
                for (int i = 0; i < lines.Length; i += 1)
                    if (lines[i] != string.Empty)
                        text += lines[i] + "\n";

                if (this.p_WriteAsCDATA)
                    return indent + "<![CDATA[\n" + text.Trim('\n') + "\n" + indent + "]]>";
                else
                    return HttpUtility.HtmlEncode(text).Trim('\n');
            }
            else
                return "";
        }

        /// <summary>
        /// The parent node of this node.
        /// </summary>
        public Node Parent
        {
            get
            {
                return this.p_Parent;
            }
        }

        /// <summary>
        /// The type of this node (string representation).
        /// </summary>
        public string Type
        {
            get
            {
                return this.p_Type;
            }
        }

        /// <summary>
        /// The name of this node (i.e. xml element type).
        /// </summary>
        public string Name
        {
            get
            {
                return this.p_Name;
            }
        }

        /// <summary>
        /// The value of this node (i.e. text data).
        /// </summary>
        public string Value
        {
            get
            {
                return this.p_Value;
            }
            set
            {
                this.p_Value = value;
            }
        }

        /// <summary>
        /// Whether the node should be written as CDATA.
        /// </summary>
        public bool WriteAsCDATA
        {
            get
            {
                return this.p_WriteAsCDATA;
            }
            set
            {
                this.p_WriteAsCDATA = value;
            }
        }

        /// <summary>
        /// The attributes associated with this node.
        /// </summary>
        public Dictionary<string, string> Attributes
        {
            get
            {
                return this.p_Attributes;
            }
        }

        /// <summary>
        /// A list of children of this node.
        /// </summary>
        public List<Node> Children
        {
            get
            {
                return this.p_Children;
            }
        }

        /// <summary>
        /// Returns a list of child XML elements whose XML type is equal to name.
        /// </summary>
        /// <param name="name">The XML type of the elements.  Use null to indicate all child elements.</param>
        /// <returns>A list of child XML elements.</returns>
        public List<Node> GetChildElements(string name)
        {
            List<Node> ns = new List<Node>();
            foreach (Node c in this.p_Children)
            {
                if (c.p_Type == "element" && (name == null || c.p_Name.ToLowerInvariant() == name.ToLowerInvariant()))
                    ns.Add(c);
            }
            return ns;
        }

        /// <summary>
        /// Returns the first child XML element whose XML type is equal to name.
        /// </summary>
        /// <param name="name">The XML type of the element.</param>
        /// <returns>The first child XML element, or null if there is none.</returns>
        public Node GetChildElement(string name)
        {
            foreach (Node c in this.p_Children)
            {
                if (c.p_Type == "element" && c.p_Name.ToLowerInvariant() == name.ToLowerInvariant())
                    return c;
            }
            return null;
        }

        /// <summary>
        /// Returns the first text child node within this node.
        /// </summary>
        /// <returns>The first child XML text element, or null if there is none.</returns>
        public Node GetText()
        {
            foreach (Node c in this.p_Children)
            {
                if (c.p_Type == "text" && c.p_Name == null)
                    return c;
            }
            return null;
        }
    }
}
