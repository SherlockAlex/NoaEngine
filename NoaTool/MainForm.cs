using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static NoaTool.Animator;
using static NoaTool.Sprite;

namespace NoaTool
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();

            // 设置 DataGridView 的属性
            animatorFrameGridView.AllowDrop = true;
            animatorFrameGridView.SelectionMode = DataGridViewSelectionMode.FullRowSelect;
            animatorFrameGridView.MouseDown += dataGridView_MouseDown;
            animatorFrameGridView.MouseMove += dataGridView_MouseMove;
            animatorFrameGridView.DragOver += dataGridView_DragOver;
            animatorFrameGridView.DragDrop += dataGridView_DragDrop;

            animatorFrameGridView.SelectionChanged += AnimatorDataGridView_SelectionChanged;

        }

        DataGridViewRow selectedRow;
        string currentImagePath;

        private void ShowImage(string filePath)
        {
            //显示图片到pictureBox中
            try
            {
                currentPictureBox.SizeMode = PictureBoxSizeMode.Zoom;
                FileStream fileStream = new FileStream(currentImagePath, FileMode.Open, FileAccess.Read);
                currentPictureBox.Image = Image.FromFile(filePath);
            }catch
            {

            }
            
        }

        private void openFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //打开文件
            //读取图片
            string fileName;
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
                openFileDialog.ShowDialog();                //显示打开本地文件的窗口
                fileName = openFileDialog.FileName;
                currentImagePath = fileName;
                //inputFileTextBox.Text = fileName;
            }

            ShowImage(fileName);

        }

        private void newSprToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //保存贴图文件
            string fileName;
            using (SaveFileDialog saveFileDialog = new SaveFileDialog())
            {
                saveFileDialog.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
                saveFileDialog.Filter = "*.spr|spr file";
                saveFileDialog.DefaultExt = ".spr";
                saveFileDialog.FileName = "sprite";
                saveFileDialog.ShowDialog();
                fileName = saveFileDialog.FileName;
            }

            //保存贴图
            Bitmap bmp = new Bitmap(currentImagePath);
            List<uint> image = Sprite.ReadImageRGB(currentImagePath);
            SpriteFile spriteFile;
            spriteFile.images = image;
            spriteFile.width = bmp.Width;
            spriteFile.height = bmp.Height;
            spriteFile.x = ((int)spritePosXNumericUpDown.Value);
            spriteFile.y = ((int)spritePosYNumericUpDown.Value);
            bmp.Dispose();




            if (image == null)
            {
                MessageBox.Show("保存失败");
                return;
            }
            int success = Sprite.Save(spriteFile, fileName);
            if (success == 0)
            {
                MessageBox.Show("保存成功");
                return;
            }
            MessageBox.Show("保存失败");
        }

        private void newAmtToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //保存动画文件
            
            //文件名
            string fileName;
            using (SaveFileDialog saveFileDialog = new SaveFileDialog())
            {
                saveFileDialog.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
                saveFileDialog.Filter = "*.amt|animator file";
                saveFileDialog.DefaultExt = ".amt";
                saveFileDialog.FileName = "animator";
                saveFileDialog.ShowDialog();
                fileName = saveFileDialog.FileName;
            }

            //List<List<uint>> data= new List<List<uint>>();
            //for (int i = 0;i<animatorFrameGridView.RowCount-1;i++)
            //{
            //    string imagePath = animatorFrameGridView.Rows[i].Cells[0].Value as string;
            //    List<uint> frame = Sprite.ReadImageRGB(imagePath);
            //    data.Add(frame);
            //}

            AnimatorFile animatorFile = new AnimatorFile();
            animatorFile.data = new List<SpriteFile>();
            for (int i = 0;i<animatorFrameGridView.RowCount-1;i++)
            {
                string imagePath = animatorFrameGridView.Rows[i].Cells[0].Value as string;
                
                Bitmap bmp = new Bitmap(imagePath);
                List<uint> image = Sprite.ReadImageRGB(imagePath);
                SpriteFile spriteFile;
                spriteFile.images = image;
                spriteFile.width = bmp.Width;
                spriteFile.height = bmp.Height;
                spriteFile.x = ((int)spritePosXNumericUpDown.Value);
                spriteFile.y = ((int)spritePosYNumericUpDown.Value);

                animatorFile.data.Add(spriteFile);
                animatorFile.posx = spriteFile.x;
                animatorFile.posy = spriteFile.y;
                animatorFile.w = bmp.Width;
                animatorFile.h = bmp.Height;

                bmp.Dispose();

            }

            //if (animatorFile == null)
            //{
            //    MessageBox.Show("保存失败");
            //    return;
            //}

            int success = Animator.Save(animatorFile, fileName);
            if (success == 0)
            {
                MessageBox.Show("保存成功");
                return;
            }
            MessageBox.Show("保存失败");

            
        }

        private void addFrameButton_Click(object sender, EventArgs e)
        {
            //添加动画帧
            //打开文件
            //读取图片
            string fileName=null;
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
                openFileDialog.ShowDialog();                    //显示打开本地文件的窗口
                fileName = openFileDialog.FileName;
                currentImagePath = fileName;
                if (fileName == "")
                {
                    return;
                }
                //添加到gridview中
                DataGridViewTextBoxColumn dataColumn = new DataGridViewTextBoxColumn();
                animatorFrameGridView.Columns.Add(dataColumn);
                animatorFrameGridView.Columns[0].ReadOnly = true;
                int index = animatorFrameGridView.Rows.Add();
                animatorFrameGridView.Rows[index].Cells[0].Value = fileName;

                //保存路径信息到animatorFrameGridView中
                //inputFileTextBox.Text = fileName;
            }

            //显示图片到pictureBox中
            if (fileName==null)
            {
                return;
            }
            ShowImage(fileName);
        }

        private void removeFrameButton_Click(object sender, EventArgs e)
        {
            //删除动画帧
            //删除选中的单元格
            if (selectedRow==null)
            {
                return;
            }
            animatorFrameGridView.Rows.Remove(selectedRow);
        }

        private void SaveFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //保存工程文件
        }

        private void AnimatorDataGridView_SelectionChanged(object sender, EventArgs e)
        {
            selectedRow = animatorFrameGridView.CurrentRow;
            if (selectedRow == null)
            {
                return;
            }
            if (selectedRow.Cells[0].Value==null)
            {
                return;
            }
            ShowImage(selectedRow.Cells[0].Value as string);
        }

        //实现拖动单元格排序
        private void dataGridView_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                // 获取鼠标按下的行索引
                int rowIndex = animatorFrameGridView.HitTest(e.X, e.Y).RowIndex;

                // 开始拖动操作
                if (rowIndex >= 0)
                {
                    DataGridViewRow row = animatorFrameGridView.Rows[rowIndex];
                    animatorFrameGridView.DoDragDrop(row, DragDropEffects.Move);
                }
            }
        }

        private void dataGridView_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                // 检查拖动操作是否开始
                if (animatorFrameGridView.SelectedRows.Count > 0)
                {
                    // 设置拖动效果为移动
                    animatorFrameGridView.DoDragDrop(animatorFrameGridView.SelectedRows[0], DragDropEffects.Move);
                }
            }
        }

        private void dataGridView_DragOver(object sender, DragEventArgs e)
        {
            // 获取鼠标拖动过程中的位置
            Point mouseOverLocation = animatorFrameGridView.PointToClient(new Point(e.X, e.Y));

            // 获取目标行的索引
            int targetRowIndex = animatorFrameGridView.HitTest(mouseOverLocation.X, mouseOverLocation.Y).RowIndex;

            // 显示插入标记
            if (targetRowIndex >= 0)
            {
                animatorFrameGridView.Rows[targetRowIndex].DefaultCellStyle.BackColor = Color.Gray;
            }

            // 确定拖放效果为移动
            e.Effect = DragDropEffects.Move;
        }

        private void dataGridView_DragDrop(object sender, DragEventArgs e)
        {
            // 获取拖动的行
            DataGridViewRow draggedRow = (DataGridViewRow)e.Data.GetData(typeof(DataGridViewRow));

            // 获取插入标记的索引
            int targetRowIndex = animatorFrameGridView.HitTest(animatorFrameGridView.PointToClient(new Point(e.X, e.Y)).X, animatorFrameGridView.PointToClient(new Point(e.X, e.Y)).Y).RowIndex;

            // 如果目标行索引有效，则移动行
            if (targetRowIndex >= 0)
            {
                // 插入到新位置
                animatorFrameGridView.Rows.Remove(draggedRow);
                animatorFrameGridView.Rows.Insert(targetRowIndex, draggedRow);
            }

            // 恢复颜色
            foreach (DataGridViewRow row in animatorFrameGridView.Rows)
            {
                row.DefaultCellStyle.BackColor = Color.White;
            }
        }

        private void newMapFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //将图片导出

            //保存贴图文件
            string fileName;
            using (SaveFileDialog saveFileDialog = new SaveFileDialog())
            {
                saveFileDialog.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
                saveFileDialog.Filter = "*.map|map file";
                saveFileDialog.DefaultExt = ".map";
                saveFileDialog.FileName = "level";
                saveFileDialog.ShowDialog();
                fileName = saveFileDialog.FileName;
            }

            if (fileName=="")
            {
                MessageBox.Show("保存失败");
                return;
            }

            int success = Level.ExportMap(currentImagePath,fileName);

            if (success == 0)
            {
                MessageBox.Show("保存成功");
                return;
            }
            MessageBox.Show("保存失败");

            
        }

        private void newLevelFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //将图片导出

            //保存贴图文件
            string fileName;
            using (SaveFileDialog saveFileDialog = new SaveFileDialog())
            {
                saveFileDialog.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
                saveFileDialog.Filter = "*.map|map file";
                saveFileDialog.DefaultExt = ".map";
                saveFileDialog.FileName = "level";
                saveFileDialog.ShowDialog();
                fileName = saveFileDialog.FileName;
            }

            if (fileName == "")
            {
                MessageBox.Show("保存失败");
                return;
            }

            int success = Level.ExportMap(currentImagePath, fileName);

            if (success == 0)
            {
                MessageBox.Show("保存成功");
                return;
            }
            MessageBox.Show("保存失败");

        }
    }
}
