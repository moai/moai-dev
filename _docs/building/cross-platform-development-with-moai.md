---
title: "Cross Platform Development With Moai"
---

Setting Up for Cross Platform Development
-----------------------------------------

When structuring a project folder for cross platform development, it is recommended that you create a root folder for the project and put all of your Lua and game assets in a subfolder. For each build system or IDE you intend to use, create a separate folder alongside the Lua folder to house your platform specific IDE projects. In the root of your project folder, you can also add any scripts and utilities you need to use during development. In the Moai project sample folders, this usually means a batch file to run the Lua code using the Windows Moai host.

![]({{ site.baseurl }}/img/docs/Crossplat4.png)

If you find you are frequently updating the Moai host, you may also want to place a copy of the host binary under a local 'bin' folder in your project to ensure that you won't be subject to breaking changes. This is good practice anyway, as your project will still be able to run even after many updates to Moai.

![]({{ site.baseurl }}/img/docs/Crossplat3.png)
