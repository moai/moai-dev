---
title: "Using Moai Direct"
---

There are three steps to using a Moai Direct Service. First, you need to create a client key. Next, you'll need to sign up for the Moai Direct Service with that client key. And, finally, you'll need to call the Moai Direct Service from your client application.

#### Create a Client Key

In order to make use of Moai Direct Services you will first need to create a client key. A client key can be created following the 'Create Key' link in the left-hand column of <http://dashboard.moaicloud.com/admin.php>.

Creating a client key simply involves giving it a name to help you recognize it.

#### Sign up for a Moai Direct Service

Once your client key has been created you are ready to start associating services with it. Pick the service you wish to start using, assign it a DB namespace to use and then click 'Create'. The DB namespace allows you to segregate data created by this service from data you create in your own custom services. It is strongly recommended that you assign a namespace name. When choosing whether or not to assign a namespace to the service you should review the documentation for the service to determine whether or not it will be creating any collections with names that may clash with collections you create.

#### Make calls to a Moai Direct Service

After you've registered the service with your client key you can start making requests. Every request you make should include your client key as documented in [ Making Cloud Calls](makingcloudcalls.html) . This allows the service to access the DB namespace you assigned it.

Some services may also require you to sign your requests. Signing requests is also detailed at [ Making Cloud Calls](makingcloudcalls.html), and allos the service to verify that you are making the request.
