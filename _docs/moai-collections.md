---
title: "Moai Collections"
---

Moai Collections exposes a REST interface to your mongo database. To use this service you must first create a client key and add the service to it. A signature is not required but we do recommend using one. We also recommend accessing this service over https.

Example URL
-----------

Moai Collections can be accessed like any other Moai Cloud Service. An example url for this service would be:

[`https://services.moaicloud.com/moai/collections?clientkey=myClientKey`](https://services.moaicloud.com/moai/collections?clientkey=myClientKey)

Resources
---------

### Collections

The collections endpoint provides visiblity into the available collections

URI /moai/collections

  Verb   Params   Notes
  ------ -------- -----------------------------------
  GET             Returns a list of all collections

### Collection

The collection endpoint allows you to find or create objects

URI /moai/collections/{collection}

  Verb   Params       Body                Notes
  ------ ------------ ------------------- -------------------------------------------------------------------------------------------------------------------------------------------------------------------
  GET    q,l,sk,c,s                       Searches through {collection} and returns a JSON array of the objects it finds
  POST   upsert       JSON encoded data   Saves the JSON encoded POST body and stores it in {collection}. Returns the id of the new object. Use 'upsert' as a query param to update the object if it exists

**Params**

  -------- ----------------------------------------------------------------------------------------------------------------------
  q        A mongo query to execute
  l        An integer to limit the returned number of records
  sk       An integer specifying the number of records to skip before starting to return.
  c        Return just the count of matching records
  s        A sort query to apply. Provide keys and 1 for descending, -1 for ascending e.g. {key1:1,key2:-1}
  upsert   A flag telling the service to insert the object if it does not exist. Can be set to any value. (i.e. 'true', 1, yes)
  -------- ----------------------------------------------------------------------------------------------------------------------

### Object

The object endpoint allows you to interact with an individual object in your mongo db.

URI /moai/collections/{collection}/{id}

  Verb     Params   Body                Notes
  -------- -------- ------------------- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  GET                                   Returns the record identified by {id} in {collection}
  DELETE                                Deletes the record identified by {id} in {collection}
  PUT      upsert   JSON encoded data   Replaces the record identified by {id} in {collection} with the JSON encoded POST body. Any \_id specified in the POST body will be replaced with {id}. If the object does not exist it will be created if the upsert param is set on the request

**Params**

  -------- ----------------------------------------------------------------------------------------------------------------------
  upsert   A flag telling the service to insert the object if it does not exist. Can be set to any value. (i.e. 'true', 1, yes)
  -------- ----------------------------------------------------------------------------------------------------------------------

Return Codes
------------

  Code   Name                    Description
  ------ ----------------------- -------------
  200    Ok                      
  201    Created                 
  400    Bad Request             
  401    Unauthorized            
  404    Not Found               
  405    Method Not Allowed      
  500    Internal Server Error   

Common Operations
-----------------

Let's Assume the Moai Collections service has been installed and configured to operate with a dbnamespace of 'data'. Additionally within my mongo database I have a collection called `data.objects`. Given those conditions below are some example interactions with the Moai Collections service.

### List Collections

**`GET` `/moai/collections`**\
**`Body:`**` None`\
`curl -i -H "x-clientkey: myClientKey" `[`https://services.moaicloud.com/moai/collections`](https://services.moaicloud.com/moai/collections)\
```
HTTP/1.1 200 OK
content-type: application/json
content-length: 11
```

```js
 ["objects"]
```

### Create an Object

**`POST` `/moai/collections/objects`**\
**`Body:`**` JSON data`\
`curl -i -X POST -d "{name:'new-object'}" -H "x-clientkey: myClientKey" `[`https://services.moaicloud.com/moai/collections/objects`](https://services.moaicloud.com/moai/collections/objects)\
```
HTTP/1.1 201 Created
content-type: application/json
content-length: 32
```

```js
 {_id:"4f0624a5c06c3bc2155de2b5"}
```

### Load an Object

**`GET` `/moai/collections/objects`**\
**`Body:`**` None`\
`curl -i -H "x-clientkey: myClientKey" `[`https://services.moaicloud.com/moai/collections/objects/4f0624a5c06c3bc2155de2b5`](https://services.moaicloud.com/moai/collections/objects/4f0624a5c06c3bc2155de2b5)\
```
HTTP/1.1 200 OK
content-type: application/json
content-length: 56
```

```js
 {"name":"new-object","_id":["4f0624a5c06c3bc2155de2b5"]}
```

### Delete an Object

**`DELETE` `/moai/collections/objects/`**\
**`Body:`**` None`\
`curl -i -X DELETE -H "x-clientkey: myClientKey" `[`https://services.moaicloud.com/moai/collections/objects/4f0624a5c06c3bc2155de2b5`](https://services.moaicloud.com/moai/collections/objects/4f0624a5c06c3bc2155de2b5)\
```
HTTP/1.1 200 OK
content-type: application/json
content-length: 4
```

```js
 "{}"
```

### Update an Object

**`PUT` `/moai/collections/objects`**\
**`Body:`**` JSON data`\
`curl -X PUT -i -d "{name:'updated-object'}" -H "x-clientkey: myClientKey" `[`https://services.moaicloud.com/moai/collections/objects/4f0624a5c06c3bc2155de2b5`](https://services.moaicloud.com/moai/collections/objects/4f0624a5c06c3bc2155de2b5)\
```
HTTP/1.1 200 OK
content-type: application/json
content-length: 4
```

```js
 "{}"
```

### Search for Object by 'name'

**`GET` `/moai/collections/objects`**\
**`Body:`**` None`\
```
curl -i -H "x-clientkey: myClientKey" `[`https://services.moaicloud.com/moai/collections/objects?q`](https://services.moaicloud.com/moai/collections/objects?q)`={name:'updated-object'}
HTTP/1.1 200 OK
content-type: application/json
content-length: 62
```

```js
 [{"name":"updated-object","_id":["4f0624a5c06c3bc2155de2b5"]}]
```

### Get a Count of all Objects

**`GET` `/moai/collections/objects`**\
**`Body:`**` None`\
`curl -i -H "x-clientkey: myClientKey" `[`https://services.moaicloud.com/moai/collections/objects?c=1`](https://services.moaicloud.com/moai/collections/objects?c=1)\
```
HTTP/1.1 200 OK
content-type: application/json
content-length: 1
```

```js
 1
```

### Get First 10 Objects in a Collection

**`GET` `/moai/collections/objects`**\
**`Body:`**` None`\
`curl -i -H "x-clientkey: myClientKey" `[`https://services.moaicloud.com/moai/collections/objects?l=10`](https://services.moaicloud.com/moai/collections/objects?l=10)\
```
HTTP/1.1 200 OK
content-type: application/json
content-length: 62
```

```js
 [{"name" : "updated-object", "_id" : ["4f0624a5c06c3bc2155de2b5"]}]
```

### Get Next 10 Objects in a Collection

**`GET` `/moai/collections/objects`**\
**`Body:`**` None`\
`curl -i -H "x-clientkey: myClientKey" `[`https://services.moaicloud.com/moai/collections/objects?sk=10&l=10`](https://services.moaicloud.com/moai/collections/objects?sk=10&l=10)\
```
HTTP/1.1 200 OK
content-type: application/json
content-length: 2
```

```js
 {}
```


