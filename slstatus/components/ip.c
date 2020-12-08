/* See LICENSE file for copyright and license details. */
#include <ifaddrs.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <net/if.h>
#if defined(__OpenBSD__)
	#include <sys/types.h>
	#include <sys/socket.h>
#elif defined(__FreeBSD__)
	#include <netinet/in.h>
	#include <sys/socket.h>
#endif

#include "../util.h"

static const char *
ip(const char *interface, unsigned short sa_family)
{
	struct ifaddrs *ifaddr, *ifa;
	int s;
	char host[NI_MAXHOST];

	if (getifaddrs(&ifaddr) < 0) {
		warn("getifaddrs:");
		return NULL;
	}

	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		if (!ifa->ifa_addr) {
			continue;
		}
		s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in6),
		                host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
		if (!strcmp(ifa->ifa_name, interface) &&
		    (ifa->ifa_addr->sa_family == sa_family)) {
			freeifaddrs(ifaddr);
			if (s != 0) {
				warn("getnameinfo: %s", gai_strerror(s));
				return NULL;
			}
			return bprintf("%s", host);
		}
	}

	freeifaddrs(ifaddr);

	return NULL;
}

const char *
listInterfaces() {
	struct if_nameindex *if_nidxs, *intf;
	char interfaceNames[500];
	strcpy(interfaceNames, "");
	char iNameBuffer[40];

	if_nidxs = if_nameindex();
	if ( if_nidxs != NULL )
	{
		for (intf = if_nidxs; intf->if_index != 0 || intf->if_name != NULL; intf++)
		{
				
			if(strcmp("lo", intf->if_name) != 0) 
			{
				const char *ip_address = ip(intf->if_name, AF_INET);
				if (ip_address != NULL) 
				{
					sprintf(iNameBuffer, "%s ", intf->if_name);
					strcat(interfaceNames, iNameBuffer);
				}
			}
			
		}

		if_freenameindex(if_nidxs);
		int l = strlen(interfaceNames);
		interfaceNames[l - 1] = '\0';
		return bprintf("%s", interfaceNames); 
	}
	return NULL;
}

const char *
usbUp(const char *sym)
{
	        const char *ip_address = ip("usb0", AF_INET);
		        if(ip_address == NULL) {
				                return "";
						        }
			        return sym;
}


const char *
wlanUp(const char *sym)
{
	const char *ip_address = ip("wlo1", AF_INET);
	if(ip_address == NULL) {
		return "";
	}
	return sym;
}

const char *
ipv4(const char *interface)
{
	return ip(interface, AF_INET);
}

const char *
ipv6(const char *interface)
{
	return ip(interface, AF_INET6);
}
