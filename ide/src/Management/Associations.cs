using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.IO;
using System.Windows.Forms;
using MOAI.Properties;

namespace MOAI.Management
{
    public static class Associations
    {
        private static ImageList p_ImageList = new ImageList();
        private static HandlerDictionary p_Handlers = new HandlerDictionary
            {
                { "rs", "Script", typeof(Designers.Code.Designer) },
                { "rks", "Script", typeof(Designers.Code.Designer) },
                { "lua", "Script", typeof(Designers.Code.Designer) },
                { "xml", "XML", typeof(Designers.Code.Designer) },
                { "rkw", "World", null },
                { "rka", "Area", null },
                { "ogg", "Audio", null },
                { "png", "Image", null },
                { "bmp", "Image", null },
                { "jpg", "Image", null },
                { "tga", "Image", null },
                { "pcx", "Image", null },
                { "psd", "Image", null },
                { "3ds", "Model", null },
                { "b3d", "Model", null },
                { "csm", "Model", null },
                { "dae", "Model", null },
                { "dmf", "Model", null },
                { "x", "Model", null },
                { "obj", "Model", null },
                { "my3d", "Model", null },
                { "oct", "Model", null },
                { "mesh", "Model", null },
                { "lmts", "Model", null },
                { "bsp", "Model", null },
                { "md2", "Model", null },
                { "md3", "Model", null },
                { "rkm", "Model", null },
                { "c", "CScript", null },
                { "cc", "CScript", null },
                { "cpp", "CScript", null },
                { "h", "CHeader", null },
                { "hpp", "CHeader", null },
                { "folder", "Folder", null }
            };

        private static Bitmap m_ReprImage = Properties.Resources.image;

        /// <summary>
        /// Initalizes the static image list.
        /// </summary>
        static Associations()
        {
            Associations.p_ImageList.Images.Add("Solution", Resources.solution);
            Associations.p_ImageList.Images.Add("Project", Resources.project);
            Associations.p_ImageList.Images.Add("Model", Resources.model);
            Associations.p_ImageList.Images.Add("Folder", Resources.folder);
            Associations.p_ImageList.Images.Add("Audio", Resources.audio);
            Associations.p_ImageList.Images.Add("Area", Resources.area);
            Associations.p_ImageList.Images.Add("Image", Resources.image);
            Associations.p_ImageList.Images.Add("ProjectReference", Resources.projectreference);
            Associations.p_ImageList.Images.Add("Reference", Resources.reference);
            Associations.p_ImageList.Images.Add("Script", Resources.script);
            Associations.p_ImageList.Images.Add("CHeader", Resources.cheader);
            Associations.p_ImageList.Images.Add("CScript", Resources.cscript);
            Associations.p_ImageList.Images.Add("World", Resources.world);
            Associations.p_ImageList.Images.Add("NotFound", Resources.not_found);
            Associations.p_ImageList.Images.Add("NotFoundFolder", Resources.not_found_folder);
        }

        /// <summary>
        /// Handles requests to open image files by opening the appropriate
        /// designer.
        /// </summary>
        /// <param name="manager"></param>
        /// <param name="path"></param>
        public static void ImageHandler(MOAI.Manager manager, string path)
        {

        }

        /// <summary>
        /// A public function which provides access to the image representations
        /// for extensions.  Returns null if there is no appropriate representation.
        /// </summary>
        /// <param name="ext">The extension to fetch the representation for.</param>
        public static Image GetImageData(string ext)
        {
            foreach (KeyValuePair<string, HandlerDetails> h in Associations.p_Handlers)
                if (ext.ToLowerInvariant() == h.Key.ToLowerInvariant())
                    return Associations.ImageList.Images[h.Value.ImageKey];

            return null;
        }

        /// <summary>
        /// A public function which provides access to the image keys for situations
        /// which involve the use of the ImageList property to access images.  Returns
        /// null if there is no appropriate representation.
        /// </summary>
        /// <param name="ext">The extension to fetch the representation for.</param>
        public static string GetImageKey(string ext)
        {
            foreach (KeyValuePair<string, HandlerDetails> h in Associations.p_Handlers)
                if (ext.ToLowerInvariant() == h.Key.ToLowerInvariant())
                    return h.Value.ImageKey;

            return null;
        }

        /// <summary>
        /// A public function which returns the type of designer for this filetype.
        /// Returns null if there is no appropriate representation.
        /// </summary>
        public static Type GetDesignerType(string ext)
        {
            foreach (KeyValuePair<string, HandlerDetails> h in Associations.p_Handlers)
                if (ext.ToLowerInvariant() == h.Key.ToLowerInvariant())
                    return h.Value.DesignerType;

            return null;
        }

        /// <summary>
        /// The static image list.
        /// </summary>
        public static ImageList ImageList
        {
            get
            {
                return Associations.p_ImageList;
            }
        }
    }

    /// <summary>
    /// A derived class that allows the handler dictionary to specify both the key, handler
    /// and the image representation of the type (for use in solution explorer)
    /// </summary>
    public class HandlerDictionary : Dictionary<string, HandlerDetails>
    {
        public void Add(string key, string image, Type designer)
        {
            base.Add(key, new HandlerDetails(image, designer));
        }
    }

    public struct HandlerDetails
    {
        public Type DesignerType;
        public string ImageKey;

        public HandlerDetails(string image, Type designer)
        {
            this.DesignerType = designer;
            this.ImageKey = image;
        }
    }
}
