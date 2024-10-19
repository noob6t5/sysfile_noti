#include<stdio.h>
#include<string.h>
#include<sys/inotify.h>
#include<fcntl.h>
#include<curl/curl.h>
#include<stdlib.h>


// For Config data's

typedef struct 
{
    char telegram_token[128];
    char chat_id[64];
} Config;

int config_data(const char *filename,Config *config){
    FILE *file = fopen(filename, "r");
    if(file!){
        perror("Error opening Config File");
        return -1;
    }
}

// For Telegram Bot
void tele_msg(const char *message){}




// Main Fucntion
int main(void){}
