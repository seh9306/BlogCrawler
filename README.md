# BlogCrawler

## 사용 라이브러리

HTTP Module : Boost 1.71.0 (asio)

XML Parser Module : pugixml

## 크롤링 대상 사이트 분석

대상 사이트를 요청하면 response body에 불필요한 데이터가 포함되어 있어 걸러주어야함.
1. herbsutter.com/

- Article list ( path = "/" )
```html
<!DOCTYPE HTML>
<html>
    ...
    <aside id="archives-4" ...>
        ...
        <ul>
            <li>
                <a href="https://herbsutter.com/2019/10">October 2019</a>
            </li>
            ...
        </ul>
    </aside>
</html>
```

xPath of archives : **//*[@id="archives-4"]**

- Article according to date ( path = "/[year]/[month]")

```html
<!DOCTYPE HTML>
<html>
    ...
    <main id="main">
        ...
       <article id="post-3799" ...>
           <header class="entry-header">
	    		<h1 class="entry-title">
                    <a href="https://herbsutter.com/2019/09/26/my-cppcon-2019-talk-video-is-online/">
                    <!-- title -->
                    </a>
                </h1>		
            </header>
            ...
       </article>
       <article id="post-3786" ...>
           <!-- Article content -->
       </article>
       <article id="post-3780" ...>
           <!-- Article content -->
       </article>
        ...
    </main>
</html>
```

xPath of articles : **//*[@id="main"]/article**

xPath of article[i]'s url : **//*[@id="main"]/article[i]/header/h1/a**

- Article ( path = "/[year]/[month]/[title]")

```html
<!DOCTYPE HTML>
<html>
    ...
    <main id="main">
        ...
       <article id="post-3799" ...>
            ...
            <div class="entry-content">
                <!-- Article content -->
        	</div>
       </article>
    </main>
</html>
```

xPath of article : **//*[@id="main"]/article**

---