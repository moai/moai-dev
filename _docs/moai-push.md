---
title: "Moai Push"
---

Moai Push consists of two seperate services Moai Push Service and Moai Push Tokens. Both services expose a REST interface while the Moai Push Service also exposes a dashboard plugin. The split in services was designed to help keep your data more secure. You can use the Moai Push Token service from your games to operate on individual tokens. In the event your client key is compromised the only exposure you have is at the token level. No one can send notifications through that API. The Moai Push Service on the other hand is more sensitive so we recomend that you do not embed it's client key within insecure code and that you call it using your secret key. The Moai Push Service gives you full control over Moai Push including updating and deleting tokens and channels as well as APIs for sending push notifications.

Moai Push Tokens
================

Moai Push Tokens provides api access to individual tokens. It is seperated from the main Moai Push Service as a means of limiting exposure to your data. This service is intended to be used in locations where the security of the code can not be guarenteed. An example url for this service would be:

[`https://services.moaicloud.com/moai/pushtokens/{token_id}?clientkey=myClientKey`](https://services.moaicloud.com/moai/pushtokens/{token_id}?clientkey=myClientKey)

The domain name is omited in the rest of the page.

Resources
---------

### Token Id

The token id endpoint is handled but the Moai Push Tokens service. It exposes GET, PUT, POST and DELETE operations for a token, allowing you to create, delete load and update a token. For a token to be valid it must contain a provider. To send to a token it must be subscribe to a channel. If no provided is included it defaults to 'APPLE'. Tokens may be subscribed to multiple channels but may only have a single provider.

[`https://`](https://)`.../moai/pushtokens/{token_id}`

  Verb     Params   Notes
  -------- -------- -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  GET               Returns the token
  PUT               Updates the token
  POST              Creates a new token. Post body should contain json with a provider ("APPLE", "GOOGLE" or "GCM") and a list of the channels to subscribe to. If no post body defaults to {provider:"APPLE",channels:["all"]}
  DELETE            Deletes the token

#### Common Operations

Let's Assume the Moai Ticker service has been installed and configured to operate with a dbnamespace of 'data'. Additionally through the dashboard plugin I have created some ticker headlines. Given those conditions below are some example interactions with the Moai Ticker service.

##### Create a Token

**`GET` `/moai/pushtokens/{token` `id}`**\
**`Body:`**` {provider:"GOOGLE",channels:["all","GoogleUsers"]}`\
```
curl -i -H "x-clientkey: myClientKey" `[`https://`](https://)`.../moai/pushtokens/{token_id}
HTTP/1.1 201 Created
content-type: application/json
content-length: 438
```

```js
{
  _id : "token id",
  provider : "GOOGLE",
  channels : ["all", "GoogleUsers"]
}
```

Moai Push Service
=================

Moai Push service exposes the full push capability of the Moai Platform. It contains REST APIs for starting push jobs, managing channels and tokens. It also exposes a dashboard plugin that provides a view into all your push jobs, an interface for uploading and panaging your push credentials and a push composer for creating new push jobs.

Resources
---------

### tokens

The tokens resource provides access to the complete list of tokens.

[`https://`](https://)`.../moai/pushservice/tokens`

  Verb   Params                                     Notes
  ------ ------------------------------------------ ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  GET    count,skip,limit,active,channel,provider   Returns all tokens. To only get a count of tokens set count=1. To limit the number of tokens returned set limit=<int>. To skip the first n tokens set skip=n. To only return active tokens set active=1. To limit results to certain channels or providers set those parameters

#### Common Operations

##### Load all active tokens

**`GET` `/moai/pushservice/tokens?active=1`**\
**`Body:`**` `\
```
curl -i -H "x-clientkey: myClientKey" `[`https://`](https://)`.../moai/pushservice/tokens?active=1
HTTP/1.1 200 Ok
content-type: application/json
content-length: 438
```

```js
[
  {
    _id : "token id",
    provider : "GOOGLE",
    channels : ["all","GoogleUsers"]
  },
  {
    _id : "token id 2",
    provider : "APPLE",
    channels : ["all"]
  }
]
```

### channels

The channels endpoint provides methods for manipulating the channels tokens are subscribed to. At this time you may only download the list of channels

[`https://`](https://)`.../moai/pushservice/channels`

  Verb   Params   Notes
  ------ -------- ------------------------
  GET             Returns all channe;s }

#### Common Operations

##### Get all Channels

**`GET` `/moai/pushservice/channels`**\
**`Body:`**` `\
```
curl -i -H "x-clientkey: myClientKey" `[`https://`](https://)`.../moai/pushservice/channels
HTTP/1.1 200 Ok
content-type: application/json
content-length: 438
```

```js
["all", "GoogleUsers"]
```

### channels/{id}

Provies methods for interacting with a single channle. Allows for the deletion and creation of channels.

[`https://`](https://)`.../moai/pushservice/channel/{channel}`

  Verb     Params   Notes               POST      Create a new channel
  -------- -------- ------------------- ------ -- ----------------------
  DELETE            Deletes a channel

#### Common Operations

##### Create a Channel

**`POST` `/moai/pushservice/channel/{channel}`**\
**`Body:`**` `\
```
curl -i -H "x-clientkey: myClientKey" `[`https://`](https://)`.../moai/pushservice/new-channel
HTTP/1.1 201 Created
content-type: application/json
content-length: 2
```

```js
{}
```

##### Delete a Channel

**`DELETE` `/moai/pushservice/channel/{channel}`**\
**`Body:`**` `\
```
curl -i -H "x-clientkey: myClientKey" `[`https://`](https://)`.../moai/pushservice/new-channel
HTTP/1.1 200 Ok
content-type: application/json
content-length: 2
```

```js
{}
```

### jobs

The jobs endpoint is the resource you use to create new push jobs. To successfully create a new push job you must first configure the service with push credentials as well as upload tokens and associate those tokens with channels. The configuration of the service must be done through the dashboard plugin.

Once the service is configured you can create push jobs by posting the data you wish to send.

To send push notifications to APPLE users you must provide an alert message. You may additionally provide extra json data that is provided to your application along with the alert. To use Apple's sandbox for sending the message add a key "use\_sandbox" set to true. Apple uses the alert as the text in the notification. At this time we do not support setting any of the other fields supported by Apple. For more information on Apple push refer to their [documentation](http://developer.apple.com/library/mac/#documentation/NetworkingInternet/Conceptual/RemoteNotificationsPG/ApplePushService/ApplePushService.html)

The json data to post for an Apple push is:

```js
{
  aps : {
    alert : "Text to Send", 
    <any additional json for your app>
  }
}
```

To send push notifications to GOOGLE C@DM or GCM (in alpha) users you must provide a collapse key. Google additionally allows you to provide arbitray key value pairs that are passed to your app with the message. These keys are set in the data section of the posted json. It is optional. The collapse key is not the message shown to the user, but rather a key used to combine multiple messages recieved by the user. For more information on Google push, or Cloud to Device Messaging (C2DM) please refer to their [documentation.](http://code.google.com/android/c2dm/) For information on GCM please see their [documentation](http://developer.android.com/guide/google/gcm/index.html). Moai Cloud GCM support is in alpha.

The json data to post for a Google push is:

```js
{
  c2dm : {
    collapse_key : "key", 
    data : {
      <key> : <value>, 
      <key2> :<value2> 
      ...
    }
  }
}
```

To send messages to both Apple and Google users you should inlcude both sets of data in the posted json:

```js
{
  aps : {
    alert : "Text to Send", 
    <any additional json for your app>
  }, 
  c2dm : {
    collapse_key : "key", 
    data : {
      <key> : <value>, 
      <key2> : <value2> 
      ...
    }
  }
}
```

The last piece of required data is the list of channels to send the messages to. You can optionally provide a name for the job. So the complete POST body should look like this:

```js
{
  aps : {
    alert : "Text to Send", 
    <any additional json for your app>
  }, 
  c2dm : {
    collapse_key : "key", 
    data : {
      <key> : <value>, 
      <key2> :<value2> 
      ...
    }
  },
  channels : ["channel1","channel2"],
  name : "My new push job"
}
```

[`https://`](https://)`.../moai/pushservice/jobs`

  Verb   Params   Notes
  ------ -------- -----------------------------------------------------------------------
  POST            Creates a push job. Post body should contain json as described above.

#### Common Operations

##### Create a Job

**`POST` `/moai/pushservice/jobs`**\
**`Body:`**`  {aps: {alert: "I'm in your phone!"}, c2dm: {collapse_key: "New Message", data: { title: "New Message", message: "I'm in your phone!"} }, channels: ["all"]}`\
```
curl -i -H "x-clientkey: myClientKey" `[`https://`](https://)`.../moai/pushservice/jobs
HTTP/1.1 200 OK
content-type: application/json
content-length: 438
```

```js
[
  {
    id : "4f3b092ba8ad2c7bf494a7e9", 
    status : "NEW", 
    aps : {alert : "I'm in your phone!"}, 
    channels : ["all"],
    provider: "APPLE"
  },
  {
    id : "6a5b092ba8ad2c7bf494a7e9", 
    status : "NEW", 
    c2dm : { collapse_key : "New Message", 
    data : { title: "New Message", message: "I'm in your phone!"} }, 
    channels : ["all"],
    provider: "GOOGLE",
  },
  {
    id : "2a5b092ba8ad2c7bf494a7e4", 
    status : "NEW", 
    c2dm : { collapse_key : "New Message", 
    data : { title: "New Message", message: "I'm in your phone!"} }, 
    channels : ["all"],
    provider: "GCM",
  }
]
```

The current valid statuses for jobs are NEW, ERROR, QUEUED, IN\_PROGRESS, COMPLETE. In the event a job has an error the error message will be displayed in the dashboard and available in the status\_message field. The system will create one job for each targeted push platform (APPLE, GOOGLE & GCM)

Dashboard Plugin
----------------

Return Codes
============

  Code   Name                    Description
  ------ ----------------------- -------------
  200    Ok                      
  201    Created                 
  400    Bad Request             
  401    Unauthorized            
  404    Not Found               
  405    Method Not Allowed      
  500    Internal Server Error   


