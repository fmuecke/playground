using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.IO;
using System.Xml;

namespace ComicStripper
{
    public partial class ComicStripper: Form
    {
        private const string appTitle = "ComicStripper";
        private const string version = "0.2";
        private const string author = "Florian Mücke";
        private DateTime date;
        private string localPath, nopicUrl, url;
        XmlDocument doc;

        //private string yStr, mStr, dStr;
        Comic comicCH;
        Comic comicGA;
        ComicHaegar comicHA;

        public ComicStripper()
        {
            InitializeComponent();
            date = DateTime.Today;
            // http://images.ucomics.com/comics/ch/2007/ch071212.gif
            // http://images.ucomics.com/comics/ga/2007/ga071212.gif
            // http://cserver.king-online.com/content/Hagar_The_Horrible?date=2007-12-12&referer=http://www.dailyink.com&uid=xvkv6o&token=r5be25
            // http://cserver.king-online.com/content/Spiderman?date=2007-12-12&referer=http://www.dailyink.com&uid=xvkv6o&token=r5be25

            comicCH = new Comic( "Calvin and Hobbes", "ch", "gif", "http://images.ucomics.com/comics/" );
            comicGA = new Comic( "Garfield", "ga", "gif", "http://images.ucomics.com/comics/" );
            comicHA = new ComicHaegar( "Hägar the Horrible", "ha", "gif" );
            comboBox.Items.Add( comicCH );
            comboBox.Items.Add( comicGA ); 
            comboBox.Items.Add( comicHA );
            localPath = Directory.GetCurrentDirectory() + "\\data\\";
            nopicUrl = localPath + "images\\nopic.png";
            comboBox.SelectedIndex = 0;
            getComicsFromXml( localPath + "comics.xml" );
            update();
        }

        private void update()
        {
            Comic comic = (Comic) comboBox.SelectedItem;
            url = localPath + comic.Name + "\\" + date.Year + "\\" + comic.getFileName( date );
            if( !Directory.Exists( localPath + comic.Name ) ) Directory.CreateDirectory( localPath + comic.Name );
            if( !Directory.Exists( localPath + comic.Name + "\\" + date.Year ) ) Directory.CreateDirectory( localPath + comic.Name + "\\" + date.Year );
            if( !File.Exists( url ) )
            {
                // pic not available, try to download
                //MessageBox.Show( comic.getRemoteUrl( date ) );
                //System.Uri uri = new System.Uri( "http://cserver.king-online.com/content/Hagar_The_Horrible?date=" + date.Year + "-" + date.Month + "-" + date.Day + "&referer=http://www.dailyink.com&uid=xvkv6o&token=r5be25");
                System.Uri uri = new System.Uri( comic.getRemoteUrl( date ) );
                WebClient client = new WebClient();
                statusBarLabel.Text = "image " + comic.getRemoteUrl( date ) + " added to local store";
                try
                {
                    client.DownloadFile( uri, url );
                }
                catch( WebException )
                {
                    // file does not exist or uri is not acessable
                    url = nopicUrl;
                    statusBarLabel.Text = "image not available";
                }
                client.Dispose();
            }
            else
            {
                statusBarLabel.Text = "image sucessfully loaded";
            }
            
            try { pictureBox.Load( url ); }
            catch( Exception ) { MessageBox.Show( this, "Unable to open image " + url, "error" ); return; }
            pictureBox.Height = pictureBox.Image.Height;
            pictureBox.Width = Math.Max( pictureBox.Image.Width, 450 );
            this.Height = pictureBox.Height + 103;
            this.Width = pictureBox.Width + 4;
            this.Text = /* appTitle + ": " +*/ comic.Name + " - " + date.ToLongDateString();
            Update();
        }

        private void buttonPrev_Click( object sender, EventArgs e )
        {
            date = date.AddDays( -1 );
            update();
        }

        private void buttonNext_Click( object sender, EventArgs e )
        {
            date = date.AddDays( 1 );
            update();
        }

        private void buttonToday_Click( object sender, EventArgs e )
        {
            date = DateTime.Today;
            update();
        }

        private void comboBox_SelectedIndexChanged( object sender, EventArgs e )
        {
            update();
        }

        // About
        private void aboutToolStripMenuItem1_Click( object sender, EventArgs e )
        {
            MessageBox.Show( this, appTitle + "\n\nv" + version + "\n" +author+"\nDecember 2007", "About "+appTitle,
                MessageBoxButtons.OK, MessageBoxIcon.Information );
        }

        private void buttonUp_Click( object sender, EventArgs e )
        {
            comboBox.SelectedIndex = Math.Max( 0, comboBox.SelectedIndex - 1 );
        }

        private void buttonDown_Click( object sender, EventArgs e )
        {
            comboBox.SelectedIndex = Math.Min( comboBox.Items.Count - 1, comboBox.SelectedIndex + 1 );
        }

        private void getComicsFromXml( string fileName )
        {
            doc = new XmlDocument();
            try
            {
                doc.Load( fileName );
            }
            catch( Exception )
            {
                
                //throw;
            }
            System.Console.WriteLine( doc.FirstChild.Name );
        }
    }
}