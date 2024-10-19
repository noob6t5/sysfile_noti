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
        perror("Error opening Config File"); // fprintf not necessary 
        return -1;         // error 
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
        fclose(file);
        return 0;
    }
}
// For Telegram Bot.......

int tele_msg(const char *message,const Config *config) {
    CURL *curl;
    CURLcode res; //return code
    char url[256];
    snprintf(url, sizeof(url), "https://api.telegram.org/bot%s/sendMessage?chat_id=%s&text=%s", config->telegram_token, config->chat_id, message);

    curl = curl_easy_init();
    if(!curl){
        perror("CURL ain't working ");
        return -1;
    }

    // http req
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
        fprintf(stderr, "CURL error: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return -1;
    }
    curl_easy_cleanup(curl);
    return 0;
}

// Main Fucntion
int main(void)
{

    printf("Everything fine :)\n");
}
