---
title: "Moai Leaderboard"
---

The MoaiCloud Leaderboard service is a general purpose leaderboard. It is a REST based service that currently supports a few basic operations. POSTs can be made to a board to record scores and GETs can be used to either retrieve a desired set of scores or scores close to a player's best.

Adding scores is done by POSTing to http://services.moaicloud.com/moai/leaderboard/\<board\>. If the board doesn't exist it will be created. The body of the request should contain a userid, a username and the score to be posted. The newly created score record will be returned along with the position it holds in the board. A few additional options can be set passing unique equal to true in the body ensures that each userid can only have 1 entry on each board. By passing a sort=asc the score returned will have it's position determined by sorting scores from lowest to highest.

Retrieving scores is done by making a GET request to http://services.moaicloud.com/moai/leaderboard/\<board\>. Scores are ranked with the highest first unless a sort=asc is provided. sort=desc can be provided to explicitly ask for scores to be ranked high to low. There are two ways to retrieve scores. By passing a 1-based 'page' number and a pagesize a board can be paged through from start to finish. If a userid is included the user's score will also be returned. By passing neighborhood=true the scores closest to the provided userid can be retrieved. By passing before and after counts a number of scores before and after the user's will be returned. If the user does not have an entry on the board the top before+after+1 scores will be returned.

Database
--------

#### Created Collections

**scores** - Stores score information for all boards

  Field      Type      Description
  ---------- --------- -----------------------------
  username   string    A user provided name
  score      integer   Recorded score
  date       integer   GMT seconds since the epoch
  userid     string    Unique user identifier

Examples
--------

#### Recording a Score

##### Request

**POST** - <http://services.moaicloud.com/moai/leaderboard/MyLeaderboard>

**BODY**

```js
{ 
  "userid": "123XYZ"
  "username": "JoePlayer"
  "score": 6578
  "sort": "desc"
  "unique": "true"
}
```

##### Response

```js
{ 
  userid:"123XYZ", 
  username:"JoePlayer", 
  board:"MyLeaderboard", 
  score:6578, 
  date:12345677, 
  rank:76 
}
```

#### Retrieving a Score

##### Request

**GET** - <http://services.moaicloud.com/moai/leaderboard/MyLeaderboard?userid=123XYZ&pagesize=4&page=2&sort=desc>

##### Response

```js
{
  "5":{userid:"999XYZ", username:"JoePublic",
    board:"MyLeaderboard", score:6578, date:12345677, rank:5},

  "6":{userid:"123ABC", username:"JanePlayer",
    board:"MyLeaderboard", score:6577, date:13345680, rank:6}, 

  "7":{userid:"456XYZ", username:"JoeWinner",
    board:"MyLeaderboard", score:6576, date:12345612, rank:7}, 

  "8":{userid:"456ABC", username:"JoeLoser",
    board:"MyLeaderboard", score:6575, date:19345677, rank:8},

  "76":{userid:"123XYZ", username:"JoePlayer",
    board:"MyLeaderboard", score:65, date:12345677, rank:76}

}
```

#### Retrieving Neighborhood Scores

##### Request

**GET** - <http://services.moaicloud.com/moai/leaderboard/MyLeaderboard?userid=123XYZ&after=2&before=2&sort=desc>&:neighborhood=true

##### Response

```js
{
  "74":{userid:"999XYZ", username:"JoePublic", 
    board:"MyLeaderboard", score:67, date:12345677, rank:74},

  "75":{userid:"123ABC", username:"JanePlayer", 
    board:"MyLeaderboard", score:66, date:13345680, rank:75}, 

  "77":{userid:"456XYZ", username:"JoeWinner", 
    board:"MyLeaderboard", score:64, date:12345612, rank:77}, 

  "78":{userid:"456ABC", username:"JoeLoser", 
    board:"MyLeaderboard", score:63, date:19345677, rank:78},

  "76":{userid:"123XYZ", username:"JoePlayer", 
    board:"MyLeaderboard", score:65, date:12345677, rank:76}
}
```


