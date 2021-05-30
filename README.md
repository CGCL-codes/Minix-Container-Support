# Minix-Container-Support
This project is based on the open-source project [Minix 3](https://github.com/Stichting-MINIX-Research-Foundation/minix). In this project, we try to implement the resource isolation mechanism and view isolation mechanism necessary for containers in the micro-kernel architecture operating system. We provide an implementation plan that is different from the traditional macro-kernel architecture operating system: out of kernel. Make full use of the structural characteristics of micro-kernel to realize these mechanisms in the user space of operating system.

## Microkernel-Based Container Resource Isolation Mechanism (MCRIM)
MCRIM makes physical resources of the system user-controllable. The CPU and memory resources that container process can use are limited to a certain range. It ensures that there will be no disorderly competition for physical resources between containers, so as to avoid mutual interference between containers.

### Description
MCRIM is composed of a pseudo file system and various resource isolation subsystems existing in the user space of operating system: 
- **Pseudo File System** (Cgroupfs) acts as a medium for users to interact with each resource isolation subsystem, and forwards control information to each resource isolation subsystem.
- Combined with existing process scheduling strategy of system, **CPU Resource Isolation Subsystem** allocates time slices to processes according to CPU weight value set by users, so as to ensure the quantitative allocation of CPU resources. 
- According to the system’s memory allocation strategy in units of physical blocks, **Memory Resource Isolation Subsystem** determines whether each memory request of target process exceeds the hard limit value set by users, and then determines whether to satisfy its memory request, thereby limiting the amount of memory resources that the process can use. 
- **Freezer Process Control Subsystem** changes the status flag bit of process in the kernel process table through system calls, and controls running status of the process with the help of process scheduling mechanism in the kernel.

### Usage
- Go to the main directory of source code and switch to `dev` branch. Compile whole system.
```
git checkout dev
make world
```
- Reboot the system and install Cgroupfs.
```
cd minix/fs/cgroupfs
make
make install
```
- Mount Cgroupfs
```
mount -t cgroupfs none /mnt
```
- Now we can pass control information through Cgroupfs. First, we write the cpu weight information of a certain process to the `cpu_shares` file in the `cpu` directory.
```
# In the initial state, the weight of each process is 1024 by default. The ratio of CPU occupancy rate between processes is equal to the ratio of CPU weight value. Therefore, if we want a process to have a higher CPU utilization rate, we will assign it a larger CPU weight value.
echo process_pid + ' ' + weight_value > /mnt/cpu/cpu_shares
# eg. echo '618 1024' > /mnt/cpu/cpu_shares
```
- We can also write memmory limit information of a certain process to the `vm_limit_in_bytes` file in the `memory` directory.
```
# In the initial state, memory usage of each process is limitless.
echo process_pid + ' ' + vm_limit_in_bytes > /mnt/memory/vm_limit_in_bytes
# eg. echo '618 10240' > /mnt/memory/vm_limit_in_bytes
```
- We can write process state control information of a certain process to the `freezer_state` file in the `freezer` directory.
```
# Process suspended -> FROZEN       Process wakeup -> THAWED
echo process_pid + ' ' + process_state > /mnt/freezer/freezer_state
eg. echo '618 FROZEN' > /mnt/freezer/freezer_state     echo '618 THAWED' > /mnt/freezer/freezer_state
```
