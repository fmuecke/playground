using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Mp3Mgr
{
    public class Mp3Album
    {
        public string Path
        { get; set; }

        public string Artist
        { get; private set; }

        public string Album
        { get; private set; }

        public string Additional
        { get; private set; }

        public List<Mp3File> Tracks
        { get; private set; }

        public string FolderImage { get; private set; }

        public bool IsOk
        { get; private set; }

        public string CheckResult
        { get; private set; }

        public Mp3Album(string basePath, string path)
        {
            if (!path.StartsWith(basePath))
            {
                throw new ArgumentException("input paths are incompatible");
            }

            Path = path;
            System.IO.DirectoryInfo dirInfo = new System.IO.DirectoryInfo(Path);
            Tracks = new List<Mp3File>();
            try
            {
                foreach (var file in dirInfo.GetFiles("*.mp3", System.IO.SearchOption.AllDirectories))
                {
                    Mp3File track = new Mp3File(file.Name);

                    Tracks.Add(track);
                }

                List<string> images = new List<string>();
                foreach (var file in dirInfo.GetFiles("*.jpg", System.IO.SearchOption.TopDirectoryOnly))
                {
                    if (file.Name.ToLower().Contains("folder") || file.Name.ToLower().Contains("cover"))
                    {
                        images.Clear();
                        images.Add(file.FullName);

                        break;
                    }

                    images.Add(file.FullName);
                }

                if (images.Count == 0)
                {
                    //string s = System.IO.Path.GetDirectoryName(System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName);
                    //s = System.Reflection.Assembly.GetExecutingAssembly().Location;
                    images.Add(System.IO.Path.GetDirectoryName(System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName) + "\\MissingFolderImage.jpg");
                }

                FolderImage = images[0];
            }
            catch (UnauthorizedAccessException) { }

            // get relevant path tokens
            char[] separator = { '\\' };
            var pathTokens = path.Substring(basePath.Length, path.Length - basePath.Length).Split(separator, StringSplitOptions.RemoveEmptyEntries);

            // check for "artist - album"
            if (pathTokens.Length < 1)
            {
                CheckResult = PathChecker.CheckResult.PathTooShort.ToString();
                IsOk = false;
                return;
            }

            var separatorPos = pathTokens[pathTokens.Length - 1].IndexOf(" - ");
            if (separatorPos > 0)
            {
                Artist = pathTokens[pathTokens.Length - 1].Substring(0, separatorPos);
                Album = pathTokens[pathTokens.Length - 1].Substring(separatorPos + 3);
            }
            else
            {
                if (pathTokens.Length < 2)
                {
                    CheckResult = PathChecker.CheckResult.PathTooShort.ToString();
                    IsOk = false;
                    return;
                }

                Artist = pathTokens[pathTokens.Length - 2];
                Album = pathTokens[pathTokens.Length - 1];
            }
            PathChecker checker = new PathChecker(path, basePath);

            //var result = checker.PerformCheck();
            //CheckResult = result.ToString();

            //Artist = checker.ArtistFolderName;
            //Album = checker.AlbumFolderName;

            //IsOk = result == PathChecker.CheckResult.Ok;
            IsOk = true;
        }
    }
}
