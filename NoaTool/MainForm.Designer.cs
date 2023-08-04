namespace NoaTool
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openFileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.SaveFileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.导出ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newAmtToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newSprToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.removeFrameButton = new System.Windows.Forms.Button();
            this.addFrameButton = new System.Windows.Forms.Button();
            this.animatorFrameGridView = new System.Windows.Forms.DataGridView();
            this.currentPictureBox = new System.Windows.Forms.PictureBox();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.animatorFrameGridView)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.currentPictureBox)).BeginInit();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1010, 28);
            this.menuStrip1.TabIndex = 5;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openFileToolStripMenuItem,
            this.SaveFileToolStripMenuItem,
            this.导出ToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(53, 24);
            this.fileToolStripMenuItem.Text = "文件";
            // 
            // openFileToolStripMenuItem
            // 
            this.openFileToolStripMenuItem.Name = "openFileToolStripMenuItem";
            this.openFileToolStripMenuItem.Size = new System.Drawing.Size(224, 26);
            this.openFileToolStripMenuItem.Text = "打开";
            this.openFileToolStripMenuItem.Click += new System.EventHandler(this.openFileToolStripMenuItem_Click);
            // 
            // SaveFileToolStripMenuItem
            // 
            this.SaveFileToolStripMenuItem.Name = "SaveFileToolStripMenuItem";
            this.SaveFileToolStripMenuItem.Size = new System.Drawing.Size(224, 26);
            this.SaveFileToolStripMenuItem.Text = "保存";
            this.SaveFileToolStripMenuItem.Click += new System.EventHandler(this.SaveFileToolStripMenuItem_Click);
            // 
            // 导出ToolStripMenuItem
            // 
            this.导出ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newAmtToolStripMenuItem,
            this.newSprToolStripMenuItem});
            this.导出ToolStripMenuItem.Name = "导出ToolStripMenuItem";
            this.导出ToolStripMenuItem.Size = new System.Drawing.Size(224, 26);
            this.导出ToolStripMenuItem.Text = "导出";
            // 
            // newAmtToolStripMenuItem
            // 
            this.newAmtToolStripMenuItem.Name = "newAmtToolStripMenuItem";
            this.newAmtToolStripMenuItem.Size = new System.Drawing.Size(224, 26);
            this.newAmtToolStripMenuItem.Text = "动画文件(*.amt)";
            this.newAmtToolStripMenuItem.Click += new System.EventHandler(this.newAmtToolStripMenuItem_Click);
            // 
            // newSprToolStripMenuItem
            // 
            this.newSprToolStripMenuItem.Name = "newSprToolStripMenuItem";
            this.newSprToolStripMenuItem.Size = new System.Drawing.Size(224, 26);
            this.newSprToolStripMenuItem.Text = "贴图文件(*.spr)";
            this.newSprToolStripMenuItem.Click += new System.EventHandler(this.newSprToolStripMenuItem_Click);
            // 
            // splitContainer1
            // 
            this.splitContainer1.Location = new System.Drawing.Point(12, 45);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.removeFrameButton);
            this.splitContainer1.Panel1.Controls.Add(this.addFrameButton);
            this.splitContainer1.Panel1.Controls.Add(this.animatorFrameGridView);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.currentPictureBox);
            this.splitContainer1.Size = new System.Drawing.Size(986, 524);
            this.splitContainer1.SplitterDistance = 326;
            this.splitContainer1.TabIndex = 7;
            // 
            // removeFrameButton
            // 
            this.removeFrameButton.Location = new System.Drawing.Point(3, 468);
            this.removeFrameButton.Name = "removeFrameButton";
            this.removeFrameButton.Size = new System.Drawing.Size(320, 53);
            this.removeFrameButton.TabIndex = 2;
            this.removeFrameButton.Text = "删除帧";
            this.removeFrameButton.UseVisualStyleBackColor = true;
            this.removeFrameButton.Click += new System.EventHandler(this.removeFrameButton_Click);
            // 
            // addFrameButton
            // 
            this.addFrameButton.Location = new System.Drawing.Point(3, 409);
            this.addFrameButton.Name = "addFrameButton";
            this.addFrameButton.Size = new System.Drawing.Size(320, 53);
            this.addFrameButton.TabIndex = 1;
            this.addFrameButton.Text = "添加帧";
            this.addFrameButton.UseVisualStyleBackColor = true;
            this.addFrameButton.Click += new System.EventHandler(this.addFrameButton_Click);
            // 
            // animatorFrameGridView
            // 
            this.animatorFrameGridView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.animatorFrameGridView.Location = new System.Drawing.Point(3, 3);
            this.animatorFrameGridView.Name = "animatorFrameGridView";
            this.animatorFrameGridView.RowHeadersWidth = 51;
            this.animatorFrameGridView.RowTemplate.Height = 27;
            this.animatorFrameGridView.Size = new System.Drawing.Size(320, 400);
            this.animatorFrameGridView.TabIndex = 0;
            //this.animatorFrameGridView.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.animatorFrameGridView_CellContentClick);
            // 
            // currentPictureBox
            // 
            this.currentPictureBox.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.currentPictureBox.Location = new System.Drawing.Point(3, 3);
            this.currentPictureBox.Name = "currentPictureBox";
            this.currentPictureBox.Size = new System.Drawing.Size(650, 400);
            this.currentPictureBox.TabIndex = 0;
            this.currentPictureBox.TabStop = false;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1010, 581);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainForm";
            this.Text = "NoaGameEngineTool";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.animatorFrameGridView)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.currentPictureBox)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem SaveFileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openFileToolStripMenuItem;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.PictureBox currentPictureBox;
        private System.Windows.Forms.Button removeFrameButton;
        private System.Windows.Forms.Button addFrameButton;
        private System.Windows.Forms.DataGridView animatorFrameGridView;
        private System.Windows.Forms.ToolStripMenuItem 导出ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newAmtToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newSprToolStripMenuItem;
    }
}