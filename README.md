# sysfile_noti
A tool in C and inline asm to get notified about file changes in Linux system directly via telegram bot using  `inotify` API to monitor a directory.

# Features
- Monitors file creation, deletion, modification, and movements in a specified directory  .
- Sends real-time notifications to a Telegram bot.
- Configurable via a simple `config.ini` file.
- Get Notified like this:
  
 **File created: example.txt**
 
 **File modified: data.log**

 **File deleted: old_file.txt**
  
 **File moved to: new_location.txt**


#  Usuage & Compile 

  Keep your token and chat_id in `config.ini`
  
**telegram-token=  here**

**chat_id=  here**

 Specify directory to monitor . **const char *path = "/home/user";** 

lcurl is needed to compile it ..`gcc -o notify notify.c -lcurl`

#  Todo
- Integrating the with other montioring tool for monitoring all linux directory.
- Including auto run's when the system is `on`
- Implemting with shell's to ask user to run ..
