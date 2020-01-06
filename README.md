# Crawler

The crawler is a Win 32 Application that searches `https://devblogs.microsoft.com/oldnewthing/` and `https://herbsutter.com/` using [boost asio](https://think-async.com/Asio/).

In addition, it has the following additional functions.
* The request and parsing parts were implemented in `parallel`.
* `Object-oriented` design makes it easy to add a web blog crawler and an api crawler.
* When searching, the virtual list is displayed even `if the number of items is large`.

Please check Project details for more information.



<br/><br/>

## Project Details

For details on project structure and technical design, please see the [Project Readme](/readme/Project_Readme.md).


<br/><br/>

## IDE

Microsoft Visual Studio 2017 version 15.9.11


<br/><br/>

## Fast Start

### Install Crawler

~~~
git clone https://github.com/seh9306/Crawler.git
~~~

### Submodule Update

~~~
git submodule init 
git submodule update
~~~

### Connect Dll Link

> It tells the `Crawler.exe` the path to the `sqlite.dll` file.


<br/><br/>

## Demo

### Gif

> Crawl

<p align="center">
   <img src="/md_images/crawl.gif" width="740px" height="383px"/>
</p>

> Seach

<p align="center">
   <img src="/md_images/search.gif" width="740px" height="383px"/>
</p>

> Crawl Complete

<p align="center">
   <img src="/md_images/complete.gif" width="740px" height="383px"/>
</p>
