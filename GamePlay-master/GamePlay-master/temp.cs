using System;
using System.Net;
using System.ComponentModel;
class Program
{
    static string file = "gameplay-deps.zip";
    static string url = "https://github.com/gameplay3d/GamePlay/releases/download/v3.0.0/" + file;
    static bool done = false;
    static void Main(string[] args)
    {
        try
        {
            WebClient client = new WebClient();
            client.Proxy = null;
            client.DownloadProgressChanged += new DownloadProgressChangedEventHandler(DownloadProgressChanged);
            client.DownloadFileCompleted += new AsyncCompletedEventHandler(DownloadFileCompleted);
            Console.Write("Downloading " + file + ": 0%    ");
            client.DownloadFileAsync(new Uri(url), file);
            while (!done) System.Threading.Thread.Sleep(500);
        }
        catch (Exception x)
        {
            Console.WriteLine("Error: " + x.Message);
        }
    }
    static void DownloadProgressChanged(object sender, DownloadProgressChangedEventArgs e)
    {
        Console.Write("\rDownloading " + file + ": " + e.ProgressPercentage + "%    ");
    }
    static void DownloadFileCompleted(object sender, System.ComponentModel.AsyncCompletedEventArgs e)
    {
        Console.WriteLine("\rDownloading " + file + ": Done.    ");
        done = true;
    }
}
