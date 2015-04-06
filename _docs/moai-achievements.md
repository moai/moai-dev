---
title: "Moai Achievements"
---

The Moai Cloud Achievement service allows users to define single or multi-step achievements and track individual user progress through them. It is a REST based service that supports two basic operations. POSTs can be made to record user progress in the user\_achievements table and GETs can be made to retrieve user progress. It requires one collection to be created and populated by the user. The collection should be named 'achievements' and reside in the dbnamespace configured by the calling client. The achievements collection is defined below in the database section of this document.

Recording progress is done by POSTing to http://services.moaicloud.com/moai/achievements/\<userid\>. The body of the POST needs to contain the achievementid and stepscompleted. The modified user\_achievement record is returned along with a special congrats flag if the user has completed the achievement for the first time.

Retrieving progress is done simply by making a GET request to http://services.moaicloud.com/moai/achievements/\<userid\>. A full list of all the game's achievements and the user's progress through each is returned.

Database
--------

#### Required Collections

**achievements** - Defines the achievements that can be earned

  Field             Type      Required
  ----------------- --------- ----------
  achievementid     string    yes
  postdescription   string    no
  predescription    string    no
  steps             integer   yes
  title             string    no

#### Created Collections

**user\_achievements** - Stores user progress through each achievement

  Field           Type      Description
  --------------- --------- --------------------------------------
  userid          string    Unique user identifier
  achievementid   string    achievements table record identifier
  completed       integer   Steps completed in the achievement

Making Requests
---------------

#### Recording User Progress

##### Request

**POST** - [http://services.moaicloud.com/moai/achievements/{userid}](http://services.moaicloud.com/moai/achievements/{userid})

:   userid - A unique identifier for the user

**BODY**

:   achievementid - string - Points to a record in the achievements collection to record progress for
:   stepscompleted - integer - The number of steps the user has completed in this achievement

##### Response

```js
{ 
  achievementid : string,
  completed : integer,
  congrats : boolean,
  postdescription : string,
  predescription : string,
  steps : integer,
  title : string,
  userid : string
}
```


:   **achievementid:** The record in the achievement table
:   **completed:** The number of steps the user has completed
:   **congrats:** [optional] Returned only if this is the first time the user completed the achievement
:   **postdescription:** Description to be displayed when the achievement is completed
:   **predescription:** Description to be displayed before an achievement is completed
:   **steps:** The total number of steps to complete this achievement
:   **title:** The title of this achievement
:   **userid:** The unique identifier provided for the user

#### Retrieving User Progress

##### Request

**GET** - [http://services.maoicloud.com/moai/achievements/{userid}](http://services.maoicloud.com/moai/achievements/{userid})

:   userid - A unique identifier for the user

##### Response

```js
{ 
  "Some Achievement ID" : {
    achievementid : string, matches "Some Achievement ID"
    completed : integer
    postdescription : string
    predescription : string
    steps : integer
    title : string
    userid : string
  },
  "Achievement 2 ID" : {
    Idem ...
  },
  More achievements...
}
```


:   **achievementid:** The record in the achievement table
:   **completed:** The number of steps the user has completed
:   **postdescription:** Description to be displayed when the achievement is completed
:   **predescription:** Description to be displayed before an achievement is completed
:   **steps:** The total number of steps to complete this achievement
:   **title:** The title of this achievement
:   **userid:** The unique identifier provided for the user

Examples
--------

#### Recording Progress

##### Request

**POST** - <http://services.moaicloud.com/moai/achievements/123UserId>

**BODY** achievementid = MyAchievement stepscompleted = 5

##### Response

```js
{
  userid : "123UserId", 
  completed : 5, 
  achievementid : "MyAchievement", 
  postdescription : "You Did That Thing", 
  predescription : "Do A Thing", 
  steps : 5, 
  title : "An Achievement", 
  congrats : true
}
```

#### Retrieving User Progress

##### Request

**GET** - <http://services.maoicloud.com/moai/achievements/123UserId>

##### Response

```js
{ 
  "MyAchievement" : {  
    achievementid : "MyAchievement", 
    completed : 5, 
    congrats : true,
    postdescription : "You Did That Thing", 
    predescription : "Do A Thing", 
    steps : 5, 
    title : "An Achievement", 
    userid : "123UserId"
  }, 
  "MyOtherAchievement" : {
    achievementid : "MyOtherAchievement", 
    completed : 0, 
    postdescription : "You Did Something Else", 
    predescription : "Do A Different Thing", 
    steps : 1,
    title : "A Different Achievement",
    userid : "123UserId"
  }
}
```


