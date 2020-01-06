#ifndef QUERY_QUERY_H
#define QUERY_QUERY_H

namespace query
{

constexpr char* const BEGIN_TRANSACTION = "BEGIN TRANSACTION;";
constexpr char* const COMMIT = "COMMIT;";

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
		CONTENT      VARCHAR(10000)  NOT NULL,
		DATE         INTEGER NOT NULL
);
)d";

constexpr char* const INSERT_ARTICLES_QUERY = R"d(
INSERT INTO ARTICLE (TITLE,URL,IMAGEPATH,CONTENT,DATE)
VALUES 
   (?,?,?,?,?);
)d";

constexpr char* const SELECT_ALL_ARTICLES_QUERY = R"d(
SELECT *
FROM ARTICLE;
)d";

constexpr char* const SELECT_LIKE_ARTICLES_QUERY = R"d(
SELECT * 
FROM ARTICLE 
WHERE content LIKE '%)d";

constexpr char* const SELECT_ORDER_BY_MATCH_COUNT_ARTICLES_QUERY = R"d(
select *,
(length(content) - length(replace(content, ?, ''))) / length( ? ) as match_count
from ARTICLE
where match_count != 0
order by match_count desc, date desc;)d";

constexpr char* const SELECT_LIKE_ARTICLES_QUERY_END = "%';";

}

#endif
