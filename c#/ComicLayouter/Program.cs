using System;
using System.IO;
using System.Collections.Generic;
using System.Text;

namespace ComicLayouter
{
    class Program
    {
        static void Main( string[] args )
        {
            int year = 2008;
            string title = "Calvin and Hobbes " + year;
            const string nameTag = "ch";
            const string fileType = "gif";

            DateTime date = new DateTime( year, 1, 1 );
            switch( date.DayOfWeek )
            {
                case DayOfWeek.Sunday: 
                    date = date.AddDays(  1 ); break;
                case DayOfWeek.Monday: 
                    date = date.AddDays(  0 ); break;
                case DayOfWeek.Tuesday: 
                    date = date.AddDays( -1 ); break;
                case DayOfWeek.Wednesday: 
                    date = date.AddDays( -2 ); break;
                case DayOfWeek.Thursday: 
                    date = date.AddDays( -3 ); break;
                case DayOfWeek.Friday: 
                    date = date.AddDays( +3 ); break;
                case DayOfWeek.Saturday: 
                    date = date.AddDays( +2 ); break;
            }

            string monthStr = "";
            int week = 1;
            string file = nameTag+year+".html";
            string header = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\"><html><head><title>" + title + "</title><style type=\"text/css\">@page{margin:0cm}</style></head><body>";
            string footer = "</body></html>";
            bool first = true;

            StreamWriter sw = new StreamWriter( file, false, Encoding.ASCII );
            sw.WriteLine( header );
            while( week <= 52 )
            {
                sw.Write( "<div" );
                if( first )
                    first = false;
                else
                    sw.Write( " style=\"page-break-before:always\"" );
                sw.WriteLine( ">" );
                sw.Write( "<div><b>" );
                sw.Write( year + " Week " + week + ":</b> " );
                switch( date.Month )
                {
                    case 1: monthStr = "January"; break;
                    case 2: monthStr = "February"; break;
                    case 3: monthStr = "March"; break;
                    case 4: monthStr = "April"; break;
                    case 5: monthStr = "May"; break;
                    case 6: monthStr = "June"; break;
                    case 7: monthStr = "July"; break;
                    case 8: monthStr = "August"; break;
                    case 9: monthStr = "September"; break;
                    case 10: monthStr = "October"; break;
                    case 11: monthStr = "November"; break;
                    case 12: monthStr = "December"; break;
                }
                sw.Write( monthStr + " " + date.Day );
                if( date.Day == 1 || date.Day == 21 || date.Day == 31 ) sw.Write( "st" );
                else if( date.Day == 2 || date.Day == 22 ) sw.Write( "nd" );
                else if( date.Day == 3 || date.Day == 23 ) sw.Write( "rd" ); 
                else sw.Write( "th" );
                sw.Write( " - " );
                if( date.Month != date.AddDays( 6 ).Month )
                    switch( date.AddDays( 6 ).Month )
                    {
                        case 1: monthStr = "January"; break;
                        case 2: monthStr = "February"; break;
                        case 3: monthStr = "March"; break;
                        case 4: monthStr = "April"; break;
                        case 5: monthStr = "May"; break;
                        case 6: monthStr = "June"; break;
                        case 7: monthStr = "July"; break;
                        case 8: monthStr = "August"; break;
                        case 9: monthStr = "September"; break;
                        case 10: monthStr = "October"; break;
                        case 11: monthStr = "November"; break;
                        case 12: monthStr = "December"; break;
                    }
                sw.Write( monthStr );
                sw.Write( " " + date.AddDays( 6 ).Day );
                if( date.AddDays( 6 ).Day == 1 || date.AddDays( 6 ).Day == 21 || date.AddDays( 6 ).Day == 31 ) sw.Write( "st" );
                else if( date.AddDays( 6 ).Day == 2 || date.AddDays( 6 ).Day == 22 ) sw.Write( "nd" );
                else if( date.AddDays( 6 ).Day == 3 || date.AddDays( 6 ).Day == 23 ) sw.Write( "rd" );
                else sw.Write( "th" );
                sw.WriteLine( "</div>" );
                // day 1
                sw.Write( "<img src=\"" + date.Year + "\\" + nameTag + date.Year.ToString().Substring( 2 ) );
                if( date.Month <= 9 ) sw.Write( "0" );
                sw.Write( date.Month );
                if( date.Day <= 9 ) sw.Write( "0" );
                sw.Write( date.Day + "." + fileType + "\"><br><br>\n" );
                date = date.AddDays( 1 );
                // day 2
                sw.Write( "<img src=\"" + date.Year + "\\" + nameTag + date.Year.ToString().Substring( 2 ) );
                if( date.Month <= 9 ) sw.Write( "0" );
                sw.Write( date.Month );
                if( date.Day <= 9 ) sw.Write( "0" );
                sw.Write( date.Day + "." + fileType + "\"><br><br>\n" );
                date = date.AddDays( 1 );
                // day 3
                sw.Write( "<img src=\"" + date.Year + "\\" + nameTag + date.Year.ToString().Substring(2) );
                if( date.Month <= 9 ) sw.Write( "0" );
                sw.Write( date.Month );
                if( date.Day <= 9 ) sw.Write( "0" );
                sw.Write( date.Day + "." + fileType + "\"><br><br>\n" );
                date = date.AddDays( 1 );
                // day 4
                sw.Write( "<img src=\"" + date.Year + "\\" + nameTag + date.Year.ToString().Substring(2) );
                if( date.Month <= 9 ) sw.Write( "0" );
                sw.Write( date.Month );
                if( date.Day <= 9 ) sw.Write( "0" );
                sw.Write( date.Day + "." + fileType + "\"><br><br>\n" );
                date = date.AddDays( 1 );
                // day 5
                sw.Write( "<img src=\"" + date.Year + "\\" + nameTag + date.Year.ToString().Substring( 2 ) );
                if( date.Month <= 9 ) sw.Write( "0" );
                sw.Write( date.Month );
                if( date.Day <= 9 ) sw.Write( "0" );
                sw.Write( date.Day + "." + fileType + "\"><br><br>\n" );
                date = date.AddDays( 1 );
                // day 6
                sw.Write( "<img src=\"" + date.Year + "\\" + nameTag + date.Year.ToString().Substring( 2 ) );
                if( date.Month <= 9 ) sw.Write( "0" );
                sw.Write( date.Month );
                if( date.Day <= 9 ) sw.Write( "0" );
                sw.Write( date.Day + "." + fileType + "\"><br><br>\n" );
                date = date.AddDays( 1 );
                // day 7
                sw.Write( "<img src=\"" + date.Year + "\\" + nameTag + date.Year.ToString().Substring( 2 ) );
                if( date.Month <= 9 ) sw.Write( "0" );
                sw.Write( date.Month );
                if( date.Day <= 9 ) sw.Write( "0" );
                sw.Write( date.Day + "." + fileType + "\">\n" );
                sw.WriteLine( "</div>\n\n" );
                date = date.AddDays( 1 );
                week++;
            }
            sw.WriteLine( footer );
            sw.Close();
        }
    }
}
