---
title: "Getting Started with Moai Cloud"
---

### Moai Cloud Services

A Moai Cloud service is, at its core, a handler for HTTP web requests. Any HTTP(S) capable web client such as a web browser or a mobile application created with the Moai SDK may communicate with a Moai Cloud service. (see: MOAIHttpTask and Moai Cloud Basics Part 2)

### main.lua

Every Moai Cloud web service must contain at least a single Lua script file named main.lua with a main( ) function defined. Moai Cloud services may contain any number of files but the file types are limited to .lua script files and .png image files.

When a request is received by Moai Cloud for a given service, the request parameters will be packaged in a Lua table and passed in as an argument to the main( ) function. If you use the "hello world" template when creating a Moai Cloud service, this file will be created for you.

As many other functions and variables as necessary may be defined in main.lua but it must contain at least the main( ) function which in all cases returns a response to the calling client.

**`main(web,` `req)`**` - The entry point for all Moai Cloud service requests`\
```
       `**`web`**` - a Lua object containing various methods for managing replies to service requests 
                     (see web object) and convenience methods for inspecting the HTTP request.
       `**`req`**` - a Lua table containing the raw HTTP request headers. (see sample below)
```

### Web Object

The ‘web’ parameter passed to main( ) is a Lua object containing various methods for managing replies to service requests:

**`web:params()`**` - returns a Lua table containing the HTTP request parameters.`

**`web:path()`**` - returns the path portion of the requested URL`

**`web:method()`**` - returns the HTTP method used to make the request (e.g. GET, POST, PUT, DELETE)`

**`web:request_is_json()`**` - returns a flag indicating if the HTTP Content-Type header in the request was set to ‘application-json’`

**`web:request_is_xhr()`**` - returns a flag indicating if the HTTP x-requested-with header was set to XMLHttpRequest`

**`web:zap_session()`**` - resets the session cookie`

**`web:set_cookie(cookie)`**` - sets the content of the ‘set-cookie’ value in the response header. See `[`HTTP` `Cookies`](http://en.wikipedia.org/wiki/HTTP_cookie#Setting_a_cookie)`.`\
`       `**`cookie`**` - a string containing the contents of the cookie to set`

**`web:get_cookie()`**` - returns the value of the ‘cookie’ HTTP header. See HTTP Cookies.`

**`web:session()`**` - returns the HTTP session ID as provided by the HTTP request header`

**`web:send(data)`**` - sends a JSON encoded document to the requesting client with no additional response headers.`\
`       `**`data`**` - a Lua table that should be encoded to JSON and returned to the client.`

**`web:close()`**` - closes the request without sending a response.`

**`web:json(data` `[,` `ctype])`**` - sends a JSON encoded document to the requesting client with an HTTP response code of 200 (success).`\
```
       `**`data`**` - a Lua table that should be encoded to JSON and returned to the client.
       `**`ctype`**` - (optional) a string indicating the MIME type of the response. The default value is ‘application/json’.
```

**`web:redirect(url)`**` - Sends an HTTP response code of 303 indicating that the calling client should refer to the passed URL instead.`\
`       `**`url`**` - the URL that should be used instead.`

**`web:error(data,` `code,` `status` `[,headers])`**` - reports an error and then closes the connection.`\
```
       `**`data`**` - a string containing the response body
       `**`code`**` - a number containing the desired HTTP response code
       `**`status`**` - a string representing the HTTP response status. Typically set to ‘OK’
       `**`headers`**` - a Lua table containing any custom response headers to return (e.g. content-type)
```

**`web:not_found([msg])`**` -returns an HTTP 404 with the specified message. If msg is not specified, `\
```
                                        ‘Not Found’ will be returned as the response body.
       `**`msg`**` - the message to return with the response.
```

**`web:unauthorized([msg])`**` -returns an HTTP 401 with the specified message. If msg is not specified, `\
```
                                             ‘Unauthorized’ will be returned as the response body.
       `**`msg`**` - the message to return with the response.
```

**`web:forbidden([msg])`**` -returns an HTTP 403 with the specified message. If msg is not specified, `\
```
                                        ‘Forbidden’ will be returned as the response body.
       `**`msg`**` - the message to return with the response.
```

```
'''web:bad_request([msg]) -returns an HTTP 400 with the specified message. If msg is not specified, 
                                          ‘Not Found’ will be returned as the response body.
       `**`msg`**` - the message to return with the response.
```

```
'''web:page(data, code, status, headers) - returns an HTTP response to the caller
       `**`data`**` - a string containing the response body
       `**`code`**` - a number containing the desired HTTP response code
       `**`status`**` - a string representing the HTTP response status. Typically set to ‘OK’
       `**`headers`**` - a Lua table containing any custom response headers to return (e.g. Content-Type, Content-Length)
```

```
 `**`web:ok(msg)`**` - a convenience method that will return a message to the client with an HTTP response code of 200 ‘OK’
       `**`msg`**` - (optional) a string representing the response body. If not specified ‘OK’ will be used.
```

### Request Parameters

Frequently when an HTTP request is made, the URL will contain optional request parameters (a.k.a. query string). These parameters occur at the end of the URL and are ‘&’ delimited.

The query string parameters and values as well as posted form data are parsed and inserted as a table into the req object passed to main(...). This table can be accessed via req.params. The table will contain key value pairs corresponding to the arguments passed on the query string.

Alternately, a client may use the HTTP POST method to send data to the service. POSTed parameters are also available via req.params.

This raw query string is also available via req.headers.QUERY. Raw POSTed data is available via req.body

Sample Request Header

```
conn_id = "4"
sender = "C33AD7EE-C034-4629-BD6F-1AFB3540AEB0" 
path = "/hello_world"
session_id = "APP-85b4838e22218ec8d04c9ea9f8dd1415"
body = ""
data = { }
headers = {
connection = "keep-alive"
x-forwarded-for = "69.84.244.131"
accept = "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
PATTERN = "/hello_world()$"
PATH = "/hello_world"
user-agent = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_6_7) AppleWebKit/534.30 (KHTML, like Gecko) Chrome/12.0.742.91 Safari/534.30"
host = "services.moaicloud.com"
accept-charset = "ISO-8859-1,utf-8;q=0.7,*;q=0.3"
URI = "/hello_world?arg1=val1"
accept-language = "en-US,en;q=0.8"
accept-encoding = "gzip,deflate,sdch"
VERSION = "HTTP/1.1"
METHOD = "GET"
QUERY = "arg1=val1"
}
```

### Using Images

Moai Cloud only supports PNG type images. To include an image in your application resources, simply include your PNG file in the ZIP file you upload when creating your new service. The PNG file may exist along any path in the ZIP file. To return an image to a calling client you can use the moai.getfile(...) method, which will return the raw data of the image. This data can then be sent to the client along with the appropriate header. (see sample code below)

### Sample Code

#### Hello World

`'''function''' main(web, req)
     '''return''' web:ok(“hello moai cloud!”)
 '''end'''`

#### Dump Request Headers

`'''function''' main(web, req)
     '''return''' web:ok(moai.table_print(req))
 '''end'''`

#### Get an Image

`'''function''' main(web, req)
     '''local''' image_data = moai.getfile('moai_logo.png')
     '''local''' header = {}
     header['Content-Type'] = 'image/png'
     header['Content-Length'] = #image_data
     '''return''' web:page(image_data, 200, 'OK', header)
 '''end'''`
