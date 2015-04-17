---
title: "Protecting Your Lua Files"
order: 0
---

Packaging Lua Code
------------------

There are a few different ways to approach packaging your Lua scripts for inclusion in your app. The first (and simplest) is to simply copy your Lua script folder verbatim into the app bundle, ship them in a resource folder that installs with your app (in the case of a desktop app) or store them in an archive and unpack them to writable storage.

The shortcoming with the simple method is that all your Lua scripts are available on the device in plaintext. While this is fine if you want to encourage users to mod your game, in some cases you may want to obfuscate your code to make accessing it more difficult.

It should be emphasized here that there is a big difference between obfuscation and encryption. Any savvy would-be hacker, with enough time and effort, can decompile your game code. This is true of code written in any language, so Moai does not introduce any new security vulnerabilities. The point of obfuscating your Lua code is to prevent the casual hacker from gaining easy access to your game logic.

There are several options for obfuscation:

**Running Lua files through luac:** You can compile your Lua files using luac. These files are no longer in plaintext and therefore are harder to change.

**Wrapping compiled Lua files into your executable:** After compiling all of your Lua files using luac, you can convert them to hex, dump them into header files, and compile this data in alongside your source code. This approach ensures you don't have script files sitting in resource folders, but it requires the implementation of a custom loader for the Lua chunks you've put into the header files. You won't be able to use Lua's loadfile command with this set up.

**Encrypting your compiled Lua files:** After compiling all of your Lua files using luac, you can also encrypt them. You must then hide your encryption key somewhere. The easiest approach is simply to hide the key in your app, but it's also the most vulnerable.

None of these options is truly secure. No matter which technique you use, you are still vulnerable to hacking on some level. The only thing you can do is raise the level of sophistication and time required to compromise the system. It's probably best to prevent casual hacking of plaintext files, but anything more advanced will require your own research into the issues surrounding digital rights management.
