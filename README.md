# Minix-Container-Support
This project is based on the open-source project [Minix 3](https://github.com/Stichting-MINIX-Research-Foundation/minix). In this project, we try to implement the resource isolation mechanism and view isolation mechanism necessary for containers in the micro-kernel architecture operating system. We provide an implementation plan that is different from the traditional macro-kernel architecture operating system: out of kernel. Make full use of the structural characteristics of micro-kernel to realize these mechanisms in the user space of operating system.

## Microkernel-Based Container Resource Isolation Mechanism (MCRIM)
MCRIM makes physical resources of the system user-controllable. The CPU and memory resources that container process can use are limited to a certain range. It ensures that there will be no disorderly competition for physical resources between containers, so as to avoid mutual interference between containers.

### Description
MCRIM is composed of a pseudo file system and various resource isolation subsystems existing in the user space of operating system: 
- **Pseudo File System** acts as a medium for users to interact with each resource isolation subsystem, and forwards control information to each resource isolation subsystem.
- Combined with existing process scheduling strategy of system, **CPU Resource Isolation Subsystem** allocates time slices to processes according to CPU weight value set by users, so as to ensure the quantitative allocation of CPU resources. 
- According to the system’s memory allocation strategy in units of physical blocks, **Memory Resource Isolation Subsystem** determines whether each memory request of target process exceeds the hard limit value set by users, and then determines whether to satisfy its memory request, thereby limiting the amount of memory resources that the process can use. 
- **Freezer Process Control Subsystem** changes the status flag bit of process in the kernel process table through system calls, and controls running status of the process with the help of process scheduling mechanism in the kernel.

### Usage


