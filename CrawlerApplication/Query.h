#ifndef QUERY_QUERY_H
#define QUERY_QUERY_H

constexpr char* const CHECK_TABLE_QUERY = R"d(
SELECT name 
FROM   sqlite_master 
WHERE  type IN( 'table', 'view' ) 
       AND name NOT LIKE 'sqlite_%' 
UNION ALL 
SELECT name 
FROM   sqlite_temp_master 
WHERE  type IN( 'table', 'view' ) 
ORDER  BY 1; 
)d";

constexpr char* const CREATE_ARTICLE_TABLE_QUERY = R"d(
CREATE TABLE ARTICLE(  
		ID INTEGER PRIMARY KEY AUTOINCREMENT,
		TITLE        VARCHAR(255)    NOT NULL,
		URL          VARCHAR(255)    NOT NULL,
		IMAGEPATH    VARCHAR(255)    NOT NULL,
		CONTENT      VARCHAR(10000)  NOT NULL
);
)d";

constexpr char* const INSERT_ARTICLES_QUERY = R"d(
INSERT INTO ARTICLE (TITLE,URL,IMAGEPATH,CONTENT)
VALUES 
   ("1","2","3","4");
)d";

constexpr char* const SELECT_ALL_ARTICLES_QUERY = R"d(
SELECT *
FROM ARTICLE;
)d";

#endif
