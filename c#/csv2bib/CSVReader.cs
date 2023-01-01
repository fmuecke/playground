using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace csv2bib {
    
    class CSVReader {
        private StreamReader infile;
        private String[] header;
        public char separator = ',';
        public char quotations = '"';

        public CSVReader(String filename) {
            infile = new StreamReader(filename, Encoding.Default);
        }

        public void writeData(String filename){
            Console.WriteLine("using " + separator + " as separator");
            Console.WriteLine("using " + quotations +" as quotations");
            String line = infile.ReadLine();
            header = split(line, separator, quotations);
            for (int i = 0; i < header.Length; i++) {
                header[i] = header[i].Trim(quotations).ToLower();
            }
            if (!header[0].Equals("name") || !header[1].Equals("reference")) {
                Console.WriteLine("ERROR: file is in the wrong format");//Exception!
                return;
            }
            StreamWriter outfile = new StreamWriter(filename, false, Encoding.Default);
            String[] data;
            
            while (infile.EndOfStream == false) {
                line = infile.ReadLine();
                data = split(line, separator, quotations);
                data[0] = data[0].Trim(quotations);
                data[1] = data[1].Trim(quotations).ToUpper();
                outfile.WriteLine("@"+data[1]+"{"+data[0]+",");
                for (int i = 2; i < header.Length; i++) {
                    data[i] = data[i].Trim(quotations);
                    if (data[i].Length>0) {
                        outfile.WriteLine("\t"+header[i]+"={"+data[i]+"},");
                    }
                }
                // remove last comma
                outfile.Flush(); //write to file
                outfile.BaseStream.Position = outfile.BaseStream.Position - 3; //step back 3 chars (",\n")
                //close this section
                outfile.WriteLine("\n}\n");
            }
            outfile.Close();
            Console.WriteLine("bib file generated");
        }

        public void close() {
            infile.Close();
        }

        public void setSeparator(char s) {
            this.separator = s;
        }

        public void setQuotations(char t) {
            this.quotations = t;
        }

        private String[] split(String str, char s, char q) {
            int vals = 1; //that's for the first/last one (depending on your point of view)
            int index=0;
            char[] charArray = str.ToCharArray();
            bool open = false; //quotations open?

            //first count the values
            for (int i = 0; i < charArray.Length; i++) {
                if (charArray[i].Equals(q)) open = !open;
                else if (charArray[i].Equals(s) && !open) {
                    vals++;
                }
            }
            String[] ret = new String[vals];
            vals = 0;
            for (int i = 0; i < charArray.Length; i++) {
                if (charArray[i].Equals(q)) open = !open;
                else if (charArray[i].Equals(s) && !open) {
                    ret[vals++] = new String(charArray, index, i - index);
                    index = i + 1; //separator
                }
            }
            ret[vals] = new String(charArray, index, charArray.Length - index);
            return ret;
        }

    }
}
