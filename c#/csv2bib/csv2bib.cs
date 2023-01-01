using System;
using System.Collections.Generic;
using System.Text;

namespace csv2bib
{
    class csv2bib
    {
        static void help() {
            Console.WriteLine("csv2bib (c) F. Muecke, August 2006\n" +
                    "usage: csv2bib csvfile bibfile <separator> <qoutations>\n" +
                    "\noptional parameters:\n" +
                    "  separator: separates the columns, default: semicolon\n" +
                    "  qoutations: surrounds text, default: \", can only be specified with separator");
            System.Environment.Exit(0);
        }
        
        static void Main(string[] args)
        {
            if (args.Length == 0 || args.Length > 4) help();
            CSVReader csv = new CSVReader(args[0]);
            if (args.Length > 2)
            {
                if (args[2].Length == 1) csv.setSeparator(args[2].ToCharArray()[0]);
                else help();
            }
            if (args.Length > 3)
            {
                if (args[3].Length == 1) csv.setQuotations(args[3].ToCharArray()[0]);
                else help();
            }
            csv.writeData(args[1]);
            csv.close();
        }
    }
}
