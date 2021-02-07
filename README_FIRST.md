# VirologyPlayground

## Project Vaccin

### How to set ?

#### Edit fields:


* #define SOURCE_HOST_IP "192.168.5.0"
* #define MASK "255.255.255.0"
* #define USER "dstevens"
* #define DEST_FILE "/root/vaccin"
* #define BEGINSCAN 6
* #define ENDSCAN 12

#### Compile with :

* gcc -O3 -Wall -W -Wstrict-prototypes -Werror output.c

### How it works ?

The program will try so setup a ssh tunnel with every host that is in the range [BEGINSCAN,ENDSCAN] of SOURCE_HOST_IP / MASK you provided.

Then, for each host that is "alive" it will connect through ssh with the specified **USER** , and *scp* a file (referenced with **arg[0]**) to the destination folder **DEST_FILE**.

BONUS : After each scp, it will run the script from remote host.