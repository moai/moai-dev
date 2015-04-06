---
title: "Moai Cloud Dashboard Plugins"
---

Dashboard Plugins
-----------------

Dashboard Plugins give you a way to create admin user interfaces for your services that can be easily accessed from within your Moai Dashboard. A dashboard plugin is essentially just html that your service provides. Moai Dashboard will access your service's dashboard.lua file and embed the results.

To enable plugins for your service simple check the 'enable plugins' checkbox while creating a new service. For services that already exist you can modify this setting on the edit service page and then upload a dashboard.lua file. Once you've enabled plugins the dashboard will automatically provide a link to your plugin.

While a plugin is just HTML there are some restrictions in regards to what you can do. Plugins are not allowed to embed or use javascript. Only inline CSS styles are allowed. Additionally to link to other pages within your plugin you need to use the `moai:` protocol. e.g. If you wanted to link to the main page of your plugin you would link to `moai:///`. If you wanted to link to a sub page then you would use `moai://page`.

Security
--------

Access to a Dashboard Plugin is limited. Users must be logged into the dashboard to access a plugin. Additionally the user accessing a plugin must either be the owner of the service that is providing the plugin, or they must be accessing the plugin with a valid client key that they own and that is associated with the plugin's service.

Limitiations
------------

Currently you can not use javascript in a dashboard plugin. Forms within a plugin may only submit back to the plugin (`action="moai://"`) Additionally you may not be able to issue redirects. We also whitelist the tags that are available for use within a plugin

```
a, abbr, acronym, address, blockquote, br, cite, code, dd, dfn, div, dl, dt, em, h1, h2, h3, h4, h5, h6, kbd, li, ol, p, pre, q, samp, span, strong, ul, var, form, input, option, optgroup, select, textarea, fieldset, legend, label, button
```

Examples: Hello World
---------------------

This is the Hello World service that is installed into your account when you create a new service with the Hello World template. This service will pull a string form your mongodb and display it. The string can be set through a dashboard plugin.

### main.lua

This is the file that powers the externally accessible service. It first attempts to load the message from mongo if it does not find one it presents the user with a default message.

```lua
function main(web, req)
  local cursor = mongodb:query("messages", '{_id:"msg"}')
  local msg = 'Please add a message in the dashboard'
  
  if (cursor:has_more()) then
    msg = cursor:next()['msg']
  end
  
  web:page(msg or 'Visit the Dashboard Plugin to define a message', 200, 'OK')
end 
```

### dashboard.lua

This is the file that provides two plugins to the dashboard. The first plugin presents the user with a form asking for a new message. The second plugin receives the submit from that form, saves the message and then provides the user with a link back to the first plugin.

```lua
function main(web, req)
  
  local path = web:path()
  
  if (string.find(path, "/addmessage$")) then
    local TEMPLATE = Tir.compile_view [[
{% raw %}    <h1>{{ title }}</h1>{% endraw %}
    
    <a href="moai:///">Update Message</a>
    ]]  

    local data = {}
    data['_id'] = 'msg'
    data['msg'] = web:params()['msg']

    mongodb:update("messages", {_id = data['_id']}, data, true, false)
     
    local page = TEMPLATE {title="Message Saved"}

    web:page(page, 200, "Ok")
  else
    local TEMPLATE = Tir.compile_view [[
{% raw %}  <h1>{{ title }}</h1>{% endraw %}

  <form id="helloworld" action="moai://addmessage" method="GET">
    Message: <input type="text" name="msg"/><br/>
    <input type="submit" value="Submit" />
  </form>
  ]]  

    local page = TEMPLATE {title="Enter Message"}

    web:page(page, 200, "Ok")
  end
end
```
