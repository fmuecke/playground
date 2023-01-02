using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;
using ExifLib;

namespace ExifApp1
{
    
    class Program
    {
        private static string[] GetKeywords(string filespec)
        {
            BitmapDecoder decoder = new JpegBitmapDecoder(new FileStream(filespec, FileMode.Open), BitmapCreateOptions.None, BitmapCacheOption.None);
            BitmapMetadata meta = (BitmapMetadata)decoder.Frames[0].Metadata;
            return meta.Keywords.ToArray<string>();
        }

        static void Main(string[] args)
        {
            var x = GetKeywords(@"G:\4Picasa\test.jpg");
            
            // Instantiate the reader
            ExifReader reader = new ExifReader(@"G:\4Picasa\test.jpg");

            // Extract the tag data using the ExifTags enumeration

            var tags = Enum.GetValues(typeof(ExifTags));
            foreach (ExifTags tag in tags)
            {
                object val;
                if (reader.GetTagValue(tag, out val))
                {
                    // Arrays don't render well without assistance.
                    string renderedTag;
                    if (val is Array)
                    {
                        var array = (Array)val;

                        renderedTag = "";
                        if (array.Length > 0)
                        {

                            foreach (object item in array)
                            {
                                renderedTag += item + ",";
                            }

                            renderedTag = renderedTag.Substring(0, renderedTag.Length - 1);
                            Console.WriteLine("{0}: {1}", tag.ToString(), renderedTag);
                        }
                    }
                    else
                    {
                        Console.WriteLine("{0}: {1}", tag.ToString(), val);
                    }
                }
            }
        }
    }
}
