# Processes and Threads (Week 2)

## User Mode and Kernel Mode

![deepexplain](https://ars.els-cdn.com/content/image/1-s2.0-S0895717709003409-gr1.jpg)
![UK](https://docs.microsoft.com/en-us/windows-hardware/drivers/gettingstarted/images/userandkernelmode01.png)
 -  A processor in a computer running Windows has two different modes: user mode and kernel mode. The processor switches between the two modes depending on what type of code is running on the processor. Applications run in user mode, and core operating system components run in kernel mode. While many drivers run in kernel mode, some drivers may run in user mode.
When you start a user-mode application, Windows creates a process for the application. The process provides the application with a private virtual address space and a private handle table. Because an application's virtual address space is private, one application cannot alter data that belongs to another application. Each application runs in isolation, and if an application crashes, the crash is limited to that one application. Other applications and the operating system are not affected by the crash.

## Boot Sequence
![Boot2](https://www.linuxnix.com/wp-content/uploads/2013/04/Linux-Booting-process.png)
 - Power on --> 
    - First CPU runs an instruction in memory for the BIOS
        - instruction contains a jump instruction that transfers to the BIOS(UEFI Program) start-up program This program runs a power-on self test (POST) to check that devices the computer will rely on are functioning properly. Then, the BIOS goes through the configured boot sequence until it finds a device that is bootable. Once BIOS has found a bootable device, BIOS loads the bootsector and transfers execution to the boot sector.
    - If BIOS found the boot device is a hard drive, it will be a master boot record (MBR). The MBR code checks the partition table for an active partition. If one is found, the MBR code loads that partition’s boot sector and executes it. The boot sector is often operating system specific, however in most operating systems its main function is to load and execute the operating system kernel, which continues startup. If there is no active partition or the active parition’s boot sector is invalid, the MBR may load a secondary boot loader which will select a partition and load its boot boot secotr, which usually loads the corresponding operating system kernel.

## UEFI Secure Boot (VMWare Virtualization)
![UEFI](https://docs.vmware.com/en/VMware-vSphere/6.5/com.vmware.vsphere.security.doc/images/GUID-AACA1A15-B7AC-46AF-81D1-92657FF3FDB6-high.png)

## Ububtu/Linux Booting Process 
![Boot1](https://crybit.com/wp-content/uploads/2017/01/Linux-booting-process.png)

## Process Creation - System Initialization (Linux System)
![0](https://upload.wikimedia.org/wikipedia/commons/thumb/6/65/Simplified_Structure_of_the_Linux_Kernel.svg/1200px-Simplified_Structure_of_the_Linux_Kernel.svg.png)
## Process Creation - System Initialization (Windows 7)
![1](https://social.technet.microsoft.com/wiki/cfs-filesystemfile.ashx/__key/communityserver-wikis-components-files/00-00-00-00-05/1538.WindowsBootProcess.png)

## Process Creation (Cont.)
 - **Foreground Process**
    - A process which interacts with users
 - **Background process**
    - A process running without interaction with users
    - Used to handle some activity such as email, web pages, news, printing, and so on
    - They are called **daemon processes**
 - The **ps** program in UNIX can be used to list the running processes
 - In UNIX, **fork** system call is used to create a new process.
 - Fork creates **an exact clone** of the calling process.
 - A process can be created via **fork** system call
    - A creating process is called a **parent process**
    - A newly created process is called a **child process**
 - A parent process create a child process, which, in turn create other processes, **forming a tree of processes**
    - Child process is a duplicate of parent process
    - Child has a program loaded into it
 - Resource sharing between parent and child processes
    - Parent and children share all resources
    - Children share subset of parent’s resources
    - Parent and child share no resources
 - Parent and child share no resources
    - Parent and children share all resources
    - Children share subset of parent’s resources
    - Parent and child share no resources
 - **Process Creation**
![PC](http://www.cs.rpi.edu/~goldsd/docs/fall2014-csci4210/04-fork-diagram.png)
    - How to Execution
        - Parent and children execute concurrently
        - Parent waits until children terminate
    - Address space
        - Child duplicate of parent: **fork()** - *system call creates new process*
        - Child has a program loaded into it: **exec()** – *system calls loads a binary file to replace the process’s memory space and start its execution*
## Create a process via **fork system call**     

```C
int main()
{
    pid_t pid;

    /* fork another process */
    pid = fork();

    if(pid < 0) {/* error occured */
        fprintf(stderr, "Fork Failed");
        exit(-1);
        }
    else if(pid == 0 ) {/* child process */
    execlp("/bin/ls", "ls", NULL);
    }
    else { /* parent process */
    /* parent will wait for the child to complete */
    wait (NULL);
    printf("Child Complete");
    exit(0);
    }
}

```
## Example how to create process via **fork system call**
```C
int main()
{
    pid_t pid; int x = 10 ;

    /* fork another process */
    pid = fork();

    if(pid < 0) {/* error occured */
        fprintf(stderr, "Fork Failed");
        exit(-1);
        }
    x = x + 2 ; // 12
    printf(" x = %d" , x)
    else if(pid == 0 ) {/* child process */
    execlp("/bin/ls", "ls", NULL);
    print(" x again = %d", x)
    }
    else { /* parent process */
    /* parent will wait for the child to complete */
    wait (NULL);
    printf("Child Complete x = %d",x);
    exit(0);
    }
}

```

## A tree of processes as Hierarchy (Based on Solaris System)
![tree](https://s3.amazonaws.com/answer-board-image/200910161217316339129225154687502972.jpg)
 - List of process
    - sched - top process
    - pageout -memory management
    - fsflush -file system management
    - init - root process for all user processes
    - inetd - networking services
    - dtlogin - user login
    - **Windows has no concept of a process hierarchy**
    - **All processes are equal**
    - **When a process is created, the parent is given a special token , called a 
“handle”, that it can use to control the child**

## Process Termination
 - Conditions which terminate processes
    1. Normal exit (voluntary) (status = 0)
    2. Error exit (voluntary) exit(2) with error code 2
    3. Fatal error (involuntary) 
    4. Killed by another process (involuntary)
 - Examples:
    - **Error: dividing by zero (core dump)**   
    - **referencing nonexistent memory**
    - **Fatal: no such file**
    - **kill: UNIX system call "kill"**
    - **end task in Windows**

