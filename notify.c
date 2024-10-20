#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <fcntl.h>
#include <curl/curl.h>



#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))
// For Config data's
typedef struct
{
    char telegram_token[128];
    char chat_id[64];
} Config;

// Unsure input
static char *trim_newline(char *str)
{
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
    return str;
}

int config_data(const char *filename, Config *config)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening Config File");
        return -1;
    }
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '#' || line[0] == ';' || line[0] == '\n')
            continue;
        if (strncmp(line, "token=", 6) == 0)
        {
            strncpy(config->telegram_token, line + 6, sizeof(config->telegram_token) - 1);
            config->telegram_token[strcspn(config->telegram_token, "\n")] = '\0';
        }
        else if (strncmp(line, "chat_id=", 8) == 0)
        {
            strncpy(config->chat_id, line + 8, sizeof(config->chat_id) - 1);
            config->chat_id[strcspn(config->chat_id, "\n")] = '\0';
        }
    }
    fclose(file); // Close the file outside the loop
    return 0;
}

// For Telegram Bot.......
int tele_msg(const char *message, const Config *config)
{
    CURL *curl;
    CURLcode res; // return code
    char url[256];

    curl = curl_easy_init();
    if (!curl)
    {
        perror("CURL initialization failed");
        return -1;
    }

    // URL encode the message
    char *encoded_message = curl_easy_escape(curl, message, 0);
    if (!encoded_message)
    {
        perror("Error encoding the message");
        curl_easy_cleanup(curl);
        return -1;
    }

    snprintf(url, sizeof(url), "https://api.telegram.org/bot%s/sendMessage?chat_id=%s&text=%s",
             config->telegram_token, config->chat_id, encoded_message);

    //
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
        fprintf(stderr, "CURL error: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        curl_free(encoded_message); // Free the encoded message
        return -1;
    }

    curl_easy_cleanup(curl);
    curl_free(encoded_message); // Free the encoded message
    return 0;
}

/*
Event's Handling delete,update,move,create:
 https://man7.org/linux/man-pages/man7/inotify.7.html
*/
void event_handle(const struct inotify_event *event,const Config *config){
    if (event->len==0)
    return;
    char message[256] = {0};
    if(event->mask&IN_CREATE){
        snprintf(message, sizeof(message), "New File created: %s", event->name);
    }
    else if (event->mask & IN_DELETE)
    {
        snprintf(message, sizeof(message), "File deleted: %s", event->name);
    }
    else if (event->mask & IN_MODIFY)
    {
        snprintf(message, sizeof(message), "File modified: %s", event->name);
    }
    else if (event->mask & IN_MOVED_TO)
    {
        snprintf(message, sizeof(message), "File moved to: %s", event->name);
    }
    else if (event->mask & IN_MOVED_FROM)
    {
        snprintf(message, sizeof(message), "File moved from: %s", event->name);
    }

if (strlen(message) > 0)
{
    if (tele_msg(message, config) != 0)
    {
        fprintf(stderr, "Error: Failed to send message for event on file: %s\n", event->name);
    }
}
}

// Main Fucntion
int main()
{
    Config config;
    if (config_data("config.ini", &config) != 0)
    {
        fprintf(stderr, "Error: Failed to load config.\n");
        return EXIT_FAILURE;
    }

    const char *path = "/home/rishi";
    if (config_data("config.ini", &config) != 0)
    {
        fprintf(stderr, "Error: Failed to load configuration. Exiting.\n");
        return EXIT_FAILURE;
    }

int fd=inotify_init();
if (fd<0){
    perror("Error in inotify_init ....");
    return EXIT_FAILURE;
}
int wd = inotify_add_watch(fd, path, IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVE);
if (wd == -1)
{
    perror("Error: inotify_add_watch() failed");
    close(fd);
    return EXIT_FAILURE;
}
char buffer[EVENT_BUF_LEN];

while (1)
{
    int length = read(fd, buffer, EVENT_BUF_LEN);
    if (length < 0)
    {
        perror("Error: read() failed");
        break;
    }

    for (int i = 0; i < length;)
    {
        struct inotify_event *event = (struct inotify_event *)&buffer[i];
        event_handle(event, &config);
        i += EVENT_SIZE + event->len;
    }
}

inotify_rm_watch(fd, wd);
close(fd);

return EXIT_SUCCESS;
printf("Everything fine :)\n");
}
