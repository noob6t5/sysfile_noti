# **sysfile_noti**

`sysfile_noti` is a tool written in C with inline assembly to monitor file changes on a Linux system and notify you through a Telegram bot. It leverages the `inotify` API to track file events such as creation, modification, deletion, and movement within a specified directory.

## **Features**
- Monitors file creation, deletion, modification, and movements in a specified directory.
- Sends real-time notifications to a Telegram bot.
- Configurable via a simple `config.ini` file.
-  notifications you will get in Bot :
  - **File created: example.txt**
  - **File modified: data.log**
  - **File deleted: old_file.txt**
  - **File moved to: new_location.txt**
![changes](https://github.com/user-attachments/assets/6fac4f8a-21e7-4015-85ec-3e2dd81bb0c1)



## **Usage & Compilation**

1. **Configuration**:
   - Create a `config.ini` file with your Telegram bot token and chat ID:
     ```ini
     token=<your-telegram-token>
     chat_id=<your-chat-id>
     ```

2. **Specify Directory**:
   - Update the `path` in the source code to define the directory you want to monitor:
     ```c
     const char *path = "/home/user";
     ```

3. **Dependencies**:
   - Install `libcurl` to handle HTTP requests:
     ```bash
     sudo apt-get install libcurl4-openssl-dev
     ```

4. **Compilation**:
   - Compile the program using `gcc` with the `-lcurl` flag:
     ```bash
     gcc -o notify notify.c -lcurl
     ```

5. **Run**:
   - Start monitoring the specified directory:
     ```bash
     ./notify
     ```

## **Add more features to suit your infrastructure**
- Add functionality to monitor all directories in the Linux filesystem.
- Enable auto-run when the system starts.
- Integrate shell scripts for user interaction before execution.
