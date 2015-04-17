---
title: "Moai Hosts"
order: -850
---

Moai was designed to be platform independent. This means a unique application is required for each platform and operating system to 'host' Moai. The host application is responsible for configuring Moai's input profile and sending input events from the operating system to Moai. The host application may also inject platform and project specific extensions directly into Moai's Lua runtime.

Moai ships with reference host implementations for iOS, Android, Chrome Native Client and desktop (using GLUT, an OpenGL demo framework that runs on Windows, OSX and Linux). There are also specialized hosts contributed by the community such as Akuma and Moai IDE.
