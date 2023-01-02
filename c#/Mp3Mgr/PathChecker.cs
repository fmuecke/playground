using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Mp3Mgr
{
    public class PathChecker
    {
        private string _fullPath = null;
        private string _basePath = null;
        private string _fileName = null;
        private string _albumFolderName = null;
        private string _artistFolderName = null;

        public string FileName
        {
          get { return _fileName; }
          private set { _fileName = value; }
        }

        public string AlbumFolderName
        {
          get { return _albumFolderName; }
          private set { _albumFolderName = value; }
        }

        public string ArtistFolderName
        {
          get { return _artistFolderName; }
          private set { _artistFolderName = value; }
        }

        public enum CheckResult
        {
            Ok,
            BasePathDoesNotMatch,
            InvalidFile,
            PathTooShort,
            ArtistFolderHasUnderscores,
            AlbumFolderHasUnderscores,
            FileNameHasUnderscores,
            IncorrectFileName
        }
        
        public PathChecker(string path, string basePath)
        {
            _fullPath = path;
            _basePath = basePath;
        }

        public CheckResult PerformCheck()
        {
            if (!_fullPath.StartsWith(_basePath))
            {
                return CheckResult.BasePathDoesNotMatch;
            }

            char[] separator = {'\\'};
            var pathTokens = _fullPath.Substring(_basePath.Length, _fullPath.Length - _basePath.Length).Split(separator, StringSplitOptions.RemoveEmptyEntries);

            // path must have at least three tokens (artist/album/title)
            if (pathTokens.Length < 3)
            {
                return CheckResult.PathTooShort;
            }

            FileName = pathTokens[pathTokens.Length - 1];
            AlbumFolderName = pathTokens[pathTokens.Length - 2];
            ArtistFolderName = pathTokens[pathTokens.Length - 3];

            ////
            //// path has to end with a file that has ".mp3" extension
            ////
            //if (!FileName.EndsWith(".mp3") || !System.IO.File.Exists(_fullPath))
            //{
            //    return CheckResult.InvalidFile;
            //}

            //
            // path must not contain underscores
            //
            if (ArtistFolderName.Contains('_'))
            {
                return CheckResult.ArtistFolderHasUnderscores;
            }

            if (AlbumFolderName.Contains('_'))
            {
                return CheckResult.AlbumFolderHasUnderscores;
            }

            if (FileName.Contains('_'))
            {
                return CheckResult.FileNameHasUnderscores;
            }

            // file name must begin with 
            //System.Text.RegularExpressions.Regex regex = new System.Text.RegularExpressions.Regex(
            //    "[0-9]{2,3}\\s(-\\s){0,1}[\\w()\\[\\].,'!&#]+[\\s\\w()\\[\\].,'!&#]*[.]mp3");

            //if (!regex.IsMatch(FileName))
            //{
            //    return CheckResult.IncorrectFileName;
            //}

            return CheckResult.Ok;
        }

    }
}
