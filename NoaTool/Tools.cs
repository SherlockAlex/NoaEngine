using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NoaTool
{
    public static class Animator
    {
        /// <summary>
        /// 保存动画到本地
        /// </summary>
        /// <param name="data">动画帧数据</param>
        /// <param name="file">文件路径</param>
        public static int Save(List<List<uint>> data, string file)
        {
            try
            {
                using (BinaryWriter writer = new BinaryWriter(File.Open(file, FileMode.Create)))
                {
                    // 行 列 元素1 元素2
                    // 写入二维列表的行数
                    writer.Write((uint)data.Count);

                    foreach (List<uint> row in data)
                    {
                        // 写入每行的元素个数
                        writer.Write((uint)row.Count);

                        foreach (uint value in row)
                        {
                            // 写入每个元素
                            writer.Write(value);
                        }
                    }
                }

                return 0; // 返回成功保存的标识
            }
            catch (Exception e)
            {
                Console.WriteLine("保存文件出错：" + e.Message);
                return -1; // 返回保存失败的标识
            }
        }
    }

    public static class Sprite{

        public static List<uint> ReadImageRGB(string imagePath)
        {
            List<uint> rgbValues = new List<uint>();

            try
            {
                // 读取图片
                Bitmap image = new Bitmap(imagePath);

                // 遍历每个像素点
                for (int x = 0; x < image.Width; x++)
                {
                    for (int y = 0; y < image.Height; y++)
                    {
                        // 获取当前像素的RGB值
                        Color pixelColor = image.GetPixel(x, y);
                        uint rgb = (uint)(pixelColor.R | pixelColor.G << 8 | pixelColor.B<<16);
                        // 添加到列表中
                        rgbValues.Add(rgb);
                    }
                }

                // 释放图片资源
                image.Dispose();

                return rgbValues;
            }
            catch (Exception e)
            {
                Console.WriteLine("读取图片出错：" + e.Message);
                return null;
            }
        }

        public static int Save(List<uint> data, string filename)
        {
            using (BinaryWriter writer = new BinaryWriter(File.Open(filename, FileMode.Create)))
            {
                // 写入data的大小
                writer.Write(data.Count);

                // 写入vector的内容
                foreach (uint num in data)
                {
                    writer.Write(num);
                }

                return 0;

            }
        }

        public static uint[] Load(string filename)
        {
            if (!File.Exists(filename))
            {
                Console.WriteLine("文件不存在！");
                return null;
            }

            using (BinaryReader reader = new BinaryReader(File.Open(filename, FileMode.Open)))
            {
                // 读取data的大小
                int size = reader.ReadInt32();

                // 分配动态内存空间
                uint[] data = new uint[size];

                // 读取vector的内容
                for (int i = 0; i < size; i++)
                {
                    data[i] = reader.ReadUInt32();
                }

                return data;
            }
        }

    }
}
