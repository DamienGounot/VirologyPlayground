//compilation: gcc -O3 -Wall -W -Wstrict-prototypes -Werror vaccin_nom_prenom.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define SOURCE_HOST_IP "192.168.5.0"
#define MASK "255.255.255.0"
#define SSH "/usr/bin/ssh -qq"
#define SCP "/usr/bin/scp -q -p"
#define DEST_FILE "/home/dstevens/vaccin"
#define port 22

#define beginScanRange 6
#define endScanRange 12

#define hostListSize (endScanRange-beginScanRange+1)

static int nbHost;

void initList(int* hostList, int listSize)
{
    for(int i = 0; i < listSize; i++)
    {
        hostList[i]=0;
    }
}



char* createIP(char* host_ip, int lastDigit)
{
    int dotNumber = 0;
    char* IP = malloc (sizeof(char)*20);
    char* tmp = malloc (sizeof(char)*2);
    char strDigit[5];
    //printf("Source Host: <%s>\n",host_ip);
    //printf("Last digit: <%d>\n",lastDigit);
    int len = strlen(host_ip);
    for (int i=0; i < len; i++)
    {
        tmp[0] = host_ip[i];
        strcat(IP,tmp);
        if(host_ip[i] == 46) // 46 is dot
        {
            dotNumber ++;
            if(dotNumber == 3)
            {
                break;
            }
        }
    }

    sprintf(strDigit, "%d", lastDigit);
    strcat(IP, strDigit);
    return IP;
}

int * scanNetwork(void)
{
    struct in_addr mask, sourceIP,hostIP;
    struct sockaddr_in sockHostIP;
    unsigned long int hostMask =0,i;
    int sock,so_error;
    fd_set fdset;
    struct timeval tv;
    static int localHostList [hostListSize];
    nbHost = 0;

    initList(localHostList,hostListSize);
    
    inet_aton(SOURCE_HOST_IP,&sourceIP);
    inet_aton(MASK,&mask);
    hostMask = sourceIP.s_addr & mask.s_addr;

    printf("[DEBUG] Begin Network %s Scan between [%d,%d] on port: %d\n",SOURCE_HOST_IP,beginScanRange,endScanRange,port);

    for(i = beginScanRange; i < endScanRange; i++)
    {
        
        hostIP.s_addr = htonl(ntohl(hostMask)+i);
        printf("[DEBUG] Test on: %s:%d\t",inet_ntoa(hostIP),port);
        fflush(stdout);
        bzero(&sockHostIP, sizeof(sockHostIP));
        sockHostIP.sin_addr = hostIP;
        sockHostIP.sin_port = htons(port);
        sockHostIP.sin_family = AF_INET;
        sock = socket(AF_INET,SOCK_STREAM,0);
        fcntl(sock,F_SETFL,O_NONBLOCK);
        connect(sock,(struct sockaddr*)&sockHostIP,sizeof(sockHostIP));
        FD_ZERO(&fdset);
        FD_SET(sock,&fdset);
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        if(select(sock+1,NULL,&fdset,NULL,&tv) == 1)
        {
            socklen_t len = sizeof so_error;
            getsockopt(sock,SOL_SOCKET,SO_ERROR,&so_error,&len);
            if(so_error == 0){
                printf("Alive\n");
                localHostList[nbHost] = i;
                nbHost++;
            }
        }else
        {
            printf("Dead\n");
        }
        close(sock);
        
    }
    return localHostList;
}

void colonize(int *list, char src[])
{
    char cmd[256];
    char tmp[64];

    for(int i = 0; i < nbHost ; i++)
    {   
        char* actualIP = createIP(SOURCE_HOST_IP,list[i]);
        sprintf(cmd,SCP);
        sprintf(tmp, " %s dstevens@%s:", src,actualIP);
        strcat(cmd,tmp);
        strcat(cmd,DEST_FILE);
        printf("%s\n", cmd);
        //system(cmd);
    }
    
}

int main(int argc, char *argv[])
{
    if(argc) printf("### %s\n", argv[0]);

    int *hostList = (int*)malloc(hostListSize*sizeof(int));

    hostList = scanNetwork();

    colonize(hostList, argv[0]);
    
    return EXIT_SUCCESS;
}