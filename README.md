<h1>poddl - podcast downloader</h1>
<h2>poddl is a small cross platform command line tool for downloading all, individual, or a range of podcast episodes from an RSS feed</h2>

How to use:  
> $ ./poddl http://url.to.rss /OutputPath

Optional arguments:  
-l = Only display list of episodes  
-n [n] = Download a single episode  
-n [n-n] = Download a range of episodes
-s = Store files as nnn.<ext>, where nnn counts from 1..number of files

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
