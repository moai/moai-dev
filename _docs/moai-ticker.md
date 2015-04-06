---
title: "Moai Ticker"
---

Moai Ticker exposes a REST interface and a dashboard plugin that allows you to easily populate and serve news tickers within your game. To use this service you must first create a client key and add the service to it. A signature is not required but we do recommend using one. We also recommend accessing this service over https.

Example URL
-----------

Moai Ticker can be accessed like any other Moai Cloud Service. An example url for this service would be:

[`https://services.moaicloud.com/moai/ticker/news?clientkey=myClientKey`](https://services.moaicloud.com/moai/ticker/news?clientkey=myClientKey)

The domain name is omited in the rest of the page.

Resources
---------

### News

The news endpoint is the only endpoint this service exposes. It exposes the collection of ticker headlines that you have defined within the dashboard plugin. You can limit the results to headlines that were tagged with a certain value by providing the tags parameter. The tags parameter should be a comma delimited list of tags to search for.

[`https://`](https://)`.../moai/ticker/news`

  Verb   Params   Notes
  ------ -------- --------------------------------------------------------------------------
  GET    tags     Returns a list of all ticker headlines, or just those tagged with 'tags'

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

Let's Assume the Moai Ticker service has been installed and configured to operate with a dbnamespace of 'data'. Additionally through the dashboard plugin I have created some ticker headlines. Given those conditions below are some example interactions with the Moai Ticker service.

### Retrieve All Headlines

**`GET` `/moai/ticker/news`**\
**`Body:`**` None`\
```
curl -i -H "x-clientkey: myClientKey"  `[`https://`](https://)`.../moai/ticker/news
HTTP/1.1 200 OK
content-type: application/json
content-length: 438
```

```js
[
  { id : "4f3b092ba8ad2c7bf494a7e9",
  text : "Nothing to fit fresh prince - this takes you to the item store",
  action : "openItemStore"
  },
  {
    id : "4f3b095461fff38b403443e1", 
    text : "You wanna see our app in the market?? Click here!",
    action : "openMarket"
  }
]
```

### Retrieve All Headlines Tagged with 'Android'

**`GET` `/moai/ticker/news`**\
**`Body:`**` None`\
```
curl -i -H "x-clientkey: myClientKey"  `[`https://`](https://)`.../moai/ticker/news?tags=Android
HTTP/1.1 200 OK
content-type: application/json
content-length: 238
```

```js
[
  {
    id : "4f3b095461fff38b403443e1",
    text : "You wanna see our app in the market?? Click here!",
    action : "openMarket"
  }
]
```

Dashboard Plugin
----------------

This service adds a plugin to your dashboard after you associate it with a client key. From within the plugin you can define new actions, create headlines and associate them with actions and tags. An action is a game specific identifier and is something you can use to drive a particular action, like opening your virtual item store, when a user sees or interacts with your news headline. You can also optionally associate a value with a headline so you can use a generic action like 'openURL' and have different headlines lead to different urls.
