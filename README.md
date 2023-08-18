<h1>poddl - podcast downloader</h1>
<h2>a cross platform command line podcast downloader for batch downloading all, individual, or a range of podcast episodes from an RSS feed</h2>

How to use:  
> $ ./poddl http://url.to.rss /OutputPath

Optional arguments:  
-o = Output path (needed if arguments are passed)  
-l = Only display list of episodes  
-r = Download/List newest episodes first  
-i = Add episode index / number to file names   
-s = Use episode index / number as file names (nnn.ext)  
-z N = Zero pad index/number when -i or -s are used (default = 3 if N are left out)  
-n N = Download a single episode  
-n N-N = Download a range of episodes  
-h = Quit when first existing file is found  
-h "search string" = Quit when first existing file matches the input string

<h3>Precompiled binaries</h3>

<table>
  <tr><th>OS</th><th></th></tr>
  <tr>
    <td>
      Windows x64
    </td>
    <td>
      <a href="https://github.com/freshe/poddl/raw/master/bin/poddl-win-x64.zip">Download</a>
    </td>
  </tr>
  <tr>
    <td>
      Windows arm64
    </td>
    <td>
      <a href="https://github.com/freshe/poddl/raw/master/bin/poddl-win-arm64.zip">Download</a>
    </td>
  </tr>
  <tr>
    <td>
      Linux x64 (Ubuntu 20.04)
    </td>
    <td>
      <a href="https://github.com/freshe/poddl/raw/master/bin/poddl-ubuntu-x64.zip">Download</a>
    </td>
  </tr>
  <tr>
    <td>
      Linux x64 (Debian 11)
    </td>
    <td>
      <a href="https://github.com/freshe/poddl/raw/master/bin/poddl-debian-x64.zip">Download</a>
    </td>
  </tr>
</table>

Build from source:
> $ g++ *.cpp -O2 -std=c++11 -lcurl -o poddl

Website  
https://www.fredrikblank.com/poddl/

Podcast Archiver (for Mac)  
https://www.fredrikblank.com/podcast-archiver/

Podcast Saver (for iOS)  
https://www.fredrikblank.com/podcast-saver/
