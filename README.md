<h1>poddl - podcast downloader</h1>
<h2>a cross platform command line podcast downloader for batch downloading all, individual, or a range of podcast episodes from an RSS feed</h2>

<h3>Precompiled binaries</h3>

<table>
  <tr><th>OS</th><th></th></tr>
  <tr>
    <td>
      Windows
    </td>
    <td>
      <a href="https://github.com/freshe/poddl/raw/master/bin/poddl-win-x64.zip">Download</a>
    </td>
  </tr>
  <!--
  <tr>
    <td>
      Windows arm64
    </td>
    <td>
      <a href="https://github.com/freshe/poddl/raw/master/bin/poddl-win-arm64.zip">Download</a>
    </td>
  </tr> -->
  <tr>
    <td>
      Linux x64 (Ubuntu 20.04, 22.04)
    </td>
    <td>
      <a href="https://github.com/freshe/poddl/raw/master/bin/poddl-ubuntu-x64.zip">Download</a>
    </td>
  </tr>
  <tr>
    <td>
      Linux x64 (Debian 11, 12)
    </td>
    <td>
      <a href="https://github.com/freshe/poddl/raw/master/bin/poddl-debian-x64.zip">Download</a>
    </td>
  </tr>
</table>

<h3>Build from source</h3>

```bash
g++ *.cpp -O2 -std=c++11 -lcurl -o poddl
```

<h3>How to use</h3>

```bash
./poddl "https://url.to.rss" -o /OutputPath
```

<h3>Options</h3>

-o = Output path (needed if arguments are passed)  
-l = Only display list of episodes  
-r = Download/List newest episodes first  
-i = Add episode index / number to file names   
-s = Use episode index / number as file names (nnn.ext)  
-z N = Zero pad episode index / number when -i or -s are used (default = 3 if N are left out)  
-n N = Download a single episode  
-n N-N = Download a range of episodes  
-t N = Download only N episodes  
-h = Quit when first existing file is found  
-h "search string" = Quit when first existing file matches the input string

<h3>Examples</h3>

```bash
# Download all episodes
./poddl "http://url.to.rss" -o /OutputPath

poddl.exe "http://url.to.rss" -o C:\OutputPath
```

```bash
# Download latest episode
./poddl "http://url.to.rss" -r -t 1 -o /OutputPath

poddl.exe "http://url.to.rss" -r -t 1 -o C:\OutputPath
```

```bash
# Download episode 1 to 10
./poddl "http://url.to.rss" -n 1-10 -o /OutputPath

poddl.exe "http://url.to.rss" -n 1-10 -o C:\OutputPath
```

```bash
# Download episode 5
./poddl "http://url.to.rss" -n 5 -o /OutputPath

poddl.exe "http://url.to.rss" -n 5 -o C:\OutputPath
```

Website  
https://www.fredrikblank.com/poddl/

Podcast Archiver (for Mac)  
https://www.fredrikblank.com/podcast-archiver/

Podcast Saver (for iOS)  
https://www.fredrikblank.com/podcast-saver/
