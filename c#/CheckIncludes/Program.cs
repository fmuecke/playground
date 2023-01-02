using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace CheckIncludes
{
	public class IncludeData
	{
		public string _fileName = null;
		public List<string> _includes = null;
		public List<string> _forwards = null;
		public List<string> _references = null;
		public List<string> _declarations = null;

		public IncludeData(string fileName)
		{
			_fileName = fileName;
			_includes = new List<string>();
			_forwards = new List<string>();
			_references = new List<string>();
			_declarations = new List<string>();
		}
	}


	class Program
	{
		public static void PrintUsage()
		{
			Console.WriteLine("usage: CheckIncludes.exe <base folder>");
		}

		public static void Main(string[] args)
		{
			if (args.Length < 1 
				|| !Directory.Exists(args[0]))
			{
				PrintUsage();
				return;
			}

			string path = args[0];

			List<string> headerFiles = new List<string>();
			List<IncludeData> includeDataAll = new List<IncludeData>();
			headerFiles.AddRange(Directory.EnumerateFiles(path, "*.h", SearchOption.AllDirectories));

			//foreach(string fileName in headerFiles)
			//	Console.WriteLine(fileName);

			Dictionary<string, string> classRefs = new Dictionary<string, string>();
			foreach (string fileName in headerFiles)
			{
				using (var reader = new StreamReader(fileName))
				{
					IncludeData data = new IncludeData(fileName);
					bool openComment = false;
					while (true)
					{
						string line = reader.ReadLine();
						if (line == null)
							break;

						if (line == string.Empty)
							continue;

						line = line.Trim();

						if (openComment)
						{
							if (line.EndsWith("*/"))
							{
								openComment = false;
							}

							continue;
						}

						if (line.StartsWith("//"))
							continue;

						if (line.StartsWith("/*"))
						{
							openComment = true;
							continue;
						}

						string[] lineTokens = line.Split();
						for (int i = 1; i <= lineTokens.Length; ++i)
						{
							if (lineTokens[i - 1] == "#include")
							{

								if (lineTokens[i].StartsWith("\"") || lineTokens[i].StartsWith("<"))
								{
									data._includes.Add(lineTokens[i].Substring(1, lineTokens[i].Length - 2));
								}
								else
								{
									Console.Error.WriteLine("Invalid include string: {0}\n\tfile: {1}", lineTokens[i], fileName);
								}

								break;
							}
							
							if ((lineTokens[i - 1] == "class" || lineTokens[i - 1] == "struct") && lineTokens.Length > i)
							{
								string token = lineTokens[i];
								if (token.EndsWith(";"))
								{
									data._forwards.Add(token.Substring(0, token.Length - 1));
									break;
								}

								//if (classRefs.ContainsKey(lineTokens[i]))
								//{
								//    Console.Error.WriteLine(string.Format("Duplicate definition of {0}:\n\t{1}\n\t{2}",
								//        lineTokens[i],
								//        classRefs[lineTokens[i]],
								//        fileName));
								//}
								//else
								//{
									data._declarations.Add(lineTokens[i]);
									break;
								//}
							}
						}
					}
					includeDataAll.Add(data);
				}
			}

			foreach (var entry in classRefs)
			{
				Console.WriteLine(entry.Key + ": " + entry.Value);
			}
		}
	}
}
