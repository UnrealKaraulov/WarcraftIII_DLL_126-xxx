
#include <io.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#if (_WIN32_WINNT <= 0x0501)
int inet_pton(int af, const char* src, void* dst)
{
    struct sockaddr_storage ss;
    int size = sizeof(ss);
    char src_copy[INET6_ADDRSTRLEN + 1];

    memset(&ss, 0, sizeof(ss));
    /* stupid non-const API */
    strncpy(src_copy, src, INET6_ADDRSTRLEN + 1);
    src_copy[INET6_ADDRSTRLEN] = 0;

    if (WSAStringToAddressA(src_copy, af, NULL, (struct sockaddr*)&ss, &size) == 0) {
        switch (af) {
        case AF_INET:
            *(struct in_addr*)dst = ((struct sockaddr_in*)&ss)->sin_addr;
            return 1;
        case AF_INET6:
            *(struct in6_addr*)dst = ((struct sockaddr_in6*)&ss)->sin6_addr;
            return 1;
        }
    }
    return 0;
}
#endif