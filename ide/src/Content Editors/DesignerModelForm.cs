using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using IrrlichtNETCP;
using IrrlichtNETCP.Inheritable;
using System.Threading;
using DockPanelSuite;
using Roket3D.ContentEditors.Classes;

namespace Roket3D.ContentEditors
{
    public partial class DesignerModelForm : EditorBase
    {
        public IDE MainForm;
        private IrrlichtDevice p_Device;
        private Rect p_Dimensions;
        private Boolean p_DeviceClosed = false;
        private Boolean p_IsActive = true;
        private IntPtr p_WindowHandle;
        private Thread p_DeviceThread = null;
        private AnimatedMeshSceneNode p_Node;
        private GUIStaticText p_ModelInformation;
        private Model p_Model = null;
        private Boolean p_IsRotating = false;
        private Double p_XRotate = 0;
        private Double p_YRotate = 0;
        private CameraSceneNode p_Camera;
        private Point p_OriginalPoint = new Point(0, 0);
        private String m_Texture = "";

        public DesignerModelForm(IDE main)
        {
            InitializeComponent();
            this.MainForm = main;
        }

        private void DesignerModelForm_Load(object sender, EventArgs e)
        {
            this.p_WindowHandle = this.Handle;

            // Create the model information object.
            this.p_Model = new Model();
            this.p_Model.SetInitial("Unknown", "Unknown");
            this.p_Model.SetModelInformation(4, 5, 2);
            this.p_Model.SetTextureInformation(512, 512);
            this.PropertyEditor.SelectedObject = this.p_Model;

            /*
            this.textureToolStripComboBox.Items.Clear();
            this.textureToolStripComboBox.Items.AddRange(this.MainForm.GetAllFilesOfType(Xml.FileType.Image).ToArray());
            if (this.textureToolStripComboBox.Items.Count == 0)
                this.textureToolStripComboBox.Enabled = false;
            else
            {
                try
                {
                    String tex = this.File.Settings["texture"];
                    Roket3D.Xml.File imageFile = null;
                    foreach (Roket3D.Xml.File f in this.textureToolStripComboBox.Items)
                    {
                        if (f.ToString() == tex)
                        {
                            imageFile = f;
                        }
                    }
                    if (imageFile == null)
                    {
                        // The image file currently associated with the model is not
                        // currently in the project.  Here we should prompt the user
                        // to add it to the project.
                        //
                        // TODO: Implement add-to-project prompt.
                        this.textureToolStripComboBox.Items.Insert(0, "<image not in project>");
                        this.textureToolStripComboBox.SelectedIndex = 0;
                        m_Texture = tex;
                    }
                    else
                    {
                        // Just select the correct index for the combo box.
                        this.textureToolStripComboBox.SelectedItem = imageFile;
                        m_Texture = this.textureToolStripComboBox.SelectedItem.ToString();
                    }
                }
                catch (KeyNotFoundException)
                {
                    this.textureToolStripComboBox.SelectedIndex = 0;
                    m_Texture = this.textureToolStripComboBox.SelectedItem.ToString();
                }
            }*/
        }

        private void DesignerModelForm_Shown(object sender, EventArgs e)
        {
            //GenerateDevice();

            p_DeviceThread = new Thread(new ThreadStart(RunIrrlichtLoop));
            p_DeviceThread.Start();
        }

        private void textureToolStripComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Change the texture.
            /*
            if (m_Texture != "") // when the index is first begin set, this will still be "".  Since
                                          // we dont want to prompt the user when the first open the model
                                          // this prevents the message from showing.
            {
                if (textureToolStripComboBox.Items[0].ToString() == "<image not in project>" &&
                    textureToolStripComboBox.SelectedIndex == 0)
                {
                    MessageBox.Show("Please select a valid texture.", "Invalid Texture", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
                else if (textureToolStripComboBox.Items[0].ToString() == "<image not in project>")
                {
                    textureToolStripComboBox.Items.RemoveAt(0);
                }
            }

            if (p_Device != null)
            {
                p_Device.FileSystem.WorkingDirectory = Environment.CurrentDirectory;
                p_Node.SetMaterialTexture(0, p_Device.VideoDriver.GetTexture(
                    textureToolStripComboBox.SelectedItem.ToString()
                    ));
                m_Texture = textureToolStripComboBox.SelectedItem.ToString();
            }
            */
        }

        private void RegenerateDevice()
        {
            // If the device is null, then it hasn't been
            // set up at all yet.
            if (p_Device != null)
            {
                // Close the existing device.
                p_DeviceClosed = true;
                p_DeviceThread.Join();

                p_DeviceClosed = false;
                this.Refresh();

                //GenerateDevice();

                p_DeviceThread = new Thread(new ThreadStart(RunIrrlichtLoop));
                p_DeviceThread.Start();
            }
        }

        private void GenerateDevice()
        {
            p_Device = new IrrlichtDevice(DriverType.Direct3D9,
                                            new Dimension2D(640, 480),
                                            32, false, true, true, true, this.p_WindowHandle);
            this.p_Dimensions = new Rect(0, 0, this.Width, this.Height);

            /*Now we set the caption of the window to some nice text. Note that there is a 'L' in front of the string: the Irrlicht Engine uses
wide character strings when displaying text.
            */
            p_Device.WindowCaption = "Hello World! - Irrlicht Engine Demo";
            p_Device.FileSystem.WorkingDirectory = Environment.CurrentDirectory;

            //
            VideoDriver driver = p_Device.VideoDriver;
            SceneManager smgr = p_Device.SceneManager;
            GUIEnvironment guienv = p_Device.GUIEnvironment;

            p_ModelInformation = guienv.AddStaticText("Hello World! This is the Irrlicht Software engine!",
                new Rect(new Position2D(10, 36), new Dimension2D(500, 16)), true, false, guienv.RootElement, -1, true);

            // We need to write the irrlicht font to the temporary folder so that the
            // engine can read it.
            if (!System.IO.Directory.Exists(System.IO.Path.GetTempPath() + "\\Roket3D"))
                System.IO.Directory.CreateDirectory(System.IO.Path.GetTempPath() + "\\Roket3D");
            Roket3D.Properties.Resources.irrlicht_font.Save(System.IO.Path.GetTempPath() + "\\Roket3D\\irrlicht_font.png");
            p_ModelInformation.OverrideFont = guienv.GetFont(System.IO.Path.GetTempPath() + "\\Roket3D\\irrlicht_font.png");
            System.IO.File.Delete(System.IO.Path.GetTempPath() + "\\Roket3D\\irrlicht_font.png");

            //
            AnimatedMesh mesh = smgr.GetMesh(this.Path);
            p_Node = smgr.AddAnimatedMeshSceneNode(mesh);
            //node.SetMaterialFlag(MaterialFlag.NormalizeNormals, false);
            //node.SetMaterialFlag(MaterialFlag.PointCloud, false);
            //node.SetMaterialFlag(MaterialFlag.Wireframe, false);
            //node.SetMaterialFlag(MaterialFlag.BackFaceCulling, false);
            //node.SetMaterialFlag(MaterialFlag.ZBuffer, false);
            //node.SetMaterialFlag(MaterialFlag.ZWriteEnable, false);
            //node.SetMaterialFlag(MaterialFlag.TrilinearFilter, false);
            //MessageBox.Show(((Int32)MaterialFlag.TrilinearFilter).ToString());
            //node.SetMaterialFlag(MaterialFlag.BilinearFilter, false);
            //node.SetMaterialFlag(MaterialFlag.AnisotropicFilter, false);
            //node.SetMaterialFlag(MaterialFlag.FogEnable, false);
            //node.SetMaterialFlag(MaterialFlag.GouraudShading, false);
            //node.SetMaterialFlag(MaterialFlag.TextureWrap, false);
            //MessageBox.Show(((Int32)MaterialFlag.MaterialFlagCount).ToString());

            //
            if (p_Node != null)
            {
                p_Node.SetMaterialFlag(MaterialFlag.Lighting, false);
                p_Node.SetMaterialTexture(0, driver.GetTexture(m_Texture));
            }
            //
            p_Camera = smgr.AddCameraSceneNode(smgr.RootSceneNode);
            //cam.Position = new Vector3D(0, 30, -40);
            p_Camera.Target = new Vector3D(0, 0, 0);

            float newY = (float)Math.Sin(this.p_YRotate * Math.PI / 180) * 80;
            float dist = (float)Math.Cos(this.p_YRotate * Math.PI / 180) * 80;

            float newX = (float)Math.Sin(this.p_XRotate * Math.PI / 180) * dist;
            float newZ = (float)Math.Cos(this.p_XRotate * Math.PI / 180) * dist;

            p_Camera.Position = new Vector3D(newX, newY, newZ);
        }

        public override void OnActiveTabChanged()
        {
            this.CheckIfActive();
        }

        private void CheckIfActive()
        {
            this.p_IsActive = (this.MainForm.DockWorkspace.ActiveTab == this);

            if (!this.p_IsActive)
            {
                if (!p_DeviceClosed)
                {
                    p_DeviceClosed = true;
                    p_DeviceThread.Join();
                }
            }
            else
            {
                if (p_DeviceClosed)
                {
                    p_DeviceClosed = false;
                    GenerateDevice();

                    p_DeviceThread = new Thread(new ThreadStart(RunIrrlichtLoop));
                    p_DeviceThread.Start();
                }
            }
        }

        private void RunIrrlichtLoop()
        {
            GenerateDevice();
            
            while (p_Device.Run() && !p_DeviceClosed)
            {
                p_ModelInformation.Text = "Model Current Texture: " + m_Texture;

                p_Device.VideoDriver.BeginScene(true, true, new IrrlichtNETCP.Color(255, 100, 101, 140));
                p_Device.SceneManager.DrawAll();
                p_Device.GUIEnvironment.DrawAll();

                /*device.VideoDriver.Draw2DRectangle(
                        new Rect(
                            device.VideoDriver.ViewPort.X,
                            device.VideoDriver.ViewPort.Y,
                            device.VideoDriver.ViewPort.Width,
                            device.VideoDriver.ViewPort.Height
                            ),
                        IrrlichtNETCP.Color.From(255,244, 243, 241)
                        );*/

                p_Device.VideoDriver.EndScene();
            }

            p_Device.Dispose();
        }

        // This is the event reciever.
        /*private bool device_OnEvent(Event e)
        {
            // See if the mouse left cursor was just pressed.
            // If so, we should go into rotate model mode.
            if (e.Type == EventType.MouseInputEvent &&
                e.MouseInputEvent == MouseInputEvent.LMousePressedDown)
            {
                device.CursorControl.Visible = false;
                isRotating = true;
                originalPoint = e.MousePosition.dotNETPoint;
            }
            // If we are in rotate mode, move the model depending
            // on the mouse position.
            if (e.Type == EventType.MouseInputEvent &&
                isRotating &&
                e.MouseInputEvent != MouseInputEvent.LMouseLeftUp)
            {
                // Do the camera rotation here.
                this.xRotate += e.MousePosition.X - originalPoint.X;
                this.yRotate += e.MousePosition.Y - originalPoint.Y;

                cam.Position.SetLength(300);
                cam.Position.RotateXYBy(this.yRotate, cam.Target);
                cam.Position.RotateXZBy(this.xRotate, cam.Target);
            }
            // See if the mouse left cursor was released
            if (e.Type == EventType.MouseInputEvent &&
                e.MouseInputEvent == MouseInputEvent.LMouseLeftUp)
            {
                device.CursorControl.Visible = true;
                isRotating = false;
            }

            return false;
        }*/

        private void DesignerModelForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!p_DeviceClosed)
            {
                p_DeviceClosed = true;
                p_DeviceThread.Abort();
                p_Device.Close();
            }
        }

        public override void TerminateThread()
        {
            if (!p_DeviceClosed)
            {
                p_DeviceClosed = true;
                p_DeviceThread.Abort();
                p_Device.Close();
            }
        }

        public override void OnResizeEnd()
        {
            if (this.p_Dimensions.Width != this.Width || this.p_Dimensions.Height != this.Height)
            {
                RegenerateDevice();
            }
        }

        private void DesignerModelForm_ResizeEnd(object sender, EventArgs e)
        {
            OnResizeEnd();
        }

        private void DesignerModelForm_MouseDown(object sender, MouseEventArgs e)
        {
            // See if the mouse left cursor was just pressed.
            // If so, we should go into rotate model mode.
            if (e.Button == MouseButtons.Left)
            {
                p_Device.CursorControl.Visible = false;
                p_IsRotating = true;
                p_OriginalPoint = e.Location;
            }
        }

        private void DesignerModelForm_MouseMove(object sender, MouseEventArgs e)
        {
            // If we are in rotate mode, move the model depending
            // on the mouse position.
            if (p_IsRotating)
            {
                // Do the camera rotation here.
                this.p_XRotate += e.X - p_OriginalPoint.X;
                this.p_YRotate += e.Y - p_OriginalPoint.Y;
                if (this.p_YRotate > 89) this.p_YRotate = 89;
                if (this.p_YRotate < -89) this.p_YRotate = -89;
                p_OriginalPoint = e.Location;
                
                float newY = (float)Math.Sin(this.p_YRotate * Math.PI / 180) * 80;
                float dist = (float)Math.Cos(this.p_YRotate * Math.PI / 180) * 80;

                float newX = (float)Math.Sin(this.p_XRotate * Math.PI / 180) * dist;
                float newZ = (float)Math.Cos(this.p_XRotate * Math.PI / 180) * dist;

                p_Camera.Position = new Vector3D(newX, newY, newZ);
            }
        }

        private void DesignerModelForm_MouseUp(object sender, MouseEventArgs e)
        {
            // See if the mouse left cursor was released
            if (e.Button == MouseButtons.Left)
            {
                p_Device.CursorControl.Visible = true;
                p_IsRotating = false;
            }
        }
    }
}
