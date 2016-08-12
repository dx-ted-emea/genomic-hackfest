# Locked-Down Desktop environment #
Sometimes it is desirable to provide access to data for analysis and inspection ,but to limit further processing of these data. That means, they shouldn't be copied using Cut/Paste into other apps, they should not be printed, copied to USB devices or to networked file systems.

Ideally one would like to have something like a full blown DRM system for such a scenario, but that's out of scope for this project. We can meet the requirements by setting up a Remote Desktop host that has disabled all of these copying features and thus provides a reasonable safe environment.

What we describe next works as well for an On-Premise Remote-Desktop server as for its host equivalent, a Windows Server with Remote-Desktop services as part of Azure RemoteApps.

In order to lock down a Windows Remote Desktop Server, you have to do the following

1. Open a command window in Administrator mode
2. type gpedit.msc

This starts the Group-Policy editor. In this tool navigate to the following

Computer Configuration -> Administrative Templates -> Windows Components -> Remote Desktop Services -> Remote Desktop Session Host

Now edit the following elements

### Disable Device redirection ###
In "Device and Resource Redirection" we lock down all devices/services that may be used to copy the data. Please do not change the smart card redirection, as that may be necessary to autheticate if the company uses smart cards with AAD-DS.

![Group Policy](http://i.imgur.com/gVsTsiA.png)

### Disable Printing ###
In "Printer Redirection" we simply block client printer redirection, that implicitly blocks all other remote printing options.

![Group Policy](http://i.imgur.com/QfGiNdt.png)

## Azure Remote Apps ##
[This video](https://azure.microsoft.com/en-us/documentation/videos/microsoft-ignite-2015-fundamentals-of-microsoft-azure-remoteapp-management-and-administration/ "Azure RemoteApps basics") very nicely summarizes the typical administrative lifecycle of Azure RemoteApps. The first 30min of the video should be sufficient to understand the principles. What you learn there is, that it's all about setting up an image of a Windows Server with Remote Desktop Services and your software pre-installed. You then generalize that image, which then will be used to elasticly bring up/down servers to provide these apps to your users via the RDP protocol. Clients exists for Windows, Android and iOS. You may even join these RDP servers to a domain you've created with AAD-DS and ideally privide a single-Sign-On (SSO) experience for your users.

Please be aware, that this are all infrastructure services in the cloud, thus it is your responsibility to keep the software on these images up-to-date.

