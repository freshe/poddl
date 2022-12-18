# poddl - podcast downloader
*poddl is a small cross platform command line tool for downloading all, individual, or a range of podcast episodes from an RSS feed.*

How to use:  
> $ ./poddl http://url.to.rss /OutputPath

Optional arguments:  
-l = Only display list of episodes  
-n [n] = Download a single episode  
-n [n-n] = Download a range of episode

How to build:
> $ g++ *.cpp -O2 -std=c++11 -lcurl -o poddl

Precompiled binaries  
<sub><sup>Windows x64</sup></sub>  
https://github.com/freshe/poddl/raw/master/bin/poddl-win-x64.zip  
<sub><sup>Linux x64 (Tested on Debian 11)</sup></sub>  
https://github.com/freshe/poddl/raw/master/bin/poddl-linux-x64.zip

Website  
https://www.fredrikblank.com/poddl/

Podcast Archiver (for Mac)  
https://www.fredrikblank.com/podcast-archiver/

Podcast Saver (for iOS)  
https://www.fredrikblank.com/podcast-saver/