using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static NoaTool.Animator;
using static NoaTool.Sprite;

namespace NoaTool
{

    public class Level
    {
        public struct Map
        {
            public int w;
            public int h;
            public List<uint> image;
        }

        public static int ExportMap(string imageFilePath, string fileName)
        {
            // 读取图像文件
            Bitmap image = new Bitmap(imageFilePath);

            // 创建Map结构体对象
            Map map = new Map();
            map.w = image.Width;
            map.h = image.Height;
            map.image = new List<uint>();

            // 逐像素读取灰度值并保存到image列表中
            for (int y = 0; y < image.Height; y++)
            {
                for (int x = 0; x < image.Width; x++)
                {
                    Color pixelColor = image.GetPixel(x, y);
                    uint grayScale = (uint)((pixelColor.R + pixelColor.G + pixelColor.B) / 3);
                    map.image.Add(grayScale);
                }
            }

            // 将结构体保存到本地二进制文件
            try
            {
                using (FileStream fs = new FileStream(fileName, FileMode.Create))
                {
                    using (BinaryWriter writer = new BinaryWriter(fs))
                    {
                        writer.Write(map.w);
                        writer.Write(map.h);
                        foreach (uint pixel in map.image)
                        {
                            writer.Write(pixel);
                        }
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("保存文件出错：" + e.Message);
                return -1;
            }

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

        [Serializable]
        public struct AnimatorFile
        {
            public List<SpriteFile> data;
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

        public static int Save(AnimatorFile animatorFile, string file)
        {
            using (BinaryWriter writer = new BinaryWriter(File.Open(file, FileMode.Create)))
            {
                writer.Write(animatorFile.data.Count);
                foreach (var spriteFile in animatorFile.data)
                {
                    writer.Write(spriteFile.images.Count);
                    foreach (var image in spriteFile.images)
                    {
                        writer.Write(image);
                    }
                    writer.Write(spriteFile.x);
                    writer.Write(spriteFile.y);
                    writer.Write(spriteFile.width);
                    writer.Write(spriteFile.height);
                }
                writer.Write(animatorFile.posx);
                writer.Write(animatorFile.posy);
                writer.Write(animatorFile.w);
                writer.Write(animatorFile.h);
            }
            return 0;
        }

    }

    public static class Sprite{

        [Serializable]
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

    //public static class Gun
    //{
    //    public struct GunFile
    //    {
    //        public int deltax;         //枪支的屏幕坐标偏移量
    //        public int deltay;
    //        public int damage;         //子弹威力

    //        //第一帧大小
    //        public int w;
    //        public int h;

    //        public AnimatorFile amt;   //枪支动画
    //    }

    //    //将gunFile的数据内容保存到本地二进制文件file中，成功返回0
    //    public static int Save(GunFile gunFile, string file)
    //    {
    //        try
    //        {
    //            // 创建二进制写入器
    //            using (BinaryWriter writer = new BinaryWriter(File.Open(file, FileMode.Create)))
    //            {
    //                // 写入枪支属性
    //                writer.Write(gunFile.deltax);
    //                writer.Write(gunFile.deltay);
    //                writer.Write(gunFile.damage);
    //                writer.Write(gunFile.w);
    //                writer.Write(gunFile.h);

    //                // 写入枪支动画属性
    //                writer.Write(gunFile.amt.posx);
    //                writer.Write(gunFile.amt.posy);
    //                writer.Write(gunFile.amt.w);
    //                writer.Write(gunFile.amt.h);

    //                // 写入枪支动画数据
    //                foreach (List<uint> frameData in gunFile.amt.data)
    //                {
    //                    foreach (uint pixel in frameData)
    //                    {
    //                        writer.Write(pixel);
    //                    }
    //                }
    //            }

    //            return 0; // 返回0表示成功保存文件
    //        }
    //        catch (Exception e)
    //        {
    //            Console.WriteLine($"保存文件失败：{e.Message}");
    //            return -1; // 返回-1表示保存文件失败
    //        }
    //    }

    //}

    public static class TileSet
    {
        public struct PixelData
        {
            public int id;
            public SpriteFile sprites;
            //public List<uint> Pixels;
        }

        public static List<PixelData> SplitImage(Bitmap image, int width, int height)
        {
            List<PixelData> result = new List<PixelData>();
            int id = 0;

            int widthCount = image.Width / width;
            int heightCount = image.Height / height;

            MessageBox.Show($"切分图片为瓦片\n横:{widthCount}\n纵:{heightCount}");

            for (int y = 0; y < heightCount * height; y += height)
            {
                for (int x = 0; x < widthCount * widthCount; x += width)
                {
                    SpriteFile spriteFile = new SpriteFile
                    {
                        images = new List<uint>(),
                        x = x,
                        y = y,
                        width = width,
                        height = height
                    };

                    for (int i = 0; i < width; i++)
                    {
                        for (int j = 0; j < height; j++)
                        {
                            Color pixelColor = image.GetPixel(x + i, y + j);
                            uint pixelValue = (uint)((pixelColor.R) | (pixelColor.G << 8) | pixelColor.B<<16);
                            spriteFile.images.Add(pixelValue);
                        }
                    }

                    PixelData pixelData = new PixelData
                    {
                        id = id,
                        sprites = spriteFile
                    };

                    result.Add(pixelData);
                    id++;
                }
            }

            return result;

        }

        public static void SaveTileSetData(List<PixelData> tileSet, string fileName)
        {
            using (BinaryWriter writer = new BinaryWriter(File.Open(fileName, FileMode.Create)))
            {
                writer.Write(tileSet.Count);

                foreach (PixelData pixelData in tileSet)
                {
                    writer.Write(pixelData.id);
                    writer.Write(pixelData.sprites.images.Count);
                    writer.Write(pixelData.sprites.x);
                    writer.Write(pixelData.sprites.y);
                    writer.Write(pixelData.sprites.width);
                    writer.Write(pixelData.sprites.height);

                    foreach (uint pixelValue in pixelData.sprites.images)
                    {
                        writer.Write(pixelValue);
                    }
                }
            }
        }
    }
}
