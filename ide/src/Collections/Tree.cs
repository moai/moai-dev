using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Collections;

namespace MOAI.Collections
{
    class Tree : Node
    {
        private Tree()
            : base(null, null, null, null)
        {
        }

        internal static Tree FromXml(XmlReader reader)
        {
            Tree t = new Tree();
            Stack<Node> stack = new Stack<Node>();
            stack.Push(t); // Set the tree as the top node for parenting.

            while (reader.Read())
            {
                switch (reader.NodeType)
                {
                    case XmlNodeType.Element:
                        Dictionary<string, string> attrs = new Dictionary<string, string>();
                        string name = reader.Name;
                        if (reader.MoveToFirstAttribute())
                        {
                            attrs.Add(reader.Name, reader.Value);
                            while (reader.MoveToNextAttribute())
                                attrs.Add(reader.Name, reader.Value);
                        }
                        stack.Push(new Node(stack.Peek(), "element", name, null, attrs));
                        break;
                    case XmlNodeType.Text:
                    case XmlNodeType.CDATA:
                        Node n = new Node(stack.Peek(), "text", null, reader.Value.Trim(new char[] { ' ', '\n', '\r'}));
                        n.WriteAsCDATA = (reader.NodeType == XmlNodeType.CDATA);
                        stack.Peek().Children.Add(n);
                        break;
                    case XmlNodeType.EndElement:
                        Node ne = stack.Pop();
                        stack.Peek().Children.Add(ne);
                        break;
                }
            }

            return t;
        }

        internal string ToXml()
        {
            string xml = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n";

            foreach (Node n in this.Children)
                xml += n.GetXml("");

            return xml;
        }

        internal static Tree FromEmpty()
        {
            Tree t = new Tree();
            Stack<Node> stack = new Stack<Node>();
            stack.Push(t); // Set the tree as the top node for parenting.

            return t;
        }
    }
}
