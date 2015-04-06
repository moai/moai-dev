---
title: "Deploying Code"
---

Deploying Code with Git
-----------------------

**This is currently in private beta.**

**As with any beta software you may encounter bugs, including data loss, and should be cautious if using it in a production environment**

Moai Cloud allows you to deploy your code using the Git push command. Once you've provided us with a public key you'll be able to deploy code to Moai Cloud by adding a manifest.json file to the root of your repository and issuing a Git push command.

### Git command format

In order to push to the cloud you'll want to add an additional remote location to your current Git setup.

To add a remote that pushes to a single specific service use the following command:

```
 git remote add moaicloud git@sol.moaicloud.com:`<username>`/`<route>`.git
```

To add a remote that pushes code to all of your services at once:

```
 git remote add moaicloud git@sol.moaicloud.com:`<username>`.git
```

You can add multiple remotes to a single git repository so you should be able to flexibly define multiple ways to deploy your code.

**Example:** Adding user test-user's service echo

```
 git remote add echo git@sol.moaicloud.com:test-user/echo.git
```

**Example:** Adding all of test-user's services

```
 git remote add moaicloud git@sol.moaicloud.com:test-user.git
```

Once you've added your remote location you can deploy committed code with:

```
 git push echo master
```

or

```
 git push moaicloud master
```

We only accept pushes to the master branch.

### Manifest File

The manifest file (/manifest.json) defines which code to deploy where.

Manifest Format:

```
 [
   {
     route : "service",
     code_folder : "service/",
     lib_folder : "utils/"
   }
 ]
```

The route should match the route in the dashboard.

All files in 'code\_folder' are copied to the root of your code folder. The same is true for lib\_folder. So if your repository looked like this:

```
 service/
        /main.lua
        /folder/file.lua
 utils/split.lua
      /web.lua
```

Then your code folder would look like this in the dashboard:

```
 main.lua
 folder/file.lua
 split.lua
 web.lua
```

If there is a conflict with file names we will copy the second file over the first. lib\_folder is optional.
