---
title: "Moai Cloud Basics Part One"
---

This tutorial will help you get started with the Moai Cloud by deploying a application to it and then creating a client to pull information from that application.

Creating your Hello World Moai Web Application
----------------------------------------------

After you log into your account, you’ll see the main Moai Cloud dashboard. Here is where you can check the deployed status of your application.

![]({{ site.baseurl }}/img/docs/Cloud1.jpg)

If this is your first time logging into the dashboard, you won’t have any applications. Thankfully, it is very easy to create a new one. Click on the “Create Application” button.

![]({{ site.baseurl }}/img/docs/Cloud2.jpg)

You’ll then be moved to the Create Application screen with a few inputs.

![]({{ site.baseurl }}/img/docs/Cloud3.jpg)

First up is your application name. Next is the version number of your application. And finally, the route that will be used to access your application and the files within. After you’ve filled those out, select “Use Moai ‘Hello World’ Template” and then click “Install Service.” Now the Hello World template is part of your Moai Cloud Applications, but it hasn’t been deployed yet, so it can’t be used.

![]({{ site.baseurl }}/img/docs/Cloud4.jpg)

To deploy it to the Moai Cloud, click on the box where the application is listed. After clicking on that, you will be sent to the app info screen.

![]({{ site.baseurl }}/img/docs/Cloud5.jpg)

You can probably figure out what to do here… go ahead and click on the “Deploy Application” button.

![]({{ site.baseurl }}/img/docs/Cloud6.jpg)

Now that your application has been deployed, you will notice that above the deployment status there is a new link.

![]({{ site.baseurl }}/img/docs/Cloud7.jpg)

Click on that link and…

![]({{ site.baseurl }}/img/docs/Cloud8.jpg)

You now have a web service that has been created and posted to its own personal URL in less than a minute.

If you click on the application’s box, you will be taken to the application information screen. From here, you can check the status of your application and you can also edit the application’s resources.

![]({{ site.baseurl }}/img/docs/Cloud9.jpg)

Yes, with the Moai Cloud service a user can actually edit code after it has been deployed. This is a convenient option for when you are still in the development stage and wanting to test a change rather quickly, but it is highly suggested to never attempt to edit any of your application once they are in production. You don’t want to accidentally ruin your application while you have hundreds of people accessing it, do you?

(Note: Please also see this tutorial on duplicating applications.)

For now, return to the dashboard. Now that you have your Hello Tutorial Moai Cloud Application up and deployed, it’s time to actually do something with it.

Creating your Hello World Client
--------------------------------

So now that you have the URL of your Moai Cloud Application, it’s time to create a client to talk to the application and take advantage of it. Below is a super simple client that will pull information from your cloud application.

```
MOAI_CLOUD_URL = “YOUR URL HERE”
    
MOAISim.openWindow ( "Cloud", 64, 64 )
    
function findString ( task )
   print ( task:getString() )
end
task = MOAIHttpTask.new ()
task:setCallback ( findString )
task:httpGet ( MOAI_CLOUD_URL )
```

First off, a note about HTTPTask. HTTPTask is Moai’s object for performing asynchronous http actions, and it requires a loop in order to function properly. The way the asynchronous fetcher works is that it starts fetching the data that is requested, then the program continues to work, and finally when the fetching is completed, it interrupts the program with the results of the fetch. That being said, the opening of the game window will simulate a game loop.

Next, you will create a function for printing of the string. After making that, create a MOAIHttpTask and then set the function to callback to the findString function after it finishes “httpGet” from the cloud URL.

Running this program so far we will get:

![]({{ site.baseurl }}/img/docs/Cloud10.jpg)

And congratulations! You have successfully created a program that pulls information from your Moai Cloud Application…

but having a program that can only display text from the cloud onto the debug screen isn’t really much of a program. With a few edits, you can alter this program so that it will instead take the string from the cloud and print it into a textbox. To do that, you’ll need to first set up the textbox and font (and also increase the size of our window). You should remember all of this from the [Textbox tutorial](http://ec2-184-72-5-146.us-west-1.compute.amazonaws.com/mediawiki/index.php/Textboxes).

```
MOAISim.openWindow ( "Textboxes", 320, 480 )
    
viewport = MOAIViewport.new ()
viewport:setScale ( 320, 480 )
viewport:setSize ( 320, 480 )
 
layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )
    
charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'
 
font = MOAIFont.new ()
font:loadFromTTF ( 'Bastarda-K.ttf', charcodes, 16, 163 )
    
textbox = MOAITextBox.new ()
textbox:setFont ( font )
textbox:setTextSize ( font:getScale ())
textbox:setRect ( -100, 100, 100, -100 )
textbox:setYFlip ( true )
```

Now that the viewport, layer, font, and textbox are ready, it’s time to insert the string from the website and insert the textbox onto the layer. This is accomplished very easily by going into the findString function and adding the following lines:

```
hello = task:getString ()
textbox:setString ( hello )
layer:insertProp ( textbox )
```

Now run the altered program.

![]({{ site.baseurl }}/img/docs/Cloud11.jpg)

You have now successfully pulled information from our Moai Cloud application and placed it into the client. You can also go back into the Moai Cloud Dashboard and edit the string so you get something different.

![]({{ site.baseurl }}/img/docs/Cloud12.jpg) ![]({{ site.baseurl }}/img/docs/Cloud13.jpg)

Save the file and relaunch your program.

![]({{ site.baseurl }}/img/docs/Cloud14.jpg)

Ta-da! Keep your eyes peeled for a few more tutorials on the Moai Cloud, including a tutorial on uploading data to the Moai Cloud.
