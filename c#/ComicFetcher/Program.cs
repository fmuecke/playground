using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;



namespace fetcher
{
    class Program
    {
        [DllImport( "msvcrt.dll" )]
        static extern bool system( string str );

        static void Main( string[] args )
        {
            bool force = false;
            foreach ( string s in args )
            { if ( s.CompareTo( "-f" ) == 0 ) force = true; }
            int failCount = 0;
            int existCount = 0;
            // http://images.ucomics.com/comics/ch/2007/ch071209.gif
            // http://www.jratt.com:88/comics/calvinandhobbes/calvinandhobbes_2004-07-27.gif
            // http://cserver.king-online.com/content/Hagar_The_Horrible?date=2004-11-11&referer=http://www.dailyink.com&uid=xvkv6o&token=r5be25
            // http://cserver.king-online.com/content/Phantom?date=2007-12-12&referer=http://www.dailyink.com&uid=xvkv6o&token=r5be25
            // http://cserver.king-online.com/content/Spiderman?date=2007-12-12&referer=http://www.dailyink.com&uid=xvkv6o&token=r5be25
            string name = "hagar";
            System.DateTime endDate = new DateTime( 1980, 1, 1 );
            for ( System.DateTime day = System.DateTime.Today; !day.Equals( endDate ); day = day.Subtract( new TimeSpan( 1, 0, 0, 0 ) ) )
            {
                string yStr, mStr, dStr, fName;
                yStr = day.Year.ToString().Substring( 2 ).ToString();
                mStr = ( day.Month < 10 ) ? "0" + day.Month.ToString() : day.Month.ToString();
                dStr = ( day.Day < 10 ) ? "0" + day.Day.ToString() : day.Day.ToString();
                //fName = name + yStr + mStr + dStr + ".gif";
                fName = name + "_" + day.Year + "-" + mStr + "-" + dStr + ".gif";
                if( System.IO.File.Exists( fName ) ) existCount++;
                //else if ( system( "wget http://www.jratt.com:88/comics/" + name + "/" +/* day.Year + "/" +*/ fName ) )
                else if( system( "wget \"http://cserver.king-online.com/content/Spiderman?date=" + day.Year + "-" + day.Month + "-" + day.Day + "&referer=http://www.dailyink.com&uid=xvkv6o&token=r5be25\" -O ph" + yStr + mStr + dStr + ".gif" ) ) ;
                    failCount++;
                if ( failCount > 50 && !force )
                {
                    System.Console.WriteLine( "\nERROR: files not found > 10 -> aborting..." );
                    break;
                }
                if ( existCount > 100 && !force )
                {
                    System.Console.WriteLine( "\nERROR: files existing > 5 -> aborting..." );
                    break;
                }

            }
            system( "pause" );
        }
    }
}
