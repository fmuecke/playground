using System;
using System.Collections.Generic;
using System.Text;

namespace ComicStripper
{
    class Comic
    {
        #region Attributes
        private string name;
        public string Name
        {
            get { return name; }
            set { name = value; }
        }
        private string nameTag;
        public string NameTag
        {
            get { return nameTag; }
            set { nameTag = value; }
        }
        private string imageType;
        public string ImageType
        {
            get { return imageType; }
            set { imageType = value; }
        }
        private string remotePath;
        public string RemotePath
        {
            get { return remotePath; }
            set { remotePath = value; }
        }
        
        #endregion

        public Comic( string name, string nameTag, string imageType, string remotePath )
        {
            this.name = name;
            this.nameTag = nameTag;
            this.imageType = imageType;
            this.remotePath = remotePath.EndsWith( "/" ) ? remotePath : remotePath + "/";
        }

        public string getFileName( DateTime date )
        {
            string yStr = date.Year.ToString().Substring( 2 );
            string mStr = ( date.Month > 9 ) ? date.Month.ToString() : "0" + date.Month.ToString();
            string dStr = ( date.Day > 9 ) ? date.Day.ToString() : "0" + date.Day.ToString();
            return nameTag + yStr + mStr + dStr + "." + imageType;
        }

        public string getRemoteUrl( DateTime date )
        {
            return remotePath + nameTag + "/" + date.Year + "/" + getFileName( date );
        }

        public override string ToString() { return name; }
    }

    class ComicHaegar : Comic
    {
        public ComicHaegar( string name, string nameTag, string imageType ):base( name, nameTag, imageType, "" )
        {
        }

        public new string getRemoteUrl( DateTime date )
        {
            return "http://cserver.king-online.com/content/Hagar_The_Horrible?date=" + date.Year + "-" + date.Month + "-" + date.Day + "&referer=http://www.dailyink.com&uid=xvkv6o&token=r5be25";
            //return "http://cserver.king-online.com/content/Hagar_The_Horrible%3fdate=" + date.Year + "-" + date.Month + "-" + date.Day + "%26referer%3dhttp%3a%2f%2fwww.dailyink.com%26uid%3dxvkv6o%26token%3dr5be25";
        }
    }
}
