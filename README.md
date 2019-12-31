# BlogCrawler

## 사용 라이브러리

HTTP Module : Boost 1.71.0 (asio)

HTML5 parser Module : gumbo
XML parser Module로 pugixml, tinyxml2, libxml2, rapidxml 등을 이용해보았음.
하지만 xml parser는 HTML5 태그를 지원하지 않음. (syntax error)

- example
```html
<meta charset="UTF-8">
<br>
```
위와 같이 닫는 태그가 없는 경우 xml 문법 오류가 발생함.

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

selector archives : **"#archives-4 > ul > li"**

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

---
2. devblogs.microsoft.com/oldnewthing/

reponse body(html)에 \n00006000\n 형식으로 비정상 적인 값이 들어있는 케이스가 존재.
