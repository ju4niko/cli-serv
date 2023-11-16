#include <stdio.h>

/*
 * para utilizar la libcurl instalarla para ubuntu:
 *
 *   sudo apt-get install libcurl4-openssl-dev 
 *
 * y compilar con:
 *
 *   gcc -o libcurl-ej libcurl-ej.c -lcurl 
 *
 * */
#include <curl/curl.h>

int main(void) {
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:17001/medicion");
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        const char *data = "{\"val1\": 5, \"val2\": 1}";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    curl_global_cleanup();
    return 0;
}
