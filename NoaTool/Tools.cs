using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace NoaTool
{

    public class Level
    {
        public struct Map
        {
            public int w;
            public int h;
            public IntPtr image;
        }

        public static int ExportMap(string imageFilePath, string fileName)
        {
            List<byte> grayscaleValues = new List<byte>();
            int width, height;

            // 加载图片
            using (Bitmap image = new Bitmap(imageFilePath))
            {
                width = image.Width;
                height = image.Height;

                // 遍历图片的每个像素
                for (int y = 0; y < height; y++)
                {
                    for (int x = 0; x < width; x++)
                    {
                        // 获取当前像素的颜色
                        Color pixelColor = image.GetPixel(x, y);

                        // 将颜色转换为灰度值
                        byte grayscaleValue = (byte)((pixelColor.R + pixelColor.G + pixelColor.B) / 3);

                        // 将灰度值添加到结果列表
                        grayscaleValues.Add(grayscaleValue);
                    }
                }
            }

            // 将灰度值列表转换为字节数组
            byte[] data = grayscaleValues.ToArray();

            // 创建 Map 结构体
            Map map = new Map
            {
                w = width,
                h = height,
                image = Marshal.AllocHGlobal(data.Length)
            };

            // 将字节数组复制到 Map 结构体的内存指针中
            Marshal.Copy(data, 0, map.image, data.Length);

            // 将 Map 结构体保存到本地二进制文件中
            using (FileStream fileStream = new FileStream(fileName, FileMode.Create))
            {
                using (BinaryWriter writer = new BinaryWriter(fileStream))
                {
                    writer.Write(map.w);
                    writer.Write(map.h);

                    byte[] imageData = new byte[data.Length];
                    Marshal.Copy(map.image, imageData, 0, data.Length);
                    writer.Write(imageData);
                }
            }

            // 释放内存
            Marshal.FreeHGlobal(map.image);

            Console.WriteLine("Map 已成功导出。");
            return 0;
        }


    }

    public static class Animator
    {
        /// <summary>
        /// 保存动画到本地
        /// </summary>
        /// <param name="data">动画帧数据</param>
        /// <param name="file">文件路径</param>
        /// 

        public struct AnimatorFile
        {
            public List<List<uint>> data;
            public int posx;
            public int posy;
            public int w;
            public int h;
        }

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

        public struct SpriteFile
        {
            //spr文件信息
            public List<uint> images;
            public int x; 
            public int y;
            //图片大小
            public int width;
            public int height;
        }

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

        public static int Save(SpriteFile spriteFile, string file)
        {
            try
            {
                using (BinaryWriter writer = new BinaryWriter(File.Open(file, FileMode.Create)))
                {
                    // 写入 List<uint> images
                    writer.Write(spriteFile.images.Count);
                    foreach (uint image in spriteFile.images)
                    {
                        writer.Write(image);
                    }
                    // 写入 int x, int y, int width, int height
                    writer.Write(spriteFile.x);
                    writer.Write(spriteFile.y);
                    writer.Write(spriteFile.width);
                    writer.Write(spriteFile.height);
                }
                return 0;
            }
            catch (Exception e)
            {
                Console.WriteLine("保存失败：" + e.Message);
                return -1;
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
