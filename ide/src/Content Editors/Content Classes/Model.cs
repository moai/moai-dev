using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace Roket3D.ContentEditors.Classes
{
    class Model
    {
        private string m_Filename = "";
        private long m_Vertexes = 0;
        private long m_Lines = 0;
        private long m_Faces = 0;

        private string m_AssociatedTexture = "";
        private long m_TextureWidth = 0;
        private long m_TextureHeight = 0;

        [Category("Model Information")]
        public string Filename
        {
            get { return this.m_Filename; }
        }

        [Category("Model Information")]
        public long Vertexes
        {
            get { return this.m_Vertexes; }
        }

        [Category("Model Information")]
        public long Lines
        {
            get { return this.m_Lines; }
        }

        [Category("Model Information")]
        public long Faces
        {
            get { return this.m_Faces; }
        }

        [Category("Texture Information")]
        public string Texture
        {
            get { return this.m_AssociatedTexture; }
            set { this.m_AssociatedTexture = value; }
        }

        [Category("Texture Information")]
        public long TextureWidth
        {
            get { return this.m_TextureWidth; }
        }

        [Category("Texture Information")]
        public long TextureHeight
        {
            get { return this.m_TextureHeight; }
        }

        public void SetTextureInformation(long width, long height)
        {
            this.m_TextureWidth = width;
            this.m_TextureHeight = height;
        }

        public void SetModelInformation(long vertexes, long lines, long faces)
        {
            this.m_Vertexes = vertexes;
            this.m_Lines = lines;
            this.m_Faces = faces;
        }

        public void SetInitial(string filename, string texture)
        {
            this.m_Filename = filename;
            this.m_AssociatedTexture = texture;
        }
    }
}
