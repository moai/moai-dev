---
title: "Using MongoDB With Moai Cloud"
---

The native data store for Moai Cloud applications is the NoSQL database MongoDB (www.mongodb.org). Unlike traditional relational databases with tables and fields, mongoDB is a ‘document store’ that can store and query JSON documents and/or serialized Lua tables directly without the need for an ORM.

Each Moai Cloud user account is allocated a private mongoDB database that is shared among all Moai Cloud services owned by that account. Your MongoDB database is exposed to Moai Cloud services via the mongodb object in the [Moai Cloud Lua Environment](moai-cloud-lua-environment.html).

Inserting a new document
------------------------

Inserting a new document into your mongodb database is accomplished by the `mongodb.insert(…)` method. The insert method takes two arguments, a namespace and a Lua table or JSON document to insert. For example, if you wish to insert a JSON document representing a user’s score for a game, you would do something like this:

```
mongodb:insert(‘high_scores', "{player_id:'1234', game_id:'7890', score:'102541'}")
```

Alternately, if you wanted to insert the same document using a Lua table, you could accomplish the same insert as above with the following code:

`local score = {player_id='1234', game_id='7890', score='102541'}
  
 mongodb:insert(‘high_scores', score)`

Tip: For all practical purposes, mongoDB does not support the notion of unique keys. Therefore, if you call insert(…) multiple times with the same value, two copies of the document will be added to the collection.

Finding documents
-----------------

Locating a document or documents in your mongodb database is done via the `mongodb.query(…)` method and a resulting cursor object. The `query(…)` method takes two arguments, a namespace as above, and a Lua table or JSON document describing in whole or in part the documents you wish to retrieve.

If you wanted to find all of the high score documents in the high\_scores collection belonging to a given user you could do the following:

```
local cursor = mongodb:query(‘high_scores', {player_id='1234'})
```

Once the query has executed, you can iterate over the results using the returned cursor object:

`while cursor:has_more() do
 
    local score = cursor:next()
 
    print( score.player_id )
 
    print( score.game_id )
 
    print( score.score )
 
 end`

If you want to retrieve scores for more than one user at a time, you can use the MongoDB `$or<code> keyword. For example:

{% raw %} <code>local cursor = mongodb:query(‘high_scores', {['$or'] =  {{player_id='1234'}, {player_id='2345'}})`{% endraw %}

More information about MongoDB queries and various keywords available can be found on the [Advanced Queries](http://www.mongodb.org/display/DOCS/Advanced+Queries) page on the MongoDB website.

Removing documents
------------------

Removing a document from your mongoDB database is accomplished by the `mongodb.remove(…)` method. The remove method takes three arguments, a namespace, a query and a flag. The namespace argument is identical to that described above. The query is a Lua table or JSON document that describes in whole or in part the document(s) you wish to remove from a collection. The flag (‘justOne’) when set to true will cause the driver to remove at most one document satisfying the query. If this flag is set to false, all documents satisfying the query will be removed.

Given the document structure described in “Inserting a new document”, if you wanted to remove all scores in the high\_scores collection for game\_id ’7890′ you could do the following:

```
mongodb:remove(‘high_scores', {game_id='7890'}, false)
```

If you wanted to remove only the high scores for player\_id ’1234′ for game\_id ’7890′ you could do the following:

```
mongodb:remove(‘high_scores', {player_id='1234', game_id='7890'})
```

Updating documents
------------------

Updating a document in your mongoDB database is accomplished, unsurprisingly, by the `mongob.update(…)` method. Similar to `remove(…)`, `update(…)` takes a namespace and a query as it’s first two arguments. The third argument, “modifier” is a Lua table or JSON document describing the update to make to the document or documents satisfying the query. Caveat: By default, an update will replace the document or documents satisfying the query with the contents of “modifier”.

The fourth argument “upsert” if true will instruct the driver to insert a new document described by the “modifier” argument if no other documents satisfy the specified query. Finally, the fifth argument “multi”, if false, will instruct the driver to either update only the first document found that satisfies the query. If “multi” is true, all documents satisfying the query will be updated.

If you wish to replace the high score entry described above with a new one, you could do the following:

`mongob:update(‘high_scores', {player_id='1234', game_id='7890', score='102541'}, 
                                       {player_id='1234', game_id='9990', score='6540'}, false, false)`

Replacing an entire document however, is rarely what you want. Typically one wants to update only a single field in a given document. To accomplish this, you can either describe the entire document to update as the “modifier” or you can use the mongoDB keyword \$set to update only a subset of fields. For example, if you wish to update only the high score based on player\_id and game\_id, you could do the following:

```
mongodb:update(‘high_scores', {player_id='1234', game_id='7890'}, {['$set']={score='200050'}}, false, false)
```

Dropping collections
--------------------

Typically you would remove a collection by selecting Drop in the RockMongo Database Admin. You can do it in the lua sandbox, however, but be careful you do not drop your gridfs collections (fs.files, fs.chunks) or your users collection. One will remove your code, the other your ability to log in. To output the collection names:

```
mongodb:list_collections()
```

To drop a collection named "rooms":

```
mongodb:drop_collection( 'rooms' )
```

Indexes
-------

Indexes enhance query performance, often dramatically. It's important to think about the kinds of queries your application will need so that you can define relevant indexes. Once that's done, actually creating the indexes in MongoDB is relatively easy and we provide two distinct ways of doing that. One way is through the database admin discussed at the end of this document, the other is through the sandbox methods ensure\_index and get\_indexes. We currently do not expose an API for dropping indexes.

```
mongodb:ensure_index(collection, keys, unique, name)
```

```
collection - The collection the index should be created on
keys - The keys to index {key1=1, key2=1}
unique - Should the index enforce uniqueness
name - The name for the index
```

Returns true the first time it is called, false afterwards.

```
mongodb:get_indexes(collection)
```

Returns all indexes on the collections

Namespaces
----------

Many of the MongoDB methods take a namespace as a parameter. A MongoDB namespace in Moai Cloud is effectively name of a collection. (e.g. high\_scores). Managing namespace names within your services to prevent unintentional collection clashes is your responsibility.

By using a client key when calling services, the consumer can specify a namespace prefix to append to all of the service's tables. This allows the consumer to guard against naming clashes between different services, but also allows for data sharing when desired.

Under the Hood
--------------

In the Moai Cloud environment, mongoDB services are provided using the open source driver, [luamongo](http://www.github.com/moai/luamongo). For more information about what methods and functionality are exposed via the luamongo driver, visit the [project page on GitHub](http://www.github.com/moai/luamongo).

Administering Your Database
---------------------------

On the Moai Cloud dashboard, you can find a link for “Database Admin.” This link will open a mongoDB administrative interface called RockMongo that will allow you to view, modify, download, export and/or migrate your database data.
