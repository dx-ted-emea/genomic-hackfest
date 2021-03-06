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

## Azure RemoteApp ##
[This video](https://azure.microsoft.com/en-us/documentation/videos/microsoft-ignite-2015-fundamentals-of-microsoft-azure-remoteapp-management-and-administration/ "Azure RemoteApps basics") very nicely summarizes the typical administrative lifecycle of Azure RemoteApps. The first 30min of the video should be sufficient to understand the principles. What you learn there is, that it's all about setting up an image of a Windows Server with Remote Desktop Services and your software pre-installed. You then generalize that image, which then will be used to elasticly bring up/down servers to provide these apps to your users via the RDP protocol. Clients exists for Windows, Android and iOS. You may even join these RDP servers to a domain you've created with AAD-DS and ideally privide a single-Sign-On (SSO) experience for your users.

Please be aware, that this are all infrastructure services in the cloud, thus it is your responsibility to keep the software on these images up-to-date.

If it's just to provide a locked down Web browser, you may go further by looking into the mechanisms to lock down the browser itself, e.g. taking of certain menu's to prevent certain functionality. This is beyond the scope of this document, but you usually find detailed documentation on the Web how to lock down the browser of choice (e.g. take [this](http://kb.mozillazine.org/Locking_preferences "Firefox lock-down") as a starter for Firefox.).

Of course there are other areas to take into consideration for a locked down environment, not just the plain UI level copy prevention described above. Areas like for example to limit the networking reach of the remote app server, e.g. to make sure that these apps can only reach out to the configured internal network but not to the internet. This may prevent a user to use the browser session to save the page he is currently viewing and then browse to a prepared Website whith upload capabilities and then upload the saved local file. For this, conventional well known mechanisms like firewall and/or proxy configuration can be used, but this is not in the scope of this document. We just describe the prevention of copying on the UI level.


## Alternatives to Azure RemoteApp ##
On Aug 12th 2016 Microsoft announced the withdrawal of Azure RemoteApp. It will continue to serve existing customers for one more year before the service will be shutdown completely. Microsoft discusses the recommended alternatives in [this](https://blogs.technet.microsoft.com/enterprisemobility/2016/08/12/application-remoting-and-the-cloud/?WT.mc_id=azurebg_email_Trans_1164_Azure_RemoteApp_Sunset "RemoteApp sunset") article. One of the alternatives is to host Windows Server with Remote Desktop Services as an IaaS solution on Azure, a scenario in which the above proposed solution will work.
 
