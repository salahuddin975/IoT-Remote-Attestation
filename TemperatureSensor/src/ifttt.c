#include <stdio.h>
#include <curl/curl.h>
#define BUFFER_MAX 4096

 
int ifttt(char*where, char *v1, char *v2, char *v3)
{
  CURL *curl;
  CURLcode res;
  struct curl_slist *headers = NULL;
  char sbuf[BUFFER_MAX];

  sprintf(sbuf, "{ \"value1\" : \"%s\", \"value2\" : \"%s\", \"value3\" : \"%s\"}", v1, v2, v3);
 
  /* In windows, this will init the winsock stuff */ 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* get a curl handle */ 
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, where);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sbuf);
    headers = curl_slist_append(headers,"Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fopen("/dev/null", "w"));
 
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK) 
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
 
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return(res == CURLE_OK);
  }
  curl_global_cleanup();
  return 0;
}

