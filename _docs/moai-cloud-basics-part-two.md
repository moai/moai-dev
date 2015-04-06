---
title: "Moai Cloud Basics Part Two"
---

This tutorial will show you how to create a simple Moai Cloud application that will change its content with input from the user, and also create a client program that will edit the application and print out the new content. This will require two separate lua files; first, the cloud application, and second, the client. The tutorial will be broken up into those two sections.

The steps that your client and application will go through once they finished are: the user will launch the script, the script will ask the user to input their name, after the user inputs their name the program will then send that data to the cloud application, the application will overwrite whatever information was there before with the new input, then the application will send a string back to the script which will then take that information and open a text box with the string contents and print that to a window. That being said, the first piece of this puzzle that will be constructed is the Cloud Application.

Cloud Application
-----------------

Before getting started, make sure that you have at least skimmed over the Moai Cloud documents (Getting Started, MongoDB, Lua Environment) to get familiar with Moai Cloud and MongoDB. Now to begin, you’ll need to make another web service (just like in the previous Moai Cloud tutorial.) You can just use the Hello World Template, and once that’s been created, you’ll go into the resources and edit them. Go ahead and delete all the pre-placed Hello World code and replace it with the following. Don’t worry, this article will step through the code piece by piece.

```
function main(web, req)

   local params = web:params()
   local return_value = ""
 
   if( params.name ) then
       mongodb:update('names', {}, {name=params.name}, true, false)
       return_value = "Your name has been updated to:  " .. params.name
   else

       local cursor = mongodb:query('names', {})

       if( cursor:has_more() ) then
           local name_record = cursor:next()
           return_value = "You told us that your name is:  " .. name_record.name
       else
           return_value = "You haven't told us your name yet!"
       end 
   end
    
   web:page(return_value, 200, 'OK')
end
```

First off, as was stated in the the “Getting started with Moai Cloud” documentation, the application must feature a function defined with “main()”.

```
main ( web, req )
```

Web is a Lua object containing various methods for managing replies to service requests, and req is a Lua table containing raw HTTP request headers.

```
local params = web:params()
local return_value = ""
```

Next, create two local variables; a table of “web:params()” called “params”, and a “return\_value”, which will be used as a string when the messages are returned.

After that is where the main portion of the script comes into play.

```
if ( params.name ) then
```

will check if ant input has been received from the user; then,

```
mongodb:update('names', {}, {name=params.name}, true, false)
return_value = "Your name has been updated to:  " .. params.name
```

If there has been input, then mongodb will search for a table called “names”, and it will replace the first entry in this table with the params.name (our input from before). Now, if there isn’t a table already created, one will be automatically created on the fly for you. After this, the application will then print the string followed by the name you have just inserted into the table.

If you didn’t just pass through an argument, then the next bit will work instead.

```
local cursor = mongodb:query('names', {})
```

First a local cursor is cretaed as a mongodb query (read about queries and cursors in the MongoDB Moai Cloud documentation)

```
if( cursor:has_more() ) then
     local name_record = cursor:next()
     return_value = "You told us that your name is:  " .. name_record.name
```

With that cursor the program checks to see if there is an entry in the table. If there is an entry, it creates name\_record to be that string, and then like before, returns a string with the record.

And finally, if there hasn’t ever been an entry in the table, the program will return

```
return_value = "You haven't told us your name yet!"
```

After all that has been determined, you will make your page display the return\_value when a user accesses it.

And that’s a simple cloud application! After deploying the application, you can click on the URL given to you on the Moai Cloud Dashboard and you will be taken to your application’s page:

![]({{ site.baseurl }}/img/docs/CloudToo1.jpg)

Of course it hasn’t been given a name yet, and it says as much. If you remember from the “Getting Started with Moai Cloud” documentation, you can manually submit request parameters by simply typing in the URL followed by a question mark, the argument type, an equals sign, then the value of the argument. So you can input the following:

<http://YOUR_URL?name=Beta> User

Entering that URL will then take you to your new, freshly updated application.

![]({{ site.baseurl }}/img/docs/CloudToo2.jpg)

And now, if you reload just the URL (without the “?name=Beta User”) you’ll get

![]({{ site.baseurl }}/img/docs/CloudToo3.jpg)

because you have already submitted a name to the table.

Now that you have a working cloud application, it is time to create a client to take advantage of this application. This will be similar to the previous example from the Moai Cloud Basics tutorial, with the addition of user input.

Client
------

You will need the client to do the following things; Open up a window, accept user input, send that input to the cloud, then receive input from the cloud and open a textbox with that information. Previous tutorials have already covered how to open up a window and how to create a textbox, so this tutorial will just skip over those. One of the things you will need to create is a function that will encode input to the end of the URL, and then you will use that function with an httpGet. (remember httpGet from the previous Cloud tutorial?)

For the url encoding, use the encode function that is given on the Lua website [here](http://www.lua.org/pil/20.3.html); there’s no reason to reinvent the wheel.

```
function urlEncode ( t )
     local s = ""
          for k,v in pairs ( t ) do
          s = s .. "&" .. urlEscape ( k ) .. "=" .. urlEscape ( v )
     end
          return string.sub ( s, 2 ) -- remove first '&'
end
```

Once you have that, it’s time to get down to business. After the constants and window setup, this is the next bit of code to add.

```
io.write ("What is your name? ")
name = io.read ()
print ("Your name is " .. name )
 
function taskCallback ( task )
    print ( task:getString () )
    text = ( task:getString ())
    textbox:setString ( text )
    layer:insertProp ( textbox )
end
task = MOAIHttpTask.new ()
task:setCallback ( taskCallback )
task:httpGet ( MOAI_CLOUD_URL … “?" .. urlEncode ( { name = name } ))
```

The first chunk will take an input from the user and set that input to a variable called “name.” The second chunk will print the name onto the debug screen.

Now it should again look very familiar from the previous cloud tutorial, with one major change. On the httpGet function call, instead of only putting the MOAI\_CLOUD\_URL in there, you are also adding a question mark after the URL, and then using the urlEncode function to add your user input and send it up to your service.

When all this is added in, this is what you should get when you run the program.

![]({{ site.baseurl }}/img/docs/CloudToo4.jpg)

and if you go to the URL, you will get

![]({{ site.baseurl }}/img/docs/CloudToo5.jpg)

With that, you have not only successfully created a cloud application that will take in variables given to it by a user, but will display those arguments, and on top of that create a client that will take advantage of this application.

Listed below is the complete code for the client.

```
MOAI_CLOUD_URL = "YOUR_URL"
    
MOAISim.openWindow ( "Textboxes", 320, 480 )
    
viewport = MOAIViewport.new ()
viewport:setScale ( 320, 480 )
viewport:setSize ( 320, 480 )
    
layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )
    
---------------------------------------------------------------
function urlEncode ( t )    
   local s = ""

   for k,v in pairs ( t ) do
       s = s .. "&" .. urlEscape ( k ) .. "=" .. urlEscape ( v )
   end
 
   return string.sub ( s, 2 ) -- remove first '&'
end
 
charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'
font = MOAIFont.new ()
font:loadFromTTF ( 'arialbd.ttf', charcodes, 16, 163 )
       
textbox = MOAITextBox.new ()
textbox:setFont ( font )
textbox:setTextSize ( font:getScale ())
textbox:setRect ( -160, -80, 160, 80 )
textbox:setYFlip ( true )
    
io.write ("What is your name? ")
name = io.read ()
print ("Your name is " .. name )
    
function taskCallback ( task )
       
   print ( task:getString () )
   text = ( task:getString ())
   textbox:setString ( text )
   layer:insertProp ( textbox )
end
    
task = MOAIHttpTask.new ()
task:setCallback ( taskCallback )
task:httpGet ( MOAI_CLOUD_URL .. "?" .. urlEncode ( { name = name } ))
```
