# Azure based Identity Managment #
For  Azure based commercial solutions we assume the use of Azure Active Directory (AAD). At its core AAD provides an environment for Identity and Access management for Web  and cloud based SaaS applications. From a developer perspective it is a provider of the well known Internet standards like e.g. OAuth 2. If you are a developer and used to these standards, AAD will be quite straightforward for you. See [this](https://azure.microsoft.com/en-us/documentation/articles/active-directory-token-and-claims/ "AAD Token Types") article to understand more details of the token types supported. We will not further dive into this basic aspect of AAD.
## Specific Scope of this Hackfest ##
This project deals with the requirement, that certain components need LDAP and/or Kerberos support for their authentication and authorization. AAD has (at the time of this writing in preview) premium feature called *Azure Active Directory Domain Services* (AAD-DS) (see [here](https://azure.microsoft.com/en-us/documentation/articles/active-directory-ds-overview/ "AAD Domain Services"). As the name indicates this is "classic" Active Directory as a service in the cloud. As such it offers LDAP and Kerberos as we need in this project. It is very likely that many enterprise scenarios in the future - when they go to the cloud - will also use AAD-DS.
### AAD-DS Setup ###
The setup and configration is not in the scope of this project as it is assumed to be provided by the customer. We just provision it in order to have a test and development environment without going into further details. [This](https://azure.microsoft.com/en-us/documentation/articles/active-directory-ds-getting-started/ "AAD-DS setup step-by-step") link provides a detailed step-by-step instruction how to setup and configure an AAD-DS environment (including public secure LDAP access over the Internet if required).

It is important to note, that AAD-DS at the moment of this writing is deployed in Azure Service Manager mode (aka "old portal") and the network associated with the domain (e.g. where the cloud based domain controllers are hosted) is a v1 network. When you've provisioned your AAD-DS (which takes a while), you'll see the IP addresse of the domain controllers (two of them are usually created) in this network. You must notice them as you'll have to use them later.

![AAD-DS DNS Configuration](http://i.imgur.com/nprL7fO.png)

### Connecting to ARM environment ###
Normally, most future resources in Azure will be created using Azure Resource Manager (ARM), e.g. Virtual Machines, Networks etc. In order to connect these machines with the AAD-DS network (remember, this is a v1 network), you have to connect these networks with a site-to-site gateway.

![v1v2 Gateway](http://i.imgur.com/juVWaWe.png)

A detailed instruction how to create such a gateway is [here](https://azure.microsoft.com/en-us/documentation/articles/virtual-networks-arm-asm-s2s-howto/ "ASM-ARM Gateway HowTo"). In this instruction is one step that may cause some headache if you don't read word-by-word what they say there. It is in Step 3, action 4:

![Step 3, Action 4](http://i.imgur.com/2ad90na.png)

As all other scripts in this article run against an ARM session, this really must be run in a separate Powershell session that is logged on to your account/subscription in ASM mode.

## Joining a Linux machine to the AAD-DS domain ##
[Here](https://azure.microsoft.com/en-us/documentation/articles/active-directory-ds-admin-guide-join-rhel-linux-vm/ "Join RHEL to AAD-DS") is an article that explains how to manually join aRedHat Enterprise Linux machine to an AAD-DS domain. What we did in this project is, we generalized and automated the approach by scripts.

First, let's note that the virtual machine you want to join should have as DNS nameservers the AAD-DS provided nameservers, otherwise the auto-discovery of the domain controllers / LDAP services is unlikely to work. You have to configure that in your VM.

![DNS for VMs](http://i.imgur.com/mh4NaOf.png)

You'll find this dialog if you look for the network interfaces you've created for your VM, and at least one of these interfaces should have these DNS servers defined (you get these IP addresses from your AAD-DS setup, see above).

Now logon to the Linux machine and copy the etc and the bin directory to some location on the machine. We assume you checked out this git repo on your work machine and use scp or similar secure file transfer tools to copy these directories to the cloud machine. These scripts have been tested for ReddHat 7.2, CentOS 7.2 and Ubuntu 16.04.

Now proceed as sollows

1. Edit file etc/az_domain and store there in a single line the name of your AAD-DS domain
2. sudo ./bin/az_install_prereqs (This installs the required software to join AD)
3. sudo ./bin/az_configure_krb5 (This discovers the domain controllers and configures Kerberos)
4. sudo realm discover YOUR_DOMAIN (replace YOUR_DOMAIN by your domain name)
5. sudo realm join YOUR_DOMAIN -U your_admin@YOUR_DOMAIN

This last step joins your machine to the AD domain. Please note, that Kerberos requires that you spell the domain in uppercase only.

You can now for example try

kinit username@YOUR_DOMAIN

and if this succeeds you can type *klist* to see the information about your Kerberos ticket granted by AAD-DS. 

What's not in the scope of this project is now how you use this Identity and Access infrastructure for your applications, e.g. how to enable for example Apache to use Kerberos for authentication, or how to authenticate users in a Database using kerberos. This is often specific to these tools, but in almost all cases well documented.

