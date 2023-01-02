using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Mp3Mgr
{
    public class Mp3File
    {
        public string FileName { get; set; }
        public string Title { get; private set; }
        public int TrackNo { get; private set; }
        public string OptionalArtist { get; private set; }
        public string OptionalAlbum { get; private set; }
        public string Suffix { get; private set; }

        public bool IsValid
        { get; private set; }

        public Mp3File(string fileName)
        {
            Assign(fileName);
        }

        private void Assign(string fileName)
        {
            IsValid = false;

            FileName = fileName;
            if (!string.IsNullOrEmpty(FileName) && !FileName.Contains('\\') && !FileName.Contains('_') && FileName.EndsWith(".mp3"))
            {
                // file name must 
                // - begin with track number
                // - followed by a space
                // - followed by some alphanumeric chars
                // - ending with ".mp3"
                //System.Text.RegularExpressions.Regex regex = new System.Text.RegularExpressions.Regex(
                //"[0-9]{2,3}\\s(-\\s){0,1}[\\w()\\[\\].,'!&#]+[\\s\\w()\\[\\].,'!&#]*[.]mp3");

                //System.Text.RegularExpressions.Regex regex = 
                //    new System.Text.RegularExpressions.Regex("^\\d{2,3} [\\w\\-,#\\+..]+( {0,1}[\\w\\-,#\\+..]+)*[.]mp3$");

                //if (!regex.IsMatch(FileName))
                //{
                //    return;
                //}

                // extract track no (all digits till space)
                var spacePos = FileName.IndexOf(' ');
                if (-1 == spacePos || spacePos > 3)
                {
                    return;
                }

                int trackNo = 0;
                if (!Int32.TryParse(FileName.Substring(0, spacePos), out trackNo))
                {
                    return;
                }
                TrackNo = trackNo;

                // check for artist (separated by " - ")
                string toProcess = FileName.Substring(spacePos + 1, FileName.Length - (spacePos + 1) - 4); // also remove ".mp3"
                if (toProcess.Contains(" - "))
                {
                    var separatorPos = toProcess.IndexOf(" - ");
                    OptionalArtist = toProcess.Substring(0, separatorPos);
                    Title = toProcess.Substring(separatorPos + 3);
                }
                else
                {
                    Title = toProcess;
                }

                // check for optional album
                if (Title.Contains(" - "))
                {
                    var separatorPos = Title.IndexOf(" - ");
                    OptionalAlbum = Title.Substring(0, separatorPos);
                    Title = Title.Substring(separatorPos + 3);
                }

                IsValid = true;
            }
        }
    }
}
